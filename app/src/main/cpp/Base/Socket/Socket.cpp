//
// Created by octopus on 6/5/25.
//

#include "Socket.h"

std::shared_ptr<rtc::WebSocket> Socket::webSocket = nullptr;
int Socket::reconnectAttempts = 0;
std::atomic<bool> Socket::isReconnecting(false);
std::mutex Socket::reconnectMutex;
std::thread Socket::reconnectThread;
std::atomic<bool> Socket::isPingThreadRunning(false);
std::thread Socket::pingThread;
std::chrono::time_point<std::chrono::steady_clock> Socket::lastPongReceived;
bool Socket::lastPong = false;

void Socket::connectSocket() {
    reconnectAttempts++;

    if (reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
        Common::NLog("Connect failed max attempts", "Socket::connectSocket", LogLevel::ERROR);
        ASService::disableSelf("Connect failed max attempts", true);
        return;
    }

    Base::updateLine(
            "Connect " + to_string(reconnectAttempts) + "/" + to_string(MAX_RECONNECT_ATTEMPTS),
            true);

    while (!Device::isNetworkAvailable()) {
        updateLine("Network not available", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    rtc::WebSocketConfiguration configuration;
    configuration.connectionTimeout = std::chrono::milliseconds(60000);
    configuration.pingInterval = std::chrono::milliseconds(0);
    configuration.maxMessageSize = 1024 * 1024;


    configuration.caCertificatePemFile = "/data/user/0/" PACKAGE "/files/cacert.pem";

    webSocket = std::make_shared<rtc::WebSocket>(configuration);

    webSocket->onOpen([]() {
        onOpen();
    });

    webSocket->onClosed([]() {
        Common::NLog("WebSocket connection closed", "Socket::onClosed", LogLevel::ERROR);
        stopPingThread();
        attemptReconnect();
    });

    webSocket->onError([](const string &error) {
        Common::NLog("WebSocket connection onError: " + error, "Socket::onError", LogLevel::ERROR);
        stopPingThread();
        attemptReconnect();
    });

    webSocket->onMessage([&](rtc::variant<rtc::binary, string> data) {
        if (!std::holds_alternative<string>(data))
            return;

        onMessage(std::get<string>(data));
    });

    string url = "";
    if (Common::strContains(BASE_URL, "api.maxcloudphone.com")) {
        url = "wss://socket.maxcloudphone.com/deviceRHub?access_Token=" + Base::jwt;
    } else {
        string endpoint = Common::strReplace(BASE_URL, "https://", "");
        url = "wss://" + endpoint + "deviceRHub?access_Token=" + Base::jwt;
    }

    Common::NLog(url, "connect ENDPOINT");

    webSocket->open(url);
}

void Socket::onOpen() {
    Base::updateLine("Connect Success!!!", true);
    Common::NLog("Connect Success!!!", "Socket::onOpen");

    reconnectAttempts = 0;

    string setupMessage = JSON()
            .set("protocol", "json")
            .set("version", 1).toString();

    webSocket->send(setupMessage + static_cast<char>(0x1E));

    sendPing();

    // Start ping thread to send ping every minute
    startPingThread();
}


void Socket::onMessage(string message) {
    try {
        if (message.empty()) return;

        if (message.back() == static_cast<char>(0x1E)) {
            message.pop_back();
        }

        if (message == "{}") return;

        if (!JSON::isJSON(message)) {
            Common::NLog("message is not JSON", "Socket::onMessage", LogLevel::ERROR);
            return;
        }

        JSON jsonSignal(message);

        if (jsonSignal.getInt("type") == 6) {
            bool isPing = sendPing();

            string logPing = isPing ? "Success" : "Failed";

            Common::NLog("Retrive Ping from server, ping send : " + logPing, "Socket::onMessage");

            return;
        }

        Common::NLog(jsonSignal.toString(), "Socket::onMessage", LogLevel::DEBUG);

        string target = jsonSignal.getString("target");

        if (target == "MESSAGE") {
            JSON jsonMessage{jsonSignal.getString("arguments.0")};

            string type = jsonMessage.getString("type");
            string clientId = jsonMessage.getString("clientId");

            if (type == "PONG") {
                lastPongReceived = std::chrono::steady_clock::now();

                lastPong = jsonMessage.getBool("pong");
            }

            if (type == "START_STREAM") {
                Device::unlockScreen();

                if (!Media::isStreaming()) {
                    if (!Media::startStreamVideo()) return;

                    Media::startRotationDetector();
                }

                RTC::addRTCInstance(clientId, jsonMessage.getList("data"));
            }

            if (type == "STOP_STREAM") {
                RTC::removeRTCInstance(clientId);

                if (RTC::isEmptyStream())
                    Media::cleanUp();
            }

            if (type == "TRANSFER_SDP")
                RTC::sendSignalToRTCInstance(clientId, jsonMessage.getString("data"));
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Socket::onMessage", LogLevel::ERROR);
    }
}


bool Socket::sendMessage(string target, vector<string> arguments) {
    try {
        nlohmann::json messageJSON;

        messageJSON["type"] = 1;
        messageJSON["target"] = target;
        messageJSON["arguments"] = arguments;

        string messageSend = messageJSON.dump() + static_cast<char>(0x1E);

        webSocket->send(messageSend);

        return true;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Socket::sendMessage", LogLevel::ERROR);
        return false;
    }

}


bool Socket::sendPing() {
    if (!webSocket->isOpen()) {
        Common::NLog("Cannot send ping, webSocket is not open", "Socket::sendPing",
                     LogLevel::ERROR);
        return false;
    }

    try {
        string pingMessage = JSON()
                .set("type", 6)
                .toString();

        webSocket->send(pingMessage + static_cast<char>(0x1E));

        return true;
    } catch (const std::exception &e) {
        Common::NLog(std::string("Error sending ping: ") + e.what(), "Socket::sendPing",
                     LogLevel::ERROR);
    }

    return false;
}

void Socket::attemptReconnect() {
    std::lock_guard<std::mutex> lock(reconnectMutex);

    if (isReconnecting.load()) {
        return;
    }

    if (reconnectThread.joinable()) {
        reconnectThread.join();
    }

    isReconnecting.store(true);

    reconnectThread = std::thread([]() {
        Common::NLog("Starting reconnection thread", "Socket::attemptReconnect", LogLevel::DEBUG);

        for (int loop = RECONNECT_DELAY_SECONDS; loop > 0; loop--) {
            Base::updateLine("Reconnecting in " + to_string(loop) + " seconds", true);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        connectSocket();

        isReconnecting.store(false);
    });

    reconnectThread.detach();
}

void Socket::startPingThread() {
    if (!Common::strContains(BASE_URL, "api.maxcloudphone.com")) return;


    stopPingThread();

    isPingThreadRunning.store(true);

    pingThread = std::thread([]() {
        Common::NLog("Starting ping thread", "Socket::startPingThread", LogLevel::DEBUG);

        while (isPingThreadRunning.load()) {
            try {
                for (int i = 0; i < PING_PONG_DELAY_SECONDS && isPingThreadRunning.load(); i++) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }

                if (!isPingThreadRunning.load()) continue;

                if (webSocket == nullptr || !webSocket->isOpen()) {
                    webSocket->close();
                    Common::NLog("WebSocket not available, closing connection",
                                 "Socket::pingThread",
                                 LogLevel::ERROR);
                    continue;
                }

                Common::NLog("Ping thread running", "Socket::pingThread", LogLevel::DEBUG);

                lastPong = false;

                sendMessage("PingPong");

                for (int loop = 10; loop > 0; loop--) {
                    if (lastPong) break;
                    delay(1);
                }

                auto timeout = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now() - lastPongReceived).count();

                if (!lastPong || timeout > PING_PONG_TIMEOUT) {
                    webSocket->close();
                    Common::NLog("lastPong is false, closing connection",
                                 "Socket::pingThread", LogLevel::ERROR);
                    continue;
                }

                Common::NLog("Ping thread stopped", "Socket::startPingThread", LogLevel::DEBUG);
            } catch (const std::exception &e) {
                Common::NLog(e.what(), "Socket::startPingThread",
                             LogLevel::ERROR);
            }
        }

    });

    pingThread.detach();
}

void Socket::stopPingThread() {
    if (isPingThreadRunning.load()) {
        Common::NLog("Stopping ping thread", "Socket::stopPingThread", LogLevel::DEBUG);
        isPingThreadRunning.store(false);

        // Wait a bit for thread to finish
        if (pingThread.joinable()) {
            pingThread.join();
        }
    }
}


jobject Socket::firebaseObject = nullptr;


void Socket::initFirebase() {
    JNIEnv *env = JExt::getEnv();

    jclass classFireB = JExt::findClass("com/google/firebase/database/FirebaseDatabase");
    jmethodID getInstanceMethod = JExt::findMethod(classFireB, "getInstance",
                                                   "(Ljava/lang/String;)Lcom/google/firebase/database/FirebaseDatabase;",
                                                   true);

    string decrypt = cryptor::decrypt(
            "UDJHO0ZUdx1UFSICPV43FCkaOFkvYlcwDVVoHicaTC5WKkYaaRxfHSgEM1AxFT0TI1YoLUUmSlU1HWc=");

    jstring path = JExt::stringToJstring(decrypt);


    jobject instance = env->CallStaticObjectMethod(classFireB, getInstanceMethod, path);

    // Giải phóng jstring path
    env->DeleteLocalRef(path);

    jmethodID getReferenceMethod = JExt::findMethod(classFireB, "getReference",
                                                    "()Lcom/google/firebase/database/DatabaseReference;");
    jobject dbReference = env->CallObjectMethod(instance, getReferenceMethod);

    // Giải phóng instance object
    env->DeleteLocalRef(instance);

    jclass classFireB2 = JExt::findClass("com/google/firebase/database/DatabaseReference");
    jmethodID childMethod = JExt::findMethod(classFireB2, "child",
                                             "(Ljava/lang/String;)Lcom/google/firebase/database/DatabaseReference;");

    jobject jContext = Base::getContext();

    jobject appPref = Storage::newPref(jContext, "Config");

    string token = Storage::getPrefValue(appPref, "token");

    string payload = Common::parseJWTPayload(token);

    if (payload.empty()) {
        while (true) {
            Base::updateLine("Invalid JWT Token, Please login and try again", true);
            ASService::delay(5);
        }
    }

    JSON jsonToken(payload);

    string deviceId = jsonToken.getString("DeviceId");

    jstring jdeviceId = JExt::stringToJstring(deviceId);

    dbReference = env->CallObjectMethod(dbReference, childMethod, jdeviceId);

    if (dbReference == nullptr) {
        Common::NLog("dbReference is null", "SignalR::initFirebase", LogLevel::ERROR);
        return;
    }

    env->DeleteLocalRef(jdeviceId);

    jfieldID dbReferenceField = env->GetStaticFieldID(Base::baseVM.jClass,
                                                      "dbReference",
                                                      "Lcom/google/firebase/database/DatabaseReference;");

    env->SetStaticObjectField(Base::baseVM.jClass, dbReferenceField, dbReference);

    firebaseObject = env->NewGlobalRef(dbReference);

    jmethodID initDatabaseMethod = JExt::findMethod(
            Base::baseVM.jClass, "initSocket",
            "()V", true);
    env->CallStaticVoidMethod(Base::baseVM.jClass, initDatabaseMethod);
}

jobject Socket::getFirebase() {
    return firebaseObject;
}

void Socket::updateFirebase(const string &key, const string &value) {
    JNIEnv *env = JExt::getEnv();

    jobject dbReference = getFirebase();

    jstring jKey = JExt::stringToJstring(key);
    jstring jValue = JExt::stringToJstring(value);

    env->CallStaticVoidMethod(dialogVM.dialogClass, dialogVM.updateDbMethod,
                              dbReference, jKey, jValue);

    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(jValue);
}

void Socket::forceReconnect() {
    try {
        webSocket->close();
    }
    catch (std::exception e) {
        Common::NLog(e.what(), "Ex: forceReconnect");
    }
}