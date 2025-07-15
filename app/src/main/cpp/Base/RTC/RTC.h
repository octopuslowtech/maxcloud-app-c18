//
// Created by Admin on 1/3/2025.
//

#ifndef MAXCLOUD_RTC_H
#define MAXCLOUD_RTC_H

/**
 * +---------------------------+
 * | @author octopuslowtech   |
 * | @date on 15/10/2004      |
 * +---------------------------+
 * ┌─────────────────────────────────────────────────────────────┐
 * │┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐│
 * ││Esc│!1 │@2 │#3 │$4 │%5 │^6 │&7 │*8 │(9 │)0 │_- │+= │|\ │`~ ││
 * │├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┤│
 * ││ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{[ │}] │ BS  ││
 * │├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤│
 * ││ Ctrl │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter  ││
 * │├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┤│
 * ││ Shift  │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│Shift │Fn ││
 * │└─────┬──┴┬──┴──┬┴───┴───┴───┴───┴───┴──┬┴───┴┬──┴┬─────┴───┘│
 * │      │Fn │ Alt │         Space         │ Alt │Win│ octopus  │
 * │      └───┴─────┴───────────────────────┴─────┴───┘ lowtech  │
 * └─────────────────────────────────────────────────────────────┘
 *
 */

#include <Base.h>
#include <ASService.h>
#include <ASAction.h>
#include <Media.h>
#include <Socket.h>
#include <OWrapper.h>

#include <rtc/rtc.h>
#include <rtc/rtc.hpp>
#include <ThreadSafeQueue.h>

#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaCodec.h>
#include "RTCModel.h"

class RTC : public std::enable_shared_from_this<RTC> {
public:
    struct RTCThreadEntry {
        std::string clientId;
        std::shared_ptr<RTC> instance;
        std::thread thread;
    };

    static void addRTCInstance(string clientId, vector<string> lstICE, bool isMedia = true);

    static void removeRTCInstance(string clientId);

    static void cleanUp();

    static bool isEmpty();

    static bool isEmptyStream();

    static void sendSignalToRTCInstance(string clientId, string data);

    static std::vector<std::shared_ptr<RTC>> getRTCInstances();

    explicit RTC(string _clientId, vector<string> lstICE);

    void sendFrameToTrack(rtc::binary sample, uint64_t sampleTimestamp);

    void onScreenRotate(bool isRotate);

    void sendToChannel(ReportControlType type, const std::string &input);

    void sendFileToChannel(ReportControlType type, const std::string &filePath, const std::string &fileName);

    void onTransferSDP(const std::string &data);

    void startProcessing();

    void onClose();

    bool isMediaTransport = true;
private:
    static std::vector<RTCThreadEntry> lstRTCInstance;

    static std::mutex mutexRTC;

    std::mutex fileMutex, controlMutex;

    RTCThreadEntry rtcThreadEntry;

    string clientId;

    vector<string> lstICE;

    std::shared_ptr<rtc::PeerConnection> peerConnection;

    std::shared_ptr<rtc::DataChannel> controlChannel, fileChannel;

    std::chrono::time_point<std::chrono::steady_clock> lastFileReceivedTime;

    std::string fileName;
    size_t fileSize = 0;
    size_t receivedBytes = 0;
    std::vector<uint8_t> fileBuffer;
    string tranferPackageName;

    FileTranferType lastTranferType;



    std::shared_ptr<rtc::Track> videoTrack;

    std::shared_ptr<rtc::RtcpSrReporter> srReporter;

    ThreadSafeQueue<std::string> dataQueue;

    bool isStop = false;

    void setupVideoTrack();

    void setupControlChannel();

    void setupFileChannel();

    static void log_callback(rtc::LogLevel level, const std::string &message);
 };


#endif //MAXCLOUD_RTC_H
