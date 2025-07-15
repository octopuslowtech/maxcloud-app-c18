//
// Created by Admin on 12/26/2024.
//

#include "Media.h"

jobject Media::mediaProjectionManager = nullptr;

jint Media::resultCode = 0;
jobject Media::resultData = nullptr;

int Media::resolutionWidth = 1280;
//int Media::resolutionWidth = 854;
//
int Media::resolutionHeight = 720;
//int Media::resolutionHeight = 480;

static struct {
    jclass mediaProjectClass;

    jclass mediaProjectManagerClass;

    jclass intentClass;

    jmethodID getMediaProjectionMethod;

    jclass virtualDisplayClass;

    jclass handlerClass, looperClass;

    jmethodID handlerConstructor;

    jmethodID getMainLooperMethod;

    jclass mediaProjectionCallbackClass;

    jmethodID registerCallbackMethod, mediaProjectionCallbackContructor;
} mediaVM;

void Media::init() {
    mediaVM.mediaProjectClass = JExt::findClass("android/media/projection/MediaProjection", true);

    mediaVM.mediaProjectManagerClass = JExt::findClass(
            "android/media/projection/MediaProjectionManager", true);

    mediaVM.intentClass = JExt::findClass("android/content/Intent", true);

    mediaVM.getMediaProjectionMethod = JExt::findMethod(mediaVM.mediaProjectManagerClass,
                                                        "getMediaProjection",
                                                        "(ILandroid/content/Intent;)Landroid/media/projection/MediaProjection;");

    mediaVM.virtualDisplayClass = JExt::findClass("android/hardware/display/VirtualDisplay", true);

    mediaVM.handlerClass = JExt::findClass("android/os/Handler", true);

    mediaVM.handlerConstructor = JExt::findMethod(mediaVM.handlerClass, "<init>",
                                                  "(Landroid/os/Looper;)V");

    mediaVM.looperClass = JExt::findClass("android/os/Looper", true);

    mediaVM.getMainLooperMethod = JExt::findMethod(mediaVM.looperClass,
                                                   "getMainLooper",
                                                   "()Landroid/os/Looper;", true);

    mediaVM.mediaProjectionCallbackClass = JExt::findClass(
            "com/maxcloud/app/Extensions/MediaProjectionCallback", true);

    mediaVM.registerCallbackMethod = JExt::findMethod(mediaVM.mediaProjectClass,
                                                      "registerCallback",
                                                      "(Landroid/media/projection/MediaProjection$Callback;Landroid/os/Handler;)V");

    mediaVM.mediaProjectionCallbackContructor = JExt::findMethod(
            mediaVM.mediaProjectionCallbackClass,
            "<init>",
            "()V");
}

bool Media::isRunning() {
    return resultData != nullptr;
}

void Media::initMediaProject(JNIEnv *env, jobject thiz, jint jResultCode, jobject jResultData) {
    resultCode = jResultCode;
    resultData = env->NewGlobalRef(jResultData);

    jclass contextClass = JExt::findClass("android/content/Context");

    jstring jMediaProjectionServiceString = JExt::stringToJstring("media_projection");

    jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, "getSystemService",
                                                        "(Ljava/lang/String;)Ljava/lang/Object;");

    jobject jContext = getContext();

    jobject mediaProjectionManagerLocal = env->CallObjectMethod(jContext, getSystemServiceMethod,
                                                                jMediaProjectionServiceString);

    mediaProjectionManager = env->NewGlobalRef(mediaProjectionManagerLocal);

    env->DeleteLocalRef(jMediaProjectionServiceString);
    env->DeleteLocalRef(contextClass);
}

std::pair<bool, std::string> Media::screenShot(OutputType outputType, int quality) {
    if (!isRunning()) {
        Common::NLog("resultData is null", "startMedia");
        return {false, "resultData is null"};
    }

    bool wasStreaming = isStreaming();

    if (wasStreaming)
        stopStreamVideo();

    JNIEnv *env = JExt::getEnv();

    jobject mediaProjection = env->CallObjectMethod(mediaProjectionManager,
                                                    mediaVM.getMediaProjectionMethod,
                                                    resultCode, resultData);

    AImageReader *imageReader = nullptr;

    AImageReader_new(sWidth, sHeight, AIMAGE_FORMAT_RGBA_8888, 2,
                     &imageReader);

    ANativeWindow *nativeWindow = nullptr;

    AImageReader_getWindow(imageReader, &nativeWindow);

    jobject surface = ANativeWindow_toSurface(env, nativeWindow);

    jmethodID createVirtualDisplayMethod = JExt::findMethod(mediaVM.mediaProjectClass,
                                                            "createVirtualDisplay",
                                                            "(Ljava/lang/String;IIIILandroid/view/Surface;Landroid/hardware/display/VirtualDisplay$Callback;Landroid/os/Handler;)Landroid/hardware/display/VirtualDisplay;");

    jobject virtualDisplay = env->CallObjectMethod(mediaProjection,
                                                   createVirtualDisplayMethod,
                                                   JExt::stringToJstring("chacnosetontaithatlau"),
                                                   sWidth,
                                                   sHeight,
                                                   sDpi,
                                                   16,
                                                   surface,
                                                   nullptr, nullptr);

    env->DeleteLocalRef(surface);

    AImage *image = nullptr;
    AHardwareBuffer *hardwareBuffer = nullptr;

    std::pair<bool, std::string> result = {false, ""};

    try {
        int attempts = 60; // 60 attempts x 50ms = 3 seconds
        while (attempts-- > 0) {
            if (image) {
                AImage_delete(image);
                image = nullptr;
            }

            media_status_t status = AImageReader_acquireLatestImage(imageReader, &image);
            if (status == AMEDIA_OK && image != nullptr) {
                break;
            }
            delay2(60);
        }

        if (image == nullptr)
            throw std::runtime_error("Failed to acquire image after retries");

        media_status_t status = AImage_getHardwareBuffer(image, &hardwareBuffer);

        if (status != AMEDIA_OK || hardwareBuffer == nullptr)
            throw std::runtime_error("Failed to get AHardwareBuffer");

        AHardwareBuffer_Desc desc;
        AHardwareBuffer_describe(hardwareBuffer, &desc);

        void *mappedData = nullptr;

        int status22 = AHardwareBuffer_lock(hardwareBuffer, AHARDWAREBUFFER_USAGE_CPU_READ_RARELY,
                                            -1, nullptr, &mappedData);
        if (status22 != 0 || !mappedData)
            throw std::runtime_error("Failed to lock AHardwareBuffer");

        uint8_t *rgbaData = static_cast<uint8_t *>(mappedData);

        unsigned long jpegSize = 0;

        unsigned char *jpegBuffer = compressImage(rgbaData, jpegSize, desc.width, desc.height,
                                                  desc.stride * 4, quality);

        if (jpegBuffer == nullptr)
            throw std::runtime_error("Failed to compress image");

        AHardwareBuffer_unlock(hardwareBuffer, nullptr);

        if (outputType == OutputType::FILE || outputType == OutputType::BOTH) {
            if (Storage::existPath(SCREENSHOT_PATH))
                Storage::removePath(SCREENSHOT_PATH);

            FILE *outputFile = fopen(SCREENSHOT_PATH, "wb");
            if (!outputFile) {
                throw std::runtime_error("Failed to open file for writing: " SCREENSHOT_PATH);
            }

            size_t written = fwrite(jpegBuffer, 1, jpegSize, outputFile);
            fclose(outputFile);

            if (written != jpegSize) {
                throw std::runtime_error("Failed to write full JPEG data to file");
            }

            result = {Storage::existPath(SCREENSHOT_PATH), SCREENSHOT_PATH};
        }

        if (outputType == OutputType::BASE64 || outputType == OutputType::BOTH) {
            string base64 = convertToBase64(jpegBuffer, jpegSize);
            result = {!base64.empty(), base64};
        }

        tjFree(jpegBuffer);
    } catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: screenShot");
        result = {false, e.what()};
    } catch (...) {
        Common::NLog("Unknown error occurred", "Ex: screenShot");
        result = {false, "Unknown error occurred"};
    }

    if (image) AImage_delete(image);

    if (virtualDisplay) {
        jmethodID releaseMethod = JExt::findMethod(mediaVM.virtualDisplayClass, "release", "()V");
        env->CallVoidMethod(virtualDisplay, releaseMethod);
        env->DeleteLocalRef(virtualDisplay);
    }

    if (mediaProjection) {
        jmethodID stopMethod = JExt::findMethod(mediaVM.mediaProjectClass, "stop", "()V");
        env->CallVoidMethod(mediaProjection, stopMethod);
        env->DeleteLocalRef(mediaProjection);
    }

    if (nativeWindow) ANativeWindow_release(nativeWindow);

//    if (imageReader) {
//        AImageReader_delete(imageReader);
//        imageReader = nullptr;
//    }

    if (wasStreaming)
        startStreamVideo();

    return result;
}

AMediaCodec *Media::mediaCodec = nullptr;

ANativeWindow *Media::inputSurface = nullptr;

jobject Media::virtualDisplay = nullptr;

jobject Media::mediaProjection = nullptr;

std::thread Media::streamingThread;

std::atomic<bool> Media::isStreamingThreadRunning{false};

std::thread Media::detectRotateThread;

std::atomic<bool> Media::isDetectRotateThreadRunning{false};

bool Media::isStreaming() {
    return isStreamingThreadRunning;
}


bool Media::startStreamVideo() {
    if (!isRunning()) {
        Common::NLog("resultData is null", "startMedia", LogLevel::ERROR);
        return false;
    }

    JNIEnv *env = JExt::getEnv();

    jobject mediaProjectionlocal = env->CallObjectMethod(mediaProjectionManager,
                                                         mediaVM.getMediaProjectionMethod,
                                                         resultCode, resultData);

    if (!mediaProjectionlocal) {
        Common::NLog("Failed to get MediaProjection", "Media::startStreamVideo", LogLevel::ERROR);
        return false;
    }

    mediaProjection = env->NewGlobalRef(mediaProjectionlocal);

    // register callback

    jobject mainLooper = env->CallStaticObjectMethod(mediaVM.looperClass,
                                                     mediaVM.getMainLooperMethod);

    jobject handlerObject = env->NewObject(mediaVM.handlerClass, mediaVM.handlerConstructor,
                                           mainLooper);
    if (!handlerObject) {
        Common::NLog("Failed to create Handler", "Media", LogLevel::ERROR);
        return false;
    }

    jobject callbackObject = env->NewObject(mediaVM.mediaProjectionCallbackClass,
                                            mediaVM.mediaProjectionCallbackContructor);
    env->CallVoidMethod(mediaProjection, mediaVM.registerCallbackMethod, callbackObject,
                        handlerObject);

    mediaCodec = AMediaCodec_createEncoderByType("video/avc"); // Sử dụng H264

    AMediaFormat *format = AMediaFormat_new();
    AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_WIDTH, resolutionWidth);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_HEIGHT, resolutionHeight);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_BIT_RATE, 800000); // tang birate
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_FRAME_RATE, 30);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 2);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, 2130708361);
    AMediaFormat_setInt64(format, AMEDIAFORMAT_KEY_REPEAT_PREVIOUS_FRAME_AFTER,
                          200000);

    media_status_t err = AMediaCodec_configure(mediaCodec, format, NULL, NULL,
                                               AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    AMediaFormat_delete(format);

    if (err != AMEDIA_OK) {
        Common::NLog("Hardware codec failed. Trying software codec.", "Media", LogLevel::ERROR);
        AMediaCodec_delete(mediaCodec);
        return false;
    }

    if (AMediaCodec_createInputSurface(mediaCodec, &inputSurface) != AMEDIA_OK || !inputSurface) {
        Common::NLog("Failed to create input surface for MediaCodec", "Media::startStreamVideo",
                     LogLevel::ERROR);
        AMediaCodec_delete(mediaCodec);
        return false;
    }


    if (AMediaCodec_start(mediaCodec) != AMEDIA_OK) {
        Common::NLog("Failed to start MediaCodec", "Media::startStreamVideo", LogLevel::ERROR);
        AMediaCodec_delete(mediaCodec);
        return false;
    }

    jobject surface = ANativeWindow_toSurface(env, inputSurface);

    jmethodID createVirtualDisplayMethod = JExt::findMethod(mediaVM.mediaProjectClass,
                                                            "createVirtualDisplay",
                                                            "(Ljava/lang/String;IIIILandroid/view/Surface;Landroid/hardware/display/VirtualDisplay$Callback;Landroid/os/Handler;)Landroid/hardware/display/VirtualDisplay;");

    jobject virtualDisplayLocal = env->CallObjectMethod(mediaProjection,
                                                        createVirtualDisplayMethod,
                                                        JExt::stringToJstring("vainamnuataselaai"),
                                                        resolutionWidth,
                                                        resolutionHeight,
                                                        sDpi,
                                                        16,
                                                        surface,
                                                        nullptr, nullptr);

    if (!virtualDisplayLocal) {
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        Common::NLog("VirtualDisplay is not created or null", "fetchEncodedFrame", LogLevel::ERROR);
        return false;
    }

    virtualDisplay = env->NewGlobalRef(virtualDisplayLocal);

    env->DeleteLocalRef(surface);

    Media::fetchEncodedFrame();

    return true;
}


std::vector<uint8_t> Media::processFrame(const uint8_t *frameData, size_t frameSize,
                                         const std::vector<uint8_t> &spsData,
                                         const std::vector<uint8_t> &ppsData) {
    std::vector<uint8_t> result;
    std::vector<size_t> nalUnitOffsets;

    if (!frameData || frameSize < 4) {
        Common::NLog("Invalid SPS/PPS: SPS size=" + std::to_string(spsData.size()) + ", PPS size=" +
                     std::to_string(ppsData.size()), "processFrame", LogLevel::ERROR);
        return result;
    }
    if (spsData.empty() || ppsData.empty() || spsData.size() < 4 || ppsData.size() < 4) {
        Common::NLog("Invalid SPS/PPS: SPS size=" + std::to_string(spsData.size()) + ", PPS size=" +
                     std::to_string(ppsData.size()), "processFrame", LogLevel::ERROR);
        return result;
    }

    for (size_t i = 0; i <= frameSize - 4;) {
        if (frameData[i] == 0x00 && frameData[i + 1] == 0x00 &&
            frameData[i + 2] == 0x00 && frameData[i + 3] == 0x01) {
            nalUnitOffsets.push_back(i);
            i += 4;
        } else {
            i++;
        }
    }
    nalUnitOffsets.push_back(frameSize);

    size_t estimatedSize = frameSize + nalUnitOffsets.size() * 4;
    if (!spsData.empty() && !ppsData.empty()) {
        estimatedSize += (spsData.size() + ppsData.size() + 8) * (nalUnitOffsets.size() / 2);
    }
    result.reserve(estimatedSize);

    // Xử lý NAL units
    for (size_t i = 0; i < nalUnitOffsets.size() - 1; i++) {
        size_t startIndex = nalUnitOffsets[i] + 4;
        size_t endIndex = nalUnitOffsets[i + 1];
        if (startIndex >= frameSize || endIndex > frameSize) {
            Common::NLog("Invalid NAL unit range: start=" + std::to_string(startIndex) + ", end=" +
                         std::to_string(endIndex), "processFrame", LogLevel::ERROR);
            continue;
        }

        uint8_t nalType = frameData[startIndex] & 0x1F;
        if (nalType == 5) {
            result.insert(result.end(), {0x00, 0x00, 0x00, 0x01});
            result.insert(result.end(), spsData.begin(), spsData.end());
            result.insert(result.end(), {0x00, 0x00, 0x00, 0x01});
            result.insert(result.end(), ppsData.begin(), ppsData.end());
        }
        result.insert(result.end(), {0x00, 0x00, 0x00, 0x01});
        result.insert(result.end(), frameData + startIndex, frameData + endIndex);
    }

    return result;
}

void Media::fetchEncodedFrame() {
    if (isStreamingThreadRunning.exchange(true)) return;

    streamingThread = std::thread([]() {
        std::vector<uint8_t> spsData;
        std::vector<uint8_t> ppsData;
        AMediaCodecBufferInfo bufferInfo;

        while (isStreamingThreadRunning) {
            ssize_t status = AMediaCodec_dequeueOutputBuffer(mediaCodec, &bufferInfo, 1000);
            if (status == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
                AMediaFormat *newFormat = AMediaCodec_getOutputFormat(mediaCodec);

                if (newFormat) {
                    void *sps = nullptr, *pps = nullptr;
                    size_t spsSize = 0, ppsSize = 0;

                    if (AMediaFormat_getBuffer(newFormat, "csd-0", &sps, &spsSize)) {
                        spsData.assign(static_cast<uint8_t *>(sps),
                                       static_cast<uint8_t *>(sps) + spsSize);
                    }

                    if (AMediaFormat_getBuffer(newFormat, "csd-1", &pps, &ppsSize)) {
                        ppsData.assign(static_cast<uint8_t *>(pps),
                                       static_cast<uint8_t *>(pps) + ppsSize);
                    }

                    AMediaFormat_delete(newFormat);
                }
            }

            if (status < 0 || bufferInfo.size < 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }

            uint8_t *outputBuffer = AMediaCodec_getOutputBuffer(mediaCodec, status, nullptr);

            if (!outputBuffer) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }

            std::vector<uint8_t> processedFrame = processFrame(
                    outputBuffer + bufferInfo.offset,
                    bufferInfo.size,
                    spsData,
                    ppsData
            );

            rtc::binary processedFrameBinary(
                    reinterpret_cast<const std::byte *>(processedFrame.data()),
                    reinterpret_cast<const std::byte *>(processedFrame.data() +
                                                        processedFrame.size())
            );

            auto currentTime = bufferInfo.presentationTimeUs;

            auto lstRTC = RTC::getRTCInstances();
            size_t rtcCount = lstRTC.size();

            if (rtcCount > 1) {
#pragma omp parallel for
                for (size_t i = 0; i < rtcCount; ++i) {
                    lstRTC[i]->sendFrameToTrack(processedFrameBinary, currentTime);
                }
            } else if (rtcCount == 1) {
                lstRTC[0]->sendFrameToTrack(processedFrameBinary, currentTime);
            }

            AMediaCodec_releaseOutputBuffer(mediaCodec, status, false);
        }
    });
}

void Media::startRotationDetector() {
    if (isDetectRotateThreadRunning.exchange(true)) return;

    detectRotateThread = std::thread([]() {
        std::tuple<int, int, bool> screenFirst = Device::getScreenSize();

        bool lastIsRotate = std::get<2>(screenFirst);
        // điện thoại còn xoay ngang thì đời còn khổ :))

        while (isDetectRotateThreadRunning) {
            std::tuple<int, int, bool> screen = Device::getScreenSize();

            bool currentIsRotate = std::get<2>(screen);

            if (currentIsRotate != lastIsRotate) {
                lastIsRotate = currentIsRotate;

                auto lstRTC = RTC::getRTCInstances();
                size_t rtcCount = lstRTC.size();
                for (size_t i = 0; i < rtcCount; ++i) {
                    lstRTC[i]->onScreenRotate(currentIsRotate);
                }
            }

            delay(1);
        }
    });
}

void Media::stopRotationDetector() {
    if (isDetectRotateThreadRunning.exchange(false)) {
        if (detectRotateThread.joinable()) {
            detectRotateThread.join();
        }
    }
}

void Media::stopStreamVideo() {
    JNIEnv *env = JExt::getEnv();

    if (isStreamingThreadRunning.exchange(false)) {
        if (streamingThread.joinable()) {
            streamingThread.join();
        }
    }

    if (mediaCodec) {
        AMediaCodec_stop(mediaCodec);
        AMediaCodec_delete(mediaCodec);
        mediaCodec = nullptr;
    }

    if (virtualDisplay) {
        jmethodID releaseMethod = JExt::findMethod(mediaVM.virtualDisplayClass, "release", "()V");
        env->CallVoidMethod(virtualDisplay, releaseMethod);
        env->DeleteGlobalRef(virtualDisplay);
        virtualDisplay = nullptr;
    }

    if (inputSurface) {
        ANativeWindow_release(inputSurface);
        inputSurface = nullptr;
    }

    if (mediaProjection) {
        jmethodID stopMethod = JExt::findMethod(mediaVM.mediaProjectClass, "stop", "()V");
        env->CallVoidMethod(mediaProjection, stopMethod);
        env->DeleteGlobalRef(mediaProjection);
        mediaProjection = nullptr;
    }
}

unsigned char *
Media::compressImage(const uint8_t *rgbaBuffer, unsigned long &jpegSize, int width, int height,
                     int pitch, int quality) {
    try {
        tjhandle jpegCompressor = tjInitCompress();
        if (!jpegCompressor) {
            throw std::runtime_error("Failed to initialize turboJPEG compressor");
        }

        unsigned char *jpegBuffer = nullptr;

        Common::NLog(
                "Compressing image: " + to_string(width) + "x" + to_string(height) + " pitch: " +
                to_string(pitch));

        int result = tjCompress2(
                jpegCompressor,
                rgbaBuffer,
                width,
                pitch,
                height,
                TJPF_RGBA,
                &jpegBuffer,
                &jpegSize,
                TJSAMP_420,
                quality,
                TJFLAG_FASTDCT
        );

        if (result != 0) {
            std::string errorMsg = tjGetErrorStr();
            tjDestroy(jpegCompressor);
            throw std::runtime_error("TurboJPEG compression error: " + errorMsg);
        }

        tjDestroy(jpegCompressor);
        return jpegBuffer;
    }
    catch (const std::exception &e) {
        Common::NLog("Compressing image error: " + std::string(e.what()));
        return nullptr;
    }
    catch (...) {
        Common::NLog("Compressing image unknown error");
        return nullptr;
    }
}

string Media::convertToBase64(const unsigned char *buffer, size_t size) {
    if (buffer == nullptr || size == 0) {
        throw std::invalid_argument("Invalid buffer or size for Base64 encoding");
    }
    const std::string base64Chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

    std::string base64Str;
    size_t i = 0;
    unsigned char charArray3[3];
    unsigned char charArray4[4];

    while (size--) {
        charArray3[i++] = *(buffer++);
        if (i == 3) {
            charArray4[0] = (charArray3[0] & 0xfc) >> 2;
            charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
            charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
            charArray4[3] = charArray3[2] & 0x3f;

            for (i = 0; i < 4; i++) {
                base64Str += base64Chars[charArray4[i]];
            }
            i = 0;
        }
    }

    if (i > 0) {
        for (size_t j = i; j < 3; j++) {
            charArray3[j] = '\0';
        }

        charArray4[0] = (charArray3[0] & 0xfc) >> 2;
        charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
        charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
        charArray4[3] = charArray3[2] & 0x3f;

        for (size_t j = 0; j < i + 1; j++) {
            base64Str += base64Chars[charArray4[j]];
        }

        while ((i++ < 3)) {
            base64Str += '=';
        }
    }

    return base64Str;
}

void Media::cleanUp() {
    Media::stopStreamVideo();
    Media::stopRotationDetector();
    RTC::cleanUp();
}