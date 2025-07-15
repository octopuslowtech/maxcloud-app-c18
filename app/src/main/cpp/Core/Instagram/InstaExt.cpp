//
// Created by Admin on 12/14/2024.
//

#include "InstaExt.h"
#include <iostream>

Task InstaExt::handleLogin() {
    if (!openSocial()) {
        if (!isStop)
            saveStatusAcc("Login", "Open Package Fail");
        return Task::FAIL;
    }

    struct LoginStruct {
        int maxInput2FA = 3;
        string loginUid = "";
        string oldOtp;
        int maxGetOtp = 2;
        bool isSelectLoginWithPassword = false;
        bool isSwtichMethod = false;
        bool isLoginStep = false;
    } login;

    if (setting.loginType == LoginType::RANDOM)
        setting.loginType = Common::ranBool() ? LoginType::UID : LoginType::MAIL;

    if (setting.loginType == LoginType::MAIL || setting.loginType == LoginType::MAIL_OTP) {
        if (account.email.empty()) {
            setting.loginType == LoginType::UID;
            login.loginUid = account.uid;
        } else {
            login.loginUid = account.email;
            login.isSwtichMethod = true;
        }
    } else
        login.loginUid = account.uid;
    string xml = "";
    // -------------------------- //
    int maxTryAgain = 3;

    for (int loop = 35; loop > 0; loop--) {
        if (isStopped()) return Task::FAIL;
        updateLine("Login Instagram - " + to_string(loop));
        delay(1);
        xml = dumpXml();
        string screen = detectScreen(xml);
        if (screen == "CONTINUE") continue;
        if (screen == "ERROR") return Task::FAIL;

        if (account.password.empty() && (screen == "IG_LOGIN")) {
            saveStatusAcc("Login", "Password empty");
            delay(5, "Password empty");
            return Task::FAIL;
        }
        if (screen == "IG_LOGIN") {
            inputText("//*[@text='Username, email or mobile number' or @content-desc='Username, email or mobile number' or @text='Username, email address or mobile number' or @content-desc='Username, email address or mobile number']",account.uid);
            delay(1);


            inputText("//*[@text='Password' or @content-desc='Password']", account.password);
            delay(1);
            //hideKeyboard();
            xml = dumpXml();
            tap("//*[@text='Log In' or @content-desc='Log In']", xml);
            delay(1);
            continue;
        }
        if (screen == "META_LOGIN_INCORRECT_PASSWORD") {
            account.status = AccountStatus::WRONG;
            saveStatusAcc("Login", "Incorrect password");
            return Task::FAIL;
        }
        if (screen == "IG_LOGIN_NOTFOUND_ACCOUNT") {
            account.status = AccountStatus::INVALIDATE;
            saveStatusAcc("Login", "Notfound Account");
            return Task::FAIL;
        }
        if (screen == "IG_SUSPENDED_ACCOUNT") {
            account.status = AccountStatus::CHECKPOINT;
            saveStatusAcc("Login", "Checkpoint");
            return Task::FAIL;
        }

        if (screen == "IG_LOGIN_APPROVE_2FA_POPUP") {
            tap("//*[@content-desc='Try another way']");
            delay(1);
            xml = dumpXml();
            if (existNode("//*[@text='Choose a way to confirm it’s you' or @text=\"Choose a way to confirm that it's you\" or @content-desc=\"Choose a way to confirm that it's you\"]", xml, 3))
                tap("//*[@text='Authentication app']", xml);
            delay(1, 2);
            tap("//*[@text='Continue' or @content-desc='Continue']", xml);
            continue;
        }
        // "META_2FA_ENTER_CODE"
        if (screen == "META_LOGIN_ENTER_2FA") {
            maxTryAgain--;
            if (maxTryAgain < 0)
                break;

            if (account.twoFA.empty()) {
                saveStatusAcc("Login", "2FA empty");
                return Task::FAIL;
            }
            string twofaOTP = API::get2FA(account.twoFA);
            if (twofaOTP.empty()) {
                delay(5, "Get 2FA failed");
                continue;
            }
            inputText("//*[@content-desc='Code']", twofaOTP);
            delay(1);
            tap("//*[@text='Continue' or @content-desc='Continue']", xml);
            continue;
        }
        if (screen == "IG_LOGIN_APPROVE_2FA_FAIL" || screen == "IG_PAGE_NOT_AVAILABLE") {
            maxTryAgain--;
            if (maxTryAgain < 0)
                break;

            closeAppAction(social.package);
            openApp(social.package);
            continue;
        }
        if (screen == "IG_POPUP_LOGIN_SAVE_LOGIN_INFO") {
            tap("//*[@text='Not Now' or @content-desc='Not Now']", xml);
            delay(2);
            continue;
        }
        if (screen == "IG_HOME") {
            tap("//*[@content-desc='Profile']");
            delay(1);
            continue;
        }
        if (screen == "IG_PROFILE") {
//            vector<Node> nodes = getNodes(
//                    "//*[@class='android.widget.Button' and string-length(@text) > 0 and not(contains(@text, 'follow')) and not(contains(@text, 'posts')) and not(contains(@text, 'following')) and not(contains(@text, 'message'))and not(contains(@text, 'Photo by'))]",
//                    xml);
//            if (nodes.empty()) {
//                delay(1);
//                continue;
//            }
//           ` for (Node s: nodes) {
//                if (Common::strToLower(account.uid) == s.text) {
//                    Common::NLog("Login Success");
//                    updateLine("Login Success");
//                    account.status = AccountStatus::LIVE;
//                    account.isLogin = true;
//                    return Task::DONE;
//                }
//            }`

            updateLine("Login Success");
            account.status = AccountStatus::LIVE;
            account.isLogin = true;
            return Task::DONE;
            continue;
        }
    }
    return Task::FAIL;
}

//ResultScreen InstaExt::gotoProfile(string link, string message) {
//    if (link.empty()) {
//        link = "https://www.instagram.com/" + account.uid;
//    } else {
//        if (!Common::strStartsWith(link, "https://")) {
//            link = "https://www.instagram.com/" + link;
//        }
//    }
//
//    std::string user = Common::regexGroups1(link, "https://www\\.instagram\\.com/([^/?]+)");
//    if (!user.empty()) {
//        user = Common::strSplit(user, "?")[0];
//    }
//
//    //goto profile
//    message = message.empty() ? "Goto profile" : message;
//    gotoLink(link, message, Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
//
//    string xml;
//    for (int loop = 15; loop > 0; loop--) {
//        xml = dumpXml();
//        string screen = detectScreen(xml);
//        if (screen == "CONTINUE") continue;
//        if (screen == "ERROR") return ERROR;
//        if (screen == "IG_ANOTHER_PROFILE") {
//            //string username = Common::regexGroups1(xml, "\"@([^\\s]+)\"", -1);
//            vector<Node> nodes = getNodes(
//                    "//*[@class='android.widget.TextView' and string-length(@text) > 0 and not(contains(@text, 'follow')) and not(contains(@text, 'posts')) and not(contains(@text, 'following')) and not(contains(@text, 'message'))and not(contains(@text, 'Photo by'))]",
//                    xml);
//            if (nodes.empty()) {
//                delay(1);
//                continue;
//            }
//            for (Node s: nodes) {
//                if (Common::strToLower(user) == s.text) {
//                    updateLine("Goto profile success");
//                    return DONE;
//
//                }
//            }
//            return DONE;
//        }
//
//        if (existNode("//*[@text='Loading']", xml)) {
//            if (waitNodeHide("//*[@text='Loading']", 5))
//                continue;
//            tap("//*[@content-desc='Back to previous screen']", xml);
//            delay(1);
//            gotoLink(link, message, Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
//            continue;
//        }
//
//        if (loop % 4 == 0) {
//            gotoLink(link, message, Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
//            continue;
//        }
//
//        if (swipeAndScreenNotChange(-1))
//            break;
//    }
//    return NOMAP;
//}


bool InstaExt::gotoReel() {
    for (int i = 0; i < 10; i++) {
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "FB_SESSION_EXPIRED")
            continue;

        if (screen == "IG_REEL" ||
            existNode("//android.widget.FrameLayout[@content-desc='Reels' and @selected='true']",
                      xml))
            return true;

        string element = existNodes(0, xml, {
                "//android.widget.FrameLayout[@content-desc='Reels' and @selected='false']",
        });

        if (element ==
            "//android.widget.FrameLayout[@content-desc='Reels' and @selected='false']") {
            tapCenter(element, xml);
            delay(2);
            continue;
        }

        gotoLink("instagram://reels");
        delay(2);
    }
    return false;
}

bool InstaExt::gotoCreatePostImg() {
    if (!gotoHome())
        return false;
    delay(2);

    auto timeStart = high_resolution_clock::now();
    int timeout = 60;

    while (true) {
        if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
            break;

        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "FB_SESSION_EXPIRED")
            break;
        if (existNode("//*[@text='New post']", xml))
            return true;

        string element = existNodes(0, xml, {
                "//android.widget.FrameLayout[@content-desc='Create' and @selected='false']",
                "(//*[contains(@text, 'Allow')])[2]",
                "//*[@text='POST' or @content-desc='POST']",
        });

        if (element == "//android.widget.FrameLayout[@content-desc='Create' and @selected='false']"
         || element == "(//*[contains(@text, 'Allow')])[2]") {
            tap(element, xml);
            delay(2);
            continue;
        }
        if (element == "//android.widget.TextView[@text='POST' or @content-desc='POST']") {
            tapCenter(element, xml);
            delay(2);
            continue;
        }

    }
    return false;
}

bool InstaExt::gotoCreateReel() {
    if (!gotoReel())
        return false;
    for (int i = 0; i < 20; i++) {
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "IG_SUSPENDED_ACCOUNT")
            continue;

        if (screen == "IG_REEL_CAMERA" ||
            existNode("//android.widget.ImageView[@content-desc='Gallery']", xml))
            return true;

        string element = existNodes(0, xml, {
                "//android.widget.FrameLayout[@content-desc='Reels' and @selected='false']",
                "//android.widget.Button[@content-desc='Create a reel']",
                "//android.widget.Button[@text='Allow']",
                "//*[@resource-id='com.instagram.android:id/cam_dest_clips']"
        });

        if (element ==
            "//android.widget.FrameLayout[@content-desc='Reels' and @selected='false']") {
            tapCenter(element, xml);
            delay(2);
            continue;
        }
        if (element == "//android.widget.Button[@content-desc='Create a reel']") {
            tapCenter(element, xml);
            delay(2);
            continue;
        }
        if (element == "//android.widget.Button[@text='Allow']") {
            tapCenter(element, xml);
            delay(2);
            continue;
        }
        if (element == "//*[@resource-id='com.instagram.android:id/cam_dest_clips']") {
            tapCenter(element, xml);
            delay(2);
            continue;
        }
        gotoLink("instagram://reels-camera");
        delay(2);
    }

    return false;

}

bool InstaExt::gotoCreateStory() {
    for (int i = 0; i < 20; ++i) {
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "FB_SESSION_EXPIRED")
            continue;
        string element = existNodes(0, xml, {
                "//android.widget.FrameLayout[@content-desc='Create' and @selected='false']",
                "//*[@text='STORY' or @content-desc='STORY']",
                "//*[@text='Allow Instagram to take pictures and record video?']",
                "//*[@resource-id='com.android.packageinstaller:id/permission_message']",
        });
        if (element ==
            "//android.widget.FrameLayout[@content-desc='Create' and @selected='false']") {
            tap(element, xml);
            delay(2);
            continue;
        }
        if (element == "//*[@text='STORY' or @content-desc='STORY']") {
            Node node = getNode(element, xml);
            if (node.centerX == sWidth / 2)
                return true;
            tap(element, xml);
            delay(2);
            continue;
        }
        if (element == "//*[@resource-id='com.android.packageinstaller:id/permission_message']") {
            tap("//*[@resource-id='com.android.packageinstaller:id/permission_allow_button']", xml);
            delay(2);
            continue;
        }
        gotoLink("instagram://story-camera");
        delay(2);
    }
    return false;
}

bool InstaExt::gotoHome() {
    for (int i = 0; i < 10; i++) {
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "FB_SESSION_EXPIRED")
            continue;

        if (screen == "IG_HOME")
            return true;

        string element = existNodes(0, xml, {
                "//*[@resource-id='com.instagram.android:id/feed_tab']",
        });

        if (element == "//*[@resource-id='com.instagram.android:id/feed_tab']") {
            tap(element, xml);
            delay(2);
            continue;
        }

        gotoLink("https://www.instagram.com");
        delay(2);
    }

    return false;
}

bool InstaExt::gotoProfile(string user) {
    if (user != "") {
        if (Common::strStartsWith(user, "https://")) {
            if (!Common::strEndsWith(user, "/"))
                user += "/";
            user = Common::regexGroups1(user, "instagram.com/(.*?)/");
        }
        gotoLink("instagram://user?username=" + user);
        delay(2);
        return true;
    }

    for (int i = 0; i < 10; i++) {
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "IG_SUSPENDED_ACCOUNT")
            continue;

        if (screen == "IG_PROFILE" && existNode("//*[@content-desc='Edit profile']", xml))
            return true;

        string element = existNodes(0, xml, {
                "//*[@content-desc='Profile']",
                "//*[@resource-id='com.instagram.android:id/profile_tab']",
        });

        if (!element.empty()) {
            tap(element, xml);
            delay(2);
            continue;
        }

        if (!gotoHome())
            return false;
    }
    return false;
}

void InstaExt::gotoHashtag(string hashtag) {
    gotoLink("instagram://tag?name=" + hashtag);
}

void InstaExt::gotoInbox() {
    gotoLink("instagram://direct-inbox");
}

void InstaExt::gotoSearch() {
    gotoLink("instagram://search");
}

void InstaExt::gotoSettings() {
    gotoLink("instagram://settings");
}

void InstaExt::gotoPost(string link) {
    string id = getIdPostFromLink(link);
    gotoLink("instagram://media?id=" + id);
}

string InstaExt::getIdPostFromLink(string link) {
    if (!Common::strStartsWith(link, "https://"))
        return link;

    string id = "";
    if (!Common::strEndsWith(link, "/"))
        link += "/";

    vector<string> lstPatter = {
            "instagram.com/p/(.*?)/",
            "instagram.com/reels/(.*?)/",
            "instagram.com/reel/(.*?)/",
    };

    for (int i = 0; i < lstPatter.size(); i++) {
        id = Common::regexGroups1(link, lstPatter[i]);
        if (id != "" && !Common::strContains(id, "&"))
            break;
    }

    return id;
}

void InstaExt::gotoLink(const string &uri) {
    ADB::runShell(
            "am start -a android.intent.action.VIEW -d \"" + uri + "\" com.instagram.android");
}

//void InstaExt::gotoLink(const string &uri, const string &message, int delayTime) {
//    JNIEnv *env = JExt::getEnv();
//    jobject jContext = getContext();
//
//
//    jclass intentClass = JExt::findClass("android/content/Intent");
//
//
//    jmethodID intentConstructor = JExt::findMethod(intentClass, "<init>",
//                                                   "(Ljava/lang/String;Landroid/net/Uri;)V");
//    jstring actionView = env->NewStringUTF("android.intent.action.VIEW");
//    jclass uriClass = JExt::findClass("android/net/Uri");
//    jmethodID parseMethod = JExt::findMethod(uriClass, "parse",
//                                             "(Ljava/lang/String;)Landroid/net/Uri;", true);
//
//    jstring uriString = JExt::stringToJstring(uri);
//    jobject jUri = env->CallStaticObjectMethod(uriClass, parseMethod, uriString);
//
//    jobject intent = env->NewObject(intentClass, intentConstructor, actionView, jUri);
//
//    jint flag = env->GetStaticIntField(intentClass,
//                                       env->GetStaticFieldID(intentClass,
//                                                             "FLAG_ACTIVITY_NEW_TASK", "I"));
//    jmethodID addFlagsMethod = JExt::findMethod(intentClass, "addFlags",
//                                                "(I)Landroid/content/Intent;");
//    env->CallObjectMethod(intent, addFlagsMethod, flag);
//
//    jstring packageName = JExt::stringToJstring(social.package);
//    jmethodID setPackageMethod = JExt::findMethod(intentClass, "setPackage",
//                                                  "(Ljava/lang/String;)Landroid/content/Intent;");
//    env->CallObjectMethod(intent, setPackageMethod, packageName);
//
//    jclass browserClass = JExt::findClass("android/provider/Browser");
//    jfieldID extraAppIdField = env->GetStaticFieldID(browserClass, "EXTRA_APPLICATION_ID",
//                                                     "Ljava/lang/String;");
//    jstring extraAppId = (jstring) env->GetStaticObjectField(browserClass, extraAppIdField);
//
//    jclass contextClass = JExt::findClass("android/content/Context");
//    jmethodID startActivityMethod = JExt::findMethod(contextClass, "startActivity",
//                                                     "(Landroid/content/Intent;)V");
//    env->CallVoidMethod(jContext, startActivityMethod, intent);
//
//    env->DeleteLocalRef(actionView);
//    env->DeleteLocalRef(uriClass);
//    env->DeleteLocalRef(uriString);
//    env->DeleteLocalRef(jUri);
//    env->DeleteLocalRef(intent);
//    env->DeleteLocalRef(packageName);
//    env->DeleteLocalRef(browserClass);
//    env->DeleteLocalRef(extraAppId);
//    env->DeleteLocalRef(contextClass);
//
//    if (delayTime > 0)
//        delay(delayTime, message);
//}

string InstaExt::handleScreen(string &xml) {
    if (isStopped()) return "ERROR";

    string screen = detectScreen(xml);

    if (Common::strContains(screen, "IG_POPUP_LOGOUT")) {
        removeBackup();
    }


    if (Common::strContains(screen, "_POPUP_")) return "CONTINUE";



    if (!Common::strStartsWith(screen, "IG_") && !isAppOpened(social.package)
        && !Common::strContains(currentActivity, "android.inputmethodservice.SoftInputWindow")) {
        if (account.countReopenApp == 3) {
            account.countReopenApp = 0;
            openSocial(false);
            return "CONTINUE";
        }
        account.countReopenApp++;
        delay(2, "Notfound Social Screen : " + currentActivity);
        return "CONTINUE";
    }
    return screen;
}

bool InstaExt::handleLogout() {
    Common::NLog(account.id);
    Common::NLog(account.uid);
    return false;
}


bool InstaExt::IsBlock() {
    return isBlock;
}

bool InstaExt::IsStop() {
    return isStopped();
}

bool InstaExt::IsLogined() {
    return isLogined;
}

bool InstaExt::IsLogout() {
    return isLogout;
}

bool InstaExt::IsRelogined() {
    return isRelogined;
}

bool InstaExt::IsSomethingWentWrong() {
    return isSomethingWentWrong;
}

bool InstaExt::IsCheckpoint() {
    return !typeCheckpoint.empty();
}

void InstaExt::ResetVariable() {
    isBlock = false;
    isLogined = false;
    isLogout = false;
    isRelogined = false;
    isSomethingWentWrong = false;
    typeCheckpoint = "";
}
//{
//"uid": "232234322422s23",
//"name": "4LmB4Lih4Lib4LmI4Liy4LmE4Liq4LmIIOC4leC4sOC4oeC4reC4hyA", // mã hoá base64 tên để chia job theo tên
//"types": "review_page" // ig thì thêm type ig: instagram_like,instagram_like_all,instagram_comment,instagram_comment_all,
// instagram_follow,instagram_follow_all (đó là các type instagram). đuôi _all để cho các job tên ngoại (tên ngoại + việt)
//}

vector<Social::Job> InstaExt::getJobs() {
//    string lstType = "";
//
//    if (jobSetting.isFollow)
//        lstType += "instagram_follow,instagram_follow_all,";
//    if (jobSetting.isLike)
//        lstType += "instagram_like,instagram_like_all";
//    if (jobSetting.isComment)
//        lstType += "instagram_comment,instagram_comment_all";
//
//    if (Common::strEndsWith(lstType, ",")) {
//        lstType = lstType.substr(0, lstType.length() - 1);
//    }
//
//    JSON jsonBody{};
//    jsonBody.set("uid", account.uid);
//    jsonBody.set("name", Common::base64_encode(Common::strTrim(account.displayName)));
//    jsonBody.set("types", lstType);
//
//    vector<Job> lstJobs;
//    auto timeStart = high_resolution_clock::now();
//    while (!isStopped() && !Common::hasExceededTimeout(timeStart, jobSetting.delayGetJob)) {
//        string response = RequestTDTT("get-jobs", jsonBody.toString());
//
//        JSON responseJson = JSON(response);
//        if (responseJson.getBool("success")) {
//            vector<JSON> lstData = JSON(responseJson).getArray("data");
//            for (auto obj: lstData) {
//                Job newJob;
//                newJob.id = obj.getString("job_id");
//                newJob.type = obj.getString("type");
//                newJob.field = obj.getString("action");
//                newJob.link = obj.getString("link");
//                newJob.content = obj.getString("job_info");
//                lstJobs.push_back(newJob);
//            }
//            return lstJobs;
//        }
//
//        string message = responseJson.getString("message");
//
//        string timeWait = Common::regexGroups1(message, "Vui lòng chờ (\\d+) giây");
//        int delayTime = timeWait.empty() ? 5 : Common::strToInt(timeWait);
//        int remainingTime = jobSetting.delayGetJob - Common::getElapsedTime(timeStart);
//        if (remainingTime < delayTime) {
//            break;
//        }
//        delay(delayTime, status + " delay get job");
//    }

    return vector<Social::Job>();

}

void InstaExt::hideKeyboard(){
    handleKeyboard(Keyboard::HIDE);
    delay(1);
}