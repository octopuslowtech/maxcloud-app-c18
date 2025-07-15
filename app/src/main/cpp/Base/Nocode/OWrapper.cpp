//
// Created by octopus on 3/13/25.
//

#include "OWrapper.h"

void OWrapper::executeScriptNoCode(std::string scriptId) {
    if (scriptId.empty()) {
        Base::updateLine("Script Id is empty");
        return;
    }

    // get json script here
    JSON jsonScript = {
            {"scriptId", scriptId}
    };

    JSON jsonEdges = jsonScript.getInstance("edge");
    JSON jsonNodes = jsonScript.getInstance("node");
    JSON jsonVariables = jsonScript.getInstance("variable");

    for (auto item: jsonEdges.getList("data")) {

    }
}

Task OWrapper::executeWithWrapper(JSON json) {
    auto start = std::chrono::steady_clock::now();

    Common::NLog(json.toString(), "executeWithWrapper");

    int delayBefore = json.getInt("delayBefore");

    delay(delayBefore);

    Task taskResult = handleAction(json);

    int delayAfter = json.getInt("delayAfter");

    delay(delayAfter);

    if (RTC::isEmpty() || taskResult != Task::DONE) return taskResult;

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    Common::NLog("Total execution time: " + std::to_string(duration) + " ms", "executeWithWrapper");

    string formatLog = "Thực thi xong trong " + std::to_string(duration) + " ms";

    sendRTCLog(formatLog);
    return taskResult;
}

Task OWrapper::handleAction(JSON json) {
    NodeType nodeType = static_cast<NodeType>(json.getInt("type"));

    if (nodeType == NodeType::BACK) {
        ASService::performAction(Perform::BACK, 1, 1);
        return Task::DONE;
    }

    if (nodeType == NodeType::HOME) {
        ASService::performAction(Perform::HOME, 1, 1);
        return Task::DONE;
    }

    if (nodeType == NodeType::RECENT) {
        ASService::performAction(Perform::RECENT, 1, 1);
        return Task::DONE;
    }

    // tap, input, swipe

    if (nodeType == NodeType::START_APP) {
        string packageName = json.getString("packageName");
        Device::openApp(packageName);
        return Task::DONE;
    }

    //STOP_APP = 4,
    //    INSTALL_APP = 5,
    //    UNINSTALL_APP = 6,
    //    IS_INSTALL_APP = 7,

    if (nodeType == NodeType::STOP_APP) {
        string packageName = json.getString("packageName");
        bool result = ASAction::closeAppAction(packageName);
        return result ? Task::DONE : Task::FAIL;
    }

    if (nodeType == NodeType::INSTALL_APP) {
        string path = json.getString("path");
        string message = json.getString("message");

        // bool result and result task

        bool result = ASAction::installApkAction(path, message, "", true);

        return result ? Task::DONE : Task::FAIL;
    }

    if (nodeType == NodeType::UNINSTALL_APP) {
        return Task::DONE;
    }

    if (nodeType == NodeType::IS_INSTALL_APP) {
        string packageName = json.getString("packageName");

        return Device::existPackage(packageName) ? Task::DONE : Task::FAIL;
    }


    return Task::FAIL;
}


void OWrapper::sendRTCLog(string log) {
    auto lstRTC = RTC::getRTCInstances();
    size_t rtcCount = lstRTC.size();

#pragma omp parallel for
    for (size_t i = 0; i < rtcCount; ++i) {
        lstRTC[i]->sendToChannel(ReportControlType::EXPORT_LOG, log);
    }
}