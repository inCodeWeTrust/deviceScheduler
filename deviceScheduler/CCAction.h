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
    boolean             actionDone;
    
    CCAction(String actionName);
    ~CCAction();
    
    void evokeTaskJumpToTask(CCDeviceFlow* targetDeviceFlow, scheduledTask validTaskID, deviceAction targetAction, scheduledTask followingTaskID, int notificationCode = 0, String notificationText = "");
    void evokeTaskJump(CCDeviceFlow* targetDeviceFlow, scheduledTask validTaskID, deviceAction targetAction, int notificationCode = 0, String notificationText = "");
    
    void evokeBreak(CCDeviceFlow* targetDeviceFlow, scheduledTask validTaskID, int notificationCode = 0, String notificationText = "");
    void evokeBreak(int notificationCode = 0, String notificationText = "");

    
    void                setActionDone();

};

#endif /* defined(__deviceScheduler__CCAction__) */
