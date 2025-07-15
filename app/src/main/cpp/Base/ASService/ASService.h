#ifndef ASSERVICE_H
#define ASSERVICE_H

#include <ASModel.h>

#include <ADB.h>
#include <Media.h>
#include <API.h>
#include <Device.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using IResult = API::IResult;

class ASService : public Base {
public:
    static string currentPackage;

    static string currentActivity;

    static string currentAction;

    static bool isASRunning;

    static std::chrono::time_point<high_resolution_clock> startTimeProxy;

    static void initService();

    static AppSetting appSetting;

    static vector<Screen> lstScreen;

    static vector<Screen> lstScreenSocial;

    static int countUnknownScreen;

    static std::map<string, jobject> lstNodeWId;

    static jobject lastEditTextFocus;
    static string lastEditTextFocusContent;

    static API::IResult OCRequest(string url, string body = "");

    static bool isAuthDevice();

    static void mouseMove(int x, int y, int type);

    static void tap(int x, int y);

    static void tapLong(int x, int y);

    static bool tapLong(Node node);

    static bool tap(string xpath, string xml = "", int timeout = 0);

    static bool tap(Node node);

    static bool tapCenter(string xpath, string xml = "", int timeout = 0);

    static bool tapCenter(Node node);

    static bool tapWait(string xpath, string xml = "", int maxTap = 10);

    static bool inputText(string xpath, string text, bool isClear = true);

    static bool inputText(Node node, string text, bool isClear = true);

    static void inputTextOnLast(string xpath, string text);

    static void doubleTap(int x, int y);

    static void swipe(int x1, int y1, int x2, int y2, int duration = 1000);

    static void swipe(Node nodeStart, Node nodeEnd, int duration = 1000);

    static void swipe(int typeScroll = 1, int countScroll = 1, int duration = 0);

    static void swipeH(int typeScroll = 1, int countScroll = 1, int duration = 0);

    static bool swipeAndScreenNotChange(int typeScroll = 1, int countScroll = 1, int duration = 0);

    static bool swipeHAndScreenNotChange(int typeScroll = 1, int countScroll = 1, int duration = 0);

    static void performAction(Perform perform, int count = 1, int delay = 0);

    static void back(int count = 1, int delay = 1);

    static void handleKeyboard(Keyboard action, string value = "");

    static void clearText(string xpath = "");

    static bool inputEnter(string xpath = "", string xml = "");

    static bool findImageAndClick(const std::string& templateBase64);

    static string dumpXml(bool isGenId = false);

    static string existNodes(int timeout, string &xml, vector<string> lstXpath);

    static bool existNode(string xpath, string &xml, int timeout = 0);

    static bool waitNode(string xpath, string &xml, int timeout);

    static bool waitNodeHide(string xpath, int timeout);

    static vector<string> getListText(string &xml);

    static vector<Node> getNodes(string xpath, string &xml, int limitNode = 0);

    static vector<Node> getNodes(vector<string> lstXpath, string &xml, int limitNode = 0);

    static Node getNode(vector<string> lstXpath, string &xml, int timeout = 0);

    static Node getNode(string xpath, string &xml, int timeout = 0);

    static string getNodeContent(pugi::xml_node xmlNode);

    static void findNonEmptyTextNodes(const pugi::xml_node &node, const std::string &currentPath,
                                      std::vector<std::string> &lstXpath);

    static bool setupResource();

    static string detectScreen(string xml = "");

    static void addUnknownScreen(bool isLocal = true, string action = "");

    static bool isAppOpened(string package);

    static void disableSelf(string message, bool isClear = true);

    static void onEvent(JNIEnv *env, jobject thiz, jobject jEvent);

    enum class HelperAction {
        PING_PONG,
        BACKUP_DEVICE,
        RESTORE_DEVICE,
        START_VPN,
        STOP_VPN
    };

    static string
    sendEventToHelper(HelperAction action, vector<std::pair<string, string>> lstExtra = {});

    static void initNotification();

private:
    static void initASBL();

    static Node nodeObjectToASNode(jobject currentNode);

    static bool isScreenMatch(const std::vector<std::string> &lstXpath, string xml);

    static void
    traverseNodeInfo(string &xml, jobject nodeInfo, int depth = 0, bool isGenId = false);

    static void doDispatchGesture(int x1, int y1, int x2, int y2, int duration);

    static vector<string> getXpath(string xml);

    static cv::Mat decodeBase64ToMat(const std::string &base64String);
};

#endif // ASSERVICE_H