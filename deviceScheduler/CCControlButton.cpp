//
//  CCControlButton.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 01.04.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//


#include "CCControlButton.h"



CCControlButton::CCControlButton(unsigned int buttonIndex, String buttonName, unsigned char button_pin, boolean buttonActiv, boolean pullup) {
    
    this->buttonIndex = buttonIndex;
    this->buttonName = buttonName;
    this->button_pin = button_pin;
    this->buttonActiv = buttonActiv;
    this->pullup = pullup;
    
    if (pullup) {
        pinMode(button_pin, INPUT_PULLUP);
    } else {
        pinMode(button_pin, INPUT);
    }
    
//    this->notificationCode = CONTROLLBUTTON_PRESSED_CODE;
//    this->notificationText = CONTROLLBUTTON_PRESSED_NOTIFICATION;
    
    countOfActions = 0;
    
    
    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: setup "));
        Serial.print(buttonName);
        Serial.print(F(", button_pin: "));
        Serial.print(button_pin);
        Serial.print(F(", buttonActiv: "));
        Serial.print(buttonActiv);
        Serial.print(F(", pullup: "));
        Serial.print(pullup);
        Serial.print(F(", at $"));
        Serial.println((long)this, HEX);
    }
    
    action = new buttonAction[8];

}
CCControlButton::~CCControlButton() {
    delete [] action;
    countOfActions = 0;
}


void CCControlButton::evokeTaskJump(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction, int notificationCode, String notificationText) {
    evokeTaskJumpToTask(targetDevice, validTask, targetAction, validTask + 1, notificationCode, notificationText);
}
void CCControlButton::evokeTaskJumpToTask(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction, scheduledTask followingTask, int notificationCode, String notificationText) {
    action[countOfActions].targetDevice = targetDevice;
    action[countOfActions].validTask = validTask;
    action[countOfActions].targetAction = targetAction;
    action[countOfActions].followingTask = followingTask;
    
    action[countOfActions].notificationCode = notificationCode;
    action[countOfActions].notificationText = notificationText;

    action[countOfActions].actionDone = false;
    
    
    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: add Action for "));
        Serial.print(buttonName);
        Serial.print(F(", targetDevice: "));
        Serial.print(action[countOfActions].targetDevice);
        Serial.print(F(", validTask: "));
        Serial.print(action[countOfActions].validTask);
        Serial.print(F(", targetAction: "));
        Serial.print(action[countOfActions].targetAction);
        Serial.print(F(", followingTask: "));
        Serial.print((int)action[countOfActions].followingTask);
        Serial.println();
    }

    countOfActions++;
    
}


void CCControlButton::evokeBreak(schedulerDevice targetDevice, scheduledTask validTask, int notificationCode, String notificationText) {
    action[countOfActions].targetDevice = targetDevice;
    action[countOfActions].validTask = validTask;
    action[countOfActions].targetAction = BREAK_LOOP;
    action[countOfActions].followingTask = 1;
    
    action[countOfActions].notificationCode = notificationCode;
    action[countOfActions].notificationText = notificationText;
    
    action[countOfActions].actionDone = false;
    
    
    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: add Break-Action for "));
        Serial.print(buttonName);
        Serial.print(F(", targetDevice: "));
        Serial.print(action[countOfActions].targetDevice);
        Serial.print(F(", validTask: "));
        Serial.println(action[countOfActions].validTask);
    }
    
    countOfActions++;
    
}
void CCControlButton::evokeBreak(int notificationCode, String notificationText) {
    evokeBreak(-1, -1, notificationCode, notificationText);
}


void CCControlButton::deleteActions() {
    countOfActions = 0; 
}


boolean CCControlButton::readButtonState() {
    state = digitalRead(button_pin);
    activ = (state == buttonActiv);
    
    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_MONITORING) {
        Serial.print(F("[CCControlButton]: "));
        Serial.print(buttonName);
        Serial.print(F(", state: "));
        Serial.print(state);
        Serial.print(F(", activ: "));
        Serial.println(activ);
    }
    

    return activ;
}

boolean CCControlButton::isActiv(){return activ;}
boolean CCControlButton::getState(){return state;}

CCControlButton::buttonAction CCControlButton::getAction(unsigned char a){return action[a];}
void CCControlButton::setActionDone(unsigned char a){action[a].actionDone = true;}
void CCControlButton::resetActionDone(unsigned char a){action[a].actionDone = false;}
String CCControlButton::getButtonName(){return buttonName;}
String CCControlButton::getButtonActiv(){if (buttonActiv) return "HIGH"; return "LOW";}
unsigned char CCControlButton::getCountOfActions(){return countOfActions;}



