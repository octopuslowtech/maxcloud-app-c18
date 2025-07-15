//
// Created by Admin on 1/3/2025.
//

#include "RTC.h"

std::vector<RTC::RTCThreadEntry> RTC::lstRTCInstance;
std::mutex RTC::mutexRTC;

template<class T>
rtc::weak_ptr<T> make_weak_ptr(rtc::shared_ptr<T> ptr) { return ptr; }

void RTC::addRTCInstance(string clientId, vector<string> lstICE, bool isMedia) {
    try {
        std::lock_guard<std::mutex> lock(mutexRTC);

        auto it = std::find_if(lstRTCInstance.begin(), lstRTCInstance.end(),
                               [&clientId](const RTCThreadEntry &entry) {
                                   return entry.clientId == clientId;
                               });

        if (it != lstRTCInstance.end()) {

            it->instance->onClose();

            if (it->thread.joinable()) {
                it->thread.join();
            }

            lstRTCInstance.erase(it);

            Common::NLog("Stream already exists for clientId: " + clientId, "RTC");
        }

        auto rtcInstance = std::make_shared<RTC>(clientId, lstICE);
//        rtcInstance->isMediaTransport = isMedia;
        std::thread rtcThread([rtcInstance]() {
            rtcInstance->startProcessing();
        });

        lstRTCInstance.push_back(RTCThreadEntry{clientId, rtcInstance, std::move(rtcThread)});
    } catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::addRTCInstance", LogLevel::ERROR);
    }
}

void RTC::removeRTCInstance(string clientId) {
    try {
        std::lock_guard<std::mutex> lock(mutexRTC);

        auto it = std::find_if(lstRTCInstance.begin(), lstRTCInstance.end(),
                               [&clientId](const RTCThreadEntry &entry) {
                                   return entry.clientId == clientId;
                               });

        if (it == lstRTCInstance.end()) return;
        it->instance->onClose();

        if (it->thread.joinable()) {
            it->thread.join();
        }

        lstRTCInstance.erase(it);

        Common::NLog("Stopping stream for clientId: " + clientId, "RTC");
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::removeRTCInstance", LogLevel::ERROR);
    }
}

bool RTC::isEmpty() {
    try {
        std::lock_guard<std::mutex> lock(mutexRTC);
        return lstRTCInstance.empty();
    } catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::isEmpty", LogLevel::ERROR);
    }
}

bool RTC::isEmptyStream() {
    try {
        std::lock_guard<std::mutex> lock(mutexRTC);
        return std::none_of(lstRTCInstance.begin(), lstRTCInstance.end(),
                            [](const RTCThreadEntry &entry) {
                                return entry.instance->isMediaTransport;
                            });

    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::isEmptyStream", LogLevel::ERROR);
        return true;
    }
}

void RTC::sendSignalToRTCInstance(string clientId, string data) {
    try {

        auto it = std::find_if(lstRTCInstance.begin(), lstRTCInstance.end(),
                               [&clientId](const RTCThreadEntry &entry) {
                                   return entry.clientId == clientId;
                               });

        if (it == lstRTCInstance.end()) {
            Common::NLog("No active stream found for clientId: " + clientId, "RTC");
            return;
        }

        it->instance->onTransferSDP(data);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::sendSignalToRTCInstance", LogLevel::ERROR);
    }
}

std::vector<std::shared_ptr<RTC>> RTC::getRTCInstances() {
    try {
        std::lock_guard<std::mutex> lock(mutexRTC);

        std::vector<std::shared_ptr<RTC>> filteredInstances;
        filteredInstances.reserve(lstRTCInstance.size());

        std::transform(lstRTCInstance.begin(), lstRTCInstance.end(),
                       std::back_inserter(filteredInstances), [](const RTCThreadEntry &entry) {
                    return entry.instance;
                });

        return filteredInstances;
    } catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::getRTCInstances", LogLevel::ERROR);
        return {};
    }
}

RTC::RTC(std::string
         _clientId, vector<string>
         _lstICE) : clientId(std::move(_clientId)),
                    lstICE(std::move(_lstICE)) {
    try {
        rtc::InitLogger(rtc::LogLevel::Debug, log_callback);

        rtc::Configuration config;

        for (auto ice: lstICE) {
            vector<string> iceSplit = Common::strSplit(ice, "|");
            if (iceSplit.empty() || !Common::strIsNumber(iceSplit[1])) continue;

            config.iceServers.emplace_back(iceSplit[0], Common::stringToUint16(iceSplit[1]),
                                           iceSplit[2], iceSplit[3]);
        }


        peerConnection = std::make_shared<rtc::PeerConnection>(config);

        peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
            if (state == rtc::PeerConnection::State::Connected) {
                std::thread([]() {
                    for (int loop = 10; loop > 0; loop--) {
                        bool isRunning = Base::isRunning;
                        Base::updateColorStatus(!isRunning);
                        Base::delay2(65);
                        Base::updateColorStatus(isRunning);
                        Base::delay2(500);
                    }
                }).detach();
            }
        });

        peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
            json message = {{"type",      "candidate"},
                            {"candidate", std::string(candidate)},
                            {"sdpMid",    candidate.mid()}};

            Socket::sendMessage("SendToClient", {clientId, "TRANSFER_SDP", message.dump()});
        });

        peerConnection->onLocalDescription([this](rtc::Description description) {
            JSON jsonTranfer;
            jsonTranfer.set("type", description.typeString());
            jsonTranfer.set("sdp", std::string(description));

            if (description.typeString() == "offer") {
                jsonTranfer.set("ice", lstICE);
            }
            Socket::sendMessage("SendToClient", {clientId, "TRANSFER_SDP", jsonTranfer.toString()});

        });

        setupVideoTrack();

        setupFileChannel();

        setupControlChannel();

        JSON jsonDeviceInfo;

        std::tuple<int, int, bool> screen = Device::getScreenSize();

        jsonDeviceInfo.set("width", std::get<0>(screen));
        jsonDeviceInfo.set("height", std::get<1>(screen));

        Socket::sendMessage("SendToClient", {clientId, "TRANSFER_INFO", jsonDeviceInfo.toString()});
    } catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::Core", LogLevel::ERROR);
        removeRTCInstance(clientId);
    }
}

void RTC::setupVideoTrack() {
    if (!isMediaTransport) return;
    const uint32_t ssrc = 1;
    const string cname = "video-stream";
    const uint8_t payloadType = 102;
    const string msid = "stream1";
    try {
        rtc::Description::Video video(cname, rtc::Description::Direction::SendOnly);

        video.addH264Codec(payloadType);

        video.addSSRC(ssrc, cname, msid, cname);

        videoTrack = peerConnection->addTrack(video);

        auto rtpConfig = std::make_shared<rtc::RtpPacketizationConfig>(ssrc, cname, payloadType,
                                                                       rtc::H264RtpPacketizer::defaultClockRate);

        auto packetizer = std::make_shared<rtc::H264RtpPacketizer>(
                rtc::NalUnit::Separator::ShortStartSequence, rtpConfig);


        srReporter = std::make_shared<rtc::RtcpSrReporter>(rtpConfig);

        packetizer->addToChain(srReporter);

        auto nackResponder = std::make_shared<rtc::RtcpNackResponder>();

        packetizer->addToChain(nackResponder);

        videoTrack->setMediaHandler(packetizer);


//        videoTrack->onOpen([this]() {
//            Common::NLog("videoTrack onOpen", "OCTOPUSLOWTECH");
//        });
//
//        videoTrack->onClosed([this]() {
//            Common::NLog("videoTrack onClosed", "OCTOPUSLOWTECH");
//        });
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::setupVideoTrack", LogLevel::ERROR);
    }
}

void RTC::setupControlChannel() {
    try {
        controlChannel = peerConnection->createDataChannel("controlChanel");

        controlChannel->onMessage([this](auto data) {
            std::lock_guard<std::mutex> lock(controlMutex);
            try {
                string action = std::get<std::string>(data);
                if (action.empty()) {
                    Common::NLog("action is empty", "controlChannel::onMessage", LogLevel::ERROR);
                    return;
                }

                Common::NLog(action, "setupControlChannel::onMessage");

                JSON json(action);

                ControlType controlType = static_cast<ControlType>(json.getInt("type"));

                // core
                if (controlType == ControlType::EXECUTE_SINGLE_NODE) {
                    OWrapper::executeWithWrapper(json.getString("node"));
                }

                if (controlType == ControlType::MOUSE_MOVE ||
                    controlType == ControlType::MOUSE_DOWN ||
                    controlType == ControlType::MOUSE_UP) {
                    ASService::mouseMove(json.getInt("x"), json.getInt("y"), json.getInt("type"));
                }

                if (controlType == ControlType::INPUT_TEXT) {
                    //            if (ASService::lastEditTextFocus == nullptr) return;
                    //            Node nodeEditText;
                    //            nodeEditText.nodeInfo = ASService::lastEditTextFocus;

                    string input = json.getString("msg");

                    if (input == "WakeUp") {
                        return;
                    }
                    if (input == "[BACKSPACE]") {
                        ASService::handleKeyboard(Keyboard::MATCHA);
                        //                if(!ASService::lastEditTextFocusContent.empty())
                        //                    ASService::lastEditTextFocusContent.pop_back();
                        //                ASService::inputText(nodeEditText, ASService::lastEditTextFocusContent);
                    } else if (input == "[ENTER]")
                        ASService::handleKeyboard(Keyboard::ENTER);
                    else if (input == "[CTRL_C]") {
                        ASService::handleKeyboard(Keyboard::COPY_TEXT);
                        Base::delay2(200);
                        string clipboard = Device::getClipboard();
                        sendToChannel(ReportControlType::EXPORT_CLIPBOARD, clipboard);
                    } else if (input == "[CTRL_A]")
                        ASService::handleKeyboard(Keyboard::SELECT_ALL);

                    else if (input == "[ARROWRIGHT]")
                        ASService::handleKeyboard(Keyboard::ARROW_RIGHT);
                    else if (input == "[ARROWLEFT]")
                        ASService::handleKeyboard(Keyboard::ARROW_LEFT);
                    else {
                        ASService::handleKeyboard(Keyboard::INPUT, input);
                        //                ASService::lastEditTextFocusContent += input;
                        //                ASService::inputText(nodeEditText, ASService::lastEditTextFocusContent);
                    }
                }

                if (controlType == ControlType::BACK) {
                    ASService::performAction(Perform::BACK);
                }
                if (controlType == ControlType::RESET_FACTORY) {
                    ASAction::factoryResetAction();
                }

                if (controlType == ControlType::HOME) {
                    ASService::performAction(Perform::HOME);
                }
                if (controlType == ControlType::UNLOCK_SCREEN) {
                    Device::unlockScreen();
                }

                if (controlType == ControlType::RECENT) {
                    ASService::performAction(Perform::RECENT);
                }

                if (controlType == ControlType::CONNECT_PROXY) {
                    if (Base::isRunning && !Base::isStop) {
                        Base::showToast("Please stop current action and try again");
                        return;
                    }

                    string proxyStr = json.getString("msg");

                    if (proxyStr.empty()) {
                        Base::showToast("Notfound Proxy");
                        return;
                    }

                    ASService::appSetting.proxyType = ProxyType::PROXY;
                    ASService::appSetting.proxyStr = proxyStr;

                    ASService::appSetting.proxy.proxyIP = "";
                    ASService::appSetting.proxy.proxyPort = "";
                    ASService::appSetting.proxy.proxyUser = "";
                    ASService::appSetting.proxy.proxyPass = "";

                    ASAction::setupNetwork();
//
//                    if (ADB::isConnected) {
//                        ADB::runShell("am start -a android.intent.action.VIEW -d https://ip8.com");
//                    }
                }

                if (controlType == ControlType::DISCONNECT_PROXY) {
                    ASService::sendEventToHelper(ASService::HelperAction::STOP_VPN, {});
                    Base::showToast("Disconnect Proxy");
                }

                if (controlType == ControlType::SCREEN_SHOT) {
                    auto [success, base64String] = Media::screenShot(Media::OutputType::BASE64, 99);
                    if (!success) {
                        Base::showToast("Failed to take screenshot");
                        return;
                    }
                    Base::showToast("Screenshot taken");

                    sendToChannel(ReportControlType::EXPORT_SCREEN_SHOT, base64String);
                }

                if (controlType == ControlType::REBOOT) {
                    Base::showToast("Rebooting device");
                    ADB::runShell("adb reboot");
                }

                if (controlType == ControlType::GET_XML ||
                    controlType == ControlType::BUSINESS_DUMP_XML) {
                    auto [success, base64String] = Media::screenShot(Media::OutputType::BASE64, 99);
                    if (!success) {
                        Base::showToast("Failed to take screenshot");
                        return;
                    }
                    JSON jsonBody;
                    string xml = ASService::dumpXml();
                    jsonBody.set("xml", xml);

                    Common::NLog(xml, "DUMPXML");
                    jsonBody.set("activity", ASService::currentActivity);
                    jsonBody.set("package", ASService::currentPackage);
                    jsonBody.set("image", base64String);

                    string messageJson = jsonBody.toString();

                    sendToChannel(ReportControlType::EXPORT_XML, messageJson);
                    Base::showToast("Export Success");
                }

                if (controlType == ControlType::GET_CLIPBOARD) {
                    ASService::handleKeyboard(Keyboard::COPY_TEXT);
                    Base::delay2(200);
                    string clipboard = Device::getClipboard();
                    sendToChannel(ReportControlType::EXPORT_CLIPBOARD, clipboard);
                }

                if (controlType == ControlType::GET_INSTALLED_PACKAGE) {
                    string packageJson = Device::getPackagesInstalledWithIcon();

                    if (!packageJson.empty())
                        sendToChannel(ReportControlType::EXPORT_INSTALLED_PACKAGE, packageJson);

                }

                if (controlType == ControlType::OPEN_PACKAGE) {
                    Device::openApp(json.getString("packageSpec"));
                }

                if (controlType == ControlType::OPEN_APP_INFO) {
                    Device::openPackageInfo(json.getString("packageSpec"));
                }

                if (controlType == ControlType::FORCE_STOP) {
                    bool result = ASAction::closeAppAction(json.getString("packageSpec"));

                    Base::showToast("Force stop " + json.getString("packageSpec") +
                                    (result ? " success" : " failed"));
                }

                if (controlType == ControlType::CLEAR_APP) {
                    bool result = ASAction::clearDataAppAction(json.getString("packageSpec"));

                    Base::showToast("Clear app " + json.getString("packageSpec") +
                                    (result ? " success" : " failed"));
                }

                if (controlType == ControlType::UNINSTALL_APP) {
                    ADB::runShell("pm uninstall " + json.getString("packageSpec"));
                }

                if (controlType == ControlType::CHANGE_DEVICE) {
                    if (Device::isROMModified) {
                        ASAction::changeDeviceByROM();

                        Base::delay(2);

                        Base::updateLine("Changing Failed", true);

                        return;
                    }

                    if (ADB::isRoot) {
                        Base::updateLine("Change Device");

                        string package = "ru.andr7e.deviceinfohw";
                        ASAction::closeAppAction(package);

                        bool isSuccess = ASAction::changeDevicebyLSPosed();

                        if (!isSuccess) {
                            Base::updateLine("Change Device Fail", true);
                            return;
                        }

                        Device::openApp(package);

                        Base::updateLine("Change Device Success", true);

                        return;
                    }

                    Base::updateLine("Device not rooted", true);
                }

                if (controlType == ControlType::RERENDER_FRAME) {
                    for (int loop = 3; loop > 0; loop--) {
                        bool isRunning = Base::isRunning;
                        Base::updateColorStatus(!isRunning);
                        Base::delay2(65);
                        Base::updateColorStatus(isRunning);
                        Base::delay2(65);
                    }
                }

                if (controlType == ControlType::BACKUP_APP) {
                    if (!Device::isROMModified && !ADB::isRoot) {
                        Base::showToast("Device not support backup");
                        return;
                    }
                    string packageName = json.getString("packageSpec");

                    Base::updateLine("Backup app: " + json.getString("packageSpec"), true);

                    Storage::clearMCPTemp();

                    string pathOutput = TEMP_FOLDER + packageName + "_" + Common::getDateNow();

                    bool result = ASAction::backupApp(packageName, pathOutput);

                    Base::updateLine(
                            "Backup app " + packageName + (result ? " success" : " failed"));

                    sendFileToChannel(ReportControlType::EXPORT_FILE, pathOutput + ".zip",
                                      packageName + "_" + Common::getDateNow() + ".zip");
                }

                if (controlType == ControlType::BUSINESS_TAP) {
                    int x = json.getInt("x");
                    int y = json.getInt("x");
                    if (x < 0 || y < 0) {
                        Base::showToast("Invalid coordinates");
                        return;
                    }
                    ASService::tap(x, y);
                }

                if (controlType == ControlType::BUSINESS_SWIPE) {
                    int x1 = json.getInt("x1");
                    int y1 = json.getInt("y1");
                    int x2 = json.getInt("x2");
                    int y2 = json.getInt("y2");
                    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
                        Base::showToast("Invalid coordinates");
                        return;
                    }
                    ASService::swipe(x1, y1, x2, y2);
                }

                if (controlType == ControlType::BUSINESS_INPUT_TEXT) {
                    string input = json.getString("input");
                    if (input.empty()) {
                        Base::showToast("Invalid input");
                        return;
                    }
                    ASService::handleKeyboard(Keyboard::INPUT, input);
                }


                if (controlType == ControlType::BUSINESS_SHELL) {
                    string command = json.getString("command");
                    int timeout = json.getInt("timeout");
                    if (timeout <= 0) timeout = 60;

                    string result = ADB::runShell(command, timeout);

                    sendToChannel(ReportControlType::EXPORT_SHELL_RESULT, result);
                }

                if (controlType == ControlType::SWITCH_GO_TIENG_VIET) {
                    ASAction::switchLabanKey();
                    Base::showToast("Switch Keyboard Success");
                }
                if (controlType == ControlType::BUSSINESS_TAP_XPATH) {
                    string xpath = json.getString("xpath");
                    int timeout = json.getInt("timeout");

                    auto start = std::chrono::steady_clock::now();

                    string xml = ASService::dumpXml();

                    JSON jsonResult{};

                    if (!ASService::existNode(xpath, xml, timeout)) {
                        jsonResult.set("success", false);
                        jsonResult.set("error", "Notfound : " + xpath);
                    } else {
                        jsonResult.set("success", ASService::tap(xpath, xml, timeout));
                    }

                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - start).count();
                    jsonResult.set("time", duration);
                    sendToChannel(ReportControlType::EXPORT_TAP_XPATH_RESULT,
                                  jsonResult.toString());
                }


                if (controlType == ControlType::BUSSINESS_INPUT_XPATH) {
                    string xpath = json.getString("xpath");
                    int timeout = json.getInt("timeout");
                    string textInput = json.getString("text");
                    bool isClear = json.getBool("isClear");

                    auto start = std::chrono::steady_clock::now();

                    string xml = ASService::dumpXml();

                    JSON jsonResult{};

                    if (!ASService::existNode(xpath, xml, timeout)) {
                        jsonResult.set("success", false);
                        jsonResult.set("error", "Notfound : " + xpath);
                    } else {
                        jsonResult.set("success", ASService::inputText(xpath, textInput, isClear));
                    }
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - start).count();
                    jsonResult.set("time", duration);
                    sendToChannel(ReportControlType::EXPORT_INPUT_XPATH_RESULT,
                                  jsonResult.toString());
                }


            }
            catch (const std::exception &e) {
                Common::NLog(e.what(), "controlChannel onMessage", LogLevel::ERROR);
            }
            catch (...) {
                Common::NLog("Unknown error in controlChannel onMessage",
                             "controlChannel onMessage",
                             LogLevel::ERROR);
            }
        });

//        controlChannel->onOpen([this]() {
//            Common::NLog("controlChannel onOpen", "OCTOPUSLOWTECH", LogLevel::WARNING);
//        });
//
//        controlChannel->onClosed([this]() {
//            Common::NLog("controlChannel onClosed", "OCTOPUSLOWTECH", LogLevel::WARNING);
//        });
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "setupControlChannel", LogLevel::ERROR);
    }
    catch (...) {
        Common::NLog("Unknown error in setupControlChannel", "setupControlChannel",
                     LogLevel::ERROR);
    }

}

#define MAX_FILE_SIZE 1000 * 1024 * 1024 // 1GB

#define MIN_INTERVAL_BETWEEN_FILES 30 * 1000 // 10s

void RTC::setupFileChannel() {
    try {
        fileChannel = peerConnection->createDataChannel("fileChannel");

        fileChannel->onMessage([this](auto data) {
            std::lock_guard<std::mutex> lock(fileMutex);

            try {
                if (std::holds_alternative<std::string>(data)) {
                    string messageStr = std::get<std::string>(data);

                    JSON json{messageStr};
                    Common::NLog(messageStr);
                    FileTranferType type = static_cast<FileTranferType>(json.getInt("type"));

                    if (type != FileTranferType::FILE_COMPLETE) {
                        fileName = json.getString("name");
                        fileSize = json.getInt("size");
                        fileBuffer.clear();
                        fileBuffer.reserve(fileSize);
                        receivedBytes = 0;

                        if (type == FileTranferType::INSTALL_APK
                            || type == FileTranferType::RESTORE_APP) {
                            fileName = Common::strReplace(fileName, "-", "");
                            fileName = Common::strReplace(fileName, " ", "");
                            lastTranferType = type;
                        }

                        if (type == FileTranferType::RESTORE_APP) {
                            tranferPackageName = json.getString("config.packageSpec");
                        }

                        auto now = std::chrono::steady_clock::now();
                        auto timeSinceLastFile = std::chrono::duration_cast<std::chrono::milliseconds>(
                                now - lastFileReceivedTime).count();

                        if (timeSinceLastFile < MIN_INTERVAL_BETWEEN_FILES) {
                            string message = "Too many attempts to download files - try after 10s";
                            Base::showToast(message);
                            fileChannel->send(
                                    JSON().set("type", "FILE_ERROR").set("message",
                                                                         message).toString());
                            return;
                        }

                        lastFileReceivedTime = now;

                        if (fileSize > MAX_FILE_SIZE) {
                            string message = "Please input file size < 500MB";
                            Base::showToast(message);
                            fileChannel->send(
                                    JSON().set("type", "FILE_ERROR").set("message",
                                                                         message).toString());
                            return;
                        }

                        Base::showToast("Downloading file: " + fileName);

                    } else {
                        if (!Storage::existPath("/sdcard/Download"))
                            Storage::createFolder("/sdcard/Download");

                        string filePath = "/sdcard/Download/" + fileName;

                        std::ofstream outputFile(filePath, std::ios::binary);
                        if (!outputFile) {
                            Base::showToast("Failed to save file: " + filePath);

                            fileBuffer.clear();
                            fileBuffer.shrink_to_fit();
                            return;
                        }

                        outputFile.write(reinterpret_cast<char *>(fileBuffer.data()),
                                         fileBuffer.size());
                        outputFile.close();

                        if (receivedBytes != fileSize) {
                            Base::updateLine("File download failed: data missing", true);
                            return;
                        }

                        Base::updateLine("File saved: /sdcard/Download", true);

                        if (Common::strEndsWith(filePath, ".jpg") ||
                            Common::strEndsWith(filePath, ".png") ||
                            Common::strEndsWith(filePath, ".mp4") ||
                            Common::strEndsWith(filePath, ".jpeg") ||
                            Common::strEndsWith(filePath, ".mov")) {
                            Base::delay2(200);
                            for (int loop = 5; loop > 0; loop--) {
                                Storage::scanMedia(filePath);
                                Base::delay2(300);
                            }
                        }

                        if (lastTranferType == FileTranferType::INSTALL_APK) {
                            std::thread([filePath]() {
                                if (ASAction::installApkAction(filePath, "", "", false))
                                    Storage::removePath(filePath);
                            }).detach();
                        }
                        if (lastTranferType == FileTranferType::RESTORE_APP) {
                            std::string packageNameCopy = tranferPackageName;
                            std::thread([packageNameCopy, filePath]() {
                                if (ASAction::restoreApp(packageNameCopy, filePath)) {
                                    Storage::removePath(filePath);
                                    Device::openApp(packageNameCopy);
                                    Base::updateLine("Restore Success", true);
                                } else {
                                    Base::updateLine("Restore Failed", true);
                                }
                            }).detach();
                        }
                        fileBuffer.clear();
                        fileBuffer.shrink_to_fit();
                    }
                } else if (std::holds_alternative<rtc::binary>(data)) {
                    auto &binaryData = std::get<rtc::binary>(data);

                    if (receivedBytes + binaryData.size() > MAX_FILE_SIZE) {
                        fileChannel->send(JSON().set("type", "FILE_ERROR").set("message",
                                                                               "File size exceeded the limit").toString());
                        Base::showToast("File size exceeded the limit");
                        return;
                    }

                    fileBuffer.insert(fileBuffer.end(),
                                      reinterpret_cast<const uint8_t *>(binaryData.data()),
                                      reinterpret_cast<const uint8_t *>(binaryData.data()) +
                                      binaryData.size());
                    receivedBytes += binaryData.size();
                }
            }
            catch (const std::exception &e) {
                Common::NLog(e.what(), "fileChannel onMessage", LogLevel::ERROR);
            }
            catch (...) {
                Common::NLog("Unknown error in fileChannel onMessage", "fileChannel onMessage",
                             LogLevel::ERROR);
            }
        });

//    fileChannel->onOpen([this]() {
//        fileChannel->send("Hello from fileChannel");
//        Common::NLog("fileChannel onOpen", "OCTOPUSLOWTECH");
//    });

//    fileChannel->onClosed([this]() {
//        Common::NLog("fileChannel onClosed", "OCTOPUSLOWTECH");
//    });
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "setupFileChannel", LogLevel::ERROR);
    }
    catch (...) {
        Common::NLog("Unknown error in setupFileChannel", "setupFileChannel",
                     LogLevel::ERROR);
    }
}

void RTC::sendToChannel(ReportControlType type, const string &input) {
    try {
        if (!fileChannel->isOpen()) {
            Common::NLog("Channel not open", "sendToChannel", Common::LogLevel::ERROR);
            return;
        }

        const size_t CHUNK_SIZE = 16384; // 16KB chunks
        size_t totalSize = input.size();
        size_t chunks = (totalSize + CHUNK_SIZE - 1) / CHUNK_SIZE;

        JSON metaJson{};
        metaJson.set("type", static_cast<int>(type));
        metaJson.set("action", static_cast<int>(ReportControlType::EXPORT_START));
        fileChannel->send(metaJson.toString());

        for (size_t i = 0; i < chunks; i++) {
            size_t start = i * CHUNK_SIZE;
            size_t length = std::min(CHUNK_SIZE, totalSize - start);

            string chunk = input.substr(start, length);

            fileChannel->send(JSON()
                                      .set("action",
                                           static_cast<int>(ReportControlType::EXPORT_CONTENT))
                                      .set("chunk", chunk)
                                      .set("type", static_cast<int>(type))
                                      .toString());
        }

        fileChannel->send(JSON()
                                  .set("action",
                                       static_cast<int>(ReportControlType::EXPORT_COMPLETE))
                                  .set("type", static_cast<int>(type))
                                  .toString());
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "sendToChannel Ex", LogLevel::ERROR);
    }
    catch (...) {
        Common::NLog("Unknown error in sendToChannel", "sendToChannel Ex", LogLevel::ERROR);
    }
}


void RTC::onScreenRotate(bool isRotate) {
    sendToChannel(ReportControlType::EXPORT_SCREEN_ROTATION, isRotate ? "1" : "0");
}

void RTC::sendFileToChannel(ReportControlType type, const std::string &filePath,
                            const std::string &fileName) {
    try {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            Base::showToast("Failed to open file: " + filePath);
            return;
        }

        JSON metaJson{};
        metaJson.set("type", static_cast<int>(type));
        metaJson.set("fileName", fileName);
        metaJson.set("action", static_cast<int>(ReportControlType::EXPORT_START));
        fileChannel->send(metaJson.toString());

        const size_t CHUNK_SIZE = 16384;
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(CHUNK_SIZE);
        size_t bytesRead = 0;

        while (bytesRead < fileSize) {
            size_t chunkSize = std::min(CHUNK_SIZE, fileSize - bytesRead);
            file.read(buffer.data(), chunkSize);
            size_t actualRead = file.gcount();
            if (actualRead == 0) break;

            fileChannel->send(
                    reinterpret_cast<const std::byte *>(reinterpret_cast<const uint8_t *>(buffer.data())),
                    actualRead);

            bytesRead += actualRead;
        }

        file.close();

        fileChannel->send(JSON().set("action",
                                     static_cast<int>(ReportControlType::EXPORT_COMPLETE)).toString());
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "sendFileToChannel Ex", LogLevel::ERROR);
    }
    catch (...) {
        Common::NLog("Unknown error in sendFileToChannel", "sendFileToChannel Ex",
                     LogLevel::ERROR);
    }
}

void RTC::sendFrameToTrack(rtc::binary sample, uint64_t sampleTimestamp) {
    try {
        if (!videoTrack || !videoTrack->isOpen()) return;


        if (sample.empty()) return;

        auto elapsedSeconds = double(sampleTimestamp) / (1000 * 1000);

        uint32_t elapsedTimestamp = srReporter->rtpConfig->secondsToTimestamp(elapsedSeconds);

        srReporter->rtpConfig->timestamp =
                srReporter->rtpConfig->startTimestamp + elapsedTimestamp;

        auto reportElapsedTimestamp =
                srReporter->rtpConfig->timestamp - srReporter->lastReportedTimestamp();

        if (srReporter->rtpConfig->timestampToSeconds(reportElapsedTimestamp) > 1) {
            srReporter->setNeedsToReport();
        }

        videoTrack->send(sample);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "sendFrameToTrack Ex", LogLevel::ERROR);
    }
    catch (...) {
        Common::NLog("Unknown error in sendFrameToTrack", "sendFrameToTrack Ex",
                     LogLevel::ERROR);
    }
}

void RTC::startProcessing() {
    while (!isStop) {
        try {
            if (dataQueue.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            std::string data = dataQueue.pop();

            JSON json(data);

            string type = json.getString("type");

            if (type == "answer") {
                if (peerConnection->signalingState() ==
                    rtc::PeerConnection::SignalingState::Stable) {
                    Common::NLog("Received answer in stable state", "RTC", LogLevel::ERROR);
                } else {
                    peerConnection->setRemoteDescription(rtc::Description(json.getString("sdp"),
                                                                          rtc::Description::Type::Answer));
                    Common::NLog("Add answer success", "startProcessing");
                }
            } else if (type == "candidate") {
                string candidate = json.getString("candidate");
                string sdpMid = json.getString("sdpMid");

                peerConnection->addRemoteCandidate(rtc::Candidate(candidate, sdpMid));
            } else {
                Common::NLog("Unknow type TRANSFER_SDP", "startProcessing");
            }
        } catch (const std::exception &e) {
            Common::NLog(e.what(), "startProcessing Ex", LogLevel::ERROR);
        }
        catch (...) {
            Common::NLog("Unknown error in startProcessing", "startProcessing Ex",
                         LogLevel::ERROR);
        }

    }
    Common::NLog("RTC thread stopped", "RTC", LogLevel::WARNING);
}

void RTC::onTransferSDP(const std::string &data) {
    try {
        dataQueue.push(data);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::cleanUp Ex", LogLevel::ERROR);
    }
    catch (...) {
        Common::NLog("Unknown error in RTC::cleanUp", "RTC::cleanUp Ex", LogLevel::ERROR);
    }
}

void RTC::onClose() {
    try {
        isStop = true;
        while (!dataQueue.empty()) {
            dataQueue.pop();
        }
        if (controlChannel) {
            controlChannel->close();
            controlChannel.reset();
            controlChannel = nullptr;
        }

        if (fileChannel) {
            fileChannel->close();
            fileChannel.reset();
            fileChannel = nullptr;
        }

        if (videoTrack) {
            videoTrack->close();
            videoTrack.reset();
            videoTrack = nullptr;
        }
        if (srReporter) {
            srReporter.reset();
            srReporter = nullptr;
        }

        if (peerConnection) {
            peerConnection->close();
            peerConnection.reset();
            peerConnection = nullptr;
            Common::NLog("PeerConnection closed for clientId: " + clientId, "RTC");
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "onClose Ex", LogLevel::ERROR);
    }
    catch (...) {
        Common::NLog("Unknown error in onClose", "onClose Ex", LogLevel::ERROR);
    }
}

void RTC::log_callback(rtc::LogLevel level, const std::string &message) {
    return;
    if (!Base::isDebug) return;
    Common::NLog("RTC :" + message, "RTC",
                 (level == rtc::LogLevel::Error || level == rtc::LogLevel::Warning)
                 ? LogLevel::ERROR : LogLevel::DEBUG);
}

void RTC::cleanUp() {
    try {
        if (!lstRTCInstance.empty()) {
            for (auto &entry: lstRTCInstance) {
                entry.instance->onClose();

                if (entry.thread.joinable()) {
                    entry.thread.join();
                }
            }
        }

        lstRTCInstance.clear();

        rtcCleanup();
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "RTC::cleanUp Ex", LogLevel::ERROR);
    }
    catch (...) {
        Common::NLog("Unknown error in RTC::cleanUp", "RTC::cleanUp Ex", LogLevel::ERROR);
    }
}