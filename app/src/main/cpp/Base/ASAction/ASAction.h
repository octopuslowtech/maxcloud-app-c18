//
// Created by Admin on 12/29/2024.
//

#ifndef MAXCLOUD_ASACTION_H
#define MAXCLOUD_ASACTION_H

#include <ADB.h>
#include <ASService.h>
#include <Device.h>
#include <Socket.h>


class ASAction : public Base {
public:
    static void initStatic();

    static void initPermission();

    static void enableMediaProject();

    static void enableVPNService();

    static void enableKeyboard();

    static void enableOverlay();

    static void enableIgnoreBatteryOptimize();

    static void enableLSPosed();

    static bool installApkFromS3(string packageName,string path);

    static bool
    installApkAction(string path, string message = "", string package = "", bool isAuto = true);

    static bool openAppInfoAction(const string &package, const string &title = "");

    static bool closeAppAction(const string &package, string title = "");

    static bool clearDataAppAction(const string &package, string title = "");

    static void changeAirplaneMode();

    static bool setupPhone(const Proxy &proxy);

    static bool setupNetwork();

    static jboolean isDeviceLoggedIn(JNIEnv *env, jobject thiz, jobject jcontext);

    static jboolean isLoginToken(JNIEnv *env, jobject thiz, jobject jcontext);

    static jboolean startServiceNative(JNIEnv *env, jobject thiz);

    static jstring
    loginDevice(JNIEnv *env, jobject thiz, jobject jcontext, jstring jName, jstring jToken,
              jstring jSerial, jstring jVersion, jstring jSdk, jboolean isSaveDevice);

    static void onConnected(JNIEnv *env, jobject thiz);

    static void onDisconnected(JNIEnv *env, jobject thiz);

    static jstring getProductNumber(JNIEnv *env, jobject thiz);

    static void factoryResetAction();

    static bool backupApp(const string& packageName, string pathOutput = "");

    static bool restoreApp(const string& packageName, string pathInput);

    static void changeDeviceByROM();

    static bool changeDevicebyLSPosed();

    static void disableScreenRotate();

    static void switchLabanKey();

private:
    static void appendBuildProp(string &content, vector<string> lstKey , const string &value);
};


#endif //MAXCLOUD_ASACTION_H
