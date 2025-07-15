#include "TiktokExt.h"

class TiktokCore : public TiktokExt {
public:




    void handleTest() {
        updateLine("handleTest");


        updateLine("handleDone");
    }


    bool loginGoogleAccount() {
        string userWeb1989 = "ssonnam20017";
        string passWeb1989 = "Long0987793405@";
        string productIdWeb1989 = "5";
        if (userWeb1989.empty() ||
            passWeb1989.empty() ||
            productIdWeb1989.empty()) {
            return ERROR;
        }
        auto result = buyGmail(userWeb1989, passWeb1989, productIdWeb1989);

        if (!result.first) return ERROR;
        string mailGG = Common::strSplit(result.second, "|")[0];
        string mkGG = Common::strSplit(result.second, "|")[1];

        bool loginDone = false;

        JSON json;
        json.set("action", "android.settings.SYNC_SETTINGS");
        if (!Device::startActivityByIntent(json)) {
            delay(5, "Can't open settings", true);
            return false;
        }
        searchAddAccount:
        string xml = dumpXml();
        if (!existNode("//*[@text='Add account']", xml, 3)) {
            swipe();
            delay(1);
            goto searchAddAccount;
        }
        tap("//*[@text='Add account']");

        searchGoogleAccount:
        xml = dumpXml();
        if (!existNode("//*[@text='Google']", xml, 3)) {
            swipe();
            delay(1);
            goto searchGoogleAccount;
        }
        tap("//*[@text='Google']");

        for (int i = 0; i < 100; ++i) {
            if (isStopped())
                return ERROR;
            xml = dumpXml();
            string screen = detectScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen == "TIK_REGMAIL_INPUTMAIL") {
                inputText("//android.widget.EditText[@class='android.widget.EditText']", mailGG);
                delay(1);
                tap("//*[@text='Next']");
                delay(1);
                continue;
            }

            if (screen == "TIK_REGMAIL_INPUTPASSMAIL") {
                inputText("//android.widget.EditText[@class='android.widget.EditText']", mkGG);
                delay(1);
                tap("//*[@text='Next']");
                delay(1);
                loginDone = true;
                continue;
            }
            if (screen == "TIK_REGMAIL_INPUTPASSMAIL_ERROR") {
                inputText("//android.widget.EditText[@class='android.widget.EditText']", mkGG);
                delay(1);
                tap("//*[@text='Next']");
                delay(1);
                loginDone = true;
                continue;
            }


            if (screen == "GOOGLE_RE_CAPTCHA") {
                delay(60, "Captcha Awards: ");
                continue;
            }


            if (screen == "GOOGLE_ADD_PHONE_NUMBER") {
                searchYesIm:
                xml = dumpXml();
                if (!existNode("//*[@text='Yes, I’m in']", xml, 3)) {
                    swipe();
                    delay(1);
                    goto searchYesIm;
                }
                tap("//*[@text='Yes, I’m in']");
                delay(1);
                continue;
            }

            if (screen == "GOOGLE_REVIEW_YOUR_ACCOUNT") {
                tap("//*[@text='Next']");
                delay(1);
                continue;
            }
            if (screen == "GOOGLE_PRIVACE_AND_TERMS") {
                searchIAgree:
                xml = dumpXml();
                if (!existNode("//*[@text='I agree']", xml, 3)) {
                    if (swipeAndScreenNotChange()) {
                        continue;
                    }
                    delay(1);
                    goto searchIAgree;
                }
                delay(1);
                continue;
            }

            if (loginDone) {
                if (existNode("//*[@text='Add account']", xml, 0)) {
                    updateLine("Login Google Account Done");
                    return true;
                }
                continue;
            }
        }
        return false;
    }

    JSON registerTiktokV3(JSON json) {
        vector<string> lstEmail = {"cvdfd56456", "ewrwe34534", "fegert", "345345terte", "ert43435",
                                   "rtrt4343"
        };
        string email = Common::ranItem(lstEmail);

        JSON output;
        int delayBirthdayFrom = json.getInt("delayBirthdayFrom");
        int delayBirthdayTo = json.getInt("delayBirthdayTo");

        int ranTimeComfirmNameFrom = 20;
        int ranTimeComfirmNameTo = 30;
        bool ckbSwitchTBA = json.getBool("isSwitchBusiness");

        string txtPassword = json.getString("defaultPassword");
        bool ckbRandomPass = json.getBool("isRandomPass");
        string password = Common::randomChar(txtPassword);
        if (ckbRandomPass) {
            int nudCountCharPass = json.getInt("countRandomerCharPass");
            password = Common::ranString(1, "A") + Common::ranString(1, "a") +
                       Common::ranString(1, "0") +
                       Common::ranString(nudCountCharPass - 4, "a_0_A") + "@";
        }

        string name;
        bool ckbSetName = json.getBool("isRename");
        bool isRegDone = false;
        int countAgain = 0;
        int maxAgain = 3;
        registerGoogleAccount(email);
        delay(10, 30, "Reg GooGle Account Done: ");
        if (ckbSetName) {
            int cbbTypeName = json.getInt(
                    "typeName");
            if (cbbTypeName == 0) {
                name = Common::ranNameViet();
            } else if (cbbTypeName == 1) {
                name = Common::ranNameNgoai();
            } else if (cbbTypeName == 2) {
                vector<string> lstFirstName = json.getListSplit("firstName");
                vector<string> lstMiddleName = json.getListSplit("middleName");
                vector<string> lstLastName = json.getListSplit("lastName");

                if (lstFirstName.empty() &&
                    lstMiddleName.empty() &&
                    lstLastName.empty()) {
                    name = Common::ranBool(50) == true ? Common::ranNameViet()
                                                       : Common::ranNameNgoai();
                }

                name = Common::ranItem(lstFirstName) + " " + Common::ranItem(lstMiddleName) +
                       " " +
                       Common::ranItem(lstLastName);
                name = Common::strReplace(name, "  ", " ");
            } else {
                vector<string> lstFullName = json.getListSplit("fullName");
                if (lstFullName.empty())
                    name = Common::ranBool(50) == true ? Common::ranNameViet()
                                                       : Common::ranNameNgoai();

                name = Common::ranItem(lstFullName);
            }
        }

        if (!openSocial(true)) {
            saveStatusAcc("Reg", "Open Package Fail");
            return output;
        }
        for (int loop = 300; loop > 0; loop--) {
            if (isStopped()) {
                output.set("error", "stop");
                break;
            }
            string xml = dumpXml();
            string screen = detectScreen(xml);
            if (screen == "TIK_PROFILE") {
                if (isRegDone) {
                    string username = Common::regexGroups1(xml, "@(.*?)\"");
                    if (username.empty())
                        continue;
                    //updateLine("Up Video");
                    //upImageOrVideo(30, 35);
                    updateLine("Interacting after reg acc");
                    JSON configs = JSON();
                    configs.set("nudSoLuongFrom", 1);//số lượng video from
                    configs.set("nudSoLuongTo", 2);//số lượng video to
                    configs.set("nudTimeFrom", 10);//thời gian xem video from
                    configs.set("nudTimeTo", 15);//thời gian xem video to
                    configs.set("ckbInteract", true);//có like không?
                    configs.set("ckbFollow", true);//có Follow không?
                    action.jsonConfig = configs;
                    actionTuongTacVideoRandom();
                    output.set("success", true);
                    output.set("username", username);
                    output.set("password", "");
                    output.set("email", email);
                    return output;

                }
                vector<Node> lstNodes = getNodes(
                        "//android.widget.FrameLayout/android.view.ViewGroup/android.widget.LinearLayout/child::*",
                        xml);
                if (lstNodes.size() > 0) {
                    for (int i = lstNodes.size() - 1; i >= 0; i--) {
                        Node node = lstNodes.at(i);
                        if (node.width > node.height) {
                            tap(node.left + node.width - 50, node.centerY);
                            delay(2);
                            break;
                        }
                    }
                    continue;
                }
            }
            if (screen == "TIK_LOGIN") {
                if (!tap("//*[@text='Don’t have an account? Sign up']", xml))
                    tap("//*[@text='Use phone / email / username' or @text='use phone/email/username']",
                        xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP") {
                //
                Node swipeFrom = getNode("//*[@text='Use phone or email']", xml, 0);
                Node swipeTo = getNode("//*[@text='Sign up for TikTok']", xml, 0);
                string xpathGG = "//*[@text='Continue with Google']";
                if (!existNode(xpathGG, xml, 0)) {
                    swipe(swipeFrom.centerX, swipeFrom.centerY, swipeTo.centerX,
                          swipeTo.centerY,
                          500);
                    xml = dumpXml();
                }
                tap(xpathGG, xml);
                delay(1, 2, "Login Google...");
                continue;
            }
            if (screen == "TIK_WAITING_GOOGLE") {
                delay(10, 15);
                continue;
            }
            if (screen == "TIK_EXIST_GOOGLE_MAIL") {
                string emailGoogleFtTiktok = "//*[@text='" + email + "@gmail.com']";
                if (existNode(emailGoogleFtTiktok, xml, 3)) {
                    tap(emailGoogleFtTiktok);
                    delay(1);
                    continue;
                }
                continue;
            }
            if (screen == "TIK_LOGIN_PHONE" || screen == "TIK_LOGIN_EMAIL") {
                maxAgain--;
                if (maxAgain < 0)
                    break;

                performAction(Perform::BACK, 2, 1);

                //tap("//*[@content-desc='Back to previous screen']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP_BIRTHDAY") {
                updateLine("set Birthday");
                string xpathDMY = "//*[@text='Next']/following-sibling::*[last()]/child::*/child::*";
                for (int i = 1; i <= 3; ++i) {
                    Node node = getNode(xpathDMY + "[" + to_string(i) + "]", xml);
                    int countScroll = Common::ranNumber(2, 5);
                    if (i == 3)
                        countScroll += 20;
                    for (int j = 0; j < countScroll; ++j) {
                        swipe(Common::ranNumber(node.left, node.right),
                              Common::ranNumber(node.top, node.top + 20),
                              Common::ranNumber(node.left, node.right),
                              Common::ranNumber(node.bottom, node.bottom - 20), 500);
                        delay(1);
                    }
                }
                delay(delayBirthdayFrom, delayBirthdayTo, "delay birthday");
                if (tap("//*[@text='Next']"))
                    delay(3);
                isRegDone = true;
                continue;
            }
            if (screen == "TIK_SIGN_UP_NICKNAME") {
                if (!ckbSetName) {
                    delay(10);
                    tap("//*[@content-desc='Skip']", xml);
                    delay(5, "delay...");
                    continue;
                }
                inputText("//android.widget.EditText", name);
                delay(5);
                tap("//*[@text='Confirm']", xml);
                delay(30, "delay confirm:..");
                continue;

            }
            if (screen == "TIK_LIVE") {
                performAction(Perform::BACK, 1, 1);
                continue;
            }
            if (screen == "TIK_NEW_CHAT") {
                performAction(Perform::BACK, 1, 1);
                continue;
            }

            if (screen == "TIK_INBOX") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
        }

        return JSON{};

    }

    bool registerGoogleAccount(string email) {

        string passMail = "0987793405@";

        vector<string> lstFirstName = {"Long", "Nam", "Hoang", "Duong", "Duc", "Toan", "Thu"};
        string firstName = Common::ranItem(lstFirstName);

        vector<string> lstLastName = {"Hoang", "Nguyen", "Bui", "Le"};
        string lastName = Common::ranItem(lstLastName);

        int birthdayAccountFrom = 1980;
        int birthdayAccountTo = 2004;
        int birthdayAccount = Common::ranNumber(birthdayAccountFrom, birthdayAccountTo);

        bool regDone = false;

        JSON json;
        json.set("action", "android.settings.SYNC_SETTINGS");
        if (!Device::startActivityByIntent(json)) {
            delay(5, "Can't open settings", true);
            return false;
        }
        searchAddAccount:
        string xml = dumpXml();
        if (!existNode("//*[@text='Add account']", xml, 3)) {
            swipe();
            delay(1);
            goto searchAddAccount;
        }
        tap("//*[@text='Add account']");

        searchGoogleAccount:
        xml = dumpXml();
        if (!existNode("//*[@text='Google']", xml, 3)) {
            swipe();
            delay(1);
            goto searchGoogleAccount;
        }
        tap("//*[@text='Google']");

        for (int i = 0; i < 100; ++i) {
            if (isStopped())
                return ERROR;
            xml = dumpXml();
            string screen = detectScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen == "TIK_REGMAIL_INPUTMAIL") {
                string xpathCreateAccount = "//*[@text='Create account']";
                tap(xpathCreateAccount);
                if (existNode(xpathCreateAccount, xml, 3)) {
                    Node nodeCreateAccount = getNode(xpathCreateAccount, xml, 0);
                    xml = dumpXml();
                    tap(nodeCreateAccount.centerX,
                        nodeCreateAccount.centerY + nodeCreateAccount.height);
                }
                delay(1);
                continue;
            }

            if (screen == "GOOGLE_RE_CAPTCHA") {
                delay(60, "Captcha Awards: ");
                continue;
            }

            if (screen == "GOOGLE_ENTER_YOUR_NAME") {
                inputText("(//android.widget.EditText[@class='android.widget.EditText'])[1]",
                          firstName);
                delay(3);
                inputText("(//android.widget.EditText[@class='android.widget.EditText'])[2]",
                          lastName);
                delay(2);
                tap("//*[@text='Next']");
                delay(5);
                continue;
            }

            if (screen == "GOOGLE_YOUR_BIRTHDAY") {
                tap("//*[@text='Month']");
                delay(1);
                int month = Common::ranNumber(1, 12);
                tap("(//*[@resource-id='android:id/text1'])[" + to_string(month) + "]");
                delay(1);

                int day = Common::ranNumber(1, 30);
                inputText("//android.widget.EditText[@resource-id='day']", to_string(day));
                delay(1);
                inputText("//android.widget.EditText[@resource-id='year']",
                          to_string(birthdayAccount));
                delay(1);
                // Giới tính
                tap("//*[@text='Gender']");
                delay(1);

                int gender = Common::ranNumber(1, 2);
                tap("(//*[@resource-id='android:id/text1'])[" + to_string(gender) + "]");
                delay(3);
                regDone = true;
                tap("//*[@text='Next']");
                delay(1);

                continue;
            }

            if (screen == "GOOGLE_USERNAME") {
                inputText("//android.widget.EditText[@class='android.widget.EditText']", email);
                delay(1);
                tap("//*[@text='Next']");
                delay(1);

                continue;
            }
            if (screen == "GOOGLE_PASSWORD") {
                inputText("//android.widget.EditText[@class='android.widget.EditText']", passMail);
                delay(1);
                tap("//*[@text='Next']");
                delay(1);
                continue;
            }
            if (screen == "GOOGLE_ADD_PHONE_NUMBER") {
                searchYesIm:
                xml = dumpXml();
                if (!existNode("//*[@text='Yes, I’m in']", xml, 3)) {
                    swipe();
                    delay(1);
                    goto searchYesIm;
                }
                tap("//*[@text='Yes, I’m in']");
                delay(1);
                continue;
            }

            if (screen == "GOOGLE_REVIEW_YOUR_ACCOUNT") {
                tap("//*[@text='Next']");
                delay(1);
                continue;
            }
            if (screen == "GOOGLE_PRIVACE_AND_TERMS") {
                searchIAgree:
                xml = dumpXml();
                if (!existNode("//*[@text='I agree']", xml, 3)) {
                    if (swipeAndScreenNotChange()) {
                        continue;
                    }
                    delay(1);
                    goto searchIAgree;
                }
                //tap("//*[@text='I agree']");
                delay(1);
                continue;
            }
            if (regDone) {
                if (existNode("//*[@text='Add account']", xml, 0)) {
                    updateLine("Reg Google Account Done");
                    return true;
                }
                continue;
            }
        }

        return true;
    }

    bool downloadAppCHPlay(string appNameDownload) {
        updateLine("Force Stop CH Play");
        closeAppAction("com.android.vending");
        updateLine("Open CH Play");
        openApp("com.android.vending");
        string xml = dumpXml();
        string xpathCHPlay = existNodes(3, xml, {
                "//*[@package='com.android.vending']",
        });
        if (!existNode(xpathCHPlay, xml, 3)) {
            updateLine("Open CH Play Fail");
        }
        updateLine("Open CH Play DONE");

        for (int i = 0; i < 100; ++i) {
            xml = dumpXml();
            if (isStopped())
                return ERROR;
            string screen = detectScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "GOOGLE_CHPLAY_HOME") {
                tap("//*[@text='Search']");
                delay(1);
                continue;
            }
            if (screen == "GOOGLE_CHPLAY_SEARCH") {
                tap("//*[@text='Search apps & games']");
                delay(2);
                inputText("//android.widget.EditText[@class='android.widget.EditText']",
                          appNameDownload);
                //handleKeyboard(Keyboard::INPUT, appNameDownload);
                delay(2);
                tap("//*[contains(@content-desc,\"'" + appNameDownload + "' \")]");
                delay(1);

                if (!existNode("//*[contains(@content-desc,'" + appNameDownload + "')]", xml, 3)) {
                    updateLine("Search App FAIL");
                    return NOMAP;
                }

                updateLine("Tap Install");

                tap("//*[contains(@content-desc,'" + appNameDownload +
                    "')]/following-sibling::*[1]/*[@content-desc='Install']");
                if (existNode("//*[@content-desc='Open']", xml, 5000)) {
                    updateLine("Download DONE");
                    return true;
                }
                updateLine("Download FAIL");
                return false;
                //continue;
            }

        }


        return true;
    }

    bool searchGoogle(string textSearch, string textHttp) {
        closeAppAction("com.android.chrome");
        openApp("com.android.chrome");
        delay(3);
        for (int i = 0; i < 50; ++i) {
            if (isStopped())
                return false;
            string xml = dumpXml();
            string xpath = existNodes(0, xml, {
                    "//*[@text='No thanks']",
                    "//*[@text='More']",
                    "//*[@text='Got it']",
                    "//android.widget.EditText",
                    "//*[@text='Google Search']",
                    //"//*[contains(@text,'"+textHttp+"')]"
            });

            if (xpath == "//android.widget.EditText" || xpath == "//*[@text='Google Search']") {
                tap(xpath);
                delay(1);
                xml = dumpXml();
                inputText("//android.widget.EditText", textSearch);
                delay(5);
                xml = dumpXml();
                if (xpath == "//*[@text='Google Search']") {
                    handleKeyboard(Keyboard::ENTER);
                } else
                    tap("//*[@text='" + textSearch + "'][1]");
                searchLink:
                if (isStopped())
                    return false;
                delay(5, "Wait load website Search: ");
                xml = dumpXml();
                if (existNode("//*[@text='More search results']", xml, 0)) {
                    tap("//*[@text='More search results']");
                    delay(3);
                    xml = dumpXml();
                }
                if (!existNode("//*[@text='" + textHttp + "']", xml, 0)) {
                    swipe();
                    goto searchLink;
                }
                if (!tapAndScreenNotChange("//*[(@text='" + textHttp + "')]", 3)) {
                    int totalTime = 60;
                    int maxScrolls = 10;
                    int typeScroll = 1;
                    auto start = std::chrono::high_resolution_clock::now();
                    int elapsedTime = 0;

                    while (elapsedTime < totalTime) {
                        updateLine("Viewed: " + to_string(elapsedTime) + " s");

                        int scrollTimes = rand() % maxScrolls + 1;
                        if (swipeAndScreenNotChange(typeScroll, scrollTimes)) {
                            typeScroll *= -1;
                        }
                        updateLine("Viewed: " + to_string(elapsedTime) + "  s");

                        int delayTime = rand() % 5 + 1;
                        delay(delayTime, "View Web");
                        updateLine("Viewed: " + to_string(elapsedTime) + "  s");

                        auto now = std::chrono::high_resolution_clock::now();
                        elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(
                                now - start).count();
                    }
                    updateLine("Search Success");
                    return true;
                }

                updateLine("Search Failed");
                return false;
            }


            if (xpath != "") {
                tap(xpath);
                delay(1, 2);
                continue;
            }

        }
        return true;
    }

    //SCRIPT GOOGLE
    bool tapAndScreenNotChange(string xpath, int delayTime) {
        string xml = dumpXml();
        tap(xpath);
        delay(delayTime);
        return xml == dumpXml();
    }

    ResultScreen actionEarnMoney() {
        if (!getJobSetting()) return NOMAP;

        auto [success, jsonSetup] = setupTDTT();

        if (!success) return NOMAP;


        if (jobSetting.json.getInt("mode") == 0) {
            if (jobSetting.isLike && jsonSetup.isContains("openFavorite")
                && !jsonSetup.getBool("openFavorite")) {
                actionPublicLike();
            }

            int ranNewfeed = Common::ranNumber(jobSetting.countNewFeedFrom,
                                               jobSetting.countNewFeedTo);
            int total = 0, countSuccess = 0, countFailLienTiep = 0, countJobForLuotNewfeed = 0;
            int countRetriveCoin = 2;

            string statusJob = "";


            while (!isStop) {
                updateStatusJob(statusJob, countSuccess, total);

                updateLine("Get job...");

                vector<Job> lstJobs = getTDTTJobs(statusJob);
                if (lstJobs.empty()) {
                    saveStatusAcc("TDTT", statusJob, 0,
                                  countSuccess > 0 ? Task::DONE : Task::FAIL);
                    delay(5, statusJob + "Tạm thời hết job cho UID này");
                    break;
                }

                if (getUsedMemoryPercentage() > 70)
                    openSocial(true);

                for (const Job &job: lstJobs) {
                    if (countSuccess >= jobSetting.limitJobSuccess ||
                        countFailLienTiep >= jobSetting.limitJobFail) {

                        if (countSuccess >= jobSetting.limitJobSuccess) {
                            saveStatusAcc("TDTT", statusJob, 0, Task::DONE);
                            account.updateInfo("isBlock", false);
                        }
                        if (countFailLienTiep >= jobSetting.limitJobFail) {
                            saveStatusAcc("Block",
                                          statusJob + "FAIL " +
                                          to_string(countFailLienTiep) +
                                          " count");
                            account.updateInfo("isBlock", true);
                        }

                        nhanCoin(job);
                        return NOMAP;
                    }
                    total++;

                    if (total % 5 == 0 && !isAuthDevice()) return NOMAP;

                    updateStatusJob(statusJob, countSuccess, total);

                    updateLine(statusJob);

                    Task taskResult = Task::NONE;
                    if (job.field == "tiktok_like") {
                        taskResult = jobLike(job, statusJob);
                    }
                    if (job.field == "tiktok_follow")
                        taskResult = followUser(job.link, statusJob,
                                                jobSetting.isOpenLinkBySearch);
                    if (job.field == "tiktok_comment1")
                        taskResult = jobComment(job, statusJob);

                    bool isRetriveCoin = job.field == "tiktok_comment1";


                    if (!isRetriveCoin) {
                        JSON jsonJobDaDuyet = duyetJob(job, taskResult == Task::DONE);
                        if (jsonJobDaDuyet.getBool("success")) {
                            isRetriveCoin =
                                    jsonJobDaDuyet.getInt("cache") >= countRetriveCoin;
                        }
                    }

                    if (isRetriveCoin) {
                        int countJobSuccess = nhanCoin(job);
                        if (countJobSuccess < 1)
                            countFailLienTiep += countRetriveCoin;
                        else {
                            countSuccess += countJobSuccess;
                            if (countSuccess > total) countSuccess = total;
                            countFailLienTiep = 0;
                        }
                    }

                    if (jobSetting.isNewfeedAfterJobSuccess) {
                        countJobForLuotNewfeed++;

                        if (countJobForLuotNewfeed >= ranNewfeed) {
                            countJobForLuotNewfeed = 0;
                            ranNewfeed = Common::ranNumber(jobSetting.countNewFeedFrom,
                                                           jobSetting.countNewFeedTo);

                            JSON newfeedConfig{};
                            newfeedConfig.set("nudSoLuongFrom", 2);
                            newfeedConfig.set("nudSoLuongTo", 3);
                            newfeedConfig.set("nudTimeFrom", 5);
                            newfeedConfig.set("nudTimeTo", 10);
                            newfeedConfig.set("ckbInteract", true);
                            newfeedConfig.set("nudPercentInteract",
                                              Common::ranNumber(20, 30));

                            action.jsonConfig = newfeedConfig;

                            actionTuongTacVideoJob();
                            continue;
                        }
                    }

                    updateStatusJob(statusJob, countSuccess, total);


                    delay(Common::ranNumber(jobSetting.delayJobFrom, jobSetting.delayJobTo),
                          statusJob);
                }
            }
        } else {
            JSON newfeedConfig{};
            newfeedConfig.set("nudThoiGianFrom", jobSetting.json.getInt("nudThoiGianFrom"));
            newfeedConfig.set("nudThoiGianTo", jobSetting.json.getInt("nudThoiGianTo"));
            newfeedConfig.set("ckbInteract", jobSetting.json.getBool("isInteractNewfeed"));
            newfeedConfig.set("nudPercentInteract", jobSetting.json.getInt("nudPercentInteract"));
            newfeedConfig.set("nudTimeFrom", 1);
            newfeedConfig.set("nudTimeTo", 10);
            newfeedConfig.set("cbbOptionsPost", 1);
            action.jsonConfig = newfeedConfig;
            actionTuongTacVideoRandom();
        }

        return NOMAP;
    }

    JSON duyetJob(Job job, bool isSuccess) {
        JSON jsonBody{};
        jsonBody.set("jobId", job.id);
        jsonBody.set("field", job.field);
        jsonBody.set("uidId", account.uid);
        jsonBody.set("isSuccess", isSuccess);
        jsonBody.set("isJobDie", false);

        string note = isSuccess ? "da lam xong" : "chua lam";
        jsonBody.set("note", "job live, " + note);


        string response = RequestTDTT("reports", jsonBody.toString());
        JSON jsonJobDaDuyet = JSON(response);

        return jsonJobDaDuyet;
    }

    int nhanCoin(Job job) {
        JSON jsonBody{};
        jsonBody.set("field", job.field);
        jsonBody.set("uidId", account.uid);
        if (job.field == "tiktok_comment1")
            jsonBody.set("jobId", job.id);

        string responseGetCoin = RequestTDTT("get-coins", jsonBody.toString());

        JSON jsonNhanXu(responseGetCoin);

        if (job.field == "tiktok_comment1") {
            bool isSuccess = jsonNhanXu.getBool("success");
            if (isSuccess)
                return 1;
            return 0;
        }

        string dataRes = jsonNhanXu.getString("data");

        if (dataRes.empty()) return 0;

        vector<string> dataSplit = Common::strSplit(dataRes, "/");

        if (dataSplit.empty()) return 0;

        string jobSucessStr = dataSplit[0];
        int jobSuccess = std::stoi(jobSucessStr);

        account.coinReceived += jsonNhanXu.getInt("coin_received");

        return jobSuccess;
    }

    ResultScreen actionDocThongBao() {
        bool doneNotify = false;

        bool isFollow = action.getBool("ckbFollow");
        int nudCountFollowFrom = action.getInt("nudCountFollowFrom");
        int nudCountFollowTo = action.getInt("nudCountFollowTo");
        int totalFollow = Common::ranNumber(nudCountFollowFrom, nudCountFollowTo);
        int countFollow = 0;
        bool doneFollow = false;

        bool isSayHi = action.getBool("ckbSayHi");
        int nudCountSayHiFrom = action.getInt("nudCountSayHiFrom");
        int nudCountSayHiTo = action.getInt("nudCountSayHiTo");
        int totalSayHi = Common::ranNumber(nudCountSayHiFrom, nudCountSayHiTo);
        int countSayHi = 0;

        int delayFrom = 3;
        int delayTo = 5;
        //----------------------------------------------------------

        //bộ đếm số thông báo
        string xpathFollow = "";
        string xml = dumpXml();
        gotoHome(xml);

        for (int loop = 300;
             loop > 0; loop--) {
            updateLine("Read notify...");
            if (isStopped()) {
                return ERROR;
            }
            delay(1);
            xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "TIK_HOME") {
                tap("//*[@content-desc='Inbox']");
                continue;
            }

            if (screen == "TIK_PROFILE") {
                tap("//*[@content-desc='Inbox']");
                continue;
            }
            if (screen == "TIK_INBOX") {
                if (!doneNotify) {
                    string xpathNotify = existNodes(0, xml, {
                            "//*[@text='Activities']",
                            "//*[@text='Activity']"
                    });
                    if (!existNode(xpathNotify, xml, 5))
                        swipeAndScreenNotChange();
                    tap(xpathNotify, xml);
                    delay(1);
                    continue;
                }

                if (isSayHi) {
                    if (!existNode(
                            "//*[@resource-id='com.zhiliaoapp.musically:id/crv']//android.widget.TextView",
                            xml, 0)) {
                        swipe();
                        delay(1);
                        xml = dumpXml();
                        xpathFollow = existNodes(0, xml, {
                                "//*[@text='Follow']",
                                "//*[@text='Follow back']"
                        });
                        if (xpathFollow != "")
                            return NOMAP;
                        continue;
                    }
                    if (tap(
                            "//*[@resource-id='com.zhiliaoapp.musically:id/crv']//android.widget.TextView"))
                        countSayHi++;
                    if (countSayHi >= totalSayHi) {
                        isSayHi = false;
                        continue;
                    }
                    delay(delayFrom, delayTo);
                    continue;
                };
                return NOMAP;
            }
            if (screen == "TIK_TOK_NOTIFY") {
                if (existNode("//*[@text='Notifications about your account will appear here']", xml,
                              3)) {
                    updateLine("No Notify");
                    return NOMAP;
                }
                if (!doneNotify) {
                    if (existNode("//*[@text='View all']", xml, 3)) {
                        tap("//*[@text='View all']");
                        delay(3, "Checking Notify");
                        xml = dumpXml();
                    }
                    doneNotify = true;
                }

                xpathFollow = existNodes(0, xml, {
                        "//*[@text='Follow']",
                        "//*[@text='Follow back']"
                });

                if (isFollow) {
                    if (xpathFollow != "") {
                        tap(xpathFollow);
                        countFollow++;
                        delay(delayFrom, delayTo);
                        if (countFollow == totalFollow)
                            performAction(Perform::BACK, 1, 1);
                        continue;
                    }
                } else {
                    if (xpathFollow != "") {
                        performAction(Perform::BACK, 1, 1);
                        continue;
                    }
                }
                if (swipeAndScreenNotChange()) {
                    performAction(Perform::BACK, 1, 1);
                    continue;
                }
                continue;
            }
        }
        return NOMAP;
    }

    ResultScreen viewProductLiveStream() {
        //
        bool ckbAddToCart = true;
        int delayTimeViewFrom = 1;
        int delayTimeViewTo = 1;
        //
        for (int loop = 300; loop > 0; loop--) {
            if (isStopped()) {
                return ERROR;
            }

            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            //*[@text='Buy']
            if (screen == "TIK_LIVE") {
                delay(3, 5, "View Live...");
                if (!existNode("//*[@text='Buy']/parent::*", xml, 2)) {
                    tap("//*[@text='Shop']/parent::*");
                }
                tap("//*[@text='Buy']/parent::*");
                delay(1, 2);
                continue;
            }

            if (screen == "TIK_ADD_TO_CART") {
                performAction(Perform::BACK, 1, 1);
                continue;
            }
            if (screen == "TIK_VIEW_TIKTOKSHOP") {
                viewProductTiktokShop();
                return DONE;
                //continue;
            }
            if (screen == "TIK_SHOP_LIVE_ALL_PRODUCT") {
                delay(5, "viewproduct");
                performAction(Perform::BACK, 1, 1);
                return DONE;
            }

        }
        return DONE;
    }

    ResultScreen actionFollowUserByProfile() {
        //input
        int nudSoLuongFollowerFrom = action.getInt("nudFollowerSoLuongFrom");
        int nudSoLuongFollowerTo = action.getInt("nudFollowerSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> linkUserLst = action.getList("lstUserLink");
        int nudSoLuongLinkFrom = action.getInt("nudSoLuongLinkFrom");
        int nudSoLuongLinkTo = action.getInt("nudSoLuongLinkTo");

        //code here
        int countSuccess = 0;
        int totalFollower = Common::ranNumber(nudSoLuongFollowerFrom, nudSoLuongFollowerTo);

        int countLink = 0;
        int totalLink = Common::ranNumber(nudSoLuongLinkFrom, nudSoLuongLinkTo);


        string status =
                "Follow (" + to_string(countSuccess + 1) + "/" + to_string(totalFollower) + ") ";

        for (int i = 0; i < 50; ++i) {
            if (isStopped())
                return ERROR;
            if (linkUserLst.empty())
                return NOMAP;
            string linkUser = Common::ranItem(linkUserLst);
            gotoProfile(linkUser);
            Common::removeItem(linkUserLst, linkUser);
            updateLine("User: " + to_string(countLink) + "/" + to_string(totalLink));
            countSuccess = 0;
            while (!isStopped()) {
                updateLine(status + "Checking");

                string xml = dumpXml();
                string screen = handleScreen(xml);
                if (screen == "CONTINUE") continue;
                if (screen == "ERROR") return ERROR;

                if (screen == "TIK_ANOTHER_PROFILE") {
                    string xpathFollower = existNodes(0, xml, {
                            "//*[@text='Followers']",
                            "//*[@text='Follower']"
                    });

                    tap(xpathFollower, xml);
                    delay(2);
                    continue;
                }
                if (screen == "TIK_PROFILE_FOLLOWERS") {
                    string xpathFollow = existNodes(0, xml, {
                            "//*[@text='Follow']",
                            "//*[@text='Follow back']"
                    });
                    if (!tap(xpathFollow, xml)) {
                        updateLine(status + "Swipe");
                        if (swipeAndScreenNotChange())
                            break;
                        continue;
                    }

                    delay(nudDelayFrom, nudDelayTo, status + "delay");

                    countSuccess++;
                    if (countSuccess >= totalFollower) {
                        countLink++;
                        break;
                    }
                    status = "Follow (" + to_string(countSuccess + 1) + "/" +
                             to_string(totalFollower) + ") ";
                    continue;
                }
                if (screen == "TIK_ANOTHER_PROFILE") {
                    performAction(Perform::BACK, 1, 1);
                    delay(2);
                    continue;
                }

                updateLine(status + "Swipe");
                if (swipeAndScreenNotChange())
                    break;
                delay();
            }
            if (countLink >= totalLink)
                return NOMAP;
            delay(3, "Delay Open Profile: " + status);
        }
        return NOMAP;
    }

    ResultScreen actionAddProducTikTokShop() {
        //input
        string countProduct;

        vector<string> linkLst = {
                "https://vt.tiktok.com/ZS2eAXSqL/",
                "https://vt.tiktok.com/ZS2eAbA8Y/",
                "https://vt.tiktok.com/ZS2eAUfH6/",
                "https://vt.tiktok.com/ZS2eAuSrs/",
                "https://vt.tiktok.com/ZS2eAXejV/",
                "https://vt.tiktok.com/ZS2eAVPy4/",
                "https://vt.tiktok.com/ZS2eAyhFb/",
        };
        int nudDelayFrom = 3;
        int nudDelayTo = 5;


        string url;
        int countAdded = 0;
        int totalAdd = linkLst.size();
        //code here
        for (int i = 0; i < 40; ++i) {
            if (isStopped() || linkLst.empty())
                return NOMAP;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            updateLine(
                    "Added success : " + to_string(countAdded) + "/" + to_string(totalAdd));
            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']");
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                tap(
                        "//*[@resource-id=\"com.zhiliaoapp.musically:id/may\"]/android.widget.LinearLayout[1]/android.widget.LinearLayout[2]");
                delay(1, 2);
                xml = dumpXml();
                tap("//*[@content-desc='TikTok Shop Creator Center']");
                delay(1, 2);
                continue;

            }
            if (screen == "TIK_TIKTOK_SHOP") {

                swipe();
                delay(3);

                xml = dumpXml();
                if (!existNode("//*[@content-desc='Product Marketplace']", xml, 5))
                    swipe();
                delay(3);

                tap("//*[@content-desc='Product Marketplace']");
                delay(1, 2);

                continue;
            }

            if (screen == "TIK_PRODUCT_MARKET") {

                Node xpathSearch = getNode("//*[@content-desc='Search']", xml, 3);

                int xAdd = Common::ranNumber(xpathSearch.width, xpathSearch.right);

                int yAdd = Common::ranNumber(
                        xpathSearch.top - (xpathSearch.bottom - xpathSearch.top),
                        (xpathSearch.top - (xpathSearch.bottom - xpathSearch.top)) +
                        10);

                tap(xAdd, yAdd);
                delay(2);
                xml = dumpXml();
                tap(
                        "(//*[@class='android.view.ViewGroup']/*[@class='android.view.ViewGroup']/*[@class='android.view.ViewGroup'])[last()]",
                        xml);
                delay(1, 2);
                continue;

            }

            if (screen == "TIK_ADD_PRODUCT_LINK") {
                url = Common::ranItem(linkLst);

                tap("//*[@class='com.bytedance.ies.xelement.input.LynxInputView']");
                delay(1);
                //handleKeyboard(1, url);
                delay(2);
                xml = dumpXml();
                Common::removeItem(linkLst, url);

                if (existNode(
                        "//*[@resource-id=\"android:id/content\"]/android.view.ViewGroup[1]/android.view.ViewGroup[1]/android.view.ViewGroup[1]/android.view.ViewGroup[1]",
                        xml, 3))
                    tap(
                            "//*[@resource-id=\"android:id/content\"]/android.view.ViewGroup[1]/android.view.ViewGroup[1]/android.view.ViewGroup[1]/android.view.ViewGroup[1]");
                delay(1);

                continue;

            }
            if (screen == "TIK_ADD_TO_SHOWCASE") {

                tap("//*[@content-desc='Add to showcase']");
                delay(1, 2);
                continue;

            }
            if (screen == "TIK_ADDED_TO_SHOWCASE") {
                delay(nudDelayFrom, nudDelayTo, "Delay Add:");
                countAdded++;
                updateLine(
                        "Added success : " + to_string(countAdded) + "/" + to_string(totalAdd));
                performAction(Perform::BACK, 1, 1);
                if (linkLst.empty()) {
                    performAction(Perform::BACK, 2, Common::ranNumber(2, 4));
                    updateLine("Add All Product");
                    return NOMAP;
                }
                continue;

            }
        }
        return NOMAP;
    }

    ResultScreen actionPublicLike() {
        string xml = dumpXml();
        gotoHome(xml);
        for (int loop = 300; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                updateLine("tap Setting");
                tap("//*[@content-desc='Profile menu']");
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_PROFILE_MENU") {
                updateLine("tap Setting and privacy");
                tap("//*[@text='Settings and privacy' or @text='settings and privacy']", dumpXml());
                delay(2, 3);
                continue;
            }
            if (screen == "TIK_SETTING_PRIVACY") {
                updateLine("tap Privacy");
                tap("//*[@text='Privacy']");
                delay(1);
                continue;
            }
            if (screen == "TIK_SETTING_LIKED_VIDEO") {
                checkLikedVideos:
                if (!existNode("//*[@text='Liked videos']", xml, 1)) {
                    swipe();
                    goto checkLikedVideos;
                }
                delay(1);
                tap("//*[@text='Liked videos']");
                delay(1);
                continue;
            }

            if (screen == "TIK_OPEN_LIKED_VIDEO") {
                if (existNode(
                        "//*[@text='Everyone']/parent::*/following-sibling::*/*[@class='android.widget.RadioButton' and @checked='false']",
                        xml, 3)) {
                    tap("//*[@text='Everyone']");
                    delay(1, 2);
                    updateLine("Public Liked Done");
                } else
                    updateLine("Public Liked Already");
                delay(3, "Goto Home:..");
                performAction(Perform::BACK, 3, 1);
                return DONE;
            }

        }
        return NOMAP;
    }

    ResultScreen actionPublicMessages() {
        for (int loop = 300; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                updateLine("tap Setting");
                tap("//*[@content-desc='Profile menu']");
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_PROFILE_MENU") {
                updateLine("tap Setting and privacy");
                tap("//*[@text='Settings and privacy']", dumpXml());
                delay(2, 3);
                continue;
            }
            if (screen == "TIK_SETTING_PRIVACY") {
                updateLine("tap Privacy");
                tap("//*[@text='Privacy']");
                delay(1);
                continue;
            }
            if (screen == "TIK_SETTING_LIKED_VIDEO") {
                checkDirectMessages:
                if (!existNode("//*[@text='Direct messages']", xml, 1)) {
                    swipe();
                    goto checkDirectMessages;
                }
                delay(1);
                tap("//*[@text='Direct messages']");
                delay(1);
                continue;
            }

            if (screen == "TIK_PUBLIC_MESSAGES") {
                tap("(//*[@text='Direct messages'])[2]/parent::*");
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_PUBLIC_MESSAGES_RADIO") {
                if (existNode(
                        "//*[@text='Everyone']/parent::*/following-sibling::*/*[@checked='true']",
                        xml, 2)) {
                    updateLine("Public Already");
                    performAction(Perform::BACK, 4, 1);
                    return NOMAP;
                }
                tap("//*[@text='Everyone']/parent::*");
                delay(3);
                updateLine("Public DONE");
                performAction(Perform::BACK, 4, 1);
                return DONE;
            }

        }
        return NOMAP;
    }

    ResultScreen switchToBusinessAccount(
            bool ckbSkipEmail,
            bool ckbSkipBio,
            bool maybeLater = false,
            string email = "",
            string bioContent = ""
    ) {
        for (int loop = 50; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            //DONE
            // thêm 1 boolean true false cho phép chọn là maylater hoặc up vidoe now

            string xml = dumpXml();
            string screen = handleScreen(xml);

            if (screen == "TIK_HOME") {
                updateLine("TIK_HOME");
                tap("//*[@text='Profile']");
                delay(1, 2);
                continue;
            }

            if (screen == "TIK_PROFILE") {
                updateLine("TIK_PROFILE");
                if (!existNode("//*[@content-desc='Profile menu']", xml, 3)) {
                    return ERROR;
                }
                tap("//*[@content-desc='Profile menu']");
                delay(1, 2);
                continue;
            }

            if (screen == "TIK_PROFILE_MENU") {
                updateLine("TIK_PROFILE_MENU");
                tap("//*[@text='Settings and privacy']");
                delay(1, 2);
                continue;
            }

            if (screen == "TIK_SWITCH_BUSSINESS") {
                performAction(Perform::BACK, 1, 1);
                delay(1, "Switch Success");
                return DONE;
            }

            string xpath = existNodes(0, xml, {
                    "//*[@content-desc='Account']",
                    "//*[@content-desc='Switch to Personal Account']",
                    "//*[@content-desc='Switch to Business Account']",
                    "//*[@text='Next' and @enabled='true']",
                    "//android.widget.Image",
                    "//*[@text='Maybe later']",
            });

            if (screen == "TIK_ADD_CONTACT_MAIL") {
                if (ckbSkipEmail)
                    tap("//*[@text='Skip']");
                else {
                    inputText("//android.widget.EditText", email);
                    delay(1);
                    tap("//*[@text='Add']");
                }
                delay(1);
                continue;
            }
            if (screen == "TIK_ADD_BIO") {
                if (ckbSkipBio)
                    tap("//*[@text='Skip']");
                else {
                    inputText("//android.widget.EditText", bioContent);
                    delay(1);
                    tap("//*[@text='Confirm']");
                }
                delay(1);
            }

            if (xpath == "//*[@content-desc='Account']" ||
                xpath == "//*[@content-desc='Switch to Business Account']" ||
                xpath == "//*[@text='Next' and @enabled='true']") {
                tap(xpath);
                delay(1, 2);
                continue;
            }

            if (xpath == "//*[@content-desc='Switch to Personal Account']") {
                return DONE;
            }

            if (xpath == "//android.widget.Image") {
                int typeCategory = Common::ranNumber(1, 6);
                string choose = "(" + xpath + ")[" + to_string(typeCategory) + "]";
                tap(choose);
                delay(1, 2);
                continue;
            }

            if (xpath == "//*[@text='Maybe later']") {
                if (maybeLater) {
                    tap(xpath);
                    delay(1, 2);
                    return DONE;
                }
                tap("//*[@text='Create now']");
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_UPLOAD_PHOTOS") {
                actionUpPost();
                continue;
            }
            if (screen == "TIK_POST_VIDEO") {
                updateLine("Post Video");
                if (!existNode("//*[@text='Post video']", xml, 3)) {
                    return ERROR;
                }
                tap("//*[@text='Post video']");
                delay(1, 2);
                return DONE;
            }
        }
        return NOMAP;
    }

    ResultScreen actionBuffTopComment() {
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        int delayFrom = action.getInt("nudDelayFrom");
        int delayTo = action.getInt("nudDelayFrom");

        int totalComment = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);

        bool ckbLike = action.getBool("ckbLike");
        bool ckbComment = action.getBool("ckbComment");
        vector<string> lstComment = action.getList("lstComment");
        vector<string> lstLink = action.getList("lstLink");

        // input
        updateLine("Open Comment");

        //
        string linkComment = Common::ranItem(lstLink);
        gotoLink(linkComment);
        string xml = dumpXml();
        if (!existNode("//android.widget.EditText", xml, 5))
            return ERROR;
        //
        for (int loop = 300; loop > 0; loop--) {
            if (isStopped()) {
                return ERROR;
            }

            xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen == "TIK_BUFF_TOP_COMMENT") {

                if (ckbLike) {
                    tap("(//*[@content-desc='Like or undo like'])[1]");
                    delay(1);
                }

                tap("(//*[@text='Reply'])[1]");

                delay(2);

                string comment = Common::ranItem(lstComment);
                inputText("//android.widget.EditText", comment);
                Common::removeItem(lstComment, comment);

                delay(3, "Tap send");

                tap("//*[@content-desc='Post comment']");
                delay(1);
                return DONE;
            }

        }
        return NOMAP;
    }

    ResultScreen actionViewProductTiktokShopByLink() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        // thời gian xem sản phẩm

        int delayTimeViewFrom = action.getInt("nudDelayFrom");
        int delayTimeViewTo = action.getInt("nudDelayFrom");

        int totalProduct = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);

        vector<string> lstLinkTiktokProduct = action.getList("lstLink");


        //code
        for (int i = 0; i < 50; ++i) {
            if (isStopped())
                return ERROR;
            if (lstLinkTiktokProduct.empty())
                return DONE;
            string linkTiktokProduct = Common::ranItem(lstLinkTiktokProduct);
            updateLine("Close App Tiktok");

            closeSocial();


            updateLine("Goto View");
            gotoLink(linkTiktokProduct);
            delay(3, "Open TikTok Shop");
            string xml = dumpXml();
            if (!existNode("//*[@text='Add to cart']", xml, 10)) {
                updateLine("Link Error");
                return NOMAP;
            }
            viewProductTiktokShop(delayTimeViewFrom, delayTimeViewTo);
            Common::removeItem(lstLinkTiktokProduct, linkTiktokProduct);
            delay(3, 4, "delay View");
        }
        return NOMAP;
    }

    ResultScreen
    viewProductTiktokShop(int delayTimeViewTo = 3, int delayTimeViewFrom = 5) {
        //input
        for (int loop = 50; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen == "TIK_VIEW_TIKTOKSHOP") {
                delay(3 + delayTimeViewFrom, 5 + delayTimeViewTo, "Viewing Product");
                if (swipeAndScreenNotChange(1, 2))
                    return NOMAP;
                delay(2, 2);
                continue;

            }
            if (screen == "TIK_PRODUCT_VIEWED") {
                updateLine("View Done");
                performAction(Perform::BACK, 1, 1);
                return DONE;
            }

            //if (screen == "TESST_ADDDED_TO_CART") {
            //    performAction(Perform::BACK,1,1)
            //    delay(1, 2);
            //    continue;
            //}
            //if (screen == "TIK_ADD_TO_CART") {
            //    string xpath = existNodes(0, xml, {
            //            "(//*[contains(@class, 'android.widget.Button') and not(@text='Confirm')])[1]",
            //            "(//*[@class='android.widget.ScrollView']//*[@class='android.widget.TextView'])[2]",
            //            "(//*[@class='android.view.ViewGroup'])[5]/following-sibling::*[1]",
            //    });
            //    if (xpath != "") {
            //        tap(xpath);
            //        delay(1, 2);
            //    }
            //    tap("//*[@text='Confirm']");
            //    delay(1, 2);
            //
            //    xml = dumpXml();
            //    screen = handleScreen(xml);
            //    if (screen == "TIK_VIEW_TIKTOKSHOP")
            //        return DONE;
            //    continue;
            //}
        }
        return NOMAP;
    }

    ResultScreen addLinkProductTikTokShop(string url) {
        for (int i = 0; i < 40; ++i) {
            if (isStopped())
                return ERROR;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "TIK_PROFILE") {
                //tap()
            }

        }
        return NOMAP;
    }

    ResultScreen changeMail(string mail) {
        string oldOtp = "";
        for (int i = 0; i < 40; ++i) {
            if (isStopped())
                return ERROR;
            string xml = dumpXml();
            string screen = handleScreen(xml);

            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                tap("//*[@content-desc='Profile menu']");
                delay(1);
                continue;
            }
            if (screen == "TIK_PROFILE_MENU") { ;
                tap("//*[@text='Settings and privacy']");
                delay(1);
                continue;
            }

            string xpath = existNodes(0, xml, {
                    "(//*[@text='Account'])[2]",
                    "//*[@content-desc='Account information']",
                    "//*[@text='Email']",
                    "//*[@text='Change email']",
                    "//*[@text='Send code']",
                    "//*[@text='Enter 6-digit code']",
                    "//*[@text='Get updates']"
            });
            if (screen == "TIK_REG_ADDMAIL") {
                updateLine("MAIL_CHANGE");
                inputText("//android.widget.EditText[@text='Email address']", mail);
                delay(1);
                performAction(Perform::BACK, 1);
                tap("//*[@text='Send code']", dumpXml(), 5);
                delay(1);
                continue;
            }


            if (xpath == "//*[@text='Change email']") {
                updateLine("tap Change mail");
                tap(xpath);
                delay(2, 4);
                if (!existNode("//android.widget.EditText[@text='Email address']", xml,
                               3)) {
                    performAction(Perform::BACK, 1, 1);
                }
                inputText("//android.widget.EditText[@text='Email address']", mail);
                performAction(Perform::BACK, 1, 3);
                tap("//*[@text='Send code']", dumpXml(), 5);
                delay(2, 3);
                continue;
            }
            if (xpath == "//*[@text='Enter 6-digit code']" ||
                screen == "TIK_SIGN_UP_VERIFY_EMAIL") {
                updateLine("input OTP");
                string otp = getOtpFromEmail(mail, 60, oldOtp);
                if (otp.empty() || otp == oldOtp) {
                    //tap resend
                    tap("//*[@text='Resend code']");
                    delay(2);
                    continue;
                }

                oldOtp = otp;
                updateLine("Input OTP");
                inputText("//android.widget.EditText", otp);
                delay(1, 2);
                return DONE;

            }

            if (xpath != "") {
                tap(xpath);
                delay(2, 3);
                continue;
            }
        }
        return NOMAP;

    }

    bool closeSetting() {
        openAppInfoAction("com.android.settings");
        for (int loop = 50; loop > 0; loop--) {
            if (isStopped())
                return ERROR;

            string xml = dumpXml();

            if (!existNode("//*[@package='com.android.settings']", xml, 0))
                break;
            performAction(Perform::BACK, 1, 1);
        }
        return false;
    }

    bool removeGoogleAccount() {
        JSON json;
        json.set("action", "android.settings.SYNC_SETTINGS");
        if (!Device::startActivityByIntent(json)) {
            delay(5, "Can't open settings", true);
            return false;
        }
        removeGG:
        string xml = dumpXml();
        if (!existNode("//*[contains(@text,'Accounts')]", xml, 3)) {
            if (isStopped())
                return ERROR;
            performAction(Perform::BACK, 1, 2);
            goto removeGG;
        }

        //if (!existNode(xpathAccManager, xml, 0)) {
        //    closeSetting();
        //    startActivity("android.settings.SYNC_SETTINGS");
        //}

        for (int loop = 50; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            xml = dumpXml();
            string screen = detectScreen(xml);
            if (Common::strContains(screen, "_POPUP_")) continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "TIK_REMOVE_GOOGLE") {
                if (!existNode("(//*[@text='Google'])[2]", xml, 3)) {
                    tap("//*[@text='Google']", xml);
                    delay(1, 2);
                    continue;
                }
                tap("(//*[@text='Google'])[2]");
                delay(1);
                continue;
            }
            if (screen == "TIK_REMOVE_GOOGLE_1") {
                string xpathRemove = "//*[@text='REMOVE ACCOUNT']";
                tap(xpathRemove, xml);
                delay(2);
                xml = dumpXml();
                if (existNode(xpathRemove, xml, 3)) {
                    tap(xpathRemove);
                    continue;
                }
                updateLine("Remove Success");
                continue;
            }

            if (screen == "TIK_REMOVE_GOOGLE_NOACC") {
                return true;
            }

            if (screen == "TIK_REMOVE_GOOGLE_2") {
                tap("//*[@text='REMOVE ACCOUNT']", xml);

                delay(1, 2);
                return true;
            }
        }
        return NOMAP;
    }

    ResultScreen Open2FaByPassword() {
        Node twoFa;
        for (int loop = 300; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            string xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "TIK_HOME") {
                updateLine("TIK_HOME");
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                updateLine("TIK_PROFILE");
                tap("//*[@content-desc='Profile menu']");
                delay(1);
                continue;
            }
            if (screen == "TIK_PROFILE_MENU") {
                updateLine("TIK_PROFILE_MENU");
                tap("//*[@text='Settings and privacy']");
                delay(1);
                continue;
            }
            if (screen == "TIK_SETTING_PRIVACY") {
                tap("//*[@text='Security']");
                delay(1);
                continue;
            }
            if (screen == "TIK_2FA") {
                tap("//*[@text='2-step verification']");
                delay(1);
                continue;
            }
            if (screen == "TIK_2FA_VERI") {

                string xpathBox = existNodes(0, xml, {
                        "//*[@content-desc='Phone, You’ll receive a verification code via SMS.' and @checked ='true']",
                        "//*[contains(@content-desc,'Email, You’ll receive a verification code at ') and @checked ='true']",
                        "//*[@content-desc='Authenticator, Install an app to generate your verification code.' and @checked='false']",
                });

                if (xpathBox != "") {
                    tap(xpathBox);
                    delay(1);
                    continue;
                }
                tap("//*[@text='Turn on']");
                delay(1);
                continue;
            }

            if (screen == "TIK_2FA_VERI_PASS") {
                inputText("//android.widget.EditText[@text='Password']",
                          account.password);
                delay(1);
                performAction(Perform::BACK, 1, 1);
                tap("//*[@text='Next']");
                continue;
            }
            if (screen == "TIK_2FA_COPY") {
                twoFa = getNode("//*[@text='Copy key']/preceding-sibling::*[1]", xml,
                                0);
                if (!twoFa.empty())
                    account.twoFA = twoFa.text;
                tap("//*[@text='Next']");
                continue;
            }
            if (screen == "TIK_INPUT_2FA") {
                string otpTwoFa = API::get2FA(twoFa.text);
                inputText("//android.widget.EditText", otpTwoFa);
            }
        }
        return NOMAP;
    }

    ResultScreen actionUpPost() {
        if (!Device::isKeyboardEnabled()) {
            saveStatusAcc("ERROR", "Please enable MaxCloud Keyboard to run this action", 10,
                          Task::FAIL);
            return ResultScreen::NOMAP;
        }
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");
        bool ckbScrollImage = action.getBool("ckbAnhCuon");
        int nudAnhCuon = action.getInt("nudAnhCuon");
        string galleryId = action.getString("galleryId");
        string galleryName = action.getString("galleryName");
        if (!ckbScrollImage) {
            nudAnhCuon = 1;
        }
        vector<MediaS3> lstMedia = getScriptMedia(galleryId, nudAnhCuon);
        if (lstMedia.empty())
            return ERROR;

        bool isSuccess = false;
        int nudTimeOutFrom = action.getInt("nudDelayFrom");
        int nudTimeOutTo = action.getInt("nudDelayFrom");


        bool ckbAddTrendSound = action.getBool("ckbAddTrendSound");
        bool isSoundAdjusted = false;

        string imgContentId = action.getString("imgContentId");

        string postContentId = action.getString("postContentId");

        bool ckbAutoCut = action.getBool("ckbAutoCut");
        int timeCreateVideo = 5;

        bool ckbAddFilter = action.getBool("ckbAddFilter");
        vector<string> txtFilterName = action.getList("txtFilterName");
        string filterName = Common::ranItem(txtFilterName);
        int cbbRandomFilter = action.getInt("cbbRandomFilter");
        bool isFilterFound = false;

        bool ckbAddLinkProduct = action.getBool("ckbLink");
        vector<string> lstLinkProduct = action.getList("lstLink");
        string linkProduct = Common::ranItem(lstLinkProduct);
        string productName;
        string displayName;
        size_t pos = linkProduct.find('|');

        if (pos != std::string::npos) {
            productName = linkProduct.substr(0, pos);
            displayName = linkProduct.substr(pos + 1);
        }

        bool ckbPostFilename = action.getBool("ckbPostFilename");
        if (ckbPostFilename) {
            string fileName = lstMedia[0].name;
            size_t pos2 = fileName.find("&");
            postContentId = fileName;
            if (pos2 != std::string::npos) {
                postContentId = fileName.substr(0, pos2);
                productName = fileName.substr(pos2 + 1);
            }
        }

        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (screen != "TIK_HOME") {
            gotoHome(xml);
            xml = "";
        }

        for (int loop = 50; loop > 0; loop--) {
            updateLine("Up Post - " + to_string(loop));
            delay(1);
            xml = dumpXml();
            screen = handleScreen(xml);

            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "TIK_HOME" && !isSuccess || screen == "TIK_PROFILE") {
                updateLine("Tap Create");
                tap("//*[@content-desc='Create']", xml);
                delay(5, "Opening Camera");
                continue;
            }
            if (screen == "TIK_UPLOAD_PHOTOS") {
                updateLine("tap Album");
                tap("//*[@text='Upload']", xml);
                delay(2);
                continue;
            }

            if (screen == "TIK_ALBUM_PHOTO") {
                if (existNode(
                        "//*[@resource-id='com.zhiliaoapp.musically:id/hh3' and @checked='false' or @resource-id='com.ss.android.ugc.trill:id/h0x']",
                        xml, 3))
                    tap(
                            "//*[@resource-id='com.zhiliaoapp.musically:id/hh3' and @checked='false' or @resource-id='com.ss.android.ugc.trill:id/h0x']",
                            xml);
                //check up ảnh cuộn hay ảnh thường
                delay(1);
                string xpathImageScroll;

                xml = dumpXml();
                // click ảnh cuộn
                for (int i = 1; i < nudAnhCuon + 1; ++i) {
                    xpathImageScroll =
                            "//*[@resource-id=\"com.zhiliaoapp.musically:id/eh4\"]/android.widget.FrameLayout[" +
                            to_string(i) +
                            "]/android.widget.FrameLayout[1]/android.widget.Button[1]";
                    if (existNode(xpathImageScroll, xml, 0)) {
                        tap(xpathImageScroll);
                        delay(1, 2);
                    }
                }

                delay(3);
                xml = dumpXml();
                string xpathCreateVideo = existNodes(3, xml, {
                        "//*[contains(@text,'Next ')]",
                        "//*[@text='Next']"
                });
                if (ckbAutoCut) {
                    xpathCreateVideo = "//*[@text='AutoCut']";
                    timeCreateVideo = 30;
                }
                tap(xpathCreateVideo);
                updateLine("Loading Video");
                existNode("//*[@text='Text']", xml, 300);
                updateLine("Loaded Video");
                continue;
            }

            //special case
            if (existNode("//*[@text='Upload']", xml, 0) &&
                existNode("//*[@text='Effects']", xml, 0)) {
                tap("//*[@text='Upload']");
                string xpathVideoImage = "//*[@resource-id='com.zhiliaoapp.musically:id/h84']";


                if (existNode(xpathVideoImage, xml, 0)) {

                    for (int i = 1; i < nudAnhCuon + 1; ++i) {
                        xpathVideoImage = "(" + xpathVideoImage + ")" + "[" + to_string(i) + "]";
                        if (existNode(xpathVideoImage, xml, 0)) {
                            tap(xpathVideoImage);
                            delay(1, 2);
                            continue;
                        }
                        tap("//*[@text='Photos']");
                        int imageCount = 1;
                        i = imageCount;
                    }
                }
                string xpathCreateVideo = existNodes(3, xml, {
                        "//*[contains(@text,'Next ')]",
                        "//*[@text='Next']"
                });
                tap(xpathCreateVideo);
                updateLine("Loading Video");
                existNode("//*[@text='Text']", xml, 300);
                updateLine("Loaded Video");
                continue;
            }
            if (screen == "TIK_CHANGE_SOUND") {
                tap("(//*[@class='android.widget.Button'])[1]");
                delay(1);
                continue;
            }
            if (screen == "TIK_EDIT_PHOTO" ||
                existNode("//*[@text='Text']", xml) && existNode("//*[@text='Stickers']", xml)) {
                if (ckbAddFilter) {

                    if (existNode("//*[@text='Filters']", xml)) {
                        tap("//*[@text='Filters']",
                            xml); // Thêm lệnh tap vào Filters nếu nó tồn tại
                        delay(2);
                    } else {
                        tap("//*[@resource-id='com.zhiliaoapp.musically:id/mkt']");
                        delay(2);
                        tap("//android.widget.TextView[@text='Effects']/following::android.widget.ImageView[2]");
                        delay(2);
                        xml = dumpXml();
                    }

                    if (isFilterFound ||
                        cbbRandomFilter == 1) { // Nếu người dùng chọn filter theo tên
                        int maxSwipe = 10;
                        for (int i = 0; i < maxSwipe; ++i) {
                            xml = dumpXml();
                            // Kiểm tra xem filter mong muốn có trên màn hình không?
                            string xpathFilter = "//*[@text='" + filterName + "']";
                            if (!existNode(xpathFilter, xml)) {
                                vector<Node> lstFilters = getNodes(
                                        "//*[@resource-id='com.zhiliaoapp.musically:id/d7n']", xml);
                                swipe(lstFilters.back(), lstFilters.front());
                                delay(2);
                                xml = dumpXml();
                            }
                            tap(xpathFilter, xml);
                            delay(2);
                            isFilterFound = true;
                        }
                    }
                    if (isFilterFound || cbbRandomFilter == 0) {
                        xml = dumpXml();
                        vector<Node> lstFilters = getNodes(vector<string>{
                                "//*[@resource-id='com.zhiliaoapp.musically:id/d7n']",
                        }, xml);

                        if (!lstFilters.empty()) {
                            size_t maxIndex = lstFilters.size() - 1;
                            int randomIndex = Common::ranNumber(0, static_cast<int>(maxIndex));
                            Node selectedFilter = lstFilters[randomIndex];
                            updateLine("Apply random filter (" + to_string(randomIndex) + ")");
                            tapCenter(selectedFilter);
                            delay(2);
                            isFilterFound = true;
                        } else {
                            updateLine("No filters found in the list!");
                        }
                    }
                    back();
                    delay(2);
                }

                if (ckbAddTrendSound && !isSoundAdjusted) {
                    xml = dumpXml();
                    updateLine("Add trend sound...");
                    string element = existNodes(0, xml, {
                            "//*[@resource-id='com.zhiliaoapp.musically:id/bkb']",
                    });
                    if (element == "//*[@resource-id='com.zhiliaoapp.musically:id/bkb']") {
                        tap(element, xml);
                        delay(2);
                        continue;
                    }
                }


                tap("//*[@text='Next']");
                delay(2);
                continue;
                //}

                tap("//*[@text='Text']", xml);
                delay(1, 2);

                //thay viền chữ
                tap("(//*[@text='Done']//preceding-sibling::*[1]/child::*)[3]");
                tap("(//*[@text='Done']//preceding-sibling::*[1]/child::*)[3]");
                delay(1);

                // nhập nội dung
                auto [success, content, itemId] = getScriptContent(imgContentId);

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
                xml = dumpXml();
                string xDone = "//*[@text='Done']";
                if (!existNode(xDone, xml, 3))
                    performAction(Perform::BACK, 1, 1);

                tap(xDone);
                delay(2);

                continue;
            }


            if (screen == "TIK_ADD_SOUND" && !isSoundAdjusted) {
                string element = existNodes(0, xml, {
                        "//*[@resource-id='com.zhiliaoapp.musically:id/oqv']",
                });
                if (element == "//*[@resource-id='com.zhiliaoapp.musically:id/oqv']") {
                    tap(element, xml);
                    delay(2);
                    xml = dumpXml();
                    //Lấy vị trí của thanh added sound
                    string addSoundXpath = "//*[@resource-id='com.zhiliaoapp.musically:id/klc']";
                    Node addSoundNode = getNode(addSoundXpath, xml);
                    if (!addSoundNode.empty()) {
                        int tapX = addSoundNode.left + (addSoundNode.width / 9);
                        int tapY = addSoundNode.centerY;

                        tap(tapX, tapY);
                        delay(2);
                    }
                    string xpathDoneButton = "//*[@text='Done']";
                    if (existNode(xpathDoneButton, xml)) {
                        tap(xpathDoneButton, xml);
                        delay(1);
                        isSoundAdjusted = true;
                    }
                }
            }

            if (screen == "TIK_ADD_PRODUCT") {
                tap("//*[@class='com.bytedance.ies.xelement.input.LynxInputView']");
                delay(3);
                handleKeyboard(Keyboard::INPUT, linkProduct);

                delay(1);

                handleKeyboard(Keyboard::ENTER);

                delay(2, "Waiting Search");

                tap("//*[@content-desc='Add'][1]");

                delay(3);

                // check popup
                string xpathPopup = "//*[@resource-id=\"android:id/content\"]/android.view.ViewGroup[1]/android.view.ViewGroup[1]/android.view.ViewGroup[1]/android.view.ViewGroup[1]";
                xml = dumpXml();
                if (existNode(xpathPopup, xml, 3))
                    tap(xpathPopup);
                delay(10, "Waiting load NameProduct: ");
                continue;
            }

            if (screen == "TIK_ADD_A_PRODUCT") {
                delay(5, "Loading Product");
                //tap("//*[@class='com.bytedance.ies.xelement.input.LynxInputView']");
                //delay(2);
                //handleKeyboard(Keyboard::INPUT, displayName);
                performAction(Perform::BACK, 1);
                delay(3, "delay Add..");
                tap("//*[@content-desc='Add']");
                delay(1, 2);
                continue;
            }

            if (screen == "TIK_POST_ADD_LINK") {
                tap("//*[@text='Post a product to your video']");
                delay(15, "Waiting load List Product: ");
                continue;
            }

            if (screen == "TIK_POST_CONFIG") {
                updateLine("Post Setting");

                if (ckbXoaAnhDaDung) {
                    deleteScriptMedia(lstMedia);
                }

                if (ckbAddLinkProduct) {
                    ckbAddLinkProduct = false;
                    if (existNode("//*[@text='Filters']", xml)) {
                        tap("//*[@text='Filters']",
                            xml); // Thêm lệnh tap vào Filters nếu nó tồn tại
                        delay(2);
                    } else {
                        tap("//*[@resource-id='com.zhiliaoapp.musically:id/mkt']");
                        delay(2);
                        tap("//android.widget.TextView[@text='Effects']/following::android.widget.ImageView[2]");
                        delay(2);
                        xml = dumpXml();
                    }
                    if (tapAndScreenNotChange("//*[@content-desc='Add link']", 3)) {
                        updateLine("Need open Tiktok Shop");
                        saveStatusAcc("Add link", "Need open Tiktok Shop or Video need Music");
                        return NOMAP;
                    }
                    delay(1, 2);
                    continue;
                }

                if (postContentId != "") {
                    if (ckbPostFilename) {
                        inputText("//android.widget.EditText", postContentId + " ");
                        delay(1);
                    } else {
                        auto [success, content, itemId] = getScriptContent(postContentId);

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
                    }

                    performAction(Perform::BACK, 1, 2);
                }
                //setting post
                string xpathTap = existNodes(0, xml,
                                             vector<string>{
                                                     "//*[@text='Friends can view this post']",
                                                     "//*[@text='Only you can view this post']",
                                                     "//*[@content-desc='Allow comments' and @checked='false']",
                                                     "//*[@resource-id=\"com.zhiliaoapp.musically:id/jfv\"]"
                                             });
                if (xpathTap == "//*[@content-desc='Allow comments' and @checked='false']") {
                    tap(xpathTap);
                    delay(1);
                    continue;
                }
                if (xpathTap == "//*[@resource-id=\"com.zhiliaoapp.musically:id/jfv\"]") {
                    tap(xpathTap);
                    // popup hoi ban quyen
                    delay(3);
                    tap("//*[@text='Post video']");
                    int ranTimeDone = Common::ranNumber(nudTimeOutFrom, nudTimeOutTo);
                    delay(ranTimeDone, "Delay Time Upload");
                    isSuccess = true;
                    continue;
                }

                if (!xpathTap.empty()) {
                    tap(xpathTap);
                    delay(1);
                    tap("(//*[@class='android.widget.RadioButton'])[1]");
                    delay(1);
                    continue;
                }
            }
            if (isSuccess && screen == "TIK_HOME" || screen == "TIK_FRIENDS") {
                tap("//*[@text='Profile']");
                delay(2);


                return DONE;
            }
        }
        return NOMAP;
    }

    ResultScreen changePassword(string mail, string password) {
        string oldOtp = "";

        for (int loop = 50; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            string xml = dumpXml();
            string screen = handleScreen(xml);

            if (screen == "CONTINUE")
                continue;

            if (screen == "ERROR") return ERROR;

            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
            if (screen == "TIK_ACCOUNT_INFO") {
                performAction(Perform::BACK, 1, 1);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                updateLine("tap Setting");
                tap("//*[@content-desc='Profile menu']");
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_PROFILE_MENU") {
                updateLine("tap Setting and privacy");
                tap("//*[@text='Settings and privacy']", dumpXml());
                delay(2, 3);
                continue;
            }
            if (screen == "TIK_SETTING_PRIVACY") {
                updateLine("tap Account");
                tap("(//*[@text='Account'])[2]");
                delay(1);
                continue;
            }
            if (screen == "TIK_ACCOUNT_SETTING") {
                updateLine("Tap Password");
                tap("//*[@content-desc='Password']", xml);
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_ACCOUNT_OTPCHANGEPASS" || screen == "TIK_SIGN_UP_VERIFY_EMAIL") {
                string otp = getOtpFromEmail(mail, 60, oldOtp);
                if (otp.empty() || otp == oldOtp) {
                    //tap resend
                    tap("//*[@text='Resend code']");
                    delay(2);
                    continue;
                }

                oldOtp = otp;
                updateLine("Input OTP");
                inputText("//android.widget.EditText", otp);
                delay(5);
                continue;
            }
            if (screen == "TIK_ACCOUNT_INPUTPASSWORD") {
                updateLine("Input NewPassword");
                //password_value
                inputText("//android.widget.EditText[@text='Enter password']",
                          password);
                delay(1, 2);
                performAction(Perform::BACK, 1, 2);

                tap("//*[@text='Next']");
                updateLine("Changed Password DONE");

                xml = dumpXml();
                gotoHome(xml);
                return DONE;
            }
        }
        return NOMAP;
    }

    JSON
    runRegAccGmailRunJob(JSON json) {
        JSON output;
        string currentVersion = getPackageVersion(social.package);

        int delayBirthdayFrom = json.getInt("delayBirthdayFrom");
        int delayBirthdayTo = json.getInt("delayBirthdayTo");

        int ranTimeComfirmNameFrom = 20;
        int ranTimeComfirmNameTo = 30;
        bool ckbSwitchTBA = json.getBool("isSwitchBusiness");

        string userWeb1989 = json.getString("userBuyMail");
        string passWeb1989 = json.getString("passBuyMail");
        string productIdWeb1989 = json.getString("productIdBuyMail");

        if (userWeb1989.empty() ||
            passWeb1989.empty() ||
            productIdWeb1989.empty()) {
            output.set("error", "API Key is empty");
            return output;
        }

        auto result = buyGmail(userWeb1989, passWeb1989, productIdWeb1989, json.getInt("typeMail"));

        if (!result.first) {
            output.set("error", "Can't get gmail : " + result.second);
            return output;
        }
        string mailGG = Common::strSplit(result.second, "|")[0];
        string mkGG = Common::strSplit(result.second, "|")[1];

        string txtPassword = json.getString("defaultPassword");
        bool ckbRandomPass = json.getBool("isRandomPass");
        string password = Common::randomChar(txtPassword);
        if (ckbRandomPass) {
            int nudCountCharPass = json.getInt("countRandomerCharPass");
            password = Common::ranString(1, "A") + Common::ranString(1, "a") +
                       Common::ranString(1, "0") +
                       Common::ranString(nudCountCharPass - 4, "a_0_A") + "@";
        }

        string name;
        bool ckbSetName = json.getBool("isRename");
        bool isRegDone = false;
        //code here

        if (ckbSetName) {
            int cbbTypeName = json.getInt(
                    "typeName");
            if (cbbTypeName == 0) {
                name = Common::ranNameViet();
            } else if (cbbTypeName == 1) {
                name = Common::ranNameNgoai();
            } else if (cbbTypeName == 2) {
                vector<string> lstFirstName = json.getListSplit("firstName");
                vector<string> lstMiddleName = json.getListSplit("middleName");
                vector<string> lstLastName = json.getListSplit("lastName");

                if (lstFirstName.empty() &&
                    lstMiddleName.empty() &&
                    lstLastName.empty()) {
                    name = Common::ranBool(50) == true ? Common::ranNameViet()
                                                       : Common::ranNameNgoai();
                }

                name = Common::ranItem(lstFirstName) + " " + Common::ranItem(lstMiddleName) +
                       " " +
                       Common::ranItem(lstLastName);
                name = Common::strReplace(name, "  ", " ");
            } else {
                vector<string> lstFullName = json.getListSplit("fullName");
                if (lstFullName.empty())
                    name = Common::ranBool(50) == true ? Common::ranNameViet()
                                                       : Common::ranNameNgoai();

                name = Common::ranItem(lstFullName);
            }
        }

        string domain = json.getString("mailDomain");
        string emailChange =
                Common::ranString(10, "a") + Common::ranString(5, "0") + "@" + domain;

        //reg acc

        bool isClearData = json.getBool("clearDataAfterRegister");

        if (isClearData)
            clearDataAppAction(social.package);

        if (!openSocial(true)) {
            saveStatusAcc("Reg", "Open Package Fail");
            return output;
        }

        int countAgain = 0;
        int maxAgain = 3;

        string xml;
        updateLine("Reg acc");
        for (int loop = 300; loop > 0; loop--) {
            if (isStopped()) {
                removeGoogleAccount();
                output.set("error", "stop");
                break;
            }

            xml = dumpXml();
            string screen = detectScreen(xml);

            if (Common::strContains(screen, "_POPUP_")) continue;
            if (screen == "TIK_HOME") {
                string xpathTap = existNodes(0, xml, {"//*[@text='Sign up']",
                                                      "//*[@content-desc='Like'][@selected='false']",
                                                      "//*[@text='Profile']"
                });
                if (!xpathTap.empty()) {
                    tap(xpathTap, xml);
                    delay(2);
                    continue;
                }
            }
            if (screen == "TIK_PROFILE") {
                if (isRegDone) {
                    string username = Common::regexGroups1(xml, "@(.*?)\"");
                    if (username.empty())
                        continue;
                    //updateLine("Up Video");
                    //upImageOrVideo(30, 35);
                    updateLine("Interacting after reg acc");
                    JSON configs = JSON();
                    configs.set("nudSoLuongFrom", 3);//số lượng video from
                    configs.set("nudSoLuongTo", 10);//số lượng video to
                    configs.set("nudTimeFrom", 3);//thời gian xem video from
                    configs.set("nudTimeTo", 15);//thời gian xem video to
                    configs.set("ckbInteract", true);//có like không?
                    configs.set("ckbFollow", false);//có Follow không?
                    action.jsonConfig = configs;
                    actionTuongTacVideoRandom();
                    actionPublicLike();
                    actionTuongTacVideoRandom();
                    //updateLine("close app");
                    //if (!openSocial(true)) {
                    //    saveStatusAcc("Reg", "Open Package Fail");
                    //    output.set("error", "error");
                    //    break;
                    //}

                    //delay(3, "waiting change mail:..");
                    //if (!emailChange.empty())
                    //    changeMail(emailChange);

                    //delay(1, 2, "waiting change password:..");
                    //
                    //updateLine("change password");
                    //
                    //if (changePassword(emailChange, password) == DONE) {
                    //    account.uid = username;
                    //    if (ckbSwitchTBA)
                    //        switchToBusinessAccount(true, true, true);
                    //    delay(1, 2);
                    output.set("success", true);
                    output.set("username", username);
                    output.set("password", password);
                    output.set("email", mailGG);
                    //}

                    return output;
                }

                vector<Node> lstNodes = getNodes(
                        "//android.widget.FrameLayout/android.view.ViewGroup/android.widget.LinearLayout/child::*",
                        xml);
                if (lstNodes.size() > 0) {
                    for (int i = lstNodes.size() - 1; i >= 0; i--) {
                        Node node = lstNodes.at(i);
                        if (node.width > node.height) {
                            tap(node.left + node.width - 50, node.centerY);
                            delay(2);
                            break;
                        }
                    }
                    continue;
                }

            }


            if (screen == "TIK_LOGIN") {
                if (!tap("//*[@text='Don’t have an account? Sign up']", xml))
                    tap("//*[@text='Use phone / email / username']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP") {
                //
                Node swipeFrom = getNode("//*[@text='Use phone or email']", xml, 0);
                Node swipeTo = getNode("//*[@text='Sign up for TikTok']", xml, 0);

                string xpathGG = "//*[@text='Continue with Google']";
                if (!existNode(xpathGG, xml, 0)) {
                    swipe(swipeFrom.centerX, swipeFrom.centerY, swipeTo.centerX,
                          swipeTo.centerY,
                          500);
                    xml = dumpXml();
                }
                tap(xpathGG, xml);
                delay(1, 2, "Login Google...");
                continue;
            }

            if (screen == "TIK_WAITING_GOOGLE") {
                delay(10, 15);
                continue;
            }

            if (screen == "TIK_EXIST_GOOGLE_MAIL") {
                removeGoogleAccount();
                delay(3, "clear app..");
                if (isClearData)
                    clearDataAppAction(social.package);
                if (!openSocial(true)) {
                    saveStatusAcc("Reg", "Open Package Fail");
                    output.set("error", "error");
                    break;
                }
                delay(1);
                continue;
            }
            if (screen == "TIK_LOGIN_PHONE" || screen == "TIK_LOGIN_EMAIL") {
                maxAgain--;
                if (maxAgain < 0)
                    break;

                performAction(Perform::BACK, 2, 1);

                //tap("//*[@content-desc='Back to previous screen']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_REGMAIL_INPUTPASSMAIL") {
                delay(1, "password...");
                inputText("//android.widget.EditText", mkGG);
                performAction(Perform::BACK, 1, 2);

                tap("//*[@text='Next']");
                delay(30, "Time out login");
                continue;
            }
            if (screen == "TIK_REGMAIL_INPUTMAIL") {
                string xpathInput = existNodes(0, xml, {
                        "(//android.widget.EditText)[2]",
                        "//android.widget.EditText"
                });
                delay(1, 2, "account...");
                inputText(xpathInput, mailGG);
                performAction(Perform::BACK, 1, 2);

                delay(1, 2);
                tap("//*[@text='Next']");
                delay(3);
                continue;
            }
            if (screen == "TIK_ACCOUNT_NOTFIND_GG_ACCOUNT") {
                inputText("//android.widget.EditText", mailGG);
                delay(2);
                performAction(Perform::BACK, 1, 1);
                tap("//*[@text='Next']");
                delay(1, 2);
                continue;
            }

            if (screen == "TIK_REGMAIL_INPUTPASSMAIL_ERROR") {
                delay(1, "password...");
                inputText("//android.widget.EditText", mkGG);
                delay(2);
                performAction(Perform::BACK, 1, 2);
                xml = dumpXml();
                tap("//*[@text='Next']");
                delay(30, "Time out login");
                continue;
            }
            if (screen == "TIK_REG_GOOGLE_AGREE") {
                if (!existNode("//*[@text='I agree']", xml, 0))
                    swipe();
                delay(2, "Tap Agree");
                tap("//*[@text='I agree']");
                delay(1, 2);
                continue;
            }

            //block mail chuyển mail khác
            if (screen == "TIK_USE_ANOTHER_MAIL") {
                tap("//*[@text='Use another account']", xml);
                output.set("error", "error");
                break;
            }
            if (screen == "TIK_LOGIN_EXIST_ACCOUNT") {
                countAgain++;
                if (countAgain > maxAgain) {
                    output.set("error", "error");
                    break;
                }


                tap("(//*[@text='Profile'])[last()]", xml);
                delay(3);
                continue;
            }
            if (screen == "TIK_SIGN_UP_BIRTHDAY" && !isRegDone) {
                updateLine("set Birthday");
                if (currentVersion == "33.6.1") {
                    string xpathDMY = "//*[@text='Next']/following-sibling::*[last()]/child::*/child::*";
                    for (int i = 1; i <= 3; ++i) {
                        Node node = getNode(xpathDMY + "[" + to_string(i) + "]", xml);
                        int countScroll = Common::ranNumber(2, 5);
                        if (i == 3)
                            countScroll += 20;
                        for (int j = 0; j < countScroll; ++j) {
                            swipe(Common::ranNumber(node.left, node.right),
                                  Common::ranNumber(node.top, node.top + 20),
                                  Common::ranNumber(node.left, node.right),
                                  Common::ranNumber(node.bottom, node.bottom - 20), 500);
                            delay(1);
                        }
                    }

                }
                if (currentVersion == "37.4.4") {
                    string xpathMonth = "//*[@content-desc='Month picker']";
                    Node nodeMonth = getNode(xpathMonth, xml);
                    string xpathDay = "//*[@content-desc='Day picker']";
                    Node nodeDay = getNode(xpathDay, xml);
                    string xpathYears = "//*[@content-desc='Year picker']";
                    Node nodeYears = getNode(xpathYears, xml);

                    vector<int> scrollCounts = {Common::ranNumber(2, 5), Common::ranNumber(2, 5),
                                                Common::ranNumber(22, 25)
                    };

                    vector<Node> nodes = {nodeMonth, nodeDay, nodeYears};
                    for (int i = 0; i < nodes.size(); ++i) {
                        Node node = nodes[i];
                        int countScroll = scrollCounts[i];
                        for (int j = 0; j < countScroll; ++j) {
                            swipe(
                                    Common::ranNumber(node.left, node.right),
                                    Common::ranNumber(node.top, node.top + 20),
                                    Common::ranNumber(node.left, node.right),
                                    Common::ranNumber(node.bottom, node.bottom - 20),
                                    500
                            );
                            delay(1);
                        }
                    }
                }

                delay(delayBirthdayFrom, delayBirthdayTo, "delay birthday");
                if (tap("//*[@text='Next']"))
                    delay(3);
                isRegDone = true;


                continue;
            }
            if (screen == "TIK_SIGN_UP_NICKNAME") {
                if (!ckbSetName) {
                    delay(10);
                    tap("//*[@content-desc='Skip']", xml);
                    delay(5, "delay...");
                    continue;
                }
                inputText("//android.widget.EditText", name);
                delay(5);
                tap("//*[@text='Confirm']", xml);
                delay(30, "delay confirm:..");
                continue;

            }

            if (screen == "TIK_LIVE") {
                performAction(Perform::BACK, 1, 1);
                continue;
            }
            if (screen == "TIK_NEW_CHAT") {
                performAction(Perform::BACK, 1, 1);
                continue;
            }

            if (screen == "TIK_INBOX") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
            if (screen == "TIK_SIGN_UP_EXIST_ACCOUNT") {
                output.set("error", "account_exists");
                break;

            }

            if (screen == "TIK_SIGN_UP_BLOCK") {
                output.set("error", "block");
                break;
            }

            string xpath = existNodes(0, xml, {
                    "//*[@text='Welcome to your new account']",
                    "//*[@text='I understand']",
                    "//*[@text='Switch account']",
            });

            if (xpath == "//*[@text='Welcome to your new account']") {
                swipe(1, 6);
                delay(1);
                xml = dumpXml();
                if (existNode("//*[@text='I understand']", xml, 3))
                    tap("//*[@text='I understand']");
                delay(1);
                continue;
            }
            if (xpath == "//*[@text='I understand']") {
                tap(xpath);
                delay(1, 2);
                continue;
            }
            if (xpath == "//*[@text='Switch account']") {
                if (tap("//*[@text='Add account']", xml)) {
                    delay(2);
                    continue;
                }

                if (swipeAndScreenNotChange()) {
                    performAction(Perform::BACK, 1, 1);
                    handleLogout();

                }
                continue;
            }
            delay(1);
        }
        updateLine("Remove Google");
        return output;
    }

    JSON runRegAccGmail(JSON json) {
        //input
        JSON output;
        string currentVersion = getPackageVersion(social.package);

        int delayBirthdayFrom = json.getInt("delayBirthdayFrom");
        int delayBirthdayTo = json.getInt("delayBirthdayTo");

        int ranTimeComfirmNameFrom = 20;
        int ranTimeComfirmNameTo = 30;
        bool ckbSwitchTBA = json.getBool("isSwitchBusiness");

        string userWeb1989 = json.getString("userBuyMail");
        string passWeb1989 = json.getString("passBuyMail");
        string productIdWeb1989 = json.getString("productIdBuyMail");

        if (userWeb1989.empty() ||
            // passWeb1989.empty() ||
            productIdWeb1989.empty()) {
            output.set("error", "API Key is empty");
            return output;
        }

        auto result = buyGmail(userWeb1989, passWeb1989, productIdWeb1989, json.getInt("typeMail"));

        if (!result.first) {
            output.set("error", "Can't get gmail : " + result.second);
            return output;
        }

        string mailGG = Common::strSplit(result.second, "|")[0];
        string mkGG = Common::strSplit(result.second, "|")[1];


        string txtPassword = json.getString("defaultPassword");
        bool ckbRandomPass = json.getBool("isRandomPass");
        string password = Common::randomChar(txtPassword);
        if (ckbRandomPass) {
            int nudCountCharPass = json.getInt("countRandomerCharPass");
            password = Common::ranString(1, "A") + Common::ranString(1, "a") +
                       Common::ranString(1, "0") +
                       Common::ranString(nudCountCharPass - 4, "a_0_A") + "@";
        }

        string name;
        bool ckbSetName = json.getBool("isRename");
        bool isRegDone = false;
        //code here

        if (ckbSetName) {
            int cbbTypeName = json.getInt(
                    "typeName");
            if (cbbTypeName == 0) {
                name = Common::ranNameViet();
            } else if (cbbTypeName == 1) {
                name = Common::ranNameNgoai();
            } else if (cbbTypeName == 2) {
                vector<string> lstFirstName = json.getListSplit("firstName");
                vector<string> lstMiddleName = json.getListSplit("middleName");
                vector<string> lstLastName = json.getListSplit("lastName");

                if (lstFirstName.empty() &&
                    lstMiddleName.empty() &&
                    lstLastName.empty()) {
                    name = Common::ranBool(50) == true ? Common::ranNameViet()
                                                       : Common::ranNameNgoai();
                }

                name = Common::ranItem(lstFirstName) + " " + Common::ranItem(lstMiddleName) +
                       " " +
                       Common::ranItem(lstLastName);
                name = Common::strReplace(name, "  ", " ");
            } else {
                vector<string> lstFullName = json.getListSplit("fullName");
                if (lstFullName.empty())
                    name = Common::ranBool(50) == true ? Common::ranNameViet()
                                                       : Common::ranNameNgoai();

                name = Common::ranItem(lstFullName);
            }
        }

        string domain = json.getString("mailDomain");
        string emailChange =
                Common::ranString(10, "a") + Common::ranString(5, "0") + "@" + domain;

        //reg acc

        bool isClearData = json.getBool("clearDataAfterRegister");

        if (isClearData)
            clearDataAppAction(social.package);

        if (!openSocial(true)) {
            saveStatusAcc("Reg", "Open Package Fail");
            return output;
        }

        int countAgain = 0;
        int maxAgain = 3;

        string xml;
        updateLine("Reg acc");
        for (int loop = 300; loop > 0; loop--) {
            if (isStopped()) {
                removeGoogleAccount();
                output.set("error", "stop");
                break;
            }

            xml = dumpXml();
            string screen = detectScreen(xml);

            if (Common::strContains(screen, "_POPUP_")) continue;
            if (screen == "TIK_HOME") {
                string xpathTap = existNodes(0, xml, {"//*[@text='Sign up']",
                                                      "//*[@content-desc='Like'][@selected='false']",
                                                      "//*[@text='Profile']"
                });
                if (!xpathTap.empty()) {
                    tap(xpathTap, xml);
                    delay(2);
                    continue;
                }
            }
            if (screen == "TIK_PROFILE") {
                if (isRegDone) {
                    string username = Common::regexGroups1(xml, "@(.*?)\"");
                    if (username.empty())
                        continue;
                    //updateLine("Up Video");
                    //upImageOrVideo(30, 35);
                    updateLine("Interacting after reg acc");
                    JSON configs = JSON();
                    configs.set("nudSoLuongFrom", 1);//số lượng video from
                    configs.set("nudSoLuongTo", 2);//số lượng video to
                    configs.set("nudTimeFrom", 10);//thời gian xem video from
                    configs.set("nudTimeTo", 15);//thời gian xem video to
                    configs.set("ckbInteract", true);//có like không?
                    configs.set("ckbFollow", true);//có Follow không?
                    action.jsonConfig = configs;
                    actionTuongTacVideoRandom();


                    updateLine("close app");
                    if (!openSocial(true)) {
                        saveStatusAcc("Reg", "Open Package Fail");
                        output.set("error", "error");
                        break;
                    }

                    delay(3, "waiting change mail:..");
                    if (!emailChange.empty())
                        changeMail(emailChange);

                    delay(1, 2, "waiting change password:..");

                    updateLine("change password");

                    if (changePassword(emailChange, password) == DONE) {
                        account.uid = username;
                        if (ckbSwitchTBA)
                            switchToBusinessAccount(true, true, true);
                        delay(1, 2);
                        output.set("success", true);
                        output.set("username", username);
                        output.set("password", password);
                        output.set("email", emailChange);
                    } else {
                        output.set("error", "error");
                    }

                    return output;
                }

                vector<Node> lstNodes = getNodes(
                        "//android.widget.FrameLayout/android.view.ViewGroup/android.widget.LinearLayout/child::*",
                        xml);
                if (lstNodes.size() > 0) {
                    for (int i = lstNodes.size() - 1; i >= 0; i--) {
                        Node node = lstNodes.at(i);
                        if (node.width > node.height) {
                            tap(node.left + node.width - 50, node.centerY);
                            delay(2);
                            break;
                        }
                    }
                    continue;
                }

            }


            if (screen == "TIK_LOGIN") {
                if (!tap("//*[@text='Don’t have an account? Sign up']", xml))
                    tap("//*[@text='Use phone / email / username']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP") {
                //
                Node swipeFrom = getNode("//*[@text='Use phone or email']", xml, 0);
                Node swipeTo = getNode("//*[@text='Sign up for TikTok']", xml, 0);

                string xpathGG = "//*[@text='Continue with Google']";
                if (!existNode(xpathGG, xml, 0)) {
                    swipe(swipeFrom.centerX, swipeFrom.centerY, swipeTo.centerX,
                          swipeTo.centerY,
                          500);
                    xml = dumpXml();
                }
                tap(xpathGG, xml);
                delay(1, 2, "Login Google...");
                continue;
            }

            if (screen == "TIK_WAITING_GOOGLE") {
                delay(10, 15);
                continue;
            }

            if (screen == "TIK_EXIST_GOOGLE_MAIL") {
                removeGoogleAccount();
                delay(3, "clear app..");
                if (isClearData)
                    clearDataAppAction(social.package);
                if (!openSocial(true)) {
                    saveStatusAcc("Reg", "Open Package Fail");
                    output.set("error", "error");
                    break;
                }
                delay(1);
                continue;
            }
            if (screen == "TIK_LOGIN_PHONE" || screen == "TIK_LOGIN_EMAIL") {
                maxAgain--;
                if (maxAgain < 0)
                    break;

                performAction(Perform::BACK, 2, 1);

                //tap("//*[@content-desc='Back to previous screen']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_REGMAIL_INPUTPASSMAIL") {
                delay(1, "password...");
                inputText("//android.widget.EditText", mkGG);
                performAction(Perform::BACK, 1, 2);

                tap("//*[@text='Next']");
                delay(30, "Time out login");
                continue;
            }
            if (screen == "TIK_REGMAIL_INPUTMAIL") {
                string xpathInput = existNodes(0, xml, {
                        "(//android.widget.EditText)[2]",
                        "//android.widget.EditText"
                });
                delay(1, 2, "account...");
                inputText(xpathInput, mailGG);
                performAction(Perform::BACK, 1, 2);

                delay(1, 2);
                tap("//*[@text='Next']");
                delay(3);
                continue;
            }
            if (screen == "TIK_ACCOUNT_NOTFIND_GG_ACCOUNT") {
                inputText("//android.widget.EditText", mailGG);
                delay(2);
                performAction(Perform::BACK, 1, 1);
                tap("//*[@text='Next']");
                delay(1, 2);
                continue;
            }

            if (screen == "TIK_REGMAIL_INPUTPASSMAIL_ERROR") {
                delay(1, "password...");
                inputText("//android.widget.EditText", mkGG);
                delay(2);
                performAction(Perform::BACK, 1, 2);
                xml = dumpXml();
                tap("//*[@text='Next']");
                delay(30, "Time out login");
                continue;
            }
            if (screen == "TIK_REG_GOOGLE_AGREE") {
                if (!existNode("//*[@text='I agree']", xml, 0))
                    swipe();
                delay(2, "Tap Agree");
                tap("//*[@text='I agree']");
                delay(1, 2);
                continue;
            }

            if (existNode("//*[@text=\"Never lose your contacts\"]", xml, 0)) {
                tap("//*[@text=\"Don't turn on\"]");
                delay(2, "");
                continue;
            }

            if (existNode(
                    "//*[@text=\"Complete a few suggestions to get the most out of your Google Account\"]",
                    xml, 0)) {
                tap("//*[@text=\"Not now\"]");
                delay(2);
                continue;
            }

            if (existNode("//*[@text=\"Back up device data\"]", xml, 0)) {
                tap("//*[@text=\"Accept\"]");
                delay(2);
                continue;
            }

            //block mail chuyển mail khác
            if (screen == "TIK_USE_ANOTHER_MAIL") {
                tap("//*[@text='Use another account']", xml);
                output.set("error", "error");
                break;
            }
            if (screen == "TIK_LOGIN_EXIST_ACCOUNT") {
                countAgain++;
                if (countAgain > maxAgain) {
                    output.set("error", "error");
                    break;
                }


                tap("(//*[@text='Profile'])[last()]", xml);
                delay(3);
                continue;
            }
            if (screen == "TIK_SIGN_UP_BIRTHDAY" && !isRegDone) {
                updateLine("set Birthday");
                if (currentVersion == "33.6.1") {
                    string xpathDMY = "//*[@text='Next']/following-sibling::*[last()]/child::*/child::*";
                    for (int i = 1; i <= 3; ++i) {
                        Node node = getNode(xpathDMY + "[" + to_string(i) + "]", xml);
                        int countScroll = Common::ranNumber(2, 5);
                        if (i == 3)
                            countScroll += 20;
                        for (int j = 0; j < countScroll; ++j) {
                            swipe(Common::ranNumber(node.left, node.right),
                                  Common::ranNumber(node.top, node.top + 20),
                                  Common::ranNumber(node.left, node.right),
                                  Common::ranNumber(node.bottom, node.bottom - 20), 500);
                            delay(1);
                        }
                    }

                }
                if (currentVersion == "37.4.4") {
                    string xpathMonth = "//*[@content-desc='Month picker']";
                    Node nodeMonth = getNode(xpathMonth, xml);
                    string xpathDay = "//*[@content-desc='Day picker']";
                    Node nodeDay = getNode(xpathDay, xml);
                    string xpathYears = "//*[@content-desc='Year picker']";
                    Node nodeYears = getNode(xpathYears, xml);

                    vector<int> scrollCounts = {Common::ranNumber(2, 5), Common::ranNumber(2, 5),
                                                Common::ranNumber(22, 25)
                    };

                    vector<Node> nodes = {nodeMonth, nodeDay, nodeYears};
                    for (int i = 0; i < nodes.size(); ++i) {
                        Node node = nodes[i];
                        int countScroll = scrollCounts[i];
                        for (int j = 0; j < countScroll; ++j) {
                            swipe(
                                    Common::ranNumber(node.left, node.right),
                                    Common::ranNumber(node.top, node.top + 20),
                                    Common::ranNumber(node.left, node.right),
                                    Common::ranNumber(node.bottom, node.bottom - 20),
                                    500
                            );
                            delay(1);
                        }
                    }
                }

                delay(delayBirthdayFrom, delayBirthdayTo, "delay birthday");
                if (tap("//*[@text='Next']"))
                    delay(3);
                isRegDone = true;


                continue;
            }
            if (screen == "TIK_SIGN_UP_NICKNAME") {
                if (!ckbSetName) {
                    delay(10);
                    tap("//*[@content-desc='Skip']", xml);
                    delay(5, "delay...");
                    continue;
                }
                inputText("//android.widget.EditText", name);
                delay(5);
                tap("//*[@text='Confirm']", xml);
                delay(30, "delay confirm:..");
                continue;

            }

            if (screen == "TIK_LIVE") {
                performAction(Perform::BACK, 1, 1);
                continue;
            }
            if (screen == "TIK_NEW_CHAT") {
                performAction(Perform::BACK, 1, 1);
                continue;
            }

            if (screen == "TIK_INBOX") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
            if (screen == "TIK_SIGN_UP_EXIST_ACCOUNT") {
                output.set("error", "account_exists");
                break;

            }

            if (screen == "TIK_SIGN_UP_BLOCK") {
                output.set("error", "block");
                break;
            }

            string xpath = existNodes(0, xml, {
                    "//*[@text='Welcome to your new account']",
                    "//*[@text='I understand']",
                    "//*[@text='Switch account']",
            });

            if (xpath == "//*[@text='Welcome to your new account']") {
                swipe(1, 6);
                delay(1);
                xml = dumpXml();
                if (existNode("//*[@text='I understand']", xml, 3))
                    tap("//*[@text='I understand']");
                delay(1);
                continue;
            }
            if (xpath == "//*[@text='I understand']") {
                tap(xpath);
                delay(1, 2);
                continue;
            }
            if (xpath == "//*[@text='Switch account']") {
                if (tap("//*[@text='Add account']", xml)) {
                    delay(2);
                    continue;
                }

                if (swipeAndScreenNotChange()) {
                    performAction(Perform::BACK, 1, 1);
                    handleLogout();

                }
                continue;
            }
            delay(1);
        }
        updateLine("Remove Google");
        return output;
    }

    void handleRegisterAccount() {
        resetVariable();

        if (!setupPhone({})) {
            delay(2, "Setup Phone failed");
            return;
        }

        if (!setupNetwork()) {
            delay(2, "Setup Network failed");
            return;
        }

        IResult response = OCRequest("tiktokapp/get-register-setting");

        if (!response.succeeded) {
            delay(5, "Get Register config failed");
            return;
        }

        JSON json = response.json;

        JSON result;

        if (json.getInt("typeMail") == 0) {
            result = runRegAcc(json);
        } else {
            removeGoogleAccount();
            result = runRegAccGmail(json);
            removeGoogleAccount();
        }

        if (!result.getBool("success")) {
            string error = result.getString("error");
            if (error == "error") {
                updateLine("Reg fail");
                return;
            }
            if (error == "stop") {
                updateLine("Stop reg");
                return;
            }

            delay(5, "Reg fail: " + error);
            return;
        }

        //Reg success
        string username = result.getString("username");
        string password = result.getString("password");
        string email = result.getString("email");

        account.uid = username;

        string galleryId = json.getString("galleryId");

        if (!galleryId.empty()) {
            updateLine("Up avatar");
            action = {};
            action.setString("galleryId", galleryId);
            actionUpAvatar();
        }

        Storage::writeFile(APP_FOLDER + Common::currentDate() + ".txt",
                           username + "|" + password + "|" + email + "|" +
                           Common::currentDatetime() + "\n",
                           true);

        JSON jsonAddAccount;
        jsonAddAccount.set("uid", username);
        jsonAddAccount.set("password", password);
        jsonAddAccount.set("email", email);

        IResult resultAdd = OCRequest("tiktokapp/add-register-account",
                                      jsonAddAccount.toString());
        if (!resultAdd.succeeded) {
            delay(10, "Upload Account failed");
            Common::NLog(resultAdd.response, "Upload Account failed");
            return;
        }

        account.id = resultAdd.data;

        if (json.getBool("isUpPost")) {
            updateLine("Up Post");
            action = {};
            action.setString("galleryId", json.getBool("isPostSameAvatar") ? "" : galleryId);

//            action.galleryId = json.getBool("isPostSameAvatar") ? "" : json.getString("galleryId");

            actionUpPost();
        }

        if (json.getBool("isVerifyMail") && json.getInt("typeMail") == 0) {
            updateLine("Verify: Get otp");
            string link = getOtpFromEmail(email);
            if (!link.empty() && Common::strStartsWith(link, "https")) {
                updateLine("Verify: Wait");
                gotoLink(link);
                delay(5);

                string xml;
                string resultVerify = existNode(
                        "//*[@text='Thanks for verifying!']", xml, 30)
                                      ? "success" : "fail";
                updateLine("Verify: " + resultVerify);
            } else {
                updateLine("Verify: fail (No otp)");
            }
        }

        if (json.getBool("isEarnMoney")) {
            account.resetVariable();
            actionEarnMoney();
        }


        countRegisterAccount++;
        if (countRegisterAccount >= json.getInt("limitRegAccountPerRun")) {
            isStop = true;
            updateLine(
                    "Register Success : " + to_string(countRegisterAccount) + " Account");
            return;
        }
    }

    JSON runRegAcc(JSON json) {
        JSON output;

        //input
        string txtPassword = json.getString("defaultPassword");
        bool ckbRandomPass = json.getBool("isRandomPass");
        string password = Common::randomChar(txtPassword);

        if (ckbRandomPass) {
            int nudCountCharPass = json.getInt("countRandomerCharPass");
            password = Common::ranString(1, "A") + Common::ranString(1, "a") +
                       Common::ranString(1, "0") +
                       Common::ranString(nudCountCharPass - 4, "a_0_A") + "@";
        }

        int delayBirthdayFrom = json.getInt("delayBirthdayFrom");
        int delayBirthdayTo = json.getInt("delayBirthdayTo");

        string name;
        bool ckbSetName = json.getBool("isRename");
        bool isRegDone = false;

        if (ckbSetName) {
            int cbbTypeName = json.getInt(
                    "typeName");
            if (cbbTypeName == 0) {
                name = Common::ranNameViet();
            } else if (cbbTypeName == 1) {
                name = Common::ranNameNgoai();
            } else if (cbbTypeName == 2) {
                vector<string> lstFirstName = json.getListSplit("firstName");
                vector<string> lstMiddleName = json.getListSplit("middleName");
                vector<string> lstLastName = json.getListSplit("lastName");

                if (lstFirstName.empty() &&
                    lstMiddleName.empty() &&
                    lstLastName.empty()) {
                    name = Common::ranBool(50) == true ? Common::ranNameViet()
                                                       : Common::ranNameNgoai();
                }

                name = Common::ranItem(lstFirstName) + " " + Common::ranItem(lstMiddleName) +
                       " " +
                       Common::ranItem(lstLastName);
                name = Common::strReplace(name, "  ", " ");
            } else {
                vector<string> lstFullName = json.getListSplit("fullName");
                if (lstFullName.empty())
                    name = Common::ranBool(50) == true ? Common::ranNameViet()
                                                       : Common::ranNameNgoai();

                name = Common::ranItem(lstFullName);
            }
        }

        string domain = json.getString("mailDomain");
        string email = Common::ranString(10, "a") + Common::ranString(5, "0") + "@" + domain;

        //reg acc
        updateLine("Reg acc");

        bool isClearData = json.getBool("clearDataAfterRegister");

        if (isClearData)
            clearDataAppAction(social.package);

        if (!openSocial(false)) {
            saveStatusAcc("reg", "Open Package Fail");
            return output.set("error", "stop");
        }

        int maxAgain = 3;
        int maxInputBirthday = 3;

        string xml;

        updateLine("Reg acc");
        for (int loop = 50; loop > 0; loop--) {
            if (isStopped()) {
                output.set("error", "stop");
                break;
            }

            xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") {
                output.set("error", "error");
                break;
            }
            if (screen == "TIK_HOME") {
                string xpathTap = existNodes(0, xml, {"//*[@text='Sign up']",
                                                      "//*[@content-desc='Like'][@selected='false']",
                                                      "//*[@text='Profile']"
                });
                if (!xpathTap.empty()) {
                    tap(xpathTap, xml);
                    delay(2);
                    continue;
                }
            }
            if (screen == "TIK_PROFILE") {
                if (isRegDone) {
                    string username = Common::regexGroups1(xml, "@(.*?)\"");
                    if (username.empty())
                        continue;

                    output.set("success", true);
                    output.set("username", username);
                    output.set("password", password);
                    output.set("email", email);

                    return output;
                }

                vector<Node> lstNodes = getNodes(
                        "//android.widget.FrameLayout/android.view.ViewGroup/android.widget.LinearLayout/child::*",
                        xml);
                if (lstNodes.size() > 0) {
                    for (int i = lstNodes.size() - 1; i >= 0; i--) {
                        Node node = lstNodes.at(i);
                        if (node.width > node.height) {
                            tap(node.left + node.width - 50, node.centerY);
                            delay(2);
                            break;
                        }
                    }
                    continue;
                }

            }
            if (screen == "TIK_LOGIN") {
                if (!tap("//*[@text='Don’t have an account? Sign up']", xml))
                    tap("//*[@text='Use phone / email / username']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP") {
                tap("//*[@text='Use phone or email']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_LOGIN_PHONE" || screen == "TIK_LOGIN_EMAIL") {
                maxAgain--;
                if (maxAgain < 0)
                    break;

                performAction(Perform::BACK, 2, 1);

                //tap("//*[@content-desc='Back to previous screen']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP_PHONE") {
                tap("//*[@text='Email' or @text='Email / Username']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP_EMAIL") {
                if (existNodes(0, xml, {"//*[@text='No internet connection']",
                                        "//*[@text='Please try again or log in with a different method.']"
                }) !=
                    "")
                    break;

                Node nodeInput = getNode("//android.widget.EditText", xml);
                if (nodeInput.text == "" || Common::strContains(nodeInput.text, " ")) {
                    inputText(nodeInput, email);
                    delay(1);
                    continue;
                }

                if (existNode(
                        "//*[@text='@gmail.com' or @text='@hotmail.com' or @text='@outlook.com']",
                        xml)) {
                    performAction(Perform::BACK, 1, 2);

                    continue;
                }

                string xpath = existNodes(0, xml, {
                        "//*[@text='Next' or @text='Continue']",
                        "(//*[@text='Log in'])[2]",
                });
                if (xpath.empty()) {
                    delay(2);
                    continue;
                }

                Node node = getNode(xpath, xml);

                tap(node);

//                tap(Common::ranNumber(node.left, node.right),
//                    Common::ranNumber(node.top, node.top + 30));
                delay(2);
                continue;
            }
            if (existNode("//*[contains(text(), 'Incorrect account or password')]", xml)) {
                performAction(Perform::BACK, 2, 1);
                continue;
            }
            if (screen == "TIK_LOGIN_INCORRECT_PASSWORD") {

                //tap("//*[@content-desc='Back to previous screen']", xml);
                //delay(2);
                continue;
            }
            if (screen == "TIK_LOGIN_EXIST_ACCOUNT") {
                maxAgain--;
                if (maxAgain < 0)
                    break;

                tap("(//*[@text='Profile'])[last()]", xml);
                delay(3);
                continue;

                //tap("//*[@text='Profile']/following-sibling::*//android.widget.ImageView", xml);
                //delay(2);
                //continue;
            }
            if (screen == "TIK_SIGN_UP_PASS") {
                inputText("//android.widget.EditText", password);
                delay(1);

                xml = "";
                if (!existNode("//*[@text='Next' or @text='Continue']", xml)) {
                    performAction(Perform::BACK, 1, 1);
                    delay(1);
                    xml = "";
                }

                Node node = getNode("//*[@text='Next' or @text='Continue']", xml);
                if (node.empty())
                    continue;
                tap(Common::ranNumber(node.left, node.right),
                    Common::ranNumber(node.top, node.top + 30));
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP_BIRTHDAY") {
                maxInputBirthday--;
                if (maxInputBirthday < 0)
                    break;

                string xpathDMY = "//*[@text='Next']/following-sibling::*[last()]/child::*/child::*";

                for (int i = 1; i <= 3; ++i) {
                    Node node = getNode(xpathDMY + "[" + to_string(i) + "]", xml);

                    int countScroll = Common::ranNumber(2, 5);
                    if (i == 3)
                        countScroll += 20;
                    for (int j = 0; j < countScroll; ++j) {
                        swipe(Common::ranNumber(node.left, node.right),
                              Common::ranNumber(node.top, node.top + 20),
                              Common::ranNumber(node.left, node.right),
                              Common::ranNumber(node.bottom, node.bottom - 20), 500);
                        delay(1);
                    }
                }

                delay(delayBirthdayFrom, delayBirthdayTo, "delay birthday");
                int ranNumberDMY = Common::ranNumber(1, 3);
                Node node = getNode(xpathDMY + "[" + to_string(ranNumberDMY) + "]",
                                    xml);
                int randNumber = Common::ranNumber(1, 2);
                int randDuration = Common::ranNumber(10, 200);
                for (int i = 0; i < randNumber; ++i) {
                    swipe(Common::ranNumber(node.left, node.right),
                          Common::ranNumber(node.top, node.top + 20),
                          Common::ranNumber(node.left, node.right),
                          Common::ranNumber(node.bottom, node.bottom - 20),
                          randDuration);
                }
                delay(Common::ranNumber(5, 10), "chờ bấm next");
                if (tap("//*[@text='Next']"))
                    delay(10);
                isRegDone = true;
                continue;
            }
            if (screen == "TIK_SIGN_UP_VERIFY_EMAIL") {
                if (existNode(
                        "//*[@text='Maximum number of attempts reached. Try again later.']",
                        xml)) {
                    output.set("error", "maximum_attempts_reached");
                    break;
                }

                output.set("error", "verify_email");
                break;

                //string otp = "";
                //if (otp.empty()) {
                //    tap("//*[@text='Resend code']", xml);
                //    delay(2);
                //    continue;
                //}
                //
                //inputText("//android.widget.EditText", otp);
                //delay(2);
                //continue;
            }
            if (screen == "TIK_SIGN_UP_NICKNAME") {
                if (name.empty()) {
                    tap("//*[@text='Skip']", xml);
                    delay(2);
                    continue;
                }

                inputText("//android.widget.EditText", name);
                delay(1);
                tap("//*[@text='Confirm']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_SIGN_UP_EXIST_ACCOUNT") {
                output.set("error", "account_exists");
                break;
            }
            if (screen == "TIK_SIGN_UP_BLOCK") {
                output.set("error", "block");
                break;
            }

            string xpath = existNodes(0, xml, {
                    "//*[@text='Sign up']",
                    "//*[@text='Switch account']",
                    "//*[@text='Use authenticator app']"
            });
            if (xpath == "//*[@text='Sign up']") {
                tap(xpath, xml);
                delay(2);
                continue;
            }
            if (xpath == "//*[@text='Switch account']") {
                if (tap("//*[@text='Add account']", xml)) {
                    delay(2);
                    continue;
                }

                if (swipeAndScreenNotChange()) {
                    performAction(Perform::BACK, 1, 1);
                    handleLogout();

                }
                continue;
            }
            if (xpath == "//*[@text='Use authenticator app']") {
                tap(xpath, xml);
                delay(2);
                continue;
            }

            delay(1);
        }

        return output;
    }

    void handleScanAccountInDevice() {
        if (!setupPhone({}))
            return;

        if (!setupNetwork()) {
            return;
        }
        bool startScan = false;

        if (!openSocial(true)) {
            updateLine("Scan account error : Open Package Fail");
            return;
        }

        string xml;
        for (int loop = 50; loop > 0; loop--) {
            if (isStopped()) return;

            updateLine("Scan account -" + to_string(loop));
            delay(1);
            xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") {
                updateLine("Scan account failed");
                return;
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

                vector<Node> lstNodes = getNodes(
                        "//android.widget.FrameLayout/android.view.ViewGroup/android.widget.LinearLayout/child::*",
                        xml);
                if (lstNodes.empty()) continue;
//
                for (int i = lstNodes.size() - 1; i >= 0; i--) {
                    Node node = lstNodes.at(i);
                    if (node.width > node.height) {
                        tap(node.right - 50, node.bottom - Common::ranNumber(5, 10));
                        delay(2);
                        xml = dumpXml();
                        if (!existNode("//*[@text='Switch account']", xml, 3))
                            performAction(Perform::BACK, 1, 1);
                        else
                            startScan = true;

                        delay(2);
                        break;
                    }
                }

                if (!startScan) {
                    xml = dumpXml();
                    string xpathSwitchAccount = existNodes(0, xml, {
                            "//*[@resource-id='com.ss.android.ugc.trill:id/jwa' or @resource-id='com.ss.android.ugc.trill:id/jzf' or @resource-id='com.ss.android.ugc.trill:id/k76' or @resource-id='com.ss.android.ugc.trill:id/k81' or @resource-id='com.zhiliaoapp.musically:id/jko' or @resource-id='com.ss.android.ugc.trill:id/kfe' or @resource-id='com.ss.android.ugc.trill:id/kg0']",
                            "//*[@class='android.widget.LinearLayout']//*[@class='android.widget.LinearLayout']//*[@class='android.widget.RelativeLayout']//*[@class='android.widget.LinearLayout']//*[@class='android.widget.FrameLayout']//*[@class='android.widget.Button']",
                    });

                    if (!xpathSwitchAccount.empty()) {
                        tap(xpathSwitchAccount);
                        delay(2);
                        if (!existNode("//*[@text='Switch account']", xml, 3))
                            performAction(Perform::BACK, 1, 1);
                    }
                    delay(2);
                }
//bắt đầu scan
                xml = dumpXml();
                vector<string> lstUid;
                if (existNode("//*[@text='Switch account']", xml, 10)) {
                    vector<Node> textValueLst = getNodes(
                            "//*[@class='android.widget.TextView' and string-length(@text) > 0 and not(contains(@text, 'switch account')) and not(contains(@text, 'add account'))]",
                            xml);
                    for (const Node &node: textValueLst) {
                        string uid = node.text;
                        lstUid.push_back(uid);
                    }
                }


                if (lstUid.empty()) {
                    updateLine("Not-found account in this device !!!");
                    return;
                }
                JSON jsonBody;
                jsonBody.set("lstExecute", lstUid);

                IResult response = OCRequest("tiktokapp/scan-account", jsonBody.toString());

                updateLine(
                        "Scan account " +
                        string(response.succeeded ? "success" : "failed") +
                        "!!!");
                return;
            }


            if (screen == "TIK_SIGN_UP"
                || screen == "TIK_LOGIN"
                || screen == "TIK_LOGIN_PHONE"
                || screen == "TIK_LOGIN_EMAIL"
                || screen == "TIK_NO_LOGIN"
                || screen == "TIK_LOGIN_EXIST_ACCOUNT") {
                updateLine("Not-found account in this device !!!");
                return;
            }
        }
    }

    ResultScreen handleAction() {
        int usedMemoryPercentage = getUsedMemoryPercentage();
        Common::NLog(to_string(usedMemoryPercentage), "usedMemoryPercentage");
        if (usedMemoryPercentage > 70)
            openSocial(true);

        if (action.scriptType == "READ_NOTIFICATIONS")
            return actionDocThongBao();
        if (action.scriptType == "REACT_VIDEO_RANDOM")
            return actionTuongTacVideoRandom();
        if (action.scriptType == "REACT_VIDEO_UID")
            return actionTuongTacVideoChiDinh();
        if (action.scriptType == "REACT_VIDEO_KEYWORD")
            return actionTuongTacVideoTuKhoa();
        if (action.scriptType == "REACT_VIDEO_USER")
            return actionTuongTacVideoUser();
        if (action.scriptType == "REACT_LIVE_RANDOM")
            return actionTuongTacLiveRandom();
        if (action.scriptType == "REACT_LIVE_CHIDINH")
            return actionTuongTacLiveChiDinh();
        if (action.scriptType == "FOLLOW_USER_UID")
            return actionFollowUserChiDinh();
        if (action.scriptType == "FOLLOW_USER_KEYWORD")
            return actionFollowUserTheoTuKhoa();
        if (action.scriptType == "FOLLOW_USER_SUGGEST")
            return actionFollowUserTheoGoiY();
        if (action.scriptType == "FOLLOW_BACK")
            return actionFollowBack();
        if (action.scriptType == "MESSAGE_UID")
            return actionNhanTinChiDinh();
        if (action.scriptType == "BUFF_COMMENT")
            return actionBuffTopComment();
        if (action.scriptType == "BUFF_VIEW_PRODUCT")
            return actionViewProductTiktokShopByLink();
        if (action.scriptType == "EARN_MONEY")
            return actionEarnMoney();
        if (action.scriptType == "FOLLOW_UID_PROFILE")
            return actionFollowUserByProfile();
        if (action.scriptType == "UP_POST")
            return actionUpPost();
        if (action.scriptType == "UP_AVATAR")
            return actionUpAvatar();
        if (action.scriptType == "CHANGE_BIO")
            return actionChangeBio();
        if (action.scriptType == "CHANGE_NAME")
            return actionChangeName();
        if (action.scriptType == "PROFILE_PUBLIC_LIKE")
            return actionPublicLike();
        if (action.scriptType == "SLEEP")
            return actionNghiGiaiLao();
        if (action.scriptType == "REMOVE_POST_ON_WALL")
            return actionRemovePostOnWall();
        return NOMAP;
    }

    ResultScreen actionChangeBio() {
        //input
        string postContentId = action.getString("postContentId");

        updateLine("Change Bio");
        string xml = dumpXml();
        gotoHome(xml);
        //code here
        for (int loop = 50; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            xml = dumpXml();
            string screen = handleScreen(xml);

            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }

            if (screen == "TIK_PROFILE") {
                tap("//*[@text='Edit profile']");
                delay(1);
                continue;
            }

            if (screen == "TIK_PROFILE_EDIT") {
                tap("//*[contains(@content-desc, 'Bio')]");
                delay(1, 2);
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
                tap("//*[@content-desc='Save']");
                delay(1, 2);
                updateLine("set Bio Success");
                return DONE;
            }
        }
        return NOMAP;
    }

    ResultScreen actionChangeName() {
        //input
        string name;

        int cbbTypeName = action.getInt(
                "cbbTypeName");//0-ngẫu nhiên, 1-chỉ định 1, 2-chỉ định 2
        if (cbbTypeName == 0) {
            int typeName = action.getInt(
                    "typeName");//ngẫu nhiên: 0-tên việt, 1-tên ngoại

            name = typeName == 0 ? Common::ranNameViet() : Common::ranNameNgoai();
        } else if (cbbTypeName == 1) {
            vector<string> lstFirstName = action.getList("lstFirstName");
            vector<string> lstMiddleName = action.getList("lstMiddleName");
            vector<string> lstLastName = action.getList("lstLastName");

            name = Common::ranItem(lstFirstName) + " " + Common::ranItem(lstMiddleName) +
                   " " +
                   Common::ranItem(lstLastName);
            name = Common::strReplace(name, "  ", " ");
        } else {
            vector<string> lstFullName = action.getList("lstFullName");
            name = Common::ranItem(lstFullName);
        }

        //code here
        for (int loop = 50; loop > 0; loop--) {
            if (isStopped())
                return ERROR;
            string xml = dumpXml();
            string screen = handleScreen(xml);

            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }

            if (screen == "TIK_PROFILE") {
                tap("//*[@text='Edit profile']");
                delay(1);
                continue;
            }

            if (screen == "TIK_PROFILE_EDIT") {
                tap("(//*[contains(@content-desc, 'Name')])[1]");
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_CHANGE_NAME") {
                inputText("//android.widget.EditText", name);
                delay(1);
                tap("//*[@content-desc='Save']");
                delay(1, 2);
                continue;
            }
            if (screen == "TIK_BLOCK_CHANGE_NAME") {
                performAction(Perform::BACK, 1, 1);
                updateLine("Block change name");
                return NOMAP;
            }
            if (screen == "TIK_SYNC_NAME") {
                string xpathCheckBox = existNodes(0, xml, {
                        "//android.widget.CheckBox[@checked='true']"
                });
                if (existNode(xpathCheckBox, xml, 3)) {
                    tap(xpathCheckBox);
                    delay(1, 2);
                }

                tap("//*[@text='Confirm']");
                updateLine("Edit done");
                delay(1, 2);
                return DONE;
            }
        }
        return NOMAP;
    }

    ResultScreen actionFollowBack() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //code here
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (screen == "ERROR") return ERROR;
        if (screen != "TIK_HOME" && screen != "TIK_PROFILE")
            gotoHome(xml);

        int countSuccess = 0;
        int total = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);

        string status =
                "Follow (" + to_string(countSuccess + 1) + "/" + to_string(total) + ") ";
        while (!isStopped()) {
            updateLine(status + "Checking");

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                string xpathFollower = existNodes(0, xml, {
                        "//*[@text='Followers']",
                        "//*[@text='Follower']"
                });

                tap(xpathFollower, xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_PROFILE_FOLLOWERS") {
                if (!tap("//*[@text='Follow back']", xml)) {
                    updateLine(status + "Swipe");
                    if (swipeAndScreenNotChange())
                        break;
                    continue;
                }

                delay(nudDelayFrom, nudDelayTo, status + "delay");

                countSuccess++;
                if (countSuccess >= total)
                    return DONE;
                status = "Follow (" + to_string(countSuccess + 1) + "/" + to_string(total) +
                         ") ";
                continue;
            }
            if (screen == "TIK_ANOTHER_PROFILE") {
                performAction(Perform::BACK, 1, 1);
                delay(2);
                continue;
            }

            updateLine(status + "Swipe");
            if (swipeAndScreenNotChange())
                break;
            delay();
        }

        return NOMAP;
    }

    ResultScreen actionFollowUserTheoGoiY() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //code here
        string xml = dumpXml();
        string screen = handleScreen(xml);
        if (screen == "ERROR") return ERROR;
        if (screen != "TIK_HOME" && screen != "TIK_PROFILE")
            gotoHome(xml);

        int countSuccess = 0;
        int total = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);

        string status =
                "Follow (" + to_string(countSuccess + 1) + "/" + to_string(total) + ") ";
        while (!isStopped()) {
            updateLine(status + "Checking");

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                string xpathFollower = existNodes(0, xml, {
                        "//*[@text='Followers']",
                        "//*[@text='Follower']"
                });
                tap(xpathFollower, xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_PROFILE_FOLLOWERS" || screen == "TIK_PROFILE_FOLLOWING") {
                if (!tap("//*[@text='Suggested']", xml))
                    tap("//*[contains(@text,'Friends ')]", xml);

                delay(2);
                continue;
            }
            if (screen == "TIK_PROFILE_FRIENDS") {
                tap("//*[@text='Suggested']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_PROFILE_SUGGESTED") {
                if (!tap("//*[@text='Follow' or @text='Follow back']", xml)) {
                    updateLine(status + "Swipe");
                    if (swipeAndScreenNotChange())
                        break;
                    continue;
                }

                delay(nudDelayFrom, nudDelayTo, status + "delay");

                countSuccess++;
                if (countSuccess >= total)
                    return DONE;
                status = "Follow (" + to_string(countSuccess + 1) + "/" + to_string(total) +
                         ") ";
                continue;
            }
            if (screen == "TIK_ANOTHER_PROFILE") {
                performAction(Perform::BACK, 1, 1);
                delay(2);
                continue;
            }

            updateLine(status + "Swipe");
            if (swipeAndScreenNotChange())
                break;
            delay();
        }

        return NOMAP;
    }

    ResultScreen actionFollowUserTheoTuKhoa() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");
        vector<string> lstTuKhoa = action.getList("txtTuKhoa");

        //code here
        string tuKhoa = Common::ranItem(lstTuKhoa);
        ResultScreen result = searchTiktok(tuKhoa, "Users");
        if (result != DONE)
            return result;
        delay(3);

        int total = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        string xml;
        string screen;
        string status;

        while (!isStopped()) {
            status = "Follow (" + to_string(countSuccess + 1) + "/" + to_string(total) +
                     ") ";
            updateLine(status + "Checking");

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (!tap("//*[@text='Follow']", xml)) {
                updateLine(status + "Swipe");
                if (swipeAndScreenNotChange())
                    break;
                continue;
            }

            delay(nudDelayFrom, nudDelayTo, status + "delay");

            countSuccess++;
            if (countSuccess >= total)
                return DONE;
        }

        return NOMAP;
    }

    ResultScreen actionTuongTacLiveChiDinh() {
        //input
        string txtLink = action.getString("txtLink");

        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");
        int nudDelayInteractFrom = action.getInt("nudDelayInteractFrom");
        int nudDelayInteractTo = action.getInt("nudDelayInteractTo");

        bool ckbFollow = action.getBool("ckbFollow");
        int nudPercentFollow = action.getInt("nudPercentFollow");
        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int nudDelayCommentFrom = action.getInt("nudDelayCommentFrom");
        int nudDelayCommentTo = action.getInt("nudDelayCommentTo");
        string commentContentId = action.getString("commentContentId");

        bool ckbShareLive = action.getBool("ckbLiveS");

        int nudPercentShare = action.getInt("nudPercentLiveS");
        int nudDelayShareFrom = action.getInt("nudDelayLiveSFrom");
        int nudDelayShareTo = action.getInt("nudDelayLiveSTo");

        bool ckbTuongTacLienTuc = action.getBool("ckbTuongTacLienTuc");
        int nudDelayFrom = action.getInt("nudDelayFrom");
        int nudDelayTo = action.getInt("nudDelayTo");

        //code here
        if (commentContentId.empty())
            ckbComment = false;
        string xml;
        string screen;
        string status;
        int countVideo = 0;

        //open live
        ResultScreen rScreen = gotoLive(txtLink, status + "Open live");
        if (rScreen != DONE)
            return rScreen;

        auto timeStart = high_resolution_clock::now();
        int timeout = Common::ranNumber(nudTimeFrom, nudTimeTo);

        while (!isStopped()) {
            xml = dumpXml();

            string screenTemp = detectScreen(xml);
            if (screenTemp == "TIK_CAPTCHA_DRAG_SLIDE" ||
                screenTemp == "TIK_CAPTCHA_DRAG" ||
                screenTemp == "TIK_CAPTCHA_SELECT" ||
                screenTemp == "TIK_CAPTCHA_POPUP_ERROR") {
                performAction(Perform::BACK, 1, 1);
                delay(5, "Close captcha");
                xml = dumpXml();
                continue;
            }

            screen = handleScreen(xml);

            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeLive(xml))
                    delay(nudDelayInteractFrom, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");

                if (commentLive(commentContentId, xml)) {
                    delay(nudDelayCommentFrom, nudDelayCommentTo,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollow && Common::ranBool(nudPercentFollow)) {
                updateLine(status + "Follow...");
                if (followLive(xml)) {
                    delay(nudDelayFollowFrom, nudDelayFollowTo,
                          status + "Follow done, delay");
                    xml = "";
                }
                ckbFollow = false;
            }
            if (ckbShareLive && Common::ranBool(nudPercentShare)) {
                updateLine(status + "Sharing...");
                if (shareLiveStream(xml)) {
                    delay(nudDelayShareFrom, nudDelayShareTo, status + "Share done, delay");
                    xml = "";
                }
            }


            int remainingTime = timeout - Common::getElapsedTime(timeStart);
            if (!ckbTuongTacLienTuc) {
                delay(remainingTime, status + "Watch live, delay");
                break;
            }

            if (remainingTime <= 0)
                break;
            int delayTime = Common::findMin(
                    {remainingTime, Common::ranNumber(nudDelayFrom, nudDelayTo)});
            delay(delayTime, "Watch live, delay");
        }

        return NOMAP;
    }

    ResultScreen actionTuongTacLiveRandom() {
        //input
        int cbbOptionsPost = action.getInt("cbbOptionsPost");
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                        action.getInt("nudSoLuongTo"));
        int thoiGian = Common::ranNumber(action.getInt("nudThoiGianFrom"),
                                         action.getInt("nudThoiGianTo"));

        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");
        int nudDelayInteractFrom = action.getInt("nudDelayInteractFrom");
        int nudDelayInteractTo = action.getInt("nudDelayInteractTo");

        bool ckbFollow = action.getBool("ckbFollow");
        int nudPercentFollow = action.getInt("nudPercentFollow");
        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int nudDelayCommentFrom = action.getInt("nudDelayCommentFrom");
        int nudDelayCommentTo = action.getInt("nudDelayCommentTo");
        string commentContentId = action.getString("commentContentId");

        bool ckbShareLive = action.getBool("ckbLiveS");

        int nudPercentShare = action.getInt("nudPercentLiveS");
        int nudDelayShareFrom = action.getInt("nudDelayLiveSFrom");
        int nudDelayShareTo = action.getInt("nudDelayLiveSTo");
        //code here
        if (commentContentId.empty())
            ckbComment = false;
        string screen;
        string status;
        int countVideo = 0;
        auto start = high_resolution_clock::now();

        string xml = dumpXml();
        gotoHome(xml);

        while (!isStopped()) {
            status = "Live (" + to_string(countVideo + 1) + ") ";
            updateLine(status + "Checking...");

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen != "TIK_LIVE") {
                ResultScreen rScreen = gotoHome(xml);
                if (rScreen != DONE)
                    return rScreen;

                xml = dumpXml();
                string xpath = existNodes(0, xml, {
                        "(//android.widget.HorizontalScrollView/preceding::android.widget.ImageView)[last()]",
                        "(//android.widget.HorizontalScrollView/parent::*/following-sibling::*)[1]"
                });
                if (xpath != "" && tap(xpath, xml)) {
                    xml = dumpXml();
                    if (!existNode("//*[@text='Gift']", xml, 30)) {
                        updateLine("Can't open livestream");
//                        saveStatusAcc("Reg", "Open Package Fail");
                        return NOMAP;

                    }
                }
//                    delay(10, "Wait Loading Video Live: ");
                continue;
            }

            if (countVideo > 0) {
                updateLine(status + "Swipe...");
                swipe(1, Common::ranNumber(1, 4));

                xml = dumpXml();
            }

            delay(nudTimeFrom, nudTimeTo, status + "Watch video");
            delay(nudTimeFrom, nudTimeTo, status + "Watch video");

            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeLive(xml))
                    delay(nudDelayInteractFrom, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");

                if (commentLive(commentContentId, xml)) {
                    delay(nudDelayCommentFrom, nudDelayCommentTo,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollow && Common::ranBool(nudPercentFollow)) {
                updateLine(status + "Follow...");
                if (followLive(xml)) {
                    delay(nudDelayFollowFrom, nudDelayFollowTo,
                          status + "Follow done, delay");
                    xml = "";
                }
            }

            if (ckbShareLive && Common::ranBool(nudPercentShare)) {
                updateLine(status + "Sharing...");
                if (shareLiveStream(xml)) {
                    delay(nudDelayShareFrom, nudDelayShareTo, status + "Share done, delay");
                    xml = "";
                }
            }
            countVideo++;
            bool isDone =
                    cbbOptionsPost == 0 ? countVideo >= soLuong
                                        : Common::hasExceededTimeout(
                            start,
                            thoiGian);
            if (isDone)
                return DONE;
        }

        return NOMAP;
    }

    ResultScreen actionNghiGiaiLao() {
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");

        int delayCount = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        while (delayCount > 0 && !isStopped()) {
            updateLine("Sleep - " + to_string(delayCount));
            delay(1);
            delayCount--;
        }
        return NOMAP;
    }

    ResultScreen actionUpAvatar() {
        updateLine("Up avatar...");

        string galleryId = action.getString("galleryId");
        bool ckbSkipIfHave = action.getBool("ckbSkipIfHave");
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");

        vector<MediaS3> lstMedia;

        string xml;

        string screen = handleScreen(xml);
        if (screen == "ERROR") return ERROR;
        if (screen != "TIK_HOME" && screen != "TIK_PROFILE")
            gotoHome(xml);

        for (int loop = 30; loop > 0; loop--) {
            if (isStopped())
                break;

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                string xpathAvatar = existNodes(0, xml, {
                        "//*[contains(@text,'@')]/preceding::android.widget.ImageView[1]/preceding-sibling::*[1]/*[@text='Add']",//chưa có avatar
                        "//*[@content-desc='Profile photo']/*[@text='Add']",//chưa có avatar
                        "//*[contains(@text,'@')]/preceding::android.widget.ImageView[1]",//đã có avatar
                });
                if (xpathAvatar ==
                    "//*[contains(@text,'@')]/preceding::android.widget.ImageView[1]" &&
                    ckbSkipIfHave)
                    return DONE;

                tap("//*[@text='Edit profile' or @text='set up profile']", xml);
                delay(2);
                continue;
            }
            if (screen == "TIK_PROFILE_EDIT") {
                lstMedia = getScriptMedia(galleryId);

                if (lstMedia.empty())
                    return ERROR;

                string xpath = existNodes(0, xml, {
                        "//*[@text='Change photo']/preceding::*[1]",
                        "//android.widget.ScrollView//android.widget.ImageView[2]",
                        "//*[@text='Edit photo or avatar' or @content-desc='Edit photo or avatar']",
                });
                tap(xpath, xml);
                delay(2);
                continue;
            }

            string xpath = existNodes(0, xml, {
                    "//*[@text='Select from Gallery']",
                    "//*[@text='Upload photo']",
                    "//*[@text='All media']",
                    Common::strReplace("//*[@text='{xpath}']", "{xpath}", PACKAGE),
                    "//*[@text='Crop']",
            });
            if (xpath == "//*[@text='Select from Gallery']" ||
                xpath == "//*[@text='Upload photo']") {
                tap(xpath, xml);
                delay(2);
                continue;
            }
            if (xpath == "//*[@text='All media']") {
                tap("//android.widget.ImageView/following-sibling::*", xml);
                delay(1);

                xml = "";
                if (existNode("//*[@text='Image size is too small']", xml)) {
                    tap("//*[@text='OK']", xml);
                    delay(1);
                    performAction(Perform::BACK);
                    continue;
                }

                tap("//*[@text='Confirm']", xml);
                delay(2);
                continue;
            }


            if (xpath == "//*[@text='Crop']") {
                if (tap("//android.widget.CheckBox[@checked='true']"))
                    delay(1);

                if (tap("//*[@text='Save & post' or @text='Save']", xml)) {
                    delay(3);

                    if (waitNodeHide("//*[@text='Uploading...']", 60))
                        delay(2);

                    if (ckbXoaAnhDaDung)
                        deleteScriptMedia(lstMedia);

                    return DONE;
                }
            }
            if (xpath.empty()) {
                if (existNode("//*[@text='Back']", xml) &&
                    existNode("//*[@text='Confirm']", xml)) {
                    tap("//*[@text='Back']", xml);
                    delay(1);

                    lstMedia = getScriptMedia(galleryId);

                    if (lstMedia.empty())
                        return ERROR;

                    continue;
                }
                if (existNode("//*[@text='All']", xml)) {
                    performAction(Perform::BACK, 1, 1);
                    delay(1);

                    lstMedia = getScriptMedia(galleryId);

                    if (lstMedia.empty())
                        return ERROR;

                    continue;
                }
            }
            delay(1);
        }

        return NOMAP;
    }

    ResultScreen actionTuongTacVideoChiDinh() {
        //input
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                        action.getInt("nudSoLuongTo"));
        vector<string> lstLink = action.getList("txtLink");

        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");
        int nudDelayInteractFrom = action.getInt("nudDelayInteractFrom");
        int nudDelayInteractTo = action.getInt("nudDelayInteractTo");

        bool ckbFollow = action.getBool("ckbFollow");
        int nudPercentFollow = action.getInt("nudPercentFollow");
        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        bool ckbAddFavorites = action.getBool("ckbAddFavorites");
        int nudPercentAddFavorites = action.getInt("nudPercentAddFavorites");
        int nudDelayAddFavoritesFrom = action.getInt("nudDelayAddFavoritesFrom");
        int nudDelayAddFavoritesTo = action.getInt("nudDelayAddFavoritesTo");

        bool ckbRepost = action.getBool("ckbRepost");
        int nudPercentRepost = action.getInt("nudPercentRepost");
        int nudDelayRepostFrom = action.getInt("nudDelayRepostFrom");
        int nudDelayRepostTo = action.getInt("nudDelayRepostTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int nudDelayCommentFrom = action.getInt("nudDelayCommentFrom");
        int nudDelayCommentTo = action.getInt("nudDelayCommentTo");
        string commentContentId = action.getString("commentContentId");

        bool ckbTagName = action.getBool("ckbTagName");
        string tagName = action.getString("tagName");

        //code here
        if (commentContentId.empty())
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
            delay(nudTimeFrom, nudTimeTo, status + "Watch video");

            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(nudDelayInteractFrom, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");

                if (commentVideo(commentContentId, xml, ckbTagName, tagName)) {
                    delay(nudDelayCommentFrom, nudDelayCommentTo,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollow && Common::ranBool(nudPercentFollow)) {
                updateLine(status + "Follow...");
                if (followVideo(xml)) {
                    delay(nudDelayFollowFrom, nudDelayFollowTo,
                          status + "Follow done, delay");
                    xml = "";
                }
            }

            if (ckbRepost && Common::ranBool(nudPercentRepost)) {
                updateLine(status + "Repost...");
                if (repostVideo(xml)) {
                    delay(nudDelayRepostFrom, nudDelayRepostTo,
                          status + "Repost done, delay");
                    xml = "";
                    if (countVideo == 0)
                        handleScreen(xml);
                }
            }

            if (ckbAddFavorites && Common::ranBool(nudPercentAddFavorites)) {
                updateLine(status + "Add Favorites...");
                if (addFavoritesVideo(xml))
                    delay(nudDelayAddFavoritesFrom, nudDelayAddFavoritesTo,
                          status + "Add Favorites done, delay");
            }

            countVideo++;
            if (countVideo >= soLuong)
                return DONE;

            if (lstLink.empty())
                break;
            link = Common::ranItem(lstLink);
        }

        return NOMAP;
    }

    void swipeVideo() {
        int typeScroll = Common::ranBool(90) ? 1 : -1;
        if (typeScroll == 1)
            swipe(typeScroll, Common::ranNumber(1, 2), 250);
        else
            swipe(typeScroll, Common::ranNumber(1, 2), 10);
        delay(1, 2);
    }

    ResultScreen actionTuongTacVideoRandom() {
        //input

        Common::NLog(action.jsonConfig.toString(), "actionTuongTacVideoRandom");

        int cbbOptionsPost = action.getInt("cbbOptionsPost");
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                        action.getInt("nudSoLuongTo"));

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

        bool ckbFollow = action.getBool("ckbFollow");
        int nudPercentFollow = action.getInt("nudPercentFollow");
        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        bool ckbAddFavorites = action.getBool("ckbAddFavorites");
        int nudPercentAddFavorites = action.getInt("nudPercentAddFavorites");
        int nudDelayAddFavoritesFrom = action.getInt("nudDelayAddFavoritesFrom");
        int nudDelayAddFavoritesTo = action.getInt("nudDelayAddFavoritesTo");

        bool ckbRepost = action.getBool("ckbRepost");
        int nudPercentRepost = action.getInt("nudPercentRepost");
        int nudDelayRepostFrom = action.getInt("nudDelayRepostFrom");
        int nudDelayRepostTo = action.getInt("nudDelayRepostTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int nudDelayCommentFrom = action.getInt("nudDelayCommentFrom");
        int nudDelayCommentTo = action.getInt("nudDelayCommentTo");
        string commentContentId = action.getString("commentContentId");

        bool ckbTagName = action.getBool("ckbTagName");
        string tagName = action.getString("tagName");

        //tuong tác nâng cao = là tên của TIKTOK không phải username
        vector<string> lstUser = action.getList("txtUserPro");
        //vector <string> lstUser = {"Máy Tính Bảng","Phim Cực Ngắn","Phạm Duy Anh"};
        string xpathlstFind = "//*[contains(@text,";

        for (string item: lstUser) {
            xpathlstFind += "'" + item + "') or contains(@text,";

        }
        if (Common::strEndsWith(xpathlstFind, "or contains(@text,")) {
            xpathlstFind = xpathlstFind.substr(0, xpathlstFind.length() - 19);
        }
        xpathlstFind = xpathlstFind + "]";
        xpathlstFind = Common::strToLower(xpathlstFind);

        int cbbOptionsPostPro = action.getInt("cbbOptionsPostPro");
        int soLuongPro = Common::ranNumber(action.getInt("nudSoLuongFromPro"),
                                           action.getInt("nudSoLuongToPro"));
        int thoiGianPro = Common::ranNumber(action.getInt("nudThoiGianFromPro"),
                                            action.getInt("nudThoiGianToPro"));


        int nudTimeFromPro = action.getInt("nudTimeFromPro");
        int nudTimeToPro = action.getInt("nudTimeToPro");

        bool ckbInteractPro = action.getBool("ckbInteractPro");
        int nudPercentInteractPro = action.getInt("nudPercentInteractPro");
        int nudDelayInteractFromPro = action.getInt("nudDelayInteractFromPro");
        int nudDelayInteractToPro = action.getInt("nudDelayInteractToPro");

        bool ckbFollowPro = action.getBool("ckbFollowPro");
        int nudPercentFollowPro = action.getInt("nudPercentFollowPro");
        int nudDelayFollowFromPro = action.getInt("nudDelayFollowFromPro");
        int nudDelayFollowToPro = action.getInt("nudDelayFollowToPro");

        bool ckbAddFavoritesPro = action.getBool("ckbAddFavoritesPro");
        int nudPercentAddFavoritesPro = action.getInt("nudPercentAddFavoritesPro");
        int nudDelayAddFavoritesFromPro = action.getInt("nudDelayAddFavoritesFromPro");
        int nudDelayAddFavoritesToPro = action.getInt("nudDelayAddFavoritesToPro");

        bool ckbRepostPro = action.getBool("ckbRepostPro");
        int nudPercentRepostPro = action.getInt("nudPercentRepostPro");
        int nudDelayRepostFromPro = action.getInt("nudDelayRepostFromPro");

        int nudDelayRepostToPro = action.getInt("nudDelayRepostToPro");
        bool ckbCommentPro = action.getBool("ckbCommentPro");
        int nudPercentCommentPro = action.getInt("nudPercentCommentPro");
        int nudDelayCommentFromPro = action.getInt("nudDelayCommentFromPro");
        int nudDelayCommentToPro = action.getInt("nudDelayCommentToPro");
        string commentContentIdPro = action.getString("commentContentIdPro");

        int countVideoPro = 0;
        bool tuongTacUser = false;

        if (commentContentId.empty())
            ckbComment = false;
        string xml;
        string screen;
        string status;
        int countVideo = 0;
        auto start = high_resolution_clock::now();

        while (!isStopped()) {
            status = "Video (" + to_string(countVideo + 1) + ") ";
            updateLine(status + "Checking...");
            xml = dumpXml();

            screen = handleScreen(xml);

            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen != "TIK_HOME") {
                ResultScreen rScreen = gotoHome(xml);
                if (rScreen != DONE)
                    return rScreen;

                if (tap(vector<string>{"//*[@text='For You']", "//*[@text='Following']",
                                       "//*[@text='Friends']"
                }[cbbTuyChon]))
                    delay(2);
                xml = dumpXml();
            }

            if (countVideo > 0) {
                updateLine(status + "Swipe...");

                swipeVideo();
                xml = dumpXml();
                //tuongtacnangcao
                if (!lstUser.empty()) {
                    if (existNode(xpathlstFind, xml, 5)) {
                        ckbInteract = true;
                        ckbFollow = true;
                        tuongTacUser = true;
                        if (commentContentId.empty())
                            ckbComment = false;
                        else
                            ckbComment = true;
                        ckbAddFavorites = true;
                        ckbRepost = true;
                    }
                }

            }

            delay(nudTimeFrom, nudTimeTo, status + "Watch video");

            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(nudDelayInteractFrom, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");
//                if (lstContent_Clone.empty())
//                    lstContent_Clone = vector<string>(lstContent);
//                string content = Common::ranItem(lstContent_Clone);

                if (commentVideo(commentContentId, xml, ckbTagName, tagName)) {
                    //Common::removeItem(lstContent_Clone, content);
                    delay(nudDelayCommentFrom, nudDelayCommentTo,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollow && Common::ranBool(nudPercentFollow)) {
                updateLine(status + "Follow...");
                if (followVideo(xml)) {
                    delay(nudDelayFollowFrom, nudDelayFollowTo,
                          status + "Follow done, delay");
                    xml = "";
                }
            }

            if (ckbRepost && Common::ranBool(nudPercentRepost)) {
                updateLine(status + "Repost...");
                if (repostVideo(xml)) {
                    delay(nudDelayRepostFrom, nudDelayRepostTo,
                          status + "Repost done, delay");
                    xml = "";
                    if (countVideo == 0)
                        handleScreen(xml);
                }
            }

            if (ckbAddFavorites && Common::ranBool(nudPercentAddFavorites)) {
                updateLine(status + "Add Favorites...");
                if (addFavoritesVideo(xml))
                    delay(nudDelayAddFavoritesFrom, nudDelayAddFavoritesTo,
                          status + "Add Favorites done, delay");
            }
            //tuongtacnangcao
            if (!lstUser.empty() && tuongTacUser) {
                xml = dumpXml();
                tap("(//*[contains(@content-desc,'profile')])[1]", xml, 3);
                delay(1);
                goto tuongTacPro;
            }
            ketThucPro:
            //tuongtacnangcao
            countVideo++;
            bool isDone =
                    cbbOptionsPost == 0 ? countVideo >= soLuong
                                        : Common::hasExceededTimeout(start,
                                                                     thoiGian);
            if (isDone)
                return DONE;
        }
        tuongTacPro:
        for (int i = 0; i < 50; ++i) {
            status = "Video Pro (" + to_string(countVideoPro + 1) + ") ";
            updateLine(status + "Checking...");

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "TIK_ANOTHER_PROFILE") {
                if (!tapVideoFromProfile(xml))
                    return NOMAP;
            }
            if (countVideoPro > 0) {
                updateLine(status + "Swipe...");
                swipe();

                xml = dumpXml();
            }

            delay(nudTimeFrom, nudTimeTo, status + "Watch video");

            if (ckbInteractPro && Common::ranBool(nudPercentInteractPro)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(nudDelayInteractFromPro, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbCommentPro && Common::ranBool(nudPercentCommentPro)) {
                updateLine(status + "Comment...");
//                if (lstContent_Clone.empty())
//                    lstContent_ClonePro = vector<string>(lstContentPro);
//                string contentPro = Common::ranItem(lstContent_ClonePro);

                if (commentVideo(commentContentIdPro, xml)) {
                    // Common::removeItem(lstContent_ClonePro, contentPro);
                    delay(nudDelayCommentFromPro, nudDelayCommentToPro,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollowPro && Common::ranBool(nudPercentFollowPro)) {
                updateLine(status + "Follow...");
                if (followVideo(xml)) {
                    delay(nudDelayFollowFromPro, nudDelayFollowToPro,
                          status + "Follow done, delay");
                    xml = "";
                }
            }

            if (ckbRepostPro && Common::ranBool(nudPercentRepostPro)) {
                updateLine(status + "Repost...");
                if (repostVideo(xml)) {
                    delay(nudDelayRepostFromPro, nudDelayRepostToPro,
                          status + "Repost done, delay");
                    xml = "";
                    if (countVideoPro == 0)
                        handleScreen(xml);
                }
            }

            if (ckbAddFavoritesPro && Common::ranBool(nudPercentAddFavoritesPro)) {
                updateLine(status + "Add Favorites...");
                if (addFavoritesVideo(xml))
                    delay(nudDelayAddFavoritesFromPro, nudDelayAddFavoritesToPro,
                          status + "Add Favorites done, delay");
            }

            countVideoPro++;
            bool isDonePro =
                    cbbOptionsPostPro == 0 ? countVideoPro >= soLuongPro
                                           : Common::hasExceededTimeout(start,
                                                                        thoiGianPro);
            if (isDonePro) {
                performAction(Perform::BACK, 2, 1);
                swipeVideo();
                goto ketThucPro;
            }
        }
        //tuongtacnangcao
        return NOMAP;
    }

    ResultScreen actionTuongTacVideoTuKhoa() {
        int cbbOptionsPost = action.getInt("cbbOptionsPost");

        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                        action.getInt("nudSoLuongTo"));

        int thoiGian = Common::ranNumber(action.getInt("nudThoiGianFrom"),
                                         action.getInt("nudThoiGianTo"));

        vector<string> lstTuKhoa = action.getList("txtTuKhoa");

        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");
        int nudDelayInteractFrom = action.getInt("nudDelayInteractFrom");
        int nudDelayInteractTo = action.getInt("nudDelayInteractTo");

        bool ckbFollow = action.getBool("ckbFollow");
        int nudPercentFollow = action.getInt("nudPercentFollow");
        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        bool ckbAddFavorites = action.getBool("ckbAddFavorites");
        int nudPercentAddFavorites = action.getInt("nudPercentAddFavorites");
        int nudDelayAddFavoritesFrom = action.getInt("nudDelayAddFavoritesFrom");
        int nudDelayAddFavoritesTo = action.getInt("nudDelayAddFavoritesTo");

        bool ckbRepost = action.getBool("ckbRepost");
        int nudPercentRepost = action.getInt("nudPercentRepost");
        int nudDelayRepostFrom = action.getInt("nudDelayRepostFrom");
        int nudDelayRepostTo = action.getInt("nudDelayRepostTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int nudDelayCommentFrom = action.getInt("nudDelayCommentFrom");
        int nudDelayCommentTo = action.getInt("nudDelayCommentTo");
        string commentContentId = action.getString("commentContentId");

        bool ckbTagName = action.getBool("ckbTagName");
        string tagName = action.getString("tagName");

        //search
        string tuKhoa = Common::ranItem(lstTuKhoa);
        ResultScreen result = searchTiktok(tuKhoa, "Videos");
        if (result != DONE)
            return result;
        swipe(1, Common::ranNumber(1, 7));
        delay(5, "Search loading: ");
        string xml = dumpXml();
        string xpathVideo = existNodes(3, xml, {
                "//*[@resource-id=\"com.zhiliaoapp.musically:id/giy\"]/android.widget.FrameLayout[1]/android.widget.LinearLayout[1]/android.widget.FrameLayout[1]",
                "//androidx.recyclerview.widget.RecyclerView/android.widget.FrameLayout/android.widget.LinearLayout/android.widget.FrameLayout"
        });

        if (!tap(xpathVideo))
            return NOMAP;
        delay(3);

        //code here
        if (commentContentId.empty())
            ckbComment = false;
        string screen;
        string status;
        int countVideo = 0;
        auto start = high_resolution_clock::now();

        while (!isStopped()) {
            status = "Video (" + to_string(countVideo + 1) + ") ";
            updateLine(status + "Checking...");

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (countVideo > 0) {
                updateLine(status + "Swipe...");
                swipe();

                xml = dumpXml();
            }

            delay(nudTimeFrom, nudTimeTo, status + "Watch video");

            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(nudDelayInteractFrom, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");

                if (commentVideo(commentContentId, xml, ckbTagName, tagName)) {
                    delay(nudDelayCommentFrom, nudDelayCommentTo,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollow && Common::ranBool(nudPercentFollow)) {
                updateLine(status + "Follow...");
                if (followVideo(xml)) {
                    delay(nudDelayFollowFrom, nudDelayFollowTo,
                          status + "Follow done, delay");
                    xml = "";
                }
            }

            if (ckbRepost && Common::ranBool(nudPercentRepost)) {
                updateLine(status + "Repost...");
                if (repostVideo(xml)) {
                    delay(nudDelayRepostFrom, nudDelayRepostTo,
                          status + "Repost done, delay");
                    xml = "";
                    if (countVideo == 0)
                        handleScreen(xml);
                }
            }

            if (ckbAddFavorites && Common::ranBool(nudPercentAddFavorites)) {
                updateLine(status + "Add Favorites...");
                if (addFavoritesVideo(xml))
                    delay(nudDelayAddFavoritesFrom, nudDelayAddFavoritesTo,
                          status + "Add Favorites done, delay");
            }

            countVideo++;
            bool isDone =
                    cbbOptionsPost == 0 ? countVideo >= soLuong
                                        : Common::hasExceededTimeout(
                            start,
                            thoiGian);
            if (isDone)
                return DONE;
        }

        return NOMAP;
    }

    ResultScreen actionTuongTacVideoUser() {
        //input
        int cbbOptionsPost = action.getInt("cbbOptionsPost");
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                        action.getInt("nudSoLuongTo"));
        int thoiGian = Common::ranNumber(action.getInt("nudThoiGianFrom"),
                                         action.getInt("nudThoiGianTo"));
//        vector<string> lstUser = action.getList("txtUser");

        string userContentId = action.getString("userContentId");
        int nudTimeFrom = action.getInt("nudTimeFrom");
        int nudTimeTo = action.getInt("nudTimeTo");

        bool ckbInteract = action.getBool("ckbInteract");
        int nudPercentInteract = action.getInt("nudPercentInteract");
        int nudDelayInteractFrom = action.getInt("nudDelayInteractFrom");
        int nudDelayInteractTo = action.getInt("nudDelayInteractTo");

        bool ckbFollow = action.getBool("ckbFollow");
        int nudPercentFollow = action.getInt("nudPercentFollow");
        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        bool ckbAddFavorites = action.getBool("ckbAddFavorites");
        int nudPercentAddFavorites = action.getInt("nudPercentAddFavorites");
        int delayAddFavoritesFrom = action.getInt("nudDelayAddFavoritesFrom");
        int nudDelayAddFavoritesTo = action.getInt("nudDelayAddFavoritesTo");

        bool ckbRepost = action.getBool("ckbRepost");
        int nudPercentRepost = action.getInt("nudPercentRepost");
        int delayRepostFrom = action.getInt("nudDelayRepostFrom");
        int delayRepostTo = action.getInt("nudDelayRepostTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int delayCommentFrom = action.getInt("nudDelayCommentFrom");
        int delayCommentTo = action.getInt("nudDelayCommentTo");
        string commentContentId = action.getString("commentContentId");

        bool ckbTagName = action.getBool("ckbTagName");
        string tagName = action.getString("tagName");
        //search
        //string user = Common::ranItem(lstUser);

        //Get userId trước khi gotoProfile
        auto [success, content, itemId] = getScriptContent(userContentId);
        if (!success) {
            saveStatusAcc("Get User","Failed",5, Task::FAIL);
            return NOMAP;
        }
        ResultScreen rScreen = gotoProfile(content, "Go to profile");
        // Xóa nội dung nếu itemId không rỗng trước khi trả về
        if (!itemId.empty())
            deleteScriptContent(itemId);

        if (rScreen != DONE)
            return rScreen;

        if (!tapVideoFromProfile())
            return NOMAP;
        //code here
        if (commentContentId.empty())
            ckbComment = false;
        string xml;
        string screen;
        string status;
        int countVideo = 0;
        auto start = high_resolution_clock::now();

        while (!isStopped()) {
            status = "Video (" + to_string(countVideo + 1) + ") ";
            updateLine(status + "Checking...");

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (countVideo > 0) {
                updateLine(status + "Swipe...");
                swipe();

                xml = dumpXml();
            }

            delay(nudTimeFrom, nudTimeTo, status + "Watch video");

            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(nudDelayInteractFrom, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");

                if (commentVideo(commentContentId, xml, ckbTagName, tagName)) {
                    delay(delayCommentFrom, delayCommentTo,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollow && Common::ranBool(nudPercentFollow)) {
                updateLine(status + "Follow...");
                if (followVideo(xml)) {
                    delay(nudDelayFollowFrom, nudDelayFollowTo,
                          status + "Follow done, delay");
                    xml = "";
                }
            }

            if (ckbRepost && Common::ranBool(nudPercentRepost)) {
                updateLine(status + "Repost...");
                if (repostVideo(xml)) {
                    delay(delayRepostFrom, delayRepostTo,
                          status + "Repost done, delay");
                    xml = "";
                    if (countVideo == 0)
                        handleScreen(xml);
                }
            }

            if (ckbAddFavorites && Common::ranBool(nudPercentAddFavorites)) {
                updateLine(status + "Add Favorites...");
                if (addFavoritesVideo(xml))
                    delay(delayAddFavoritesFrom, nudDelayAddFavoritesTo,
                          status + "Add Favorites done, delay");
            }

            countVideo++;
            bool isDone =
                    cbbOptionsPost == 0 ? countVideo >= soLuong
                                        : Common::hasExceededTimeout(
                            start,
                            thoiGian);
            if (isDone)
                return DONE;
        }

        return NOMAP;
    }

    ResultScreen actionFollowUserChiDinh() {
        //input
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int delayFrom = action.getInt("nudDelayFrom");
        int delayTo = action.getInt("nudDelayTo");
        bool ckbMaxFollowFail = action.getBool("ckbMaxFollowFail");
        bool ckbOpenLinkBySearch = action.getBool("ckbOpenLink");
        int nudMaxFollowFail = action.getInt("nudMaxFollowFail");
        vector<string> lstUser = action.getList("txtId");

        bool openLinkBySearch = action.getBool("ckbOpenLink");
        //code here

        if (!ckbMaxFollowFail)
            nudMaxFollowFail = 9999;
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        string xml;
        string status;

        while (!isStopped()) {
            if (lstUser.empty())
                break;
            string user = Common::ranItem(lstUser);
            Common::removeItem(lstUser, user);

            status = action.actionName + " (" + to_string(countSuccess + 1) + "/" +
                     to_string(soLuong) + ") ";
            Task taskResult = followUser(user, status, openLinkBySearch);
            if (taskResult == Task::DONE) {
                countSuccess++;
                if (countSuccess >= soLuong)
                    break;
                delay(delayFrom, delayTo, status + "success");
                continue;
            }
            if (taskResult == Task::FAIL) {
                nudMaxFollowFail--;
                if (nudMaxFollowFail <= 0)
                    break;
            }
        }

        return NOMAP;
    }

    ResultScreen actionNhanTinChiDinh() {
        //input
        vector<string> lstUser = action.getList("txtId");
        int nudSoLuongFrom = action.getInt("nudSoLuongFrom");
        int nudSoLuongTo = action.getInt("nudSoLuongTo");
        int delayFrom = action.getInt("nudDelayFrom");
        int delayTo = action.getInt("nudDelayTo");
        bool ckbSendAnh = action.getBool("ckbSendAnh");
        string galleryId = action.getString("galleryId");
        string commentContentId = action.getString("commentContentId");

        //code here
        int soLuong = Common::ranNumber(nudSoLuongFrom, nudSoLuongTo);
        int countSuccess = 0;
        string xml;
        string status;

        while (!isStopped()) {
            if (lstUser.empty())
                break;
            string user = Common::ranItem(lstUser);
            Common::removeItem(lstUser, user);

            status = action.actionName + " (" + to_string(countSuccess + 1) + "/" +
                     to_string(soLuong) + ") ";

            //gotoProfile
            ResultScreen rScreen = gotoProfile(user, status + "go to profile");
            if (rScreen != DONE) continue;

            //nhắn tin
            updateLine(status + "message");
            if (messageUser(commentContentId, xml)) {
                if (ckbSendAnh) {
                    updateLine(status + "send image");
                    handleSendAnh(galleryId);
                }
                countSuccess++;
                if (countSuccess >= soLuong)
                    break;
                delay(delayFrom, delayTo, status + "success");
            }
        }

        return NOMAP;
    }

    ResultScreen messageUser(string messageContent, string xml = "") {
        for (int loop = 30; loop > 0; loop--) {
            if (isStopped())
                break;

            xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen == "TIK_ANOTHER_PROFILE") {
                if (!tap("//*[contains(@text,'Message') or contains(@text,'Send a')]",
                         xml))
                    break;
                delay(2);
                continue;
            }
            if (screen == "TIK_MESSAGE") {
                string xInput = "//android.widget.EditText[@text='Send a message...']";

                auto [success, content, itemId] = getScriptContent(messageContent);

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

                tap("//android.widget.EditText/following::android.widget.ImageView[last()]");
                delay(2);

                xml = "";
                if (existNode(xInput, xml, 60))
                    return existNode("//*[@text='Not sent']", xml) ? NOMAP : DONE;
                break;
            }
            if (screen == "TIK_MESSAGE_BLOCK")
                break;

            delay(1);
        }

        return NOMAP;
    }

    bool handleSendAnh(string image) {
        bool ckbXoaAnhDaDung = action.getBool("ckbXoaAnhDaDung");

        vector<MediaS3> lstMedia = getScriptMedia(image);

        if (lstMedia.empty())
            return false;

        //timeStart
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
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            string element = existNodes(0, xml, {
                    "(//android.widget.LinearLayout[@resource-id='com.ss.android.ugc.trill:id/f62']//android.widget.ImageView)[2]",
                    "(//android.widget.LinearLayout[@resource-id='com.zhiliaoapp.musically:id/fbf']//android.widget.ImageView)[2]",
                    "//*[@text='Allow TikTok to access photos, media, and files on your device?']",
                    "(//*[@resource-id='com.zhiliaoapp.musically:id/dv5'])[1]",
            });
            if (element == "(//android.widget.LinearLayout[@resource-id='com.ss.android.ugc.trill:id/f62']//android.widget.ImageView)[2]"
                || element == "(//android.widget.LinearLayout[@resource-id='com.zhiliaoapp.musically:id/fbf']//android.widget.ImageView)[2]") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element ==
                "//*[@text='Allow TikTok to access photos, media, and files on your device?']") {
                tap("//*[@text='Allow']");
                delay(2);
                continue;
            }
            if (element == "(//*[@resource-id='com.zhiliaoapp.musically:id/dv5'])[1]") {
                tap(element, xml);
                delay(2);
                tap("//*[@text='Send (1)']");
                delay(1);
                waitNodeHide("//*[@text='Sending']", 60);

                if (ckbXoaAnhDaDung) {
                    deleteScriptMedia(lstMedia);
                }
                return true;
            }
        }
        return false;
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
        bool isDeleteFound = false;
        //timeStart
        auto timeStart = high_resolution_clock::now();
        int timeout = 60;
        updateLine(
                "Remove post (" + to_string(countSuccess + 1) + "/" + to_string(soLuong) +
                ")");
        string xml = dumpXml();
        gotoHome(xml);

        //code here
        while (true) {
            if (isStopped())
                return ERROR;
            xml = dumpXml();
            string screen = handleScreen(xml);
            if (screen == "TIK_HOME") {
                tap("//*[@text='Profile']");
                delay(1);
                continue;
            }
            if (screen == "TIK_PROFILE") {
                if (existNode("//*[@text='Upload']", xml)) {
                    updateLine("No posts to delete...");
                    return NOMAP;
                }
                string xpathVideo = "(//*[@resource-id='com.zhiliaoapp.musically:id/cover'])[1]";
                if (!existNode(xpathVideo, xml)) {
                    tap("//*[@resource-id='com.zhiliaoapp.musically:id/eyp']");
                    delay(1);
                }
                tap(xpathVideo, xml);
                delay(1);
                continue;
            }
            string element = existNodes(0, xml, {
                    "//*[@resource-id='com.zhiliaoapp.musically:id/kv7']",
                    "//*[@text='Share to']",
                    "//*[@text='Delete this post?']",
                    "//*[@text='Delete this video?']",
            });
            if (element == "//*[@resource-id='com.zhiliaoapp.musically:id/kv7']") {
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='Share to']") {
                element = "//*[@content-desc='Delete']";
                int maxSwipes = 5;
                for (int i = 0; i < maxSwipes; ++i) {
                    //   xml = dumpXml();
                    if (!existNode(element, xml)) {
                        vector<Node> deleteNodes = getNodes(
                                "//*[@resource-id='com.zhiliaoapp.musically:id/kus']", xml);
                        swipe(deleteNodes.back(), deleteNodes.front(), 2000);
                        delay(2);
                        xml = dumpXml();
                    }
                    if (existNode(element, xml, 3))
                        break;
                }
                tap(element, xml);
                delay(2);
                continue;
            }
            if (element == "//*[@text='Delete this post?']"
                || element == "//*[@text='Delete this video?']") {
                tap("//*[@text='Delete']", xml);
                countSuccess++;
                delay(nudDelayFrom, nudDelayTo,
                      "Remove Post (" + to_string(countSuccess) + "/" + to_string(soLuong) +
                      ") success, delay");
                if (countSuccess >= soLuong)
                    return DONE;
                updateLine(
                        "Remove post (" + to_string(countSuccess + 1) + "/" +
                        to_string(soLuong) +
                        ")");
                timeStart = high_resolution_clock::now();
                continue;
            }
        }
        return NOMAP;
    }

    bool tapVideoFromProfile(string xml = "") {
        if (tap("//*[@content-desc='Videos' and @selected='false']", xml)) {
            delay(2);
            xml = "";
        }

        Node node = getNode("//*[@content-desc='Videos' and @selected='true']", xml);
        if (node.top > sHeight * 3 / 4)
            return false;

        string xVideo = existNodes(0, xml, {
                "//*[@text='Pinned']/parent::*",
                "//androidx.recyclerview.widget.RecyclerView//android.widget.ImageView"
        });
        if (xVideo.empty())
            return false;

        tap(xVideo, xml);
        delay(3);
        return true;
    }

    Task jobLike(Job job, const string &status) {
        gotoLink(job.link, status + " - Open video",
                 Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
        delay(2);
        if (Common::strContains(detectScreen(), "_POPUP_"))
            delay(2);

        updateLine(status + "Like...");
        string xml = dumpXml();
        if (!likeVideo(xml)) {
            if (!existNode("//*[@content-desc='Like']", xml))
                return Task::FAIL;
            if (existNode("//*[@content-desc='Like'][@selected='true']", xml))
                return Task::DONE;
            return Task::NONE;
        }

        return Task::DONE;
    }

    Task followUser(string user, string &status, bool openProfileBySearch = false) {
        user = Common::strToLower(user);
        string xml;
        if (openProfileBySearch) {
            xml = dumpXml();
            gotoHome(xml);
            if (!Common::strStartsWith(user, "https://")) {
                if (!Common::strStartsWith(user, "@"))
                    user = "@" + user;
                user = "https://www.tiktok.com/" + user;
            }
            //get user from link
            string userSearch = Common::regexGroups1(user + "/", "@(.*?)/");
            userSearch = Common::strSplit(userSearch, "?")[0];

            searchTiktok(userSearch, "Users");
            xml = "";
            if (existNode("//*[@text='" + userSearch + "']", xml, 0)) {
                tap("(//*[@text='" + userSearch + "'])[2]");
                delay(3);
            }

        } else {
            if (gotoProfile(user, status + " - Go to profile") != DONE)
                return Task::FAIL;
        }
        vector<string> lstXpath = {
                "//*[@text='Following']/parent::*/parent::*/parent::*/following::*[1]//*[@text='Follow back' or @text='Follow']",
                "//*[@text='Following']/parent::*/parent::*/following::*[1]//*[@text='Follow back' or @text='Follow']",
        };

        xml = dumpXml();

        string xpathFollow = existNodes(0, xml, lstXpath);

        if (xpathFollow.empty())
            return Task::FAIL;

        if (setting.followProfileType == FollowProfileType::RANDOM)
            setting.followType = static_cast<FollowProfileType>(Common::ranNumber(0, 1));

        if (setting.followType == FollowProfileType::VIDEO && tapVideoFromProfile()) {
            delay(4, 8, status + " - Watch video");

            dumpXml();
            if (existNode("//*[contains(@text,' parts · Updated ')]", xml)) {
                tap(
                        "//*[contains(@text,' parts · Updated ')]/preceding::android.widget.Button");
                delay(2);
            }
            updateLine(status + " - Follow...");

            if (!followVideo())
                return Task::FAIL;

        } else {
            updateLine(status + " - Follow");
            if (!tap(xpathFollow))
                return Task::FAIL;
        }

        delay(5, status + " - Follow");
        return Task::DONE;
    }


    bool jobCommentLiveStream(string job, const string &status) {
        gotoLink(job, status + " - Open Comment",
                 Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
        string xml = dumpXml();
        if (!existNode("//*[@text='Add comment']", xml, 10))
            return false;
        if (Common::strContains(detectScreen(), "_POPUP_"))
            delay(2);

        return commentLive("Mãi mãi dinh cao nheeee", xml) ? true : false;
    }

    Task jobComment(Job job, const string &status) {
        gotoLink(job.link, status + " - Open Comment",
                 Common::ranNumber(setting.delayOpenLinkFrom, setting.delayOpenLinkTo));
        delay(3);
        string xml = dumpXml();

        if (Common::strContains(detectScreen(), "_POPUP_"))
            delay(2);

        return commentVideoJob(job.content, xml) ? Task::DONE : Task::FAIL;
    }

    bool likeLive(string xml = "") {
        doubleTap(sWidth / 2, sHeight / 2);
        delay(2);

        return true;
    }

    bool followLive(string xml = "") {
        Node node = getNode(
                "//*[contains(@content-desc,'Follow ')]//*[@text='Follow']", xml);
        if (node.empty())
            return false;

        int x = Common::ranNumber(node.left, node.right);
        int y = Common::ranNumber(node.bottom - 20, node.bottom);
        tap(x, y);
        delay(2);

        return true;
    }

    bool commentLive(string commentContent, string xml = "") {
        string xAddComment = "//*[@text='Add comment']";
        string xInput = "//android.widget.EditText";

        string xpath = existNodes(0, xml, {xAddComment});
        if (xpath.empty())
            return false;

        tap(xpath, xml);
        delay(2);

        xml = "";
        if (!existNode(xInput, xml)) {
            performAction(Perform::BACK, 1, 1);
            return false;
        }

        string oldText = getNode(xInput, xml).text;

        tap(xInput, xml);
        delay(1);

        auto [success, content, itemId] = getScriptContent(commentContent);

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

        xml = dumpXml();
        string xSubmit = existNodes(0, xml, {
                "//android.widget.EditText/following::android.widget.ImageView[last()]"
        });
        if (xSubmit.empty()) {
            performAction(Perform::BACK, 1, 1);
            return false;
        }

        tap(xSubmit, xml);
        delay(2);
        updateLine("Check Comment");
        if (!existNode(xInput + "[@text='" + content + "']", xml, 10)) {
            updateLine("Comment fail");
            return false;

        }
        updateLine("Comment done");
        return true;
    }


    bool likeVideo(string xml = "") {
        string xpath = existNodes(0, xml,
                                  {"//*[@content-desc='Like'][@selected='false']"});
        if (xpath.empty())
            return false;

        if (Common::ranBool()) {
            //Cách 1: Tap vào nút like
            tap(xpath, xml);
        } else {
            //Cách 2: double tap vào màn hình
            doubleTap(sWidth / 2, sHeight / 2);
            delay(2);

            //nếu chưa like thì tap nút like
            xml = dumpXml();
            xpath = existNodes(0, xml,
                               {"//*[@content-desc='Like'][@selected='false']"});
            if (!xpath.empty())
                tap(xpath, xml);
        }

        return true;
    }

    bool followVideo(string xml = "") {
        string xpath = existNodes(0, xml,
                                  {"//*[contains(@content-desc,'Follow ')]/android.widget.ImageView"});
        if (xpath.empty())
            return false;

        tap(xpath, xml);
        return true;
    }

    bool followVideoTEST(string xml = "") {

        handleScreen(xml);
        string xpath = existNodes(0, xml,
                                  {"//*[contains(@content-desc,'Follow ')]/android.widget.ImageView"});
        if (xpath.empty())
            return false;

        tap(xpath, xml);
        return true;
    }

    bool commentVideo(string commentContent, string xml = "", bool ckbTagName = false,
                      string tagName = "") {
        string xAddComment = "//*[contains(@content-desc,'Read or add comments.')]//android.widget.ImageView";
        string xInput = "(//android.widget.EditText)[2]";
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

        string finalContent = "";

        //tag tên
        if (ckbTagName) {
            string xml = "";
            inputText("//android.widget.EditText", tagName);
            delay(2);
            Node editText = getNode("//android.widget.EditText", xml, 3);
            tapLong(editText.left + 90, editText.top + 30);
            delay(7, "delay TagName");
            tap("//*[@content-desc='Mention someone in a comment']");
            delay(5, "tag people");
            xml = dumpXml();
            tap("//*[@class='android.widget.TextView' and @text='" + tagName + "']", xml,
                3);
            finalContent = editText.text;
            delay(3);
        }

        //inputText(xInput, content, false);
        auto [success, content, itemId] = getScriptContent(commentContent);

        if (!success) {
            saveStatusAcc("Get Script Content", "Failed", 5, Task::FAIL);
            return NOMAP; // chay tiep hoac dung lai
        }

        // comment, dang bai, .....
        if (ckbTagName) {
            finalContent += " " + content;
        } else {
            finalContent = content;
        }

        inputText(xInput, content, false);
        // xoa content sau khi su dung

        if (!itemId.empty()) {
            deleteScriptContent(itemId);
        }
        delay(1);

        xml = dumpXml();
        string xSubmit = existNodes(0, xml, {
                "//*[@content-desc='Post comment']",
                "//*[@content-desc='Add emoji to comment']/parent::*/child::*[last()]"
        });
        if (xSubmit.empty()) {
            performAction(Perform::BACK, 2, 1);
            return false;
        }

        tap(xSubmit, xml);
        delay(2, "check sent comment");
        xml = dumpXml();
        if (existNode(xSubmit, xml, 3))
            tap(xSubmit, xml);
        delay(2);

        xml = "";
        bool isSuccess = existNode(xInput + "[@text='" + oldText + "']", xml, 30);
        delay(1);

        performAction(Perform::BACK, 1, 1);
        return isSuccess;
    }

    bool addFavoritesVideo(string xml = "") {
        string xpath = existNodes(0, xml,
                                  {"//*[@content-desc='Add or remove this video from Favorites.']//android.widget.ImageView[@selected='false']"});
        if (xpath.empty())
            return false;

        tap(xpath, xml);
        return true;
    }

    bool shareLiveStream(string xml = "") {
        if (xml == "")
            xml = dumpXml();

        string xpath = existNodes(3, xml, {
                "//*[contains(@content-desc,' Share video')]",
                "//*[contains(@content-desc,'Share')]"
        });
        if (!xpath.empty()) {
            tap(xpath, xml);
            delay(3);
        }


        xml = dumpXml();
        xpath = existNodes(3, xml,
                           {"//*[@content-desc=\"Copy link\"]"});

        if (xpath.empty())
            return false;

        tap(xpath, xml);
        delay(2);
        return true;
    }

    bool repostVideo(string xml = "") {
        string xpath = existNodes(0, xml, {"//*[@text='\u200ERepost']"});
        if (!xpath.empty()) {
            tap(xpath, xml);
            delay(2);
            return true;
        }

        xpath = existNodes(0, xml,
                           {"//*[@text='Share' or contains(@content-desc,'Share video')]//android.widget.ImageView"});
        if (xpath.empty())
            return false;

        tap(xpath, xml);
        delay(2);

        xml = "";
        xpath = existNodes(10, xml,
                           {"//*[@text='Repost']/preceding-sibling::*",
                            "//*[@text='Share to']"
                           });
        if (xpath != "//*[@text='Repost']/preceding-sibling::*") {
            performAction(Perform::BACK, 1, 1);
            return false;
        }

        tap(xpath, xml);
        return true;
    }

    ResultScreen actionTuongTacVideoJob() {
        //input

        Common::NLog(action.jsonConfig.toString(), "actionTuongTacVideoRandom");

        int cbbOptionsPost = action.getInt("cbbOptionsPost");
        int soLuong = Common::ranNumber(action.getInt("nudSoLuongFrom"),
                                        action.getInt("nudSoLuongTo"));

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

        bool ckbFollow = action.getBool("ckbFollow");
        int nudPercentFollow = action.getInt("nudPercentFollow");
        int nudDelayFollowFrom = action.getInt("nudDelayFollowFrom");
        int nudDelayFollowTo = action.getInt("nudDelayFollowTo");

        bool ckbAddFavorites = action.getBool("ckbAddFavorites");
        int nudPercentAddFavorites = action.getInt("nudPercentAddFavorites");
        int nudDelayAddFavoritesFrom = action.getInt("nudDelayAddFavoritesFrom");
        int nudDelayAddFavoritesTo = action.getInt("nudDelayAddFavoritesTo");

        bool ckbRepost = action.getBool("ckbRepost");
        int nudPercentRepost = action.getInt("nudPercentRepost");
        int nudDelayRepostFrom = action.getInt("nudDelayRepostFrom");
        int nudDelayRepostTo = action.getInt("nudDelayRepostTo");

        bool ckbComment = action.getBool("ckbComment");
        int nudPercentComment = action.getInt("nudPercentComment");
        int nudDelayCommentFrom = action.getInt("nudDelayCommentFrom");
        int nudDelayCommentTo = action.getInt("nudDelayCommentTo");
        vector<string> lstContent = action.getList("txtComment",
                                                   action.getInt("typeNganCach"));
        vector<string> lstContent_Clone(lstContent);

        bool ckbTagName = action.getBool("ckbTagName");
        string tagName = action.getString("tagName");

        //tuong tác nâng cao = là tên của TIKTOK không phải username
        vector<string> lstUser = action.getList("txtUserPro");
        //vector <string> lstUser = {"Máy Tính Bảng","Phim Cực Ngắn","Phạm Duy Anh"};
        string xpathlstFind = "//*[contains(@text,";

        for (string item: lstUser) {
            xpathlstFind += "'" + item + "') or contains(@text,";

        }
        if (Common::strEndsWith(xpathlstFind, "or contains(@text,")) {
            xpathlstFind = xpathlstFind.substr(0, xpathlstFind.length() - 19);
        }
        xpathlstFind = xpathlstFind + "]";
        xpathlstFind = Common::strToLower(xpathlstFind);

        int cbbOptionsPostPro = action.getInt("cbbOptionsPostPro");
        int soLuongPro = Common::ranNumber(action.getInt("nudSoLuongFromPro"),
                                           action.getInt("nudSoLuongToPro"));
        int thoiGianPro = Common::ranNumber(action.getInt("nudThoiGianFromPro"),
                                            action.getInt("nudThoiGianToPro"));


        int nudTimeFromPro = action.getInt("nudTimeFromPro");
        int nudTimeToPro = action.getInt("nudTimeToPro");

        bool ckbInteractPro = action.getBool("ckbInteractPro");
        int nudPercentInteractPro = action.getInt("nudPercentInteractPro");
        int nudDelayInteractFromPro = action.getInt("nudDelayInteractFromPro");
        int nudDelayInteractToPro = action.getInt("nudDelayInteractToPro");

        bool ckbFollowPro = action.getBool("ckbFollowPro");
        int nudPercentFollowPro = action.getInt("nudPercentFollowPro");
        int nudDelayFollowFromPro = action.getInt("nudDelayFollowFromPro");
        int nudDelayFollowToPro = action.getInt("nudDelayFollowToPro");

        bool ckbAddFavoritesPro = action.getBool("ckbAddFavoritesPro");
        int nudPercentAddFavoritesPro = action.getInt("nudPercentAddFavoritesPro");
        int nudDelayAddFavoritesFromPro = action.getInt("nudDelayAddFavoritesFromPro");
        int nudDelayAddFavoritesToPro = action.getInt("nudDelayAddFavoritesToPro");

        bool ckbRepostPro = action.getBool("ckbRepostPro");
        int nudPercentRepostPro = action.getInt("nudPercentRepostPro");
        int nudDelayRepostFromPro = action.getInt("nudDelayRepostFromPro");

        int nudDelayRepostToPro = action.getInt("nudDelayRepostToPro");
        bool ckbCommentPro = action.getBool("ckbCommentPro");
        int nudPercentCommentPro = action.getInt("nudPercentCommentPro");
        int nudDelayCommentFromPro = action.getInt("nudDelayCommentFromPro");
        int nudDelayCommentToPro = action.getInt("nudDelayCommentToPro");

        vector<string> lstContentPro = action.getList("txtCommentPro",
                                                      action.getInt("typeNganCachPro"));

        vector<string> lstContent_ClonePro(lstContentPro);
        int countVideoPro = 0;
        bool tuongTacUser = false;

        if (lstContent.empty())
            ckbComment = false;
        string xml;
        string screen;
        string status;
        int countVideo = 0;
        auto start = high_resolution_clock::now();

        while (!isStopped()) {
            status = "Video (" + to_string(countVideo + 1) + ") ";
            updateLine(status + "Checking...");
            xml = dumpXml();

            screen = handleScreen(xml);

            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;

            if (screen != "TIK_HOME") {
                ResultScreen rScreen = gotoHome(xml);
                if (rScreen != DONE)
                    return rScreen;

                if (tap(vector<string>{"//*[@text='For You']", "//*[@text='Following']",
                                       "//*[@text='Friends']"
                }[cbbTuyChon]))
                    delay(2);
                xml = dumpXml();
            }

            if (countVideo > 0) {
                updateLine(status + "Swipe...");

                swipeVideo();
                xml = dumpXml();
                //tuongtacnangcao
                if (!lstUser.empty()) {
                    if (existNode(xpathlstFind, xml, 5)) {
                        ckbInteract = true;
                        ckbFollow = true;
                        tuongTacUser = true;
                        if (lstContent.empty())
                            ckbComment = false;
                        else
                            ckbComment = true;
                        ckbAddFavorites = true;
                        ckbRepost = true;
                    }
                }

            }

            delay(nudTimeFrom, nudTimeTo, status + "Watch video");

            if (ckbInteract && Common::ranBool(nudPercentInteract)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(nudDelayInteractFrom, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbComment && Common::ranBool(nudPercentComment)) {
                updateLine(status + "Comment...");
                if (lstContent_Clone.empty())
                    lstContent_Clone = vector<string>(lstContent);
                string content = Common::ranItem(lstContent_Clone);

                if (commentVideoJob(content, xml, ckbTagName, tagName)) {
                    Common::removeItem(lstContent_Clone, content);
                    delay(nudDelayCommentFrom, nudDelayCommentTo,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollow && Common::ranBool(nudPercentFollow)) {
                updateLine(status + "Follow...");
                if (followVideo(xml)) {
                    delay(nudDelayFollowFrom, nudDelayFollowTo,
                          status + "Follow done, delay");
                    xml = "";
                }
            }

            if (ckbRepost && Common::ranBool(nudPercentRepost)) {
                updateLine(status + "Repost...");
                if (repostVideo(xml)) {
                    delay(nudDelayRepostFrom, nudDelayRepostTo,
                          status + "Repost done, delay");
                    xml = "";
                    if (countVideo == 0)
                        handleScreen(xml);
                }
            }

            if (ckbAddFavorites && Common::ranBool(nudPercentAddFavorites)) {
                updateLine(status + "Add Favorites...");
                if (addFavoritesVideo(xml))
                    delay(nudDelayAddFavoritesFrom, nudDelayAddFavoritesTo,
                          status + "Add Favorites done, delay");
            }
            //tuongtacnangcao
            if (!lstUser.empty() && tuongTacUser) {
                xml = dumpXml();
                tap("(//*[contains(@content-desc,'profile')])[1]", xml, 3);
                delay(1);
                goto tuongTacPro;
            }
            ketThucPro:
            //tuongtacnangcao
            countVideo++;
            bool isDone =
                    cbbOptionsPost == 0 ? countVideo >= soLuong
                                        : Common::hasExceededTimeout(start,
                                                                     thoiGian);
            if (isDone)
                return DONE;
        }
        tuongTacPro:
        for (int i = 0; i < 50; ++i) {
            status = "Video Pro (" + to_string(countVideoPro + 1) + ") ";
            updateLine(status + "Checking...");

            xml = dumpXml();
            screen = handleScreen(xml);
            if (screen == "CONTINUE") continue;
            if (screen == "ERROR") return ERROR;
            if (screen == "TIK_ANOTHER_PROFILE") {
                if (!tapVideoFromProfile(xml))
                    return NOMAP;
            }
            if (countVideoPro > 0) {
                updateLine(status + "Swipe...");
                swipe();

                xml = dumpXml();
            }

            delay(nudTimeFrom, nudTimeTo, status + "Watch video");

            if (ckbInteractPro && Common::ranBool(nudPercentInteractPro)) {
                updateLine(status + "Like...");
                if (likeVideo(xml))
                    delay(nudDelayInteractFromPro, nudDelayInteractTo,
                          status + "Like done, delay");
            }

            if (ckbCommentPro && Common::ranBool(nudPercentCommentPro)) {
                updateLine(status + "Comment...");
                if (lstContent_Clone.empty())
                    lstContent_ClonePro = vector<string>(lstContentPro);
                string contentPro = Common::ranItem(lstContent_ClonePro);

                if (commentVideo(contentPro, xml)) {
                    Common::removeItem(lstContent_ClonePro, contentPro);
                    delay(nudDelayCommentFromPro, nudDelayCommentToPro,
                          status + "Comment done, delay");
                    xml = "";
                }
            }

            if (ckbFollowPro && Common::ranBool(nudPercentFollowPro)) {
                updateLine(status + "Follow...");
                if (followVideo(xml)) {
                    delay(nudDelayFollowFromPro, nudDelayFollowToPro,
                          status + "Follow done, delay");
                    xml = "";
                }
            }

            if (ckbRepostPro && Common::ranBool(nudPercentRepostPro)) {
                updateLine(status + "Repost...");
                if (repostVideo(xml)) {
                    delay(nudDelayRepostFromPro, nudDelayRepostToPro,
                          status + "Repost done, delay");
                    xml = "";
                    if (countVideoPro == 0)
                        handleScreen(xml);
                }
            }

            if (ckbAddFavoritesPro && Common::ranBool(nudPercentAddFavoritesPro)) {
                updateLine(status + "Add Favorites...");
                if (addFavoritesVideo(xml))
                    delay(nudDelayAddFavoritesFromPro, nudDelayAddFavoritesToPro,
                          status + "Add Favorites done, delay");
            }

            countVideoPro++;
            bool isDonePro =
                    cbbOptionsPostPro == 0 ? countVideoPro >= soLuongPro
                                           : Common::hasExceededTimeout(start,
                                                                        thoiGianPro);
            if (isDonePro) {
                performAction(Perform::BACK, 2, 1);
                swipeVideo();
                goto ketThucPro;
            }
        }
        //tuongtacnangcao
        return NOMAP;
    }


    bool commentVideoJob(string content, string xml = "", bool ckbTagName = false,
                         string tagName = "") {
        string xAddComment = "//*[contains(@content-desc,'Read or add comments.')]//android.widget.ImageView";
        string xInput = "(//android.widget.EditText)[2]";
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
        if (ckbTagName) {
            string xml = "";
            inputText("//android.widget.EditText", tagName);
            delay(2);
            Node editText = getNode("//android.widget.EditText", xml, 3);
            tapLong(editText.left + 90, editText.top + 30);
            delay(7, "delay TagName");
            tap("//*[@content-desc='Mention someone in a comment']");
            delay(5, "tag people");
            xml = "";
            tap("//*[@class='android.widget.TextView' and @text='" + tagName + "']", xml,
                3);
            content = " " + content;
            delay(3);
        }

        inputText(xInput, content, false);
        delay(1);

        xml = dumpXml();
        string xSubmit = existNodes(0, xml, {
                "//*[@content-desc='Post comment']",
                "//*[@content-desc='Add emoji to comment']/parent::*/child::*[last()]"
        });
        if (xSubmit.empty()) {
            performAction(Perform::BACK, 2, 1);
            return false;
        }

        tap(xSubmit, xml);
        delay(2, "check sent comment");
        xml = dumpXml();
        if (existNode(xSubmit, xml, 3))
            tap(xSubmit, xml);
        delay(2);

        xml = "";
        bool isSuccess = existNode(xInput + "[@text='" + oldText + "']", xml, 30);
        delay(1);

        performAction(Perform::BACK, 1, 1);
        return isSuccess;
    }


};