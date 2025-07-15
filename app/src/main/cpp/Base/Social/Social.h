#include "ASAction.h"
#include "RTC.h"

#pragma once

class Social : public ASAction, public ASService, public Device {
public:
    enum class AccountStatus {
        NONE = 0,
        LIVE = 1,
        DIE = 2,
        CHECKPOINT = 3,
        WRONG = 4,
        INVALIDATE = 5
    };

    enum class FollowProfileType {
        RANDOM = 0,
        PROFILE = 1,
        VIDEO = 2,
    };

    enum class LoginType {
        RANDOM = 0,
        UID = 1,
        MAIL = 2,
        MAIL_OTP = 3
    };

    enum class OpenLinkType {
        RANDOM = 0,
        BROWSER = 1,
        APP = 2,
        SEARCH = 3,
    };

    struct MediaS3 {
        string id;
        string name;
        string path;

        // contruct
        MediaS3(string id, string name, string path) : id(id), name(name), path(path) {}
    };

    struct Account {
        vector<string> lstUid;
        string id;
        string uid;
        string displayName;
        string password;
        string twoFA;
        string email;

        string refreshTokenMail;
        string clientIdMail;

        string deviceInfo;
        string phoneNumber;
        string cookie;
        string token;


        AccountStatus status = AccountStatus::NONE;

        string proxyIP;
        string proxyPort;
        string proxyUser;
        string proxyPass;

        // local
        bool isLogin;
        int countCaptchaFail;
        string msgAccount;
        JSON updateAccountConfigs;
        int countReopenApp;
        long coinReceived;

        bool init(JSON json) {
            try {
                id = json.getString("id");
                uid = json.getString("uid");
                displayName = json.getString("displayName");
                password = json.getString("password");
                twoFA = json.getString("twoFA");
                email = json.getString("email");




                refreshTokenMail = json.getString("refreshTokenMail");
                clientIdMail = json.getString("clientIdMail");
                deviceInfo = json.getString("deviceInfo");
                phoneNumber = json.getString("phoneNumber");

                status = static_cast<AccountStatus>(json.getInt("status"));

                string proxyStr = json.getString("proxy");

                vector<string> lstProxy = Common::strSplit(Common::strTrim(proxyStr, ": "),
                                                           ":");
                if (lstProxy.size() > 1) {
                    proxyIP = lstProxy[0];
                    proxyPort = lstProxy[1];
                    proxyUser = lstProxy.size() > 2 ? lstProxy[2] : "";
                    proxyPass = lstProxy.size() > 3 ? lstProxy[3] : "";
                }

                resetVariable();

                return !(uid.empty() && email.empty());
            }
            catch (const std::exception &e) {
                Common::NLog(e.what(), "Init Account", LogLevel::ERROR);
                return false;
            }
        }

        void resetVariable() {
            isLogin = false;
            countCaptchaFail = 0;
            msgAccount = "";
            updateAccountConfigs = {};
            countReopenApp = 0;
            coinReceived = 0;
            cookie = "";
            token = "";
        }

        void updateInfo(string key, const nlohmann::json &value) {
            updateAccountConfigs.set(key, value);
        }

    };

    struct Setting {
        bool isLogoutAfterInteract = false;
        int limitTimeRun = 0;
        int limitTimeAction = 0;
        int delayOpenLinkFrom = 0;
        int delayOpenLinkTo = 0;
        int delayCaptcha = 0;
        LoginType loginType;
        OpenLinkType openLinkType;
        FollowProfileType followProfileType;
        FollowProfileType followType;

        bool init(JSON json) {
            try {
                isLogoutAfterInteract = json.getBool("isLogoutAfterInteract");
                limitTimeRun = json.getBool("limitTimeRun") * 60;
                limitTimeAction = json.getBool("limitTimeAction") * 60;

                delayOpenLinkFrom = json.getInt("delayOpenLinkFrom");

                delayOpenLinkTo = json.getInt("delayOpenLinkTo");

                delayCaptcha = json.getInt("delayCaptcha");
                loginType = static_cast<LoginType>(json.getInt("loginAppType"));
                followProfileType = static_cast<FollowProfileType>(json.getInt(
                        "followProfileType"));

                openLinkType = static_cast<OpenLinkType>(json.getInt(
                        "openLinkType"));

                followType = followProfileType;
                return true;
            }
            catch (const std::exception &e) {
                return false;
            }
        }
    };

    struct JobSetting {
        string tokenTDTT;
        int delayJobFrom;
        int delayJobTo;
        int delayGetJob;
        int limitJobSuccess;
        int limitJobFail;
        bool isNewfeedAfterJobSuccess;
        bool isOpenLinkBySearch;
        int countNewFeedFrom;
        int countNewFeedTo;

        JSON json;

        bool isFollow;
        bool isLike;
        bool isComment;
        bool isShare;
        bool isBlock;

        int countJobSuccess;
        int countJobFailLienTiep;

        bool init(JSON json) {
            try {
                tokenTDTT = json.getString("tokenTDTT");
                delayJobFrom = json.getInt("delayJobFrom");
                delayJobTo = json.getInt("delayJobTo");
                delayGetJob = json.getInt("delayGetJob");
                limitJobSuccess = json.getInt("limitJobSuccess");
                limitJobFail = json.getInt("limitJobFail");
                isNewfeedAfterJobSuccess = json.getBool("isNewfeedAfterJobSuccess");
                isOpenLinkBySearch = json.getBool("isOpenLinkBySearch");
                countNewFeedFrom = json.getInt("countNewfeedFrom");
                countNewFeedTo = json.getInt("countNewfeedTo");
                isFollow = json.getBool("isFollow");
                isLike = json.getBool("isLike");
                isComment = json.getBool("isComment");
                isShare = json.getBool("isShare");
                isBlock = json.getBool("isBlock");

                countJobSuccess = 0;
                countJobFailLienTiep = 0;

                this->json = json;
                return true;
            }
            catch (const std::exception &e) {
                return false;
            }
        }

    };

    struct Job {
        string id;
        string type;
        string link;
        string field;
        string content;
        bool isSuccess;
        string message_error;
    };

    struct Action {
        string actionName = "";
        string scriptType = "";
        string galleryName = "";
        JSON jsonConfig;

        Action() = default;

        std::string getString(std::string name) {
            return jsonConfig.getString(std::move(name));
        }

        vector<string> getList(std::string name, int typeSplitString = 0) {
            return jsonConfig.getListSplit(std::move(name), typeSplitString);
        }

        bool getBool(std::string name) {
            return jsonConfig.getBool(std::move(name));
        }

        int getInt(std::string name) {
            return jsonConfig.getInt(std::move(name));
        }

        // add set strin and bool

        void setString(std::string name, std::string value) {
            jsonConfig.set(name, value);
        }

        void setBool(std::string name, bool value) {
            jsonConfig.set(name, value);
        }

    };

    struct SocialInfo {
        SocialType type;
        std::string name;
        std::string package;
        std::string version;
        bool isRootRequired;
        std::string path;
        std::string rootPath;
    };

    enum class LLMType {
        GEMINI_1_5_FLASH = 0,
        GEMINI_2_0_FLASH = 1,
    };

    static SocialInfo social;

    static Account account;

    static Action action;

    static Setting setting;

    static JobSetting jobSetting;

    static vector<Action> lstAction;

    static int countRegisterAccount;

    static std::chrono::time_point<high_resolution_clock> startTimeAccount;

    static std::chrono::time_point<high_resolution_clock> startTimeAction;

    static int limitTimeRun;

    static int limitTimeAction;

    static bool initSocial(const string &action);

    static bool getAccount(const string &accountId = "");

    static bool getSetting();

    static bool getJobSetting();

    static bool getScript();

    static void
    saveStatusAcc(string title, const string &msg, int delayTime = 0, Task task = Task::FAIL);

    static void handleInteractSocial(std::function<Task()> funcLogin,
                                     std::function<ResultScreen()> funcExecuteAction,
                                     bool isEarnMoney,
                                     const string &uidReq = "");

    static void handleUpdateSocial(bool isInteract = false);

    static bool closeSocial();

    static bool openSocial(bool isCloseApp = true);


    static void backupAccount();

    static bool restoreAccount();

    static void removeBackup(string uid = "");

    static bool changeDevice();

    static void resetVariable();

    static bool isStopped();

    static void updateStatusJob(string &statusJob, int countSuccess, int total);

    static std::tuple<bool, string, string> getScriptContent(string contentId);

    static void deleteScriptContent(string itemId);

    static vector<Social::MediaS3> getScriptMedia(string galleryId = "", int countMedia = 1);

    static void deleteScriptMedia(vector<MediaS3> lstMedia);


    template<typename CoreType>
    static void handleCoreActions(CoreType &core, const std::string &action) {
        if (!core.initSocial(action)) return;

        if (Common::strContains(action, "clear")) {
            updateLine("Clear data app");
            core.clearDataAppAction(core.social.package);
            updateLine("Clear data done");
            return;
        }

        if (Common::strContains(action, "test")) {
            core.handleTest();
        }

        if (Common::strContains(action, "interact")
            || Common::strContains(action, "earnmoney") ||
            Common::strContains(action, "login_uid")) {

            string uidReq = "";

            if (Common::strContains(action, "facebook_login_uid_")) {
                uidReq = Common::strReplace(action, "facebook_login_uid_", "");
                if (uidReq.empty()) {
                    Base::updateLine("Login : UID empty !!!", true);
                    return;
                }
                Common::NLog(uidReq, "login_uid");
            }

            core.handleInteractSocial(
                    [&]() -> Task {
                        core.ResetVariable();
                        return core.handleLogin();
                    },
                    [&]() -> ResultScreen { return core.handleAction(); },
                    Common::strContains(action, "earnmoney"),
                    uidReq
            );
        }


        if (Common::strContains(action, "register")) {
            core.countRegisterAccount = 0;
            while (!core.isStop) {
                core.handleRegisterAccount();
            }
        }
    }

    static string RequestTDTT(string url, string body);

    static vector<Job> getTDTTJobs(string status);

    static bool updateTDTTJob(Job job);

    static ResultScreen handleEarnMoneySocial(std::function<Task()> funcSetup);

    static void getInfoAccountFromFile();

private:
    static string
    generateScriptContent(string promptAsk, const string &APIKey, LLMType LLMType);
};

