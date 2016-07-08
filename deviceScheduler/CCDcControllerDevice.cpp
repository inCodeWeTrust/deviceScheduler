//
//  CCDcControllerDevice.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 17.03.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//


#include "CCDcControllerDevice.h"



CCDcControllerDevice::CCDcControllerDevice(String deviceName, unsigned char switching_pin, boolean switchingPin_activ) : CCDevice() {
    this->deviceName = deviceName;
    
    this->switching_pin = switching_pin;
    this->switchingPin_activ = switchingPin_activ;
    
    pinMode(switching_pin, OUTPUT);
    digitalWrite(switching_pin, !switchingPin_activ);
    isActiv = false;
    

    this->type = DCCONTROLLERDEVICE;
    this->state = SLEEPING;
    
    
    
    if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice]: setup "));
        Serial.print(deviceName);
        Serial.print(F(", switching_pin: "));
        Serial.print(switching_pin);
        Serial.print(F(", switchingPin_activ: "));
        Serial.println(switchingPin_activ);
    }
}


CCDcControllerDevice::~CCDcControllerDevice() {
    pinMode(switching_pin, INPUT);
}


void CCDcControllerDevice::enableDevice() {}
void CCDcControllerDevice::disableDevice() {}

void CCDcControllerDevice::attachDevice() {}
void CCDcControllerDevice::detachDevice() {}

void CCDcControllerDevice::reviewValues() {}

void CCDcControllerDevice::prepareNextTask() {}

void CCDcControllerDevice::prepareTask(CCTask* nextTask) {

    if (state == MOVING) {
        currentRatio = currentPosition;
        t0 += switchOnTime;
        switchOffTime -= switchOnTime;
        switchOnTime = 0;
    } else {
        currentRatio = 0;
    }

    currentTaskID = nextTask->taskID;
    
    target = nextTask->getTarget();
    velocity = nextTask->getVelocity();
    acceleration = nextTask->getAcceleration();
    deceleration = nextTask->getDeceleration();
    
    startEvent = nextTask->getStartEvent();
    stopEvent = nextTask->getStopEvent();
    startDelay = nextTask->getStartDelay();
    startTime = nextTask->getStartTime();
    timeout = nextTask->getTimeout();
//    startButton = nextTask->getStartButton();
//    stopButton = nextTask->getStopButton();
//    startTriggerDevice = nextTask->getStartTriggerDevice();
//    startTriggerTaskID = nextTask->getStartTriggerTaskID();
//    startTriggerPosition = nextTask->getStartTriggerPosition();
//    stopTriggerDevice = nextTask->getStopTriggerDevice();
//    stopTriggerTaskID = nextTask->getStopTriggerTaskID();
//    stopTriggerPosition = nextTask->getStopTriggerPosition();
    switchTaskPromptly = nextTask->getSwitchTaskPromptly();
    stopping = nextTask->getStopping();
    
    sensor = nextTask->getSensor();
    targetValue = nextTask->getTargetValue();
    approximationCurve = nextTask->getApproximationCurve();
    gap = nextTask->getGap();
    approximation = nextTask->getApproximation();

    
    // target: dutycycle [0 ... 1.0]
    // velocity: pwm frequency [Hz]
    // acceleration: time for ramping up [ms]
    // deceleration: time for ramping down [ms]
    
    switchingInterval = 1000uL / velocity;
    
    targetOnDuration = target * switchingInterval;
    
    timeForAcceleration = acceleration;
    timeForDeceleration = deceleration;
    timeForAccAndConstSpeed = 0x7FFFFFFF;
    
    switchOnTime = 0;
    
    if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_CALCULATIONDEBUG) {
        Serial.print(F("[CCDcControllerDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": prepareing... target power: "));
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
        Serial.print(deceleration);
        if (stopping ==STOP_DYNAMIC) {
            Serial.print(F(", stopping dynamically at: "));
            Serial.print(targetValue);
            Serial.print(F(", approximationCurve: "));
            Serial.print(approximationCurve);
            Serial.print(F(", gap: "));
            Serial.print(gap);
        }
        Serial.println();
    }
    
    
}


void CCDcControllerDevice::startTask() {
    if (target > 1) {                                               // values valid?
        stopTask();
    }
    else {
        state = MOVING;                                             // tag device as MOVING
        t0 = millis();
        
        switchOnTime = 0;
        switchOffTime = switchOnTime + targetOnDuration;
        currentRatio = 0;
        
        if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_BASICOUTPUT) {
            Serial.print(F("[CCDcControllerDevice]: "));
            Serial.print(deviceName);
            Serial.println(F(", starting task"));
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
        Serial.println(F(": stopping task"));
    }
}

void CCDcControllerDevice::finishTask() {
    state = SLEEPING;
    
    if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": task done"));
    }
}


void CCDcControllerDevice::operateTask() {
    elapsedTime = millis() - t0;
    
    if (elapsedTime >= switchOnTime) {
        
        if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
            Serial.print(elapsedTime);
            Serial.print(F(": switch on"));
        }
        
        if (elapsedTime < timeForAcceleration) {
            currentPosition = currentRatio + (target - currentRatio) * elapsedTime / (float)timeForAcceleration;
            
            if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                Serial.print(F(", accelerating"));
            }
            
        } else if (elapsedTime < timeForAccAndConstSpeed) {
            currentPosition = target;
            
            if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                Serial.print(F(", runing regular"));
            }
        } else {
            elapsedTime -= timeForAccAndConstSpeed;
            if (elapsedTime < timeForDeceleration) {
                currentPosition = target * (1 - elapsedTime / (float)timeForDeceleration);
                
                if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                    Serial.print(F(", decelerating"));
                }
                
            } else {
                
                if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                    Serial.print(F(", shall stop"));
                }
                
                stopTask();
                return;
            }
        }
        
        if (stopping == STOP_DYNAMIC) {
            sensorValue = analogRead(sensor);
            if (reversedApproximation) {
                relativePosition = targetValue - sensorValue;
            } else {
                relativePosition = sensorValue - targetValue;
            }
            
            if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                Serial.print(F(", stopping dynamically"));
            }
            
            if (relativePosition <= gap) {
                
                currentPosition = 0;
                targetReachedCounter++;
                
                if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
                    Serial.print(F(", times target was reached: "));
                    Serial.print(targetReachedCounter);
                }
                
                if (targetReachedCounter >= approximation) {
                    stopTask();
                }
                return;
                
            } else {
                
                // equitation first order:
                // y = -b / (x - a) + 1; (1)
                //
                // crossPoint: y = 0 ==>
                // 0 = -b / (x - a) + 1; b / (x - a) = 1;
                // b = x - a; x = a + b;
                // crossPoint at x = a + b: cp = a + b ==> a = cp - b
                //
                // in 1:
                // y = -b / (x - cp + b) + 1 = b / (cp - x - b) + 1
                
                // currentPosition *= approximationCurve / (gap * gap - relativePosition * relativePosition - approximationCurve) + 1;
                currentPosition *= approximationCurve / (gap - relativePosition - approximationCurve) + 1;
                
                targetReachedCounter = 0;
                
            }
        }
        
        digitalWrite(switching_pin, switchingPin_activ);
        isActiv = true;
        
        switchOffTime = switchOnTime + switchingInterval * currentPosition;
        switchOnTime += switchingInterval;
        if (CCDcControllerDevice_VERBOSE & CCDcControllerDevice_MOVEMENTDEBUG) {
            Serial.print(F(", scheduled off at "));
            Serial.print(switchOffTime);
            Serial.print(F(", duty cycle: "));
            Serial.print(currentPosition);
            Serial.print(F(", next cycle at "));
            Serial.println(switchOnTime);
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

void CCDcControllerDevice::getReadOut(byte theReadOut) {}

