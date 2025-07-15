//
// Created by octopus on 3/13/25.
//

#ifndef MAXCLOUD_OWRAPPER_H
#define MAXCLOUD_OWRAPPER_H

#include <Base.h>
#include <ASService.h>
#include <RTC.h>
#include "NodeType.h"

class OWrapper : public Base {
public:
    static void executeScriptNoCode(string scriptId);

    static Task executeWithWrapper(JSON json);

private:
    static Task handleAction(JSON json);

    static void sendRTCLog(string log);
};


#endif //MAXCLOUD_OWRAPPER_H
