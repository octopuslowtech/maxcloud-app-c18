//
// Created by HieuThuHai on 9/13/2024.
//

#include "GoogleExt.h"

Task GoogleExt::handleLogin() {
    struct LoginStruct {
        int maxInput2FA = 3;
        string loginUid = "";
        string oldOtp;
        int maxGetOtp = 2;
        bool isSelectLoginWithPassword = false;
        bool isSwtichMethod = false;
        bool isLoginStep = false;
    } login;

    if (!account.uid.empty()) {
        login.loginUid = account.uid;
    } else {
        login.loginUid = account.email;
        login.isSwtichMethod = true;
    }

    return Task::FAIL;
}

void GoogleExt::ResetVariable(){}

bool GoogleExt::handleLogout() {
    return false;
}

string GoogleExt::handleScreen(string &xml) {
    return std::string();
}

bool GoogleExt::checkLiveUID(string uid) {
    return false;
}

bool GoogleExt::isUid(string id) {
    return false;
}