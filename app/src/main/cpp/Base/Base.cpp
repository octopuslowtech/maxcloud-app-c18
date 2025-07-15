//
// Created by Admin on 12/26/2024.
//

#include "Base.h"

std::mutex Base::mutexBase;

string Base::jwt = "";

bool Base::isStop = false;

bool Base::isRunning = false;

bool Base::isDebug = false;

int Base::sWidth = 0;

int Base::sHeight = 0;

int Base::sDpi = 0;


string Base::BASE_URL = "";

string Base::APP_URL = "";

Base::ServiceVM Base::baseVM = {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
};

void Base::init() {
    if (baseVM.jClass != nullptr) return;

    baseVM.jClass = JExt::findClass("com/maxcloud/app/Core/MainService", true);

    baseVM.getInstanceMethod = JExt::findMethod(baseVM.jClass, "getInstance",
                                                "()Landroid/accessibilityservice/AccessibilityService;",
                                                true);

    jclass jContextWrapper = JExt::findClass("android/content/ContextWrapper");

    baseVM.getContextMethod = JExt::findMethod(jContextWrapper, "getApplicationContext",
                                               "()Landroid/content/Context;");


    baseVM.appClass = JExt::findClass("com/maxcloud/app/App", true);

    baseVM.getAppContextMethod = JExt::findMethod(baseVM.appClass, "getContext",
                                                "()Landroid/content/Context;",
                                                true);

    dialogVM.dialogClass = JExt::findClass("com/maxcloud/app/Core/DialogService", true);

    dialogVM.showToastMethod = JExt::findMethod(dialogVM.dialogClass, "showToast",
                                                "(Ljava/lang/String;)V",
                                                true);

    dialogVM.updateStatusMethod = JExt::findMethod(dialogVM.dialogClass, "updateStatus",
                                                   "(Ljava/lang/String;)V",
                                                   true);

    dialogVM.getStatusMethod = JExt::findMethod(dialogVM.dialogClass, "getStatus",
                                                "()Ljava/lang/String;",
                                                true);

    dialogVM.updateColorStatusMethod = JExt::findMethod(dialogVM.dialogClass, "updateColorStatus",
                                                        "(Z)V",
                                                        true);


    dialogVM.updateDbMethod = JExt::findMethod(dialogVM.dialogClass, "updateDb",
                                               "(Lcom/google/firebase/database/DatabaseReference;Ljava/lang/String;Ljava/lang/String;)V",
                                               true);

    dialogVM.updateDeviceNameMethod = JExt::findMethod(dialogVM.dialogClass, "updateDeviceName",
                                                       "(Ljava/lang/String;)V",
                                                       true);

    dialogVM.updateFloatingViewMethod = JExt::findMethod(dialogVM.dialogClass, "updateFloatingView",
                                                         "(Z)V",
                                                         true);
}

jobject Base::getContext() {
    JNIEnv *env = JExt::getEnv();

    jobject contextObj = nullptr;

    jobject _instance = getInstance();

    if(_instance != nullptr){
        contextObj = env->CallObjectMethod(_instance, baseVM.getContextMethod);
    }
    else{
        contextObj = env->CallStaticObjectMethod(baseVM.appClass, baseVM.getAppContextMethod);
    }

    if (contextObj == nullptr) {
        Common::NLog("contextObj is null", "getContext", Common::LogLevel::ERROR);
        return nullptr;
    }

    return contextObj;
}

jobject Base::getInstance() {
    JNIEnv *env = JExt::getEnv();
    return env->CallStaticObjectMethod(baseVM.jClass, baseVM.getInstanceMethod);
}

Base::DialogVM Base::dialogVM = {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
};

static bool isSetupView = false;

void Base::initView() {
    if (isSetupView) return;

    JNIEnv *env = JExt::getEnv();

    jmethodID initViewMethod = JExt::findMethod(dialogVM.dialogClass, "initView", "()V", true);

    env->CallStaticVoidMethod(dialogVM.dialogClass, initViewMethod);

    isSetupView = true;
}

void Base::updateLine(string content, bool isToast) {
    JNIEnv *env = JExt::getEnv();

    if (!isSetupView) {
        Base::showToast(content);
        return;
    }

    jstring jValue = JExt::stringToJstring(content);
    if (jValue == nullptr) return;

    env->CallStaticVoidMethod(dialogVM.dialogClass, dialogVM.updateStatusMethod, jValue);

    env->DeleteLocalRef(jValue);

    if (isToast)
        Base::showToast(content);
}

void Base::updateDeviceName(string newName) {
    JNIEnv *env = JExt::getEnv();

    jstring jValue = JExt::stringToJstring(newName);
    if (jValue == nullptr) return;

    env->CallStaticVoidMethod(dialogVM.dialogClass, dialogVM.updateDeviceNameMethod, jValue);
    env->DeleteLocalRef(jValue);
}

string Base::getLastStatus() {
    if (!isSetupView) return "";
    JNIEnv *env = JExt::getEnv();
    jstring jValue = (jstring) env->CallStaticObjectMethod(dialogVM.dialogClass,
                                                           dialogVM.getStatusMethod);
    return JExt::jstringToString(jValue);
}

void Base::showToast(string content) {
    JNIEnv *env = JExt::getEnv();

    jstring jValue = JExt::stringToJstring(content);
    if (jValue == nullptr) return;

    env->CallStaticVoidMethod(dialogVM.dialogClass, dialogVM.showToastMethod, jValue);
    env->DeleteLocalRef(jValue);
}

void Base::updateColorStatus(bool isRunning) {
    if (!isSetupView) return;
    JNIEnv *env = JExt::getEnv();
    auto jIsRunning = static_cast<jboolean>(isRunning);
    env->CallStaticVoidMethod(dialogVM.dialogClass, dialogVM.updateColorStatusMethod, jIsRunning);
}

void Base::delay(int timeout, string message, bool isToast) {
    if (timeout == 0) return;

    if (isToast)
        showToast(message);
    while (timeout > 0 && !isStop) {
        if (!message.empty()) {
            if (timeout == 0) {
                updateLine(message);
            } else {
                updateLine(message + " - " + to_string(timeout));
            }
        }
        delay2(1000);
        timeout--;
    }
    if (!message.empty())
        updateLine(message);
}

void Base::delay(int timeFrom, int timeTo, string message) {
    int timeout = Common::ranNumber(timeFrom, timeTo);
    delay(timeout, message);
}

void Base::delay2(long timeout, string message) {
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

void Base::updateFloatingView(bool isShow) {
    JNIEnv *env = JExt::getEnv();


    auto jIsShow = static_cast<jboolean>(isShow);

    env->CallStaticVoidMethod(dialogVM.dialogClass, dialogVM.updateFloatingViewMethod, jIsShow);
}



