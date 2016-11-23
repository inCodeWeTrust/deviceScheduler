//
//  CCControlSensor.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 01.04.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//


#include "CCControlSensor.h"



CCControlSensor::CCControlSensor(unsigned int controlIndex, String controlName, unsigned int pin) {
    
    this->controlIndex = controlIndex;
    this->controlName = controlName;
    this->type = SENSOR;
    this->pin = pin;
    
    this->sensorValue = analogRead(pin);
    this->sensorValue_prev = sensorValue;
    
    //    this->notificationCode = CONTROLLBUTTON_PRESSED_CODE;
    //    this->notificationText = CONTROLLBUTTON_PRESSED_NOTIFICATION;
    
//    countOfActions = 0;

    
    if (CONTROLSENSOR_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCControlSensor]: setup "));
        Serial.print(controlName);
        Serial.print(F(", pin: "));
        Serial.print(pin);
        if (CONTROLBUTTON_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.println((long)this, HEX);
        } else {
            Serial.println();
        }
    }    
}
CCControlSensor::~CCControlSensor() {
    if (CONTROLSENSOR_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCControlSensor]: sensor "));
        Serial.print(controlName);
        Serial.println(F(" destructed"));
    }
}



void CCControlSensor::read() {
    sensorValue = (analogRead(pin) + sensorValue_prev) / 2;
    sensorValue_prev = sensorValue;
}




