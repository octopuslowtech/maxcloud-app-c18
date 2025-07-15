//
// Created by HieuThuHai on 8/16/2024.
//

#include "JExt.h"
#include "Common.h"

JavaVM *JExt::mJavaVM = nullptr;
thread_local JNIEnv *JExt::env = nullptr;

void JExt::initialize(JavaVM *javaVM) {
    mJavaVM = javaVM;
}

JNIEnv *JExt::getEnv() {
    if (env == nullptr) {
        if (mJavaVM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
            int status = mJavaVM->AttachCurrentThread(&env, nullptr);
            if (status < 0) {
                NLOG("Failed to attach thread to JVM");
                return nullptr;
            }
        }
    }
    return env;
}

jclass JExt::findClass(const char *name, bool isGlobal) {
    JNIEnv *env = getEnv();
    jclass localClass = env->FindClass(name);
    if (localClass == nullptr) {
        Common::NLog("Cannot find : " + string(name), "findClass", Common::LogLevel::ERROR);
        return nullptr;
    }
    if (!isGlobal) return localClass;
    jclass globalClass = (jclass) env->NewGlobalRef(localClass); //NewLocalRef
    env->DeleteLocalRef(localClass);
    return globalClass;
}

jmethodID JExt::findMethod(jclass clazz, const char *name, const char *sig, bool isStatic) {
    JNIEnv *env = getEnv();
    jmethodID methodID;

    if (isStatic) {
        methodID = env->GetStaticMethodID(clazz, name, sig);
    } else {
        methodID = env->GetMethodID(clazz, name, sig);
    }

    if (methodID == nullptr) {
        Common::NLog("Cannot find : " + string(name),
                     "findMethod", Common::LogLevel::ERROR);
        return nullptr;
    }

    return methodID;
}

jfieldID JExt::findField(jclass clazz, const char *name, const char *sig, bool isStatic) {
    JNIEnv *env = getEnv();
    jfieldID fieldID;

    if (isStatic) {
        fieldID = env->GetStaticFieldID(clazz, name, sig);
    } else {
        fieldID = env->GetFieldID(clazz, name, sig);
    }
    if (fieldID == nullptr) {
        return nullptr;
    }
    return fieldID;
}

string JExt::jstringToString(jstring jStr) {
    JNIEnv *env = JExt::getEnv();
    if (jStr == nullptr) {
        return "";
    }
    const char *chars = env->GetStringUTFChars(jStr, nullptr);
    string resultString(chars);
    env->ReleaseStringUTFChars(jStr, chars);
    return resultString;
}

jstring JExt::stringToJstring(const string &cString) {
    JNIEnv *env = JExt::getEnv();
    return env->NewStringUTF(cString.c_str());
}


string JExt::jobjectToString(jobject jObject) {
    JNIEnv *env = JExt::getEnv();
    if (jObject == nullptr) {
        return "";
    }
    jclass jStringClass = env->FindClass("java/lang/String");
    if (jStringClass != nullptr && env->IsInstanceOf(jObject, jStringClass)) {
        jstring jStr = static_cast<jstring>(jObject);
        const char *chars = env->GetStringUTFChars(jStr, nullptr);
        string resultString(chars);
        env->ReleaseStringUTFChars(jStr, chars);
        return resultString;
    }
    return "";
}

bool JExt::jobject2bool(jobject jObject) {
    JNIEnv *env = JExt::getEnv();
    jclass jBooleanClass = env->FindClass("java/lang/Boolean");
    if (jBooleanClass != nullptr && env->IsInstanceOf(jObject, jBooleanClass)) {
        jmethodID booleanValueMethod = env->GetMethodID(jBooleanClass, "booleanValue",
                                                        "()Z");
        return env->CallBooleanMethod(jObject, booleanValueMethod);
    }
    return false;
}

int JExt::jint2int(jint input) {
    int intValue = static_cast<int>(input);
    return intValue;
}

