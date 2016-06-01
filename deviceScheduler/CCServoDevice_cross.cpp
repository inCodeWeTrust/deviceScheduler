//
//  CCServoDevice_cross.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#include <Servo.h>
#include <avr/pgmspace.h>

#include "CCServoDevice_cross.h"



CCServoDevice_cross::CCServoDevice_cross(String deviceName, unsigned char servo_00_pin, int minPosition_00, int midPosition_00, int maxPosition_00, unsigned char servo_01_pin, int minPosition_01, int midPosition_01, int maxPosition_01, int parkPosition) : CCServoDevice(deviceName, servo_00_pin, minPosition_00, maxPosition_00, parkPosition) {
    
    this->midPosition_00 = midPosition_00;
    
    this->servo_01_pin = servo_01_pin;
    this->minPosition_01 = minPosition_01;
    this->maxPosition_01 = maxPosition_01;
    this->midPosition_01 = midPosition_01;
    
    
    if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_MEMORYDEBUG) {
        Serial.print(F("[CCServoDevice_cross]: setup "));
        Serial.print(deviceName);
        Serial.print(F(", servo_00_pin: "));
        Serial.print(servo_pin);
        Serial.print(F(", at $"));
        Serial.print((long)this, HEX);
        Serial.print(F(", Servo 00 at $"));
        Serial.print((long)&theServo, HEX);
        Serial.print(F(", servo_01_pin: "));
        Serial.print(servo_01_pin);
        Serial.print(F(", at $"));
        Serial.print((long)this, HEX);
        Serial.print(F(", Servo 01 at $"));
        Serial.println((long)&secondServo, HEX);
//        Serial.print(F(", park: "));
//        Serial.print(parkPosition);
//        Serial.print(F(", current: "));
//        Serial.print(currentPosition);
    }
    
}


CCServoDevice_cross::~CCServoDevice_cross() {
    detachDevice();
    if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_BASICOUTPUT) {
        Serial.print(F("[CCServoDevice_cross]: "));
        Serial.print(deviceName);
        Serial.println(F(": deconstructed"));
    }
}

unsigned int CCServoDevice_cross::counterPosition(unsigned int position) {
    return 3100 - position;
}


void CCServoDevice_cross::attachDevice() {
    theServo.writeMicroseconds(parkPosition);
    currentPosition = parkPosition;
    unsigned char channel = theServo.attach(servo_pin, minPosition, maxPosition);
    
    secondServo.writeMicroseconds(counterPosition(parkPosition));
    unsigned char channel_01 = secondServo.attach(servo_01_pin, minPosition_01, maxPosition_01);
    
    
    
    if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_MEMORYDEBUG) {
        Serial.print(F("[CCServoDevice_cross]: "));
        Serial.print(deviceName);
        Serial.print(F(", parkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk: "));
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
    if (theServo.attached()) {
        theServo.detach();
        if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_BASICOUTPUT) {
            Serial.print(F("[CCServoDevice_cross]: "));
            Serial.print(deviceName);
            Serial.println(F(": Servo 00 detached now: "));
            Serial.println(theServo.attached());
        } else {
            if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_BASICOUTPUT) {
                Serial.print(F("[CCServoDevice]: "));
                Serial.print(deviceName);
                Serial.println(F(": detach: Servo 00 was not attached"));
            }
        }
    }
    if (secondServo.attached()) {
        secondServo.detach();
        if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_BASICOUTPUT) {
            Serial.print(F("[CCServoDevice_cross]: "));
            Serial.print(deviceName);
            Serial.println(F(": Servo 01 detached now: "));
            Serial.println(secondServo.attached());
        } else {
            if (CCSERVODEVICE_VERBOSE & CCSERVODEVICE_BASICOUTPUT) {
                Serial.print(F("[CCServoDevice]: "));
                Serial.print(deviceName);
                Serial.println(F(": detach: Servo 01 was not attached"));
            }
        }
    }
}



void CCServoDevice_cross::operateTask() {
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
                
                Serial.print("### init dynamical stop: sens: ");
                Serial.print(sensorValue);
                Serial.print(", init: ");
                Serial.print(initiatePerformanceValue);
                Serial.print(", fall: ");
                Serial.println(sensorValuesFalling);
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
        
        if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_MOVEMENTDEBUG) {
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
        
        if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_MOVEMENTDEBUG) {
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
        
        sensorValue = analogRead(sensor);
        
        performanceFactor = c_perform * (sensorValue - targetValue);
        
        //        int dif = targetValue - sensorValue;
        //
        //        if (dif < 0) {
        //            currentPosition += deltaDeltaNorm * (1 - 1 / dif);
        //        }
        //        if (dif > 0) {
        //            currentPosition += deltaDeltaNorm * (1 - 1 / dif);
        //        }
        
        
        // = (sensorValue - targetValue) / (initiatePerformanceValue - targetValue)
        
        //        if (performanceFactor > 0) {
        //            performanceFactor = pow(performanceFactor, stopPerformance);
        //        } else {
        //            performanceFactor = -pow(abs(performanceFactor), stopPerformance);
        //        }
        //
        
        currentPosition += deltaDeltaNorm * performanceFactor;
        currentPosition = min(currentPosition, maxPosition);
        currentPosition = max(currentPosition, minPosition);
        
        theServo.writeMicroseconds(currentPosition);
        secondServo.writeMicroseconds(counterPosition(currentPosition));
        
        if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_MOVEMENTDEBUG) {
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
        
        Serial.print("### end dynamical stop: sens: ");
        Serial.print(sensorValue);
        Serial.print(", position: ");
        Serial.println(currentPosition);
        
        if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_BASICOUTPUT) {
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
            
            if (CCSERVODEVICE_CROSS_VERBOSE & CCSERVODEVICE_CROSS_MOVEMENTDEBUG) {
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

