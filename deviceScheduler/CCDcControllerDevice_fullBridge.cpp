//
//  CCDcControllerDevice_fullBridge.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 17.03.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//


#include "CCDcControllerDevice_fullBridge.h"



CCDcControllerDevice_fullBridge::CCDcControllerDevice_fullBridge(const String deviceName, const unsigned int switching_A_pin, const bool switchingPin_A_active, const unsigned int switching_B_pin, const bool switchingPin_B_active) : CCDcControllerDevice(deviceName, switching_A_pin, switchingPin_A_active), switching_B_pin(switching_B_pin), switchingPin_B_active(switchingPin_B_active) {
        
    pinMode(switching_B_pin, OUTPUT);
    digitalWrite(switching_B_pin, !switchingPin_B_active);
    isActive_B = false;
    
    
    //        Serial.print(F("[CCDcControllerDevice_fullBridge]: setup "));
    //        Serial.print(deviceName);
    //        Serial.print(F(", switching_A_pin: "));
    //        Serial.print(switching_pin);
    //        Serial.print(F(", switchingPin_A_active: "));
    //        Serial.print(switchingPin_active);
    //        Serial.print(F(", switching_B_pin: "));
    //        Serial.print(switching_B_pin);
    //        Serial.print(F(", switchingPin_B_active: "));
    //        Serial.println(switchingPin_B_active);
    //        if (verbosity & MEMORYDEBUG) {
    //            Serial.print(F(", at $"));
    //            Serial.print((long) this, HEX);
    //        }
    //        Serial.println();

}


CCDcControllerDevice_fullBridge::~CCDcControllerDevice_fullBridge() {
    pinMode(switching_pin, INPUT);
    
    if (verbosity & BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice_fullBridge]: device "));
        Serial.print(deviceName);
        Serial.println(F(" destructed"));
    }
}

deviceInfoCode CCDcControllerDevice_fullBridge::reviewValues(CCTask* nextTask) {
    
//    if (verbosity & BASICOUTPUT) {
//        Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
//        Serial.print(deviceName);
//        Serial.print(F(" review values... "));
//    }
    if (nextTask->getTarget() > 1.0) return DEVICE_PARAMETER_ERROR;
    if (nextTask->getTarget() < -1.0) return DEVICE_PARAMETER_ERROR;
    if (nextTask->getVelocity() == 0) return DEVICE_PARAMETER_ERROR;
    if (nextTask->getAcceleration() == 0) return DEVICE_PARAMETER_ERROR;
    if (nextTask->getDeceleration() == 0) nextTask->setDeceleration(nextTask->getAcceleration());
    
//    if (verbosity_dcController & BASICOUTPUT) {
//        Serial.println(F("   done"));
//    }
    return DEVICE_OK;
}


void CCDcControllerDevice_fullBridge::prepareNextTask() {}

deviceInfoCode CCDcControllerDevice_fullBridge::prepareTask(CCTask *nextTask) {
   
    if (state == MOVING) {
        currentRatio = currentPosition;
        t0 += switchOnTime;
        switchOffTime -= switchOnTime;
        switchOnTime = 0;
    } else {
        currentRatio = 0;
    }
    
    currentTaskID = nextTask->getTaskID();
    
    target = nextTask->getTarget();
    velocity = nextTask->getVelocity();
    acceleration = nextTask->getAcceleration();
    deceleration = nextTask->getDeceleration();
    
    switching = nextTask->getSwitching();
    stopping = nextTask->getStopping();

    approximationControl = nextTask->getStopControl();
    approximationTarget = nextTask->getStopControlTarget();
    approximationCurve = nextTask->getApproximationCurve();
    gap = nextTask->getGap();
    approximation = nextTask->getApproximation();
    
    targetReachedCounter = 0;
    
    // target: dutycycle (-1.0 ... 1.0)
    // velocity: pwm frequency
    // switchingInterval: periodendauer = 1 / frequency
    // acceleration: time for ramping up
    // deceleration: time for ramping down
    
    if (stopping == STOP_DYNAMIC) {
        target = fabs(target);
    }
    
    switchingInterval = 1000uL / velocity;
    
    targetOnDuration = fabs(target) * switchingInterval;
    
    timeForAcceleration = acceleration;
    timeForDeceleration = deceleration;
    timeForAccAndConstSpeed = 0x7FFFFFFF;
    
    switchOnTime = 0;
    
    if (verbosity & CALCULATIONDEBUG) {
        Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
        Serial.print(deviceName);
        Serial.print(F(": praparing... target power: "));
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
    
    return DEVICE_OK;
}


void CCDcControllerDevice_fullBridge::startTask() {
    if (target > 1) {                                               // values valid?
        stopTask();
    }
    else {
        targetReachedCounter = 0;

        state = MOVING;                                             // tag device as MOVING
        t0 = millis();
        
        switchOnTime = 0;
        switchOffTime = switchOnTime + targetOnDuration;
        currentRatio = 0;
                
        if (verbosity & BASICOUTPUT) {
            Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
            Serial.print(deviceName);
            Serial.println(F(", starting task"));

                Serial.print(F(": approx: "));
                Serial.print(approximationCurve);
            Serial.print(F(", gap: "));
            Serial.println(gap);
            
        }
    }
    
//    operateTask();
}

void CCDcControllerDevice_fullBridge::initiateStop() {
    timeForAcceleration = millis() - t0;
    timeForAccAndConstSpeed = millis() - t0;
    target = currentPosition;
}

void CCDcControllerDevice_fullBridge::stopTask() {
    currentPosition = 0;
    digitalWrite(switching_pin, !switchingPin_active);
    isActive = false;
    digitalWrite(switching_B_pin, !switchingPin_B_active);
    isActive_B = false;
    state = MOVE_DONE;
    
    
    if (verbosity & BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
        Serial.print(deviceName);
        Serial.println(F(": stopping task"));
        Serial.print(F(": off state is "));
        Serial.println(digitalRead(switching_pin));
    }
}

void CCDcControllerDevice_fullBridge::finishTask() {
    state = SLEEPING;
        
    if (verbosity & BASICOUTPUT) {
        Serial.print(F("[CCDcControllerDevice_fullBridge]: "));
        Serial.print(deviceName);
        Serial.println(F(": task done"));
    }
}


void CCDcControllerDevice_fullBridge::operateTask() {
    elapsedTime = millis() - t0;
    
    if (elapsedTime >= switchOnTime) {
        
        if (verbosity & MOVEMENTDEBUG) {
            Serial.print(elapsedTime);
            Serial.print(F(": switched on"));
        }
        
        if (elapsedTime < timeForAcceleration) {
            currentPosition = currentRatio + (target - currentRatio) * elapsedTime / (float)timeForAcceleration;
            
            if (verbosity & MOVEMENTDEBUG) {
                Serial.print(F(", accelerating"));
            }
            
        } else if (elapsedTime < timeForAccAndConstSpeed) {
            currentPosition = target;
            
        } else {
            elapsedTime -= timeForAccAndConstSpeed;
            
            if (verbosity & MOVEMENTDEBUG) {
                Serial.print(F(", decelerating"));
            }
            
            if (elapsedTime < timeForDeceleration) {
                currentPosition = target * (1 - elapsedTime / (float)timeForDeceleration);
            } else {
                currentPosition = 0;
                stopTask();
                return;
            }
        }
        
        if (stopping == STOP_DYNAMIC) {
            sensorValue = approximationControl->value();
            
            relativePosition = approximationTarget - sensorValue;

            
            if (verbosity & MOVEMENTDEBUG) {
                Serial.print(F(", stopping dynamically"));
            }
            
            if (abs(relativePosition) <= gap) {
                
                currentPosition = 0;
                targetReachedCounter++;
                
                if (verbosity & MOVEMENTDEBUG) {
                    Serial.print(F(", times target was reached: "));
                    Serial.print(targetReachedCounter);
                }
                
                if (targetReachedCounter >= approximation) {
                    stopTask();
                }
                return;
                
            } else {
                // calculation of a decreasing value by position:
                //
                // ********* equitation first order: *********
                // y = b / (a - x) + 1; (1)
                //
                // crossPoint: y = 0 ==>
                // 0 = b / (a - x) + 1; b / (a - x) = -1;
                // b = -a + x; x = a + b;
                // crossPoint at x = a + b: cp = a + b ==> a = cp - b
                //
                // in 1:
                // *****************************************
                // *** ==> y = b / (cp - b - x) + 1; <== ***
                // *****************************************
                //
                //
                // ********* equitation second order: *********
                // y = b / (a * a - x * x) + 1; (1)
                //
                // crossPoint: y = 0 ==>
                // 0 = b / (a * a - x * x) + 1; b / (a * a - x * x) = -1; b / (x * x - a * a) = 1;
                // b = x * x - a * a;
                // crossPoint at x * x = a * a + b: cp * cp = a * a + b ==> a = sqrt(cp * cp - b);
                //
                // in 1:
                // y = b / (sqrt(cp * cp - b) * sqrt(cp * cp - b) - x * x) + 1;
                // **********************************************
                // *** ==> y = b / (cp * cp - b - x * x); <== ***
                // **********************************************
                //
                // first attempt: decreasing value == power:
                // currentPositionFirstOrder = target * (approximationCurve / ((float)gap - approximationCurve -/+ relativePosition) + 1);
                // currentPositionSecondOrder = target * (approximationCurve / ((float)gap * gap - approximationCurve - relativePosition * relativePosition) + 1);
                //
                // second attempt: decreasing value == velocity:
                // float currentVelocity = (relativePosition - relativePosition_previous) / (elapsedTime - elapsedTime_previous);
                
                currentPosition = target * (approximationCurve / ((float)gap * gap - approximationCurve - relativePosition * relativePosition) + 1);
                if (relativePosition < 0) {
                    currentPosition = - currentPosition;
                }

                targetReachedCounter = 0;
                
            }
        }
        
        if (currentPosition > 0) {
            digitalWrite(switching_B_pin, !switchingPin_B_active);
            digitalWrite(switching_pin, switchingPin_active);
            isActive = true;
        } else if (currentPosition < 0) {
            digitalWrite(switching_pin, !switchingPin_active);
            digitalWrite(switching_B_pin, switchingPin_B_active);
            isActive_B = true;
        }
        
        switchOffTime = switchOnTime + switchingInterval * fabs(currentPosition);
        switchOnTime += switchingInterval;
        if (verbosity & MOVEMENTDEBUG) {
            Serial.print(F(", scheduled off at "));
            Serial.print(switchOffTime);
            Serial.print(F(", duty cycle: "));
            Serial.print(currentPosition);
            Serial.print(F(", next cycle at "));
            Serial.println(switchOnTime);
        }
        
        
    } else if (elapsedTime > switchOffTime) {
        if (isActive == true || isActive_B == true) {
            digitalWrite(switching_pin, !switchingPin_active);
            digitalWrite(switching_B_pin, !switchingPin_B_active);
            isActive = false;
            isActive_B = false;
            
            if (verbosity & MOVEMENTDEBUG) {
                Serial.print(elapsedTime);
                Serial.print(F(": switched off at "));
                Serial.println(switchOffTime);
            }
        }
    }
}

void CCDcControllerDevice_fullBridge::getReadOut(unsigned int theReadOut) {}

