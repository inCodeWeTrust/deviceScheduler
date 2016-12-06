//
//  CCControlButton.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 01.04.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//


#include "CCControlButton.h"



CCControlButton::CCControlButton(unsigned int controlIndex, String controlName, unsigned int pin, int mode) {
    
    this->controlIndex = controlIndex;
    this->controlName = controlName;
    this->type = BUTTON;
    this->pin = pin;
    this->mode = mode;
    
    pinMode(pin, mode);
    
    //    this->notificationCode = CONTROLLBUTTON_PRESSED_CODE;
    //    this->notificationText = CONTROLLBUTTON_PRESSED_NOTIFICATION;
    
    countOfActions = 0;
    
    
    if (CONTROLBUTTON_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: setup "));
        Serial.print(controlName);
        Serial.print(F(", pin: "));
        Serial.print(pin);
        Serial.print(F(", mode: "));
        Serial.print(mode);
        if (CONTROLBUTTON_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.println((long)this, HEX);
        }
    }    
}
CCControlButton::~CCControlButton() {
    if (CONTROLBUTTON_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: button "));
        Serial.print(controlName);
        Serial.println(F(" destructed"));
    }
}




void CCControlButton::read() {
    sensorValue = digitalRead(pin);
}




