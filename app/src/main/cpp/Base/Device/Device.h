#ifndef DEVICE_H
#define DEVICE_H

#include <Base.h>
#include <Storage.h>
#include <JSON.h>
#include <ADB.h>
#include <sys/system_properties.h>

class Device : public Base {
public:
    static struct DeviceVM {
        jclass jClass;
        jmethodID installAPKMethod,
                unlockScreenMethod,
                isNetworkAvailableMethod,
                sendBroadcastAndWaitMethod,
                isDebuggingMethod,
                toggleFlashMethod,
                setWallpaperMethod,
                createWallpaperMethod,
                setClipboardMethod,
                getClipboardMethod,
                startActivityByIntentMethod,
                changeUSLanguageMethod,
                luoncomotcohoilangaymaiMethod,
                getPackagesInstalledWithIconMethod,
                isPermissionGrantedMethod
                ;

        jclass keyboardClass;
        jfieldID isRunningkeyboardField;

        jclass voidServiceClass;
        jfieldID isVoiceServiceEnabledField;


     } deviceVM;

    static void init(jobject jcontext = nullptr);

    static string deviceInfo, deviceName, deviceBuild, deviceNumber, deviceSerial;

    static string currentVersion;

    static bool isROMModified;

    static void sendBroadcast(string action, string package = "",
                              vector<std::pair<std::string, string>> lstExtra = {});

    static string sendBroadcastAndWait(JSON json);

    static string getPackageVersion(string package);

    static void setWallpaper(const string &path);

    static bool isAirplaneModeEnabled();

    static bool isVoiceServiceEnabled();

    static bool isKeyboardEnabled();

    static bool isHasSimCard();

    static bool isNetworkAvailable();

    static void toggleWiFi(bool enable = true);

    static void toggleFlash();

    static int getUsedMemoryPercentage();

    static void unlockScreen();

    static vector<string> getAllPackages();

    static bool
    startActivityByIntent(JSON json,vector<std::pair<string, string>> lstExtra = {});

    static bool isCanWriteSetting();

    static bool isCanInstallAPK();

    static bool isCanOverlay();

    static bool isInputMethodEnable();

    static bool isWirelessDebuggingEnabled();

    static bool isUSBDebuggingEnabled();

    static bool isDebugging();

    static bool installApk(const string &path);

    static void openPackageInfo(const string &package);

    static bool existPackage(string package);

    static void openApp(string package);

    static std::tuple<int, int, bool> getScreenSize(jobject jcontext = nullptr);

    static int getScreenDpi();

    static string getModelName();

    static string getModelCode();

    static bool createWallpaper(string text, string path);

    static void setClipboard(string text);

    static string getClipboard();

    static bool changeUSLanguage();

    static string getSystemProperty(const string &property);

    static bool resetFactory();

    static string getPackagesInstalledWithIcon();

    static bool isPermissionGranted(string permission);

    static string getSecureSetting(const string& name, string subName = "Secure"); // System, Global

    static int getSecureSettingInt(const string& name, string subName = "Secure"); // System, Global

    static bool isScreenRotateEnable();
};


#endif // DEVICE_H