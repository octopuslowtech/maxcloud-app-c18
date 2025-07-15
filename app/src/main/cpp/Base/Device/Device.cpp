#include "Device.h"
#include "../ASService/ASModel.h"

string Device::deviceInfo;
string Device::deviceName;
string Device::deviceBuild;
string Device::deviceNumber;
string Device::deviceSerial;

string Device::currentVersion;

bool Device::isROMModified = false;

Device::DeviceVM Device::deviceVM = {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
};

void Device::init(jobject jcontext) {
    if (deviceVM.jClass != nullptr) return;

    deviceVM.jClass = JExt::findClass("com/maxcloud/app/Extensions/AppExt", true);

    deviceVM.installAPKMethod = JExt::findMethod(deviceVM.jClass, "installAPK",
                                                 "(Ljava/lang/String;Landroid/content/Context;)Z",
                                                 true);

    deviceVM.isNetworkAvailableMethod = JExt::findMethod(deviceVM.jClass,
                                                         "isNetworkAvailable",
                                                         "(Landroid/content/Context;)Z",
                                                         true);

    deviceVM.unlockScreenMethod = JExt::findMethod(deviceVM.jClass, "unlockScreen",
                                                   "(Landroid/content/Context;)V",
                                                   true);

    deviceVM.sendBroadcastAndWaitMethod = JExt::findMethod(deviceVM.jClass, "sendBroadcastAndWait",
                                                           "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;",
                                                           true);


    deviceVM.toggleFlashMethod = JExt::findMethod(deviceVM.jClass, "toggleFlash",
                                                  "(Landroid/content/Context;)V",
                                                  true);

    deviceVM.setWallpaperMethod = JExt::findMethod(deviceVM.jClass, "setWallpaper",
                                                   "(Landroid/content/Context;Ljava/lang/String;)Z",
                                                   true);

    deviceVM.createWallpaperMethod = JExt::findMethod(deviceVM.jClass, "createWallpaper",
                                                      "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)Z",
                                                      true);


    deviceVM.isDebuggingMethod = JExt::findMethod(deviceVM.jClass, "isDebugging",
                                                  "()Z",
                                                  true);

    deviceVM.getClipboardMethod = JExt::findMethod(deviceVM.jClass, "getClipboard",
                                                   "(Landroid/content/Context;)Ljava/lang/String;",
                                                   true);

    deviceVM.setClipboardMethod = JExt::findMethod(deviceVM.jClass, "setClipboard",
                                                   "(Landroid/content/Context;Ljava/lang/String;)V",
                                                   true);

    deviceVM.startActivityByIntentMethod = JExt::findMethod(deviceVM.jClass,
                                                            "startActivityByIntent",
                                                            "(Landroid/content/Context;Ljava/lang/String;)Z",
                                                            true);

    deviceVM.changeUSLanguageMethod = JExt::findMethod(deviceVM.jClass, "changeLanguage",
                                                       "(Landroid/content/Context;)Z",
                                                       true);

    deviceVM.luoncomotcohoilangaymaiMethod = JExt::findMethod(deviceVM.jClass,
                                                              "luoncomotcohoilangaymai",
                                                              "(Landroid/content/Context;)Z",
                                                              true);

    deviceVM.getPackagesInstalledWithIconMethod = JExt::findMethod(deviceVM.jClass,
                                                                   "getPackagesInstalledWithIcon",
                                                                   "(Landroid/content/Context;)Ljava/lang/String;",
                                                                   true);

    deviceVM.isPermissionGrantedMethod = JExt::findMethod(deviceVM.jClass,
                                                          "isPermissionGranted",
                                                          "(Landroid/content/Context;Ljava/lang/String;)Z",
                                                          true);

    deviceVM.keyboardClass = JExt::findClass("com/maxcloud/keyboard/latin/LatinIME", true);

    deviceVM.isRunningkeyboardField = JExt::findField(deviceVM.keyboardClass, "IsRunning", "Z",
                                                      true);

    deviceVM.voidServiceClass = JExt::findClass(
            "com/maxcloud/app/Core/Airplane/MyInteractionService", true);
    deviceVM.isVoiceServiceEnabledField = JExt::findField(deviceVM.voidServiceClass, "IsRunning",
                                                          "Z", true);

    if (jcontext == nullptr)
        jcontext = getContext();

    isDebug = isDebugging();

    string arrowVersion = getSystemProperty("ro.arrow.version");

    arrowVersion = Common::strToLower(arrowVersion);

    isROMModified = Common::strContains(arrowVersion, "arrow");

    sDpi = getScreenDpi();

    std::tuple<int, int, bool> screen = getScreenSize(jcontext);

    sWidth = std::get<0>(screen);
    sHeight = std::get<1>(screen);
}

void Device::sendBroadcast(string action, string package,
                           vector<std::pair<std::string, string>> lstExtra) {
    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();

    jclass intentClass = JExt::findClass("android/content/Intent");

    jmethodID intentConstructor = JExt::findMethod(intentClass, "<init>", "()V");
    jobject intentObject = env->NewObject(intentClass, intentConstructor);

    jmethodID setActionMethod = JExt::findMethod(intentClass, "setAction",
                                                 "(Ljava/lang/String;)Landroid/content/Intent;",
                                                 false);
    jstring jAction = JExt::stringToJstring(action);
    env->CallObjectMethod(intentObject, setActionMethod, jAction);
    env->DeleteLocalRef(jAction);

    if (package.length() > 0) {
        jmethodID setPackageMethod = JExt::findMethod(intentClass, "setPackage",
                                                      "(Ljava/lang/String;)Landroid/content/Intent;");
        jstring jPackage = JExt::stringToJstring(package);
        env->CallObjectMethod(intentObject, setPackageMethod, jPackage);
        env->DeleteLocalRef(jPackage);
    }

    if (lstExtra.size() > 0) {
        jmethodID putExtraMethod = JExt::findMethod(intentClass, "putExtra",
                                                    "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;");
        for (size_t i = 0; i < lstExtra.size(); ++i) {
            jstring jKey = JExt::stringToJstring(lstExtra[i].first);
            jstring jValue = JExt::stringToJstring(lstExtra[i].second);
            env->CallObjectMethod(intentObject, putExtraMethod, jKey, jValue);
            env->DeleteLocalRef(jKey);
            env->DeleteLocalRef(jValue);
        }
    }

    jclass contextClass = JExt::findClass("android/content/Context");

    jmethodID sendBroadcastMethod = JExt::findMethod(contextClass, "sendBroadcast",
                                                     "(Landroid/content/Intent;)V");

    env->CallVoidMethod(jContext, sendBroadcastMethod, intentObject);


    env->DeleteLocalRef(intentObject);
    env->DeleteLocalRef(intentClass);
}


string Device::sendBroadcastAndWait(JSON json) {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    string jsonStr = json.toString();
    if (jsonStr.empty()) {
        Common::NLog("JSON is empty", "sendBroadcastAndWait");
        return "";
    }
    jstring jAction = JExt::stringToJstring(jsonStr);

    jobject result = env->CallStaticObjectMethod(deviceVM.jClass,
                                                 deviceVM.sendBroadcastAndWaitMethod, jContext,
                                                 jAction);

    string resultStr = JExt::jobjectToString(result);

    env->DeleteLocalRef(jAction);
    env->DeleteLocalRef(result);

    return resultStr;
}

string Device::getPackageVersion(string package) {
    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();
    jstring jPackage = JExt::stringToJstring(package);

    jclass contextClass = JExt::findClass("android/content/Context");

    jmethodID getPackageManagerMethod = JExt::findMethod(contextClass,
                                                         "getPackageManager",
                                                         "()Landroid/content/pm/PackageManager;");

    jobject packageManager = env->CallObjectMethod(jContext, getPackageManagerMethod);

    jclass packageManagerClass = JExt::findClass("android/content/pm/PackageManager");

    jmethodID getPackageInfoMethod = JExt::findMethod(packageManagerClass, "getPackageInfo",
                                                      "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject packageInfo = env->CallObjectMethod(packageManager, getPackageInfoMethod, jPackage, 0);

    jclass packageInfoClass = JExt::findClass("android/content/pm/PackageInfo");

    jfieldID versionNameField = JExt::findField(packageInfoClass, "versionName",
                                                "Ljava/lang/String;", false);
    jstring versionName = (jstring) env->GetObjectField(packageInfo, versionNameField);

    string result = JExt::jobjectToString(versionName);
    env->DeleteLocalRef(jPackage);
    env->DeleteLocalRef(packageManagerClass);
    env->DeleteLocalRef(packageInfoClass);
    env->DeleteLocalRef(packageManager);
    env->DeleteLocalRef(packageInfo);
    env->DeleteLocalRef(versionName);
    env->DeleteLocalRef(contextClass);

    return result;
}

void Device::setWallpaper(const string &path) {
    if (path.empty() || !Storage::existPath(path)) {
        Common::NLog("Path not found : " + path, "setWallpaper");
        return;
    }

    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();
    jstring jPath = JExt::stringToJstring(path);

    for (int loop = 5; loop > 0; loop--) {

        bool isSuccess = env->CallStaticBooleanMethod(deviceVM.jClass, deviceVM.setWallpaperMethod,
                                                      jContext, jPath) == JNI_TRUE;

        if (isSuccess) break;

        Base::delay(1);
    }
}

bool Device::isAirplaneModeEnabled() {
    int result = getSecureSettingInt("airplane_mode_on", "System");

    return result != 0;
}

bool Device::isVoiceServiceEnabled() {
    JNIEnv *env = JExt::getEnv();
    return env->GetStaticBooleanField(deviceVM.voidServiceClass,
                                      deviceVM.isVoiceServiceEnabledField);
}

bool Device::isHasSimCard() {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    jclass contextClass = JExt::findClass("android/content/Context");

    jfieldID telephonyServiceField = env->GetStaticFieldID(
            contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");

    jstring telephonyService = (jstring) env->GetStaticObjectField(contextClass,
                                                                   telephonyServiceField);

    jmethodID getSystemServiceMethod = JExt::findMethod(
            contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    jobject telephonyManager = env->CallObjectMethod(jContext, getSystemServiceMethod,
                                                     telephonyService);

    jclass telephonyManagerClass = JExt::findClass("android/telephony/TelephonyManager");

    jmethodID hasIccCardMethod = JExt::findMethod(telephonyManagerClass, "hasIccCard", "()Z");

    jboolean result = env->CallBooleanMethod(telephonyManager, hasIccCardMethod);

    env->DeleteLocalRef(telephonyManager);
    env->DeleteLocalRef(telephonyService);

    return result;
}

bool Device::isNetworkAvailable() {
    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();
    return env->CallStaticBooleanMethod(deviceVM.jClass, deviceVM.isNetworkAvailableMethod,
                                        jContext) == JNI_TRUE;
}

void Device::toggleWiFi(bool enable) {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    jclass contextClass = JExt::findClass("android/content/Context");

    // Lấy WIFI_SERVICE từ Context
    jfieldID wifiServiceField = env->GetStaticFieldID(
            contextClass, "WIFI_SERVICE", "Ljava/lang/String;");

    jstring wifiService = (jstring) env->GetStaticObjectField(contextClass, wifiServiceField);

    // Lấy phương thức getSystemService từ Context
    jmethodID getSystemServiceMethod = JExt::findMethod(
            contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    // Gọi getSystemService để lấy WifiManager
    jobject wifiManager = env->CallObjectMethod(jContext, getSystemServiceMethod, wifiService);

    // Lấy lớp WifiManager
    jclass wifiManagerClass = JExt::findClass("android/net/wifi/WifiManager");

    // Lấy phương thức isWifiEnabled từ WifiManager
    jmethodID isWifiEnabledMethod = JExt::findMethod(wifiManagerClass, "isWifiEnabled", "()Z");
    jboolean currentWifiState = env->CallBooleanMethod(wifiManager, isWifiEnabledMethod);

    if (currentWifiState == enable) {
        env->DeleteLocalRef(contextClass);
        env->DeleteLocalRef(wifiService);
        env->DeleteLocalRef(wifiManager);
        env->DeleteLocalRef(wifiManagerClass);
        return;
    }

    jmethodID setWifiEnabledMethod = JExt::findMethod(wifiManagerClass, "setWifiEnabled", "(Z)Z");

    env->CallBooleanMethod(wifiManager, setWifiEnabledMethod, enable);

    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(wifiService);
    env->DeleteLocalRef(wifiManager);
    env->DeleteLocalRef(wifiManagerClass);
}


void Device::toggleFlash() {
    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();
    env->CallStaticVoidMethod(deviceVM.jClass, deviceVM.toggleFlashMethod,
                              jContext);

}

int Device::getUsedMemoryPercentage() {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    // Lấy ActivityManager và MemoryInfo
    jclass activityManagerClass = JExt::findClass("android/app/ActivityManager");
    if (activityManagerClass == nullptr) return -1;

    jmethodID getMemoryInfoMethod = JExt::findMethod(activityManagerClass, "getMemoryInfo",
                                                     "(Landroid/app/ActivityManager$MemoryInfo;)V");
    if (getMemoryInfoMethod == nullptr) return -1;

    jclass memoryInfoClass = JExt::findClass("android/app/ActivityManager$MemoryInfo");
    if (memoryInfoClass == nullptr) return -1;

    jmethodID memoryInfoConstructor = JExt::findMethod(memoryInfoClass, "<init>", "()V");
    if (memoryInfoConstructor == nullptr) return -1;

    jobject memoryInfoObject = env->NewObject(memoryInfoClass, memoryInfoConstructor);
    if (memoryInfoObject == nullptr) return -1;

    // Lấy hệ thống service
    jstring serviceString = env->NewStringUTF("activity");
    jobject activityManager = env->CallObjectMethod(jContext,
                                                    JExt::findMethod(env->GetObjectClass(jContext),
                                                                     "getSystemService",
                                                                     "(Ljava/lang/String;)Ljava/lang/Object;"),
                                                    serviceString);
    env->DeleteLocalRef(serviceString);

    if (activityManager == nullptr) return -1;

    // Gọi phương thức để lấy thông tin bộ nhớ

    env->CallVoidMethod(activityManager, getMemoryInfoMethod, memoryInfoObject);


    // Lấy trường availMem và totalMem và đọc giá trị
    jfieldID availMemField = JExt::findField(memoryInfoClass, "availMem", "J");
    jfieldID totalMemField = JExt::findField(memoryInfoClass, "totalMem", "J");

    if (availMemField == nullptr || totalMemField == nullptr) return -1;

    jlong availableMemory = env->GetLongField(memoryInfoObject, availMemField);
    jlong totalMemory = env->GetLongField(memoryInfoObject, totalMemField);

    // Giải phóng các tham chiếu cục bộ
    env->DeleteLocalRef(memoryInfoObject);
    env->DeleteLocalRef(activityManager);
    env->DeleteLocalRef(memoryInfoClass);
    env->DeleteLocalRef(activityManagerClass);
    env->DeleteLocalRef(jContext);

    // Tính phần trăm bộ nhớ đang sử dụng
    int usedMemoryPercentage = 100 * (totalMemory - availableMemory) / totalMemory;

    return usedMemoryPercentage;
}

bool Device::isKeyboardEnabled() {
    JNIEnv *env = JExt::getEnv();
    return env->GetStaticBooleanField(deviceVM.keyboardClass, deviceVM.isRunningkeyboardField);
}

void Device::unlockScreen() {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    env->CallStaticVoidMethod(deviceVM.jClass, deviceVM.unlockScreenMethod, jContext);
}

vector<string> Device::getAllPackages() {
    std::vector<std::string> packageList;

    jobject jContext = getContext();


    JNIEnv *env = JExt::getEnv();

    jclass contextClass = JExt::findClass("android/content/Context");


    jmethodID getPackageManagerMethod = JExt::findMethod(contextClass, "getPackageManager",
                                                         "()Landroid/content/pm/PackageManager;");

    jobject packageManager = env->CallObjectMethod(jContext, getPackageManagerMethod);


    jclass packageManagerClass = env->GetObjectClass(packageManager);

    // 4. Gọi phương thức getInstalledPackages(int flags)
    jmethodID getInstalledPackagesMethod = JExt::findMethod(packageManagerClass,
                                                            "getInstalledPackages",
                                                            "(I)Ljava/util/List;");

    jint flags = 0; // Không cần MATCH_UNINSTALLED_PACKAGES
    jobject packageListObject = env->CallObjectMethod(packageManager, getInstalledPackagesMethod,
                                                      flags);


    // 5. Lấy lớp List
    jclass listClass = JExt::findClass("java/util/List");


    // 6. Lấy phương thức size() và get(int index) từ List
    jmethodID sizeMethod = JExt::findMethod(listClass, "size", "()I");
    jmethodID getMethod = JExt::findMethod(listClass, "get", "(I)Ljava/lang/Object;");

    // 7. Lấy kích thước danh sách
    jint size = env->CallIntMethod(packageListObject, sizeMethod);

    // 8. Duyệt qua danh sách các PackageInfo

    for (jint i = 0; i < size; i++) {
        jobject packageInfo = env->CallObjectMethod(packageListObject, getMethod, i);

        if (packageInfo == nullptr) {
            Common::NLog("packageInfo is null", "getAllPackages", LogLevel::ERROR);
            continue;
        }

        jclass packageInfoClass = env->GetObjectClass(packageInfo);

        jfieldID packageNameField = env->GetFieldID(packageInfoClass, "packageName",
                                                    "Ljava/lang/String;");

        jstring packageName = (jstring) env->GetObjectField(packageInfo, packageNameField);
        const char *packageNameStr = env->GetStringUTFChars(packageName, nullptr);

        packageList.emplace_back(packageNameStr);

        env->ReleaseStringUTFChars(packageName, packageNameStr);
        env->DeleteLocalRef(packageName);
        env->DeleteLocalRef(packageInfoClass);
        env->DeleteLocalRef(packageInfo);
    }

    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(packageManager);
    env->DeleteLocalRef(packageListObject);

    return packageList;
}

bool Device::startActivityByIntent(JSON json, vector<std::pair<string, string>> lstExtra) {
    jobject jContext = getContext();
    JNIEnv *env = JExt::getEnv();

    if (!lstExtra.empty()) {
        JSON jsonExtra;
        for (auto &item: lstExtra) {
            jsonExtra.set(item.first, item.second);
        }
        json.set("extra", jsonExtra.toNlohmannJson());
    }

    jstring jInput = JExt::stringToJstring(json.toString());

    return env->CallStaticBooleanMethod(deviceVM.jClass, deviceVM.startActivityByIntentMethod,
                                        jContext, jInput) == JNI_TRUE;
}


bool Device::isDebugging() {
    JNIEnv *env = JExt::getEnv();
    return env->CallStaticBooleanMethod(deviceVM.jClass, deviceVM.isDebuggingMethod);
}

bool Device::isCanWriteSetting() {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();
    jclass settingsClass = JExt::findClass("android/provider/Settings$System");
    jmethodID canWriteMethod = JExt::findMethod(settingsClass, "canWrite",
                                                "(Landroid/content/Context;)Z", true);

    jboolean result = env->CallStaticBooleanMethod(settingsClass, canWriteMethod, jContext);

    env->DeleteLocalRef(settingsClass);

    // Trả về kết quả
    return result;
}

bool Device::isCanInstallAPK() {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    jclass contextClass = env->GetObjectClass(jContext);
    jmethodID getPackageManagerMethod = env->GetMethodID(contextClass, "getPackageManager",
                                                         "()Landroid/content/pm/PackageManager;");
    jobject packageManager = env->CallObjectMethod(jContext, getPackageManagerMethod);

    jclass packageManagerClass = env->GetObjectClass(packageManager);
    jmethodID canRequestPackageInstallsMethod = env->GetMethodID(packageManagerClass,
                                                                 "canRequestPackageInstalls",
                                                                 "()Z");

    jboolean result = env->CallBooleanMethod(packageManager, canRequestPackageInstallsMethod);

    env->DeleteLocalRef(packageManager);
    env->DeleteLocalRef(packageManagerClass);
    env->DeleteLocalRef(contextClass);

    return result;
}

bool Device::isCanOverlay() {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    // Lấy lớp Settings
    jclass settingsClass = env->FindClass("android/provider/Settings");

    // Lấy phương thức canDrawOverlays(Context)
    jmethodID canDrawOverlaysMethod = env->GetStaticMethodID(settingsClass, "canDrawOverlays",
                                                             "(Landroid/content/Context;)Z");

    // Gọi phương thức canDrawOverlays
    jboolean result = env->CallStaticBooleanMethod(settingsClass, canDrawOverlaysMethod, jContext);

    // Dọn dẹp tham chiếu cục bộ
    env->DeleteLocalRef(settingsClass);

    // Trả về kết quả
    return result == JNI_TRUE;

}

bool Device::isInputMethodEnable() {
    string enabled_input_methods = getSecureSetting("enabled_input_methods", "Secure");

    return Common::strContains(enabled_input_methods, "com.maxcloud.keyboard.latin.LatinIME");
}

bool Device::isWirelessDebuggingEnabled() {
    int result = getSecureSettingInt("adb_wifi_enabled", "Global");

    return result == 1 ? JNI_TRUE : JNI_FALSE;
}

bool Device::isUSBDebuggingEnabled() {
    int result = getSecureSettingInt("adb_enabled", "Global");

    return result == 1 ? JNI_TRUE : JNI_FALSE;
}

bool Device::installApk(const string &path) {
    JNIEnv *env = JExt::getEnv();

    return env->CallStaticBooleanMethod(deviceVM.jClass, deviceVM.installAPKMethod,
                                        JExt::stringToJstring(path),
                                        getContext()) ==
           JNI_TRUE;
}

void Device::openPackageInfo(const string &package) {
    if (package.empty()) {
        Common::NLog("Package name is empty", "openAppInfo");
        return;
    }

    if (!Device::existPackage(package)) {
        Common::NLog("Package not found", "openAppInfo");
        return;
    }

    JNIEnv *env = JExt::getEnv();
    string appName;
    jobject jContext = getContext();
    jstring jPackage = JExt::stringToJstring(package);

    jclass contextClass = JExt::findClass("android/content/Context");

    // Tạo Intent
    jclass intentClass = JExt::findClass("android/content/Intent");

    jmethodID intentConstructor = JExt::findMethod(intentClass, "<init>",
                                                   "(Ljava/lang/String;Landroid/net/Uri;)V");

    jclass settingsClass = JExt::findClass("android/provider/Settings");

    jfieldID actionAppDetailsSettingsField = env->GetStaticFieldID(settingsClass,
                                                                   "ACTION_APPLICATION_DETAILS_SETTINGS",
                                                                   "Ljava/lang/String;");

    jstring actionAppDetailsSettings = (jstring) env->GetStaticObjectField(settingsClass,
                                                                           actionAppDetailsSettingsField);

    // Tạo Uri
    jclass uriClass = JExt::findClass("android/net/Uri");

    jmethodID parseMethod = JExt::findMethod(uriClass, "fromParts",
                                             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Landroid/net/Uri;",
                                             true);

    jstring scheme = env->NewStringUTF("package");
    jobject uri = env->CallStaticObjectMethod(uriClass, parseMethod, scheme, jPackage, nullptr);
    env->DeleteLocalRef(scheme);

    // Khởi tạo Intent
    jobject intent = env->NewObject(intentClass, intentConstructor, actionAppDetailsSettings, uri);

    // Thêm FLAG_ACTIVITY_NEW_TASK
    jmethodID addFlagsMethod = JExt::findMethod(intentClass, "addFlags",
                                                "(I)Landroid/content/Intent;");

    jint flagNewTask = 0x10000000;
    env->CallObjectMethod(intent, addFlagsMethod, flagNewTask);

    jmethodID startActivityMethod = JExt::findMethod(contextClass, "startActivity",
                                                     "(Landroid/content/Intent;)V");

    env->CallVoidMethod(jContext, startActivityMethod, intent);

    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(intentClass);
    env->DeleteLocalRef(settingsClass);
    env->DeleteLocalRef(uriClass);
    env->DeleteLocalRef(uri);
    env->DeleteLocalRef(intent);
    env->DeleteLocalRef(jPackage);
}


bool Device::existPackage(string packageName) {
    vector<string> lstPackage = getAllPackages();

    for (string package: lstPackage) {
        if (Common::strContains(package, packageName))
            return true;
    }
    return false;
}

void Device::openApp(string package) {
    if (package.empty()) {
        Common::NLog("Package name is empty", "openApp");
        return;
    }

    if (!Device::existPackage(package)) {
        Common::NLog("Package not found", "openAppInfo");
        return;
    }

    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    jclass contextClass = env->GetObjectClass(jContext);

    jmethodID getPackageManagerMethod = JExt::findMethod(contextClass, "getPackageManager",
                                                         "()Landroid/content/pm/PackageManager;");
    jobject packageManager = env->CallObjectMethod(jContext, getPackageManagerMethod);

    // Get the launch intent for the package
    jclass packageManagerClass = env->GetObjectClass(packageManager);
    jmethodID getLaunchIntentForPackageMethod = JExt::findMethod(packageManagerClass,
                                                                 "getLaunchIntentForPackage",
                                                                 "(Ljava/lang/String;)Landroid/content/Intent;");
    jstring jPackageName = env->NewStringUTF(package.c_str());
    jobject launchIntent = env->CallObjectMethod(packageManager, getLaunchIntentForPackageMethod,
                                                 jPackageName);

    // Check if the launch intent is not null and start the activity
    jmethodID startActivityMethod = JExt::findMethod(contextClass, "startActivity",
                                                     "(Landroid/content/Intent;)V");

    env->CallVoidMethod(jContext, startActivityMethod, launchIntent);


    // Cleanup local references
    env->DeleteLocalRef(jPackageName);
    env->DeleteLocalRef(launchIntent);
    env->DeleteLocalRef(packageManager);
    env->DeleteLocalRef(packageManagerClass);
    env->DeleteLocalRef(contextClass);
}

std::tuple<int, int, bool> Device::getScreenSize(jobject jContext) {
    if (jContext == nullptr)
        jContext = getContext();

    int width, height;

    JNIEnv *env = JExt::getEnv();


    jclass contextClass = env->GetObjectClass(jContext);

    jmethodID getSystemServiceMethod = JExt::findMethod(
            contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    // Lấy chuỗi WINDOW_SERVICE
    jstring windowServiceString = env->NewStringUTF("window");

    // Lấy đối tượng WindowManager
    jobject windowManager = env->CallObjectMethod(jContext, getSystemServiceMethod,
                                                  windowServiceString);

    // Lấy lớp WindowManager
    jclass windowManagerClass = env->GetObjectClass(windowManager);

    // Lấy phương thức getDefaultDisplay
    jmethodID getDefaultDisplayMethod = JExt::findMethod(windowManagerClass, "getDefaultDisplay",
                                                         "()Landroid/view/Display;");

    // Lấy đối tượng Display
    jobject display = env->CallObjectMethod(windowManager, getDefaultDisplayMethod);

    // Lấy lớp Display
    jclass displayClass = env->GetObjectClass(display);

    // Lấy phương thức getRealSize
    jmethodID getRealSizeMethod = JExt::findMethod(displayClass, "getRealSize",
                                                   "(Landroid/graphics/Point;)V");

    // Tạo đối tượng Point
    jclass pointClass = JExt::findClass("android/graphics/Point");
    jmethodID pointConstructor = JExt::findMethod(pointClass, "<init>", "()V");
    jobject point = env->NewObject(pointClass, pointConstructor);

    // Gọi getRealSize

    env->CallVoidMethod(display, getRealSizeMethod, point);


    // Lấy các giá trị x và y từ Point
    jfieldID xFieldID = env->GetFieldID(pointClass, "x", "I");
    jfieldID yFieldID = env->GetFieldID(pointClass, "y", "I");

    width = env->GetIntField(point, xFieldID);
    height = env->GetIntField(point, yFieldID);


    jmethodID getRotationMethod = env->GetMethodID(displayClass, "getRotation", "()I");
    jint rotation = env->CallIntMethod(display, getRotationMethod);

    bool isLandscape = (rotation == 1 || rotation == 3);

    env->DeleteLocalRef(windowServiceString);
    env->DeleteLocalRef(windowManager);
    env->DeleteLocalRef(display);
    env->DeleteLocalRef(point);
    env->DeleteLocalRef(pointClass);
    env->DeleteLocalRef(displayClass);
    env->DeleteLocalRef(windowManagerClass);

    if (width > height) {
        int temp = width;
        width = height;
        height = temp;
    }

    return std::make_tuple(width, height, isLandscape);
}

int Device::getScreenDpi() {
    JNIEnv *env = JExt::getEnv();

    jclass resourcesClass = JExt::findClass("android/content/res/Resources");

    jmethodID getSystemMethod = JExt::findMethod(resourcesClass, "getSystem",
                                                 "()Landroid/content/res/Resources;", true);

    jobject resources = env->CallStaticObjectMethod(resourcesClass, getSystemMethod);

    jclass resourcesObjClass = env->GetObjectClass(resources);

    jmethodID getDisplayMetricsMethod = JExt::findMethod(resourcesObjClass, "getDisplayMetrics",
                                                         "()Landroid/util/DisplayMetrics;");

    jobject displayMetrics = env->CallObjectMethod(resources, getDisplayMetricsMethod);

    jclass displayMetricsClass = env->GetObjectClass(displayMetrics);

    jfieldID widthPixelsField = env->GetFieldID(displayMetricsClass, "widthPixels", "I");
    jfieldID heightPixelsField = env->GetFieldID(displayMetricsClass, "heightPixels", "I");
    jfieldID densityDpiPixelsField = env->GetFieldID(displayMetricsClass, "densityDpi", "I");

    int dpi = env->GetIntField(displayMetrics, densityDpiPixelsField);

    env->DeleteLocalRef(resources);
    env->DeleteLocalRef(resourcesObjClass);
    env->DeleteLocalRef(displayMetrics);
    env->DeleteLocalRef(displayMetricsClass);
    return dpi;
}


string Device::getModelName() {
    jclass deviceClass = JExt::findClass("com/jaredrummler/android/device/DeviceName");
    jmethodID getDeviceNameMethod = JExt::findMethod(deviceClass, "getDeviceName",
                                                     "()Ljava/lang/String;", true);

    JNIEnv *env = JExt::getEnv();

    jstring jResult = (jstring) env->CallStaticObjectMethod(deviceClass, getDeviceNameMethod);

    return JExt::jstringToString(jResult);
}

string Device::getModelCode() {
    JNIEnv *env = JExt::getEnv();

    jclass buildClass = JExt::findClass("android/os/Build");
    jfieldID modelField = JExt::findField(buildClass, "MODEL", "Ljava/lang/String;", true);

    jstring jModel = (jstring) env->GetStaticObjectField(buildClass, modelField);

    return JExt::jstringToString(jModel);
}

bool Device::createWallpaper(string text, string path) {
    if (Storage::existPath(path)) return true;

    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();

    jstring jText = JExt::stringToJstring(text);

    jstring jPath = JExt::stringToJstring(path);


    return env->CallStaticBooleanMethod(deviceVM.jClass, deviceVM.createWallpaperMethod,
                                        jContext, jText, jPath) == JNI_TRUE;
}

void Device::setClipboard(string text) {
    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();

    jstring jText = JExt::stringToJstring(text);
    env->CallStaticVoidMethod(deviceVM.jClass, deviceVM.setClipboardMethod,
                              jContext, jText);
}

string Device::getClipboard() {
    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();

    jobject jResult = env->CallStaticObjectMethod(deviceVM.jClass, deviceVM.getClipboardMethod,
                                                  jContext);

    string result = JExt::jobjectToString(jResult);

    return result;
}

bool Device::changeUSLanguage() {
    if (!ADB::isConnected) {
        Common::NLog("ADB is not connected", "changeUSLanguage");
        return false;
    }

    ADB::runShell("pm grant com.maxcloud.app android.permission.CHANGE_CONFIGURATION");

    delay(1);

    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();

    bool isSuccess = env->CallStaticBooleanMethod(deviceVM.jClass,
                                                  deviceVM.changeUSLanguageMethod, jContext) ==
                     JNI_TRUE;
    return isSuccess;
}

string Device::getSystemProperty(const string &property) {
    const char *prop_str = property.c_str();

    char value[PROP_VALUE_MAX] = {0};

    int length = __system_property_get(prop_str, value);

    if (length <= 0) {
        return "Unknown";
    }

    return std::string(value, length);
}

bool Device::resetFactory() {
    if (!ADB::isConnected) {
        Common::NLog("ADB is not connected", "resetFactory");
        return false;
    }

    string result = ADB::runShell("dpm set-device-owner " PACKAGE  "/.Core.DeviceOwnerService");

    if (!Common::strContains(result, "Success") &&
        !Common::strContains(result, "Active admin set"))
        return false;

    delay(2);

    jobject jContext = getContext();

    JNIEnv *env = JExt::getEnv();

    bool isSuccess = env->CallStaticBooleanMethod(deviceVM.jClass,
                                                  deviceVM.luoncomotcohoilangaymaiMethod,
                                                  jContext) == JNI_TRUE;
    return isSuccess;
}

string Device::getPackagesInstalledWithIcon() {
    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();

    jobject jResult = env->CallStaticObjectMethod(deviceVM.jClass,
                                                  deviceVM.getPackagesInstalledWithIconMethod,
                                                  jContext);

    string result = JExt::jobjectToString(jResult);


    return result;
}

bool Device::isPermissionGranted(string permission) {
    JNIEnv *env = JExt::getEnv();

    jobject jContext = getContext();

    if(!Common::strContains(permission, "android."))
        permission = "android.permission." + permission;

    jstring jPermission = JExt::stringToJstring(permission);

    bool isSuccess = env->CallStaticBooleanMethod(deviceVM.jClass,
                                                  deviceVM.isPermissionGrantedMethod,
                                                  jContext, jPermission) == JNI_TRUE;
    return isSuccess;

}


string Device::getSecureSetting(const string &name, string subName) {
    if(ADB::isConnected){
        string result = ADB::runShell("settings get " + Common::strToLower(subName) + " " + name);
        return result;
    }

    JNIEnv *env = JExt::getEnv();

    jobject context = getContext();

    jclass contextClass = JExt::findClass("android/content/Context");
    jmethodID getContentResolverMethod = JExt::findMethod(contextClass, "getContentResolver",
                                                          "()Landroid/content/ContentResolver;");
    jobject contentResolver = env->CallObjectMethod(context, getContentResolverMethod);

    // Secure, System, Global
    subName = "android/provider/Settings$" + subName;

    jclass settingsSystemClass = JExt::findClass(subName.c_str());

    jmethodID getStringMethod = JExt::findMethod(settingsSystemClass, "getString",
                                                 "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;",
                                                 true);

    jstring resultModeKey = JExt::stringToJstring(name);

    jobject jResult = env->CallStaticObjectMethod(settingsSystemClass, getStringMethod,
                                                  contentResolver,
                                                  resultModeKey);

    string result = JExt::jobjectToString(jResult);

    env->DeleteLocalRef(contentResolver);
    env->DeleteLocalRef(resultModeKey);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(settingsSystemClass);
    env->DeleteLocalRef(context);

    return result;
}


int Device::getSecureSettingInt(const string &name, string subName) {
    if(ADB::isConnected){
        string result = ADB::runShell("settings get " + Common::strToLower(subName) + " " + name);
        return Common::strToInt(result);
    }

    JNIEnv *env = JExt::getEnv();

    jobject context = getContext();

    jclass contextClass = JExt::findClass("android/content/Context");
    jmethodID getContentResolverMethod = JExt::findMethod(contextClass, "getContentResolver",
                                                          "()Landroid/content/ContentResolver;");
    jobject contentResolver = env->CallObjectMethod(context, getContentResolverMethod);

    // Secure, System, Global
    subName = "android/provider/Settings$" + subName;

    jclass settingsSystemClass = JExt::findClass(subName.c_str());

    jmethodID getIntMethod = JExt::findMethod(settingsSystemClass, "getInt",
                                              "(Landroid/content/ContentResolver;Ljava/lang/String;)I",
                                              true);

    jstring resultModeKey = JExt::stringToJstring(name);

    jint result = env->CallStaticIntMethod(settingsSystemClass, getIntMethod, contentResolver,
                                           resultModeKey);

    env->DeleteLocalRef(contentResolver);
    env->DeleteLocalRef(resultModeKey);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(settingsSystemClass);
    env->DeleteLocalRef(context);

    return result;
}


bool Device::isScreenRotateEnable() {
    int accelerometer_rotation = getSecureSettingInt("accelerometer_rotation", "System");

    return accelerometer_rotation != 0;
}




