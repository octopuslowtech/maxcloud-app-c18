//
// Created by HieuThuHai on 8/17/2024.
//

#ifndef MAXCLOUD_SERVICEMODEL_H
#define MAXCLOUD_SERVICEMODEL_H

#endif //MAXCLOUD_SERVICEMODEL_H

#include <string>
#include <vector>
#include <jni.h>

#pragma once
using std::string, std::vector;

enum ResultScreen {
    NOMAP,
    CONTINUE,
    DONE,
    ERROR,
};

enum class ProxyType {
    WIFI = 0,
    MOBILE_4G = 1,
    PROXY = 2
};

struct Proxy {
    string proxyIP;
    string proxyPort;
    string proxyUser;
    string proxyPass;
    int expiredTime;

    Proxy(string proxyIP = "",
          string proxyPort = "",
          string proxyUser = "",
          string proxyPass = "",
          int expiredTime = 0)
            :
            proxyIP(proxyIP),
            proxyPort(proxyPort),
            proxyUser(proxyUser),
            proxyPass(proxyPass),
            expiredTime(expiredTime) {
    }
};

struct AppSetting {
    string proxyStr;
    bool isLowBrightness;
    bool isMuteAudio;
    Proxy proxy;
    ProxyType proxyType;

    AppSetting() {
        proxyStr = "";
        proxy = {};
        isLowBrightness = false;
        isMuteAudio = false;
        proxyType = ProxyType::WIFI;
    }
};

enum class SocialType {
    TIKTOK = 0,
    FACEBOOK = 1,
    TWITTER = 2,
    INSTAGRAM = 3,
    THREADS = 4,
    GOOGLE = 5,

    // UNKNOW
    UNKNOWN = 9,
    ANOTHER = 10,
};

struct Node {
    string id;
    string package;
    string className;
    string text;
    string contentDesc;
    string resourceId;
    bool checkable;
    bool checked;
    bool clickable;
    bool enabled;
    bool focusable;
    bool scrollable;
    bool editable;
    bool longClickable;
    bool selected;
    bool password;
    bool displayed;
    string bounds;

    int width;
    int height;
    int centerX;
    int centerY;
    int left;
    int top;
    int right;
    int bottom;

    jobject nodeInfo;

    string xpath;

    Node(int centerX = 0,
           int centerY = 0)
            :
            centerX(centerX),
            centerY(centerY) {
    }

public:
    bool empty() {
        return className.empty() &&
                                       text.empty() &&
                                       contentDesc.empty() &&
                                       resourceId.empty();
    }

    bool operator==(const Node &other) const {
        return className == other.className &&
               text == other.text &&
               contentDesc == other.contentDesc &&
               resourceId == other.resourceId &&
               displayed == other.displayed &&
               checkable == other.checkable &&
               checked == other.checked &&
               clickable == other.clickable &&
               scrollable == other.scrollable &&
               enabled == other.enabled &&
               focusable == other.focusable &&
               longClickable == other.longClickable &&
               password == other.password &&
               selected == other.selected &&
               width == other.width &&
               height == other.height &&
               //centerX == other.centerX &&
               //centerY == other.centerY &&
               left == other.left &&
               top == other.top &&
               right == other.right &&
               bottom == other.bottom;
    }
};

struct ScreenNode {
    vector<string> lstXpath;
};

struct ScreenAction {
    string xpath = "";
    string action = "";
    int delayS = 1; //nghỉ sau khi tương tác với element
public:
};

struct Screen {
    std::vector<ScreenNode> lstParent;
    std::vector<ScreenAction> lstAction;
    string screenName = "";
    int countElement = 0;
    SocialType socialType;
};

enum class Task {
    NONE,
    DONE,
    FAIL,
};



enum class Perform{
    BACK = 1,
    HOME = 2,
    RECENT = 3,
    NOTIFICATIONS = 4,
    QUICK_SETTINGS = 5,
    POWER_DIALOG = 6,
    LOCK_SCREEN = 8,
    TAKE_SCREENSHOOT = 9,
};

enum class Keyboard {
    ENTER,
    CLEAR,
    INPUT,
    MATCHA,
    HIDE,
    SELECT_ALL,
    COPY_TEXT,
    ARROW_RIGHT,
    ARROW_LEFT,
    MOVE_LAST
};
