#include "Social.h"

Social::SocialInfo Social::social;

Social::Account Social::account;

Social::Action Social::action;

Social::Setting Social::setting;

Social::JobSetting Social::jobSetting;

vector<Social::Action> Social::lstAction;

int Social::countRegisterAccount = 0;

std::chrono::time_point<high_resolution_clock> Social::startTimeAccount = high_resolution_clock::now();
std::chrono::time_point<high_resolution_clock> Social::startTimeAction = high_resolution_clock::now();

int Social::limitTimeRun = 0;
int Social::limitTimeAction = 0;

void Social::resetVariable() {
    currentAction = "";
    limitTimeRun = 0;
    limitTimeAction = 0;
    startTimeProxy = std::chrono::time_point<std::chrono::high_resolution_clock>();
    startTimeAccount = std::chrono::time_point<std::chrono::high_resolution_clock>();
    startTimeAction = std::chrono::time_point<std::chrono::high_resolution_clock>();
}

static vector<Social::SocialInfo> lstSocial = {
        {SocialType::TIKTOK,    "Tiktok",    "com.zhiliaoapp.musically", "33.6.1",         false},
        {SocialType::FACEBOOK,  "Facebook",  "com.facebook.katana",      "428.0.0.26.108", true},
        {SocialType::INSTAGRAM, "Instagram", "com.instagram.android",    "361.0.0.0.84",   true},
        {SocialType::GOOGLE,    "Twitter",   "",    "",   false},
};


bool Social::initSocial(const string &action) {
    if (Common::strContains(action, "tiktok"))
        social.type = SocialType::TIKTOK;
    else if (Common::strContains(action, "facebook"))
        social.type = SocialType::FACEBOOK;
    else if (Common::strContains(action, "twitter"))
        social.type = SocialType::TWITTER;
    else if (Common::strContains(action, "instagram"))
        social.type = SocialType::INSTAGRAM;
    else if (Common::strContains(action, "threads"))
        social.type = SocialType::THREADS;
    else if (Common::strContains(action, "google"))
        social.type = SocialType::GOOGLE;
    else
        return true;

    auto it = std::find_if(lstSocial.begin(), lstSocial.end(),
                           [](const Social::SocialInfo &info) {
                               return info.type == social.type;
                           });


    if (it == lstSocial.end()) {
        delay(5, "Not-found Social");
        return false;
    }

    social = *it;

    social.path = APP_FOLDER + Common::strToLower(social.name) + "/";
    social.rootPath = "/data/data/" + Common::strToLower(social.package) + "/";

    if (!isDebug && social.isRootRequired && !ADB::isRoot) {
        while (!isStop) {
            updateLine("Please root device!!!", true);
            delay(5);
        }
        return false;
    }

    if (!Storage::existPath(social.path))
        Storage::createFolder(social.path);

    if (social.type == SocialType::TIKTOK && !existPackage(social.package))
        social.package = "com.ss.android.ugc.trill";

    if (!social.package.empty() &&  !existPackage(social.package)) {
        if (social.type == SocialType::TIKTOK
            || social.type == SocialType::GOOGLE) {
            delay(20, "Please install " + social.name + " app !!!");
            return false;
        }

        if (!installApkFromS3(social.package, Common::strToLower(social.name))) {
            delay(20, "Please install " + social.name + " app !!!");
            return false;
        }
    }

    if (!social.version.empty() && getPackageVersion(social.package) != social.version) {
        showToast("ur running old social version, please update to " + social.version);
    }

    lstScreenSocial.clear();

    for (const auto &screen: lstScreen) {
        if (screen.socialType == SocialType::ANOTHER
            || screen.socialType == social.type) {
            lstScreenSocial.push_back(screen);
        }
    }

    return true;
}

bool Social::getAccount(const string &accountId) {
    auto start = high_resolution_clock::now();
    int timeout = 60;
    while (!isStop) {
        if (Common::hasExceededTimeout(start, timeout)) {
            isStop = true;
            break;
        }
        updateLine("Get Account");

        JSON json;

        json.set("socialType", static_cast<int>(social.type));

        if (!accountId.empty())
            json.set("id", accountId);

        IResult response = OCRequest("socialApp/get-account/", json.toString());

        if (!response.succeeded) {
            delay(10, response.message);
            if (accountId.length() > 0)
                return false;
            continue;
        }

        if (!account.init(response.json)) {
            saveStatusAcc("Account", "init failed", 5);
            return false;
        }

        if (social.type == SocialType::TIKTOK) {
            IResult responseScript = OCRequest("tiktokapp/get-uid/");
            if (!responseScript.succeeded) {
                saveStatusAcc("Account", "Get UID Logined fail", 5);
                return false;
            }

            account.lstUid = responseScript.json.getInstance().get<vector<string >>();
        }

        return true;
    }
    return false;
}

bool Social::getSetting() {
    IResult response = OCRequest(
            "socialApp/get-setting?socialType=" + to_string(static_cast<int>(social.type)));

    if (!response.succeeded) {
        saveStatusAcc("Setting", "Get Setting fail", 5);
        return false;
    }

    if (!setting.init(response.json)) {
        saveStatusAcc("Setting", "init failed", 5);
        return false;
    }

    limitTimeRun = setting.limitTimeRun;
    limitTimeAction = setting.limitTimeAction;

    return true;
}

bool Social::getJobSetting() {
    IResult response = OCRequest(
            "socialApp/get-job-setting?socialType=" + to_string(static_cast<int>(social.type)) +
            "&accountId=" + account.id);

    if (!response.succeeded) {
        saveStatusAcc("JobSetting", "Get fail", 5);
        return false;
    }

    if (!jobSetting.init(response.json)) {
        saveStatusAcc("JobSetting", "init failed", 5);
        return false;
    }

    return true;
}

bool Social::getScript() {
    updateLine("Get Script");

    try {
        string uri =
                "socialApp/get-script?socialType=" + to_string(static_cast<int>(social.type)) +
                "&accountId=" +
                account.id;

        IResult response = OCRequest(uri);

        if (!response.succeeded) {
            if (Common::strContainsLower(response.message, "Not-found Script")) {
                saveStatusAcc("Script", "Get script failed", 10);
            }
            return false;
        }

        for (const auto &item: response.json.getInstance()) {
            Action action;

            action.actionName = item["name"];

            action.scriptType = item["actionType"];

            string jsonString = item["jsonConfig"].get<string>();

            action.jsonConfig = JSON(jsonString);

            lstAction.push_back(action);
        }

        if (lstAction.empty()) {
            saveStatusAcc("Script", "Notfound Script Action", 5);
            return false;
        }

        return true;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: getScript", LogLevel::ERROR);
        return false;
    }
}

void Social::saveStatusAcc(string title, const string &msg, int delayTime, Task task) {
    title = title.empty() ? action.actionName : title;
    string fullMsg = (task == Task::FAIL ? "(0)" : "(1)") + title + ": " + msg;

    if (!Common::strContains(account.msgAccount, fullMsg))
        account.msgAccount += fullMsg + "|";

    delay(delayTime, title + ": " + msg);
}

void Social::handleInteractSocial(std::function<Task()> funcLogin,
                                  std::function<ResultScreen()> funcExecuteAction,
                                  bool isEarnMoney, const string &uidReq) {
    bool isLoginOne = false;

    while (!isStop && !isLoginOne) {
        resetVariable();

        closeSocial();

        if (!uidReq.empty())
            isLoginOne = true;

        if (!getAccount(uidReq)) continue;

        if (isStopped()) continue;

        if (!getSetting()) continue;

        if (isStopped()) continue;

        if (!setupPhone(
                Proxy{account.proxyIP, account.proxyPort, account.proxyUser, account.proxyPass}))
            continue;

        if (isStopped()) continue;

        if (!setupNetwork()) {
            continue;
        }

        if (isStopped()) continue;

        if (social.isRootRequired) {
            if (!changeDevice()) {
                handleUpdateSocial(false);
                continue;
            }

            if (!restoreAccount()) {
                handleUpdateSocial(false);
                continue;
            }

            ADB::grantPermission(social.package, {"READ_EXTERNAL_STORAGE", "WRITE_EXTERNAL_STORAGE",
                                                  "CAMERA", "RECORD_AUDIO"});
        }

        startTimeAccount = high_resolution_clock::now();
        startTimeAction = high_resolution_clock::now();

        if (isStopped()) continue;

        Task taskSetup = funcLogin();

        if (taskSetup == Task::FAIL) goto endLoop;

        if (taskSetup == Task::NONE) continue;

        lstAction.clear();

        if (isEarnMoney) {
            Action action{};
            action.actionName = "Earn Money";
            action.scriptType = "EARN_MONEY";
            lstAction.push_back(action);
        } else {
            if (!getScript()) goto endLoop;
        }

        while (!isStopped()) {
            if (lstAction.empty()) {
                delay(5, "Done all action");
                break;
            }

            startTimeAction = high_resolution_clock::now();

            action = lstAction.front();
            lstAction.erase(lstAction.begin());
            currentAction = Common::strReplace(action.actionName, " - Copy", "");

            if (action.scriptType.empty()) continue;

            ResultScreen resultAction = funcExecuteAction();

            if (resultAction == ERROR)
                saveStatusAcc(currentAction, "Error", 0, Task::FAIL);

            if (resultAction == DONE)
                saveStatusAcc(currentAction, "Done", 0, Task::DONE);

            if (resultAction == ERROR || isStopped())
                break;
        }

        endLoop:
        handleUpdateSocial(true);
    }
}


void Social::getInfoAccountFromFile() {
    try {
        if (social.type == SocialType::FACEBOOK) {
            string authentication = ADB::runShell("cat " + social.rootPath + "/authentication");

            std::regex tokenPattern("EAAA([a-zA-Z0-9]+)");
            std::smatch tokenMatch;
            if (std::regex_search(authentication, tokenMatch, tokenPattern)) {
                if (account.token.empty())
                    account.token = authentication.substr(tokenMatch.position(0),
                                                          tokenMatch.length(0));
            }

            std::string jsonData = "";
            std::regex jsonPattern("\\[(.*?)\\]");
            std::smatch jsonMatch;
            if (std::regex_search(authentication, jsonMatch, jsonPattern)) {
                jsonData =
                        "{\"data\": " +
                        authentication.substr(jsonMatch.position(0), jsonMatch.length(0)) +
                        "}";
            }

            JSON inputObject{jsonData};
            std::string tempCookie = "";
            for (auto &element: inputObject.getInstance("data")) {
                if (element.is_object()) {
                    tempCookie += element["name"].get<std::string>() + "=" +
                                  element["value"].get<std::string>() + ";";
                    if (element["name"].get<std::string>().find("c_user") != std::string::npos) {
                        if (account.uid.empty())
                            account.uid = element["value"];
                    }
                }
            }

            if (!tempCookie.empty()) {
                if (account.cookie.empty())
                    account.cookie = tempCookie;
            }

            std::string input = ADB::runShell(
                    "cat " + social.rootPath + "app_light_prefs/com.facebook.katana/logged_in_" +
                    account.uid);
            input = std::regex_replace(input, std::regex("[\\u0000-\\u001F]+"), " ");

            // Lấy email
            std::regex emailPattern("emails\\s+([a-zA-Z0-9\\_\\@\\-\\.]+)");
            std::smatch emailMatch;
            if (std::regex_search(input, emailMatch, emailPattern)) {
                if (account.email.empty())
                    account.email = emailMatch[1].str();
            }

            // Lấy avatar URL
//            std::regex picPattern("pic_square\\s+([^\\s]+)");
//            std::smatch picMatch;
//            if (std::regex_search(input, picMatch, picPattern)) {
//                std::string tempUrl = picMatch[1].str();
//                if (tempUrl.substr(0, 5) != "https") {
//                    tempUrl = tempUrl.substr(1); // Nếu không bắt đầu bằng "https", bỏ ký tự đầu
//                }
//                if(account.avtUrl.empty())
//                    account.avtUrl = tempUrl;
//            }

            // Lấy tên hiển thị
            std::regex namePattern(" name(.*?)is_pushable");
            std::smatch nameMatch;
            if (std::regex_search(input, nameMatch, namePattern)) {
                if (account.displayName.empty())
                    account.displayName = nameMatch[1].str();
            }

            // Lấy giới tính
//            std::regex genderPattern("gender\\s+([^\\s]+)");
//            std::smatch genderMatch;
//            if (std::regex_search(input, genderMatch, genderPattern)) {
//                if(account.gender.empty())
//                    account.gender = genderMatch[1].str();
//            }

            // Lấy số điện thoại
            std::regex phonePattern("phone_full_number\":\"(.*?)\"");
            std::smatch phoneMatch;
            if (std::regex_search(input, phoneMatch, phonePattern)) {
                if (account.phoneNumber.empty())
                    account.phoneNumber = phoneMatch[1].str();
            }
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: getInfoAccountFromFile", LogLevel::ERROR);
    }

}


void Social::handleUpdateSocial(bool isInteract) {
    backupAccount();

    updateLine("Update Account");

    if (account.id.empty()) {
        if (isStop) return;
        delay(5, "Update failed - Account Id is invalid !!!");
        return;
    }

    if (isStop)
        saveStatusAcc("", "Stop Account Success", 0, Task::DONE);

    if (account.status == AccountStatus::LIVE && account.isLogin) {
        if (startTimeAccount.time_since_epoch().count() != 0) {
            int runTime = Common::getElapsedTime(startTimeAccount);
            saveStatusAcc("Time", to_string(runTime) + "second", 0, Task::DONE);
        }

        getInfoAccountFromFile();
    }

    account.updateInfo("id", account.id);
    account.updateInfo("msgAccount", account.msgAccount);
    account.updateInfo("uid", account.uid);
    account.updateInfo("displayName", account.displayName);
    account.updateInfo("status", account.status);
    account.updateInfo("socialType", static_cast<int>(social.type));

    if (!account.deviceInfo.empty())
        account.updateInfo("deviceInfo", account.deviceInfo);

    if (account.coinReceived > 0)
        account.updateInfo("coinEarned", account.coinReceived);

    if (isInteract)
        account.updateInfo("isInteract", isInteract);

    IResult response = OCRequest("SocialApp/update-account/",
                                 account.updateAccountConfigs.toString());
    if (!response.succeeded)
        delay(5, "Update Account FAIL");
}

bool Social::closeSocial() {
    return closeAppAction(social.package);
}

bool Social::openSocial(bool isCloseApp) {
    if (!isCloseApp && isAppOpened(social.package))
        return true;

    for (int loop = 3; loop > 0; loop--) {
        if (isCloseApp) {
            closeSocial();
            delay(1);
        }

        openApp(social.package);

        delay(5, "OPEN " + social.name);

        for (int loopOpen = 15; loopOpen > 0; loopOpen--) {

            if (isStopped()) return false;

            updateLine("OPEN " + social.name + " - " + to_string(loopOpen));

            string screen = detectScreen();

            if (Common::strContains(screen, "_POPUP_")) continue;

            if (isAppOpened(social.package)) return true;

            if (Common::strContains(screen, "TIK_") && social.type == SocialType::TIKTOK)
                return true;

            if (Common::strContains(screen, "FACEBOOK_") && social.type == SocialType::FACEBOOK)
                return true;

            if (Common::strContains(screen, "TWITTER_") && social.type == SocialType::TWITTER)
                return true;

            if (Common::strContains(screen, "INSTAGRAM_") && social.type == SocialType::INSTAGRAM)
                return true;

            if (Common::strContains(screen, "THREADS_") && social.type == SocialType::THREADS)
                return true;

            delay(1);
        }
        isCloseApp = true;
    }
    return false;
}


bool Social::isStopped() {
    if (isStop) return true;

    if (limitTimeAction > 0 &&
        startTimeAction != std::chrono::time_point<std::chrono::high_resolution_clock>()
        && Common::hasExceededTimeout(startTimeAction, limitTimeAction))
        return true;

    if (limitTimeRun > 0 &&
        startTimeAccount != std::chrono::time_point<std::chrono::high_resolution_clock>()
        && Common::hasExceededTimeout(startTimeAccount, limitTimeRun))
        return true;

    if (appSetting.proxy.expiredTime > 0 &&
        startTimeProxy != std::chrono::time_point<std::chrono::high_resolution_clock>()
        && Common::hasExceededTimeout(startTimeProxy, appSetting.proxy.expiredTime - 60)) {
        setupNetwork();
        return false;
    }
    return false;
}

void Social::backupAccount() {
    if (!social.isRootRequired) return;

    if (!account.isLogin) return;

    updateLine("Backup account");

    backupApp(social.package, social.path + account.uid);
}

bool Social::restoreAccount() {
    if (!social.isRootRequired) return true;


    if (account.deviceInfo.empty()) {
        saveStatusAcc("Restore", "Device info empty", 5);
        return false;
    }

    updateLine("Restore Account");

    if (social.type == SocialType::FACEBOOK) {
        string logged = ADB::runShell(
                "\"ls /data/data/" + social.package +
                "/app_light_prefs/com.facebook.katana/ | grep logged_in_\"");
        if (Common::strContains(logged, account.uid)) return true;
    }

    string pathInput = social.path + account.uid;

    if (!restoreApp(social.package, pathInput))
        clearDataAppAction(social.package);


    return true;
}

bool Social::changeDevice() {
    string param = "";

    IResult response = OCRequest(APP_URL + "change-device" + param);

    if (!response.succeeded) {
        delay(5, "Change Device failed");
        return false;
    }

    string deviceInfo = response.json.toString();

    if (account.deviceInfo.empty())
        account.deviceInfo = deviceInfo;
    else {
        JSON jsonOldDevice{account.deviceInfo};
        JSON newDevice{deviceInfo};

        jsonOldDevice.set("longitude", newDevice.getString("longitude"));
        jsonOldDevice.set("latitude", newDevice.getString("latitude"));

        deviceInfo = account.deviceInfo = jsonOldDevice.toString();
    }

    sendEventToHelper(HelperAction::RESTORE_DEVICE, {{"data", deviceInfo}});
    return !deviceInfo.empty();
}

void Social::removeBackup(string uid) {
    string pathRemove =
            uid.length() > 0 ? social.path + uid + ".zip" : social.path + account.uid;
    ADB::runShell("rm -rf " + pathRemove);
    ADB::runShell("rm -rf " + pathRemove + ".zip");
}

void Social::updateStatusJob(string &statusJob, int countSuccess, int total) {
    statusJob = "Job : " + to_string(countSuccess) + "/" + to_string(total)
                + "\n"
                + "Coin : " + to_string(account.coinReceived) + "\n";
}

string Social::RequestTDTT(string url, string body) {
    string header = "";

    switch (social.type) {
        case SocialType::TIKTOK:
            header = "public_cloud_phone_1";
            url = "https://public.traodoituongtac.com/api/v2/" + url;
            break;
        case SocialType::FACEBOOK:
            header = "max_phone_farm_24";
            url = "https://apiv3.traodoituongtac.com/api/v2/jobs/" + url;
            break;
        case SocialType::INSTAGRAM:
            header = "max_instagram_23";
            url = "https://apiv3.traodoituongtac.com/api/v2/jobs/" + url;
            break;
        default:
            Common::NLog("Not-found Social", "RequestTDTT");
            return "";
    }
    Common::NLog(url, "RequestTDTT");

    Http client(url, Method::POST);

    client.addBearerToken(jobSetting.tokenTDTT);

    client.setHeader("X-Api-Version: " + header);

    client.setContentType("application/json");

    if (social.type == SocialType::FACEBOOK || social.type == SocialType::INSTAGRAM) {
        JSON jsonEncrypted;
//        jsonEncrypted.set("data", Go::encryptStringTDTT(body));
        jsonEncrypted.set("data", body);
        body = jsonEncrypted.toString();
    }

    client.setBody(body);

    string response = client.connect().response;

    Common::NLog(body, "Body");

    Common::NLog(response, "Response");

    return response;
}

vector<Social::Job> Social::getTDTTJobs(string status) {
    string lstType = "";

    if (jobSetting.isFollow)
        lstType += "social.name_follow,social.name_follow_all,";

    if (jobSetting.isLike)
        lstType += "social.name_like,social.name_like_all,";

    if (jobSetting.isComment) {
        lstType += "social.name_comment1,";
        if (social.type == SocialType::TIKTOK) {
            lstType += "social.name_comment_campaign,";
            lstType += "social.name_comment_campaign_all,";
            lstType += "social.name_comment_all,";
        }
    }

    if (social.type == SocialType::FACEBOOK) {
        lstType = Common::strReplace(lstType, "social.name_", "");
    } else {
        lstType = Common::strReplace(lstType, "social.name", Common::strToLower(social.name));
    }

    if (Common::strEndsWith(lstType, ",")) {
        lstType = lstType.substr(0, lstType.length() - 1);
    }

    JSON jsonBody{};
    if (social.type == SocialType::TIKTOK) {
        jsonBody.set("uidId", account.uid);
        jsonBody.set("uidName", Common::base64_encode(Common::strTrim(account.displayName)));
        jsonBody.set("fields", lstType);
    } else {
        jsonBody.set("uid", account.uid);
        jsonBody.set("name", Common::base64_encode(Common::strTrim(account.displayName)));
        jsonBody.set("types", lstType);
    }

    vector<Job> lstJobs;
    auto timeStart = high_resolution_clock::now();

    string endpoint = social.type == SocialType::TIKTOK ? "get-jobs" : "fetch";

    while (!isStopped() && !Common::hasExceededTimeout(timeStart, jobSetting.delayGetJob)) {
        string response = RequestTDTT(endpoint, jsonBody.toString());

        JSON responseJson = JSON(response);
        if (responseJson.getBool("success")) {
            vector<JSON> lstData = JSON(responseJson).getArray("data");
            for (auto obj: lstData) {
                Job newJob;
                newJob.id = obj.getString("job_id"); // job_id
                newJob.type = obj.getString("type"); // type
                newJob.field = obj.getString("action"); // action || job type
                newJob.link = obj.getString("link"); // link
                newJob.content = obj.getString("job_info");
                lstJobs.push_back(newJob);
            }
            return lstJobs;
        }

        string message = responseJson.getString("message");

        string timeWait = Common::regexGroups1(message, "Vui lòng chờ (\\d+) giây");
        int delayTime = timeWait.empty() ? 5 : Common::strToInt(timeWait);
        int remainingTime = jobSetting.delayGetJob - Common::getElapsedTime(timeStart);
        if (remainingTime < delayTime) {
            break;
        }
        delay(delayTime, status + " delay get job");
    }

    return lstJobs;
}

bool Social::updateTDTTJob(Social::Job job) {
    JSON jsonBody{};
    if (social.type == SocialType::TIKTOK) {
        jsonBody.set("jobId", job.id);
        jsonBody.set("uidId", account.uid);

        if (job.field == "tiktok_comment1")
            jsonBody.set("jobId", job.id);
    } else {
        jsonBody.set("job_id", job.id);
        jsonBody.set("uid", account.uid);
    }

    jsonBody.set("success", true);

    string endpoint = social.type == SocialType::TIKTOK ? "get-coins" : "update";

    string responseGetCoin = RequestTDTT(endpoint, jsonBody.toString());

    if (responseGetCoin.empty()) return false;

    JSON jsonNhanXu(responseGetCoin);

    //"{\"success\":true,\"message\":\"Gửi kết quả thành công\",
    // \"data\":{\"job_id\":\"67860bfe68be71adbe5a540e|5694252\",
    // \"success\":true,\"uid\":\"100022146662830\",
    // \"ip_address\":null,\"date_update\":\"2025-01-14 15:23:36\"}}"

    bool isSucess = jsonNhanXu.getBool("success");

    if (!isSucess) return false;

    if (social.type == SocialType::TIKTOK) {
        if (job.field == "tiktok_comment1") {
            account.coinReceived += 1;
            return true;
        }
        string dataRes = jsonNhanXu.getString("data");
        if (dataRes.empty()) return false;

        vector<string> dataSplit = Common::strSplit(dataRes, "/");
        if (dataSplit.empty()) return false;

        try {
            string jobSucessStr = dataSplit[0];
            int jobSuccess = std::stoi(jobSucessStr);

        }
        catch (const std::exception &e) {
            Common::NLog(e.what(), "updateTDTTJob", LogLevel::ERROR);
            return false;
        }

        account.coinReceived += jsonNhanXu.getInt("coin_received");
    }


    return true;
}

ResultScreen Social::handleEarnMoneySocial(std::function<Task()> funcSetup) {
    if (!getJobSetting()) return NOMAP;

    Task taskSetup = funcSetup();

    if (taskSetup != Task::DONE) return ERROR;
}

std::tuple<bool, string, string> Social::getScriptContent(string contentId) {
    try {
        string uri = "socialApp/get-script-content?Id=" + contentId;

        IResult response = OCRequest(uri);
        if (!response.succeeded) {
            if (Common::strContainsLower(response.message, "Not-found Content")) {
                saveStatusAcc(currentAction, "Get content failed", 2);
            }
            return {false, {}, ""};
        }

        string content = "";

        bool isAIGenerated = response.json.getBool("isAIGenerated");

        if (isAIGenerated) {
            string apiKey = response.json.getString("apiKey");

            LLMType type = static_cast<LLMType>(response.json.getInt("llmType"));

            string promptAsk = response.json.getString("promptAsk");

            content = generateScriptContent(promptAsk, apiKey, type);
            if (content.empty()) {
                delay(2, "Generate content failed");
                return {false, {}, ""};
            }
        } else {
            content = response.json.getString("content");
        }

        bool isDeleteAfterUse = response.json.getBool("isDeleteAfterUse");

        string itemId = isDeleteAfterUse ? response.json.getString("itemId") : "";

        return {true, content, itemId};
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: getScriptContent", LogLevel::ERROR);
    }
    return {false, {}, ""};
}


void Social::deleteScriptContent(string itemId) {
    try {
        string uri = "socialApp/delete-content";

        JSON body{};
        body.set("id", itemId);

        IResult response = OCRequest(uri, body.toString());
        if (!response.succeeded)
            Common::NLog("Delete content failed", "deleteScriptContent", LogLevel::WARNING);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: deleteScriptContent", LogLevel::ERROR);
    }
}

vector<Social::MediaS3> Social::getScriptMedia(string galleryId, int countMedia) {
    if (galleryId.empty()) {
        saveStatusAcc("", "Not-found Gallery", 5);
        return {};
    }

    string path = MEDIA_FOLDER "*";

    Storage::removePath(path);

    Storage::scanMedia(MEDIA_FOLDER);

    IResult response = OCRequest(
            "socialApp/get-media?GalleryId=" + galleryId + "&count=" + to_string(countMedia));

    if (!response.succeeded) {
        delay(5, "Download Media failed");
        return {};
    }

    int countSuccess = 0;

    vector<Social::MediaS3> lstResult = {};
    for (const auto &item: response.json.getInstance()) {
        MediaS3 mediaItem{
                item["id"],
                item["name"],
                item["path"],
        };

        string url = mediaItem.path;

        string extension = "." + Common::strSplit(url, ".").back();

        Http client(url, Method::GET);

        if (Common::strStartsWith(url, APP_URL))
            client.addBearerToken(jwt);

        string output = MEDIA_FOLDER + Common::ranGuid() + extension;

        if (client.connectDownload(output)) countSuccess++;

        lstResult.push_back(mediaItem);
    }

    if (countMedia > 0 && countSuccess < countMedia / 2) {
        saveStatusAcc("", "Download Media failed", 5);
        return {};
    }
    return lstResult;
}

void Social::deleteScriptMedia(vector<MediaS3> lstMedia) {
    if (lstMedia.empty()) {
        Common::NLog("No media to delete", "deleteScriptMedia", LogLevel::WARNING);
        return;
    }
    try {
        string uri = "socialApp/delete-media";

        JSON body{};

        vector<string> lstIds;

        for (const auto &item: lstMedia) {
            lstIds.push_back(item.id);
        }

        body.set("ids", lstIds);

        IResult response = OCRequest(uri, body.toString());
        if (!response.succeeded)
            Common::NLog("Delete gallery failed", "deleteGalleryItem", LogLevel::WARNING);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: deleteGalleryItem", LogLevel::ERROR);
    }
}


string Social::generateScriptContent(string promptAsk, const string &APIKey,
                                     Social::LLMType LLMType) {
    if (promptAsk.empty() || APIKey.empty()) {
        Common::NLog("PromptAsk or APIKey is empty", "generateScriptContent", LogLevel::WARNING);
        return "";
    }

//    promptAsk = "Hãy trả lời đúng trọng tâm : " + promptAsk;

    string content = "";

    if (LLMType == LLMType::GEMINI_1_5_FLASH) {
        content = API::LLM::askGemini(APIKey, "1.5-flash", promptAsk);
    } else if (LLMType == LLMType::GEMINI_2_0_FLASH) {
        content = API::LLM::askGemini(APIKey, "2.0-flash", promptAsk);
    }
    return content;
}

