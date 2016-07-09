//
//  CCAction.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 10.07.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCAction.h"



CCAction::CCAction(String actionName) {
    this->actionName = actionName;
}

void CCAction::evokeTaskJump(CCDeviceFlow* targetDeviceFlow, scheduledTask validTaskID, deviceAction targetAction, int notificationCode, String notificationText) {
    evokeTaskJumpToTask(targetDeviceFlow, validTaskID, targetAction, validTaskID + 1, notificationCode, notificationText);
}
void CCAction::evokeTaskJumpToTask(CCDeviceFlow* targetDeviceFlow, scheduledTask validTaskID, deviceAction targetAction, scheduledTask followingTaskID, int notificationCode, String notificationText) {
    this->targetDeviceFlow = targetDeviceFlow;
    this->validTaskID = validTaskID;
    this->targetAction = targetAction;
    this->followingTaskID = followingTaskID;
    
    this->notificationCode = notificationCode;
    this->notificationText = notificationText;
    
    this->actionDone = false;
    
    
//    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
        Serial.print(F(", targetDeviceFlow: "));
        //        Serial.print(this->targetDeviceFlow);
        Serial.print(F(", validTaskID: "));
        Serial.print(this->validTaskID);
        Serial.print(F(", targetAction: "));
        Serial.print(this->targetAction);
        Serial.print(F(", followingTaskID: "));
        Serial.print((int)this->followingTaskID);
        Serial.println();
//    }
    
}


void CCAction::evokeBreak(CCDeviceFlow* targetDeviceFlow, scheduledTask validTaskID, int notificationCode, String notificationText) {
    this->targetDeviceFlow = targetDeviceFlow;
    this->validTaskID = validTaskID;
    this->targetAction = BREAK_LOOP;
    this->followingTaskID = 1;
    
    this->notificationCode = notificationCode;
    this->notificationText = notificationText;
    
    this->actionDone = false;
    
    
//    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
        Serial.print(F(", targetDeviceFlow: "));
        //        Serial.print(this->targetDeviceFlow);
        Serial.print(F(", validTaskID: "));
        Serial.println(this->validTaskID);
//    }
    
    
}
void CCAction::evokeBreak(int notificationCode, String notificationText) {
    evokeBreak(NULL, -1, notificationCode, notificationText);
}
