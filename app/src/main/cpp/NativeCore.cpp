//
// Created by Admin on 12/23/2024.
//

#include "Core/Tiktok/TiktokCore.cpp"
#include "Core/Facebook/FacebookCore.cpp"
#include "Core/Google/GoogleCore.cpp"
#include "Core/Instagram/InstaCore.cpp"

static void handleAction(string action) {
    if (!Common::isSocialAction(action)) {

        if (Common::strContains(action, "reset_socket")) {
            Common::NLog("reset_socket", "handleAction");
            Base::showToast("Reset Socket");

            Socket::forceReconnect();

            RTC::cleanUp();
            rtcCleanup();
            return;
        }

        if (Common::strContains(action, "check_update")) {
            Base::updateLine("Check Update");

            IResult response = ASService::OCRequest(
                    Base::APP_URL + "get-setting?version=" + Device::currentVersion +
                    "&IsBackground=true");
            if (!response.succeeded) {
                Base::updateLine("Check update failed");
                return;
            }

            string wallpaperUrl = response.json.getString("backgroundURL");
            if (!wallpaperUrl.empty()) {
                string imgPath = MEDIA_FOLDER + Common::ranGuid() + ".jpg";

                Http http(wallpaperUrl, Method::GET);
                if (!http.connectDownload(imgPath)) {
                    Base::showToast("Download wallpaper fail");
                    ASService::delay(5, "Download wallpaper fail");
                    return;
                }

                Device::setWallpaper(imgPath);

                Storage::removePath(imgPath);
            }

            string newVersion = response.json.getString("appVersion");

            if (newVersion.empty()) {
                ASService::delay(5, "Your current version is new");
                return;
            }

            Http http(newVersion, Method::GET);

            string outputApk = APK_FOLDER "cloud.apk";
            if (!http.connectDownload(outputApk)) {
                Base::showToast("Update - Download fail");
                ASService::delay(5, "Update - Download fail");
                return;
            }
            ASAction::installApkAction(outputApk, "MaxCloud");
        }

        if (Common::strContains(action, "change_background")) {
            Base::updateLine("Change Background");

            IResult response = ASService::OCRequest(
                    Base::APP_URL + "get-setting?version=" + Device::currentVersion +
                    "&IsBackground=true");

            if (!response.succeeded) {
                Base::updateLine("Check update failed");
                return;
            }

            string wallpaperUrl = response.json.getString("backgroundURL");
            if (!wallpaperUrl.empty()) {
                string imgPath = MEDIA_FOLDER + Common::ranGuid() + ".jpg";

                Http http(wallpaperUrl, Method::GET);
                if (!http.connectDownload(imgPath)) {
                    Base::showToast("Download wallpaper fail");
                    ASService::delay(5, "Download wallpaper fail");
                    return;
                }
                Device::setWallpaper(imgPath);

                Storage::removePath(imgPath);
            }

            string newVersion = response.json.getString("appVersion");

            if (newVersion.empty()) {
                ASService::delay(5, "Your current version is new");
                return;
            }
        }

        if (Common::strContains(action, "change_device")) {
            if (!ADB::isRoot) {
                Base::updateLine("Device not rooted", true);
                return;
            }

            Base::updateLine("Change Device");

            string package = "ru.andr7e.deviceinfohw";
            ASAction::closeAppAction(package);

            bool isSuccess = Social::changeDevice();
            if (!isSuccess) {
                Base::updateLine("Change Device Fail", true);
                return;
            }

            Device::openApp(package);

            Base::updateLine("Change Device Success", true);
        }

        if (Common::strContains(action, "post_local_screen")) {
            vector<string> lstFile = Storage::getListFiles(DEBUG_FOLDER);

            if (lstFile.size() > 5) {
                for (int i = 0; i < lstFile.size() - 5; i++) {
                    Storage::removePath(lstFile[i]);
                    Common::NLog("Remove file: " + lstFile[i]);
                }
            }

            lstFile = Storage::getListFiles(DEBUG_FOLDER);
            Base::updateLine("Post " + to_string(lstFile.size()) + " Screen");

            for (const auto &file: lstFile) {
                if (!Common::strEndsWith(file, ".txt")) continue;

                string content = Storage::readFile(file);
                ASService::OCRequest(MAXCLOUD_ENDPOINT "Dev/add-unknown-screen", content);
            }
            Base::updateLine("Post Screen Done");
        }

        if (Common::strContains(action, "post_screen")) {
            Base::updateLine("Post Screen");
            Base::delay(5);
            ASService::addUnknownScreen(false, "POST_SCREEN");
        }

        if (Common::strContains(action, "detect_screen")) {
            if (ASService::setupResource()) {
                Base::delay(2);
                string screen = ASService::detectScreen();
                if (screen == "UNKNOWN_SCREEN") {
                    Base::updateLine("Post Screen");
                    ASService::addUnknownScreen(false, "DETECT_SCREEN");
                }
                Base::updateLine(screen);
            }
        }

        if (Common::strContains(action, "export_xml")) {
            string path = APP_FOLDER "resource.txt";
            Base::delay(2);
            JSON jsonBody;
            jsonBody.set("xml", ASService::dumpXml());
            jsonBody.set("activity", ASService::currentActivity);

            Storage::writeFile(path, jsonBody.toString());
        }

        if (Common::strContains(action, "execute_script")) {
            vector<string> lstSplit = Common::strSplit(action, "_");
            string scriptId = lstSplit.size() > 1 ? lstSplit[2] : "";

            OWrapper::executeScriptNoCode(scriptId);
        }
    }

    if (Common::strStartsWith(action, "login_facebook_")) {
        action = Common::strReplace(action, "login_facebook_", "facebook_login_uid_");
    }


    if (Common::strContains(action, "tiktok")) {
        TiktokCore tikCore;
        if (Common::strContains(action, "scan_account")) {
            if (!tikCore.initSocial(action)) return;
            tikCore.handleScanAccountInDevice();
            return;
        }

        Social::handleCoreActions(tikCore, action);
        return;
    }

    if (Common::strStartsWith(action, "facebook")) {
        FacebookCore fbCore;
        Social::handleCoreActions(fbCore, action);
        return;
    }

    if (Common::strStartsWith(action, "google")) {
        GoogleCore googleCore;
        Social::handleCoreActions(googleCore, action);
        return;
    }

    if (Common::strStartsWith(action, "instagram")) {
        InstaCore instaCore;
        Social::handleCoreActions(instaCore, action);
        return;
    }


}

static void onActionReceived(JNIEnv *envObj, jobject thiz, jstring jaction, jstring jDeviceName) {
    string deviceName = JExt::jstringToString(jDeviceName);

    if (!deviceName.empty()) {
        if (deviceName != Device::deviceName) {
            jobject appPref = Storage::newPref(Base::getContext(), "Config");
            Storage::setPrefValue(appPref, "deviceName", deviceName);
        }
        Device::deviceName = deviceName;
        Base::updateDeviceName(deviceName);
    }

    string action = Common::strToLower(JExt::jstringToString(jaction));
    if (action.empty()) {
        Common::NLog("Action is empty", "onActionReceived");
        return;
    }

    Common::NLog(action, "onActionReceived");

    if (Common::strContains(action, "DISABLE_SELF")) {
        ASService::disableSelf("onActionReceived Disable Self");
    }

    std::thread nativeThread([action]() {
        Base::delay2(1);

        if (Common::strContains("noaction", action) || Common::strContains("stopaction", action)) {
            string lastMessage = Base::getLastStatus();
            if (lastMessage.empty() || lastMessage == "Connecting to server...")
                Base::updateLine("Waiting for action...");

            if (ASService::isRunning) {
                Base::showToast("Stopping action");
                ASService::isStop = true;
                return;
            }
            if (Common::strContains("stopaction", action)) {
                Socket::updateFirebase("action", "noaction");
            }
            return;
        }

        if (Common::strContains("rental_expired", action)) {
            Media::cleanUp();
//            VPN::stop();
            ASService::sendEventToHelper(ASService::HelperAction::STOP_VPN,{});
            ASAction::factoryResetAction(); // trong nay da update noaction
            return;
        }


        if (Base::isRunning) {
            Base::showToast("Please stop current action and try again");
            return;
        }

        ASService::isRunning = true;

        if (!ASService::isAuthDevice())
            return;

        Base::showToast("Executing Task");

        Base::updateColorStatus(true);

        ASAction::initStatic();
        Storage::init();
        ADB::init();
        ASAction::initPermission();

        handleAction(action);

        Base::isRunning = false;
        ASService::isStop = false;

        Socket::updateFirebase("action", "noaction");

        Base::updateColorStatus(false);
        Base::showToast("Task completed");

        JExt::mJavaVM->DetachCurrentThread();
    });
    nativeThread.detach();
}

static JNINativeMethod method_table[] = {
        {
                "authDevice",
                "(Landroid/content/Context;)Z",
                (void *) ASAction::isDeviceLoggedIn
        },
        {
                "isLoginToken",
                "(Landroid/content/Context;)Z",
                (void *) ASAction::isLoginToken
        },
        {
                "addDevice",
                "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;",
                (void *) ASAction::loginDevice
        },

        {
                "onConnected",
                "()V",
                (void *) ASAction::onConnected
        },
        {
                "onEvent",
                "(Landroid/view/accessibility/AccessibilityEvent;)V",
                (void *) ASService::onEvent
        },
        {
                "onActionReceived",
                "(Ljava/lang/String;Ljava/lang/String;)V",
                (void *) onActionReceived
        },

        {
                "initMediaProject",
                "(ILandroid/content/Intent;)V",
                (void *) Media::initMediaProject
        },
        {
                "getProductNumber",
                "()Ljava/lang/String;",
                (void *) ASAction::getProductNumber
        },
        {
                "startServiceNative",
                "()Z",
                (void *) ASAction::startServiceNative
        },

};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JExt::initialize(vm);
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass clazz = JExt::findClass("com/maxcloud/app/Core/NativeCore");
    if (clazz == nullptr) {
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, method_table, sizeof(method_table) / sizeof(method_table[0])) <
        0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}