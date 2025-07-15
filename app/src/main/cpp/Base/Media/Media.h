//
// Created by Admin on 12/26/2024.
//

#ifndef MAXCLOUD_MEDIA_H
#define MAXCLOUD_MEDIA_H

#include <Base.h>
#include <RTC.h>
#include <Storage.h>
#include <Device.h>
#include <turbojpeg.h>

#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <media/NdkImage.h>
#include <media/NdkImageReader.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaExtractor.h>

#include <rtc/rtc.h>
#include <rtc/rtc.hpp>

using binary = rtc::binary;

class Media : public Base {
public:
    enum class OutputType {
        FILE,
        BASE64,
        BOTH
    };

    static int resolutionWidth;

    static int resolutionHeight;

    static jobject mediaProjectionManager;

    static jint resultCode;

    static jobject resultData;

    static void init();

    static JNIEXPORT void JNICALL
    initMediaProject(JNIEnv *env, jobject thiz, jint jResultCode, jobject jResultData);

    static bool isRunning();

    static std::pair<bool, std::string>
    screenShot(OutputType outputType = OutputType::FILE, int quality = 20);

    static bool isStreaming();

    static bool startStreamVideo();

    static void startRotationDetector();

    static void stopRotationDetector();

    static void cleanUp();

    static void stopStreamVideo();
private:
    static AMediaCodec *mediaCodec;
    static ANativeWindow *inputSurface;
    static jobject virtualDisplay;
    static jobject mediaProjection;

    static std::thread streamingThread; // Luồng để chạy startStreamVideo
    static std::atomic<bool> isStreamingThreadRunning; // Cờ kiểm soát luồng

    static std::thread detectRotateThread;
    static std::atomic<bool> isDetectRotateThreadRunning;


    static std::vector<uint8_t> processFrame(const uint8_t *frameData, size_t frameSize,
                                             const std::vector<uint8_t> &spsData,
                                             const std::vector<uint8_t> &ppsData);
    static void fetchEncodedFrame();

    static unsigned char *
    compressImage(const uint8_t *rgbBuffer, unsigned long &jpegSize, int width, int height,
                  int pitch, int quality);

    static string convertToBase64(const unsigned char *buffer, size_t size);

};


#endif //MAXCLOUD_MEDIA_H