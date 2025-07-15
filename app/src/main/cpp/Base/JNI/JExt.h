//
// Created by HieuThuHai on 8/16/2024.
//

#ifndef MAXCLOUD_JEXT_H
#define MAXCLOUD_JEXT_H

#include <Common.h>

class JExt {
public:
    static JavaVM* mJavaVM;

    static void initialize(JavaVM* javaVM);

    static jclass findClass(const char* name,  bool isGlobal = false);

    static jmethodID findMethod(jclass clazz, const char* name, const char* sig, bool isStatic = false);

    static jfieldID  findField(jclass clazz, const char* name, const char* sig, bool isStatic = false);

    static JNIEnv* getEnv();

    static std::string jstringToString(jstring jStr);

    static jstring stringToJstring(const std::string& cString);

    static std::string jobjectToString(jobject jObject);

    static bool jobject2bool(jobject jObject);


    static int jint2int(jint input);
private:
    static thread_local JNIEnv* env;

};



#endif //MAXCLOUD_JEXT_H
