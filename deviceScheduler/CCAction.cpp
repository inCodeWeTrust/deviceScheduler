//
//  CCAction.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 10.07.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCAction.h"



CCAction::CCAction(String actionName, infoCode workflowInfoCode) {
    this->actionName = actionName;
    this->targetDeviceFlow = NULL;
    this->validTaskID = -1;
    this->targetAction = DO_NOTHING;
    this->followingTaskID = -1;
    this->workflowInfoCode = workflowInfoCode;
    this->actionDone = false;
}

void CCAction::evokeJumpToNextTask(CCDeviceFlow* targetDeviceFlow, CCTask* validTask, deviceAction targetAction) {
    this->targetDeviceFlow = targetDeviceFlow;
    this->validTaskID = validTask->taskID;
    this->targetAction = targetAction;
    this->followingTaskID = validTaskID + 1;
    
//    this->notificationCode = notificationCode;
//    this->notificationText = notificationText;
    
    
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
void CCAction::evokeJumpToTask(CCDeviceFlow* targetDeviceFlow, CCTask* validTask, deviceAction targetAction, CCTask* followingTask) {
    this->targetDeviceFlow = targetDeviceFlow;
    if (validTask == NULL) {
        this->validTaskID = -1;
    } else {
        this->validTaskID = validTask->taskID;
    }
    this->targetAction = targetAction;
    this->followingTaskID = followingTask->taskID;
    
//    this->notificationCode = notificationCode;
//    this->notificationText = notificationText;
    
    
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


void CCAction::evokeBreak(CCDeviceFlow* targetDeviceFlow, CCTask* validTask) {
    this->targetDeviceFlow = targetDeviceFlow;
    this->validTaskID = validTaskID;
    this->targetAction = BREAK_LOOP;
    this->followingTaskID = 1;
    
//    this->notificationCode = notificationCode;
//    this->notificationText = notificationText;
    
    
    
//    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
        Serial.print(F(", targetDeviceFlow: "));
        //        Serial.print(this->targetDeviceFlow);
        Serial.print(F(", validTaskID: "));
        Serial.println(this->validTaskID);
//    }
    
    
}
void CCAction::evokeBreak() {
    evokeBreak(NULL, NULL);
}

