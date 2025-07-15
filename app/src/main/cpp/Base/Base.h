//
// Created by Admin on 12/26/2024.
//

#ifndef MAXCLOUD_BASE_H
#define MAXCLOUD_BASE_H

#include <JExt.h>
#include <JSON.h>

#define PACKAGE "com.maxcloud.app"
#define PACKAGE_HELPER "vn.onox.helper"
#define APP_FOLDER "/sdcard/" PACKAGE "/"
#define MEDIA_FOLDER APP_FOLDER "media/"
#define APK_FOLDER APP_FOLDER "apk/"
#define TEMP_FOLDER APP_FOLDER "temp/"
#define DEBUG_FOLDER APP_FOLDER "debug/"
#define SCREENSHOT_PATH APP_FOLDER "screenshot.jpg"
#define MAXCLOUD_ENDPOINT "https://api.maxcloudphone.com/"

using LogLevel = Common::LogLevel;

class Base {
public:
    static std::mutex mutexBase;

    static bool isStop, isRunning;

    static bool isDebug;

    static int sWidth, sHeight, sDpi;

    static string BASE_URL, APP_URL;

    static struct ServiceVM {
        jclass jClass;
        jmethodID getContextMethod;
        jmethodID getInstanceMethod;

        jclass appClass;
        jmethodID getAppContextMethod;

     } baseVM;

    struct DialogVM{
        jclass dialogClass;
        jmethodID showToastMethod;
        jmethodID updateStatusMethod;
        jmethodID updateDeviceNameMethod;
        jmethodID updateColorStatusMethod;
        jmethodID getStatusMethod;
        jmethodID updateDbMethod;
        jmethodID updateFloatingViewMethod;
    } static dialogVM;

    static string jwt;

    static void init();

    static void initView();

    static jobject getContext();

    static jobject getInstance();

    static void updateLine(string content, bool isToast = false);

    static void updateDeviceName(string newName);

    static string getLastStatus();

    static void showToast(string content);

    static void updateColorStatus(bool isRunning);

    static void delay(int timeout = 1, string message = "", bool isToast = false);

    static void delay(int timeFrom, int timeTo, string message = "");

    static void delay2(long timeout = 1000, string message = "");

    static void updateFloatingView(bool isShow);
};


#endif //MAXCLOUD_BASE_H
