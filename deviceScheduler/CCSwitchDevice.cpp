//
//  CCSwitchDevice.cpp
//  moveServos
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "CCSwitchDevice.h"



CCSwitchDevice::CCSwitchDevice(String deviceName, unsigned char switching_pin, bool defaultState) : CCDevice() {
    this->deviceName = deviceName;
    this->switching_pin = switching_pin;
    this->defaultState = defaultState;
    
    type = SWITCHINGDEVICE;
    state = 0;
    movePointer = 0;
    countOfMoves = 0;
    
//    defaultVelocity = 0;
//    defaultAcceleration = 0;
//    defaultDeceleration = 0;
    
    
    if (CCSwitchDevice_VERBOSE & CCSwitchDevice_BASICOUTPUT) {
        Serial.print(F("[CCSwitchDevice]: setup "));
        Serial.print(deviceName);
        Serial.print(F(", switching_pin: "));
        Serial.print(switching_pin);
        Serial.print(F(", defaultState: "));
        Serial.println(defaultState);
    }

    
    attachDevice();
    
}


CCSwitchDevice::~CCSwitchDevice() {
    deleteMoves();
    detachDevice();
}


void CCSwitchDevice::enableDevice() {}
void CCSwitchDevice::disableDevice() {}

void CCSwitchDevice::attachDevice() {
    pinMode(switching_pin, OUTPUT);
    digitalWrite(switching_pin, defaultState);

    if (CCSwitchDevice_VERBOSE & CCSwitchDevice_BASICOUTPUT) {
        Serial.print(F("[CCSwitchDevice]: "));
        Serial.print(deviceName);
        Serial.println(F(" attached"));
    }
}
void CCSwitchDevice::detachDevice() {
    pinMode(switching_pin, INPUT);
}

void CCSwitchDevice::reviewValues() {}

void CCSwitchDevice::prepareNextMove() {
    
    target = theMove[movePointer]->target;
    
    startEvent = theMove[movePointer]->startEvent;
    stopEvent = theMove[movePointer]->stopEvent;
    startDelay = theMove[movePointer]->startDelay;
    startTime = theMove[movePointer]->startTime;
    timeout = theMove[movePointer]->timeout;
    stopSharply = theMove[movePointer]->stopSharply;
    startButton = theMove[movePointer]->startButton;
    stopButton = theMove[movePointer]->stopButton;
    startButtonState = theMove[movePointer]->startButtonState;
    stopButtonState = theMove[movePointer]->stopButtonState;
    startTriggerDevice = theMove[movePointer]->startTriggerDevice;
    startTriggerMove = theMove[movePointer]->startTriggerMove;
    startTriggerPosition = theMove[movePointer]->startTriggerPosition;
    stopTriggerDevice = theMove[movePointer]->stopTriggerDevice;
    stopTriggerMove = theMove[movePointer]->stopTriggerMove;
    stopTriggerPosition = theMove[movePointer]->stopTriggerPosition;
    
}


void CCSwitchDevice::startMove() {
    digitalWrite(switching_pin, target);
    //    state |= MOVING;
    //    state &= ~MOVING;
    state |= MOVE_DONE;
    
    if (CCSwitchDevice_VERBOSE & CCSwitchDevice_BASICOUTPUT) {
        Serial.print(F("[CCSwitchDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": switchEvent "));
        Serial.print((int)movePointer);
        Serial.print(F(": switch to "));
        Serial.println((bool)target);
    }
}

void CCSwitchDevice::initiateStop() {}
void CCSwitchDevice::stopMoving() {
    digitalWrite(switching_pin, defaultState);
}

void CCSwitchDevice::finishMove() {
    state &= ~MOVING & ~MOVE_DONE;
    
    if (CCSwitchDevice_VERBOSE & CCSwitchDevice_BASICOUTPUT) {
        Serial.print(F(": switchEvent "));
        Serial.print((int)movePointer);
        Serial.print(F(" done: state: "));
        Serial.println((bool)digitalRead(switching_pin));
    }
}


void CCSwitchDevice::driveDynamic() {
}
