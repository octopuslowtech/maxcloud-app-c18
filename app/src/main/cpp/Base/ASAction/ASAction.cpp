//
// Created by Admin on 12/29/2024.
//
#include "ASAction.h"

void ASAction::initStatic() {
    jobject jContext = getContext();

    Device::currentVersion = Device::getPackageVersion(PACKAGE);

    jobject appPref = Storage::newPref(jContext, "Config");

    if (appPref == nullptr) {
        isStop = true;
        return;
    }

    jwt = Storage::getPrefValue(appPref, "token");

    if (jwt.empty()) {
        ASService::disableSelf("JWT is empry");
        return;
    }

    Device::deviceInfo = Storage::getPrefValue(appPref, "deviceInfo");
    Device::deviceName = Storage::getPrefValue(appPref, "deviceName");
    Device::deviceBuild = Storage::getPrefValue(appPref, "deviceBuild");
    Device::deviceSerial = Storage::getPrefValue(appPref, "deviceSerial");
    Device::deviceNumber = Storage::getPrefValue(appPref, "deviceNumber");

    if (!Device::deviceName.empty())
        updateDeviceName(Device::deviceName);

    string endPoint = Storage::getPrefValue(appPref, "endpoint");
    BASE_URL = endPoint.empty() ? MAXCLOUD_ENDPOINT : endPoint;
    APP_URL = BASE_URL + "App/";
}

void ASAction::initPermission() {
    Device::unlockScreen();

    ASService::setupResource();

    ASService::initNotification();

    enableOverlay();

    enableMediaProject();

    if (Device::isROMModified)
        enableIgnoreBatteryOptimize();

    enableKeyboard();

    if (Device::existPackage(PACKAGE_HELPER) &&
        Device::getPackageVersion(PACKAGE_HELPER) != "1") {
        if (ADB::isConnected) {
            Storage::removePath(APK_FOLDER "helper.apk");
            ADB::runShell("pm uninstall " PACKAGE_HELPER);
        } else {
            while (Device::Device::getPackageVersion(PACKAGE_HELPER) != "1") {
                updateLine("Please update apk Helper to version 1.0", true);
                delay(10);
            }
        }
    }

    if(!Device::existPackage(PACKAGE_HELPER)){
        installApkFromS3(PACKAGE_HELPER, "helper");
    }

    while (!Device::existPackage(PACKAGE_HELPER)) {
        updateLine("Please install apk Helper", true);
        delay(10);
    }

    Device::openApp(PACKAGE_HELPER);

    delay2(500);

    for (int loop = 3; loop > 0; loop--) {
        string ping = ASService::sendEventToHelper(ASService::HelperAction::PING_PONG);
        if (ping.empty()) {
            Device::openApp(PACKAGE_HELPER);
            delay2(500);
            continue;
        }
        Common::NLog("Ping: " + ping, "initPermission");
        ADB::isLSPosed = ping == "ACTIVE";
        break;
    }

    if (!ADB::isRoot) return;

//    enableLSPosed();
}

void ASAction::enableMediaProject() {
    if (Media::isRunning()) return;

    JSON json;
    json.set("package", PACKAGE);
    json.set("class_name", PACKAGE ".UI.PermissionActivity");

    Device::startActivityByIntent(json, {{"action", "MEDIA_PROJECTION_PERMISSION"}});
    delay(1);

    int countDebug = 10;
    int retry = 0;

    vector<string> searchElement = {
            "//*[contains(@text,'start now')]",
            "//*[contains(@text,'bắt đầu ngay')]",
            "//*[contains(@text,'立即开始')]",
    };

    while (!Media::isRunning()) {
        Base::updateLine("Please enable Media Project", true);

        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);

        if (screen == "REQUIRE_MEDIA_PROJECT") {
            delay2(300);
            continue;
        }

        auto existElement = ASService::existNodes(0, xml, searchElement);

        if (!existElement.empty()) {
            ASService::tap(existElement, xml);
            delay2(300);
            continue;
        }

        if (screen == "UNKNOWN_SCREEN") {
            countDebug--;
            if (countDebug == 0)
                ASService::addUnknownScreen(false, "REQUIRE_MEDIA_PROJECT");
        }

        retry++;
        if (retry == 5) {
            retry = 0;
            Device::startActivityByIntent(json, {{"action", "MEDIA_PROJECTION_PERMISSION"}});
            delay(1);
        }
        delay2(1500);
    }
}

void ASAction::enableVPNService() {
//    if (!VPN::isStarted()) {
//        JSON json;
//        json.set("package", PACKAGE);
//        json.set("class_name", PACKAGE ".UI.PermissionActivity");
//
//        Device::startActivityByIntent(json, {{"action", "VPN_SERVICE_PERMISSION"}});
//
//        int retry = 0;
//        vector<string> searchElement = {
//                "//*[@text='ok']",
//                "//*[@text='bắt đầu ngay']"
//        };
//
//        while (!VPN::isStarted()) {
//            Base::updateLine("Please enable VPN Service", true);
//
//            string xml = ASService::dumpXml();
//            string screen = ASService::detectScreen(xml);
//
//            if (screen == "REQUIRE_VPN_POPUP") {
//                delay2(300);
//                continue;
//            }
//
//            auto existElement = ASService::existNodes(0, xml, searchElement);
//
//            if (!existElement.empty()) {
//                ASService::tap(existElement, xml);
//                delay2(300);
//                continue;
//            }
//
//            retry++;
//            if (retry == 5) {
//                Device::startActivityByIntent(json, {{"action", "VPN_SERVICE_PERMISSION"}});
//                retry = 0;
//            }
//
//            delay2(1500);
//        }
//    }
//
//    return;
//
//    if (!VPN::isAlwaysOnVPN()) {
//        disableScreenRotate();
//
//        JSON json;
//        json.set("action", "android.settings.VPN_SETTINGS");
//
//        Device::startActivityByIntent(json);
//
//        delay(2);
//
//        int retry = 0;
//
//        vector<string> lstXPath = {
//                "//*[@text='luôn giữ kết nối với vpn này.']",
//                "//*[@text='always-on vpn']"
//        };
//
//        while (!VPN::isAlwaysOnVPN()) {
//            Base::updateLine("Please enable VPN Always-on", true);
//
//            string xml = ASService::dumpXml();
//            string screen = ASService::detectScreen(xml);
//
//            if (screen == "VPN_ALWAYS_STEP_1") {
//                if (!ASService::existNode("//*[@text='maxcloud']", xml)) {
//                    updateLine("VPN Always-on - notfound ONOX");
//                    delay(2);
//                    continue;
//                }
//
//                Node settingNode = ASService::getNode("//*[@text='maxcloud']", xml);
//
//                if (settingNode.empty()) {
//                    updateLine("VPN Always-on - notfound Maxcloud node");
//                    delay(2);
//                    continue;
//                }
//
//                std::tuple<int, int, bool> screen = Device::getScreenSize();
//
//                int width = 0;
//
//                if (std::get<2>(screen))
//                    width = std::get<1>(screen);
//                else
//                    width = std::get<0>(screen);
//
//                int widthScreen = width * 9 / 10;
//
//                ASService::tap(widthScreen, settingNode.centerY);
//            }
//
//            if (screen == "VPN_ALWAYS_STEP_2") {
//                if (!VPN::isAlwaysOnVPN()) {
//                    string existElement = ASService::existNodes(1, xml, lstXPath);
//
//                    if (existElement.empty()) {
//                        updateLine("VPN Always-on - notfound switch");
//                        delay(2);
//                        continue;
//                    }
//
//                    ASService::tap(existElement);
//                }
//
//                if (Device::isROMModified && !VPN::isBlockConnectionWithoutVPN()) {
//                    ASService::tap("//*[@text='block connections without vpn']");
//                }
//            }
//
//            retry++;
//            if (retry == 5) {
//                Device::startActivityByIntent(json);
//                retry = 0;
//            }
//
//            delay2(1500);
//        }
//    }
}

void ASAction::enableKeyboard() {
    if (ADB::isConnected) {
        ADB::runShell("ime enable com.maxcloud.app/com.maxcloud.keyboard.latin.LatinIME");
        delay(2);
        ADB::runShell("ime set com.maxcloud.app/com.maxcloud.keyboard.latin.LatinIME");
        return;
    }

    if (Device::isKeyboardEnabled()) return;

    while (!Device::isKeyboardEnabled()) {
        Base::updateLine("Please enable keyboard", true);

        delay2(1500);
    }
}

void ASAction::enableOverlay() {
    if (Device::isCanOverlay()) {
        initView();
        return;
    }

    JSON json;
    json.set("package", PACKAGE);
    json.set("class_name", PACKAGE ".UI.PermissionActivity");

    Device::startActivityByIntent(json, {{"action", "OVERLAY_PERMISSION"}});

    vector<string> searchElement = {
            "//*[@text='Allow display over other apps']",
            "//*[@text='allow']",
            "//*[@text='cho phép']",
            "//*[@text='Allow Permission']",
    };

    int retry = 0;
    int countDebug = 10;
    while (!Device::isCanOverlay()) {
        Base::updateLine("Please enable overlay screen", true);

        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);

        if (screen == "OVERLAY_PERMISSION") {
            delay2(300);
            continue;
        }

        string elementFind = ASService::existNodes(0, xml, searchElement);
        if (!elementFind.empty()) {
            ASService::tap(elementFind, xml);
            delay2(300);
            continue;
        }

        if (screen == "MAX_CLOUD_PHONE") {
            Device::startActivityByIntent(json, {{"action", "OVERLAY_PERMISSION"}});
        }


        if (screen == "UNKNOWN_SCREEN") {
            countDebug--;
            if (countDebug == 0)
                ASService::addUnknownScreen(false, "OVERLAY_PERMISSION");
        }


        retry++;
        if (retry == 5) {
            retry = 0;
            Device::startActivityByIntent(json, {{"action", "OVERLAY_PERMISSION"}});
            delay(1);
        }

        delay2(1500);
    }

    Base::initView();
}


void ASAction::enableIgnoreBatteryOptimize() {
    string permissionName = "android.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS";

    if (Device::isPermissionGranted(permissionName))
        return;

    JSON json;
    json.set("package", PACKAGE);
    json.set("class_name", PACKAGE ".UI.PermissionActivity");

    Device::startActivityByIntent(json, {{"action", "REQUEST_IGNORE_BATTERY_OPTIMIZATIONS"}});

    vector<string> searchElement = {
            "//*[contains(@text,'allow')]",
            "//*[contains(@text,'cho phép')]"
    };

    int retry = 0;
    int countDebug = 10;

    while (!Device::isPermissionGranted(permissionName)) {
        Base::updateLine("Please disable battery optiomize", true);

        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);

        string elementFind = ASService::existNodes(0, xml, searchElement);
        if (!elementFind.empty()) {
            ASService::tap(elementFind, xml);
            delay2(300);
            continue;
        }

        if (screen == "MAX_CLOUD_PHONE") {
            Device::startActivityByIntent(json,
                                          {{"action", "REQUEST_IGNORE_BATTERY_OPTIMIZATIONS"}});
        }


        if (screen == "UNKNOWN_SCREEN") {
            countDebug--;
            if (countDebug == 0)
                ASService::addUnknownScreen(false, "REQUEST_IGNORE_BATTERY_OPTIMIZATIONS");
        }

        retry++;
        if (retry == 5) {
            retry = 0;
            Device::startActivityByIntent(json,
                                          {{"action", "REQUEST_IGNORE_BATTERY_OPTIMIZATIONS"}});
            delay(1);
        }

        delay2(1500);
    }
}


void ASAction::enableLSPosed() {
    if (ADB::isLSPosed) return;

    if (!Device::existPackage("com.android.shell")) {
        while (!isStop) {
            updateLine("Please install LSPosed", true);
            delay(5);
        }
    }

    updateLine("Enable LSPosed - Open");

    ADB::runShell(
            "am start -n com.android.shell/.BugreportWarningActivity -c org.lsposed.manager.LAUNCH_MANAGER -p com.android.shell -a android.intent.action.MAIN");


    for (int loop = 10; loop > 0; loop--) {
        if (isStop) break;
        updateLine("Enable LSPosed - " + to_string(loop));

        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);

        if ((ASService::existNode("//*[@content-desc='helper' or @text='helper']", xml)
             && ASService::existNode("//*[@content-desc='enable module' or @text='enable module']",
                                     xml)) || screen == "LSPOSED_STEP_2") {

            updateLine("Enable LSPosed - Tap ON");

            if (ASService::existNode("//*[contains(@text,'OFF')]", xml)) {
                updateLine("Enable LSPosed - Tap ON");
                ASService::tap("//*[contains(@text,'OFF')]", xml);
                delay(1);
                xml = ASService::dumpXml();
            }

            if (ASService::existNode("//*[contains(@text,'ON')]", xml)) {
                updateLine("Enable LSPosed - Success");

                Device::openApp(PACKAGE_HELPER);
                return;
            }
        }

        if (screen == "LSPOSED") {
            if (ASService::existNode("//*[contains(@content-desc,'Modules')]", xml)) {
                updateLine("Enable LSPosed - Tap Modules");
                ASService::tap("//*[contains(@content-desc,'Modules')]", xml);
                delay(2);
                xml = ASService::dumpXml();
            }
            if (ASService::existNode("//*[@content-desc='helper' or @text='helper']", xml)) {
                ASService::tap("//*[@content-desc='helper' or @text='helper']", xml);
                delay(2);
            }
        }
        delay(1);
    }
}

bool ASAction::installApkAction(std::string path, string message, string package, bool isAuto) {
    if (isStop) return false;

    if (path.empty() || !Storage::existPath(path)) {
        Common::NLog("Path not found : " + path, "installApkAction");
        return false;
    }

    if (!Common::strEndsWith(path, ".apk")) {
        path += ".apk";
    }

    message = "Installing " + (message.empty() ? "apk" : message);

    if (ADB::isConnected) {
        updateLine(message, true);

        if (!ADB::isRoot) {
            Base::showToast("Install apk - Copy File");
            string pathTemp = "/data/local/tmp/";
            ADB::runShell("cp -R " + path + " /data/local/tmp/", 60);
            path = pathTemp + Storage::getFileName(path);
        }

        Base::showToast("Install apk - Start Processing");

        string result = ADB::runShell("pm install -r " + path, 60);

        if (!ADB::isRoot)
            ADB::runShell("rm -rf " + path);

        if (Common::strContains(result, "Success")) {

            return true;
        }

        for (int loop = 10; loop > 0; loop--) {
            if (package.empty() || Device::existPackage(package)) return true;
            delay(1, message + " - " + to_string(loop));
        }

        return false;
    }

    if (!Device::installApk(path)) {
        delay(5, message + " - failed");
        Common::NLog("Install apk fail : " + path, "installApkAction");
        return false;
    }

    if (!isAuto) return true;

    for (int loop = 12; loop > 0; loop--) {
        delay(1);
        if (isStop) return false;
        updateLine(message + " - " + to_string(loop));
        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);
        if (screen == "CONTINUE") continue;
        if (screen == "ERROR") return false;
        if (screen == "INSTALL_APK_DONE") return true;
        if (screen == "INSTALL_GOOGLE_PLAY_PROTECT") continue;
        if (screen == "INSTALL_APK") {
            delay(5, message + " - Wait");
            continue;
        }
        if (screen == "INSTALL_INSTALLING") {
            delay(2, message + " - Wait");
            loop++;
        }
        if (ASService::existNode("//*[@text='processing']", xml)) {
            return false;
        }
    }
    return false;
}

bool ASAction::openAppInfoAction(const string &package, const string &title) {
    if (isStop) return false;

    if (ADB::isConnected) {
        ADB::runShell("monkey -p " + package + " -c android.intent.category.LAUNCHER 1");
        return true;
    }

    for (int loop = 10; loop > 0; loop--) {
        if (isStop) break;

        Device::openPackageInfo(package);
        delay(2, "Open Package Info");

        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);
        if (Common::strContains(screen, "_POPUP_")) continue;
        if (screen == "SETTING_APP_INFO") {
            if (title.empty() || ASService::existNode("//*[contains(@text,'" + title + "')]", xml))
                return true;
            ASService::performAction(Perform::BACK);
            delay(2);
            continue;
        }
        if (ASService::tap("//*[@content-desc='Navigate up' or @content-desc='Back']", xml)) {
            delay(2);
            continue;
        }
    }
    return false;
}

bool ASAction::closeAppAction(const std::string &package, string title) {
    if (package.empty()) {
        Common::NLog("Package is empty", "closeAppAction");
        return false;
    }

    if (!Device::Device::existPackage(package)) {
        Common::NLog("Package not found", "Device::openAppInfo");
        return false;
    }

    if (ADB::isConnected) {
        ADB::runShell("am force-stop " + package, 5);
        return true;
    }

    title = title.empty() ? "App" : title;
    updateLine("Close " + title);

    if (!openAppInfoAction(package)) return false;

    for (int loop = 10; loop > 0; loop--) {
        if (isStop) break;

        updateLine("Close " + title + " - " + to_string(loop));
        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);
        if (Common::strContains(screen, "_POPUP_")) continue;
        if (screen == "SETTING_APP_INFO") {
            if (ASService::existNode("//*[@text='Force stop' and @enabled='false']", xml))
                return true;
            ASService::tap("//*[@text='Force stop' or @text='OK']", xml);
            delay(2);
            continue;
        }


        delay(1);
    }

    return false;
}

bool ASAction::clearDataAppAction(const string &package, string title) {
    if (package.empty()) {
        Common::NLog("Package name is empty", "openApp");
        return false;
    }

    if (isStop) return false;
    title = title.empty() ? "App" : title;

    if (!Device::existPackage(package))
        return false;

    if (ADB::isConnected) {
        ADB::runShell("pm clear " + package);
        return true;
    }

    if (!openAppInfoAction(package)) return false;

    for (int loop = 30; loop > 0; loop--) {
        if (isStop) break;
        updateLine("Clear " + title + " - " + to_string(loop));
        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);
        if (Common::strContains(screen, "_POPUP_")) continue;
        if (screen == "SETTING_APP_INFO") {
            string element = ASService::existNodes(0, xml, vector<string>{
                    "//*[@text='Storage & cache' or @text='Storage and cache' or @text='Storage usage']",
                    "//*[contains(@text, 'internal storage')]",
                    "(//*[@text='Storage'])[last()]"
            });
            if (element.empty()) {
                ASService::swipe();
                continue;
            }

            ASService::tap(element, xml);
            delay(2);
            continue;
        }
        if (screen == "SETTING_APP_STORAGE") {
            if (ASService::existNode(
                    "//*[(@text='Clear storage' or @text='Clear Data' or @text='Clear data' or @text='Clear all data') and @enabled='false']",
                    xml))
                return true;
            ASService::tap(
                    "//*[@text='Clear storage' or @text='Clear Data' or @text='Clear data' or @text='Clear all data']",
                    xml);
            delay(2);
            continue;
        }
        delay(1);
    }

    return false;
}

void ASAction::changeAirplaneMode() {
    if (isStop) return;

    if(ADB::isRoot){
        ADB::runShell("settings put global airplane_mode_on 1");
        Base::delay2(400);
        ADB::runShell("am broadcast -a android.intent.action.AIRPLANE_MODE");
        Base::delay2(400);
        ADB::runShell("settings put global airplane_mode_on 0");
        Base::delay2(400);
        ADB::runShell("am broadcast -a android.intent.action.AIRPLANE_MODE");
        Base::delay(8,"Connect Network");
        return;
    }

    if (!Device::isAirplaneModeEnabled()) {
        Device::sendBroadcast(PACKAGE ".TURN_ON_AIRPLANE");
        delay(5);
    }

    for (int loop = 5; loop > 0; loop--) {
        if (!Device::isAirplaneModeEnabled()) {
            delay(8, "Toggle Airplane");
            return;
        }
        Device::sendBroadcast(PACKAGE ".TURN_OFF_AIRPLANE");
        delay(1);
    }

}

bool ASAction::setupPhone(const Proxy &proxy) {
    if (isStop) return false;
    ASService::appSetting = {};
    Device::unlockScreen();

    if (!proxy.proxyIP.empty())
        ASService::appSetting.proxy = proxy;

    try {
        updateLine("Get Setting");

        IResult response = ASService::OCRequest(
                APP_URL + "get-setting?version=" + Device::currentVersion);
        if (!response.succeeded) return false;

        JSON jResponse = response.json;

        string newVersion = jResponse.getString("appVersion");

        if (!newVersion.empty() && !Common::strContains(Device::currentVersion, "debug")) {
            delay(4, "ur runing in old version");
        }

        ASService::appSetting.proxyStr = jResponse.getString("proxy");
        ASService::appSetting.isMuteAudio = jResponse.getBool("isMuteAudio");
        ASService::appSetting.isLowBrightness = jResponse.getBool("isLowBrightness");
        ASService::appSetting.proxyType = static_cast<ProxyType>(jResponse.getInt("proxyType"));

        return true;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: setupPhone", LogLevel::ERROR);
        return false;
    }
    return false;
}

bool ASAction::setupNetwork() {
    if (isStop) return false;

    updateLine("Setup Network");

    if (ASService::appSetting.proxyType == ProxyType::WIFI) return true;

    if (ASService::appSetting.proxyType == ProxyType::MOBILE_4G) {
        if (!Device::isHasSimCard()) {
            delay(10, "Not found SIM Card");
            return false;
        }
        if (!Device::isVoiceServiceEnabled()) {
            delay(15, "Please enable MaxCloud assistant");
            return false;
        }

        Device::toggleWiFi(false);

        changeAirplaneMode();

        return true;
    }

    Device::toggleWiFi(true);

    ASService::sendEventToHelper(ASService::HelperAction::STOP_VPN, {});

    string proxyType = "http";
    if (Common::strStartsWith(ASService::appSetting.proxyStr, "socks5://")) {
        proxyType = "socks5";
        ASService::appSetting.proxyStr = Common::strReplace(ASService::appSetting.proxyStr,
                                                            "socks5://", "");
    }

    if (!ASService::appSetting.proxyStr.empty() &&
        (ASService::appSetting.proxy.proxyIP.empty() ||
         ASService::appSetting.proxy.proxyPort.empty())) {

        int expiredTime = -1;

        if (Common::strContains(ASService::appSetting.proxyStr, "|")) {
            vector<string> lstProxy = Common::strSplit(ASService::appSetting.proxyStr, "|");
            ASService::appSetting.proxyStr = lstProxy[0];
            string linkReset = lstProxy[1];
            updateLine("Reset proxy");

            Http client(linkReset, Method::GET);
            string rq = client.connect().response;
            delay(10, "Reset proxy  [" + linkReset + "] : " + rq);
            expiredTime = 1510200456;
        }

        if (Common::strStartsWith(ASService::appSetting.proxyStr, "http")
            || Common::strStartsWith(ASService::appSetting.proxyStr, "https")) {
            for (int i = 12; i > 0; i--) {
                if (isStop) return false;
                updateLine("Get Proxy - " + to_string(i));

                Http client(ASService::appSetting.proxyStr, Method::GET);
                string result = client.connect().response;

                JSON json = JSON(result);
                if (Common::strContains(ASService::appSetting.proxyStr, "tinsoft")) {
                    ASService::appSetting.proxyStr = json.getString("proxy");
                    expiredTime = json.getInt("timeout");
                } else if (Common::strContains(ASService::appSetting.proxyStr, "wwproxy")) {
                    ASService::appSetting.proxyStr = json.getString("data.proxy");

                    string expriedString = json.getString("data.expiredTime");
                    std::istringstream ss(expriedString);
                    std::tm tm = {};
                    ss >> std::get_time(&tm, "%Y/%m/%d %H:%M:%S");
                    auto timeT = std::mktime(&tm);
                    auto expiredTimePoint = std::chrono::system_clock::from_time_t(timeT);
                    auto now = std::chrono::system_clock::now();
                    int duration = std::chrono::duration_cast<std::chrono::seconds>(
                            expiredTimePoint - now).count();

                    expiredTime = duration;
                } else if (Common::strContains(ASService::appSetting.proxyStr, "proxyfb.com")) {
                    ASService::appSetting.proxyStr = json.getString("proxy");
                    expiredTime = json.getInt("timeout");
                } else {
                    ASService::appSetting.proxyStr = json.getString("proxy");
                    expiredTime = 0;
                }
                if (!ASService::appSetting.proxyStr.empty()) break;
                Common::NLog(result, "Get Proxy Fail");
                delay(10, result);
            }
        }

        vector<string> lstProxySplit = Common::strSplit(ASService::appSetting.proxyStr, ":");

        if (lstProxySplit.empty()) {
            delay(5, "Get Proxy Fail");
            return false;
        }

        ASService::appSetting.proxy.proxyIP = lstProxySplit[0];
        ASService::appSetting.proxy.proxyPort = lstProxySplit[1];

        ASService::appSetting.proxy.proxyUser = lstProxySplit.size() > 2 ? lstProxySplit[2] : "";
        ASService::appSetting.proxy.proxyPass = lstProxySplit.size() > 3 ? lstProxySplit[3] : "";
        ASService::appSetting.proxy.expiredTime = expiredTime;
    }

    if (ASService::appSetting.proxy.proxyIP.empty() ||
        ASService::appSetting.proxy.proxyPort.empty()) {
        delay(5, "Not found Proxy");
        return false;
    }

    ASService::startTimeProxy = high_resolution_clock::now();

    string fullProxy =
            ASService::appSetting.proxy.proxyIP + ":" + ASService::appSetting.proxy.proxyPort;

    if (!ASService::appSetting.proxy.proxyUser.empty() &&
        !ASService::appSetting.proxy.proxyPass.empty()) {
        fullProxy += ":" + ASService::appSetting.proxy.proxyUser + ":" +
                     ASService::appSetting.proxy.proxyPass;
    }


    if(Device::isROMModified){
        Common::NLog(proxyType, "proxyType");

        string dataExtra = proxyType + ":" + fullProxy;

        ASService::sendEventToHelper(ASService::HelperAction::START_VPN,
                                     {{"proxy", dataExtra}});

        delay(5, "Setup Network");

        updateLine("Connect Successfully", true);
        return true;
    }

    JSON json;
    json.set("package", PACKAGE_HELPER);
    json.set("class_name", "io.nekohasekai.sagernet.ui.MainActivity");

    Device::startActivityByIntent(json, {{"action", "MEDIA_PROJECTION_PERMISSION"}});
    delay(1);

    vector<string> searchElement = {
            "//*[contains(@text,'helper')]",
            "//*[contains(@text,'connected')]",
    };

    for (int loop = 10; loop > 0; loop--) {
        string xml = ASService::dumpXml();
        string screen = ASService::detectScreen(xml);

        if (screen == "ONOX_HELPER"
            || !ASService::existNodes(0, xml, searchElement).empty()
            || Common::strContains(ASService::currentPackage, PACKAGE_HELPER)) {

            Common::NLog(proxyType, "proxyType");

            string dataExtra = proxyType + ":" + fullProxy;

            ASService::sendEventToHelper(ASService::HelperAction::START_VPN,
                                         {{"proxy", dataExtra}});

            delay(5, "Setup Network");

            updateLine("Connect Successfully", true);
            return true;
        }
        delay(1);
    }
    return false;
}


jboolean ASAction::isDeviceLoggedIn(JNIEnv *env, jobject thiz, jobject jcontext) {
    Base::init();
    Storage::init(jcontext);
    Device::init(jcontext);

    jobject appPref = Storage::newPref(jcontext, "Config");

    if (appPref == nullptr) return JNI_FALSE;

    string token = Storage::getPrefValue(appPref, "token");

    std::thread([&token]() {
        for (int loop = 3; loop > 0; loop--) {
            if (!ADB::isConnected)
                ADB::init();

            if (ADB::isConnected) {
                if (!Device::isPermissionGranted("READ_EXTERNAL_STORAGE"))
                    ADB::grantPermission(PACKAGE, {"READ_EXTERNAL_STORAGE"});

                if (!Device::isPermissionGranted("WRITE_EXTERNAL_STORAGE"))
                    ADB::grantPermission(PACKAGE, {"WRITE_EXTERNAL_STORAGE"});

                break;
            }
            delay2(500);
        }
    }).detach();

    if (token.empty()) {
        Storage::clearPref(appPref);
        return JNI_FALSE;
    }

    string endPoint = Storage::getPrefValue(appPref, "endpoint");
    BASE_URL = endPoint.empty() ? MAXCLOUD_ENDPOINT : endPoint;
    APP_URL = BASE_URL + "App/";

    if (Base::isDebug)
        Common::NLog("Token : " + token, "isDeviceLoggedIn");

    return JNI_TRUE;
}

jboolean ASAction::isLoginToken(JNIEnv *env, jobject thiz, jobject jcontext) {
    jobject appPref = Storage::newPref(jcontext, "Config");

    if (appPref == nullptr) return JNI_FALSE;

    string token = Storage::getPrefValue(appPref, "token");

    if (token.empty())
        return JNI_FALSE;

    return JNI_TRUE;
}


jstring
ASAction::loginDevice(JNIEnv *env, jobject thiz, jobject jcontext, jstring jName, jstring jToken,
                      jstring jSerial, jstring jVersion, jstring jSdk, jboolean isSaveDevice) {

    string serial = JExt::jstringToString(jSerial);

    if (!ADB::isConnected)
        ADB::init();

    bool isDaoMaiROM = Common::strContains(Device::getSystemProperty("ro.build.display.id"),
                                           "daomai.tool");

    if ((isDaoMaiROM || Device::isROMModified) && !ADB::isConnected) {
        while (!ADB::isConnected) {
            ADB::init();

            if (ADB::isConnected) break;

            delay2(500);
        }
    }

    if (ADB::isConnected) {
        for (int loop = 5; loop > 0; loop--) {
            serial = ADB::runShell("getprop ro.serialno");
            if (!serial.empty()) break;
            delay2(200);
        }
    }

    if(!isSaveDevice){
        serial = Common::ranGuid();
        serial = Common::strReplace(serial, "-","");
    }

    if (!isSaveDevice && serial.empty()) {
        return JExt::stringToJstring("Serial is empty, plesae try again or contact to admin");
    }

    serial = Common::strReplace(serial, "\n", "");

    string modelName = Device::getModelName();

    string modelCode = Device::getModelCode();

    json jsonBody = {
            {"Token",      Common::strReplace(JExt::jstringToString(jToken), " ", "")},
            {"Name",       JExt::jstringToString(jName)},
            {"Serial",     serial},
            {"IsRooted",   ADB::isRootAccess()},
            {"OSVersion",  JExt::jstringToString(jSdk)},
            {"AppVersion", JExt::jstringToString(jVersion)},
            {"Model",      modelName},
            {"Build",      modelCode}
    };

    string strBody = Common::strReplace(jsonBody.dump(), "\n", "");

    Common::NLog(strBody, "addDevice");

    BASE_URL = MAXCLOUD_ENDPOINT;
//    BASE_URL = "https://api.onox.pro/";
//    BASE_URL = "https://devtest.maxcloudphone.com/";

    IResult response = ASService::OCRequest(BASE_URL + "App/add-device", strBody);

    string error = "";

    bool isSuccess = false;

    if (!response.succeeded) {
        if (Common::strContains(response.message, "Notfound Token")) {
            BASE_URL = "https://api.mincloudphone.io/";
            response = ASService::OCRequest(APP_URL + "App/add-device", strBody);
            if (!response.succeeded) {
                error = response.message;
            } else
                isSuccess = true;
        } else
            error = response.message;
    } else
        isSuccess = true;

    if (!isSuccess) {
        Common::NLog(error, "addDevice", LogLevel::ERROR);
        return JExt::stringToJstring(error);
    }

    APP_URL = BASE_URL + "App/";

    JSON json(response.response);

    string token = json.getString("data.token");
    int productNumber = json.getInt("data.productNumber");

    if (token.empty()) {
        return JExt::stringToJstring("Login failed - contact admin or try again");
    }

    Common::NLog(token, "addDevice");

    jobject appPref = Storage::newPref(jcontext, "Config");

    if (appPref == nullptr) return JExt::stringToJstring("");

    Storage::setPrefValue(appPref, "endpoint", BASE_URL);

    Storage::setPrefValue(appPref, "token", token);

    Storage::setPrefValue(appPref, "deviceInfo", modelName);

    Storage::setPrefValue(appPref, "deviceName", JExt::jstringToString(jName));

    Storage::setPrefValue(appPref, "deviceBuild", modelCode);

    Storage::setPrefValue(appPref, "deviceSerial", serial);

    Storage::setPrefValue(appPref, "deviceNumber", to_string(productNumber));

    return JExt::stringToJstring("");
}


void ASAction::onConnected(JNIEnv *env, jobject thiz) {
    ASService::isASRunning = true;

    Base::init();
    Storage::init();
    Device::init();
    Media::init();
    ASService::initService();
    ASAction::initStatic();

    std::thread([]() {
        for (int loop = 5; loop > 0; loop--) {
            ADB::init();
            if (ADB::isConnected) {
                ADB::runShell("settings put system screen_off_timeout 2147483647");

                ADB::runShell("settings put secure location_mode 0");

                ADB::runShell("settings put global master_sync_enabled 0");

                ADB::runShell("dumpsys battery set level 100");

                disableScreenRotate();

                break;
            }
            Base::delay2(1000);
        }
    }).detach();

    Base::isRunning = true;

    while (!Device::isNetworkAvailable()) {
        Base::updateLine("Please connect your network!!!", true);
        ASService::delay(5);
    }

    if (!Device::deviceNumber.empty()) {
        string path = APP_FOLDER "background.jpg";

        Device::createWallpaper(Device::deviceNumber, path);

        Device::setWallpaper(path);

        Storage::removePath(path);
    }

    ASAction::initPermission();

    Base::isRunning = false;

    if (!ASService::isAuthDevice()) return;

    Socket::connectSocket();

    Socket::initFirebase();

    jobject jContext = getContext();

    jobject newPref = Storage::newPref(jContext, "proxy");

    string proxy = Storage::getPrefValue(newPref, "proxy");

    if (!proxy.empty()) {
        ASService::appSetting.proxyType = ProxyType::PROXY;
        ASService::appSetting.proxyStr = proxy;

        ASService::appSetting.proxy.proxyIP = "";
        ASService::appSetting.proxy.proxyPort = "";
        ASService::appSetting.proxy.proxyUser = "";
        ASService::appSetting.proxy.proxyPass = "";

        ASAction::setupNetwork();
    }

}

void ASAction::onDisconnected(JNIEnv *env, jobject thiz) {
    ASService::isASRunning = false;

}

bool ASAction::installApkFromS3(std::string packageName, std::string path) {
    if (Device::Device::existPackage(packageName)) return true;

    string outputApk = APK_FOLDER + Common::strToLower(path) + ".apk";

    if (Storage::getFileSize(outputApk) < 2) {
        Storage::removePath(outputApk);

        IResult response = ASService::OCRequest(APP_URL + "get-apk?path=" + path);

        if (!response.succeeded)
            return false;

        string pathDownload = response.json.getString("url");

        if (!Http(pathDownload, Http::Method::GET).connectDownload(outputApk, "Download " + path))
            return false;
    }

    return installApkAction(outputApk, path, packageName);
}

void ASAction::factoryResetAction() {
    if (!ADB::isConnected) return;

    Socket::updateFirebase("action", "noaction");

    delay(1);

    if (Device::isROMModified) {
        string path = "/efs/octopus.prop";

        ADB::runShell("rm -rf " + path);
    }


    for (int loop = 5; loop > 0; loop--) {
        if (Device::isROMModified || ADB::isRoot)
            ADB::runShell(
                    "am broadcast -p android -a android.intent.action.FACTORY_RESET -e 'android.intent.extra.REASON' 'MasterClearConfirm' --ez 'android.intent.extra.WIPE_EXTERNAL_STORAGE' true --ez 'com.android.internal.intent.extra.WIPE_ESIMS' true --receiver-foreground");

        delay(5);

        ADB::runShell("setprop daomai.wipe_data true");

        delay(5);

        if (Device::resetFactory()) return;

        delay(5);
    }
}

jstring ASAction::getProductNumber(JNIEnv *env, jobject thiz) {
    return JExt::stringToJstring(Device::deviceNumber);
}

bool ASAction::backupApp(const string &packageName, string pathOutput) {
    if (pathOutput.empty()) {
        Common::NLog("Path output is empty", "backupApp");
        Base::showToast("Backup failed - Path output is empty");
        return false;
    }

    string pathOutputZip = pathOutput;

    if (!Common::strEndsWith(pathOutput, ".zip")) {
        pathOutputZip += ".zip";
    } else {
        pathOutput = Common::strReplace(pathOutput, ".zip", "");
    }


    if (Storage::existPath(pathOutput))
        Storage::removePath(pathOutput);

    if (Storage::existPath(pathOutputZip))
        Storage::removePath(pathOutputZip);

    string keystorePath = pathOutput + "/keystore/";

    Storage::createFolder(pathOutput);
    Storage::createFolder(keystorePath);

    if (Device::isROMModified) {
        ADB::copyFile("/efs/octopus.prop", pathOutput + "/device.prop");
    }

    ADB::copyFile("/data/misc/keystore/user_0/", keystorePath);

    vector<string> lstFile = {"databases", "files", "shared_prefs"};

    if (packageName == "com.facebook.katana")
        lstFile = {"databases", "app_light_prefs", "shared_prefs", "files/mobileconfig",
                   "app_gatekeepers"
        };

    if (packageName == "com.instagram.android")
        lstFile = {"databases", "app_light_prefs", "shared_prefs", "files/mobileconfig",
                   "files/single"
        };

    string rootPath = "/data/data/" + packageName + "/";

    for (const auto &item: lstFile) {
        if (Common::strContains(item, "/")) {
            vector<string> pathSplit = Common::strSplit(item, "/");

            string lastPath = pathOutput;

            for (size_t i = 0; i < pathSplit.size(); ++i) {
                lastPath += "/" + pathSplit[i];

                if (i == pathSplit.size() - 1) {
                    break;
                }

                Storage::createFolder(lastPath);
            }
            ADB::copyFile(rootPath + item, lastPath);
        } else
            ADB::copyFile(rootPath + item, pathOutput);
    }

    if (!Storage::zipFolder(pathOutput, pathOutputZip)) {
        delay(5, "Backup app " + packageName + " : Zip failed");
        Storage::removePath(pathOutput);
        Storage::removePath(pathOutputZip);
        return false;
    }

    Storage::removePath(pathOutput);

    double size = Storage::getFileSize(pathOutputZip);

    if (size < 0.5) {
        Storage::removePath(pathOutputZip);
        delay(5, "Backup Failed - Invalid Size");
        return false;
    }

    return true;
}

bool ASAction::restoreApp(const string &packageName, string pathInput) {
    if (pathInput.empty()) {
        Common::NLog("Path input is empty", "restoreApp");
//        Base::showToast("Restore failed - Path input is empty");
        return false;
    }

    if (!Common::strEndsWith(pathInput, ".zip")) {
        pathInput += ".zip";
    }

    if (!Storage::existPath(pathInput)) {
//        Common::NLog("Path input not found :" + pathInput, "restoreApp");
        Base::showToast("Restore failed - Path input not found");
        return false;
    }

    closeAppAction(packageName);

    clearDataAppAction(packageName);

    string keystorePath = pathInput + "/keystore/";

    double size = Storage::getFileSize(pathInput);

    if (size < 0.5) {
        Storage::removePath(pathInput);
        delay(5, "Restore Failed - Invalid Size");
        return false;
    }

    string pathExtra = Common::strReplace(pathInput, ".zip", "");

    if (!Storage::unzipFile(pathInput, pathExtra)) {
        Storage::removePath(pathInput);
        Storage::removePath(pathExtra);
        return false;
    }


    ADB::runShell("rm -rf /data/misc/keystore/user_0/");
    ADB::runShell("mkdir -m 777 /data/misc/keystore/user_0/");

    ADB::copyFile(keystorePath, "/data/misc/keystore/user_0/");
    Storage::removePath(keystorePath);

    string rootPath = "/data/data/" + packageName + "/";

    vector<string> lstFile = Storage::getListFiles(pathExtra);

    for (const auto &item: lstFile) {
        ADB::copyFile(item, rootPath);
    }

    ADB::chmodFile("/data/misc/keystore/user_0/");
    ADB::chmodFile(rootPath);

    string devicePropPath = pathExtra + "/device.prop";
    if (Device::isROMModified && Storage::existPath(devicePropPath)) {
        string pathOctopusProp = "/efs/octopus.prop";

        ADB::copyFile(devicePropPath, pathOctopusProp);

        Storage::removePath(devicePropPath);

        ADB::runShell("chmod 644 " + pathOctopusProp);

        ADB::runShell("adb reboot system");
    }

    return true;
}

void ASAction::changeDeviceByROM() {
    string param = "";

//    if (VPN::isRunning
//        && !appSetting.proxy.proxyIP.empty()
//        && VPN::isValidIp(appSetting.proxy.proxyIP)) {
//        param = "?proxy=" + appSetting.proxy.proxyIP;
//    }
    IResult response = ASService::OCRequest(APP_URL + "change-device" + param);

    if (!response.succeeded) {
        delay(5, "Change Device failed");
        return;
    }

    JSON jsonDevice{response.json.toString()};

    string octopusProp = "ro.build.user=" + jsonDevice.getString("user") + "\n";
    octopusProp += "ro.build.host=" + jsonDevice.getString("host") + "\n";

    // TIME UTC

    string timeString = jsonDevice.getString("time");
    long dateUTC = Common::strToLong(timeString);
    string date = Common::timestampToDateString(dateUTC);

    string dateUTCStr = to_string((dateUTC / 1000));

    appendBuildProp(octopusProp,
                    {"ro.vendor.build.date", "ro.vendor_dlkm.build.date",
                     "ro.system_ext.build.date", "ro.odm.build.date",
                     "ro.build.date", "ro.bootimage.build.date"}, date);


    appendBuildProp(octopusProp,
                    {"ro.vendor.build.date.utc", "ro.vendor_dlkm.build.date.utc",
                     "ro.system_ext.build.date.utc", "ro.odm.build.date.utc",
                     "ro.build.date.utc", "ro.bootimage.build.date.utc"}, dateUTCStr);

    string formattedDate = Common::timestampToDateString(dateUTC, "%Y-%m-%d");
    appendBuildProp(octopusProp,
                    {"ro.build.version.security_patch"}, formattedDate);
//

    appendBuildProp(octopusProp,
                    {"ro.product.model",
                     "ro.product.bootimage.model",
                     "ro.product.vendor_dlkm.model",
                     "ro.product.vendor.model",
                     "ro.product.system_ext.model", "ro.product.system.model",
                     "ro.product.odm.model"}, jsonDevice.getString("model"));


    appendBuildProp(octopusProp,
                    {"ro.product.brand",
                     "ro.product.odm.brand",
                     "ro.product.system.brand",
                     "ro.product.vendor.brand",
                     "ro.product.vendor_dlkm.brand",
                     "ro.product.system_ext.brand",
                     "ro.product.bootimage.brand",
                    }, jsonDevice.getString("brand"));

    appendBuildProp(octopusProp,
                    {"ro.product.device",
                     "ro.product.bootimage.device",
                     "ro.product.vendor_dlkm.device",
                     "ro.product.vendor.device",
                     "ro.product.system_ext.device",
                     "ro.product.system.device",
                     "ro.product.odm.device"}, jsonDevice.getString("device"));


    appendBuildProp(octopusProp,
                    {"ro.product.name",
                     "ro.build.product",
                     "ro.product.bootimage.name",
                     "ro.product.vendor_dlkm.name",
                     "ro.product.vendor.name",
                     "ro.product.system_ext.name",
                     "ro.product.system.name", "ro.product.odm.name"},
                    jsonDevice.getString("product"));


    appendBuildProp(octopusProp,
                    {
                            "ro.build.id",
                            "ro.odm.build.id",
                            "ro.vendor.build.id",
                            "ro.system.build.id",
                            "ro.system_ext.build.id",
                            "ro.vendor_dlkm.build.id"}, jsonDevice.getString("id"));


    appendBuildProp(octopusProp,
                    {"ro.build.display.id"}, jsonDevice.getString("incremental"));

    appendBuildProp(octopusProp,
                    {"ro.build.version.incremental",
                     "ro.vendor.build.version.incremental",
                     "ro.system.build.version.incremental",
                     "ro.odm.build.version.incremental",
                     "ro.bootimage.build.version.incremental",
                     "ro.vendor_dlkm.build.version.incremental",
                    }, jsonDevice.getString("display"));
    appendBuildProp(octopusProp,
                    {
                            "ro.build.fingerprint",
                            "ro.odm.build.fingerprint",
                            "ro.system.build.fingerprint",
                            "ro.system_ext.build.fingerprint",
                            "ro.vendor.build.fingerprint",
                            "ro.vendor_dlkm.build.fingerprint",

                    }, jsonDevice.getString("fingerprint"));


    appendBuildProp(octopusProp,
                    {
//                            "ro.build.version.sdk",
//                            "ro.system.build.version.sdk",
//                            "ro.odm.build.version.sdk",
//                            "ro.vendor.build.version.sdk",
//                            "ro.bootimage.build.version.sdk",
//                            "ro.vendor_dlkm.build.version.sdk",
//                            "ro.system_ext.build.version.sdk"
            }, jsonDevice.getString("sdk"));


    appendBuildProp(octopusProp,
                    {
                            "ro.build.version.release",
                            "ro.build.version.release_or_preview_display",
                            "ro.build.version.release_or_codename",
                            "ro.system.build.version.release",
                            "ro.system.build.version.release_or_codename",
                            "ro.odm.build.version.release",
                            "ro.vendor.build.version.release",
                            "ro.vendor.build.version.release_or_codename",
                            "ro.vendor_dlkm.build.version.release",
                            "ro.vendor_dlkm.build.version.release_or_codename",
                            "ro.bootimage.build.version.release",
                            "ro.bootimage.build.version.release_or_codename"

                    }, jsonDevice.getString("release"));

    appendBuildProp(octopusProp,
                    {
                            "ro.bootloader",
                            "ro.boot.bootloader"
                    }, jsonDevice.getString("bootloader"));


    appendBuildProp(octopusProp,
                    {"ro.product.manufacturer",
                     "ro.product.bootimage.manufacturer",
                     "ro.product.vendor_dlkm.manufacturer",
                     "ro.product.vendor.manufacturer",
                     "ro.product.system_ext.manufacturer",
                     "ro.product.system.manufacturer",
                     "ro.product.odm.manufacturer"}, jsonDevice.getString("manufacturer"));


    appendBuildProp(octopusProp,
                    {
                            "ro.build.description",
                    }, jsonDevice.getString("description"));


    appendBuildProp(octopusProp,
                    {
                            "ro.product.board",
                    }, jsonDevice.getString("board"));

    // SERIAL __ FAKE AFTER BACKUP FIRST


    appendBuildProp(octopusProp,
                    {
                            "gsm.version.baseband",
                    }, jsonDevice.getString("baseband"));


    appendBuildProp(octopusProp,
                    {
                            "ro.arch",
                    }, jsonDevice.getString("ro_arch"));

    appendBuildProp(octopusProp,
                    {
                            "ro.hardware",
                            "ro.boot.hardware"
                    }, jsonDevice.getString("ro_boot_hardware"));



    // boot loop
//    appendBuildProp(octopusProp,
//                    {
//                            "ro.board.platform",
//                    }, jsonDevice.getString("ro_board_platform"));
//

//    appendBuildProp(octopusProp,
//                    {"ro.product.locale",}, jsonDevice.getString("country_code"));


    // save file
    string outPath = APP_FOLDER "octopus.prop";
    if (Storage::existPath(outPath))
        Storage::removePath(outPath);

    Storage::writeFile(outPath, octopusProp);

    string path = "/efs/octopus.prop";

    ADB::runShell("rm -rf " + path);

    ADB::runShell("cp -R " APP_FOLDER "octopus.prop " + path);

    ADB::runShell("chmod 644 " + path);

    ADB::runShell("adb reboot system");
}

void ASAction::appendBuildProp(string &content, vector<string> lstKey, const string &value) {
    if (value.empty()) return;
    for (const auto &key: lstKey) {
        if (Common::strContains(content, key)) {
            Common::NLog("" + key + " already exist", "appendBuildProp");
            continue;
        }
        content += key + "=" + value + "\n";
    }
}

bool ASAction::changeDevicebyLSPosed() {
    string param = "";

    IResult response = ASService::OCRequest(APP_URL + "change-device" + param);

    if (!response.succeeded) {
        delay(5, "Change Device failed");
        return false;
    }

    string deviceInfo = response.json.toString();

    ASService::sendEventToHelper(ASService::HelperAction::RESTORE_DEVICE, {{"data", deviceInfo}});
    return !deviceInfo.empty();
}

jboolean ASAction::startServiceNative(JNIEnv *env, jobject thiz) {
    if (!ADB::isConnected) return false;

    ADB::grantPermission(PACKAGE, {"READ_EXTERNAL_STORAGE", "WRITE_EXTERNAL_STORAGE"});

    if (ASService::isASRunning) return true;

    vector<string> lstAutomation = {
            "com.github.uiautomator",
            "io.appium.settings",
            "com.github.uiautomator.test",
            "io.appium.uiautomator2.server",
            "io.appium.uiautomator2.server.test"
    };

    for (auto package: lstAutomation) {
        if (Device::existPackage(package))
            ADB::runShell("pm uninstall -k " + package);
    }

    ADB::runShell(
            "settings put secure enabled_accessibility_services " PACKAGE "/.Core.MainService");

    return true;
}

void ASAction::disableScreenRotate() {
    for (int loop = 10; loop > 0; loop--) {
        if (!Device::isScreenRotateEnable()) break;

        ADB::runShell("settings put system accelerometer_rotation 0");

        delay2(500);
    }
}

void ASAction::switchLabanKey() {
    string package = "com.vng.inputmethod.labankey";

    if (!Device::existPackage(package)) {
        if (!installApkFromS3(package, Common::strToLower("labankey"))) {
            delay(3, "Cannot install labankey", true);
        }
    }

    if (!Device::existPackage(package))
        return;

    ADB::runShell("ime enable com.vng.inputmethod.labankey/.LatinIME");

    delay(2);

    ADB::runShell("ime set com.vng.inputmethod.labankey/.LatinIME");
}

