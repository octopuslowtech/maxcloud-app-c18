//
// Created by HieuThuHai on 8/17/2024.
//

#include "TiktokExt.h"

string TiktokExt::handleScreen(string &xml) {
    if (isStopped()) return "ERROR";

    string screen = detectScreen(xml);

    if (Common::strContains(screen, "_POPUP_")) return "CONTINUE";

    if (Common::strStartsWith(screen, "TIK_CAPTCHA_")) {
        if (screen == "TIK_CAPTCHA_DRAG_SLIDE") {
            saveStatusAcc("Catpcha", "Drag Slide error", 5);
            return "ERROR";
        }

        updateLine("Catpcha - Screenshot");

        auto screenShoot = Media::screenShot(Media::OutputType::BASE64);

        if (!screenShoot.first) return "CONTINUE";

        string base64Image = screenShoot.second;

        if (base64Image.empty()) {
            delay(2, "Screenshot Fail");
            return "CONTINUE";
        }

        account.countCaptchaFail++;

        string type = "24";
        if (screen == "TIK_CAPTCHA_SELECT")
            type = "25";

        string result = omoCaptcha(type, base64Image);

        if (result.empty()) {
            tap("//*[@content-desc='Refresh']");
            delay(5, "Catpcha - Refresh");
            return "CONTINUE";
        }

        updateLine("Captcha - Execute");

        vector<string> arr = Common::strSplit(result, "|");

        int x1 = Common::strToInt(arr[0]);
        int y1 = Common::strToInt(arr[1]);
        int x2 = Common::strToInt(arr[2]);
        int y2 = Common::strToInt(arr[3]);
        if (screen == "TIK_CAPTCHA_DRAG") {
            if (existNode("//*[@text='Audio']", xml)) {
                x2 = ((x2 - x1) * 0.8);
                x2 = x1 + x2;
            }
            swipe(x1, y1, x2, y2, Common::ranNumber(2000, 3000));
        } else if (screen == "TIK_CAPTCHA_SELECT") {
            tap(x1, y1);

            delay(1);

            tap(x2, y2);

            delay(1);
            tap("//*[@text='Confirm']");
        }

        delay(setting.delayCaptcha, "Catpcha - Done");

        xml = dumpXml();

        string screen = detectScreen(xml);

        if (!Common::strStartsWith(screen, "TIK_CAPTCHA_"))
            account.countCaptchaFail = 0;

        if (account.countCaptchaFail == 5) {
            saveStatusAcc("", "Bypass Captcha FAIL", 5);
            return "ERROR";
        }

        return "CONTINUE";
    }

    if (screen == "TIK_ACCOUNT_DIE") {
        account.status = AccountStatus::DIE;
        return "ERROR";
    }

    if (screen == "UNKNOWN_SCREEN" && getListText(xml).empty() &&
        existNode("//android.widget.ProgressBar", xml)) {
        delay(3, "ProgressBar");
        return "CONTINUE";
    }

    if (!Common::strStartsWith(screen, "TIK_") && !isAppOpened(social.package)) {
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
//    string action = "com.maxcloud.onox.OPEN_LINK";
//    string package = "top.niunaijun.blackboxa";
//
//    vector<std::pair<std::string, string>> lstExtra = {
//            {"link",    uri}
//    };
//
//    sendBroadcast(action, package, lstExtra);

void TiktokExt::gotoLink(const string &uri, const string &message, int delayTime) {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();

    jclass intentClass = JExt::findClass("android/content/Intent");


    jmethodID intentConstructor = JExt::findMethod(intentClass, "<init>",
                                                   "(Ljava/lang/String;Landroid/net/Uri;)V");
    jstring actionView = env->NewStringUTF("android.intent.action.VIEW");
    jclass uriClass = JExt::findClass("android/net/Uri");
    jmethodID parseMethod = JExt::findMethod(uriClass, "parse",
                                             "(Ljava/lang/String;)Landroid/net/Uri;", true);

    jstring uriString = JExt::stringToJstring(uri);
    jobject jUri = env->CallStaticObjectMethod(uriClass, parseMethod, uriString);

    jobject intent = env->NewObject(intentClass, intentConstructor, actionView, jUri);

    jint flag = env->GetStaticIntField(intentClass,
                                       env->GetStaticFieldID(intentClass,
                                                             "FLAG_ACTIVITY_NEW_TASK", "I"));
    jmethodID addFlagsMethod = JExt::findMethod(intentClass, "addFlags",
                                                "(I)Landroid/content/Intent;");
    env->CallObjectMethod(intent, addFlagsMethod, flag);

    jstring packageName = JExt::stringToJstring(social.package);
    jmethodID setPackageMethod = JExt::findMethod(intentClass, "setPackage",
                                                  "(Ljava/lang/String;)Landroid/content/Intent;");
    env->CallObjectMethod(intent, setPackageMethod, packageName);

    jclass browserClass = JExt::findClass("android/provider/Browser");
    jfieldID extraAppIdField = env->GetStaticFieldID(browserClass, "EXTRA_APPLICATION_ID",
                                                     "Ljava/lang/String;");
    jstring extraAppId = (jstring) env->GetStaticObjectField(browserClass, extraAppIdField);

    jclass contextClass = JExt::findClass("android/content/Context");
    jmethodID startActivityMethod = JExt::findMethod(contextClass, "startActivity",
                                                     "(Landroid/content/Intent;)V");
    env->CallVoidMethod(jContext, startActivityMethod, intent);

    env->DeleteLocalRef(actionView);
    env->DeleteLocalRef(uriClass);
    env->DeleteLocalRef(uriString);
    env->DeleteLocalRef(jUri);
    env->DeleteLocalRef(intent);
    env->DeleteLocalRef(packageName);
    env->DeleteLocalRef(browserClass);
    env->DeleteLocalRef(extraAppId);
    env->DeleteLocalRef(contextClass);

    if (delayTime > 0)
        delay(delayTime, message);
}

ResultScreen TiktokExt::gotoHome(string &xml) {
    string link = "tiktok://feed";
    if (!tap("//*[@text='Home']/parent::*[@selected='false']", xml))
        gotoLink(link);

    for (int loop = 10; loop > 0; loop--) {
        if (isStopped())
            break;

        xml = dumpXml();
        string screen = handleScreen(xml);
        if (screen == "CONTINUE") continue;
        if (screen == "ERROR") return ERROR;
        if (screen == "TIK_HOME")
            return DONE;

        if (loop % 4 == 0) {
            gotoLink(link);
            continue;
        }

        if (swipeAndScreenNotChange(-1))
            break;
    };
    return NOMAP;
}

ResultScreen TiktokExt::gotoProfile(string link, string message) {
    if (link.empty()) {
        link = "https://www.tiktok.com/@" + account.uid;
    } else {
        if (!Common::strStartsWith(link, "https://")) {
            if (!Common::strStartsWith(link, "@"))
                link = "@" + link;
            link = "https://www.tiktok.com/" + link;
        }
    }

    string user = Common::regexGroups1(link + "/", "@(.*?)/");
    user = Common::strSplit(user, "?")[0];

    //goto profile
    message = message.empty() ? "Goto profile" : message;
    gotoLink(link, message, Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
    delay(2);
    string xml;
    for (int loop = 15; loop > 0; loop--) {
        xml = dumpXml();
        string screen = handleScreen(xml);
        if (screen == "CONTINUE") continue;
        if (screen == "ERROR") return ERROR;
        if (screen == "TIK_ANOTHER_PROFILE") {
            string username = Common::regexGroups1(xml, "\"@([^\\s]+)\"", -1);
            if (username.empty()) {
                delay(1);
                continue;
            }

            if (Common::strToLower(user) == username)
                return DONE;
            return DONE;
        }

        if (existNode("//*[@text='Loading']", xml)) {
            if (waitNodeHide("//*[@text='Loading']", 5))
                continue;
            tap("//*[@content-desc='Back to previous screen']", xml);
            delay(1);
            gotoLink(link, message, Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
            continue;
        }

        if (loop % 4 == 0) {
            delay(300,"Countdown");
            gotoLink(link, message, Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
            continue;
        }

        if (swipeAndScreenNotChange(-1))
            break;

    }
    return NOMAP;
}

ResultScreen TiktokExt::searchTiktok(const string &keyword, string typeSearch) {
    if (keyword.empty()) return NOMAP;

    updateLine("Searching " + typeSearch);

    string xml;
    string screen = handleScreen(xml);
    if (screen == "ERROR") return ERROR;
    if (screen != "TIK_HOME") {
        gotoHome(xml);
        xml = "";
    }

    string xSearch = existNodes(0, xml, {
            "(//android.widget.HorizontalScrollView/parent::*/following-sibling::*)[last()]",
            "//android.widget.HorizontalScrollView/following-sibling::*"
    });
    if (xSearch.empty())
        return NOMAP;

    tap(xSearch, xml);
    delay(2);

    xml = "";
    if (!existNode("//*[@text='Search']", xml, 10))
        return NOMAP;

    inputText("//android.widget.EditText", keyword);
    delay(1);

    tapCenter("//*[@text='Search']", xml);

    delay(2);
    xml = dumpXml();

    string checkScreen = existNodes(0, xml, {
            "//*[@text='Search']",
            "//*[@text='Sound search']",
            "//*[@text='English']"
    });

    if (!checkScreen.empty()) {
        if (checkScreen != "" && checkScreen != "//*[@text='Search']")
            back();
        if (checkScreen == "//*[@text='Search']")
            tapCenter("//*[@text='Search']");
    }


    xml = "";
    if (!existNode("//*[@text='" + typeSearch + "']", xml, 10))
        return NOMAP;

    tap("//*[@text='" + typeSearch + "']", xml);
    delay(3, "Tap " + typeSearch);
    //check không có kết quả tìm kiếm
    if (existNode("//*[@text='No results found']", xml, 5)) {
        updateLine("No results found");
        return NOMAP;
    }
    return DONE;
}


ResultScreen TiktokExt::gotoLive(string link, string message, int delayTime) {
    //goto home
    string xml = dumpXml();
    string screen = handleScreen(xml);
    if (screen == "ERROR") return ERROR;
    if (screen != "TIK_HOME")
        gotoHome(xml);

    //goto live
    message = message.empty() ? "Goto Live" : message;
    gotoLink(link, message, delayTime);

    for (int loop = 10; loop > 0; loop--) {
        if (isStopped())
            break;

        xml = dumpXml();
        screen = handleScreen(xml);
        if (screen == "CONTINUE") continue;
        if (screen == "ERROR") return ERROR;
        if (screen == "TIK_LIVE") return DONE;

        if (loop % 4 == 0) {
            gotoLink(link, message, delayTime);
            continue;
        }

        delay(2);
    }
    return NOMAP;
}

string TiktokExt::omoCaptcha(const string &type, const string &base64) {
    JSON createJob = API::OMOCaptcha::createJob(type, base64);

    if (createJob.isEmpty()) return "";

    string job_id = createJob.getString("job_id");

    if (job_id.empty()) return "";

    string result;
    auto timeStart = high_resolution_clock::now();
    int timeout = 300;
    while (!isStopped()) {
        if (Common::hasExceededTimeout(timeStart, timeout))
            break;

        delay(2);
        JSON jobResult = API::OMOCaptcha::getJobResult(job_id);
        if (jobResult.getString("status") == "" || jobResult.getString("status") == "running" ||
                jobResult.getString("status") == "waiting")
            continue;
        if (jobResult.getString("status") == "fail")
            return "";
        if (jobResult.getString("status") == "success") {
            result = jobResult.getString("result");
            break;
        }
    }

    return result;
}

Task TiktokExt::handleLogin() {
    if (setting.isLogoutAfterInteract)
        clearDataAppAction(social.package);

    if (!openSocial(!(setting.isLogoutAfterInteract))) {
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

    if(setting.loginType == LoginType::RANDOM)
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
    for (int loop = 35; loop > 0; loop--) {
        updateLine("Login Tiktok - " + to_string(loop));
        delay(1);
        xml = dumpXml();
        string screen = handleScreen(xml);
        if (screen == "CONTINUE") continue;
        if (screen == "ERROR") return Task::FAIL;

        if (account.password.empty() && (screen == "TIK_LOGIN" || screen == "TIK_LOGIN_EMAIL")) {
            saveStatusAcc("Login", "Password empty");
            delay(5, "Password empty");
            return Task::FAIL;
        }

        if (existNode("//*[@text='No internet connection']", xml)) {
            account.status = AccountStatus::NONE;
            saveStatusAcc("Login block", "No internet connection");
            return Task::FAIL;
        }

        if (existNode("//*[@text='Please try again or log in with a different method.']", xml)) {
            if (login.isSwtichMethod || account.email.empty()) {
                saveStatusAcc("Login block", "Please try again or log in with a different method.");
                return Task::FAIL;
            }
            login.loginUid = account.email;
            login.isSwtichMethod = true;
            continue;
        }

        if (screen == "TIK_HOME") {
            string xpathTap = existNodes(0, xml, {"//*[@text='Sign up']",
                                                  "//*[@text='Profile']"
            });
            if (!xpathTap.empty()) {
                tap(xpathTap, xml);
                delay(2);
                continue;
            }
        }

        if (screen == "TIK_PROFILE") {
            string username = Common::regexGroups1(xml, "\"@([^\\s]+)\"", -1);
            if (username.empty()) {
                delay(3, "UID empty");
                continue;
            }

            if (account.uid.empty() && login.isLoginStep)
                account.uid = username;

            if (username == account.uid) {
                vector<string> lstText = getListText(xml);
                if (lstText.size() > 0) {
                    string name = lstText.at(0);
                    if (name != "12") {
                        account.displayName = name;
                    }
                }
                account.status = AccountStatus::LIVE;
                account.isLogin = true;
                return Task::DONE;
            }

            if (!Common::containsItem(account.lstUid, username)) {
                handleLogout();
                continue;
            }
// tap đổi nick
            bool isTapSwitchAccount = false;
            vector<Node> lstNodes = getNodes(
                    "//android.widget.FrameLayout/android.view.ViewGroup/android.widget.LinearLayout/child::*",
                    xml);
            if (lstNodes.empty()) continue;
            for (int i = lstNodes.size() - 1; i >= 0; i--) {
                Node node = lstNodes.at(i);
                if (node.width > node.height) {
                    tap(node.right - 50, node.bottom - Common::ranNumber(5, 10));
                    delay(2);
                    xml = dumpXml();
                    if (!existNode("//*[@text='Switch account']", xml, 3))
                        performAction(Perform::BACK, 1, 1);
                    else
                        isTapSwitchAccount = true;

                    delay(2);
                    break;
                }
            }

            if(!isTapSwitchAccount){
                xml = dumpXml();
                string xpathSwitchAccount = existNodes(0,xml,{
                        "//*[@resource-id='com.ss.android.ugc.trill:id/jwa' or @resource-id='com.ss.android.ugc.trill:id/jzf' or @resource-id='com.ss.android.ugc.trill:id/k76' or @resource-id='com.ss.android.ugc.trill:id/k81' or @resource-id='com.zhiliaoapp.musically:id/jko' or @resource-id='com.ss.android.ugc.trill:id/kfe' or @resource-id='com.ss.android.ugc.trill:id/kg0']",
                        "//*[@class='android.widget.LinearLayout']//*[@class='android.widget.LinearLayout']//*[@class='android.widget.RelativeLayout']//*[@class='android.widget.LinearLayout']//*[@class='android.widget.FrameLayout']//*[@class='android.widget.Button']"
                });

                if (!xpathSwitchAccount.empty()) {
                    tap(xpathSwitchAccount);
                    delay(2);
                    if (!existNode("//*[@text='Switch account']", xml, 3))
                        performAction(Perform::BACK, 1, 1);
                }
                delay(2);
            }
//
            continue;
        }

        if (screen == "TIK_SIGN_UP" || screen == "TIK_POPUP_LOGIN" || screen == "TIK_SIGN_UP_BIRTHDAY") {
            tap("//*[@text='Already have an account? Log in']", xml);
            delay(2);
            continue;
        }
        if (screen == "TIK_LOGIN") {
            tap("//*[@text='Use phone number/email address/username' or"
                " @text='use phone/email/username' or"
                "@text='Use phone/email/username' or "
                "@text='use phone / email / username']",
                xml);
            delay(2);
            continue;
        }
        if (screen == "TIK_LOGIN_EXIST_ACCOUNT") {
            tap("//*[@text='Login' or @text='Log in']", xml);
            delay(2);
            continue;
        }
        if (screen == "TIK_LOGIN_PHONE") {
            tap(
                    "//*[@text='Email / Username' or"
                    " @text='Email address/Username' or"
                    " @text='email/username']",
                    xml);
            delay(2);
            continue;
        }
        if (screen == "TIK_LOGIN_EMAIL") {
            bool isScreenOnlyUser = existNode("//*[@text='Next' or @text='Continue']",
                                              xml);
            vector<Node> lstNodes = getNodes("//android.widget.EditText", xml);
            if (lstNodes.empty()) {
                continue;
            }
            if (isScreenOnlyUser) {
                //chỉ nhập user
                inputText(lstNodes.back(), login.loginUid);
                delay(1);
            } else {
                inputText(lstNodes[0], login.loginUid);
                delay(1);

                if (lstNodes.size() > 1) {
                    inputText(lstNodes[1], account.password);
                    delay(1);
                }
            }

            xml = dumpXml();
            string xpathLogin = existNodes(0, xml, {
                    "//*[@text='Next' or @text='Continue']",
                    "(//*[@text='Log in'])[last()]"
            });
            if (!xpathLogin.empty()) {
                tap(xpathLogin);
                delay(isScreenOnlyUser ? 2 : 5);
            }

            login.isLoginStep = true;
            continue;
        }
        if (screen == "TIK_LOGIN_CHECK_YOUR_EMAIL" || screen == "TIK_ACCOUNT_OTP_CHANGEPASS") {
            if (!login.isSelectLoginWithPassword) {
                login.isSelectLoginWithPassword = true;
                tap("//*[@text='Log in with password']");
                delay(2);
                continue;
            }

            login.maxGetOtp--;
            if (login.maxGetOtp < 0 ||
                existNode("//*[@text='Session expired. Exit the app and try again.']",
                          xml)) {
                account.status = AccountStatus::CHECKPOINT;
                saveStatusAcc("Login", login.maxGetOtp < 0 ? "Get OTP FAIL"
                                                           : "Session expired. Exit the app and try again.");
                return Task::FAIL;
            }

            updateLine("Get otp");
            string otp = getOtpFromEmail(account.email, 60, login.oldOtp);
            if (otp.empty()) {
                tap("//*[@text='Resend code']", xml);
                delay(2);
                continue;
            }
            login.oldOtp = otp;

            updateLine("Input otp");
            Node node = getNode(
                    "(//android.widget.EditText[@class='android.widget.EditText'])/parent::*", xml);
            delay(2);
            inputText(node, otp);
            delay(10, "Input otp");
            continue;
        }
        if (screen == "TIK_LOGIN_PASSWORD") {
            if (existNode("//*[contains(@text, 'Incorrect account or password')]", xml)) {
                account.status = AccountStatus::WRONG;
                saveStatusAcc("Login", "Incorrect password");
                return Task::FAIL;
            }
            inputText("//android.widget.EditText", account.password);
            delay(1);

            tap("//*[@text='Log in' or @text='Continue']", xml);
            delay(5);
            login.isLoginStep = true;
            continue;
        }
        if (screen == "TIK_LOGIN_SET_PASSWORD") {
            account.status = AccountStatus::WRONG;
            saveStatusAcc("Login", "Reset password");
            return Task::FAIL;
        }
        if (screen == "TIK_LOGIN_2FA_EMAIL") {
            tap("//*[@text='Try another method']", xml);
            delay(2);
            continue;
        }
        if (screen == "TIK_LOGIN_2FA") {
            if (account.twoFA.empty()) {
                saveStatusAcc("Login", "Empty 2FA");
                return Task::FAIL;
            }

            login.maxInput2FA--;
            if (login.maxInput2FA < 0) {
                saveStatusAcc("Login", "Wrong 2FA");
                return Task::FAIL;
            }

            Node node = getNode("//android.widget.EditText", xml);
            string totp = API::get2FA(account.twoFA);
            inputText(node, totp);
            delay(1);
            tap("//*[@text='Next' or @text='Continue']");
            delay(5);
            continue;
        }
        if (screen == "TIK_LOGIN_BLOCK") {
            saveStatusAcc("Login", "Block login");
            return Task::FAIL;
        }
        if (screen == "TIK_ACCOUNT_DIE") {
            account.status = AccountStatus::DIE;
            saveStatusAcc("Login", "Account die");
            return Task::FAIL;
        }
        if (screen == "TIK_LOGIN_VERIFY_CONFIRM") {
            if (!tap("//*[contains(@text,'email:') or contains(@text,'.com')]", xml)) {
                if (login.isSelectLoginWithPassword) {
                    openSocial();
                    continue;
                }

                account.status = AccountStatus::CHECKPOINT;
                saveStatusAcc("Login", "Verify identity fail");
                return Task::FAIL;
            }

            delay(1);

            tap("//*[@text='Next']", xml);

            delay(2);
            continue;
        }
        if (screen == "TIK_LOGIN_VERIFY_OTP") {
            if (account.email.empty()) {
                account.status = AccountStatus::INVALIDATE;
                saveStatusAcc("Login", "Email empty");
                return Task::FAIL;
            }

            if (!existNode("//android.widget.EditText", xml))continue;

            login.maxGetOtp--;
            if (login.maxGetOtp < 0 ||
                existNode(
                        "//*[@text='Session expired. Exit the app and try again.']", xml)) {
                account.status = AccountStatus::CHECKPOINT;
                saveStatusAcc("Login", login.maxGetOtp < 0 ? "Get OTP FAIL"
                                                           : "Session expired. Exit the app and try again.");
                return Task::FAIL;
            }

            updateLine("Get otp");
            string otp = getOtpFromEmail(account.email, 60, login.oldOtp);
            if (otp.empty()) {
                tap("//*[@text='Resend code']", xml);
                delay(2);
                continue;
            }
            login.oldOtp = otp;

            updateLine("Input otp");
            Node node = getNode("//android.widget.EditText", xml);
            delay(3);
            inputText("(//android.widget.EditText[@class='android.widget.EditText'])/parent::*",
                      otp);
            delay(10, "Input otp");
            continue;
            continue;
        }
        if (screen == "TIK_LOGIN_UPDATE") {
            saveStatusAcc("Login", "Tiktok requires update");
            return Task::FAIL;
        }
        if (screen == "TIK_LOGIN_NO_INTERNET") {
            saveStatusAcc("Login", "No internet");
            return Task::FAIL;
        }
        if (screen == "TIK_UPLOAD_PHOTOS") {
            tap("//*[@content-desc='Close']", xml);
            delay(2);
            continue;
        }

        string xpath = existNodes(0, xml, {
                "//*[@text='Switch account']",
                "//*[@text='Use authenticator app']"
        });
        if (xpath == "//*[@text='Switch account']") {
            if (!account.uid.empty() && existNode("//*[@text='" + account.uid + "']", xml)) {
                Node node = getNode("//*[@text='" + account.uid + "']", xml);
                tap(Common::ranNumber(node.left, node.right), node.top + 5);
                delay(5, "Switch account");
                continue;
            }
            if (existNode("//*[@text='Add account']", xml)) {
                Node node = getNode("//*[@text='Add account']", xml);
                tap(Common::ranNumber(node.left, node.right), node.top + 5);
                delay(2, "Add account");
                continue;
            }
            if (swipeAndScreenNotChange()) {
                string notExistsUser;

                //get list user
                vector<string> lstUser = getListText(xml);
                lstUser = Common::exceptList(lstUser, account.lstUid);
                for (const string &item: lstUser) {
                    if (!Common::strContains(item, " ") && item != "close") {
                        notExistsUser = item;
                        break;
                    }
                }

                if (notExistsUser.empty()) {
                    performAction(Perform::BACK);
                } else {
                    //lọc ra user không tồn tại
                    Node node = getNode("//*[@text='" + notExistsUser + "']", xml);
                    tap(Common::ranNumber(node.left, node.right), node.bottom - 5);
                    delay(5);
                }
                handleLogout();
            }
            continue;
        }
        if (xpath == "//*[@text='Use authenticator app']") {
            tap(xpath, xml);
            delay(2);
            continue;
        }
        if (screen == "TIK_SETTING_PRIVACY") {
            if (xpath.empty())
                swipe(1);
            continue;
        }

        delay(1);
    }

    return Task::FAIL;
}

bool TiktokExt::handleLogout() {
    string currentUser;
    string xml;

    string screen = handleScreen(xml);

    if (screen == "ERROR") return false;

    if (screen != "TIK_HOME" && screen != "TIK_PROFILE")
        gotoHome(xml);

    for (int loop = 30; loop > 0; loop--) {
        if (isStopped())
            return false;

        xml = dumpXml();
        screen = handleScreen(xml);
        if (screen == "CONTINUE") continue;
        if (screen == "ERROR") return false;
        if (screen == "TIK_HOME") {
            string xpathTap = existNodes(0, xml, {"//*[@text='Sign up']",
                                                  "//*[@text='Profile']"
            });
            if (xpathTap == "//*[@text='Sign up']")
                return true;
            if (xpathTap == "//*[@text='Profile']") {
                tap(xpathTap, xml);
                delay(2);
                continue;
            }
        }
        if (screen == "TIK_PROFILE") {
            string username = Common::regexGroups1(xml, "\"@([^\\s]+)\"", -1);
            if (username.empty())
                continue;

            if (currentUser.empty())
                currentUser = username;

            if (currentUser != username)
                return true;

            tap(
                    "(//android.widget.FrameLayout/android.view.ViewGroup/android.widget.LinearLayout/child::*)[last()]",
                    xml);
            delay(2);
            continue;
        }
        if (screen == "TIK_PROFILE_MENU") {
            tap("//*[@text='Settings and privacy']", xml);
            delay(2);
            continue;
        }
        if (screen == "TIK_LOGOUT") {
            tap("//*[@text='Log out']", xml);
            delay(2);

            if (existNode("//*[@text='Log out?']", xml)) {
                delay(3);
                return true;
            }
            continue;
        }

        if (Common::strStartsWith(screen, "TIK_SIGN_UP") ||
            Common::strStartsWith(screen, "TIK_LOGIN"))
            return true;

        string xpath = existNodes(0, xml, {
                "//*[@text='Settings and privacy']"
        });

        if (xpath == "//*[@text='Settings and privacy']") {
            if (tap("//*[@text='Log out']", xml)) {
                delay(2);
                continue;
            }
            swipe(3);
            continue;
        }

        delay(1);
    }
    return false;
}


bool TiktokExt::checkEmailGenerator(string email) {
    string domain = Common::strSplit(email, "@")[1];
    return Common::containsItem({
                                "reviewfood.vn",
                                "chupanhcuoidep.com",
                                "chupanhcuoidep.vn",
                                "haanhwedding.com",
                                "haanhwedding.vn",
                                "maxseeding.com",
                                "maxseeding.vn",
                                "smakit.vn",
                        }, domain);
}

std::pair<bool, string>
TiktokExt::buyGmail(const string &user, const string &pass, const string &productId,
                    const int &typeMail) {
    if (typeMail == 1) {
        string response = API::Mail::buyMail1989(user, pass, productId);

        if (!Common::strContains(response, "status"))
            return {false, Common::encodeUnicode(response)};

        return {true, Common::strTrim(JSON(response).getString("data.lists.0.account"))};
    } else if (typeMail == 2) {
        JSON jResponse = API::Mail::buyUnlimitMail(user, productId);

        if(jResponse.isEmpty()) return {false, "Buy UnlimitMail fail"};

        if (!jResponse.getBool("status")) return {false, jResponse.toString()};

        string email = jResponse.getString("data.0.email");

        string pass = jResponse.getString("data.0.password");
        return {true, email + "|" + pass};
    } else
        return {false, "Type mail not support"};
}

string TiktokExt::getOtpFromEmail(const string &email, int timeout, const string &oldOtp) {
    if (Common::strContains(email, "hotmail")
        || Common::strContains(email, "outlook")) {

        if(account.refreshTokenMail.empty() || account.clientIdMail.empty()){
            Base::delay(10, "Invalid Hotmail - Please input OAuth2");
            return "";
        }
        return API::Mail::getOtpHotmail(email, account.refreshTokenMail,account.clientIdMail);
    }
    else if (Common::checkEmail1secmail(email))
        return getOtp1secmail(email, timeout, oldOtp);
    else if (checkEmailGenerator(email))
        return getOtpGenerator(email, timeout, oldOtp);
    else {
        Base::delay(10, "Unsupport Email - Contact to admin");
    }
    return "";
}


string TiktokExt::getOtp1secmail(const string &email, int timeout, const string &oldOtp) {
    string mail = Common::strSplit(email, "@")[0];
    string domain = Common::strSplit(email, "@")[1];

    string otp;
    auto timeStart = high_resolution_clock::now();
    while (!isStopped()) {
        JSON responseList = API::TempMail::getListOtp1secmail(mail, domain);
        auto array = responseList.getArray();
        for (auto json: array) {
            string subject = json.getString("subject");
            otp = Common::regexGroups1(subject, "(\\d+) ");

            if (otp.empty() && !json.getString("id").empty()) {
                JSON response = API::TempMail::getOtp1secmail(mail,json.getString("id"));

                string body = response.getString("body");
                otp = Common::regexGroups1(body, ">(\\d{4,})<");
                if (otp.empty())
                    otp = Common::strReplace(Common::regexGroups1(body,
                                                  "(https://api.tiktokv.com/passport/email/authentication/index/.*?)\""),
                                     "&amp;", "&");
            }

            if (otp.empty())
                continue;

            if (otp != oldOtp)
                return otp;

            otp = "";
            break;
        }

        if (Common::hasExceededTimeout(timeStart, timeout))
            break;
        delay(3);
    }
    return otp;
}

string TiktokExt::getOtpGenerator(const string &email, int timeout, const string &oldOtp) {
    string mail = Common::strSplit(email, "@")[0];
    string domain = Common::strSplit(email, "@")[1];
    if (mail.size() > 25)
        mail = mail.substr(0, 25);

    string otp;
    auto timeStart = high_resolution_clock::now();

    string cookie = "surl=" + domain + "/" + mail + ";";

    while (!isStopped()) {
        string rq = API::TempMail::getOtpGenerator(cookie);

        vector<string> lstMessage = Common::regexGroups(rq, "href=\"/" + domain + "/" + mail +
                                                    "/(.*?)\"",
                                                1);
        if (lstMessage.empty()) {
            //delete message

            API::TempMail::deleleMailGenerator("surl=" + domain + "/" + mail + ";","delll=" + Common::regexGroups1(rq, "delll: \"(.*?)\""));


            otp = Common::regexGroups1(rq, ">(\\d{4,})<");
            if (otp.empty())
                otp = Common::strReplace(Common::regexGroups1(rq,
                                              "(https://api.tiktokv.com/passport/email/authentication/index/.*?)\""),
                                 "&amp;", "&");
            if (!otp.empty()) {
                if (otp != oldOtp)
                    return otp;

                otp = "";
            }
        } else {
            for (int i = 0; i < lstMessage.size(); ++i) {
                cookie = "surl=" + domain + "/" + mail + "/" + lstMessage.at(i) + ";";

                rq = API::TempMail::getOtpGenerator(cookie);

                API::TempMail::deleleMailGenerator("surl=" + domain + "/" + mail + ";"
                                                  ,"delll=" + Common::regexGroups1(rq, "delll: \"(.*?)\""));


                otp = Common::regexGroups1(rq, ">(\\d{4,})<");
                if (otp.empty())
                    otp = Common::strReplace(Common::regexGroups1(rq,
                                                  "(https://api.tiktokv.com/passport/email/authentication/index/.*?)\""),
                                     "&amp;", "&");
                if (!otp.empty()) {
                    if (otp != oldOtp)
                        return otp;

                    otp = "";
                    break;
                }
            }
        }

        if (Common::hasExceededTimeout(timeStart, timeout))
            break;
        delay(3);
    }

    return
            otp;
}

std::tuple<bool, JSON> TiktokExt::setupTDTT() {
    if (jobSetting.tokenTDTT.empty()) {
        saveStatusAcc("TDTT", "Not-found TDTT", 10);
        return std::make_tuple(false, JSON{});
    }

    updateLine("Setup TDTT");

    JSON json{};

    json.set("uidId", account.uid);

    string resultSetNick = RequestTDTT("config-uid", json.toString());

    JSON response{resultSetNick};

    bool isSuccess = response.getBool("success");

    if (!isSuccess) {
        delay(5, "Cấu hình nick Fail");
        saveStatusAcc("TDTT", "Setup failed");
        Common::NLog(resultSetNick, "actionEarnMoney");
    }

    return std::make_tuple(isSuccess, response);
}

void TiktokExt::ResetVariable(){}