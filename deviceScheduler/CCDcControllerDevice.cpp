//
//  CCDcControllerDevice.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 17.03.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include <Arduino.h>
#include <avr/pgmspace.h>

#include "CCDcControllerDevice.h"



CCDcControllerDevice::CCDcControllerDevice(unsigned int deviceIndex, String deviceName, unsigned char switching_pin, boolean switchingPin_activ) : CCDevice() {
    this->deviceIndex = deviceIndex;
    this->deviceName = deviceName;
    this->switching_pin = switching_pin;
    this->switchingPin_activ = switchingPin_activ;
    
    type = SWITCHINGDEVICE;
    state = SLEEPING;
    taskPointer = 0;
    countOfTasks = 0;
    
    //    defaultVelocity = 0;
    //    defaultAcceleration = 0;
    //    defaultDeceleration = 0;
    
    
    if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice]: setup "));
        Serial.print(deviceName);
        Serial.print(F(", switching_pin: "));
        Serial.print(switching_pin);
        Serial.print(F(", switchingPin_activ: "));
        Serial.println(switchingPin_activ);
    }
    
    
    attachDevice();
    
}


CCDcControllerDevice::~CCDcControllerDevice() {
    deleteTasks();
    detachDevice();
}


void CCDcControllerDevice::enableDevice() {}
void CCDcControllerDevice::disableDevice() {}

void CCDcControllerDevice::attachDevice() {
    pinMode(switching_pin, OUTPUT);
    digitalWrite(switching_pin, !switchingPin_activ);
    isActiv = false;
    
    if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice]: "));
        Serial.print(deviceName);
        Serial.println(F(" attached"));
    }
}
void CCDcControllerDevice::detachDevice() {
    pinMode(switching_pin, INPUT);
}

void CCDcControllerDevice::reviewValues() {}

void CCDcControllerDevice::prepareNextTask() {
    
    
    if (state == MOVING) {
        // *** current ratio ***
        currentRatio = currentPosition;
        t0 += switchOnTime;
        switchOffTime -= switchOnTime;
        switchOnTime = 0;
    } else {
        currentRatio = 0;
    }

    target = task[taskPointer]->target;
    velocity = task[taskPointer]->velocity;
    acceleration = task[taskPointer]->acceleration;
    deceleration = task[taskPointer]->deceleration;
    
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
    
    
    // target: dutycycle
    // velocity: pwm frequency
    //      periodendauer = 1 / frequency
    // acceleration: time for ramping up
    // deceleration: time for ramping down
    
    switchingInterval = 1000uL / velocity;
    
    targetOnDuration = target * switchingInterval;
    
    timeForAcceleration = acceleration;
    timeForDeceleration = deceleration;
    timeForAccAndConstSpeed = 0x7FFFFFFF;
    
    switchOnTime = 0;
    
    if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_CALCULATIONDEBUG) {
        Serial.print(F("[CCDcControllerDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": prepare move "));
        Serial.print(taskPointer);
        Serial.print(F(": target power: "));
        Serial.print(target);
        Serial.print(F(", frequency (n/sec): "));
        Serial.print(velocity);
        Serial.print(F(", switchingInterval (ms): "));
        Serial.print(switchingInterval);
        Serial.print(F(", targetOnDuration (ms): "));
        Serial.print(targetOnDuration);
        Serial.print(F(", acceleration: "));
        Serial.print(acceleration);
        Serial.print(F(", deceleration: "));
        Serial.println(deceleration);
    }
    
    
}


void CCDcControllerDevice::startTask() {
    if (target > 1) {     // values valid?
        stopTask();
    }
    else {
        enableDevice();                                             // setup ENABLE-pin of stepper driver
        
        state = MOVING;                                             // tag device as MOVING
        t0 = millis();
        
        switchOnTime = 0;
        switchOffTime = switchOnTime + targetOnDuration;
        currentRatio = 0;
        
        if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_BASICOUTPUT) {
            Serial.print(F("[CCDcControllerDevice]: "));
            Serial.print(deviceName);
            Serial.print(F(", start move: "));
            Serial.println((int)taskPointer);
        }
    }
    operateTask();
}

void CCDcControllerDevice::initiateStop() {
    timeForAccAndConstSpeed = millis() - t0;
}

void CCDcControllerDevice::stopTask() {
    digitalWrite(switching_pin, !switchingPin_activ);
    isActiv = false;
    state = MOVE_DONE;
    
    
    if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": stop task "));
        Serial.print((int)taskPointer);
        Serial.print(F(": off state is "));
        Serial.println(digitalRead(switching_pin));
    }
}

void CCDcControllerDevice::finishTask() {
    state = SLEEPING;
    
    if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_BASICOUTPUT) {
        Serial.print(F("finish: switchEvent "));
        Serial.print((int)taskPointer);
        Serial.print(F(" done: state: "));
        Serial.println((bool)digitalRead(switching_pin));
    }
    disableDevice();
}


void CCDcControllerDevice::operateTask() {
    elapsedTime = millis() - t0;
    
    if (elapsedTime >= switchOnTime) {
        digitalWrite(switching_pin, switchingPin_activ);
        isActiv = true;
        if (elapsedTime < timeForAcceleration) {
            currentPosition = currentRatio + (target - currentRatio) * elapsedTime / (float)timeForAcceleration;
            switchOffTime = switchOnTime + switchingInterval * currentPosition;
            switchOnTime += switchingInterval;
            if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                Serial.print(elapsedTime);
                Serial.print(F(": switched on: accellerating, switch off at "));
                Serial.print(switchOffTime);
                Serial.print(F(", duty cycle: "));
                Serial.print(currentPosition);
                Serial.print(F(", next cycle at "));
                Serial.println(switchOnTime);
            }
            return;
        } else if (elapsedTime < timeForAccAndConstSpeed) {
            currentPosition = target;
            switchOffTime = switchOnTime + targetOnDuration;
            switchOnTime += switchingInterval;
            if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                Serial.print(elapsedTime);
                Serial.print(F(": switched on, switch off at "));
                Serial.print(switchOffTime);
                Serial.print(F(", duty cycle: "));
                Serial.print(currentPosition);
                Serial.print(F(", next cycle at "));
                Serial.println(switchOnTime);
            }
            return;
        } else {
            elapsedTime -= timeForAccAndConstSpeed;
            if (elapsedTime < timeForDeceleration) {
                currentPosition = target * (1 - elapsedTime / (float)timeForDeceleration);
                switchOffTime = switchOnTime + switchingInterval * currentPosition;
                switchOnTime += switchingInterval;
                if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                    Serial.print(elapsedTime);
                    Serial.print(F(": switched on: decellerating, switch off at "));
                    Serial.print(switchOffTime);
                    Serial.print(F(", duty cycle: "));
                    Serial.print(currentPosition);
                    Serial.print(F(", next cycle at "));
                    Serial.println(switchOnTime);
                }
                return;
            }
            // if we didnt return up to here, we are done!
            stopTask();
        }
        
    } else if (elapsedTime > switchOffTime) {
        if (isActiv == true) {
            digitalWrite(switching_pin, !switchingPin_activ);
            isActiv = false;
            if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                Serial.print(elapsedTime);
                Serial.print(F(": switched off at "));
                Serial.println(switchOffTime);
            }
        }
    }
}
