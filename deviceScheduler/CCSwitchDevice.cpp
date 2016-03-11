//
//  CCSwitchDevice.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "CCSwitchDevice.h"



CCSwitchDevice::CCSwitchDevice(unsigned int deviceIndex, String deviceName, unsigned char switching_pin, boolean defaultState) : CCDevice() {
    this->deviceIndex = deviceIndex;
    this->deviceName = deviceName;
    this->switching_pin = switching_pin;
    this->defaultState = defaultState;
    
    type = SWITCHINGDEVICE;
    state = SLEEPING;
    taskPointer = 0;
    countOfTasks = 0;
    
//    defaultVelocity = 0;
//    defaultAcceleration = 0;
//    defaultDeceleration = 0;
    
    
    if (CCSWITCHDEVICE_VERBOSE & CCSWITCHDEVICE_BASICOUTPUT) {
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
    deleteTasks();
    detachDevice();
}


void CCSwitchDevice::enableDevice() {}
void CCSwitchDevice::disableDevice() {}

void CCSwitchDevice::attachDevice() {
    pinMode(switching_pin, OUTPUT);
    digitalWrite(switching_pin, defaultState);

    if (CCSWITCHDEVICE_VERBOSE & CCSWITCHDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCSwitchDevice]: "));
        Serial.print(deviceName);
        Serial.println(F(" attached"));
    }
}
void CCSwitchDevice::detachDevice() {
    pinMode(switching_pin, INPUT);
}

void CCSwitchDevice::reviewValues() {}

void CCSwitchDevice::prepareNextTask() {
    
    target = task[taskPointer]->target;
    
    startEvent = task[taskPointer]->startEvent;
    stopEvent = task[taskPointer]->stopEvent;
    switchTaskPromptly = task[taskPointer]->switchTaskPromptly;
    startDelay = task[taskPointer]->startDelay;
    startTime = task[taskPointer]->startTime;
    timeout = task[taskPointer]->timeout;
    stopping = task[taskPointer]->stopping;
    startButton = task[taskPointer]->startButton;
    stopButton = task[taskPointer]->stopButton;
    startButtonState = task[taskPointer]->startButtonState;
    stopButtonState = task[taskPointer]->stopButtonState;
    startTriggerDevice = task[taskPointer]->startTriggerDevice;
    startTriggerTask = task[taskPointer]->startTriggerTask;
    startTriggerPosition = task[taskPointer]->startTriggerPosition;
    stopTriggerDevice = task[taskPointer]->stopTriggerDevice;
    stopTriggerTask = task[taskPointer]->stopTriggerTask;
    stopTriggerPosition = task[taskPointer]->stopTriggerPosition;
    
    
    Serial.print(F("[CCSwitchDevice]: "));
    Serial.print(deviceName);
    Serial.print(F(": my start is"));
    Serial.print((int)startEvent);
    Serial.print(F(": my stop is"));
    Serial.println((int)stopEvent);
    
    
}


void CCSwitchDevice::startTask() {
    digitalWrite(switching_pin, target);
    state = MOVING;
    
    if (CCSWITCHDEVICE_VERBOSE & CCSWITCHDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCSwitchDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": start "));
        Serial.print((int)taskPointer);
        Serial.print(F(": switch to "));
        Serial.print(target);
        Serial.print(F(": read it "));
        Serial.println(digitalRead(switching_pin));
    }
}

void CCSwitchDevice::initiateStop() {
    stopTask();
}

void CCSwitchDevice::stopTask() {
    digitalWrite(switching_pin, defaultState);
    state = MOVE_DONE;

    
    
    digitalWrite(switching_pin, defaultState);
    if (CCSWITCHDEVICE_VERBOSE & CCSWITCHDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCSwitchDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": stop "));
        Serial.print((int)taskPointer);
        Serial.print(F(": switch to "));
        Serial.print(defaultState);
        Serial.print(F(": read it "));
        Serial.println(digitalRead(switching_pin));
    }
}

void CCSwitchDevice::finishTask() {
    state = SLEEPING;
    
    if (CCSWITCHDEVICE_VERBOSE & CCSWITCHDEVICE_BASICOUTPUT) {
        Serial.print(F("finish: switchEvent "));
        Serial.print((int)taskPointer);
        Serial.print(F(" done: state: "));
        Serial.println((bool)digitalRead(switching_pin));
    }
    detachDevice();
}


void CCSwitchDevice::operateTask() {
}
