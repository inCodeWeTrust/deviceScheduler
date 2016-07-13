//
//  CCWorkflow.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 16.06.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCWorkflow.h"




CCWorkflow::CCWorkflow(String workflowName) {
    this->workflowName = workflowName;
    this->countOfDeviceFlows = 0;
    this->countOfFlowControls = 0;
    this->workflowInfo = EVERYTHING_OK;
}
CCWorkflow::~CCWorkflow() {
    for (int i = countOfDeviceFlows - 1; i >= 0; i--) {
        delete deviceFlow[i];
        deviceFlow[i] = NULL;
    }
    for (int i = countOfFlowControls - 1; i >= 0; i--) {
        delete flowControl[i];
        flowControl[i] = NULL;
    }
}

CCDeviceFlow* CCWorkflow::addDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity, float defaultAcceleration, float defaultDeceleration) {
    deviceFlow[countOfDeviceFlows++] = new CCDeviceFlow(deviceFlowName, device, defaultVelocity, defaultAcceleration, defaultDeceleration);
    return deviceFlow[countOfDeviceFlows - 1];
}
CCDeviceFlow* CCWorkflow::addDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity, float defaultAcceleration) {
    deviceFlow[countOfDeviceFlows++] = new CCDeviceFlow(deviceFlowName, device, defaultVelocity, defaultAcceleration, defaultAcceleration);
    return deviceFlow[countOfDeviceFlows - 1];
}
CCDeviceFlow* CCWorkflow::addDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity) {
    deviceFlow[countOfDeviceFlows++] = new CCDeviceFlow(deviceFlowName, device, defaultVelocity, 0.0, 0.0);
    return deviceFlow[countOfDeviceFlows - 1];
}
CCDeviceFlow* CCWorkflow::addDeviceFlow(String deviceFlowName, CCDevice* device) {
    deviceFlow[countOfDeviceFlows++] = new CCDeviceFlow(deviceFlowName, device, 0.0, 0.0, 0.0);
    return deviceFlow[countOfDeviceFlows - 1];
}


CCFlowControl* CCWorkflow::addFlowControl(String controlName, CCControlButton* controlButton) {
    this->flowControl[countOfFlowControls++] = new CCFlowControl(controlName, controlButton);
    return this->flowControl[countOfFlowControls - 1];
}

/*
void CCWorkflow::reviewTasks() {
    for (unsigned char d = 0; d < countOfDevices; d++) {
        device[d]->reviewValues();
    }
}
 */
/*
void CCWorkflow::getAllActions() {
    for (int i = 0; i < countOfControlButtons; i++) {
        getActionsForControlButton(i);
    }
}
*/
/*
void CCWorkflow::getActionsForControlButton(unsigned char theButton) {
    Serial.print(F("[CCDeviceScheduler]: "));
    Serial.print(schedulerName);
    Serial.print(F(": Actions of ControlButton "));
    Serial.print(controlButton[theButton]->getButtonName());
    Serial.println(F(": "));
    for (int i = 0; i < controlButton[theButton]->getCountOfActions(); i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(": targetDevice: "));
        Serial.print(controlButton[theButton]->getAction(i).targetDevice);
        Serial.print(F(", validTask: "));
        Serial.print(controlButton[theButton]->getAction(i).validTask);
        Serial.print(F(", targetAction: "));
        Serial.print(getNameOfDeviceAction(controlButton[theButton]->getAction(i).targetAction));
        Serial.print(F(", followingTask: "));
        Serial.println((int)controlButton[theButton]->getAction(i).followingTask);
    }
    Serial.println();
}
*/
/*
void CCWorkflow::deleteAllActions() {
    for (int i = 0; i < countOfControlButtons; i++) {
        controlButton[i]->deleteActions();
    }
}
*/
/*
void CCWorkflow::postWorkflowInfo(infoCode info) {
    this->workflowInfo = info;
    if (info < 0) {
        this->infoHistory = this->infoHistory + (char)(info + 127);
    } else {
        this->infoHistory = this->infoHistory + (char)info;
    }
}
*/

String CCWorkflow::getNameOfDeviceType(deviceType t) {
    if (t == SERVODEVICE) return "Servo";
    if (t == STEPPERDEVICE) return "Stepper";
    if (t == DCCONTROLLERDEVICE) return "Switch";
    return "unknown";
}
String CCWorkflow::getNameOfTaskEvent(event e) {
    if (e == NONE) return "none";
    if (e == DATE) return "date";
    if (e == BUTTON) return "button";
    if (e == FOLLOW) return "follow";
    if (e == POSITION) return "position";
    if (e == CONTROLBUTTON) return "control button";
    return "unknown";
}
String CCWorkflow::getNameOfState(deviceState s) {
    if (s == SLEEPING) return "sleeping";
    if (s == MOVING) return "moving";
    if (s == MOVE_DONE) return "move done";
    if (s == PENDING_MOVES) return "pending moves";
    return "unknown";
}
String CCWorkflow::getNameOfStoppingMode(stoppingMode s) {
    if (s == STOP_IMMEDIATELY) return "stop immediately";
    if (s == STOP_NORMAL) return "stop normal";
    if (s == STOP_DYNAMIC) return "stop dynamic";
    return "unknown";
}
String CCWorkflow::getNameOfDeviceAction(deviceAction d) {
    if (d == START_NEXT_TASK) return "start";
    if (d == STOP_TASK_AND_SWITCH) return "stop and switch";
    if (d == STOP_TASK) return "stop";
    if (d == STOP_TASK_SHARP_AND_SWITCH) return "stop immediately and switch";
    if (d == STOP_TASK_SHARP) return "stop immediately";
    return "unknown";
}

