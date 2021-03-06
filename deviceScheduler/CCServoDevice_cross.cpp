//
//  CCServoDevice_cross.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include "CCServoDevice_cross.h"



CCServoDevice_cross::CCServoDevice_cross(const String deviceName, const unsigned int servo_00_pin, const int minPosition_00, const int midPosition_00, const int maxPosition_00, const unsigned int servo_01_pin, const int minPosition_01, const int midPosition_01, const int maxPosition_01, const int parkPosition) : CCServoDevice(deviceName, servo_00_pin, minPosition_00, maxPosition_00, parkPosition), midPosition_00(midPosition_00), servo_01_pin(servo_01_pin), minPosition_01(minPosition_01), midPosition_01(midPosition_01), maxPosition_01(maxPosition_01) {
    
    
    //        Serial.print(F("[CCServoDevice_cross]: setup cross-servo"));
    //        Serial.print(deviceName);
    //        Serial.print(F(", servo_01_pin: "));
    //        Serial.print(servo_01_pin);
    //        if (verbosity & MEMORYDEBUG) {
    //            Serial.print(F(", at $"));
    //            Serial.print((long)this, HEX);
    //            Serial.print(F(", Servo 01 at $"));
    //            Serial.print((long)&secondServo, HEX);
    //        }
    //        Serial.println();
}


CCServoDevice_cross::~CCServoDevice_cross() {
    detachDevice();
    if (verbosity & BASICOUTPUT) {
        Serial.print(F("[CCServoDevice_cross]: "));
        Serial.print(deviceName);
        Serial.println(F(": destructed"));
    }
}

unsigned int CCServoDevice_cross::counterPosition(unsigned int position) {
    return 3100 - position;
}


void CCServoDevice_cross::attachDevice() {
    theServo.writeMicroseconds(parkPosition);
    currentPosition = parkPosition;
    unsigned int channel = theServo.attach(servo_pin, minPosition, maxPosition);
    
    secondServo.writeMicroseconds(counterPosition(parkPosition));
    unsigned int channel_01 = secondServo.attach(servo_01_pin, minPosition_01, maxPosition_01);
    
    
    
    if (verbosity & MEMORYDEBUG) {
        Serial.print(F("[CCServoDevice_cross]: "));
        Serial.print(deviceName);
        Serial.print(F(", park: "));
        Serial.print(parkPosition);
        Serial.print(F(" attached: Servo 00 on channel "));
        Serial.print(channel);
        Serial.print(F(", park_01: "));
        Serial.print(counterPosition(parkPosition));
        Serial.print(F(" attached: Servo 01 on channel "));
        Serial.println(channel_01);
    }
}
void CCServoDevice_cross::detachDevice() {
    if (verbosity & BASICOUTPUT) {
        Serial.print(F("[CCServoDevice_cross]: "));
        Serial.print(deviceName);
        Serial.print(F(": detach servos: Servo 00"));
    }
    theServo.detach();
    if (verbosity & BASICOUTPUT) {
        Serial.print(F(", Servo 01"));
        Serial.println(secondServo.attached());
    }
    secondServo.detach();
}



void CCServoDevice_cross::operateTask() {
    lastCycleTime = elapsedTime;
    elapsedTime = millis() - t0;
    
    if (stopping == STOP_DYNAMIC) {
        if (dynamicalStop == false) {
            sensorValue = approximationControl->value();
            if ((sensorValue > initiatePerformanceValue && (!sensorValuesFalling)) || (sensorValue < initiatePerformanceValue && sensorValuesFalling)) {
                timeForAcceleration = elapsedTime;
                timeForConstantSpeed = 0;
                lastCycleTime = 0;
                c_perform = 1.0 / (initiatePerformanceValue - approximationTarget);
                dynamicalStop = true;
            }
        }
    }
    
    // ramp up
    if (elapsedTime < (signed long)timeForAcceleration) {
        // s = s0 + 0.5 * a * t^2 * (1000ms / sec)^2
        deltaS = elapsedTime * elapsedTime * acceleration / 2000000;
        currentPosition = startPosition + deltaS;
        theServo.writeMicroseconds(currentPosition);
        secondServo.writeMicroseconds(counterPosition(currentPosition));
        
        if (verbosity & MOVEMENTDEBUG) {
            Serial.print(F("[CCServoDevice_cross]: "));
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
    if (elapsedTime < (signed long)timeForConstantSpeed) {
        // s = s0 + s1 + deltaS = s0 + s1 + v * t
        deltaS = elapsedTime * velocity / 1000;
        currentPosition = startPosition + wayForAcceleration + deltaS;
        theServo.writeMicroseconds(currentPosition);
        secondServo.writeMicroseconds(counterPosition(currentPosition));
        
        if (verbosity & MOVEMENTDEBUG) {
            Serial.print(F("[CCServoDevice_cross]: "));
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
        
        sensorValue = approximationControl->value();
        
        performanceFactor = c_perform * (sensorValue - approximationTarget);
        
        currentPosition += deltaDeltaNorm * performanceFactor;
        currentPosition = fmin(currentPosition, maxPosition);
        currentPosition = fmax(currentPosition, minPosition);
        
        theServo.writeMicroseconds(currentPosition);
        secondServo.writeMicroseconds(counterPosition(currentPosition));
        
        if (verbosity & MOVEMENTDEBUG) {
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
            if (fabs(sensorValue - approximationTarget) < sensorTreshold) {
                if (valueCounter++ < approximation) {
                    return;
                }
            } else {
                valueCounter = 0;
                return;
            }
        }
        
        
        if (verbosity & BASICOUTPUT) {
            Serial.print(F("[CCServoDevice_cross]: "));
            Serial.print(deviceName);
            Serial.print(F(": stopped at sensor: "));
            Serial.println(sensorValue);
        }
        targetPosition = currentPosition;
        
    } else {
        // generic ramp down
        elapsedTime -= timeForConstantSpeed;
        if (elapsedTime < (signed long)timeForAcceleration) {
            //  t^2 = (timeForAcceleration - elapsedTime)^2 = (elapsedTime - timeForAcceleration)^2;
            elapsedTime -= timeForAcceleration;
            // s = sges - deltaS = 0.5 * a * t^2 = 0.5 * a * (time_in_millis / 1000)^2 = 1/2 * 1/1000000 * a * t^2
            deltaS = elapsedTime * elapsedTime * acceleration / 2000000;
            currentPosition = targetPosition - deltaS;
            theServo.writeMicroseconds(currentPosition);
            secondServo.writeMicroseconds(counterPosition(currentPosition));
            
            if (verbosity & MOVEMENTDEBUG) {
                Serial.print(F("[CCServoDevice_cross]: "));
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
    
    theServo.writeMicroseconds(targetPosition);
    secondServo.writeMicroseconds(counterPosition(currentPosition));
    currentPosition = targetPosition;
    
    stopTask();
    
}

