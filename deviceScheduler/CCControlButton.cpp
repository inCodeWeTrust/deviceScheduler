//
//  CCControlButton.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 01.04.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "CCControlButton.h"
#include "deviceScheduler.h"





CCControlButton::CCControlButton(unsigned int buttonIndex, String buttonName, unsigned char button_pin, boolean button_activ) {
    
    this->buttonIndex = buttonIndex;
    this->buttonName = buttonName;
    this->button_pin = button_pin;
    this->button_activ = button_activ;
    
    if (button_activ) {
        pinMode(button_pin, INPUT);
    } else {
        pinMode(button_pin, INPUT_PULLUP);
    }
    
    countOfActions = 0;
    
    
    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: setup "));
        Serial.print(buttonName);
        Serial.print(F(", button_pin: "));
        Serial.print(button_pin);
        Serial.print(F(", button_activ: "));
        Serial.print(button_activ);
        Serial.print(F(", at $"));
        Serial.println((long)this, HEX);
    }
    
    

}
CCControlButton::~CCControlButton() {
    free(action);
    countOfActions = 0;
}


void CCControlButton::evokeTaskJump(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction) {
    evokeTaskJumpToTask(targetDevice, validTask, targetAction, validTask + 1);
}
void CCControlButton::evokeTaskJumpToTask(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction, scheduledTask followingTask) {
    action[countOfActions].targetDevice = targetDevice;
    action[countOfActions].validTask = validTask;
    action[countOfActions].targetAction = targetAction;
    action[countOfActions].followingTask = followingTask;

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

void CCControlButton::deleteActions() {
    countOfActions = 0; 
}


boolean CCControlButton::getButtonState() {
    state = digitalRead(button_pin);
    return state;
}

boolean CCControlButton::isActiv() {
    if (getButtonState() == button_activ) {
        return true;
    }
    return false;
}
