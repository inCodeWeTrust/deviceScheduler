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
CCControlButton::~CCControlButton() {}

void CCControlButton::addAction(unsigned char targetDevice, unsigned char validTask, deviceAction targetAction, char taskAdvance) {
    action[countOfActions].targetDevice = targetDevice;
    action[countOfActions].validTask = validTask;
    action[countOfActions].targetAction = targetAction;
    action[countOfActions].taskAdvance = taskAdvance;

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
        Serial.print(F(", taskAdvance: "));
        Serial.print((int)action[countOfActions].taskAdvance);
        Serial.println();
    }

    countOfActions++;
    
}

boolean CCControlButton::getButtonState() {
    state = digitalRead(button_pin);
    return state;
}

boolean CCControlButton::isActiv() {
    if (getButtonState() == button_activ) {
        return true;
    } else {
        return false;
    }
}
