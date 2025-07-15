//
// Created by HieuThuHai on 9/13/2024.
//

#include "FacebookExt.h"

Task FacebookExt::handleLogin() {
    struct LoginStruct {
        int maxInput2FA = 3;
        string loginUid;
        string oldOtp;
        int maxGetOtp = 2;
        bool isSelectLoginWithPassword = false;
        bool isSwitchMethod = false;
        bool isLoginStep = false;
    } login;

    if (!account.uid.empty()) {
        login.loginUid = account.uid;
    } else {
        login.loginUid = account.email;
        login.isSwitchMethod = true;
    }

    //---------------------
    int maxTapLogin = 3;

    int maxEnter2FA = 3;

    int maxNoInternet = 3;

    int maxTryAgain = 3;
    int maxRetry = 3;

    int maxCaptcha = 0;

    int countGetOtp = 0;
    int maxGetOtp = 3;

    string oldOtp;

    if (!openSocial()) {
        if (!isStop)
            saveStatusAcc("Login", "Open Package Fail");
        return Task::FAIL;
    }

    auto timeStart = high_resolution_clock::now();
    int timeout = 300;
    while (true) {
        if (IsStop())
            break;

        if (Common::hasExceededTimeout(timeStart, timeout))
            break;

        string xml = dumpXml();

        if (existNode("//*[@text='choose a way to confirm it’s you' or @content-desc='choose a way to confirm it’s you']",xml)) {
            tap("//*[@content-desc='authentication app, get a code from your authentication app.']");
            delay(2);
            tap("//*[@text='continue' or @content-desc='continue']");
            delay(2);
            continue;
        }

        string screen = handleScreen(xml);
        if (Common::strContains(screen, "_POPUP_"))
            continue;

        if (IsCheckpoint()) {
            account.status = AccountStatus::CHECKPOINT;
            saveStatusAcc("Login", "Checkpoint");
            return Task::FAIL;
        }

        if (IsLogined()) {
            account.status = AccountStatus::LIVE;
            account.isLogin = true;
            return Task::DONE;
        }
        if (screen == "FB_LOGIN_NOTFOUND_ACCOUNT") {
            account.status = AccountStatus::WRONG;
            saveStatusAcc("Login", "Incorrect username");
            return Task::FAIL;
        }
        if (screen == "META_LOGIN_INCORRECT_PASSWORD") {
            account.status = AccountStatus::WRONG;
            saveStatusAcc("Login", "Incorrect password");
            return Task::FAIL;
        }
        if (screen == "FB_KEEP_YOUR_ACCOUNT_SAFE") {
            saveStatusAcc("Login", "Require enable 2FA");
            return Task::FAIL;
        }
        if (screen == "FB_LOGIN") {
            if (account.password.empty()) {
                saveStatusAcc("Login", "Password empty");
                return Task::FAIL;
            }

            maxTapLogin--;
            if (maxTapLogin < 0)
                break;

            if (getNodes("//android.widget.EditText", xml).size() < 2)
                continue;

            if (login.loginUid.empty())
                return Task::FAIL;
            inputText("(//android.widget.EditText)[1]", login.loginUid);
            delay(1);
            tap("(//android.widget.EditText)[1]");
            delay(1);
            hideKeyboard();

            inputText("(//android.widget.EditText)[2]", account.password);
            delay(1);
            tap("(//android.widget.EditText)[2]");
            delay(1);
            hideKeyboard();

            if (tapWait("//*[@text='Log in' or @content-desc='Log in' or @text='Log into my account' or @content-desc='Log into my account' or @content-desc='Log into existing account' or @text='Log into existing account']"))
                delay(5);
            continue;
        }
        if (screen == "FB_LOGIN_CONFIRM_OTP_EMAIL") {
            account.status = AccountStatus::CHECKPOINT;
            saveStatusAcc("Login", "Otp Email");
            return Task::FAIL;

            //string otp = EmailHelper.GetOtpFromEmail(0, email, passMail, oauth2, 60, "", oldOtp);
            //
            //if (otp.empty()) {
            //    if (countGetOtp < maxGetOtp) {
            //        countGetOtp++;
            //        tap("//*[@content-desc='I didn’t get the code']", "", 5);
            //        tap("//*[@content-desc='Resend confirmation code']", "", 5);
            //        continue;
            //    }
            //    break;
            //} else {
            //    oldOtp = otp;
            //    inputText("//android.widget.EditText", otp);
            //    delay(2);
            //    tap("//*[@content-desc='Next']", "", 5);
            //    continue;
            //}
        }

        if (screen == "META_LOGIN_ENTER_2FA") {
            if (account.twoFA.empty()) {
                saveStatusAcc("Login", "Empty 2FA");
                return Task::FAIL;
            }

            maxEnter2FA--;
            if (maxEnter2FA < 0) {
                if (maxTryAgain < 3)
                    break;
                saveStatusAcc("Login", "Wrong 2FA");
                return Task::FAIL;
            }

            string otp = API::get2FA(account.twoFA);
            if (otp.empty())
                continue;

            tap("//android.widget.EditText");
            delay(1);
            inputText("//android.widget.EditText", otp);
            delay(1);

            if (tap("//*[@content-desc='Continue' or @text='Continue']"))
                delay(5);
            continue;
        }
        if (screen == "FB_LOGIN_APPROVE_2FA_FAIL" || screen == "FB_PAGE_NOT_AVAILABLE") {
            maxTryAgain--;
            if (maxTryAgain < 0)
                break;

            closeAppAction(social.package);
            openApp(social.package);
            continue;
        }
        if (screen == "FB_UNABLE_TO_LOGIN") {
            maxTryAgain--;
            if (maxTryAgain < 0)
                break;

            tap("//*[contains(@text,'OK')]", xml);
            delay(2);
            continue;
        }
        if (screen == "FB_LOGIN_BLOCK") {
            saveStatusAcc("Login", "Block login");
            return Task::FAIL;
        }
        if (screen == "FB_CONNECT_LOST") {
            maxNoInternet--;
            if (maxNoInternet < 0) {
                saveStatusAcc("Login", "No internet connection");
                return Task::FAIL;
            }

            tap("//*[contains(@text,'Tap to retry')]", xml);
            delay(2);
            continue;
        }
        if (screen == "FB_SESSION_EXPIRED") {
            clearDataAppAction(social.package);
            openApp(social.package);
            continue;
        }
        if (screen == "FB_CREATE_STORY") {
            tap("//*[contains(@content-desc,'Close')]", xml);
            delay(2);
            continue;
        }
        if (screen == "FB_LOGIN_CAPTCHA") {
            maxCaptcha--;
            if (maxCaptcha < 0) {
                saveStatusAcc("Login", "Captcha");
                return Task::FAIL;
            }

            //string elImage = existNodes(0, xml, {
            //        "//*[@text=\"Can't read this?\"]/preceding::*[contains(@class,'android.widget.TextView')][1]",
            //        "//android.widget.EditText/preceding::*[contains(@class,'android.widget.Image')]",
            //});
            //if (elImage.empty()) {
            //    //Không lấy được ảnh captcha hoặc dạng captcha khác
            //    saveStatusAcc("Login", "Captcha");
            //    return Task::FAIL;
            //}
            //string bounds = GetNodeBounds(0, xml, elImage).First();
            //string base64 = ScreenShot(bounds);
            //if (base64.empty()) {
            //    tap("//*[@text='Generate new']", xml);
            //    delay(2);
            //    continue;
            //}
            //
            //string captcha = ByPassCaptchaText(4, "", base64);
            //if (captcha.empty()) {
            //    tap("//*[@text='Generate new']", xml);
            //    delay(2);
            //    continue;
            //}
            //
            //for (int i = 0; i < 10; i++) {
            //    tap("//android.widget.EditText");
            //    delay(1);
            //    inputText("//android.widget.EditText", captcha);
            //    delay(1);
            //    xml = "";
            //    if (existNode("//android.widget.EditText[@text='" + captcha + "']", xml))
            //        break;
            //}
            //
            //for (int i = 0; i < 10; i++) {
            //    tap("//*[@content-desc='Continue' or @text='Continue']");
            //    delay(2);
            //    xml = "";
            //    if (!existNode("//android.widget.EditText[@text='" + captcha + "']", xml))
            //        break;
            //}
            //continue;
        }
        if (screen == "FB_LOGIN_CAPTCHA_OK") {
            if (maxCaptcha < 10) {
                saveStatusAcc("Login", "Something went wrong");
                return Task::FAIL;
            }
            tap("//*[@text='OK' or @content-desc='OK']", xml);
            delay(2);
            continue;
        }

        //check element
        string element = existNodes(0, xml, {
                "//*[contains(@content-desc,', tab ')]/preceding-sibling::*[last()]",
                "//*[@text='•' or @content-desc='•']",
                "//*[@content-desc='Continue']",
                "//*[@content-desc='Deny']",
                "//*[@text='NOT NOW']",
                "//*[@content-desc='Retry']",
                "//*[@content-desc='Next']",
                "//*[@content-desc='Skip']",
                "//*[@text='SKIP']",
                "//*[@content-desc='Log in']", // màn 475
        });

        if (element == "//*[contains(@content-desc,', tab ')]/preceding-sibling::*[last()]") {
            if (getNode(element, xml).selected) {
                account.status = AccountStatus::LIVE;
                account.isLogin = true;
                return Task::DONE;
            }
            continue;
        }
        if (element == "//*[@text='•' or @content-desc='•']") {
            maxTryAgain--;
            if (maxTryAgain < 0) {
                saveStatusAcc("Login", "Fail");
                return Task::FAIL;
            }

            back();
            openApp(social.package);
            delay(2);
            continue;
        }
        if (element == "//*[@content-desc='Continue']" ||
            element == "//*[@content-desc='Deny']" ||
            element == "//*[@text='NOT NOW']" ||
            element == "//*[@content-desc='Next']" ||
            element == "//*[@content-desc='Skip']" ||
            element == "//*[@text='SKIP']" ||
            element == "//*[@content-desc='Log in']") {
            tap(element, xml);
            delay(2);
            continue;
        }
        if (element == "//*[@content-desc='Retry']") {
            maxRetry--;
            if (maxRetry < 0) {
                back();
                saveStatusAcc("Login", "Fail");
                return Task::FAIL;
            }
            if (existNode("//*[@content-desc='Back']", xml))
                element = "//*[@content-desc='Back']";
            tap(element, xml);
            delay(2);
            continue;
        }

        delay(1);
    }

    return Task::DONE;
//    return Task::FAIL;
}

bool FacebookExt::handleLogout() {
    return false;
}

bool FacebookExt::checkLiveUid(string uid) {
    return false;
}

bool FacebookExt::isUid(string id) {
    if (!Common::strIsNumber(id))
        return false;

    if (Common::strStartsWith(id, "1000") && id.length() == 15)
        return true;

    if (Common::strStartsWith(id, "615"))
        return true;

    if (id.length() <= 10)
        return true;
    return false;
}

bool FacebookExt::gotoLink(const string &link) {
    int maxAgain = 3;

    Start:
    maxAgain--;
    if (maxAgain < 0)
        return false;

    navigateFB(link);
    delay(2);

    string xml = dumpXml();
    string screen = handleScreen(xml);
    if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
        return false;
    if (Common::strContains(screen, "_POPUP_"))
        goto Start;
    if (screen == "FB_SESSION_EXPIRED")
        goto Start;
    if (screen == "FB_PAGE_NOT_AVAILABLE")
        return false;

    if (Common::strStartsWith(link, "fb://faceweb/f?href=https://facebook.com/sharer.php?u=")) {
        delay(3);
        return true;
    }

    if (Common::strContains(link, "/stories/")) {
        if (existNode("(//*[@text='Stories'])[2]", xml))
            return true;
    }
    return true;
}

bool FacebookExt::gotoHome() {
    for (int i = 0; i < 10; i++) {
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "FB_SESSION_EXPIRED")
            continue;

        if (screen == "FB_LIVESTREAM") {
            back();
            continue;
        }
        if (screen == "FB_POST") {
            if (existNode(
                    "//*[(contains(@content-desc,'Home, tab 1 of ') or contains(@content-desc,'Feed, tab 1 of ')) and @selected='true']",
                    xml))
                return true;
            back();
            continue;
        }

        if (screen == "FB_FEED")
            return true;

        string element = existNodes(0, xml, {
                "//*[contains(@content-desc,'Feed, tab ')]",
                "//*[contains(@content-desc,', tab ')]/preceding-sibling::*[last()]",
                "//*[@NAF='true']/following-sibling::*[@NAF='true']",//giao diện danh sách comment của post
                "//android.widget.EditText[@text='Write a comment…']",//giao diện danh sách comment của post
        });

        if (element == "//*[contains(@content-desc,'Feed, tab ')]") {
            tap(element, xml);
            delay(1);
            continue;
        }
        if (element == "//*[contains(@content-desc,', tab ')]/preceding-sibling::*[last()]") {
            if (getNode(element, xml).selected)
                return true;
        }
        if (element == "//*[@NAF='true']/following-sibling::*[@NAF='true']" ||
            element == "//android.widget.EditText[@text='Write a comment…']") {
            back();
            continue;
        }

        if (!gotoLink("fb://feed/scroll_to_top"))
            break;
    }

    return false;
}

bool FacebookExt::gotoMenu() {
    int countTapReloadMenu = 0;
    int maxTapReloadMenu = 5;
    for (int i = 0; i < 10; i++) {
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "FB_SESSION_EXPIRED")
            continue;

        if (Common::strStartsWith(screen, "FB_MENU"))
            return true;

        string element = existNodes(0, xml, {
                "//*[@content-desc='Reload Menu']",
                "//*[contains(@content-desc,'Menu, tab ')]",
        });

        if (element == "//*[@content-desc='Reload Menu']") {
            if (countTapReloadMenu >= maxTapReloadMenu) {
                return false;
            }
            tap(element, xml);
            countTapReloadMenu++;
            continue;
        }

        if (element == "//*[contains(@content-desc,'Menu, tab ')]") {
            tap(element, xml);
            delay(1);
            continue;
        }

        if (!gotoLink("fb://bookmarks"))
            break;
    }

    return false;
}

bool FacebookExt::gotoFriends() {
    for (int i = 0; i < 10; i++) {
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_"))
            continue;
        if (screen == "FB_SESSION_EXPIRED")
            continue;

        if (screen == "FB_FRIEND")
            return true;

        string element = existNodes(0, xml, {
                "//*[contains(@content-desc,'Friends, tab ')]",
        });

        if (element == "//*[contains(@content-desc,'Friends, tab ')]") {
            tap(element, xml);
            delay(1);
            continue;
        }

        if (!gotoLink("fb://friends/requests_tab"))
            break;
    }

    return false;
}

bool FacebookExt::gotoFriendsList(int typeMethod) {
    if (typeMethod == 1) {
        Start:
        if (!gotoFriends())
            return false;

        for (int i = 0; i < 10; i++) {
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                goto Start;

            //check screen
            if (screen == "FB_FRIEND_LIST")
                return true;
            if (screen == "FB_FRIEND") {
                tap("//*[@content-desc='Your friends']", xml);
                delay(2);
                continue;
            }
            openApp(social.package);
            delay(2);
        }
        return false;
    } else {
        if (!gotoLink("fb://friends"))
            return false;

        return waitScreen("FB_FRIEND_LIST", "", 60);
    }
}

bool FacebookExt::gotoGroupsList() {
    if (!gotoGroup())
        return false;

    bool isSuccess = tap("//*[@content-desc='Your groups']", "", 10);
    if (isSuccess)
        delay(2);
    return isSuccess;
}

bool FacebookExt::gotoLinkSettingsEmail() {
    return gotoLink("fb://notification_settings_email");
}

bool FacebookExt::gotoSettings() {
    return gotoLink("fb://settings");
}

bool FacebookExt::gotoSettingsSecurity() {
    return gotoLink("fb://security_settings");
}

bool FacebookExt::gotoReel() {
    ADB::runShell("am start -n com.facebook.katana/com.facebook.fbshorts.viewer.activity.FbShortsViewerActivity");
    return true;

//    vector<string> activities = {
//            "com.facebook.katana/com.facebook.fbshorts.viewer.activity.FbShortsViewerActivity",
//            "com.facebook.katana/com.facebook.katana.immersiveactivity.ImmersiveActivity",
//    };
//
//    for (const auto& activity : activities) {
//        string xml = dumpXml();
//        string screen = handleScreen(xml);
//        ADB::runShell("am start -n " + activity);
//        delay(2);
//
//        if (screen == "FB_REEL")
//            return true;
//    }
//    return false;
}

bool FacebookExt::gotoCreateReel() {
    if (!gotoReel())
        return false; // Nếu không vào được màn hình reel, thoát luôn
    delay(2);

    auto timeStart = high_resolution_clock::now();
    int timeout = 15;  // Chờ tối đa 15 giây để vào màn hình tạo reel

    while (!Common::hasExceededTimeout(timeStart, timeout)) {
        string xml = dumpXml();

        string element = existNodes(0, xml, {
                "//*[@content-desc='Create a reel' or @content-desc='Create' or @text='Create' or @content-desc='Create reel']",
        });

        if (!element.empty()) {
            tap(element, xml);
            delay(2);
            return true;  // Thành công -> Thoát ngay
        }
    }
    return false;  // Nếu hết timeout mà vẫn chưa mở được "Create a reel"
}


bool FacebookExt::gotoCreateStory() {
    return gotoLink("https://www.facebook.com/storiescomposerforadscampaign");
}

bool FacebookExt::gotoPage(const string &id) {
    if (id.empty())
        return gotoLink("fb://pagestab");
    return gotoProfile(id);
}

bool FacebookExt::gotoProfile(const string &id) {
    if (id.empty())
        return gotoLink("fb://profile");

    string link = id;
    if (Common::strIsNumber(id)) {
        link = "fb://";
        link += isUid(id) ? "profile" : "page";
        link += "/" + id;
    }

    return gotoLink(link);
}

bool FacebookExt::gotoProfileEdit() {
    return gotoLink("fb://profile_edit");
}

bool FacebookExt::gotoGroup(const string &id) {
    string link;
    string screen;

    if (id.empty()) {
        link = "fb://groups_targeted_tab";
        screen = "FB_GROUP";
    } else {
        link = id;
        if (Common::strIsNumber(id))
            link = "fb://group/" + id;
        screen = "FB_ANOTHER_GROUP";
    }

    if (!gotoLink(link))
        return false;
    string xml = dumpXml();
    string currentScreen = handleScreen(xml);
//    return waitScreen(screen, xml, 30);
   return currentScreen == "FB_GROUP" || currentScreen == "FB_GROUP_SEARCH" || currentScreen == "FB_ANOTHER_GROUP";
}

bool FacebookExt::gotoGroupMember(const string &id) {
    if (!gotoGroup(id))
        return false;

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
        updateLine(screen);
        if (screen == "FB_GROUP_MEMBER")
            return true;

        string element = existNodes(0, xml, {
                "//android.view.ViewGroup[./android.widget.Button[contains(@content-desc, 'group') and contains(@content-desc, 'members')] and not(./android.widget.Button[contains(@content-desc, 'Only members can see')])]",
                "//*[@content-desc='Members']",
        });
        if (element == "//android.view.ViewGroup[./android.widget.Button[contains(@content-desc, 'group') and contains(@content-desc, 'members')] and not(./android.widget.Button[contains(@content-desc, 'Only members can see')])]") {
            tap(element, xml);
            delay(2);
            continue;
        }

        if (element == "//*[@content-desc='Members']") {
            if (existNode("//*[@content-desc='Members']/following::*[@content-desc='See all']", xml)) {
                tap("//*[@content-desc='Members']/following::*[@content-desc='See all']", xml);
                delay(2);
                continue;
            }
            if (swipeAndScreenNotChange())
                break;
            continue;
        }
        if (swipeAndScreenNotChange())
            break;
    }
    return false;
}

bool FacebookExt::gotoWatch() {
    return gotoLink("fb://watch");
}

bool FacebookExt::gotoSearchWatch(string tuKhoa) {
    tuKhoa = Common::strTrim(tuKhoa);
    if (tuKhoa.empty())
        return false;

    if (!gotoWatch())
        return false;

    for (int i = 0; i < 2; i++) {
        if (tap("//*[@text='Video' or @content-desc='Create reel']/following-sibling::*[@content-desc='Search videos' or @content-desc='Search']", "", 5)) {
            delay(2);
            break;
        }
        swipe(-1);
    }

    string xml;
    if (!existNode("//android.widget.EditText", xml, 5))
        return false;

    inputText("//android.widget.EditText", tuKhoa);
    delay(2);

    inputEnter();
    delay(2);

    //waitForLoaded(60);
    //*[@content-desc='Video']
    return tap("//android.view.ViewGroup/android.widget.FrameLayout/android.view.View", "", 10);
}

bool FacebookExt::gotoNotify() {
    return gotoLink("fb://notifications_tab");
}

string FacebookExt::gotoPost(string link) {
    //if (link.Contains("/videos/"))
    //    return GotoLive(link).ToString().ToLower();

    //format link
    if (Common::strContains(link, "facebook.com/reel/")) {

    } else {
        if (Common::strContains(link, "photo/?fbid=")) {
            link = Common::strReplace(link, "photo/?fbid=", "photo?fbid=");
        } else if (Common::strContains(link, "photo") || Common::strContains(link, "v=")) {
            string idPost = getIdPostFromLink(link);
            if (Common::strIsNumber(idPost))
                link = "https://m.facebook.com/" + idPost;
        } else if (Common::strContains(link, "groups")) {
            string group = Common::regexGroups1(link, "groups/(.*?)/");
            if (group.empty())
                group = Common::regexGroups1(link, "groups/(.*?)\\?");
            if (!group.empty()) {
                string postId = getIdPostFromLink(link);
                //string postId = GetIdPostFromLinkGroups(link);
                if (!postId.empty())
                    link = "https://m.facebook.com/groups/" + group + "/permalink/" + postId;
            }
        } else if (Common::strContains(link, "story_fbid")) {
            string id = Common::regexGroups1(link, "&id=(\\d+)");
            if (!id.empty()) {
                string postId = getIdPostFromLink(link);
                if (!postId.empty())
                    link = "https://m.facebook.com/" + id + "/posts/" + postId;
            }
        } else if (!Common::strStartsWith(link, "http")) {
            link = "https://m.facebook.com/" + link;
        }
    }
    //else if (linkPost.Contains("/reel/"))
    //{
    //    linkPost = linkPost.Replace(@"/reel", "");
    //}

    //open link
    string xml = dumpXml();
    if (!existNode("//*[contains(@content-desc,'Make a post on Facebook')]", xml)) {
        if (!gotoHome())
            return "false";
        xml = dumpXml();
    }

    string boundsCreatePost;
    {
        Node node = getNode("//*[contains(@content-desc,'Make a post on Facebook')]", xml);
        if (!node.empty())
            boundsCreatePost = node.bounds;
    }

    int countOpenLink = 0;
    int maxOpenLink = 3;
    int countTapToRetry = 0;
    int maxTapToRetry = 3;

    if (!gotoLink(link))
        return "false";
    delay(2);

    for (int i = 1; i < 20; i++) {
        xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_") || screen == "FB_SESSION_EXPIRED") {
            if (!gotoLink(link))
                return "false";
            delay(2);
            continue;
        }
        if (screen == "FB_PAGE_NOT_FOUND" || screen == "FB_FEED" || screen == "FB_LOGIN_MFB")
            return "die_link";
        if (screen == "FB_POST" || screen == "FB_REEL" || screen == "FB_LIVESTREAM" ||
            screen == "FB_WATCH")
            return "true";

        if (screen == "FB_ANOTHER_GROUP") {
            //if (tap("//*[starts-with(@content-desc,'Join ')]", xml))
            //{
            //    DelayRandom(2, 3);
            //    if (!GotoLink(link))
            //        return "false";
            //    delay(2);
            //    continue;
            //}
            return "true";
        }

        {
            Node node = getNode("//*[contains(@content-desc,'Make a post on Facebook')]", xml);
            if (!node.empty() && node.bounds == boundsCreatePost) {
                countOpenLink++;
                if (countOpenLink > maxOpenLink)
                    break;
                if (!gotoLink(link))
                    return "false";
                delay(2);
                continue;
            }
        }

        string xpath = existNodes(0, xml, {
                "//*[starts-with(@content-desc,'Join ')]",
                "//*[@content-desc='Post Menu']",
                "//*[@content-desc='More options' or @text='More options']",
                "//*[@content-desc='More']",
                "//*[@text='Tap to retry']",
                "//*[@content-desc='See what was used to create this reel']",
                "//*[@content-desc='Reel. Swipe up to see more.']",
                "(//*[@text='Stories'])[2]",
                "//*[@content-desc='Like']",
        });

        if (xpath == "//*[starts-with(@content-desc,'Join ')]") {
            //tap(xpath, xml);
            //DelayRandom(2, 3);
            //if (!GotoLink(link))
            //    return "false";
            //delay(2);
            //continue;
            return "true";
        }
        if (xpath == "//*[@text='Tap to retry']") {
            if (countTapToRetry >= maxTapToRetry)
                break;
            countTapToRetry++;
            tap("//*[@text='Tap to retry']", xml);
            delay(5);
            continue;
        }
        if (!xpath.empty())
            return "true";
        if (existNodes(0, xml, {
                "//*[contains(@text,'The page you requested cannot be displayed right now')]",
                "//*[@text='The page you requested was not found.']"
        }) != "")
            return "false";
        if (existNode("//*[contains(@text,'Tap to view story')]", xml))
            return "true";

        if (Common::strContains(link, "/stories/") &&
            existNode("(//*[@text='Stories'])[2]", xml, 60))
            return "true";

        swipe(-1);
    }
    return "false";
}

bool FacebookExt::gotoLive(const string &link) {
    string idVideo = getIdPostFromLink(link);
    if (idVideo.empty())
        return false;

    //open link
    string xml = dumpXml();
    if (!existNode("//*[contains(@content-desc,'Make a post on Facebook')]", xml)) {
        if (!gotoHome())
            return false;
        xml = dumpXml();
    }

    string boundsCreatePost;
    {
        Node node = getNode("//*[contains(@content-desc,'Make a post on Facebook')]", xml);
        if (!node.empty())
            boundsCreatePost = node.bounds;
    }

    int countOpenLink = 0;
    int maxOpenLink = 3;
    int countTapToRetry = 0;
    int maxTapToRetry = 3;

    if (!gotoLink("fb://fullscreen_video/" + idVideo + "?loop=0"))
        return false;
    delay(2);

    for (int i = 1; i < 30; i++) {
        xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(screen, "_POPUP_") || screen == "FB_SESSION_EXPIRED") {
            if (!gotoLink("fb://fullscreen_video/" + idVideo + "?loop=0"))
                return false;
            delay(2);
            continue;
        }
        if (screen == "FB_LIVESTREAM" || screen == "FB_POST")
            return true;
        if (screen == "FB_PAGE_NOT_FOUND")
            break;

        {
            Node node = getNode("//*[contains(@content-desc,'Make a post on Facebook')]", xml);
            if (!node.empty() && node.bounds == boundsCreatePost) {
                countOpenLink++;
                if (countOpenLink > maxOpenLink)
                    break;
                if (!gotoLink("fb://fullscreen_video/" + idVideo + "?loop=0"))
                    return false;
                delay(2);
                continue;
            }
        }

        if (existNode("//*[@text='Tap to retry']", xml)) {
            if (countTapToRetry >= maxTapToRetry)
                break;
            countTapToRetry++;
            tap("//*[@text='Tap to retry']", xml);
            delay(5);
            continue;
        }

        if (existNodes(0, xml, {
                "//*[contains(@text,'The page you requested cannot be displayed right now')]",
                "//*[@text='The page you requested was not found.']"
        }) != "")
            return false;

        //if (existNodes(0, xml, {
        //    "//*[@content-desc='Video']",
        //    "//android.widget.ProgressBar"
        //}) != "")
        //{
        //    delay(2);
        //    continue;
        //}

        if (i % 5 == 0) {
            if (!gotoLink("fb://fullscreen_video/" + idVideo + "?loop=0"))
                return false;
            delay(2);
            continue;
        }
        swipe(-1);
    }
    return false;
}

void FacebookExt::navigateFB(const string &link) {
    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();


    jmethodID navigateFBMethod = JExt::findMethod(deviceVM.jClass, "navigateFB",
                                                  "(Landroid/content/Context;Ljava/lang/String;)Z",
                                                  true);
    env->CallStaticBooleanMethod(deviceVM.jClass, navigateFBMethod, jContext,
                                 JExt::stringToJstring(link));


//    jmethodID intentConstructor = JExt::findMethod(Common::serviceVM.intentClass, "<init>",
//                                                   "(Ljava/lang/String;Landroid/net/Uri;)V");
//    jstring actionView = env->NewStringUTF("android.intent.action.VIEW");
//    jclass uriClass = JExt::findClass("android/net/Uri");
//    jmethodID parseMethod = env->GetStaticMethodID(uriClass, "parse",
//                                                   "(Ljava/lang/String;)Landroid/net/Uri;");
//
//    jstring uriString = JExt::jstringToString(uri);
//    jobject jUri = env->CallStaticObjectMethod(uriClass, parseMethod, uriString);
//
//    jobject intent = env->NewObject(Common::serviceVM.intentClass, intentConstructor, actionView, jUri);
//
//
//    jclass componentNameClass = JExt::findClass("android/content/ComponentName");
//
//    jmethodID componentConstructor = JExt::findMethod(componentNameClass, "<init>",
//                                                      "(Ljava/lang/String;Ljava/lang/String;)V");
//
//    jstring packageName = JExt::jstringToString(social.package);
//
//    jstring className = env->NewStringUTF(".IntentUriHandler");
//    jobject componentName = env->NewObject(componentNameClass, componentConstructor, packageName, className);
//
//    jmethodID setComponentMethod = JExt::findMethod(Common::serviceVM.intentClass, "setComponent",
//                                                    "(Landroid/content/ComponentName;)Landroid/content/Intent;");
//
//    env->CallObjectMethod(intent, setComponentMethod, componentName);
//
//    jint flag = env->GetStaticIntField(Common::serviceVM.intentClass,
//                                       env->GetStaticFieldID(Common::serviceVM.intentClass,
//                                                             "FLAG_ACTIVITY_NEW_TASK", "I"));
//    jmethodID addFlagsMethod = JExt::findMethod(Common::serviceVM.intentClass, "addFlags",
//                                                "(I)Landroid/content/Intent;");
//    env->CallObjectMethod(intent, addFlagsMethod, flag);
//
//    jmethodID setDataMethod = JExt::findMethod(Common::serviceVM.intentClass, "setData",
//                                                  "(Landroid/net/Uri;)Landroid/content/Intent;");
//
//    env->CallObjectMethod(intent, setDataMethod, jUri);
//
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
//    env->DeleteLocalRef(contextClass);

//    if (delayTime > 0)
//        delay(delayTime, message);
}

string FacebookExt::getIdPostFromLink(string link) {
    string id = "";
    if (!Common::strEndsWith(link, "/"))
        link += "/";

    vector<string> lstPatter = {
            "story_fbid=(.*?)&",
            "story_fbid=(.*?)/",
            "permalink/(.*?)/",
            "v=(.*?)&",
            "post_id=(.*?)&",
            "v=(.*?)/",
            "videos/.*?/(\\d+)/",
            "videos/(\\d+)/",
            "videos/(\\d+)\\?",
            "videos/(.*?)/",
            "videos/(.*?)\\?",
            "reel/(.*?)/",
            "reel/(.*?)\\?",
            "posts/.*?/(\\d+)/",
            "posts/(\\d+)/",
            "posts/(\\d+)#",
            "posts/.*?/(\\d+)\\?",
            "posts/(\\d+)\\?",
            "posts/(.*?)/",
            "posts/(.*?)\\?",
            "view_tray_pagination/(.*?)/",
            "fbid=(.*?)&",
            "multi_permalinks=(.*?)&",
            "photo\\.php\\?fbid=(\\d+)"
    };

    for (const auto &item: lstPatter) {
        if (Common::strContains(item, "(.*?)")) {
            id = Common::regexGroups1(link, Common::strReplace(item, "(.*?)", "(\\d+)"), 0);
            if (!id.empty() && !Common::strStartsWith(id, "0"))
                break;
        }

        id = Common::regexGroups1(link, item, 0);
        if (!id.empty() && !Common::strContains(id, "&"))
            break;
    }

    if (id.empty()) {
        if (Common::strContains(link, "photos")) {
            vector<string> temp = Common::regexGroups(link, "/(\\d+)/");
            if (!temp.empty())
                id = temp[temp.size() - 1];
        }
    }
    return id;
}

bool FacebookExt::IsBlock() const {
    return isBlock;
}

bool FacebookExt::IsStop() {
    return isStopped();
}

bool FacebookExt::IsLogined() const {
    return isLogined;
}

bool FacebookExt::IsLogout() const {
    return isLogout;
}

bool FacebookExt::IsRelogined() const {
    return isRelogined;
}

bool FacebookExt::IsSomethingWentWrong() const {
    return isSomethingWentWrong;
}

bool FacebookExt::IsCheckpoint() {
    return !typeCheckpoint.empty();
}

void FacebookExt::ResetVariable()
{
    isBlock = false;
    isLogined = false;
    isLogout = false;
    isRelogined = false;
    isSomethingWentWrong = false;
    typeCheckpoint = "";
}

void FacebookExt::deleteDataBackupFacebook() {

}

void FacebookExt::reconnectInternet() {

}

string FacebookExt::handleScreen(string &xml) {
    if (xml.empty()) {
        xml = dumpXml();
        if (xml.empty())
            return "";
    }

    string screenName = detectScreen(xml);

    if (screenName == "FB_FEED" || Common::strStartsWith(screenName, "FB_STORY") ||
        Common::strStartsWith(screenName, "FB_POST")) {
        isLogined = true;
        typeCheckpoint = "";
    } else if (screenName == "FB_LOGIN" || screenName == "FB_SESSION_EXPIRED") {
        deleteDataBackupFacebook();

        if (IsLogined()) {
            isLogined = false;
            isLogout = true;

            //relogin
            handleLogin();
            if (isLogined) {
                isLogout = false;
                isRelogined = true;
            } else
                return "FB_LOGOUT";
        }
    } else if (Common::strStartsWith(screenName, "FB_CHECKPOINT") && typeCheckpoint.empty()) {
        if (screenName == "FB_CHECKPOINT_DISABLE") {
            typeCheckpoint = "vhh";
        } else if (Common::strStartsWith(screenName, "FB_CHECKPOINT_282")) {
            typeCheckpoint = "282";
            if (screenName == "FB_CHECKPOINT_282_PHONE")
                typeCheckpoint = "282 (Phone)";
            else if (screenName == "FB_CHECKPOINT_282_IMAGE")
                typeCheckpoint = "282 (Image)";
        } else if (Common::strStartsWith(screenName, "FB_CHECKPOINT_956")) {
            typeCheckpoint = "956";
            if (screenName == "FB_CHECKPOINT_956_OTP_EMAIL")
                typeCheckpoint = "956 (Otp mail)";
        } else if (Common::strStartsWith(screenName, "FB_CHECKPOINT_LOGIN")) {
            if (screenName == "FB_CHECKPOINT_LOGIN_DEVICE") {
                typeCheckpoint = "Login (Thiết bị)";
            } else if (screenName == "FB_CHECKPOINT_LOGIN_OTP_EMAIL") {
                typeCheckpoint = "Login (Otp mail)";
            } else {
                typeCheckpoint = "";
                if (existNode(
                        "//*[@text='Approve the login from another device.' or @content-desc='Approve the login from another device.']",
                        xml))
                    typeCheckpoint += (typeCheckpoint.empty() ? "" : "-") + string("Thiết bị");
                if (existNode("//*[@text='Email' or @content-desc='Email']", xml))
                    typeCheckpoint += (typeCheckpoint.empty() ? "" : "-") + string("Otp mail");
                typeCheckpoint = typeCheckpoint.empty() ? "Login" : ("Login (" + typeCheckpoint +
                                                                     ")");
            }
        }
    } else if (screenName == "FB_SOMETHING_WENT_WRONG") {
        closeAppAction(social.package);
        openApp(social.package);

        screenName = "FB_POPUP_";
    } else if (screenName == "FB_SUSPECT_AUTOMATED") {
        if (!tap("//*[contains(@text,'Dismiss') and contains(@content-desc,'Dismiss')]", xml)) {
            Node node = getNode(
                    "//*[@text='We suspect automated behavior on your account' or @content-desc='We suspect automated behavior on your account']",
                    xml);
            tap(node.centerX, node.bottom / 6 * 5);
            delay(2);
        }

        screenName = "FB_POPUP_";
    } else if (screenName == "FB_WHAT_HAPPENED") {
        if (tap("//*[@content-desc='Close']", xml))
            delay(2);

        //if (tap("//*[@content-desc='See why' or @content-desc='See all']", xml))
        //{
        //    delay(2);
        //    if (xml != DumpXml())
        //        Back(1, 2);
        //}

        back(1, 2);
        screenName = "FB_POPUP_";
    } else if (screenName == "FB_YES_IT_ME") {
        swipe();
        tap("//*[@text='Yes, it’s me' or @content-desc='Yes, it’s me']");
        screenName = "FB_POPUP_";
    } else if (screenName == "FB_ACCOUNT_STATUS") {
        back(1);
        screenName = "FB_POPUP_";
    } else if (screenName == "FB_NO_INTERNET") {
        //connect internet
        reconnectInternet();
        tap("//*[@content-desc='Reload Menu']", xml);

        screenName = "FB_POPUP_";
    } else if (screenName == "FB_BLOCKED") {
        isBlock = true;
    }

    if (screenName.empty()) {
        if (currentActivity == "com.facebook.ixt.enrollmenttrigger.IXTEnrollmentActivity") {
            back(1, 2);
            //CloseApp(packageFacebook);
            //delay(2);
            //OpenApp(packageFacebook);
            gotoLink("fb://feed/scroll_to_top");
            screenName = "FB_POPUP_";
        }
    }
    return screenName;
}

bool FacebookExt::waitScreen(const string &screen, string xml, int timeout) {
    auto start = high_resolution_clock::now();
    while (!isStop) {
        if (Common::hasExceededTimeout(start, timeout))
            break;

        string currentScreen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            break;
        if (Common::strContains(currentScreen, "_POPUP_"))
            continue;
        if (currentScreen == "FB_SESSION_EXPIRED")
            break;

        if (currentScreen == screen)
            return true;

        delay(1);
        xml = dumpXml();

    }
    return false;
}

void FacebookExt::hideKeyboard(){
    handleKeyboard(Keyboard::HIDE);
    delay(1);
}