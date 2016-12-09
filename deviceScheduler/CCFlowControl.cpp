//
//  CCFlowControl.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 12.07.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCFlowControl.h"






CCFlowControl::CCFlowControl(String controlName, CCControl* control, comparingMode comparing, int target) {
    this->verbosity = NO_OUTPUT;
    
    this->controlName = controlName;
    this->control = control;
    this->comparing = comparing;
    this->target = target;
    this->countOfActions = 0;
    
    //        Serial.print(F("[CCFlowControl]: setup flowControl "));
    //        Serial.print(controlName);
    //        if (verbosity & MEMORYDEBUG) {
    //            Serial.print(F(", at $"));
    //            Serial.print((long)this, HEX);
    //        }
    //        Serial.println();

}
CCFlowControl::~CCFlowControl() {
    if (verbosity & BASICOUTPUT) {
        Serial.print(F("[CCFlowControl]: "));
        Serial.print(controlName);
        Serial.print(F(": delete action "));
    }
    for (int i = countOfActions - 1; i >= 0; i--) {
        if (verbosity & BASICOUTPUT) {
            Serial.print(F(" #"));
            Serial.print(i);
        }
        delete action[i];
        action[i] = NULL;
    }
    if (verbosity & BASICOUTPUT) {
        Serial.println();
    }
}

CCAction* CCFlowControl::addAction(String actionName, workflowInfoCode workflowInfo) {
    Serial.print(F("[CCFlowControl]: add Action for "));
    Serial.println(control->getName());
    
    action[countOfActions++] = new CCAction(actionName, workflowInfo);
    return action[countOfActions - 1];
}

bool CCFlowControl::needsToFire() {
    switch (comparing) {
        case IS:
            return control->is(target);
            break;
        case IS_NOT:
            return control->isNot(target);
            break;
        case IS_GREATER_THEN:
            return control->isGreaterThen(target);
            break;
        case IS_SMALLER_THEN:
            return control->isSmallerThen(target);
            break;
    }
    return control->is(target);
}

comparingMode CCFlowControl::getComparing() {return comparing;}
int CCFlowControl::getTarget() {return target;}
String CCFlowControl::getName(){return controlName;}
unsigned int CCFlowControl::getCountOfActions(){return countOfActions;}

void CCFlowControl::setVerbosity(int verbosity) {this->verbosity = verbosity;}

