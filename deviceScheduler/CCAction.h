//
//  CCAction.h
//  deviceScheduler
//
//  Created by little Ikarus on 10.07.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCAction__
#define __deviceScheduler__CCAction__

#include "deviceScheduler.h"
#include "CCTask.h"

class CCDeviceFlow;

class CCAction {
public:
    String              actionName;
    CCDeviceFlow*       targetDeviceFlow;
    scheduledTask       validTaskID;
    deviceAction        targetAction;
    scheduledTask       followingTaskID;
    int                 notificationCode;
    String              notificationText;
    infoCode            workflowInfoCode;
    boolean             actionDone;
    
    CCAction(String actionName, infoCode workflowInfoCode);
//    ~CCAction();
    
    void evokeJumpToTask(CCDeviceFlow* targetDeviceFlow, CCTask* validTask, deviceAction targetAction, CCTask* followingTask);
//    , int notificationCode = 0, String notificationText = "");
    void evokeJumpToNextTask(CCDeviceFlow* targetDeviceFlow, CCTask* validTask, deviceAction targetAction);
//    , int notificationCode = 0, String notificationText = "");
    
    void evokeBreak(CCDeviceFlow* targetDeviceFlow, CCTask* validTask);
//    , int notificationCode = 0, String notificationText = "");
    void evokeBreak();
//    int notificationCode = 0, String notificationText = "");
    
    void                setActionDone();

};

#endif /* defined(__deviceScheduler__CCAction__) */
