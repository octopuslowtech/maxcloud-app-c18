#include "InstaExt.h"

class InstaCore : public InstaExt {
public:
    void handleTest() {
        Common::NLog("Insta Test");

//        backupAccount();
        //restoreAccount();

       JSON configs = JSON();

        Common::NLog("Test Done");

    }

    ResultScreen actionTuongtacVideoRandom() {
        int cbbOptionsPost = action.getInt("cbbOptionsPost");
        int soLuong = 5;
        //Common::ranNumber(action.getInt("nudSoLuongFrom"),
        //                action.getInt("nudSoLuongTo"));

        int nudThoiGianFrom = action.getInt("nudThoiGianFrom");
        int nudThoiGianTo = action.getInt("nudThoiGianTo");

        int thoiGian = Common::ranNumber(nudThoiGianFrom, nudThoiGianTo);

        int cbbTuyChon = action.getInt("cbbTuyChon");

        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        bool ckbInteract = action.getBool("ckbInteract");

        int nudPercentInteract = action.getInt("nudPercentInteract");
        int nudDelayInteractFrom = action.getInt("nudDelayInteractFrom");
        int nudDelayInteractTo = action.getInt("nudDelayInteractTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int nudDelayCommentFrom = action.getInt("nudDelayCommentFrom");
        int nudDelayCommentTo = action.getInt("nudDelayCommentTo");
        vector<string> lstContent = action.getList("txtComment", action.getInt("typeNganCach"));
        vector<string> lstContent_Clone = {"hello"};

        bool ckbAddNote = action.getBool("ckbAddNote");
        int nudPercentAddNote = action.getInt("nudPercentAddNote");
        int nudDelayAddNoteFrom = action.getInt("nudDelayAddNoteFrom");
        int nudDelayAddNoteTo = action.getInt("nudDelayAddNoteTo");
        vector<string> lstNotes = action.getList("txtComment", action.getInt("typeNganCach"));
        vector<string> lstNotes_Clone(lstNotes);
        string xml;
        string screen;
        string status;
        if (lstContent.empty())
            ckbComment = false;
        int countVideo = 0;
        auto start = high_resolution_clock::now();
        for (int i = 0; i < 100; ++i) {
            updateLine("Video Post: (" + to_string(countVideo + 1) + ")");
            delay(1);
            xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "ERROR")
                return ERROR;

            if (screen != "IG_HOME") {
                gotoHome();
                continue;
            }

            if (!existNode("//*[@content-desc='Like' and @selected ='false']", xml, 0)) {
                swipe();
                continue;
            }
            bool isInteractSuccess = false;
            //if (ckbInteract && Common::ranBool(nudPercentInteract)) {
            isInteractSuccess = likeVideo(xml);
            //}

            bool isCommentSuccess = false;
            if (ckbComment && Common::ranBool(nudPercentComment)) {
            string commentContent = Common::ranItem(lstContent_Clone);
            isCommentSuccess = commentVideo(commentContent, xml);
            Common::removeItem(lstContent_Clone, commentContent);
            }
            bool isAddNoteSuccess = false;
            //if (ckbAddNote && Common::ranBool(nudPercentAddNote)) {
                string noteContent = Common::ranItem(lstNotes_Clone);
                isAddNoteSuccess = addNote(xml, noteContent);
            //    Common::removeItem(lstNotes_Clone, noteContent);
            //}

            if (isInteractSuccess || isCommentSuccess || isAddNoteSuccess) {
                countVideo++;
                if (countVideo >= soLuong)
                    return NOMAP;
                swipe(1,2);
                continue;
            }
        }
        return NOMAP;
    }

    ResultScreen actionTuongTacVideoChiDinh() {
        //input
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                action.getInt("nudSoLuongTo"));
        vector<string> lstLink = {"https://www.instagram.com/p/DD9LzcYTVrl/"};
                //action.getList("txtLink");

        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");
        int nudDelayInteractFrom = action.getInt("nudDelayInteractFrom");
        int nudDelayInteractTo = action.getInt("nudDelayInteractTo");

        bool ckbRepost = action.getBool("ckbRepost");
        int nudPercentRepost = action.getInt("nudPercentRepost");
        int nudDelayRepostFrom = action.getInt("nudDelayRepostFrom");
        int nudDelayRepostTo = action.getInt("nudDelayRepostTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int nudDelayCommentFrom = action.getInt("nudDelayCommentFrom");
        int nudDelayCommentTo = action.getInt("nudDelayCommentTo");
        vector<string> lstContent = action.getList("txtComment", action.getInt("typeNganCach"));
        vector<string> lstContent_Clone(lstContent);


        //code here
        if (lstContent.empty())
            ckbComment = false;
        string xml;
        string screen;
        string status;
        string link;
        int countVideo = 0;

        if (lstLink.empty())
            return NOMAP;
        link = Common::ranItem(lstLink);

        while (!isStopped()) {
            status = "Video (" + to_string(countVideo + 1) + ") ";
            updateLine(status + "Open...");

            gotoLink(link);
            delay(2);

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            Common::removeItem(lstLink, link);
            delay(nudTimeFrom, nudTimeTo, status + "Xem video");

            //if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(nudDelayInteractFrom, nudDelayInteractTo,
                          status + "Like done, delay");
            //}

            //if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");
                if (lstContent_Clone.empty())
                    lstContent_Clone = vector<string>(lstContent);
                string content = Common::ranItem(lstContent_Clone);

                if (commentVideo(content, xml)) {
                    Common::removeItem(lstContent_Clone, content);
                    delay(nudDelayCommentFrom, nudDelayCommentTo,
                          status + "Comment done, delay");
                    xml = "";
                }
            //}


            //if (ckbRepost && Common::ranBool(nudPercentRepost)) {
                updateLine(status + "Repost...");
                if (addNote(xml,"Repost")) {
                    delay(nudDelayRepostFrom, nudDelayRepostTo,
                          status + "Repost done, delay");
                    xml = "";
                    if (countVideo == 0)
                        handleScreen(xml);
                }
            //}

            countVideo++;
            if (countVideo >= soLuong)
                return DONE;

            if (lstLink.empty())
                break;
            link = Common::ranItem(lstLink);
        }

        return NOMAP;
    }

    string getCurrentActivity() {
        return currentActivity;
    }

    bool addNote(string xml, string content) {
        string xpath = existNodes(0, xml, {
                "//*[@content-desc='Send post']"
        });
        if (xpath.empty()) {
            return false;
        }
        if (!tap(xpath, xml)) {
            return false;
        }
        delay(2);
        xml = dumpXml();

        if (existNode("//*[@content-desc='Add note']", xml, 0))
            tap("//*[@content-desc='Add note']", xml);
        delay(2);
        xml = dumpXml();
        if (existNode("//android.widget.EditText[@text='Add note…']", xml, 0)) {
            inputText("//android.widget.EditText[@text='Add note…']", content);
            delay(2);
            if (tap("//*[@content-desc='Share']")) {
                delay(2);
                return true;
            }
        }
        return false;
    }

    bool savePost(string xml) {
        string xpath = existNodes(0, xml, {
                "//*[@content-desc='Add to Saved']"
        });
        if (xpath.empty()) {
            return false;
        }
        if (!tap(xpath, xml)) {
            return false;
        }
        delay(2);
        xml = dumpXml();
        if (existNode("//*[@content-desc='Remove from saved']", xml, 0))
            return true;
        return false;
    }

    bool likeVideo(string xml) {
        string xpath = existNodes(0, xml, {
                "//*[@content-desc='Like']"
        });
        if (xpath.empty()) {
            return false;
        }
        if (!tap(xpath, xml)) {
            return false;
        }
        delay(2);
        xml = dumpXml();
        if (existNode("//*[@content-desc='Liked']", xml, 0))
            return true;
        return false;
    }

    bool commentVideo(string content, string xml = "") {
        if (xml == "")
            xml = dumpXml();

        string xpath = existNodes(0, xml, {
                "//*[@content-desc='Comment']"
        });

        if (xpath.empty()) {
            return false;
        }
        if (!tap(xpath, xml)) {
            return false;
        }
        delay(2);
        xml = dumpXml();
        if (existNode("//android.widget.EditText[contains(@text,'Add a comment ')]", xml, 0)) {
            inputText("//android.widget.EditText[contains(@text,'Add a comment ')]", content);

            delay(2);
            xml = dumpXml();
            if (existNode("//*[@content-desc='Post']", xml, 0)) {
                tap("//*[@content-desc='Post']");
                delay(2);
                back(2, 1);
                return true;
            }
        }
        return false;
    }

    Task followUser(string user, string status) {
        user = Common::strToLower(user);
        string xml;
//        if (gotoProfile(user, status + " - Go to Profile") != DONE) {
//            return Task::FAIL;
//        }

        vector<string> lstXpathFollow = {
                "//*[@text='Follow' or contains(@content-desc,'Follow ')]"
        };
        string xpathFollow = existNodes(0, xml, lstXpathFollow);
        xml = dumpXml();
        if (xpathFollow.empty()) {
            return Task::DONE;
        }
        if (!tap(xpathFollow, xml))
            return Task::FAIL;

        delay(5, status + " - Follow");
        return Task::DONE;
    }

    void handleRegisterAccount() {

        return;
    }


    ResultScreen handleAction() {
        if (action.scriptType == "READ_NOTIFICATIONS")
            return actionDocThongBao();
        if (action.scriptType == "REACT_NEWFEED")
            return actionReactNewFeed();
        if (action.scriptType == "REACT_FOLLOWER")
            return actionReactFollower();
        if (action.scriptType == "REACT_FOLLOWING")
            return actionReactFollowing();
        if (action.scriptType == "REACT_PROFILE")
            return actionReactProfile();
        if (action.scriptType == "REACT_HASHTAG")
            return actionReactHashTag();
        if (action.scriptType == "SHARE_POST")
            return actionSharePost();
        if (action.scriptType == "VIEW_DISCOVER")
            return actionViewDiscover();
        if (action.scriptType == "MESSAGE_UID")
            return actionMessageUid();
        if (action.scriptType == "REACT_POST_UID")
            return actionReactPostUid();
        if (action.scriptType == "FOLLOW_UID")
            return actionFollowUid();
        if (action.scriptType == "FOLLOW_BACK")
            return actionFollowBack();
        if (action.scriptType == "FOLLOW_KEYWORD")
            return actionFollowKeyword();
        if (action.scriptType == "FOLLOW_LIKED_POST")
            return actionFollowLikedPost();
        if (action.scriptType == "FOLLOW_FOLLOWER_USER")
            return actionFollowFollowerUser();
        if (action.scriptType == "FOLLOW_FOLLOWING_USER")
            return actionFollowFollowingUser();
        if (action.scriptType == "UNFOLLOW_USER")
            return actionUnfollowUser();
        if (action.scriptType == "REACT_REELS")
            return actionReactReel();
        if (action.scriptType == "REACT_STORY")
            return actionReactStory();
        if (action.scriptType == "UP_REEL")
            return actionUpReel();
        if (action.scriptType == "DELETE_REEL")
            return actionDeleteReel();
        if (action.scriptType == "UP_STORY")
            return actionUpStory();
        if (action.scriptType == "UP_IMAGE")
            return actionUpImage();
        if (action.scriptType == "EARN_MONEY")
            return actionEarnMoney();
        if (action.scriptType == "DELETE_POST_WALL")
            return actionDeletePostWall();
        if (action.scriptType == "SLEEP")
            return actionSleep();
        if (action.scriptType == "CHANGE_PASSWORD")
            return actionChangePassword();
        if (action.scriptType == "UP_AVATAR")
            return actionUpAvatar();
        if (action.scriptType == "ADD_MAIL")
            return actionAddMail();
        if (action.scriptType == "TURN_ON_OF_2FA")
            return actionTurnOnOff2fa();
        if (action.scriptType == "CHANGE_NAME")
            return actionChangeName();
        if (action.scriptType == "UPDATE_PROFILE")
            return actionUpdateProfile();
        if (action.scriptType == "ACCOUNT_PRIVATE")
            return actionAccountPrivate();
        if (action.scriptType == "ACCOUNT_PROFESSIONAL")
            return actionAccountProfessional();
        if (action.scriptType == "DELETE_PHONE_NUMBER")
            return actionDeletePhoneNumber();
        return NOMAP;
    }
    //Hàm vào thông báo
    bool gotoNotify() {
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

            string element = existNodes(0, xml, {
                    "//android.widget.Button[@content-desc='Activity']",
                    "//android.widget.TextView[@text='Notifications' or @content-desc='Notifications']",
            });

            if (element == "//android.widget.Button[@content-desc='Activity']") {
                tapCenter(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//android.widget.TextView[@text='Notifications' or @content-desc='Notifications']")
                return true;
        }
        return false;
    }

    //Action Đọc thông báo
    ResultScreen actionDocThongBao() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);

        int countSuccess = 0;
        int currentIndex = 0;

        //Go to notify
        if (!gotoNotify())
            return NOMAP;
        delay(2);

        updateLine("Read notify (" + to_string(countSuccess + 1) + "/" + to_string(soLuong)  + ")");

        //time start - time out
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

            vector<Node> lstElement = getNodes("//*[@resource-id='activity_feed_newsfeed_story_row']",xml);

            if (lstElement.empty())
                break;

            //find element
            if (lstElement.size() <= currentIndex) {
                if (swipeAndScreenNotChange())
                    break;
                xml = "";
                lstElement = getNodes("//*[@resource-id='activity_feed_newsfeed_story_row']",xml);
                currentIndex = 2;
                if (lstElement.size() <= currentIndex)
                    break;
            }
            tap(lstElement[currentIndex]);
            delay(2);
            countSuccess++;
            currentIndex++;

            //success -> delay
            delay(nudDelayFrom, nudDelayTo,
                  "Read notify (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                  ") success, delay");
            if (countSuccess >= soLuong)
                return DONE;

            updateLine("Read notify (" + to_string(countSuccess + 1) + "/" + to_string(soLuong)  + ")");

            //back
            for (int i = 0; i < 3; ++i) {
                xml = dumpXml();
                if (existNode("//android.widget.TextView[@text='Notifications' or @content-desc='Notifications']",xml))
                    break;
                back();
                delay(2);
            }
            timeStart = high_resolution_clock::now();
        }
        return NOMAP;
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

    //Action tương tác newfeeds
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

        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        bool ckbInteract = action.getBool("ckbInteract");//thả cảm xúc?
        int nudPercentLike = action.getInt("nudPercentLike");//xác suất thả cảm xúc

        bool ckbComment = action.getBool("ckbComment");//comment text?
        int nudPercentComment = action.getInt("nudPercentComment");//xác suất comment text
        string commentContentId = action.getString("commentContentId");//danh sách nội dung comment

        int cbbComment = action.getInt("cbbComment"); // 0 - comment text, 1 - comment gif
        bool ckbTuDongXoaNoiDung = action.getBool("ckbTuDongXoaNoiDung");

        bool isReelByKeywords = action.getBool("isReelByKeywords");

        //xử lý logic
        if (ckbInteract && nudPercentLike == 0)
            ckbInteract = false;
//        if (ckbComment && (txtComment.empty() || nudPercentCommentText == 0))
//            ckbComment = false;
//        if (ckbCommentGif && nudPercentCommentText == 0)
//            ckbCommentGif = false;
        bool allowCommentText = (cbbComment == 0) && ckbComment && !commentContentId.empty() && nudPercentComment > 0;
        bool allowCommentGif = (cbbComment == 1) && ckbComment && (nudPercentComment > 0);

        if (!allowCommentText && !allowCommentGif)
            ckbComment = false;

        int countSuccess = 0;
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            if (cbbOptionsPost == 0)
                statusAction = status + " (" + to_string(countPostInteracted + 1) + "/" + to_string(soLuongBai) + "): ";

            //view post
            delay(nudTimeFrom, nudTimeTo, statusAction + "View post");

            //Thả like nếu được bật
            bool isReactSuccess = false;
            if (ckbInteract && Common::ranBool(nudPercentLike)) {
                updateLine(statusAction + "Like...");
                JSON configs = JSON();
                JSON response = likePost(configs);

                if (response.getBool("success"))
                    isReactSuccess = true;
                if (!isReactSuccess && IsBlock())
                    break;
            }
            bool isCommentSuccess = false;
            if (ckbComment && nudPercentComment > 0 && Common::ranBool(nudPercentComment)) {
                updateLine(statusAction + "Comment...");
                JSON configs;
                if (allowCommentText){
                    configs.set("commentContent",commentContentId);
                    configs.set("cbbComment", 0);
                } else if (allowCommentGif) {
                    configs.set("cbbComment", 1);
                }
                JSON response = commentPost(configs);
                if (response.getBool("success"))
                    isCommentSuccess = true;
                if (!isCommentSuccess && IsBlock())
                    break;
            }
            if (isReactSuccess || isCommentSuccess)
                countPostInteracted++;

            bool isDone = cbbOptionsPost == 0 ? countPostInteracted >= soLuongBai
                                              : Common::hasExceededTimeout(timeStart,thoiGianTuongTac);
            if (isDone)
                break;

            if (cbbOptionsPost == 0 && countPostInteracted == 0 && !isReactSuccess && !isCommentSuccess) {
                maxSwipe--;
                if (maxSwipe < 0)
                    break;
            }
            Base::updateLine(statusAction + "swipe...");
//
//            if (isReelByKeywords){
//                back(1,2);
//                swipe();
//
//                if (!swipeAndTap("(//*[contains(@content-desc,'Reel by')])[1]","",5))
//                    break;
//                delay(1);
//            } else {
                bool isSwipeSuccess = false;
                if (screen == "IG_STORY") {//Story
                    isSwipeSuccess = !swipeHAndScreenNotChange();
                    if (isSwipeSuccess && handleScreen(xml) != "IG_STORY")
                        break;
                } else if (screen == "IG_REEL") { // reel
                    isSwipeSuccess = !swipeAndScreenNotChange(3);
                } else {
                    if (cbbOptionsPost == 0 && soLuongBai == 1)
                        isSwipeSuccess = !swipeAndScreenNotChange() || !swipeAndScreenNotChange() ||
                                         !swipeAndScreenNotChange() || !swipeAndScreenNotChange() ||
                                         !swipeAndScreenNotChange() || !swipeAndScreenNotChange();
                    else
                        isSwipeSuccess = !swipeAndScreenNotChange(1,2) || swipeAndScreenNotChange(1,2);
                }
                if (!isSwipeSuccess)
                    break;
            }
//
//            if (swipeAndScreenNotChange(1, 2))
//                break;
        }

    //Hàm Like
    JSON likePost(JSON configs) {
        // output
        JSON output = JSON();
        output.set("success", false);
        output.set("error", "");

        // Lấy XML màn hình hiện tại
        string xml = dumpXml();
        string screen = handleScreen(xml);

        // Kiểm tra trạng thái tài khoản
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            return output;
        if (Common::strContains(screen, "_POPUP_"))
            return output;

        // Kiểm tra nếu bài viết đã được like
        string likedXPath = "//*[@content-desc='Liked']";
        if (existNode(likedXPath, xml)) {
            output.set("error", "already_liked");
            return output;
        }

        // Tìm nút Like
        string likeXPath = "//*[@content-desc='Like']";
        if (!existNode(likeXPath, xml)) {
            output.set("error", "not_exist_xpath");
            return output;
        }
        // Nhấn vào nút Like
        if (tapCenter(likeXPath, xml)) {
            output.set("success", true);
            delay(2);
        }
        return output;
    }

    //Hàm comment
    JSON commentPost(JSON configs) {
        // input
        string commentContent = configs.getString("commentContent");
        int cbbComment = configs.getInt("cbbComment"); // 0 - comment text, 1 - comment GIF
        bool ckbTag = configs.getBool("ckbTag");
        vector<string> txtUidTag = configs.getList("txtUidTag");
        int typeTag = configs.getInt("typeTag");
        int nudSoLuongTagFrom = configs.getInt("nudSoLuongTagFrom");
        int nudSoLuongTagTo = configs.getInt("nudSoLuongTagTo");

        // output
        JSON output = JSON();
        output.set("success", false);
        output.set("error", "");

        // Lấy XML màn hình hiện tại
        string xml = dumpXml();
        string screen = handleScreen(xml);

        // Kiểm tra trạng thái tài khoản
        if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
            return output;
        if (Common::strContains(screen, "_POPUP_"))
            return output;
        if (screen == "IG_SUSPENDED_ACCOUNT") {
            output.set("error", "logout");
            return output;
        }
        bool isStory = (screen == "IG_STORY");
        // Tìm nút Comment

        string commentXPath;
        if (isStory) {
            commentXPath = "//*[@text='Send message']"; // XPath cho Story
        } else {
            commentXPath = "//*[@content-desc='Comment']"; // XPath cho Post
        }

        if (!existNode(commentXPath, xml)) {
            output.set("error", "not_exist_xpath");
            return output;
        }

        // Nhấn vào nút Comment hoặc send message
        tapCenter(commentXPath, xml);
        delay(2);

        // Tìm ô nhập bình luận
        string xInput;
        if (isStory) {
            xInput = existNodes(1, xml, {
                    "//android.widget.EditText[contains(@text,'Send a message')]"
            });
        } else {
            xInput = existNodes(1, xml, {
                    "//android.widget.EditText[contains(@text,'Add a comment')]"
            });
        }
        if (!existNode(xInput, xml)) {
            output.set("error", "input_box_not_found");
            if (!isStory) back(); // Chỉ back nếu là Post
            return output;
        }

        // Nhập nội dung bình luận
        if (cbbComment == 0 && !commentContent.empty()) {
            auto [success, content, itemId] = getScriptContent(commentContent);

            if(!success) {
                saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                return output; // chay tiep hoac dung lai
            }

            // comment, dang bai, .....
            // Common::NLog("Content: " + content, "handleTest");

            inputText("//android.widget.EditText", content);
            // xoa content sau khi su dung

            if(!itemId.empty()) {
                deleteScriptContent(itemId);
            }
            delay(1);
        } else if (cbbComment == 1) {
            string stickerButtonXpath = "//*[@content-desc='Sticker']";
            if (!tap(stickerButtonXpath,xml)) {
                output.set("error","cannot_open_sticker");
                if(!isStory) back(2);
                return output;
            }
            delay(2);

            xml = dumpXml();
            if (existNode("//*[@content-desc='Create avatar' or @text='Create your avatar']",xml)){
                string gifButtonXpath = "//*[@content-desc='GIFs']";
                tap(gifButtonXpath,xml);

            }
            delay(3);

            //Lấy list danh sách GIF
            xml = dumpXml();
            vector<Node> lstGif = getNodes(vector<string>{
                "//*[@resource-id='com.instagram.android:id/gif_image']",
            },xml);
            if (lstGif.empty()) {
                output.set("error","no_gif_found");
                back();
                return output;
            }
            //Chọn gif ngẫu nhiên
            int randomIndex = Common::ranNumber(0, lstGif.size() - 1);
            Node selectGif = lstGif[randomIndex];
            if (!selectGif.xpath.empty()) {
                tap(selectGif);
                delay(2);
                back();

                //Kiểm tra xem gửi GIF có lỗi không
                xml = dumpXml();
                if (existNode("//*[@text='Try Again Later']",xml)) {
                    output.set("error", "cannot_send_gif");
                    back(2);
                    return output;
                }
            }
        } else {
            output.set("error","no_comment_content");
            return output;
        }

        string sendButtonXPath;
        if (isStory) {
            sendButtonXPath = "//*[@content-desc='Send story']"; // XPath cho Story
        } else {
            sendButtonXPath = existNodes(1, xml, {
                    "//*[@content-desc='Post']"
            });
        }
        if (!tap(sendButtonXPath, xml)) {
            output.set("error", "cannot_post_comment");
            return output;
        }
        delay(2);
        if (!isStory) back(2,2);
        output.set("success", true);
        return output;
    }

    //Hàm comment tag tên
    bool commentTagUser(string commentContent, string xml = "", bool ckbTagUser = false, vector<string> txtUser = {}, int countTag = 0) {
        string xAddComment = "//*[@content-desc='Comment']";
        string xInput = "(//android.widget.EditText)";
        string xpath = existNodes(0, xml, {xAddComment});
        if (xpath.empty())
            return false;

        tap(xpath, xml);
        delay(2);

        xml = "";
        if (!existNode(xInput, xml)) {
            xInput = "//android.widget.EditText";
            if (!existNode(xInput, xml)) {
                performAction(Perform::BACK, 1, 1);
                return false;
            }
        }

        string oldText = getNode(xInput, xml).text;
        tap(xInput, xml);
        delay(1);
        xInput = "//android.widget.EditText";

        //tag tên
        string finalContent = "";
        int countSuccess = 0;
        if (ckbTagUser && !txtUser.empty()) {
            for (const string& user : txtUser) {
                if (countSuccess >= countTag)
                    break; // Dừng khi đã tag đủ số lượng

                //Nhập username
                string tag = "@" + user;
                if (countSuccess > 0) {
                    tag = " " + tag;
                }
                inputTextOnLast(xInput, tag);
                delay(1);

                // Kiểm tra xem tag được đã nhập thành công hay chưa ?
                xml = dumpXml();
                string currentText = getNode(xInput, xml, 3).text;
                if (currentText.find(tag) == string::npos)
                    continue;
                countSuccess++;
            }
        }
        // Lấy nội dung comment từ script
        auto [success, content, itemId] = getScriptContent(commentContent);
        if (!success) {
            saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
            return NOMAP;
        }

        //Lấy nội dung hiện tại của ô nhập sau khi tag

        // Gộp nội dung tag và comment
        if (ckbTagUser && countSuccess > 0) {
            finalContent += " " + content;
        } else {
            finalContent = content;
        }
        // Nhập nội dung comment
        if (countSuccess > 0) {
            inputTextOnLast(xInput, finalContent); // Thêm khoảng trắng trước content
        } else {
            inputText(xInput, finalContent);  // Nếu không có tag, nhập content trực tiếp
        }
        delay(1);
        hideKeyboard();

        if (!itemId.empty()) {
            deleteScriptContent(itemId);
        }
        delay(1);

        //Gửi comment
        xml = dumpXml();
        string xPost = existNodes(0, xml, {
                "//*[@content-desc='Post']",
                "//*[@text='Post']"
        });
        if (xPost.empty()) {
            performAction(Perform::BACK, 2, 1);
            return false;
        }
        tap(xPost, xml);
        delay(2);
        xml = dumpXml();
        if (existNode(xPost,xml,3))
            tap(xPost, xml);
        delay(2);

        xml = "";
        bool isSuccess = existNode(xInput + "[@text='" + oldText + "']", xml, 30); // Kiểm tra thành công
        delay(1);

        performAction(Perform::BACK, 1, 1);
        return isSuccess;
    }


    //Hàm trả lời comment
    bool ReplyComment(string xml, string textCmt) {
        if (!existNode("//android.widget.Button[@text='Reply' or @content-desc='Reply']", xml))
            return false;
        int typeReply = action.getInt("typeReply");
        bool ckbTuDongXoaNoiDung = action.getBool("ckbTuDongXoaNoiDung");
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;
        vector<Node> replyComment = getNodes(
                "//android.widget.Button[@text='Reply' or @content-desc='Reply']", xml);
        int countFirst = 1;
        int countMax = replyComment.size();
        switch (typeReply) {
            case 0:
                countMax = 1;
                break;
        }
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;

            xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;
            string element = existNodes(0, xml, {
                    "(//android.widget.Button[@text='Reply' or @content-desc='Reply'])[" +
                    to_string(Common::ranNumber(countFirst, countMax)) + "]",
                    "//android.widget.Button[@text='Reply to any comment with a reel']",
                    "//android.widget.EditText[@resource-id='com.instagram.android:id/layout_comment_thread_edittext']",

            });

            if (element == "(//android.widget.Button[@text='Reply' or @content-desc='Reply'])[" +
                           to_string(Common::ranNumber(countFirst, countMax)) + "]") {
                tapCenter(element, xml);
                countFirst = 100;
                countMax = 100;//reply rồi thì sẽ không reply nữa
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='Reply to any comment with a reel']") {
                tapCenter("//android.widget.Button[@text='Not now']", xml);
                delay(2);
                continue;
            }
            if (element ==
                "//android.widget.EditText[@resource-id='com.instagram.android:id/layout_comment_thread_edittext']") {
                inputText(element, textCmt);
                delay(1);
                tapCenter("//android.widget.ImageView[@content-desc='Post']", xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    // Tương tác new feed
    ResultScreen actionReactNewFeed() {
        if (!gotoHome())
            return NOMAP;
        delay(2);

        TuongTacNhieuBaiViet("React newfeed");
        return DONE;
    }


    //Handle go to follower
    bool handleGotoFollower() {
        if (!gotoProfile())
            return false;
        delay(2);
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (screen == "IG_PROFILE") {
            tap("//node[contains(@content-desc,'followers') and //android.widget.Button[@content-desc='Options']]",xml);
            delay(2);
            return true;
        }
        return false;
    }

    //Handle go to following
    bool handleGotoFollowing() {
        if (!gotoProfile())
            return false;
        delay(2);
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (screen == "IG_PROFILE") {
            tap("//node[contains(@content-desc,'following') and //android.widget.Button[@content-desc='Options']]",xml);
            delay(2);
            return true;
        }
        return false;
    }

    //Tương tác Follower
    ResultScreen actionReactFollower() {
        //input người dùng nhập vào
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongBanFrom"),
                                        action.getInt("nudSoLuongBanTo"));

        //input thêm biến xử lý
        int countSuccess = 0;
        int countFollower = 1;

        //goto
        updateLine("Go to list follower...");
        if (!handleGotoFollower())
            return NOMAP;
        delay(2);
        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;
            string element = existNodes(0, xml, {
                "(//android.widget.LinearLayout[@resource-id='com.instagram.android:id/follow_list_container'])[" + to_string(countFollower) + "]", //nhấn vào từng follower
                "//android.widget.TextView[@text='This Account is Private']", //tài khoản riêng tư
                "//android.widget.TextView[@text='No Posts Yet']",  //tài khoản chưa có bài đăng
                "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1][not(//android.widget.Button[@content-desc='Options'])]",
                "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']",

            });
            waitNodeHide("//*[not(//android.widget.FrameLayout[@resource-id='com.instagram.android:id/modal_container'])]",30);
            if (element == "(//android.widget.LinearLayout[@resource-id='com.instagram.android:id/follow_list_container'])[" + to_string(countFollower) + "]") {
                tapCenter(element,xml);
                delay(2);
                timeStart = high_resolution_clock::now();
                continue;
            }


            if (element == "//android.widget.TextView[@text='This Account is Private']"
            || element == "//android.widget.TextView[@text='No Posts Yet']") {
                back();
                countFollower++;
                delay(2);
                continue;
            }
            if (element == "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1][not(//android.widget.Button[@content-desc='Options'])]") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']"){
                TuongTacNhieuBaiViet("Interact follower(" + to_string(countSuccess + 1) + "/" +
                                     to_string(soLuong) + ")");
                countSuccess++;
                countFollower++;
                if (countSuccess >= soLuong)
                    return DONE;
                back(2,1);
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (swipeAndScreenNotChange())
                return NOMAP;
        }
        return NOMAP;
    }

    // Tương tác following
    ResultScreen actionReactFollowing() {
        //Input người dùng nhập vào
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongBanFrom"),
                                        action.getInt("nudSoLuongBanTo"));
        //Input thêm biến xử lý cần thiết
        int countSuccess = 0;
        int countFollowing = 1;
        //goto
        updateLine("Go to list following...");
        if (!handleGotoFollowing())
            return NOMAP;
        delay(2);

        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;
            string element = existNodes(0, xml, {
                "(//android.widget.LinearLayout[@resource-id='com.instagram.android:id/follow_list_container'])[" + to_string(countFollowing) + "]", //nhấn vào từng following
                "//android.widget.TextView[@text='This Account is Private']", //tài khoản riêng tư
                "//android.widget.TextView[@text='No Posts Yet']",  //tài khoản chưa có bài đăng
                "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1][not(//android.widget.Button[@content-desc='Options'])]",
                "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']",
            });
            if (element == "(//android.widget.LinearLayout[@resource-id='com.instagram.android:id/follow_list_container'])[" + to_string(countFollowing) + "]") {
                tapCenter(element,xml);
                delay(2);
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//android.widget.TextView[@text='This Account is Private']"
                || element == "//android.widget.TextView[@text='No Posts Yet']") {
                back();
                countFollowing++;
                delay(2);
                continue;
            }
            if (element == "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1][not(//android.widget.Button[@content-desc='Options'])]") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']") {
                TuongTacNhieuBaiViet("Interact following(" + to_string(countSuccess + 1) + "/" +
                                             to_string(soLuong) + ")");
                countSuccess++;
                countFollowing++;
                if (countSuccess >= soLuong)
                    return DONE;
                back(2,1);
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (swipeAndScreenNotChange())
                return NOMAP;
        }
        return NOMAP;
    }

    // Share bài viết
    ResultScreen actionSharePost() {
        openApp("com.instagram.android");
        delay(2);
        //input người dùng nhập vào
        string txtLink = action.getString("txtLink");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //input thêm biến xử lý nếu cần thiết
        int countIndex = 1;
        int countSuccess = 0;
        int soLuong = Common::ranNumber(nudSoLuongFrom,nudSoLuongTo);

        //goto
        gotoPost(txtLink);
        delay(2);

        updateLine("Share post (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

        //timeStart
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

            Node nodeSend = getNode("(//*[@text='Send'])[" + to_string(countIndex) + "]",xml,2);
            Node nodeDone = getNode("//*[@content-desc='Done']",xml,2);

            if (nodeSend.top <= nodeDone.bottom && nodeSend.bottom >= nodeDone.top) {
                updateLine("Share post (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                           "), swipe");
                if (swipeAndScreenNotChange())
                    return NOMAP;
                countIndex = 4;
                continue;
            }
            string element = existNodes(0, xml, {
                "//*[@content-desc='Send post' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_drag_handle'])]",
                "//*[@content-desc='Share']",
                "//*[@text='Chats']",
                "//*[@text='View more…']",
                "//*[@text=\"This message can't be shared\"]",
                "(//*[@text='Send'])[" + to_string(countIndex) + "]",

            });
            if (element == "//*[@content-desc='Send post' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_drag_handle'])]"
             || element == "//*[@content-desc='Share']"
             || element == "//*[@text='Chats']"
             || element == "//*[@text='View more…']"){
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text=\"This message can't be shared\"]") {
                tap("//*[@text='OK']");
                countIndex++;
                countSuccess--;
                delay(2);
                continue;
            }
            if (element == "(//*[@text='Send'])[" + to_string(countIndex) + "]") {
                tap(element,xml);
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Share post (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                      ") success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Share post (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                timeStart = high_resolution_clock::now();
                continue;
            }
        }
        return NOMAP;
    }

    //Follow back
    ResultScreen actionFollowBack() {
        //Input người dùng nhập vào
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //Input thêm biến xử lý
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        //Time start
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //Goto
        if (!gotoProfile())
            return NOMAP;
        delay(2);
        updateLine("Follow Back (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

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
                    "//node[contains(@content-desc,'followers')]",
                    "//android.widget.Button[@text='Follow back' or @content-desc='Follow back']",
            });

            if (element == "//node[contains(@content-desc,'followers')]") {
                tapCenter(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//android.widget.Button[@text='Follow back' or @content-desc='Follow back']") {
                tapCenter(element, xml);
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo, "Follow Back (" + to_string(countSuccess) + "/" + to_string(soLuong) + ") success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Follow Back (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                continue;
            }
            updateLine("Follow Back (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       "), swipe");
            if (swipeAndScreenNotChange())
                break;
        }
        return NOMAP;
    }

    //Handle goto Search by keyword
    bool handleGotoSearchKeyWord(string tuKhoa, string typeSearch){
        updateLine("Search: " + tuKhoa + "...");
        Start:
        gotoSearch();
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
                goto Start;

            string element = existNodes(0, xml, {
                    "//android.widget.TextView[@text='Search']",
                    "//android.widget.TabWidget[@text='For you']",
            });

            if (element == "//android.widget.TextView[@text='Search']"){
                inputText(element,tuKhoa);
                delay(1);
                inputEnter();
                delay(2);
                continue;
            }
            if (element == "//android.widget.TabWidget[@text='For you']"){
                element = "//android.widget.TabWidget[@text='" + typeSearch + "' and @selected='false']";
                if (!existNode(element,xml)){
                    vector<Node> lstNodes = getNodes("//android.widget.TabWidget[contains(@text,'')]",xml);
                    swipe(lstNodes.back(), lstNodes.front());
                    delay(2);
                    xml = dumpXml();
                    if (!existNode(element,xml,3))
                        return false;
                }
                tap(element,xml);
                delay(2);

                if (typeSearch == "Reels") {
                    return tap("(//*[contains(@content-desc,'Reel by')])[1]","",10);
                }
                else if (typeSearch == "Tags") {
                    if (existNode("//*[@content-desc='Get resources']",xml)){
                        updateLine("Hashtag is invalid");
                        return false;
                    }
                      bool firstTap = tap("(//androidx.recyclerview.widget.RecyclerView/android.widget.FrameLayout[@resource-id='com.instagram.android:id/row_hashtag_container']/android.widget.LinearLayout)[1]", "", 10);
                      delay(2);
                      bool secondTap = tap("//androidx.recyclerview.widget.RecyclerView/android.widget.Button[@resource-id='com.instagram.android:id/image_button'][1]", "", 10);
                      delay(2);
                      return firstTap && secondTap;
                }
                return true;
            }
        }
        return false;
    }

    //Handle Follow theo từ khóa
    ResultScreen actionFollowKeyword() {
        //Input người dùng nhập
        vector<string> txtTuKhoa = action.getList("txtTuKhoa");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //Input thêm biến xử lý
        string tuKhoa = Common::ranItem(txtTuKhoa);
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        int countIndex = 1;
        int itemPerScreen = 8; // có 8 mục profile trên màn hình

        //goto
        if (!handleGotoSearchKeyWord(tuKhoa,"Accounts"))
            return NOMAP;
        delay(2);
        updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

        //Timestart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(0, xml, {
                    "//androidx.recyclerview.widget.RecyclerView[@resource-id='com.instagram.android:id/recycler_view']/android.widget.FrameLayout[" + to_string(countIndex) + "]",
                    "//android.widget.Button[@text='Follow' and not(ancestor::androidx.recyclerview.widget.RecyclerView)]",
                    "//android.widget.LinearLayout[@resource-id='com.instagram.android:id/profile_header_actions_top_row' and not(.//android.widget.Button[@Text='Follow'])]",
            });

            if (element ==
                "//androidx.recyclerview.widget.RecyclerView[@resource-id='com.instagram.android:id/recycler_view']/android.widget.FrameLayout[" + to_string(countIndex) + "]") {
                tapCenter(element, xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='Follow' and not(ancestor::androidx.recyclerview.widget.RecyclerView)]") {
                tapCenter(element, xml);
                delay(2);
                xml = dumpXml();
                screen = handleScreen(xml);
                if (screen == "IG_POPUP_OK") {
                    updateLine("Your request is pending");
                    back(1, 2);
                    countIndex++;
                    updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                               ")");
                    continue;
                } else if (!existNode(
                        "//android.widget.Button[@text='Follow' and not(ancestor::androidx.recyclerview.widget.RecyclerView)]",
                        xml)) {
                    countSuccess++;
                    delay(nudDelayFrom, nudDelayTo,
                          "Follow (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                          ") success, delay");
                    countIndex++;
                    if (countSuccess >= soLuong)
                        return DONE;
                    updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                               ")");
                    back(1, 2);
                    timeStart = high_resolution_clock::now();
                    continue;
                } else {
                    back(1,2);
                    countIndex++;
                    continue;
                }
            }
            if (element == "//android.widget.LinearLayout[@resource-id='com.instagram.android:id/profile_header_actions_top_row' and not(.//android.widget.Button[@Text='Follow'])]") {
                updateLine("Previously followed");
                countIndex++;
                back(1,2);
                continue;
            }

            updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       "), swipe");
            if (swipeAndScreenNotChange())
                break;
            if (countIndex >= 8)
                countIndex = 3;
        }
        return NOMAP;
    }

    //Go to post link
    bool handleGotoPost(string link) {
        if (link.empty())
            return false;
        gotoPost(link);
        return true;
    }

    //Follow User like post
    ResultScreen actionFollowLikedPost() {
        //Input người dùng nhập vào
        string txtLink = action.getString("txtLink");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //Input thêm biến để xử lý
        int countSuccess = 0;
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countIndex = 1;

        //goto
        updateLine("Go to post...");
        if (!handleGotoPost(txtLink))
            return NOMAP;
        delay(2);
        updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
        //timeStart
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
               "//android.widget.Button[@resource-id='com.instagram.android:id/row_feed_like_count']",
               "//*[contains(@text,'Liked')]",
               "(//android.widget.Button[@text='Follow'])[" + to_string(countIndex) + "]",
               "//*[@text=\"Some accounts prefer to manually review followers even when they're public. Let us know if you think we made a mistake.\"]",
            });

            if (element == "//android.widget.Button[@resource-id='com.instagram.android:id/row_feed_like_count']"
            || element == "//*[contains(@text,'Liked')]") {
                tapCenter(element,xml);
                delay(2);
                waitNodeHide("//*[@resource-id='com.instagram.android:id/listview_progressbar']",30);
                continue;
            }
            if (element == "(//android.widget.Button[@text='Follow'])[" + to_string(countIndex) + "]") {
                tapCenter(element,xml);
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,"Follow (" + to_string(countSuccess) + "/" + to_string(soLuong) + ") success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//*[@text=\"Some accounts prefer to manually review followers even when they're public. Let us know if you think we made a mistake.\"]"){
                tapCenter("//android.widget.Button[@text='OK']");
                countIndex++;
                delay(2);
                continue;
            }
            updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + "), swipe");
            if (swipeAndScreenNotChange())
                break;
        }
        return NOMAP;
    }

    //Handle follow follower
    bool handleFollowFollowerUser(string uid) {
        //Input người dùng nhập vào
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        //Input thêm biến cần thiết để xử lý
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        int tapFollower = 0;

        //goto
        if (!gotoProfile(uid))
            return false;
        delay(2);

        updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
        //timeStart
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
                "//android.widget.TextView[@text='followers']",
                "//android.widget.Button[@text='Follow']",
            });
            if (element == "//android.widget.TextView[@text='followers']"){
                tap(element,xml);
                tapFollower++;
                if (tapFollower >= 3)
                    return false;
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='Follow']") {
                tap(element,xml);
                countSuccess++;
                if (countSuccess >= soLuong)
                    return true;
                delay(nudDelayFollowFrom, nudDelayFollowTo,
                      "Follow (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                      ") success, delay");
                timeStart = high_resolution_clock::now();
                continue;
            }
            updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       "), swipe");
            if (swipeAndScreenNotChange())
                return false;
        }
        return false;
    }

    //Follow follower use
    ResultScreen actionFollowFollowerUser() {
        //input người dùng nhập vào
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> txtUid = action.getList("txtUid");

        //input thêm biến xử lý
        int countSuccess = 0;
        int soLuong = txtUid.size();
        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        while (true) {
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock() || isStopped())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "FB_SESSION_EXPIRED")
                break;
            string uid = Common::ranItem(txtUid);
            if (txtUid.empty())
                return NOMAP;
            Common::removeItem(txtUid, uid);
            if (handleFollowFollowerUser(uid)) {
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Follow follower user (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                      ") success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                timeStart = high_resolution_clock::now();
                continue;
            }
        }
        return NOMAP;
    }

    //Follow following user


    //Unfollow
    ResultScreen actionUnfollowUser() {
        //input người dùng nhập vào
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //input thêm biến xử lý
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        //goto
        if (!handleGotoFollowing())
            return NOMAP;
        delay(2);

        updateLine("Unfollow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(0, xml, {
                "//android.widget.LinearLayout[.//android.widget.Button[@text='Message']]/android.widget.ImageView[@content-desc='More options' and not(//android.widget.FrameLayout[@resource-id='com.instagram.android:id/layout_container_bottom_sheet'])]",
                "//android.widget.Button[@text='Unfollow' or @content-desc='Unfollow']",

            });
            if (element == "//android.widget.LinearLayout[.//android.widget.Button[@text='Message']]/android.widget.ImageView[@content-desc='More options' and not(//android.widget.FrameLayout[@resource-id='com.instagram.android:id/layout_container_bottom_sheet'])]"){
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='Unfollow' or @content-desc='Unfollow']"){
                tap(element,xml);
                delay(2);

                xml = dumpXml();
                if (existNode("//android.widget.Button[@text='Unfollow' and @resource-id='com.instagram.android:id/primary_button']",xml)){
                    tap("//android.widget.Button[@text='Unfollow' and @resource-id='com.instagram.android:id/primary_button']");
                    delay(2);
                }
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo, "Unfollow (" + to_string(countSuccess) + "/" + to_string(soLuong) + ") success, delay...");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Unfollow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                timeStart = high_resolution_clock::now();
                continue;
            }
            updateLine("Unfollow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + "), swipe...");
            if (swipeAndScreenNotChange())
                break;
        }
        return NOMAP;
    }

    //React reel
    ResultScreen actionReactReel() {
        openApp("com.instagram.android");
        delay(2);

        int cbbDoiTuong = action.getInt("cbbDoiTuong");
        if (cbbDoiTuong == 0) {
            if (!gotoReel())
                return NOMAP;
        } else {
            string tuKhoa = Common::ranItem(action.getList("txtTuKhoa"));
            tuKhoa = Common::spinText(tuKhoa);
            updateLine("Search reel: " + tuKhoa + "...");
            if (!handleGotoSearchKeyWord(tuKhoa, "Reels")) {
                return NOMAP;
            }
//            action.jsonConfig.set("isReelByKeyWords",  true);
        }
        TuongTacNhieuBaiViet("React reel");
        return DONE;
    }

    //Up avatar
    ResultScreen actionUpAvatar() {
        updateLine("Up avatar...");

        //Input người dùng nhập
        string galleryId = action.getString("galleryId");
        bool ckbSkipIfHave = action.getBool("ckbSkipIfHave");
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");

        //Xử lý input , khai báo thêm biến cần thiết

        vector<MediaS3> lstMedia = getScriptMedia(galleryId);

        if (lstMedia.empty())
            return ERROR;

        //goto
        if (!gotoProfile())
            return NOMAP;
        delay(2);

        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 100;

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
                "//*[@resource-id='com.instagram.android:id/row_profile_header_imageview' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_drag_handle'])]",
                "//*[@text='Add profile photo']",
                "//*[@text='New profile picture']",
                "//*[@resource-id='com.instagram.android:id/next_button_textview']",
                "//*[@resource-id='com.instagram.android:id/creation_next_button']",

            });
            if (element == "//*[@resource-id='com.instagram.android:id/row_profile_header_imageview' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_drag_handle'])]"){
                Node node = getNode(element,xml,2);
                tapLong(node.centerX, node.centerY);
                delay(2);
                continue;
            }
            if (element == "//*[@text='Add profile photo']") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='New profile picture']") {
                if (ckbSkipIfHave){
                    if (existNode("//*[@text='Remove current picture']",xml)){
                        if (ckbXoaAnhDaDung) {
                            deleteScriptMedia(lstMedia);
                        }
                        return DONE;
                    }
                    ckbSkipIfHave = false;
                }
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@resource-id='com.instagram.android:id/next_button_textview']"){
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@resource-id='com.instagram.android:id/creation_next_button']"){
                tap(element,xml);
                delay(2);
                waitNodeHide("//*[@content-desc='Profile photo']",30);

                if (ckbXoaAnhDaDung) {
                    deleteScriptMedia(lstMedia);
                }
                return DONE;
            }
        }
        return NOMAP;
    }

    //Nhắn tin uid
    ResultScreen actionMessageUid() {
        //Input người dùng nhập vào
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        vector<string> txtUid = action.getList("txtUid");
        //Input thêm biến xử lý
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        //goto
        updateLine("Sent message UID (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
        //timestart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            if (isStopped())
                break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;
            if (txtUid.empty())
                return NOMAP;
            string uid = Common::ranItem(txtUid);
            Common::removeItem(txtUid, uid);

            if (handleMessageUid(uid)) {
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo, "Send message UID (" + to_string(countSuccess) + "/" +
                        to_string(soLuong) + ")");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Sent message UID (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

                timeStart = high_resolution_clock::now();
                continue;
            }
        }
        return NOMAP;
    }

    //Handle goto direct message
    bool gotoDirectMessage(string link) {
        //goto
        if (!gotoProfile(link))
            return false;
        waitNodeHide("//*[@resource-id='com.instagram.android:id/message_actions_fragment']", 30);
        delay(2);

        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(0, xml, {
                "//android.widget.LinearLayout[@resource-id='com.instagram.android:id/profile_header_actions_top_row' and not(.//android.widget.Button[@content-desc='Message'])]",
                "//android.widget.Button[@content-desc='Message']",
                "//*[@resource-id='com.instagram.android:id/message_actions_fragment']",


            });
            if (element == "//android.widget.LinearLayout[@resource-id='com.instagram.android:id/profile_header_actions_top_row' and not(.//android.widget.Button[@content-desc='Message'])]")
                return false;

            if (element == "//android.widget.Button[@content-desc='Message']") {
                tap(element,xml);
                delay(2);
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (element == "//*[@resource-id='com.instagram.android:id/message_actions_fragment']")
                return true;
        }
        return false;
    }

    //Handle send message with text
    bool handleMessageWithText() {
        string xml = dumpXml();
        if (!existNode("//android.widget.EditText[@text='Message…']",xml))
            return false;

        //Input người dùng nhập vào
        vector<string> txtNoiDung = action.getList("txtNoiDung",action.getInt("typeNganCach"));

        //Input thêm biến xử lý
        string textNoiDung = Common::ranItem(txtNoiDung);

        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;
            string element = existNodes(0, xml, {
                "//android.widget.EditText[@text='Message…']",
                "//*[@text='Send' or @content-desc='Send'  and @enabled='true']",
            });

            if (element == "//android.widget.EditText[@text='Message…']") {
                inputText(element,textNoiDung);
                delay(1);
                continue;
            }
            if (element == "//*[@text='Send' or @content-desc='Send'  and @enabled='true']") {
                tap(element,xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle send message with image
    bool handleMessageWithImage() {
        string xml = dumpXml();
        if (!existNode("//android.widget.ImageView[@content-desc='Gallery']",xml))
            return false;

        //Input người dùng nhập
        int nudSoLuongAnhFrom = action.getInt("nudSoLuongAnhFrom");
        int nudSoLuongAnhTo = action.getInt("nudSoLuongAnhTo");
        string galleryId = action.getString("galleryId");
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");

        //Input thêm biến xử lý cần thiết
        int soLuongAnh = Common::ranNumber(nudSoLuongAnhFrom,nudSoLuongAnhTo);
        int countAnh = 0;


        vector<MediaS3> lstMedia = getScriptMedia(galleryId);

        if (lstMedia.empty())
            return false;

        //goto

        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;
            string element = existNodes(0, xml, {
                "//android.widget.ImageView[@content-desc='Gallery' and not(//android.view.ViewGroup[@resource-id='com.instagram.android:id/direct_media_picker_root_container'])]",
                "//android.widget.TextView[@text='Your Gallery Is Empty']",
                "//android.widget.Button[@text='Allow']",
                "//android.widget.CheckBox[@checked='false'][1]",

            });

            if (element == "//android.widget.ImageView[@content-desc='Gallery' and not(//android.view.ViewGroup[@resource-id='com.instagram.android:id/direct_media_picker_root_container'])]"){
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.TextView[@text='Your Gallery Is Empty']")
                return false;

            if (element == "//android.widget.Button[@text='Allow']"){
                tap(element,xml);
                delay(2);
                continue;
            }
            if (countAnh >= soLuongAnh || !existNode("//android.widget.CheckBox[@checked='false'][1]",xml)) {
                xml = dumpXml();
                tap("//android.widget.TextView[contains(@text,'Send') and @enabled='true']");
                delay(2);

                if (ckbXoaAnhDaDung) {
                    deleteScriptMedia(lstMedia);
                }
                return true;
            }
            if (element == "//android.widget.CheckBox[@checked='false'][1]") {
                tap(element,xml);
                countAnh++;
                delay(2);
                continue;
            }
            swipeAndScreenNotChange();
        }
        return false;
    }

    //Handle send message by uid
    bool handleMessageUid(string link) {
        //input người dùng nhập vào
        bool ckbNhanTinVanBan = action.getBool("ckbNhanTinVanBan");
        bool ckbSendAnh = action.getBool("ckbSendAnh");

        //input thêm biến xử lý nếu cần thiết

        //goto
        if (!gotoDirectMessage(link))
            return false;
        delay(2);

        //timeStart
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
                "//*[@resource-id='com.instagram.android:id/message_actions_fragment']",
            });

            if (element == "//*[@resource-id='com.instagram.android:id/message_actions_fragment']") {
                if (ckbNhanTinVanBan) {
                    updateLine("Send message...");
                    handleMessageWithText();
                }
                if (ckbSendAnh) {
                    updateLine("Send image...");
                    handleMessageWithImage();
                }
                return true;
            }
        }
        return false;
    }

    //Action interact post by hashTag
    ResultScreen actionReactHashTag() {
        //input người dùng nhập vào
        vector<string> txtHashtag = action.getList("txtHashtag");
        //input thêm biến xử lý nếu cần thiết
        string xml = dumpXml();
        //goto
        string hashTag = Common::ranItem(txtHashtag);
        updateLine("Interact: " + hashTag + "...");
        if(!handleGotoSearchKeyWord(hashTag, "Tags"))
            return ERROR;
        delay(2);
        //timeStart
        //loop
        TuongTacNhieuBaiViet("Interact post by Hashtag");
        return DONE;

    }

    //Action View Discovery
    ResultScreen actionViewDiscover() {
        //Input người dùng nhập vào

        //Input thêm biến xử lý cần thiết

        //goto
        if (!gotoHome())
            return NOMAP;
        delay(2);
        //timeStart
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
                "//android.widget.FrameLayout[@content-desc='Search and explore' and @selected='false']",
                "//android.widget.Button[@resource-id='com.instagram.android:id/image_button'][1]",
                "//android.widget.TextView[@text='Explore']",
                "//*[@content-desc='Create a reel']",
            });
            if (element == "//android.widget.FrameLayout[@content-desc='Search and explore' and @selected='false']"
             || element == "//android.widget.Button[@resource-id='com.instagram.android:id/image_button'][1]") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.TextView[@text='Explore']"
             || element == "//*[@content-desc='Create a reel']") {
                TuongTacNhieuBaiViet("View discover");
                return DONE;
            }
        }
        return NOMAP;
    }

    //Handle Add image post
    bool selectImage(int countImage, string status = "") {
        updateLine(status + " Add image...");

        //input người dùng nhập vào
        bool ckbDangAnhNgang = action.getBool("ckbDangAnhNgang");

        //input thêm biến xử lý
        int countSuccess = 0;

        //Xử lý đăng ảnh ngang
        if (ckbDangAnhNgang) {
            string xml = dumpXml();
            string changeCropXPath = "//*[@content-desc='Change crop']";
            if (existNode(changeCropXPath, xml)) {
                if (tapCenter(changeCropXPath, xml)) {
                    ckbDangAnhNgang = false; // Tắt cờ sau khi thành công
                    delay(2);
                } else {
                    updateLine(status + " Failed to change crop");
                    return false; // Thoát nếu nhấn không thành công
                }
            } else {
                updateLine(status + " Change crop button not found");
                ckbDangAnhNgang = false; // Tắt cờ nếu không tìm thấy nút
            }
        }

        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(0, xml, {
                "//android.widget.LinearLayout[@content-desc='Select multiple' and @checked='false']",
            });
            if (element == "//android.widget.LinearLayout[@content-desc='Select multiple' and @checked='false']") {
                tap(element,xml);
                delay(2);
                continue;
            }
            // Kiểm tra só ảnh đã chọn sẵn
            auto selectedNodes = getNodes("//android.view.ViewGroup[contains(@content-desc, 'selected')]",xml);
            int alreadySelected = selectedNodes.size(); // Số ảnh instagram đã chọn sẵn
            if (alreadySelected >= countImage) {
                tap("//*[@content-desc='NEXT']"); // Nếu đủ ảnh rồi, nhấn NEXT ngay
                return true;
            }

            //Chọn thêm ảnh nếu chưa đủ số lượng
            auto lstNodes = getNodes("//android.view.ViewGroup[(contains(@content-desc,'Photo thumbnail') or contains(@content-desc,'Video thumbnail')) and not(contains(@content-desc,'selected'))]",xml);
            if (!lstNodes.empty()) {
                for (const Node &node: lstNodes) {
                    int totalSelected = alreadySelected + countSuccess; // Tổng số ảnh đã chọn
                    if (totalSelected >= countImage) {
                        tap("//*[@content-desc='NEXT']");
                        return true;
                    }
                    updateLine(status + " Add image " + to_string(totalSelected + 1) + "/" +
                               to_string(countImage));
                    tap(node);
                    delay(1);
                    countSuccess++;
                }
                delay(2);
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (swipeAndScreenNotChange())
                break;
        }
        // Kiểm tra cuối cùng xem có đủ ảnh không
        string xml = dumpXml();
        int finalSelected = getNodes("//android.view.ViewGroup[contains(@content-desc, 'selected')]", xml).size();
        return finalSelected >= countImage;
    }

    //Action up post image
    ResultScreen actionUpImage() {
        //input người dùng nhập vào
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        bool ckbVanBan = action.getBool("ckbVanBan");
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");
        string postContentId = action.getString("postContentId");
        string galleryId = action.getString("galleryId");
        int nudSoLuongAnhFrom = action.getInt("nudSoLuongAnhFrom");
        int nudSoLuongAnhTo = action.getInt("nudSoLuongAnhTo");

        //input thêm biến xử lý
        int soLuong = Common::ranNumber(nudSoLuongFrom,nudSoLuongTo);
        int countSuccess = 0;

        //goto
        if (!gotoProfile())
            return ERROR;
        delay(2);

        updateLine("Up image (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

        //Download media
        int countImage = Common::ranNumber(nudSoLuongAnhFrom, nudSoLuongAnhTo);

        vector<MediaS3> lstMedia = getScriptMedia(galleryId, countImage);

        if (lstMedia.empty())
            return ERROR;

        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (countSuccess < soLuong) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            if (existNode("//*[@text=\"Can't continue editing\"]",xml)) {
                updateLine("Can't continue editing");
                return ERROR;
            }

            if (screen == "IG_PROFILE") {
                if (!tap("//android.widget.FrameLayout[@content-desc='Create' and @selected='false']",
                         xml))
                    break;
                delay(2);
                continue;
            }

            string element = existNodes(0, xml, {
                    "//android.widget.TextView[@resource-id='com.instagram.android:id/gallery_folder_menu_tv' and not(@text='Photos') and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container'])]",
                    "//*[@text='Photos' and @resource-id='com.instagram.android:id/album_filter_title']",
                    "//android.widget.TextView[@text='Photos' and @resource-id='com.instagram.android:id/gallery_folder_menu_tv']",
                    "//android.widget.Button[@content-desc='Next' and @resource-id='com.instagram.android:id/creation_next_button']",
                    "//*[@resource-id='com.instagram.android:id/bb_primary_action_container']",
            });
            if (element == "//android.widget.TextView[@resource-id='com.instagram.android:id/gallery_folder_menu_tv' and not(@text='Photos') and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container'])]"
             || element == "//*[@text='Photos' and @resource-id='com.instagram.android:id/album_filter_title']"
             || element == "//*[@resource-id='com.instagram.android:id/bb_primary_action_container']") {
                tapCenter(element, xml);
                delay(2);
                continue;
            }

            if (element == "//android.widget.TextView[@text='Photos' and @resource-id='com.instagram.android:id/gallery_folder_menu_tv']") {
                selectImage(countImage, "Up image (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@content-desc='Next' and @resource-id='com.instagram.android:id/creation_next_button']") {
                tap(element,xml);
                delay(2);
                waitNodeHide("//*[@text=\"Loading…\" or @text=\"Processing…\"]",10);
                continue;
            }

            if (ckbVanBan) {

                auto [success, content, itemId] = getScriptContent(postContentId);

                if(!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                // Common::NLog("Content: " + content, "handleTest");

                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if(!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
                hideKeyboard();
            }

            updateLine("Up image (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ") tap share");

            xml = dumpXml();
            tapCenter("//android.widget.Button[@text='Share']",xml);
            delay(2);

            waitNodeHide("//*[@text='Finishing up']",30);

            delay(nudDelayFrom, nudDelayTo,
                  "Up image (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                  ") success, delay");
            countSuccess++;

            if (ckbXoaAnhDaDung) {
                deleteScriptMedia(lstMedia);
                lstMedia = {};
            }

            if (countSuccess < soLuong) {
                countImage = Common::ranNumber(nudSoLuongAnhFrom, nudSoLuongAnhTo);

                lstMedia = getScriptMedia(galleryId, countImage);

                if (lstMedia.empty())
                    return ERROR;

                if (!gotoProfile())
                    return ERROR;
            }
            delay(2);
            updateLine("Up image (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

            timeStart = high_resolution_clock::now();
        }
        return countSuccess >= soLuong ? DONE : NOMAP;
    }

    //Action delete reel
    ResultScreen actionDeleteReel() {
        //input người dùng nhập vào
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        //input thêm biến xử lý
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        //goto
        if (!gotoProfile())
            return NOMAP;
        delay(2);

        updateLine("Remove reel (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
        //timeStart
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
                "//*[@text='Changes to deleting photos and videos']",
                "//android.widget.ImageView[@content-desc='Reels' and @selected='false']",
                "//android.widget.GridView[@resource-id='com.instagram.android:id/clips_grid_recyclerview' and not(//android.widget.ImageView[@resource-id='com.instagram.android:id/preview_clip_thumbnail'])]",
                "(//android.widget.ImageView[@resource-id='com.instagram.android:id/preview_clip_thumbnail'])[1]",
                "//android.widget.ImageView[@content-desc='More' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container'])]",
                "//android.widget.Button[@text='Delete' and @resource-id='com.instagram.android:id/control_option_text']",
                "//android.widget.Button[@text='Delete' and @resource-id='com.instagram.android:id/primary_button']",
            });
            if (element == "//*[@text='Changes to deleting photos and videos']") {
                tap("//android.widget.Button[@text='Close']",xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.ImageView[@content-desc='Reels' and @selected='false']") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.GridView[@resource-id='com.instagram.android:id/clips_grid_recyclerview' and not(//android.widget.ImageView[@resource-id='com.instagram.android:id/preview_clip_thumbnail'])]") {
                updateLine("No have reels to delete...");
                return NOMAP;
            }
            if (element == "(//android.widget.ImageView[@resource-id='com.instagram.android:id/preview_clip_thumbnail'])[1]"
             || element == "//android.widget.ImageView[@content-desc='More' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container'])]"
             || element == "//android.widget.Button[@text='Delete' and @resource-id='com.instagram.android:id/control_option_text']") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='Delete' and @resource-id='com.instagram.android:id/primary_button']") {
                tap(element,xml);
                delay(2);
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                delay(2, 3,
                      "Remove reel (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      ") success, delay");
                timeStart = high_resolution_clock::now();
                continue;
            }
            if (swipeAndScreenNotChange())
                return NOMAP;
        }
        return NOMAP;
    }

    //Action up story
    ResultScreen actionUpStory() {
        //Input người dùng nhập vào
        int soLuongFrom = action.getInt("soLuongFrom");
        int soLuongTo = action.getInt("soLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        int typeDang = action.getInt("typeDang");
        //Input thêm biến xử lý
        int soLuong = Common::ranNumber(soLuongFrom, soLuongTo);
        int countSuccess = 0;

        //goto
        if (!gotoCreateStory())
            return NOMAP;
        delay(2);

        updateLine("Up story (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
        //timeStart
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

            switch (typeDang) {
                case 0:
                    updateLine("Up story with text...");
                    handleStoryWithText();
                    break;
                case 1:
                    updateLine("Up story with media...");
                    handleStoryWithMedia();
                    break;
            }
            delay(nudDelayFrom, nudDelayTo, "Up story (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ") success, delay");
            countSuccess++;
            if (countSuccess >= soLuong)
                return DONE;
            updateLine("Up story (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
            timeStart = high_resolution_clock::now();
            if (!gotoCreateStory())
                return NOMAP;
            continue;
        }
        return NOMAP;
    }

    //Handle up story with text
    bool handleStoryWithText() {
        //input người dùng nhập vào
        vector<string> txtNoiDung = action.getList("txtNoiDung",action.getInt("typeNganCach"));
        bool ckbXoaNguyenLieuDaDung = action.getBool("ckbXoaNguyenLieuDaDung");

        //input thêm biến xử lý
        string textNoiDung = Common::ranItem(txtNoiDung);
        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                return false;

            string element = existNodes(0, xml, {
                "//*[@content-desc='Create Mode button' and @selected='false']",
                "//android.widget.EditText[@text='Tap to type' or @text='Type something']",
                "//*[@content-desc='Create Mode button' and @selected='true']",
                "//*[@content-desc='Share to']",
                "//*[@content-desc='Share']",
                "//*[@content-desc='Done']",
            });
            if (element == "//*[@content-desc='Create Mode button' and @selected='false']") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@text='Tap to type' or @text='Type something']") {
                inputText(element,textNoiDung);
                delay(1);
                back();
                delay(1);
                continue;
            }
            if (element == "//*[@content-desc='Create Mode button' and @selected='true']") {
                if (!existNode("//android.widget.EditText[@text='Tap to type' or @text='Type something']",xml)) {
                        tap(sWidth / 2, sHeight / 2);
                        delay(2);
                        continue;
                    }
            }
            if (element == "//*[@content-desc='Share to']"
             || element == "//*[@content-desc='Share']") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Done']") {
                tap(element,xml);
                if (ckbXoaNguyenLieuDaDung)
                    Common::removeItem(txtNoiDung,textNoiDung);
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle up story with media
    bool handleStoryWithMedia() {
        //input người dùng nhập vào
        string galleryId = action.getString("galleryId");
        bool ckbChiDangAnhXoaAnhDaDang = action.getBool("ckbChiDangAnhXoaAnhDaDang");
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");
        //input thêm biến xử lý

        //download media
//        if (ckbChiDangAnhXoaAnhDaDang) {
//            if (!downloadGallery(std::move(galleryId)))
//                return false;
//        } else {
//            if (!downloadGallery(galleryId))
//                return false;
//        }

        vector<MediaS3> lstMedia = getScriptMedia(galleryId);

        if (lstMedia.empty())
            return false;


        //timeStart
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
                "//*[@content-desc='Gallery']",
                "(//android.view.View[@resource-id='com.instagram.android:id/gallery_grid_item_thumbnail'])[1]",
                "//*[@content-desc='Share to']",
                "//*[@content-desc='Share']",
                "//*[@content-desc='Done']",

            });
            if (element == "//*[@content-desc='Gallery']") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "(//android.view.View[@resource-id='com.instagram.android:id/gallery_grid_item_thumbnail'])[1]"
            || element == "//*[@content-desc='Share to']"
            || element == "//*[@content-desc='Share']") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Done']") {
                tap(element,xml);
                delay(2);

                if (ckbXoaAnhDaDung) {
                    deleteScriptMedia(lstMedia);
                }

                return true;
            }
        }
        return false;
    }
    //Action nghỉ giải lao
    ResultScreen actionSleep() {
        delay(action.getInt("nudDelayFrom"), action.getInt("nudDelayTo"), "Free time...");
        return NOMAP;
    }

    //Action bật chế độ riêng tư
    ResultScreen actionAccountPrivate() {
        updateLine("Turn on/turn off account privacy...");
        //input người dùng nhập vào
        int cbbOptionsPost = action.getInt("cbbOptionsPost");

        //goto
        gotoSettings();
        delay(2);

        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 120;

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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            bool isPrivate = existNode(
                    "//android.view.View[@checked='true'][android.view.View/android.widget.TextView[@text='Private account']][not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container'])]",
                    xml);
            bool isPublic = existNode(
                    "//android.view.View[@checked='false'][android.view.View/android.widget.TextView[@text='Private account']][not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container'])]",
                    xml);

            if (cbbOptionsPost == 0 && isPrivate) {
                updateLine("Account is already Private");
                delay(2);
                return DONE;
            } else if (cbbOptionsPost == 1 && isPublic) {
                updateLine("Account is already Public");
                delay(2);
                return DONE;
            }

            string element = existNodes(0, xml, {
                    "//*[@content-desc='Account privacy. Shows whether you account is currently set to public or private. Double tap to open a screen to change your account privacy.']",// mở account privacy lên
                    "//android.view.View[android.view.View/android.widget.TextView[@text='Private account']][not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container'])]",
                    "//*[contains(@content-desc,'Switch to ')]",
                    "//*[@text=\"Creator account can't be private\"]",
            });
            if (element == "//*[@content-desc='Account privacy. Shows whether you account is currently set to public or private. Double tap to open a screen to change your account privacy.']"
             || element == "//android.view.View[android.view.View/android.widget.TextView[@text='Private account']][not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container'])]"
             || element == "//*[contains(@content-desc,'Switch to ')]") {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text=\"Creator account can't be private\"]") {
                updateLine("Creator account can't be private");
                return NOMAP;
            }
            if (swipeAndScreenNotChange())
                return NOMAP;
        }
        return NOMAP;
    }

    //Action bật chế độ chuyên nghiệp
    ResultScreen actionAccountProfessional() {
        updateLine("Turn on/turn off account professional...");
        int cbbOptionsPost = action.getInt("cbbOptionsPost");

        //goto
        gotoSettings();
        delay(2);

        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 120;

        // Danh sách các phần tử trong giao diện
        vector<string> lstElement;
        if (cbbOptionsPost == 0) {
            lstElement = {
                    // Bật chế độ chuyên nghiệp
                    "//*[@content-desc='Account type and tools. Double tap to open a surface to manage your account type as well as other professional tools.']",
                    "//*[@text='Switch to professional account']",
                    "//*[@content-desc='Continue' and not(parent::* = //*[@text='Not now']/parent::*)]",
                    "//*[@class='android.widget.RadioButton' and @text='Digital creator' and @checked='false']",
                    "//*[@text='Done']",
                    "//android.widget.TextView[@text='Creator']/following-sibling::android.widget.RadioButton[@checked='false']",
                    "//*[@content-desc='Next']",
                    "//*[@text='Not now']",
                    "//android.widget.Button[@content-desc='Close' and //*[contains(@text, 'Set Up Your Professional Account')]]",
            };
        } else {
            lstElement = {
                    // Tắt chế độ chuyên nghiệp
                    "//*[@text='Creator tools and controls']",
                    "//android.widget.TextView[(@text='Switch account type' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_drag_handle']))]",
                    "//*[@text='Switch to personal account' and @resource-id='com.instagram.android:id/action_sheet_row_text_view']",
                    "//*[@text='Switch to personal account' and @resource-id='com.instagram.android:id/primary_button']",
            };
        }
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            //Kiểm tra trạng thái của tài khoản
            bool isOn = existNode("//*[@text='Creator tools and controls']",xml);
            bool isOff = existNode("//*[@text='Account type and tools']",xml);

            //Kiểm tra trạng thái hiện tại của tài khoản so với yêu cầu
            if (cbbOptionsPost == 0 && isOn) {
                updateLine("Account is already Professional");
                delay(2);
                return DONE;
            } else if (cbbOptionsPost == 1 && isOff) {
                updateLine("Account is already Personal");
                delay(2);
                return DONE;
            }

            string element = existNodes(0, xml, lstElement);

            if (!element.empty()) {
                tap(element,xml);
                delay(2);
                continue;
            }
            if (swipeAndScreenNotChange(1,1,1000))
                return NOMAP;
        }

        return NOMAP;
    }

    //Handle react profile by UID
    bool handleReactProfileUid(string status) {
        vector<string> lstId = action.getList("txtId");
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongProfileFrom"),
                                        action.getInt("nudSoLuongProfileTo"));
        int countSuccess = 0;

        if (lstId.empty())
            return false;

        for (int i = 0; i < soLuong; i++) {
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;

            if (lstId.empty())
                break;

            string id = Common::ranItem(lstId);
            Common::removeItem(lstId, id);

            if (!gotoProfile(id))
                return false;

            delay(3);

            string xml = dumpXml();
            string screen = handleScreen(xml);

            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                continue;

            string element = existNodes(0, xml, {
                    "//android.widget.TextView[@text='This Account is Private']",
                    "//android.widget.TextView[@text='No Posts Yet']",
                    "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1]",
                    "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']",
            });

            if (element == "//android.widget.TextView[@text='This Account is Private']" ||
                element == "//android.widget.TextView[@text='No Posts Yet']")
                return false;

            if (element == "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1]") {
                tapCenter(element);
                delay(2);
                // Cập nhật lại giao diện sau khi nhấn nút
                xml = dumpXml();
                element = existNodes(0, xml, {
                        "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']",
                });
            }
            if (element == "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']") {
                TuongTacNhieuBaiViet(status + " (" + to_string(i + 1) + "/" + to_string(soLuong) + ")");
                countSuccess++;
            }
        }

        return countSuccess > 0;
    }

    //Handle react profile
    bool handleReactProfile(string status) {
        //goto
        if (!gotoProfile())
            return false;
        delay(2);

        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(0, xml, {
                    "//*[@text=\"Create your first post\"]",
                    "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1]",
                    "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']",
            });
            if (element == "//*[@text=\"Create your first post\"]") {
                updateLine("No post to interact...");
                delay(1);
                return false;
            }

            if (element == "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1]") {
                tapCenter(element);
                delay(2);
                continue;
            }
            if (element ==
                "//android.widget.TextView[(@text='Posts' or @content-desc='Posts') and @resource-id='com.instagram.android:id/action_bar_title']") {
                TuongTacNhieuBaiViet("React your profile...");
                return true;
            }
        }
        return false;
    }

    //Action tương tác profile
    ResultScreen actionReactProfile() {
        int cbbDoiTuong = action.getInt("cbbDoiTuong");
        if (cbbDoiTuong == 0) {

            handleReactProfile("React your profile...");
            return DONE;
        } else {
            handleReactProfileUid("React Profile");
            return DONE;
        }
        return NOMAP;
    }

    //Action tương tác bài viết chỉ định
    ResultScreen actionReactPostUid() {
        //Input người dùng nhập vào
        vector<string> txtIdPost = action.getList("txtIdPost");
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                        action.getInt("nudSoLuongTo"));
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        string commentContentId = action.getString("commentContentId");

        bool ckbSave = action.getBool("ckbSave");
        int nudPercentSave = action.getInt("nudPercentSave");

        bool ckbTag = action.getBool("ckbTag");
        vector<string> txtUidTag = action.getList("txtUidTag");
        int typeTag = action.getInt("typeTag");
        int nudSoLuongTagFrom = action.getInt("nudSoLuongTagFrom");
        int nudSoLuongTagTo = action.getInt("nudSoLuongTagTo");

        //Input thêm biến xử lý
        int countSuccess = 0;
        string xml;
        string postLink = Common::ranItem(txtIdPost);
        string textUser = Common::ranItem(txtUidTag);
        string status;

        if (commentContentId.empty())
            ckbComment = false;
        if (textUser.empty())
            ckbTag = false;
        if (postLink.empty())
            return NOMAP;

        updateLine("Interact post (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        while (true) {
            status = "Post (" + to_string(countSuccess + 1) + ") ";
            updateLine(status + "Open...");

            gotoPost(postLink);
            delay(2);
            waitNode("//*[@content-desc='More actions for this post']" ,xml , 3);
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            Common::removeItem(txtIdPost, postLink);
            delay(nudTimeFrom, nudTimeTo, status + "View post...");

            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(2);
            }
            if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");
                if (commentTagUser(commentContentId, xml, ckbTag, txtUidTag , Common::ranNumber(nudSoLuongTagFrom, nudSoLuongTagTo)))
                    delay(2);
            }
            if (ckbSave && Common::ranBool(nudPercentSave)) {
                updateLine(status + "Save...");
                if (savePost(xml))
                    delay(2);
            }
            countSuccess++;
            if (countSuccess >= soLuong)
                return DONE;

            if (postLink.empty())
                break;
            postLink = Common::ranItem(txtIdPost);
        }
        return NOMAP;
    }

    //Handle Follow user
    bool followUserUid(string user) {
        if (!gotoProfile(user))
            return false;
        waitNodeHide("//*[@resource-id='com.instagram.android:id/profile_header_follow_button']",3);
        delay(2);

        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(0, xml, {
                "//android.widget.LinearLayout[@resource-id='com.instagram.android:id/profile_header_actions_top_row']//android.widget.Button[@text='Follow' or @text='Follow back']",
                "//android.widget.LinearLayout[@resource-id='com.instagram.android:id/profile_header_actions_top_row' and not(.//android.widget.Button[@Text='Follow'])]",
            });
            if (element == "//android.widget.LinearLayout[@resource-id='com.instagram.android:id/profile_header_actions_top_row']//android.widget.Button[@text='Follow' or @text='Follow back']") {
                tapCenter(element , xml);
                delay(2);
                return true;
            }
            if (element == "//android.widget.LinearLayout[@resource-id='com.instagram.android:id/profile_header_actions_top_row' and not(.//android.widget.Button[@Text='Follow'])]")
                return false;
        }
        return false;
    }


    //Action follow theo uid
    ResultScreen actionFollowUid() {
        //Input người dùng nhập vào
        vector<string> txtUid = action.getList("txtUid");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        //Input thêm biến xử lý
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        updateLine("Follow user (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            string xml = dumpXml();
            string screen = handleScreen(xml);

            string uid = Common::ranItem(txtUid);
            Common::removeItem(txtUid, uid);
            if(txtUid.empty())
                return NOMAP;

            if (followUserUid(uid)) {
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Follow user (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                      ") success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine("Follow user (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                continue;
            }
        }
        return NOMAP;
    }

    //Handle follow follower
    bool handleFollowFollowingUser(string uid) {
        //Input người dùng nhập vào
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        //Input thêm biến cần thiết để xử lý
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        int tapFollower = 0;

        //goto
        if (!gotoProfile(uid))
            return false;
        delay(5);

        updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(0, xml, {
                    "//android.widget.TextView[@text='following']",
                    "//android.widget.Button[@text='Follow']",
            });
            if (element == "//android.widget.TextView[@text='following']"){
                tap(element,xml);
                tapFollower++;
                if (tapFollower >= 3)
                    return false;
                delay(2);
                continue;
            }
            if (element == "//android.widget.Button[@text='Follow']") {
                tap(element,xml);
                countSuccess++;
                if (countSuccess >= soLuong)
                    return true;
                delay(nudDelayFollowFrom, nudDelayFollowTo,
                      "Follow (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                      ") success, delay");
                timeStart = high_resolution_clock::now();
                continue;
            }
            updateLine("Follow (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       "), swipe");
            if (swipeAndScreenNotChange())
                return false;
        }
        return false;
    }

    //Action follow following user
    ResultScreen actionFollowFollowingUser() {
        //input người dùng nhập vào
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> txtUid = action.getList("txtUid");

        //input thêm biến xử lý
        int countSuccess = 0;
        int soLuong = txtUid.size();
        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        while (true) {
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock() || isStopped())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;
            string uid = Common::ranItem(txtUid);
            if (txtUid.empty())
                return NOMAP;
            Common::removeItem(txtUid, uid);
            if (handleFollowFollowingUser(uid)) {
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Follow following user (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                      ") success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                timeStart = high_resolution_clock::now();
                continue;
            }
        }
        return NOMAP;
    }

    //Action chạy jobs kiếm tiền
    ResultScreen actionEarnMoney() {
        return NOMAP;
    }

    //Action xóa bài viết trên tường
    ResultScreen actionDeletePostWall() {
        //Input người dùng nập
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int cbbOptionsPost = action.getInt("cbbOptionsPost");

        //Input thêm biến xử lý cần thiết
        int countSuccess = 0;

        //goto profile
        if (!gotoProfile())
            return NOMAP;
        delay(2);

        updateLine("Hide post (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            if (existNode("//*[@text='Create your first post']",xml)) {
                updateLine("No post to hide...");
                delay(1);
                return NOMAP;
            }

            string element = existNodes(0, xml, {
                "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1]",
                "//android.widget.ImageView[@content-desc='More actions for this post' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_drag_handle'])]",
                "//*[@text='Archive']",
            });

            if (element == "//androidx.recyclerview.widget.RecyclerView/android.widget.LinearLayout[1]/android.widget.Button[1]") {
                tapCenter(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.ImageView[@content-desc='More actions for this post' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_drag_handle'])]") {
                tapCenter(element,xml);
                delay(2);
                continue;
            }

            if (element == "//*[@text='Archive']") {
                tap(element,xml);
                countSuccess++;
                if (countSuccess >= soLuong)
                    return DONE;
                waitNodeHide("//*[@text=\"Loading…\"]",10);
                delay(2, 3, "Hide post (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                      ") success, delay");
                timeStart = high_resolution_clock::now();
                continue;
            }
            updateLine("Hide post (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                       "), swipe");
            if (swipeAndScreenNotChange())
                return NOMAP;
        }
        return NOMAP;
    }

    //Action đổi mật khẩu
    ResultScreen actionChangePassword(){
        return NOMAP;
    }

    //Action thêm mail
    ResultScreen actionAddMail() {
        return NOMAP;
    }

    //Action bật - tắt 2FA
    ResultScreen actionTurnOnOff2fa() {
        return NOMAP;
    }

    //Action đổi tên
    ResultScreen actionChangeName() {
        //input
        string name;

        int cbbTypeName = action.getInt("cbbTypeName");
        if (cbbTypeName == 0) {
            int typeName = action.getInt("typeName"); // ngẫu nhiên : 0 - tên việt, 1 - tên ngoại
            name = typeName == 0 ? Common::ranNameViet() : Common::ranNameNgoai();
        } else if (cbbTypeName == 1) {
            vector<string> lstFirstName = action.getList("lstFirstName");
            vector<string> lstMiddleName = action.getList("lstMiddleName");
            vector<string> lstLastName = action.getList("lstLastName");

            name = Common::ranItem(lstFirstName) + " " +
                   Common::ranItem(lstMiddleName) + " " +
                   Common::ranItem(lstLastName);
            name = Common::strReplace(name, "  ", " ");
        } else if (cbbTypeName == 2) {
            vector<string> lstFullName = action.getList("lstFullName");
            name = Common::ranItem(lstFullName);
        }
        updateLine("Change name to: " + name);
        if (!gotoProfile())
            return NOMAP;
        delay(2);

        //loop
        for (int loop = 50; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            string xml = dumpXml();
            string screen = handleScreen(xml);

            if (screen == "IG_PROFILE") {
                tap("//*[@content-desc='Edit profile' or @text='Edit profile']");
                delay(2);
                continue;
            }
            if (screen == "IG_EDIT_PROFILE") {
                tap("//*[@text='Name' or @content-desc='Name']");
                delay(2);
                continue;
            }
            if (screen == "IG_CHANGE_NAME") {
                inputText("//android.widget.EditText", name);
                delay(1);
                tap("//*[@content-desc='Done']");
                delay(2);
                continue;
            }
            if (screen == "IG_CONFIRM_CHANGE_NAME"){
                tap("//*[@text='Change name']");
                delay(2);
                return DONE;
            }
        }
        return NOMAP;
    }

    //Action cập nhật thông tin
    ResultScreen actionUpdateProfile() {
        return NOMAP;
    }

    //Action xóa số điện thoại
    ResultScreen actionDeletePhoneNumber() {
        return NOMAP;
    }

    //Handle goto story
    bool gotoStory() {
        Start:
        if (!gotoHome())
            return false;
        delay(2);
        swipe(-1);
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                goto Start;
            if (screen == "IG_STORY")
                return true;
            string element = existNodes(0, xml, {
               "//*[contains(@content-desc, \"story at column 1.\")]",

            });
            if (element.empty())
                return false;

            if (!element.empty()) {
                tapCenter(element,xml);
                delay(2);
                continue;
            }
            if (!isAppOpened("com.instagram.android"))
                openApp("com.instagram.android");
        }
        return false;
    }

    //Action tương tác story
    ResultScreen actionReactStory() {
        if (!gotoStory())
            return ERROR;

        TuongTacNhieuBaiViet("React Story");
        return DONE;
    }

    //Action đăng reel
    ResultScreen actionUpReel() {
        //Input người dùng nhập vào
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom"); // Số lượng from
        int nudSoLuongTo = action.getInt("nudSoLuongTo");     // Số lượng to
        int nudDelayFrom = action.getInt("nudDelayFrom");     // Thời gian giãn cách from
        int nudDelayTo = action.getInt("nudDelayTo");         // Thời gian giãn cách to
        int nudSoLuongTagFrom = action.getInt("nudSoLuongTagFrom"); // Số lượng tag from
        int nudSoLuongTagTo = action.getInt("nudSoLuongTagTo");     // Số lượng tag to
        int nudTimeoutLoadVideo = action.getInt("nudTimeoutLoadVideo"); // Thời gian timeout load video
        bool ckbVanBan = action.getBool("ckbVanBan");
        bool ckbThemNhac = action.getBool("ckbThemNhac");
        bool ckbTag = action.getBool("ckbTag");
        bool ckbLocation = action.getBool("ckbLocation");
        bool cbbWhenTimeout = action.getBool("cbbWhenTimeout");
        bool ckbTuongTacReel = action.getBool("ckbTuongTacReel");
        string galleryId = action.getString("galleryId");
        string txtLocation = action.getString("txtLocation");
        string postContentId = action.getString("postContentId");

        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");


        //Input biến xử lý cần thiết
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;

        vector<MediaS3> lstMedia = getScriptMedia(galleryId);

        if (lstMedia.empty())
            return ERROR;

        //goto
        if (!gotoCreateReel())
            return NOMAP;
        delay(2);

        updateLine("Up reels (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;

            string xml = dumpXml();
            string screen = handleScreen(xml);
            if(IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(3, xml, {
                    "//*[@content-desc='Gallery']",
                    "(//*[contains(@content-desc,'Video thumbnail')])[1]",
                    "//android.widget.Button[@content-desc='Next' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container' or @resource-id='com.instagram.android:id/share_button'])]",
                    "//*[@content-desc='New reel' and @resource-id='com.instagram.android:id/action_bar_title']",
            });
            if (element == "//*[@content-desc='Gallery']"
                || element == "(//*[contains(@content-desc,'Video thumbnail')])[1]") {
                tap(element,xml);
                delay(2);
                continue;
            }

            if (element == "//android.widget.Button[@content-desc='Next' and not(//*[@resource-id='com.instagram.android:id/bottom_sheet_container' or @resource-id='com.instagram.android:id/share_button'])]") {
                if (ckbThemNhac) {
                    handleAddMusicReel("Up reel (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                    continue;
                } else {
                    tap(element,xml);
                    delay(2);
                    continue;
                }
            }
            xml = dumpXml();
            if (element == "//*[@content-desc='New reel' and @resource-id='com.instagram.android:id/action_bar_title']") {
                if (ckbVanBan) {
                    handleTextReel(postContentId,"Up reel (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                }
                if (ckbTag) {
                    handleTagPeople(Common::ranNumber(nudSoLuongTagFrom, nudSoLuongTagTo),"Up reel (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                }
                if (ckbLocation) {
                    handleAddLocation(txtLocation,"Up reel (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");
                }
                updateLine("Up reel (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                           ") tap share");
                xml = dumpXml();
                string nextXpath = "//*[@content-desc='Next' and @resource-id='com.instagram.android:id/share_button']";
                if (existNode(nextXpath,xml)) {
                    tap(nextXpath,xml);
                    delay(2);
                    tap("//*[@content-desc='Share']");
                    delay(2);
                } else {
                    tap("//*[@content-desc='Share']");
                    delay(2);
                }
                updateLine("Loading up reels...");

                if (ckbXoaAnhDaDung) {
                    deleteScriptMedia(lstMedia);
                }

                bool isSuccess = handleLoadingUpReel(nudTimeoutLoadVideo,cbbWhenTimeout);
                if (!isSuccess)
                    return NOMAP;

                delay(nudDelayFrom, nudDelayTo, "Up reel (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ") success, delay");
                countSuccess++;
                if (countSuccess >= soLuong)    //Khi đạt số lượng
                    return DONE;
                updateLine("Up reels (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) + ")");

                timeStart = high_resolution_clock::now();
                continue;
            }
            if (!gotoCreateReel())
                return ERROR;
        }
        return NOMAP;
    }

    //Handle add music up reel
    bool handleAddMusicReel(string status = "") {
        updateLine(status + " Add music...");
        int typeBaiHat = action.getInt("typeBaiHat");
        bool ckbMuteVolume = action.getBool("ckbMuteVolume");
        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 100;

        //loop
        while (true) {
            if(Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if(IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if(Common::strContains(screen, "_POPUP_"))
                continue;
            if(screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(3, xml, {
               "//*[@content-desc='Double tap for tools that edit your video clips.']",
               "//*[@text='Tap to add audio']",
               "//*[@content-desc='New reel' and @resource-id='com.instagram.android:id/action_bar_title']",
            });
            if (element == "//*[@content-desc='Double tap for tools that edit your video clips.']"
            || element == "//*[@text='Tap to add audio']") {
                tapCenter(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='New reel' and @resource-id='com.instagram.android:id/action_bar_title']"){
                return true;
            }

            bool musicAdded = false;
            if (typeBaiHat == 0) {
                handleAddRandomMusic(ckbMuteVolume);
            } else {
                handleAddMusicKeyWord(ckbMuteVolume);
            }
            if (musicAdded)
                continue;
        }
        return false;
    }

    //Handle add random music
    bool handleAddRandomMusic(bool isMuteVolume) {
        auto timeStart = high_resolution_clock::now();
        int timeout = 120;

        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(3, xml, {
                    "//*[@resource-id='com.instagram.android:id/track_container']",
                    "//*[@content-desc='Next']",
            });
            if (element == "//*[@resource-id='com.instagram.android:id/track_container']") {
                tapCenter(element, xml);
                delay(2);
                tapCenter("//*[@content-desc='Done']");
                delay(2);
                continue;
            }
            if (element == "//*[@content-desc='Next']") {
                if (isMuteVolume) {
                    handleMuteVolume();
                }
                tapCenter(element, xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle add music by keyword
    bool handleAddMusicKeyWord(bool isMuteVolume) {
        //input người dùng nhâp vào
        vector<string> txtDanhSachBaiHat = action.getList("txtDanhSachBaiHat");

        //input thêm biến xử lý
        string textMusic = Common::ranItem(txtDanhSachBaiHat);

        auto timeStart = high_resolution_clock::now();
        int timeout = 120;

        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(3, xml, {
                    "//android.widget.EditText[@text='Search music']",
                    "(//android.view.ViewGroup[@resource-id='com.instagram.android:id/track_container'])[1]",
                    "//*[@content-desc='Next']",

            });
            if (element == "//android.widget.EditText[@text='Search music']") {
                inputText(element, textMusic);
                delay(1);
                inputEnter();
                delay(2);
                continue;
            }
            if (element == "(//android.view.ViewGroup[@resource-id='com.instagram.android:id/track_container'])[1]") {
                tapCenter(element,xml);
                delay(2);
                tapCenter("//android.widget.Button[@content-desc='Done']");
                delay(2);
                if (isMuteVolume)
                    handleMuteVolume();
                continue;
            }
            if (element == "//*[@content-desc='Next']") {
                tapCenter(element,xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle Mute Volume
    bool handleMuteVolume() {
        auto timeStart = high_resolution_clock::now();
        int timeout = 120;
        bool isTapVolume = false;
        bool isTapPause = false;
        while (true) {
            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;

            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;
            vector<Node> lstNodes = getNodes("//*[@resource-id='com.instagram.android:id/action_bar_recycler_view']",xml);

            // Kiểm tra tap Pause trước khi swipe
            if (!isTapPause) {
                string pauseXpath = "//*[@content-desc='Pause']";
                if (existNode(pauseXpath,xml)) {
                    tapCenter(pauseXpath, xml);
                    isTapPause = true;
                    delay(2);
                    continue;
                }
            }

            // Sau khi tap pause , kiểm tra và tap vào Volume
            if (!isTapVolume) {
                xml = dumpXml();
                string volumeXpath = "//android.widget.Button[@content-desc='Volume' and not(//*[@resource-id='com.instagram.android:id/stacked_timeline_bottom_sheet'])]";
                if (existNode(volumeXpath,xml)) {
                    tapCenter(volumeXpath, xml);
                    isTapVolume = true;
                    delay(2);
                    continue;
                } else {
                    swipe(lstNodes.back(), lstNodes.front());
                    delay(1);
                    continue;
                }
            }
            string element = existNodes(3, xml, {
                    "//android.widget.ImageView[@content-desc='Mute' and not(ancestor::android.view.ViewGroup[contains(@content-desc,'Global clip audio')])]",
                    "//android.widget.Button[@text='Done']",
                    "//*[@resource-id='com.instagram.android:id/stacked_timeline_track_imageview']",
            });
            if (element == "//android.widget.ImageView[@content-desc='Mute' and not(ancestor::android.view.ViewGroup[contains(@content-desc,'Global clip audio')])]") {
                tapCenter(element, xml);
                delay(2);
                tapCenter("//android.widget.Button[@text='Done']");
                delay(2);
                continue;
            }
            if (element == "//*[@resource-id='com.instagram.android:id/stacked_timeline_track_imageview']") {
                tapCenter("//*[@resource-id='com.instagram.android:id/clips_editor_video_original_audio_volume_toggle_button_image']");
                delay(2);
                return true;
            }
        }
        return false;
    }

    //Handle add text up reel
    bool handleTextReel(string txtNoiDung , string status = "") {
        updateLine(status + " Add caption...");
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(3, xml, {
                    "//android.widget.EditText[@text='Write a caption and add hashtags…' or @text='Write a caption or add a poll…']",
            });

            if (element == "//android.widget.EditText[@text='Write a caption and add hashtags…' or @text='Write a caption or add a poll…']") {
                // Nhập nội dung
                auto [success, content, itemId] = getScriptContent(txtNoiDung);

                if(!success) {
                    saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
                    return NOMAP; // chay tiep hoac dung lai
                }

                // comment, dang bai, .....
                //  Common::NLog("Content: " + content, "handleTest");
                inputText("//android.widget.EditText", content);
                // xoa content sau khi su dung

                if(!itemId.empty()) {
                    deleteScriptContent(itemId);
                }
                delay(1);
                hideKeyboard();
                return true;
            }
        }
        return false;
    }

    //Handle Loading up reel
    bool handleLoadingUpReel(int nudTimeoutLoadVideo, int cbbWhenTimeout) {
        auto timeStart = high_resolution_clock::now();
        while (true) {
            string xml = dumpXml();
            //Kiểm tra nếu progress bar đã biến mẩt
            if (!existNode("//*[@text='Sharing to Reels…']", xml)) {
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

    //Handle Tag People reel
    bool handleTagPeople(int countTag, string status = "") {
        updateLine(status + " Tag people...");
        //Input người dùng nhập vào
        vector<string> txtUidTag = action.getList("txtUidTag");

        //Input thêm biến xử lý cần thiết
        int countSuccess = 0;

        updateLine(status + " Tag people (" + to_string(countSuccess + 1) + "/" +
                   to_string(countTag) + ")");

        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;

        //loop
        while (true) {
            string uidTag = Common::ranItem(txtUidTag);

            if (Common::hasExceededTimeout(timeStart, timeout))//Khi timeout
                break;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (IsLogout() || IsCheckpoint() || IsStop() || IsBlock())
                break;
            if (Common::strContains(screen, "_POPUP_"))
                continue;
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(3, xml, {
               "//android.widget.TextView[@text='Tag people' and @resource-id='com.instagram.android:id/title']",
               "//android.widget.TextView[@text='Add tag' and @content-desc='Add tag']",
               "//*[@text='Search for a user']",
               "(//android.widget.FrameLayout[@resource-id='com.instagram.android:id/row_search_user_container'])[1]",
            });
            if (element == "//android.widget.TextView[@text='Tag people' and @resource-id='com.instagram.android:id/title']"
            || element == "//android.widget.TextView[@text='Add tag' and @content-desc='Add tag']") {
                tapCenter(element,xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='Search for a user']") {
                inputText(element, uidTag);
                Common::removeItem(txtUidTag, uidTag);
                delay(1);
                continue;
            }
            if (element == "(//android.widget.FrameLayout[@resource-id='com.instagram.android:id/row_search_user_container'])[1]") {
                tapCenter(element,xml);
                delay(2, 3, " Tag people (" + to_string(countSuccess + 1) + "/" + to_string(countTag) + ") success, delay");
                countSuccess++;
                updateLine(status + " Tag people (" + to_string(countSuccess + 1) + "/" +
                           to_string(countTag) + ")");
                if (countSuccess >= countTag || txtUidTag.empty()) {
                    tap("//android.widget.Button[@content-desc='Done']");
                    delay(2);
                    return true;
                }
                continue;
            }
        }
        return false;
    }

    //Handle adđ location reel
    bool handleAddLocation(string location , string status = "") {
        updateLine(status + " Add location...");
        //timeStart
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
            if (screen == "IG_SUSPENDED_ACCOUNT")
                break;

            string element = existNodes(3, xml, {
                "//*[@text='Add location' and @resource-id='com.instagram.android:id/location_label']",
                "//android.widget.EditText[@text='Search for a location…']",
                "(//*[@resource-id='com.instagram.android:id/row_venue_title'])[1]",
                });
            if (element == "//*[@text='Add location' and @resource-id='com.instagram.android:id/location_label']") {
                tapCenter(element,xml);
                delay(2);
                continue;
            }
            if (element == "//android.widget.EditText[@text='Search for a location…']") {
                inputText(element, location);
                delay(1);
                continue;
            }
            if (element == "(//*[@resource-id='com.instagram.android:id/row_venue_title'])[1]") {
                tapCenter(element,xml);
                delay(2);
                return true;
            }
        }
        return false;
    }

};