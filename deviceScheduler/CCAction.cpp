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
    
    if (ACTION_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCAction]: setup action "));
        Serial.print(actionName);
        Serial.print(F(" with workflowInfo: "));
        Serial.print((int)workflowInfoCode);
        if (ACTION_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.print((long)this, HEX);
        }
        Serial.println();
    }

}

void CCAction::evokeJumpToNextTask(CCDeviceFlow* targetDeviceFlow, CCTask* validTask, deviceAction targetAction) {
    this->targetDeviceFlow = targetDeviceFlow;
    this->validTaskID = validTask->getTaskID();
    this->targetAction = targetAction;
    this->followingTaskID = validTaskID + 1;
    
//    this->notificationCode = notificationCode;
//    this->notificationText = notificationText;
    
    
    //    if (CONTROLBUTTON_VERBOSE & BASICOUTPUT) {
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
        this->validTaskID = validTask->getTaskID();
    }
    this->targetAction = targetAction;
    this->followingTaskID = followingTask->getTaskID();
    
//    this->notificationCode = notificationCode;
//    this->notificationText = notificationText;
    
    
//    if (CONTROLBUTTON_VERBOSE & BASICOUTPUT) {
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
    
    
    
//    if (CONTROLBUTTON_VERBOSE & BASICOUTPUT) {
        Serial.print(F(", targetDeviceFlow: "));
        //        Serial.print(this->targetDeviceFlow);
        Serial.print(F(", validTaskID: "));
        Serial.println(this->validTaskID);
//    }
    
    
}
void CCAction::evokeBreak() {
    evokeBreak(NULL, NULL);
}



String CCAction::getName(){return actionName;}
scheduledTask CCAction::getValidTaskID(){return validTaskID;}
deviceAction CCAction::getTargetAction(){return targetAction;}
scheduledTask CCAction::getFollowingTaskID(){return followingTaskID;}
int CCAction::getNotificationCode(){return notificationCode;}
String CCAction::getNotificationText(){return notificationText;}
infoCode CCAction::getWorkflowInfoCode(){return workflowInfoCode;}
boolean CCAction::getActionDone(){return actionDone;}
void CCAction::setActionDone(boolean d){actionDone = d;}

