#include "ASService.h"
#include "../Json/JSON.h"

string ASService::currentPackage = "";
string ASService::currentActivity = "";

string ASService::currentAction = "";

bool ASService::isASRunning = false;


std::chrono::time_point<high_resolution_clock> ASService::startTimeProxy;

AppSetting ASService::appSetting;
vector<Screen> ASService::lstScreen;
vector<Screen> ASService::lstScreenSocial;
int ASService::countUnknownScreen = 0;
std::map<string, jobject> ASService::lstNodeWId;
jobject ASService::lastEditTextFocus = nullptr;
string ASService::lastEditTextFocusContent = "";

struct {
    jclass nodeInfoClass;
    jmethodID dispatchGestureMethod;
    jmethodID getRootInActiveWindow;
    jmethodID performGlobalAction;
    jmethodID newBundleClass;
    jmethodID putCharSequenceMethod;
    jmethodID performActionWBundleMethod;
    jmethodID performActionMethod;
    jmethodID performActionIdMethod;
    jmethodID moveToMethod;
    jmethodID lineToMethod;
    jmethodID newBuilderClass;
    jmethodID addStrokeMethod;
    jmethodID buildGestureDescriptionMethod;
    jmethodID newStrokeClass;
    jmethodID newPathClass;
    jmethodID isEmptyMethod;
    jmethodID removeMethod;
    jmethodID getChildCountMethod;
    jmethodID getChildMethod;
    jmethodID getClassNameMethod;
    jmethodID getPackageNameMethod;
    jmethodID getTextMethod;
    jmethodID getDescMethod;
    jmethodID getResourceIdMethod;
    jmethodID isVisibleToUserMethod;
    jmethodID isCheckableMethod;
    jmethodID isScrollableMethod;
    jmethodID isCheckedMethod;
    jmethodID isClickableMethod;
    jmethodID isEnabledMethod;
    jmethodID isFocusableMethod;
    jmethodID isLongClickableMethod;
    jmethodID isPasswordMethod;
    jmethodID isSelectedMethod;
    jmethodID isEditableMethod;
    jmethodID toStringMethod;
    jmethodID newRectClass;
    jmethodID getBoundsInScreenMethod;
    jmethodID centerXRectMethod;
    jmethodID centerYRectMethod;

    jclass ASServiceClass;
    jclass pathClass;
    jclass bundleClass;
    jclass builderClass;
    jclass strokeClass;
    jclass rectClass;

    jfieldID leftRectField;
    jfieldID topRectField;
    jfieldID rightRectField;
    jfieldID bottomRectField;


    jmethodID getEventTypeASEvent;
    jmethodID getPackageNameASEvent;
    jmethodID getClassNameASRecord;


    jmethodID disableSelfMethod;

    jmethodID mouseMoveMethod;


    jmethodID getEventSourceMethod;
    jmethodID initNotificationMethod;
} static AServiceVM;

void ASService::initService() {
    AServiceVM.nodeInfoClass = JExt::findClass("android/view/accessibility/AccessibilityNodeInfo",
                                               true);

    AServiceVM.disableSelfMethod = JExt::findMethod(baseVM.jClass, "disableSelf", "()V");

    AServiceVM.mouseMoveMethod = JExt::findMethod(baseVM.jClass, "mouseMove", "(III)V", true);

    initASBL();
}

void ASService::initASBL() {
    AServiceVM.ASServiceClass = JExt::findClass(
            "android/accessibilityservice/AccessibilityService", true);
    AServiceVM.getRootInActiveWindow = JExt::findMethod(AServiceVM.ASServiceClass,
                                                        "getRootInActiveWindow",
                                                        "()Landroid/view/accessibility/AccessibilityNodeInfo;");


    AServiceVM.performGlobalAction = JExt::findMethod(AServiceVM.ASServiceClass,
                                                      "performGlobalAction",
                                                      "(I)Z");

    AServiceVM.initNotificationMethod = JExt::findMethod(baseVM.jClass, "initNoti", "()V", true);


    AServiceVM.bundleClass = JExt::findClass("android/os/Bundle", true);
    AServiceVM.newBundleClass = JExt::findMethod(AServiceVM.bundleClass, "<init>", "()V");


    AServiceVM.putCharSequenceMethod = JExt::findMethod(AServiceVM.bundleClass,
                                                        "putCharSequence",
                                                        "(Ljava/lang/String;Ljava/lang/CharSequence;)V");

    AServiceVM.performActionWBundleMethod = JExt::findMethod(AServiceVM.nodeInfoClass,
                                                             "performAction",
                                                             "(ILandroid/os/Bundle;)Z");

    AServiceVM.performActionMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "performAction",
                                                      "(I)Z");


    AServiceVM.performActionIdMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "performAction",
                                                        "(I)Z");

    AServiceVM.pathClass = JExt::findClass("android/graphics/Path", true);
    AServiceVM.newPathClass = JExt::findMethod(AServiceVM.pathClass, "<init>", "()V");
    AServiceVM.moveToMethod = JExt::findMethod(AServiceVM.pathClass, "moveTo", "(FF)V");
    AServiceVM.lineToMethod = JExt::findMethod(AServiceVM.pathClass, "lineTo", "(FF)V");

    AServiceVM.builderClass = JExt::findClass(
            "android/accessibilityservice/GestureDescription$Builder", true);
    AServiceVM.newBuilderClass = JExt::findMethod(AServiceVM.builderClass, "<init>", "()V");
    AServiceVM.strokeClass = JExt::findClass(
            "android/accessibilityservice/GestureDescription$StrokeDescription", true);
    AServiceVM.newStrokeClass = JExt::findMethod(AServiceVM.strokeClass, "<init>",
                                                 "(Landroid/graphics/Path;JJ)V");
    AServiceVM.addStrokeMethod = JExt::findMethod(AServiceVM.builderClass, "addStroke",
                                                  "(Landroid/accessibilityservice/GestureDescription$StrokeDescription;)"
                                                  "Landroid/accessibilityservice/GestureDescription$Builder;");

    AServiceVM.buildGestureDescriptionMethod = JExt::findMethod(AServiceVM.builderClass,
                                                                "build",
                                                                "()Landroid/accessibilityservice/GestureDescription;");
    AServiceVM.dispatchGestureMethod = JExt::findMethod(AServiceVM.ASServiceClass,
                                                        "dispatchGesture",
                                                        "(Landroid/accessibilityservice/GestureDescription;Landroid/accessibilityservice/AccessibilityService$GestureResultCallback;Landroid/os/Handler;)Z");


    AServiceVM.rectClass = JExt::findClass("android/graphics/Rect", true);


    AServiceVM.getChildCountMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "getChildCount",
                                                      "()I");
    AServiceVM.getChildMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "getChild",
                                                 "(I)Landroid/view/accessibility/AccessibilityNodeInfo;");
    AServiceVM.getPackageNameMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "getPackageName",
                                                       "()Ljava/lang/CharSequence;");

    AServiceVM.getClassNameMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "getClassName",
                                                     "()Ljava/lang/CharSequence;");
    AServiceVM.getTextMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "getText",
                                                "()Ljava/lang/CharSequence;");

    AServiceVM.getDescMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "getContentDescription",
                                                "()Ljava/lang/CharSequence;");
    AServiceVM.getResourceIdMethod = JExt::findMethod(AServiceVM.nodeInfoClass,
                                                      "getViewIdResourceName",
                                                      "()Ljava/lang/String;");

    AServiceVM.toStringMethod = JExt::findMethod(JExt::findClass("java/lang/Object"),
                                                 "toString",
                                                 "()Ljava/lang/String;");


    AServiceVM.newRectClass = JExt::findMethod(AServiceVM.rectClass, "<init>", "()V");
    AServiceVM.getBoundsInScreenMethod = JExt::findMethod(AServiceVM.nodeInfoClass,
                                                          "getBoundsInScreen",
                                                          "(Landroid/graphics/Rect;)V");
    AServiceVM.centerXRectMethod = JExt::findMethod(AServiceVM.rectClass, "centerX", "()I");
    AServiceVM.centerYRectMethod = JExt::findMethod(AServiceVM.rectClass, "centerY", "()I");

    AServiceVM.leftRectField = JExt::findField(AServiceVM.rectClass, "left", "I");
    AServiceVM.topRectField = JExt::findField(AServiceVM.rectClass, "top", "I");
    AServiceVM.rightRectField = JExt::findField(AServiceVM.rectClass, "right", "I");
    AServiceVM.bottomRectField = JExt::findField(AServiceVM.rectClass, "bottom", "I");

    AServiceVM.isVisibleToUserMethod = JExt::findMethod(AServiceVM.nodeInfoClass,
                                                        "isVisibleToUser",
                                                        "()Z");
    AServiceVM.isCheckableMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isCheckable",
                                                    "()Z");
    AServiceVM.isCheckedMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isChecked", "()Z");
    AServiceVM.isScrollableMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isScrollable",
                                                     "()Z");
    AServiceVM.isClickableMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isClickable",
                                                    "()Z");
    AServiceVM.isEnabledMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isEnabled",
                                                  "()Z");
    AServiceVM.isFocusableMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isFocusable",
                                                    "()Z");
    AServiceVM.isLongClickableMethod = JExt::findMethod(AServiceVM.nodeInfoClass,
                                                        "isLongClickable",
                                                        "()Z");
    AServiceVM.isPasswordMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isPassword",
                                                   "()Z");
    AServiceVM.isSelectedMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isSelected",
                                                   "()Z");
    AServiceVM.isEditableMethod = JExt::findMethod(AServiceVM.nodeInfoClass, "isEditable",
                                                   "()Z");


    jclass accessibilityEventClass = JExt::findClass(
            "android/view/accessibility/AccessibilityEvent");

    AServiceVM.getEventTypeASEvent = JExt::findMethod(accessibilityEventClass,
                                                      "getEventType",
                                                      "()I");

    AServiceVM.getPackageNameASEvent = JExt::findMethod(accessibilityEventClass,
                                                        "getPackageName",
                                                        "()Ljava/lang/CharSequence;");

    AServiceVM.getClassNameASRecord = JExt::findMethod(accessibilityEventClass,
                                                       "getClassName",
                                                       "()Ljava/lang/CharSequence;");

    AServiceVM.getEventSourceMethod = JExt::findMethod(accessibilityEventClass,
                                                       "getSource",
                                                       "()Landroid/view/accessibility/AccessibilityNodeInfo;");

}

API::IResult ASService::OCRequest(string url, string body) {
    url = url.find("https://") == string::npos
          && url.find("http://") == string::npos ? url = BASE_URL + url : url;

    url = Common::strReplace(url, "\n", "");

    IResult result;

    if (isStop) return result;

    try {
        Http client(url, body.empty() ? Method::GET : Method::POST);

        if (!body.empty())
            client.setBody(body);

        client.setHeader("X-Api-Version: " + Device::currentVersion);

        client.setContentType("application/json");

        if (!jwt.empty())
            client.addBearerToken(jwt);

        auto clientResult = client.connect();

        string resultString = clientResult.response;

        if (resultString.empty()) return IResult{false, "Error when execute"};

        result.response = resultString;

        if (!JSON::isJSON(result.response)) {
            result.message = "Request Invalidate";
            return result;
        }
        json jsonResult = json::parse(result.response);

        if (!jsonResult.contains("succeeded")) {
            result.message = "Request Invalidate";
            return result;
        }

        result.succeeded = jsonResult["succeeded"];

        if (!result.succeeded) {
            result.messages = jsonResult["messages"].get<std::vector<std::string >>();
            for (const string &item: result.messages) {
                result.message += item + " - ";
            }
            if (Common::strEndsWith(result.message, " - "))
                result.message = result.message.substr(0, result.message.size() - 3);

            Common::NLog(result.message, "OCRequest");

            return result;
        }

        result.data = jsonResult["data"].dump();
        if (JSON::isJSON(result.data)) {
            result.json = JSON(result.data);
        }

        string temp;
        for (char c: result.data) {
            if (c != '"') {
                temp += c;
            }
        }
        result.data = temp;

        if (!Common::strContains(url, "Dev/get-resource")) {
            Common::NLog("[" + url + "] : " + result.response, "OCRequest");
        }

        if (result.succeeded) return result;

        if (Common::strContains(result.data, "Package Expired")
            || Common::strContains(result.data, "Notfound Package in this Device")
            || Common::strContains(result.data, "You are not Authorized")
            || Common::strContains(result.data, "You are not authorized to access this resource")) {
            string data = result.data;
            updateLine(data);
            while (!isStop) {
                showToast(data);
                delay(2);
            }
        }

        if (Common::strContains(result.data, "Notfound Package in this Device")
            || Common::strContains(result.data, "You are not Authorized")
            || Common::strContains(result.data, "You are not authorized to access this resource")) {
            string data = result.data;

            for (int loop = 20; loop > 0; loop--) {
                updateLine(data, true);
                delay(5);
            }

            disableSelf(data);
            return false;
        }

    }
    catch (const std::exception &e) {
        result.message = "Request Invalidate";
        return result;
    }
}


bool ASService::isAuthDevice() {
    IResult response = OCRequest(APP_URL + "auth-device");

    if (Common::strContains(response.message, "Notfound Device")) {
        ASService::disableSelf("Notfound Device");
        return false;
    }

    if (!response.succeeded)
        ASService::delay(20, response.message, true);

    return response.succeeded;
}

void ASService::doDispatchGesture(int x1, int y1, int x2, int y2, int duration) {
    if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || duration < 0)
        return;
    JNIEnv *env = JExt::getEnv();

    jobject pathObject = env->NewObject(AServiceVM.pathClass, AServiceVM.newPathClass);

    env->CallVoidMethod(pathObject, AServiceVM.moveToMethod, (jfloat) x1, (jfloat) y1);
    env->CallVoidMethod(pathObject, AServiceVM.lineToMethod, (jfloat) x2, (jfloat) y2);

    jobject builderobject = env->NewObject(AServiceVM.builderClass, AServiceVM.newBuilderClass);

    jobject strokeobject = env->NewObject(AServiceVM.strokeClass, AServiceVM.newStrokeClass,
                                          pathObject, (jlong) 0,
                                          (jlong) duration);

    if (builderobject == nullptr) {
        Common::NLog("builderobject is null", "doDispatchGesture");
        return;
    }

    if (strokeobject == nullptr) {
        Common::NLog("strokeobject is null", "doDispatchGesture");
        return;
    }

    env->CallObjectMethod(builderobject, AServiceVM.addStrokeMethod, strokeobject);

    jobject gesture = env->CallObjectMethod(builderobject,
                                            AServiceVM.buildGestureDescriptionMethod);

    jobject _instance = getInstance();
    env->CallBooleanMethod(_instance, AServiceVM.dispatchGestureMethod, gesture, nullptr, nullptr);

    env->DeleteLocalRef(pathObject);
    env->DeleteLocalRef(builderobject);
    env->DeleteLocalRef(strokeobject);
    env->DeleteLocalRef(gesture);
}

void ASService::swipe(int x1, int y1, int x2, int y2, int duration) {
    doDispatchGesture(x1, y1, x2, y2, duration);
}

void ASService::swipe(Node nodeStart, Node nodeEnd, int duration) {
    int x1 = Common::ranNumber(nodeStart.left, nodeStart.right);
    int y1 = Common::ranNumber(nodeStart.top, nodeStart.bottom);
    int x2 = Common::ranNumber(nodeEnd.left, nodeEnd.right);
    int y2 = Common::ranNumber(nodeEnd.top, nodeEnd.bottom);

    doDispatchGesture(x1, y1, x2, y2, duration);
}

void ASService::mouseMove(int x, int y, int type) {
    if (x < 0 || y < 0) return;
    JNIEnv *env = JExt::getEnv();
    env->CallStaticVoidMethod(baseVM.jClass, AServiceVM.mouseMoveMethod, x, y, type);
}

void ASService::doubleTap(int x, int y) {
    for (int loop = 2; loop > 0; loop--) {
        tap(x, y);
        delay2(100);
    }
}

void ASService::tap(int x, int y) {
    doDispatchGesture(x, y, x, y, Common::ranNumber(45, 100));
}

void ASService::tapLong(int x, int y) {
    int duration = 3000;//Common::ranNumber(3000, 3500);
    doDispatchGesture(x, y, x, y, duration);
}

bool ASService::tapLong(Node node) {
    if (node.empty())
        return false;
    int x = Common::ranNumber(node.left, node.right);
    int y = Common::ranNumber(node.top, node.bottom);
    tapLong(x, y);
    return true;
}

bool ASService::tap(string xpath, string xml, int timeout) {
    Node node = getNode(xpath, xml, timeout);
    if (node.empty())
        return false;

    return tap(node);
}

bool ASService::tap(Node node) {
    if (node.empty())
        return false;
    int x = Common::ranNumber(node.left, node.right);
    int y = Common::ranNumber(node.top, node.bottom);

    tap(x, y);
    return true;
}

bool ASService::tapCenter(string xpath, string xml, int timeout) {
    Node node = getNode(xpath, xml, timeout);
    if (node.empty())
        return false;

    return tapCenter(node);
}

bool ASService::tapCenter(Node node) {
    if (node.empty())
        return false;

    tap(node.centerX, node.centerY);
    return true;
}

bool ASService::tapWait(string xpath, string xml, int maxTap) {
    string oldBounds = "";
    if (xml.empty())
        xml = dumpXml();

    for (int i = 0; i < maxTap; i++) {
        if (isStop)
            break;

        Node node = getNode(xpath, xml);
        if (node.empty())
            return i > 0;

        string bounds = node.bounds;
        if (bounds != oldBounds) {
            if (!oldBounds.empty())
                return true;
            oldBounds = bounds;
        }

        tap(node);
        delay(2);
        xml = dumpXml();
    }
    return false;
}

void ASService::handleKeyboard(Keyboard action, string value) {
    JNIEnv *env = JExt::getEnv();
    string actionStr = "";

    if (action == Keyboard::INPUT) {
        if (value.empty()) return;
        actionStr = "INPUT_TEXT";
    } else if (action == Keyboard::ENTER)
        actionStr = "DONE_TEXT";
    else if (action == Keyboard::CLEAR)
        actionStr = "CLEAR_TEXT";
    else if (action == Keyboard::MATCHA)
        actionStr = "MATCHA_TEXT";
    else if (action == Keyboard::SELECT_ALL)
        actionStr = "SELECT_ALL";
    else if (action == Keyboard::HIDE)
        actionStr = "HIDE_KEYBOARD";
    else if (action == Keyboard::COPY_TEXT)
        actionStr = "COPY_TEXT";
    else if (action == Keyboard::ARROW_RIGHT)
        actionStr = "ARROW_RIGHT";
    else if (action == Keyboard::ARROW_LEFT)
        actionStr = "ARROW_LEFT";
    else if (action == Keyboard::MOVE_LAST)
        actionStr = "MOVE_LAST";

    jobject jContext = getContext();

    if (jContext == nullptr) return;

    jclass intentClass = JExt::findClass("android/content/Intent");

    jmethodID newIntent = JExt::findMethod(intentClass, "<init>", "()V");
    jobject intentObj = env->NewObject(intentClass, newIntent);

    jstring jAction = JExt::stringToJstring(actionStr);
    jmethodID setActionMethod = JExt::findMethod(intentClass, "setAction",
                                                 "(Ljava/lang/String;)Landroid/content/Intent;",
                                                 false);

    intentObj = env->CallObjectMethod(intentObj, setActionMethod, jAction);

    if (!value.empty()) {
        jstring jValue = JExt::stringToJstring(Common::base64_encode(value));

        jmethodID putExtraMethod = JExt::findMethod(intentClass, "putExtra",
                                                    "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;");
        intentObj = env->CallObjectMethod(intentObj, putExtraMethod, JExt::stringToJstring("msg"),
                                          jValue);
    }

    jclass contextClass = JExt::findClass("android/content/Context");

    jmethodID sendBroadcastMethod = JExt::findMethod(contextClass, "sendBroadcast",
                                                     "(Landroid/content/Intent;)V");

    env->CallVoidMethod(jContext, sendBroadcastMethod, intentObj);

    env->DeleteLocalRef(contextClass);
}

bool ASService::inputText(string xpath, string text, bool isClear) {
    string xml = dumpXml(ADB::isRoot);
    Node node = getNode(xpath, xml);
    if (node.empty())
        return false;

    return inputText(node, text, isClear);
}

void ASService::inputTextOnLast(string xpath, string text) {
    string xml = dumpXml(ADB::isRoot);
    Node node = getNode(xpath, xml);
    if (node.empty())
        return;

    tap(node);
    delay2(Common::ranNumber(300, 400));

    handleKeyboard(Keyboard::MOVE_LAST);

    delay2(Common::ranNumber(300, 400));


    handleKeyboard(Keyboard::INPUT, text);

    delay2(Common::ranNumber(200, 400));
}


bool ASService::inputText(Node node, string text, bool isClear) {
    tap(node);
    delay2(Common::ranNumber(300, 400));

    if (isClear) {
        clearText();
        delay2(Common::ranNumber(300, 400));
    }

    handleKeyboard(Keyboard::INPUT, text);

    delay2(Common::ranNumber(200, 400));

    return true;
    // DEBUG
//    if (node.nodeInfo == nullptr) return;
//
//    if (isClear) {
//
//    }
//
//    text = Common::spinText(text);
//
//    JNIEnv *env = JExt::getEnv();
//
//    jobject bundleObject = env->NewObject(AServiceVM.bundleClass, AServiceVM.newBundleClass);
//
//    jstring jText = JExt::stringToJstring(text);
//    jstring jAction = JExt::stringToJstring("ACTION_ARGUMENT_SET_TEXT_CHARSEQUENCE");
//
//    env->CallVoidMethod(bundleObject, AServiceVM.putCharSequenceMethod, jAction, jText);
//
//    jint methodId = 0x00200000;
//    env->CallBooleanMethod(node.nodeInfo, AServiceVM.performActionWBundleMethod, methodId,
//                           bundleObject);
//    env->DeleteLocalRef(bundleObject);

}

void ASService::clearText(string xpath) {
    handleKeyboard(Keyboard::CLEAR);
    return;
}

bool ASService::inputEnter(string xpath, string xml) {
    if (!xpath.empty()) {
        Node node = getNode(xpath, xml);
        if (node.empty())
            return false;

        tap(node);
        delay(1);
    }

    if (ADB::isRoot)
        ADB::runShell("input keyevent 66");
    else
        handleKeyboard(Keyboard::ENTER);

    return true;
}

void ASService::swipe(int typeScroll, int countScroll, int duration) {
    for (int i = 0; i < countScroll; ++i) {
        int pointXform = Common::ranNumber(sWidth / 4, sWidth / 4 * 3);
        int pointYfrom = Common::ranNumber(sHeight / 4 * 3, sHeight / 4 * 3 + 50);

        int pointXto = Common::ranNumber(sWidth / 4, sWidth / 4 * 3);
        int pointYto = Common::ranNumber(sHeight / 4, sHeight / 4 + 50);

        if (duration == 0)
            duration = Common::ranNumber(2000000, 2560000) / sHeight;

        if (typeScroll == 2) {
            pointYfrom = Common::ranNumber(sHeight / 4 * 2, sHeight / 4 * 2 + 50);
        } else if (typeScroll == 3) {
            pointYfrom = Common::ranNumber(sHeight / 5 * 4, sHeight / 5 * 4 + 50);
            pointYto = Common::ranNumber(sHeight / 5, sHeight / 5 + 50);
            duration = Common::ranNumber(1000, 1500);
        }

        if (typeScroll > 0)
            swipe(pointXform, pointYfrom, pointXto, pointYto, duration);
        else
            swipe(pointXform, pointYto, pointXto, pointYfrom, duration);
        delay(2);
    }
}

void ASService::swipeH(int typeScroll, int countScroll, int duration) {
    for (int i = 0; i < countScroll; ++i) {
        int pointXform = Common::ranNumber(sWidth / 6 * 5, sWidth / 6 * 5 + 10);
        int pointXto = Common::ranNumber(sWidth / 6, sWidth / 6 + 10);
        int pointY = Common::ranNumber(sHeight / 4, sHeight / 4 * 3);

        if (duration == 0)
            duration = 1000;//(Common::ranNumber(2000000, 2560000) / screenHeight) / 2;

        if (typeScroll > 0)
            swipe(pointXform, pointY, pointXto, pointY, duration);
        else
            swipe(pointXto, pointY, pointXform, pointY, duration);
        delay(2);
    }
}

bool ASService::swipeAndScreenNotChange(int typeScroll, int countScroll, int duration) {
    string xml = dumpXml();
    if (typeScroll == 4 || typeScroll == 5)
        swipeH(typeScroll == 4 ? 1 : -1, countScroll, duration);
    else
        swipe(typeScroll, countScroll, duration);
    return xml == dumpXml();
}

bool ASService::swipeHAndScreenNotChange(int typeScroll, int countScroll, int duration) {
    string xml = dumpXml();
    swipeH(typeScroll, countScroll, duration);
    return xml == dumpXml();
}

void ASService::performAction(Perform perform, int count, int delayTime) {
    JNIEnv *env = JExt::getEnv();
    jobject _instance = getInstance();

    for (int loop = count; loop > 0; loop--) {
        int actionInt = static_cast<int>(perform);
        env->CallBooleanMethod(_instance, AServiceVM.performGlobalAction, actionInt);
        delay(delayTime);
    }
}

void ASService::back(int count, int delay) {
    performAction(Perform::BACK, count, delay);
}

static string keyResource = "00000000-0000-0000-0000-000000000000";

bool ASService::setupResource() {
    if (lstScreen.empty())
        keyResource = "00000000-0000-0000-0000-000000000000";

    for (int loop = 5; loop > 0; loop--) {
        if (isStop) return false;
        updateLine("Setup Resource (" + to_string(loop) + ")");
        try {
            IResult result = OCRequest(
                    "https://api.maxcloudphone.com/Dev/get-resource?key=" + keyResource);

            if (!result.succeeded) {
                if (result.response.empty() || lstScreen.empty()) {
                    delay(5, result.response);
                    throw std::runtime_error("Ex: setupResource - Invalid response");
                }
                return Common::strContains(result.message, "RESPONSE_SUCCESS");
            }

            lstScreen.clear();

            keyResource = result.json.getString("key");

            json jLstResounse = result.json.getInstance("lstResources");
            for (const auto &screenObj: jLstResounse) {
                if (!screenObj.contains("lstParent") || !screenObj.contains("lstAction")) {
                    throw std::runtime_error("Ex: setupResource - Missing keys in JSON response");
                }

                Screen currentScreen;
                for (const auto &parentObj: screenObj["lstParent"]) {
                    ScreenNode currentNodeParent;
                    for (const auto &childObj: parentObj["lstChild"]) {
                        if (childObj.contains("xpath")) {
                            currentNodeParent.lstXpath.push_back(childObj["xpath"]);
                        }
                    }
                    currentScreen.lstParent.push_back(currentNodeParent);
                }

                for (const auto &actionObj: screenObj["lstAction"]) {
                    ScreenAction action;
                    action.xpath = actionObj.value("xpath", "");
                    action.action = actionObj.value("action", "");
                    action.delayS = actionObj.value("delay", 0);
                    currentScreen.lstAction.push_back(action);
                }

                currentScreen.screenName = screenObj.value("name", "");
                currentScreen.socialType = static_cast<SocialType>(screenObj.value("social",
                                                                                   SocialType::UNKNOWN));
                lstScreen.push_back(currentScreen);
            }

            if (!lstScreen.empty()) {
                updateLine("Setup Resource Success");
                return true;
            }
        }
        catch (const std::exception &e) {
            Common::NLog(e.what(), "Ex: setupResource", LogLevel::ERROR);
        }
        delay(10, "Setup Resource Fail");
    }
    return false;
}


string ASService::detectScreen(string xml) {
    vector<Screen> lstQuery = lstScreenSocial.empty() ? lstScreen : lstScreenSocial;

    if (lstQuery.empty()) {
        Common::NLog("lstScreen is empty", "detectScreen");
        return "UNKNOWN_SCREEN";
    }

    if (xml.empty())
        xml = dumpXml();

    if (xml.empty()) {
        Common::NLog("XML is empty", "detectScreen");
        return "UNKNOWN_SCREEN";
    }

    Screen currentScreen;
    currentScreen.screenName = "UNKNOWN_SCREEN";
    currentScreen.countElement = 0;

    try {
        for (const Screen &screenSearch: lstQuery) {
            for (const ScreenNode &nodeParent: screenSearch.lstParent) {
                if (isScreenMatch(nodeParent.lstXpath, xml)) {
                    if (nodeParent.lstXpath.size() < currentScreen.countElement)
                        continue;
                    currentScreen = screenSearch;
                    currentScreen.countElement = nodeParent.lstXpath.size();
                }
            }
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: detectScreen", LogLevel::ERROR);
    }

    Common::NLog(currentScreen.screenName, "detectScreen");

    if (currentScreen.screenName != "UNKNOWN_SCREEN") {
        countUnknownScreen = 0;

        for (ScreenAction element: currentScreen.lstAction) {
            if (!element.xpath.empty()
                && existNode(element.xpath, xml, 1)) {
                tap(element.xpath, xml);
                delay(element.delayS);
                continue;
            }

            if (element.action == "back")
                performAction(Perform::BACK);
            else if (element.action == "home")
                performAction(Perform::HOME);
            else if (element.action == "recent")
                performAction(Perform::RECENT);
            else if (element.action == "swipeup")
                swipe(1, 1, 700);
            else if (element.action == "swipedown")
                swipe(-1);
            else if (element.action == "swipeleft")
                swipeH(1);
            else if (element.action == "swiperight")
                swipeH(-1);
            else
                continue;

            delay(element.delayS);
        }
        return currentScreen.screenName;
    }

    if (isRunning) {
        countUnknownScreen++;
        if (countUnknownScreen >= 8) {
            addUnknownScreen(true, currentAction);
            countUnknownScreen = 0;
        }
    }

    return currentScreen.screenName;
}

bool ASService::isScreenMatch(const std::vector<std::string> &lstXpath, string xml) {
    vector<Node> lstNodes;
    try {
        if (xml.empty())
            xml = dumpXml();
        pugi::xml_document doc;

        if (!doc.load_string(Common::strToLower(xml).c_str()))
            return false;

        for (const auto &xpath: lstXpath) {
            if (!doc.select_single_node(Common::strToLower(xpath).c_str())) {
                return false;
            }
        }
    }
    catch (const std::exception &e) {
//        Common::NLog(e.what(), "Ex: isScreenMatch", LogLevel::DEBUG);
        return false;
    }

    return true;
}

void ASService::addUnknownScreen(bool isLocal, string action) {
    string xml = dumpXml();

    vector<string> lstXpath = getXpath(xml);

    if (lstXpath.empty())
        return;

    if (action.empty())
        action = "noaction";

    string pageSource = action + "\n";

    pageSource += Common::strJoin(lstXpath, "\n");

    auto screenshot = Media::screenShot(Media::OutputType::BASE64);

    std::thread newThread([pageSource, screenshot, isLocal]() {
        JSON bodyJson{};
        bodyJson.set("pageSource", Common::base64_encode(pageSource));
        bodyJson.set("deviceInfo", Device::deviceInfo);

        if (screenshot.first)
            bodyJson.set("base64Image", screenshot.second);

        if (isLocal) {
            Storage::writeFile(
                    DEBUG_FOLDER + Common::currentDatetime("%Y_%m_%d_%H_%M_%S") + ".txt",
                    bodyJson.toString());

            vector<string> lstFIle = Storage::getListFiles(DEBUG_FOLDER);

            if (lstFIle.size() > 5) {
                for (int i = 0; i < lstFIle.size() - 5; i++) {
                    Storage::removePath(lstFIle[i]);
                }
            }

        } else
            OCRequest(MAXCLOUD_ENDPOINT  "Dev/add-unknown-screen",
                      bodyJson.toString());
    });
    newThread.detach();
}


bool ASService::isAppOpened(string package) {
    return Common::strContainsLower(currentPackage, package) ||
           Common::strContainsLower(currentActivity, package);
}

Node ASService::nodeObjectToASNode(jobject currentNode) {
    JNIEnv *env = JExt::getEnv();
    if (currentNode == nullptr) return Node{};
    Node node;

    jobject packageNameObj = env->CallObjectMethod(currentNode, AServiceVM.getPackageNameMethod);
    if (packageNameObj != nullptr) {
        node.package = JExt::jstringToString(
                (jstring) env->CallObjectMethod(packageNameObj, AServiceVM.toStringMethod));
        env->DeleteLocalRef(packageNameObj);
    }

    jobject classObj = env->CallObjectMethod(currentNode, AServiceVM.getClassNameMethod);
    if (classObj != nullptr) {
        node.className = JExt::jstringToString(
                (jstring) env->CallObjectMethod(classObj, AServiceVM.toStringMethod));
        env->DeleteLocalRef(classObj);
    }

    jobject textObj = env->CallObjectMethod(currentNode, AServiceVM.getTextMethod);
    if (textObj != nullptr) {
        node.text = JExt::jstringToString(
                (jstring) env->CallObjectMethod(textObj, AServiceVM.toStringMethod));
        env->DeleteLocalRef(textObj);
    }

    jobject descObj = env->CallObjectMethod(currentNode, AServiceVM.getDescMethod);
    if (descObj != nullptr) {
        node.contentDesc = JExt::jstringToString(
                (jstring) env->CallObjectMethod(descObj, AServiceVM.toStringMethod));
        env->DeleteLocalRef(descObj);
    }

    jobject resourceIdObj = env->CallObjectMethod(currentNode, AServiceVM.getResourceIdMethod);
    if (resourceIdObj != nullptr) {
        node.resourceId = JExt::jstringToString(
                (jstring) env->CallObjectMethod(resourceIdObj, AServiceVM.toStringMethod));
        env->DeleteLocalRef(resourceIdObj);
    }

    jobject rectObject = env->NewObject(AServiceVM.rectClass, AServiceVM.newRectClass);

    env->CallVoidMethod(currentNode, AServiceVM.getBoundsInScreenMethod, rectObject);

    if (rectObject != nullptr) {
        node.centerX = JExt::jint2int(env->CallIntMethod(rectObject, AServiceVM.centerXRectMethod));
        node.centerY = JExt::jint2int(env->CallIntMethod(rectObject, AServiceVM.centerYRectMethod));
        node.top = env->GetIntField(rectObject, AServiceVM.topRectField);
        node.bottom = env->GetIntField(rectObject, AServiceVM.bottomRectField);
        node.left = env->GetIntField(rectObject, AServiceVM.leftRectField);
        node.right = env->GetIntField(rectObject, AServiceVM.rightRectField);

        node.width = node.right - node.left;
        node.height = node.bottom - node.top;

        env->DeleteLocalRef(rectObject);
    }

    node.displayed = env->CallBooleanMethod(currentNode, AServiceVM.isVisibleToUserMethod);
    node.checkable = env->CallBooleanMethod(currentNode, AServiceVM.isCheckableMethod);
    node.checked = env->CallBooleanMethod(currentNode, AServiceVM.isCheckedMethod);
    node.clickable = env->CallBooleanMethod(currentNode, AServiceVM.isClickableMethod);
    node.scrollable = env->CallBooleanMethod(currentNode, AServiceVM.isScrollableMethod);
    node.enabled = env->CallBooleanMethod(currentNode, AServiceVM.isEnabledMethod);
    node.focusable = env->CallBooleanMethod(currentNode, AServiceVM.isFocusableMethod);
    node.longClickable = env->CallBooleanMethod(currentNode, AServiceVM.isLongClickableMethod);
    node.password = env->CallBooleanMethod(currentNode, AServiceVM.isPasswordMethod);
    node.selected = env->CallBooleanMethod(currentNode, AServiceVM.isSelectedMethod);
    node.editable = env->CallBooleanMethod(currentNode, AServiceVM.isEditableMethod);

    return node;
}

string ASService::dumpXml(bool isGenId) {
    JNIEnv *env = JExt::getEnv();

    string xml;

    if (isGenId) {
        for (auto item: lstNodeWId) {
            env->DeleteGlobalRef(item.second);
        }

        lstNodeWId.clear();
    }

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <hierarchy rotation=\"0\">";

    jobject _instance = getInstance();

    jobject currentObject = env->CallObjectMethod(_instance, AServiceVM.getRootInActiveWindow);

    traverseNodeInfo(xml, currentObject, 0, isGenId);

    xml += "</hierarchy>";

    env->DeleteLocalRef(currentObject);

//    if(!Common::isValidXML(xml)){
//        delay2(500);
//        return dumpXml(isGenId);
//    }

    return xml;
}

bool ASService::waitNode(string xpath, string &xml, int timeout) {
    auto start = high_resolution_clock::now();
    while (!isStop) {
        if (!getNode(xpath, xml).empty())
            return true;

        if (Common::hasExceededTimeout(start, timeout))
            break;
        delay(1);
        xml = dumpXml();
    }
    return false;
}

bool ASService::waitNodeHide(string xpath, int timeout) {
    auto start = high_resolution_clock::now();
    while (!isStop) {
        string xml = dumpXml();

        if (getNode(xpath, xml).empty())
            return true;

        if (Common::hasExceededTimeout(start, timeout))
            break;
        delay(1);
    }
    return false;
}

bool ASService::existNode(string xpath, string &xml, int timeout) {
    return !getNode(xpath, xml, timeout).empty();
}

string ASService::existNodes(int timeout, string &xml, vector<string> lstXpath) {
    auto start = high_resolution_clock::now();
    while (!isStop) {
        if (xml.empty())
            xml = dumpXml();

        for (int i = 0; i < lstXpath.size(); ++i) {
            if (!getNode(lstXpath[i], xml).empty())
                return lstXpath[i];
        }

        if (Common::hasExceededTimeout(start, timeout))
            break;
        delay(1);
        xml = "";
    }
    return "";
}

vector<string> ASService::getListText(string &xml) {
    vector<string> lstText;
    try {
        if (xml.empty())
            xml = dumpXml();

        pugi::xml_document doc;
        if (!doc.load_string(Common::strToLower(xml).c_str()))
            return lstText;

        pugi::xpath_node_set arrNodes = doc.select_nodes(Common::strToLower("//*").c_str());
        if (arrNodes.empty())
            return lstText;

        for (int i = 0; i < arrNodes.size(); ++i) {
            pugi::xpath_node currentNode = arrNodes[i];
            pugi::xml_node xmlNode = currentNode.node();

            string text = Common::strTrim(xmlNode.attribute("text").value());
            if (text.empty() || Common::containsItem(lstText, text))
                continue;

            lstText.push_back(text);
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: getListText", LogLevel::ERROR);
    }

    return lstText;
}

vector<Node> ASService::getNodes(string xpath, string &xml, int limitNode) {
    vector<Node> lstNodes;
    try {
        if (xml.empty()) xml = dumpXml();

        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load_string(Common::strToLower(xml).c_str());

        if (!result) {
            return lstNodes;
        }

        pugi::xpath_node_set arrNodes = doc.select_nodes(Common::strToLower(xpath).c_str());

        if (arrNodes.empty()) {
            return lstNodes;
        }

        for (int i = 0; i < arrNodes.size(); ++i) {
            pugi::xpath_node currentNode = arrNodes[i];
            pugi::xml_node xmlNode = currentNode.node();

            Node node;
            node.id = xmlNode.attribute("id").value();

//            auto it = lstNodeWId.find(node.id);
//            if (it != lstNodeWId.end())
//                node.nodeInfo = it->second;

            //node.package = xmlNode.attribute("package").value();
            node.className = xmlNode.attribute("class").value();
            node.text = xmlNode.attribute("text").value();
            node.contentDesc = xmlNode.attribute("content-desc").value();
            node.resourceId = xmlNode.attribute("resource-id").value();
            node.checkable = Common::strToBool(xmlNode.attribute("checkable").value());
            node.checked = Common::strToBool(xmlNode.attribute("checked").value());
            node.clickable = Common::strToBool(xmlNode.attribute("clickable").value());
            node.enabled = Common::strToBool(xmlNode.attribute("enabled").value());
            node.focusable = Common::strToBool(xmlNode.attribute("focusable").value());
            node.scrollable = Common::strToBool(xmlNode.attribute("scrollable").value());
            node.longClickable = Common::strToBool(xmlNode.attribute("long-clickable").value());
            node.selected = Common::strToBool(xmlNode.attribute("selected").value());
            node.password = Common::strToBool(xmlNode.attribute("password").value());
            node.displayed = Common::strToBool(xmlNode.attribute("displayed").value());
            node.bounds = xmlNode.attribute("bounds").value();

            node.width = Common::strToInt(xmlNode.attribute("width").value());
            node.height = Common::strToInt(xmlNode.attribute("height").value());
            node.top = Common::strToInt(xmlNode.attribute("top").value());
            node.left = Common::strToInt(xmlNode.attribute("left").value());
            node.right = Common::strToInt(xmlNode.attribute("right").value());
            node.bottom = Common::strToInt(xmlNode.attribute("bottom").value());
            node.centerX = Common::strToInt(xmlNode.attribute("centerx").value());
            node.centerY = Common::strToInt(xmlNode.attribute("centery").value());
            node.xpath = xpath;

            lstNodes.push_back(node);
            if (limitNode > 0 && lstNodes.size() >= limitNode)
                break;
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: getNodes");
    }

    return lstNodes;
}

vector<Node>
ASService::getNodes(vector<string> lstXpath, string &xml, int limitNode) {
    vector<Node> lstNodes;

    for (int i = 0; i < lstXpath.size(); ++i) {
        string xpath = lstXpath[i];

        vector<Node> lstSubNodes = getNodes(xpath, xml, limitNode);
        lstNodes.insert(lstNodes.end(), lstSubNodes.begin(), lstSubNodes.end());

        if (limitNode > 0) {
            if (lstNodes.size() >= limitNode)
                break;
            limitNode = limitNode - lstNodes.size();
        }
    }

    return lstNodes;
}

Node ASService::getNode(string xpath, string &xml, int timeout) {
    auto start = high_resolution_clock::now();
    while (!isStop) {
        if (xml.empty())
            xml = dumpXml();

        vector<Node> lstNodes = getNodes(xpath, xml, 1);
        if (lstNodes.size() > 0)
            return lstNodes[0];

        if (Common::hasExceededTimeout(start, timeout))
            break;
        delay(1);
        xml = "";
    }
    return Node{};
}

Node ASService::getNode(vector<string> lstXpath, string &xml, int timeout) {
    auto start = high_resolution_clock::now();
    while (!isStop) {
        if (xml.empty())
            xml = dumpXml();

        vector<Node> lstNodes = getNodes(lstXpath, xml, 1);
        if (lstNodes.size() > 0)
            return lstNodes[0];

        if (Common::hasExceededTimeout(start, timeout))
            break;
        delay(1);
        xml = "";
    }
    return Node{};
}

string ASService::getNodeContent(pugi::xml_node xmlNode) {
    std::stringstream ss;
    xmlNode.print(ss);
    std::string nodeContent = ss.str();
    return nodeContent;
}

void
ASService::findNonEmptyTextNodes(const pugi::xml_node &node, const std::string &currentPath,
                                 std::vector<std::string> &lstXpath) {
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
        std::string path = currentPath + "/" + child.name();

        // Kiểm tra xem node có chứa text hoặc content-desc không rỗng không
        string text = child.attribute("text").value();
        string contentDesc = child.attribute("content-desc").value();
        string className = child.attribute("class").value();
        if (!Common::containsItem({"android.widget.EditText", "android.widget.ProgressBar"},
                                  className))
            className = "*";

        if (text != "" || contentDesc != "" || className == "android.widget.ProgressBar" ||
            className == "android.widget.EditText") {
            string attribute = "";
            string value = "";
            string where = "";

            if (!text.empty()) {
                attribute = "text";
                value = text;

                if (Common::strContains(value, "'"))
                    value = "\"" + value + "\"";
                else
                    value = "'" + value + "'";

                where += "@" + attribute + "=" + value + "";
            }

            if (!contentDesc.empty()) {
                attribute = "content-desc";
                value = contentDesc;

                if (Common::strContains(value, "'"))
                    value = "\"" + value + "\"";
                else
                    value = "'" + value + "'";

                if (!where.empty())
                    where += " or ";
                where += "@" + attribute + "=" + value + "";
            }

            string xpath = "//" + className;
            if (!where.empty())
                xpath += "[" + where + "]";

            lstXpath.push_back(xpath);
        }

        // Đệ quy cho các node con
        findNonEmptyTextNodes(child, path, lstXpath);
    }
}

vector<string> ASService::getXpath(string xml) {
    vector<string> lstXpath;

    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_string(Common::strToLower(xml).c_str());
    if (!result)
        return lstXpath;

    findNonEmptyTextNodes(doc.first_child(), "", lstXpath);

    if (lstXpath.size() > 1)
        Common::removeDuplicateItem(lstXpath);
    return lstXpath;
}

void ASService::traverseNodeInfo(string &xml, jobject nodeInfo, int depth, bool isGenId) {
    JNIEnv *env = JExt::getEnv();

    if (nodeInfo == nullptr)
        return;

    Node node = nodeObjectToASNode(nodeInfo);
    if (node.width <= 0 || node.height <= 0)
        return;

    for (int i = 0; i < depth; i++)
        xml += "\t";

    if (node.className.empty())
        node.className = "node";

//    if (isGenId && node.className == "android.widget.EditText") {
//        node.id = Common::strToLower(Common::ranGuid());
//
//        jobject globalRef = env->NewGlobalRef(nodeInfo);
//        lstNodeWId.insert({node.id, globalRef});
//    }

    node.className = Common::strReplace(node.className, "$", "");

    xml += "<" + node.className;
    xml += " id=\"" + node.id + "\"";
    xml += " package=\"" + node.package + "\"";
    xml += " class=\"" + node.className + "\"";
    xml += " text=\"" + Common::replaceSpecialCharXML(node.text) + "\"";
    xml += " content-desc=\"" + Common::replaceSpecialCharXML(node.contentDesc) + "\"";
    xml += " resource-id=\"" + node.resourceId + "\"";
    xml += " checkable=\"" + Common::boolToString(node.checkable) + "\"";
    xml += " checked=\"" + Common::boolToString(node.checked) + "\"";
    xml += " clickable=\"" + Common::boolToString(node.clickable) + "\"";
    xml += " enabled=\"" + Common::boolToString(node.enabled) + "\"";
    xml += " focusable=\"" + Common::boolToString(node.focusable) + "\"";
    xml += " scrollable=\"" + Common::boolToString(node.scrollable) + "\"";
    xml += " long-clickable=\"" + Common::boolToString(node.longClickable) + "\"";
    xml += " selected=\"" + Common::boolToString(node.selected) + "\"";
    xml += " password=\"" + Common::boolToString(node.password) + "\"";
    xml += " displayed=\"" + Common::boolToString(node.displayed) + "\"";
    xml += " bounds=\"[" + to_string(node.left) + "," + to_string(node.top) + "][" +
           to_string(node.right) + "," + to_string(node.bottom) + "]\"";

    xml += " width=\"" + to_string(node.width) + "\"";
    xml += " height=\"" + to_string(node.height) + "\"";
    xml += " top=\"" + to_string(node.top) + "\"";
    xml += " left=\"" + to_string(node.left) + "\"";
    xml += " right=\"" + to_string(node.right) + "\"";
    xml += " bottom=\"" + to_string(node.bottom) + "\"";
    xml += " centerX=\"" + to_string(node.centerX) + "\"";
    xml += " centerY=\"" + to_string(node.centerY) + "\"";
    xml += "> ";

    jint childCount = env->CallIntMethod(nodeInfo, AServiceVM.getChildCountMethod);
    for (int i = 0; i < childCount; i++) {
        jobject childNodeObj = env->CallObjectMethod(nodeInfo, AServiceVM.getChildMethod, i);

        if (childNodeObj != nullptr) {
            traverseNodeInfo(xml, childNodeObj, depth + 1, isGenId);
            env->DeleteLocalRef(childNodeObj);
        }
    }

    for (int i = 0; i < depth; i++)
        xml += " ";

    xml += "</" + node.className + ">";
}


void ASService::disableSelf(string message, bool isClear) {
    JNIEnv *env = JExt::getEnv();

    if (isClear) {
        jobject jContext = getContext();

        jobject appPref = Storage::newPref(jContext, "Config");

        Storage::clearPref(appPref);

        delay2(500);

        JSON json;
        json.set("package", PACKAGE);
        json.set("class_name", PACKAGE ".UI.LoginActivity");
        json.set("new_task", true);

        Device::startActivityByIntent(json);
    }

    Common::NLog(message, "Disable Self", LogLevel::ERROR);

    for (int loop = 20; loop > 0; loop--) {
        showToast(message + " - Please clear data app and try login again");
        delay(5);
    }

    jobject jInstance = getInstance();

    env->CallVoidMethod(jInstance, AServiceVM.disableSelfMethod);
}

void ASService::onEvent(JNIEnv *env, jobject thiz, jobject jEvent) {
    if (AServiceVM.getEventTypeASEvent == nullptr) return;
    int eventType = env->CallIntMethod(jEvent, AServiceVM.getEventTypeASEvent);

    if (eventType == 32) { // TYPE_WINDOW_STATE_CHANGED
        jobject packageNameObj = env->CallObjectMethod(jEvent, AServiceVM.getPackageNameASEvent);
        jobject classNameObj = env->CallObjectMethod(jEvent, AServiceVM.getClassNameASRecord);

        if (packageNameObj == nullptr || classNameObj == nullptr) return;

        currentActivity = JExt::jstringToString(
                (jstring) env->CallObjectMethod(classNameObj, AServiceVM.toStringMethod));

        currentPackage = JExt::jstringToString(
                (jstring) env->CallObjectMethod(packageNameObj, AServiceVM.toStringMethod));

        env->DeleteLocalRef(packageNameObj);
        env->DeleteLocalRef(classNameObj);

        if (Base::isDebug)
            Common::NLog("[" + ASService::currentPackage + "] [" + ASService::currentActivity + "]",
                         "handleASEvent");
    }

    if (eventType == 8 && !Base::isRunning) { // TYPE_VIEW_FOCUSED
        Common::NLog("TYPE_VIEW_FOCUSED", "handleASEvent");
//        jobject source = env->CallObjectMethod(jEvent, AServiceVM.getEventSourceMethod);
//        if (source == nullptr) {
//            Common::NLog("source is null", "handleASEvent");
//            lastEditTextFocus = nullptr;
//            lastEditTextFocusContent = "";
//            return;
//        }
//
//        Node node = nodeObjectToASNode(source);
//
//        if (node.empty()) {
//            Common::NLog("source2 is null", "handleASEvent");
//            env->DeleteLocalRef(source);
//            lastEditTextFocus = nullptr;
//            lastEditTextFocusContent = "";
//            return;
//        }
//
//        if (node.editable) {
//            if (lastEditTextFocus != nullptr) {
//                env->DeleteGlobalRef(lastEditTextFocus);
//            }
//            lastEditTextFocus = env->NewGlobalRef(source);
//            lastEditTextFocusContent = node.text;
//        }
//        else{
//            Common::NLog("source3 is null", "handleASEvent");
//            lastEditTextFocus = nullptr;
//            lastEditTextFocusContent = "";
//        }
//
//        env->DeleteLocalRef(source);
    }

//    if (lstScreen.empty() || ASService::isRunning) return;
//
//    detectScreen();
}

string
ASService::sendEventToHelper(HelperAction action, vector<std::pair<string, string>> lstExtra) {
    JSON json;
    string actionStr = "";

    int timeout = 10;

    if (action == HelperAction::PING_PONG) {
        actionStr = "ACTION_PING_PONG";
        timeout = 5;
    }

    if (action == HelperAction::BACKUP_DEVICE)
        actionStr = "ACTION_BACKUP_DEVICE";

    if (action == HelperAction::RESTORE_DEVICE)
        actionStr = "ACTION_RESTORE_DEVICE";

    if (action == HelperAction::START_VPN)
        actionStr = "ACTION_START_VPN";

    if (action == HelperAction::STOP_VPN)
        actionStr = "ACTION_STOP_VPN";

    json.set("action", PACKAGE_HELPER "." + actionStr);
    json.set("package", PACKAGE_HELPER);
    json.set("class_name", "io.nekohasekai.sagernet.Caller");
    json.set("timeout", timeout);

    if (!lstExtra.empty()) {
        JSON jsonExtra;
        for (auto &item: lstExtra) {
            jsonExtra.set(item.first, item.second);
        }
        json.set("extra", jsonExtra.toNlohmannJson());
    }

    string result = Device::sendBroadcastAndWait(json.toString());

    return result;
}

void ASService::initNotification() {
    JNIEnv *env = JExt::getEnv();
    env->CallStaticVoidMethod(baseVM.jClass, AServiceVM.initNotificationMethod);
}

bool ASService::findImageAndClick(const std::string &templateBase64) {
    // Lấy screenshot dưới dạng base64
    auto [success, base64Image] = Media::screenShot(Media::OutputType::BASE64, 40);
    if (!success) {
        Common::NLog("Failed to capture screenshot");
        return false;
    }

    // Giải mã ảnh từ base64
    cv::Mat sceenShotImage = decodeBase64ToMat(base64Image);
    if (sceenShotImage.empty()) {
        Common::NLog("Failed to decode screenshot from base64");
        return false;
    }

    cv::Mat templateImage = decodeBase64ToMat(templateBase64);
    if (templateImage.empty()) {
        Common::NLog("Failed to decode template from base64");
        return false;
    }

    // Kiểm tra số kênh (đảm bảo cả hai ảnh đều là BGR)
    if (sceenShotImage.channels() != 3 || templateImage.channels() != 3) {
        Common::NLog("Images must be in BGR format with 3 channels");
        return false;
    }

    // Tiền xử lý: Resize nếu ảnh quá lớn
    double scaleFactor = (sceenShotImage.cols > 1080) ? 1080.0 / sceenShotImage.cols : 1.0;
    cv::Mat resizedScreenshot, resizedTemplate;
    cv::resize(sceenShotImage, resizedScreenshot, cv::Size(), scaleFactor, scaleFactor);
    cv::resize(templateImage, resizedTemplate, cv::Size(), scaleFactor, scaleFactor);

    // Chuyển sang grayscale và tiền xử lý
    cv::Mat grayScreenshot, grayTemplate;
    cv::cvtColor(resizedScreenshot, grayScreenshot, cv::COLOR_BGR2GRAY);
    cv::cvtColor(resizedTemplate, grayTemplate, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(grayScreenshot, grayScreenshot, cv::Size(5, 5), 0);
    cv::GaussianBlur(grayTemplate, grayTemplate, cv::Size(5, 5), 0);
    cv::equalizeHist(grayScreenshot, grayScreenshot);
    cv::equalizeHist(grayTemplate, grayTemplate);

    // Template matching
    cv::Mat result;
    cv::matchTemplate(grayScreenshot, grayTemplate, result, cv::TM_CCOEFF_NORMED);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // Tìm vị trí tốt nhất
    double minVal, maxVal;
    cv::Point maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, nullptr, &maxLoc);

    Common::NLog("Best match at: (" + std::to_string(maxLoc.x) + ", " + std::to_string(maxLoc.y) +
                 ") with value " + std::to_string(maxVal));

    // Tự động điều chỉnh ngưỡng
    double threshold = std::max(0.8, maxVal * 0.9);
    if (maxVal < threshold) {
        Common::NLog("Comment button not found, maxVal: " + std::to_string(maxVal));
        return false;
    }

    // Tính tọa độ trung tâm và click
    int centerX = static_cast<int>((maxLoc.x + resizedTemplate.cols / 2) / scaleFactor);
    int centerY = static_cast<int>((maxLoc.y + resizedTemplate.rows / 2) / scaleFactor);

    tap(centerX, centerY);

    return true;
}

cv::Mat ASService::decodeBase64ToMat(const string &base64String) {
    try {
        std::string decodedData = Common::base64_decode(base64String);
        std::vector<char> decodedBytes(decodedData.begin(), decodedData.end());
        return cv::imdecode(cv::Mat(decodedBytes), cv::IMREAD_COLOR);
    } catch (const std::exception &e) {
        Common::NLog("Base64 decode error: " + std::string(e.what()));
        return cv::Mat();
    }
}

