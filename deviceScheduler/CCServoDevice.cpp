//
//  CCServoDevice.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "CCServoDevice.h"

#include <Servo.h>


CCServoDevice::CCServoDevice(unsigned int deviceIndex, String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition) : CCDevice() {
    this->deviceIndex = deviceIndex;
    this->deviceName = deviceName;
    this->servo_pin = servo_pin;
    this->minPosition = minPosition;
    this->maxPosition = maxPosition;
    this->parkPosition = parkPosition;
    
    
    this->type = SERVODEVICE;
    this->state = SLEEPING;
    this->taskPointer = 0;
    this->countOfTasks = 0;
    
    this->defaultVelocity = 0;
    this->defaultAcceleration = 0;
    this->defaultDeceleration = 0;
    
    
    if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MEMORYDEBUG) {
        Serial.print(F("[CCServoDevice]: setup "));
        Serial.print(deviceName);
        Serial.print(F(", servo_pin: "));
        Serial.print(servo_pin);
        Serial.print(F(", park: "));
        Serial.print(parkPosition);
        Serial.print(F(", current: "));
        Serial.print(currentPosition);
        Serial.print(F(", at $"));
        Serial.print((long)this, HEX);
        Serial.print(F(", Servo at $"));
        Serial.println((long)&theServo, HEX);
    }

    
    attachDevice();
    
}


CCServoDevice::~CCServoDevice() {
    deleteTasks();
    detachDevice();
}


void CCServoDevice::enableDevice() {}
void CCServoDevice::disableDevice() {}

void CCServoDevice::attachDevice() {
    theServo.writeMicroseconds(parkPosition);
    currentPosition = parkPosition;
    unsigned char channel = theServo.attach(servo_pin, minPosition, maxPosition);                                                      // substitute with attatchDevice()
    
    if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MEMORYDEBUG) {
        Serial.print(F("[CCServoDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(", park: "));
        Serial.print(parkPosition);
        Serial.print(F(" attached: Servo on channel "));
        Serial.println(channel);
    }
}
void CCServoDevice::detachDevice() {
    theServo.detach();
}

void CCServoDevice::reviewValues() {}

void CCServoDevice::prepareNextTask() {
    target = task[taskPointer]->target;
    velocity = task[taskPointer]->velocity;
    acceleration = task[taskPointer]->acceleration;
    startDelay = task[taskPointer]->startDelay;
    
    startEvent = task[taskPointer]->startEvent;
    startTime = task[taskPointer]->startTime;
    startButton = task[taskPointer]->startButton;
    startButtonState = task[taskPointer]->startButtonState;
    startTriggerDevice = task[taskPointer]->startTriggerDevice;
    startTriggerTask = task[taskPointer]->startTriggerTask;
    startTriggerPosition = task[taskPointer]->startTriggerPosition;
    
    stopEvent = task[taskPointer]->stopEvent;
    timeout = task[taskPointer]->timeout;
    stopButton = task[taskPointer]->stopButton;
    stopButtonState = task[taskPointer]->stopButtonState;
    stopTriggerDevice = task[taskPointer]->stopTriggerDevice;
    stopTriggerTask = task[taskPointer]->stopTriggerTask;
    stopTriggerPosition = task[taskPointer]->stopTriggerPosition;
    
    stopping = task[taskPointer]->stopping;
    switchTaskPromptly = task[taskPointer]->switchTaskPromptly;
    
    sensor = task[taskPointer]->sensor;
    initiatePerformanceValue = task[taskPointer]->initiatePerformanceValue;
    targetValue = task[taskPointer]->targetValue;
    stopPerformance = task[taskPointer]->stopPerformance;
    approximation = task[taskPointer]->approximation;

    dynamicalStop = false;
    valueCounter = 0;
    sensorValuesFalling = (initiatePerformanceValue > targetValue);
    if (approximation == SKIP_APPROXIMATION_IMMEDIATELY) {
        sensorTreshold = 64;
    } else {
        sensorTreshold = 256.0 / approximation;
    }
    
    targetPosition = target;
    startPosition = currentPosition;
    
    wayToGo = (signed long) targetPosition - startPosition;
    
    if (wayToGo == 0) {
        timeForAcceleration = 0;
        timeForConstantSpeed = 0;
    }
    else {
        
        if (wayToGo < 0) {
            acceleration = - fabs(acceleration);
            velocity = - fabs(velocity);
            directionDown = true;
        }
        else {
            acceleration = fabs(acceleration);
            velocity = fabs(velocity);
            directionDown = false;
        }
        
        if (velocity == 0) {
            // lets say then: 20
            velocity = 20;
        }
        if (acceleration == 0) {
            // lets say then: wayForConstSpeed = 0 ==>
            // wayToGo / 2 = v * v / a ==> a = 2 * v * v / wayToGo
            acceleration = 2 * velocity * velocity / wayToGo;
        }
        
        // v^2 = 2 * a * s --> s = v^2 / (2 * a) = 0.5 * v * v / a
        wayForAcceleration = (signed long) (0.5 * velocity * velocity / acceleration);
        
        wayForConstantSpeed = wayToGo - 2 * wayForAcceleration;
        // wayForConstSpeed = 0 ==>
        // wayToGo / 2 = v * v / a ==> a = 2 * v * v / wayToGo
        
        if ((directionDown && (wayForConstantSpeed > 0)) || (!directionDown && (wayForConstantSpeed < 0))) {      // is there way to go with constant speed?
            // no constant speed drive ==> speed up the half way then slow down the other half
            wayForAcceleration = wayToGo / 2;
            
            // s = 0.5 * a * t^2 --> t = sqrt(2 * s / a) = sqrt(2 * 0.5 * wayToGo / a) * 1000ms / sec
            timeForAcceleration = sqrt(1000000.0 * wayToGo / acceleration);
            
            wayForConstantSpeed = 0;
            timeForConstantSpeed = 0;
        }
        else {
            // there will be constant speed drive
            // speed up: a = v / t --> t = v / a * 1000ms / sec
            timeForAcceleration = 1000 * velocity / acceleration;
            // constant speed: v = s / t --> t = s / v * 1000ms / sec
            timeForConstantSpeed = 1000 * wayForConstantSpeed / velocity;
        }
    }
    
    
    if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MEMORYDEBUG) {
        Serial.print(F("[CCServoDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": currentPosition at $"));
        Serial.print((long)&currentPosition, HEX);
        Serial.print(F(", targetPosition at $"));
        Serial.print((long)&targetPosition, HEX);
        Serial.print(F(", velocity at $"));
        Serial.print((long)&velocity, HEX);
        Serial.print(F(", acceleration at $"));
        Serial.println((long)&acceleration, HEX);
    }
    if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_CALCULATIONDEBUG) {
        Serial.print(F("[CCServoDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": prepare move "));
        Serial.print(taskPointer);
        Serial.print(F(": "));
        Serial.print(F("current: "));
        Serial.print(currentPosition);
        Serial.print(F(", start: "));
        Serial.print(startPosition);
        Serial.print(F(", target: "));
        Serial.print(targetPosition);
        Serial.print(F(", speed: "));
        Serial.print(velocity);
        Serial.print(F(", acceleration: "));
        Serial.print(acceleration);
        Serial.print(F(", wayToGo: "));
        Serial.print(wayToGo);
        Serial.print(F(", wayForAcceleration: "));
        Serial.print(wayForAcceleration);
        Serial.print(F(", timeForAcceleration: "));
        Serial.print(timeForAcceleration);
        Serial.print(F(", wayForConstantSpeed: "));
        Serial.print(wayForConstantSpeed);
        Serial.print(F(", timeForConstantSpeed: "));
        Serial.print(timeForConstantSpeed);
        Serial.print(F(", stopping: "));
        if (stopping == STOP_DYNAMIC) {
            Serial.print(F("yes, stopPerformance: "));
            Serial.print(stopPerformance);
            Serial.print(F(", initiatePerformanceValue: "));
            Serial.print(initiatePerformanceValue);
            Serial.print(F(", targetValue: "));
            Serial.print(targetValue);
            Serial.print(F(", stopPerformance: "));
            Serial.print(stopPerformance);
            Serial.print(F(", stopping: "));
            Serial.print(stopping);
            Serial.print(F(", sensorTreshold: "));
            Serial.println(sensorTreshold);
        } else {
            Serial.println("no");
        }
    }
}

void CCServoDevice::startTask() {
    state = MOVING;
    t0 = millis();
    
    if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_BASICOUTPUT) {
        Serial.print(F("[CCServoDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": start move "));
        Serial.println((int)taskPointer);
    }
}
void CCServoDevice::initiateStop() {
    elapsedTime = millis() - t0;
    if (elapsedTime < timeForAcceleration) {
        timeForAcceleration = elapsedTime;
        timeForConstantSpeed = 0;
        targetPosition = currentPosition + currentPosition - startPosition;
        return;
    }
    
    elapsedTime -= timeForAcceleration;
    if (elapsedTime < timeForConstantSpeed) {
        timeForConstantSpeed = elapsedTime;
        targetPosition = currentPosition + wayForAcceleration;
    }
}

void CCServoDevice::stopTask() {
    state = MOVE_DONE;
}
void CCServoDevice::finishTask() {
    state = SLEEPING;
    
    if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_BASICOUTPUT) {
        Serial.print(F("[CCServoDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": stop: move "));
        Serial.println((int)taskPointer);
    }
}





void CCServoDevice::operateTask() {
    lastCycleTime = elapsedTime;
    elapsedTime = millis() - t0;
    
    if (stopping == STOP_DYNAMIC) {
        if (dynamicalStop == false) {
            sensorValue = analogRead(sensor);
            if ((sensorValue > initiatePerformanceValue && (!sensorValuesFalling)) || (sensorValue < initiatePerformanceValue && sensorValuesFalling)) {
                timeForAcceleration = elapsedTime;
                timeForConstantSpeed = 0;
                lastCycleTime = 0;
                c_perform = 1.0 / (initiatePerformanceValue - targetValue);
                dynamicalStop = true;
            }
        }
    }
    
    // ramp up
    if (elapsedTime < timeForAcceleration) {
        // s = s0 + 0.5 * a * t^2 * (1000ms / sec)^2
        deltaS = elapsedTime * elapsedTime * acceleration / 2000000;
        currentPosition = startPosition + deltaS;
        theServo.writeMicroseconds(currentPosition);
        
        if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MOVEMENTDEBUG) {
            Serial.print(F("[CCServoDevice]: "));
            Serial.print(deviceName);
            Serial.print(F(": ramp up: cur: "));
            Serial.print((int)currentPosition);
            Serial.print(F(", start: "));
            Serial.print((int)startPosition);
            Serial.print(F(", deltaS: "));
            Serial.println((long)deltaS);
        }
        
        return;
    }
    
    // go with constant speed
    elapsedTime -= timeForAcceleration;
    if (elapsedTime < timeForConstantSpeed) {
        // s = s0 + s1 + deltaS = s0 + s1 + v * t
        deltaS = elapsedTime * velocity / 1000;
        currentPosition = startPosition + wayForAcceleration + deltaS;
        theServo.writeMicroseconds(currentPosition);
        
        if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MOVEMENTDEBUG) {
            Serial.print(F("[CCServoDevice]: "));
            Serial.print(deviceName);
            Serial.print(F(": go constant: cur: "));
            Serial.print((int)currentPosition);
            Serial.print(F(", start: "));
            Serial.print((int)startPosition);
            Serial.print(F(", wayAcc: "));
            Serial.print((int)wayForAcceleration);
            Serial.print(F(", deltaS: "));
            Serial.println((long)deltaS);
        }
        
        return;
    }
    // stop dynamically?
    if (dynamicalStop) {
        deltaDeltaNorm = (float)(elapsedTime - lastCycleTime) * velocity / 1000.0;
        
        sensorValue = analogRead(sensor);
        
        performanceFactor = c_perform * (sensorValue - targetValue);
        if (performanceFactor > 0) {
            performanceFactor = pow(performanceFactor, stopPerformance);
        } else {
            performanceFactor = -pow(abs(performanceFactor), stopPerformance);
        }
        
        currentPosition += deltaDeltaNorm * performanceFactor;
        theServo.writeMicroseconds(currentPosition);
        
        if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MOVEMENTDEBUG) {
            Serial.print("elapsedTime: ");
            Serial.print(elapsedTime);
            Serial.print(", performanceFactor: ");
            Serial.print(performanceFactor);
            Serial.print(", sensorValue: ");
            Serial.print(sensorValue);
            Serial.print(", deltaDeltaNorm: ");
            Serial.print(deltaDeltaNorm);
            Serial.print(", deltaS: ");
            Serial.print(deltaS);
            Serial.print(", currentPosition: ");
            Serial.println(currentPosition);
        }
        

        if (approximation == SKIP_APPROXIMATION_NEVER) {
            return;
        } else {
            if (abs(sensorValue - targetValue) < sensorTreshold) {
                if (valueCounter++ < approximation) {
                    return;
                }
            } else {
                valueCounter = 0;
                return;
            }
        }
        targetPosition = currentPosition;
        
    } else {
        // generic ramp down
        elapsedTime -= timeForConstantSpeed;
        if (elapsedTime < timeForAcceleration) {
            //  t^2 = (timeForAcceleration - elapsedTime)^2 = (elapsedTime - timeForAcceleration)^2;
            elapsedTime -= timeForAcceleration;
            // s = sges - deltaS = 0.5 * a * t^2 = 0.5 * a * (time_in_millis / 1000)^2 = 1/2 * 1/1000000 * a * t^2
            deltaS = elapsedTime * elapsedTime * acceleration / 2000000;
            currentPosition = targetPosition - deltaS;
            theServo.writeMicroseconds(currentPosition);
            
            if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MOVEMENTDEBUG) {
                Serial.print(F("[CCServoDevice]: "));
                Serial.print(deviceName);
                Serial.print(F(": ramp down: cur: "));
                Serial.print((int)currentPosition);
                Serial.print(F(", target: "));
                Serial.print((int)targetPosition);
                Serial.print(F(", deltaS: "));
                Serial.println((long)deltaS);
            }
            
            return;
        }
    }


    // if we didnt return up to here, we are done!
    
    
    if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MEMORYDEBUG) {
        Serial.print(F("currentPosition just after move: "));
        Serial.print((int)currentPosition);
        Serial.print(F("[in memory: "));
        Serial.println((long)&currentPosition, HEX);
    }
    
    theServo.writeMicroseconds(targetPosition);
    //    currentPosition = theServo.readMicroseconds();
    currentPosition = targetPosition;
    
    if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_MEMORYDEBUG) {
        Serial.print(F("currentPosition read from Servo: "));
        Serial.print((int)currentPosition);
        Serial.print(F("[in memory: "));
        Serial.println((long)&currentPosition, HEX);
    }
    
    stopTask();
    
}
