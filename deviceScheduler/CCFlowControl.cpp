//
//  CCFlowControl.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 12.07.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCFlowControl.h"






CCFlowControl::CCFlowControl(String controlName, CCControlButton* button) {
    this->controlName = controlName;
    this->button = button;
    this->countOfActions = 0;
    
    if (FLOWCONTROL_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCFlowControl]: setup flowControl "));
        Serial.print(controlName);
        if (FLOWCONTROL_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.print((long)this, HEX);
        }
        Serial.println();
    }

}
CCFlowControl::~CCFlowControl() {
    if (FLOWCONTROL_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCFlowControl]: "));
        Serial.print(controlName);
        Serial.print(F(": delete action "));
    }
    for (int i = countOfActions - 1; i >= 0; i--) {
        if (FLOWCONTROL_VERBOSE & BASICOUTPUT) {
            Serial.print(F(" #"));
            Serial.print(i);
        }
        delete action[i];
        action[i] = NULL;
    }
    if (FLOWCONTROL_VERBOSE & BASICOUTPUT) {
        Serial.println();
    }
}

CCAction* CCFlowControl::addAction(String actionName, infoCode workflowInfoCode) {
    Serial.print(F("[CCFlowControl]: add Action for "));
    Serial.println(button->getButtonName());
    
    action[countOfActions++] = new CCAction(actionName, workflowInfoCode);
    return action[countOfActions - 1];
}

void CCControlButton::deleteActions() {
    countOfActions = 0;
}
