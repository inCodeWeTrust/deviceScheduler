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
    this->countOfControls = 0;
    this->countOfFlowControls = 0;
    this->workflowInfo = EVERYTHING_OK;
    
    if (WORKFLOW_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCWorkflow]: setup workflow "));
        Serial.print(workflowName);
        if (WORKFLOW_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.print((long)this, HEX);
        }
        Serial.println();
    }
}
CCWorkflow::~CCWorkflow() {
    for (int df = countOfDeviceFlows - 1; df >= 0; df--) {
        if (WORKFLOW_VERBOSE & BASICOUTPUT) {
            Serial.print(F("[CCWorkflow]: "));
            Serial.print(workflowName);
            Serial.print(F(": delete deviceFlow: "));
            Serial.println(deviceFlow[df]->getName());
        }
        delete deviceFlow[df];
        deviceFlow[df] = NULL;
    }
    
    for (int fc = countOfFlowControls - 1; fc >= 0; fc--) {
        if (WORKFLOW_VERBOSE & BASICOUTPUT) {
            Serial.print(F("[CCWorkflow]: "));
            Serial.print(workflowName);
            Serial.print(F(": delete flowControl: "));
            Serial.println(flowControl[fc]->getName());
        }
        delete flowControl[fc];
        flowControl[fc] = NULL;
    }
    
    if (WORKFLOW_VERBOSE & BASICOUTPUT) {
        Serial.println();
    }
}

CCDeviceFlow* CCWorkflow::addDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity, float defaultAcceleration, float defaultDeceleration) {
    deviceFlow[countOfDeviceFlows] = new CCDeviceFlow(deviceFlowName, device, defaultVelocity, defaultAcceleration, defaultDeceleration, countOfDeviceFlows);
    countOfDeviceFlows++;
    return deviceFlow[countOfDeviceFlows - 1];
}

CCControl* CCWorkflow::addControl(CCControl* control) {
    this->control[this->countOfControls++] = control;
    return this->control[this->countOfControls - 1];
}

CCFlowControl* CCWorkflow::addFlowControl(String flowControlName, CCControl* control, comparingMode comparing, int target) {
    this->flowControl[countOfFlowControls++] = new CCFlowControl(flowControlName, control, comparing, target);
    return this->flowControl[countOfFlowControls - 1];
}



String CCWorkflow::getName(){return workflowName;}
unsigned int CCWorkflow::getCountOfDeviceFlows(){return countOfDeviceFlows;}
unsigned int CCWorkflow::getCountOfFlowControls(){return countOfFlowControls;}
unsigned int CCWorkflow::getCountOfControls(){return countOfControls;}
workflowInfoCode CCWorkflow::getWorkflowInfo(){return workflowInfo;}
void CCWorkflow::setWorkflowInfo(workflowInfoCode c){workflowInfo = c;}

/*
void CCWorkflow::postWorkflowInfo(workflowInfoCode info) {
    this->workflowInfo = info;
    if (info < 0) {
        this->infoHistory = this->infoHistory + (char)(info + 127);
    } else {
        this->infoHistory = this->infoHistory + (char)info;
    }
}
 */
