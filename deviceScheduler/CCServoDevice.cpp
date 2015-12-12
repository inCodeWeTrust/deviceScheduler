//
//  CCServoDevice.cpp
//  moveServos
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "CCServoDevice.h"

#include <Servo.h>


CCServoDevice::CCServoDevice(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition) : CCDevice() {
    this->deviceName = deviceName;
    this->servo_pin = servo_pin;
    this->minPosition = minPosition;
    this->maxPosition = maxPosition;
    this->parkPosition = parkPosition;
    
    
    type = SERVODEVICE;
    state = 0;
    movePointer = 0;
    countOfMoves = 0;
    
    defaultVelocity = 0;
    defaultAcceleration = 0;
    defaultDeceleration = 0;
    
    
    if (CCServoDevice_VERBOSE & CCServoDevice_MEMORYDEBUG) {
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
    deleteMoves();
    detachDevice();
}


void CCServoDevice::enableDevice() {}
void CCServoDevice::disableDevice() {}

void CCServoDevice::attachDevice() {
    theServo.writeMicroseconds(parkPosition);
    currentPosition = parkPosition;
    unsigned char channel = theServo.attach(servo_pin, minPosition, maxPosition);                                                      // substitute with attatchDevice()
    
    if (CCServoDevice_VERBOSE & CCServoDevice_MEMORYDEBUG) {
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

void CCServoDevice::prepareNextMove() {
    
    target = theMove[movePointer]->target;
    velocity = theMove[movePointer]->velocity;
    acceleration = theMove[movePointer]->acceleration;
    
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
    stopDynamicaly = theMove[movePointer]->stopDynamicaly;
    sensor = theMove[movePointer]->sensor;
    initiatePerformanceValue = theMove[movePointer]->initiatePerformanceValue;
    stopValue = theMove[movePointer]->stopValue;
    stopPerformance = theMove[movePointer]->stopPerformance;
    dynamicalStop = false;
    
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
    if (CCServoDevice_VERBOSE & CCServoDevice_MEMORYDEBUG) {
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
    if (CCServoDevice_VERBOSE & CCServoDevice_CALCULATIONDEBUG) {
        Serial.print(F("[CCServoDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": prepare move "));
        Serial.print(movePointer);
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
        Serial.print(F(", stopDynamicaly: "));
        if (stopDynamicaly) {
            Serial.print(F("yes, stopPerformance: "));
            Serial.print(stopPerformance);
            Serial.print(F(", initiatePerformanceValue: "));
            Serial.print(initiatePerformanceValue);
            Serial.print(F(", stopValue: "));
            Serial.print(stopValue);
            Serial.print(F(", stopPerformance: "));
            Serial.println(stopPerformance);
        } else {
            Serial.println("no");
        }
    }
}

void CCServoDevice::startMove() {
    state |= MOVING;
    t0 = millis();
    
    if (CCServoDevice_VERBOSE & CCServoDevice_BASICOUTPUT) {
        Serial.print(F("[CCServoDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": start move "));
        Serial.println((int)movePointer);
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

void CCServoDevice::stopMoving() {
    state &= ~MOVING;
    state |= MOVE_DONE;
}
void CCServoDevice::finishMove() {
    state &= ~MOVING & ~MOVE_DONE;
    
    if (CCServoDevice_VERBOSE & CCServoDevice_BASICOUTPUT) {
        Serial.print(F("[CCServoDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": stop: move "));
        Serial.println((int)movePointer);
    }
}





void CCServoDevice::driveDynamic() {
    
    elapsedTime = millis() - t0;
    
    Serial.print("sensor value is: ");
    Serial.println(analogRead(sensor));
    
    if (stopDynamicaly) {
        if (dynamicalStop == false) {
            if (analogRead(sensor) < initiatePerformanceValue) {
                initiateStop();
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
        
        if (CCServoDevice_VERBOSE & CCServoDevice_MOVEMENTDEBUG) {
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
        
        if (CCServoDevice_VERBOSE & CCServoDevice_MOVEMENTDEBUG) {
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
    
    // ramp down
    elapsedTime -= timeForConstantSpeed;
    if (elapsedTime < timeForAcceleration) {
        if (dynamicalStop) {
            deltaS = elapsedTime * velocity / 1000;
            deltaS = (analogRead(sensor) - stopValue) / (initiatePerformanceValue - stopValue) * deltaS;
            currentPosition = startPosition + wayForAcceleration + deltaS;
            if (deltaS <= 0) timeForAcceleration = 0;
        } else {
            //  t^2 = (timeForAcceleration - elapsedTime)^2 = (elapsedTime - timeForAcceleration)^2;
            elapsedTime -= timeForAcceleration;
            // s = sges - deltaS = 0.5 * a * t^2 = 0.5 * a * (time_in_millis / 1000)^2 = 1/2 * 1/1000000 * a * t^2
            deltaS = elapsedTime * elapsedTime * acceleration / 2000000;
            currentPosition = targetPosition - deltaS;
        }
        theServo.writeMicroseconds(currentPosition);
        
        if (CCServoDevice_VERBOSE & CCServoDevice_MOVEMENTDEBUG) {
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
    
    // if we didnt return up to here, we are done!
    
    
    if (CCServoDevice_VERBOSE & CCServoDevice_MEMORYDEBUG) {
        Serial.print(F("currentPosition just after move: "));
        Serial.print((int)currentPosition);
        Serial.print(F("[in memory: "));
        Serial.println((long)&currentPosition, HEX);
    }
    
    theServo.writeMicroseconds(targetPosition);
    //    currentPosition = theServo.readMicroseconds();
    currentPosition = targetPosition;
    
    if (CCServoDevice_VERBOSE & CCServoDevice_MEMORYDEBUG) {
        Serial.print(F("currentPosition read from Servo: "));
        Serial.print((int)currentPosition);
        Serial.print(F("[in memory: "));
        Serial.println((long)&currentPosition, HEX);
    }
    
    stopMoving();
    
}
