//
//  CCDcControllerDevice_fullBridge.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 17.03.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//


#include "CCDcControllerDevice_fullBridge.h"



CCDcControllerDevice_fullBridge::CCDcControllerDevice_fullBridge(String deviceName, unsigned char switching_A_pin, boolean switchingPin_A_activ, unsigned char switching_B_pin, boolean switchingPin_B_activ) : CCDcControllerDevice(deviceName, switching_A_pin, switchingPin_A_activ) {
    
    this->switching_B_pin = switching_B_pin;
    this->switchingPin_B_activ = switchingPin_B_activ;
    
    pinMode(switching_B_pin, OUTPUT);
    digitalWrite(switching_B_pin, !switchingPin_B_activ);
    isActiv_B = false;
        
    if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice_fullBridge]: setup "));
        Serial.print(deviceName);
        Serial.print(F(", switching_A_pin: "));
        Serial.print(switching_pin);
        Serial.print(F(", switchingPin_A_activ: "));
        Serial.print(switchingPin_activ);
        Serial.print(F(", switching_B_pin: "));
        Serial.print(switching_B_pin);
        Serial.print(F(", switchingPin_B_activ: "));
        Serial.println(switchingPin_B_activ);    }
}


CCDcControllerDevice_fullBridge::~CCDcControllerDevice_fullBridge() {
    pinMode(switching_pin, INPUT);
}



void CCDcControllerDevice_fullBridge::prepareNextTask() {}
    void CCDcControllerDevice_fullBridge::prepareTask(CCTask *nextTask) {
    
    
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
    
    
    switchTaskPromptly = nextTask->getSwitchTaskPromptly();
    stopping = nextTask->getStopping();
    
    sensor = nextTask->getSensor();
    targetValue = nextTask->getTargetValue();
    approximationCurve = nextTask->getApproximationCurve();
    gap = nextTask->getGap();
    approximation = nextTask->getApproximation();
    
    

    // target: dutycycle
    // velocity: pwm frequency
    // switchingInterval: periodendauer = 1 / frequency
    // acceleration: time for ramping up
    // deceleration: time for ramping down
    
    switchingInterval = 1000uL / velocity;
    
    targetOnDuration = target * switchingInterval;
    
    timeForAcceleration = acceleration;
    timeForDeceleration = deceleration;
    timeForAccAndConstSpeed = 0x7FFFFFFF;
    
    switchOnTime = 0;
    
    if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_CALCULATIONDEBUG) {
        Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
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
        Serial.println(deceleration);
    }
    
    
}


void CCDcControllerDevice_fullBridge::startTask() {
    if (target > 1) {                                               // values valid?
        stopTask();
    }
    else {
        state = MOVING;                                             // tag device as MOVING
        t0 = millis();
        
        switchOnTime = 0;
        switchOffTime = switchOnTime + targetOnDuration;
        currentRatio = 0;
        
        if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_BASICOUTPUT) {
            Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
            Serial.print(deviceName);
            Serial.println(F(", starting task"));
        }
    }
    operateTask();
}

void CCDcControllerDevice_fullBridge::initiateStop() {
    timeForAccAndConstSpeed = millis() - t0;
}

void CCDcControllerDevice_fullBridge::stopTask() {
    digitalWrite(switching_pin, !switchingPin_activ);
    isActiv = false;
    state = MOVE_DONE;
    
    
    if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
        Serial.print(deviceName);
        Serial.println(F(": stopping task"));
        Serial.print(F(": off state is "));
        Serial.println(digitalRead(switching_pin));
    }
}

void CCDcControllerDevice_fullBridge::finishTask() {
    state = SLEEPING;
    
    if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
        Serial.print(deviceName);
        Serial.println(F(": task done"));
    }
}


void CCDcControllerDevice_fullBridge::operateTask() {
    elapsedTime = millis() - t0;
    
    if (elapsedTime >= switchOnTime) {
        
        if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_MOVEMENTDEBUG) {
            Serial.print(elapsedTime);
            Serial.print(F(": switched on"));
        }
        
        if (elapsedTime < timeForAcceleration) {
            currentPosition = currentRatio + (target - currentRatio) * elapsedTime / (float)timeForAcceleration;
            
            if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_MOVEMENTDEBUG) {
                Serial.print(F(", accelerating"));
            }
            
        } else if (elapsedTime < timeForAccAndConstSpeed) {
            currentPosition = target;
            
        } else {
            elapsedTime -= timeForAccAndConstSpeed;
            
            if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_MOVEMENTDEBUG) {
                Serial.print(F(", decelerating"));
            }
            
            if (elapsedTime < timeForDeceleration) {
                currentPosition = target * (1 - elapsedTime / (float)timeForDeceleration);
            } else {
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
        
        if (currentPosition > 0) {
            digitalWrite(switching_pin, switchingPin_activ);
            isActiv = true;
        } else {
            digitalWrite(switching_B_pin, switchingPin_B_activ);
            isActiv_B = true;
        }
        
        switchOffTime = switchOnTime + switchingInterval * currentPosition;
        switchOnTime += switchingInterval;
        if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_MOVEMENTDEBUG) {
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
            digitalWrite(switching_B_pin, !switchingPin_B_activ);
            isActiv = false;
            isActiv_B = false;
            
            if (CCDcControllerDevice_fullBridge_VERBOSE & CCDcControllerDevice_fullBridge_MOVEMENTDEBUG) {
                Serial.print(elapsedTime);
                Serial.print(F(": switched off at "));
                Serial.println(switchOffTime);
            }
        }
    }
}

void CCDcControllerDevice_fullBridge::getReadOut(byte theReadOut) {}
