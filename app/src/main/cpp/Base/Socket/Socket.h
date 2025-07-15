//
// Created by octopus on 6/5/25.
//

#ifndef MAXCLOUD_SOCKET_H
#define MAXCLOUD_SOCKET_H

#include <Base.h>
#include <Storage.h>
#include <Device.h>
#include <ASService.h>
#include <mutex>

#include "rtc/websocket.hpp"
#include <chrono>
#include <thread>
#include <atomic>
#include <future>

#define MAX_RECONNECT_ATTEMPTS 50
#define RECONNECT_DELAY_SECONDS 10
#define PING_PONG_DELAY_SECONDS 60
#define PING_PONG_TIMEOUT 60 * 1000

class Socket : public Base {
public:
    static void connectSocket();

    static bool sendMessage(string target, vector<string> arguments = {});

    static void forceReconnect();

    static jobject getFirebase();

    static void updateFirebase(const string &key, const string &value);

    static void initFirebase();

private:
    static void onOpen();

    static void onMessage(string message);
    
    static void attemptReconnect();

    static bool sendPing();
    
    static void startPingThread();
    
    static void stopPingThread();

    static int reconnectAttempts;

    static std::shared_ptr<rtc::WebSocket> webSocket;

    static jobject firebaseObject;
    
    static std::atomic<bool> isReconnecting;
    
    static std::mutex reconnectMutex;
    
    static std::thread reconnectThread;
    
    static std::atomic<bool> isPingThreadRunning;
    
    static std::thread pingThread;
    
    static std::chrono::time_point<std::chrono::steady_clock> lastPongReceived;

    static bool lastPong;
};


#endif //MAXCLOUD_SOCKET_H
