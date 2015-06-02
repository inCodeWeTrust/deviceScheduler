//
//  CCStepperDevice.cpp
//  moveServos
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <avr/pgmspace.h>

#include "CCStepperDevice.h"

float deceleration, currentVelocity;

unsigned long steppingPeriod_small = 400;
unsigned long steppingPeriod_big =   600;
//char steppingMode[] = STEPPER_STOCK_STEPPINGMODES;
unsigned long lastStep;
unsigned long currentMicroStep;


CCStepperDevice::CCStepperDevice(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCode, unsigned char microStep_0_pin, unsigned char microStep_1_pin, unsigned char microStep_2_pin, float anglePerStep) :
CCDevice() {
    this->deviceName = deviceName;
    this->dir_pin = dir_pin;
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
    this->microStep_0_pin = microStep_0_pin;
    this->microStep_1_pin = microStep_1_pin;
    this->microStep_2_pin = microStep_2_pin;
    this->highestSteppingMode = highestSteppingMode;
    this->stepModeCode = new unsigned char[highestSteppingMode + 1];
    
    Serial.print("stepModeCodes: ");
    for (unsigned char codeNumber = 0; codeNumber <= highestSteppingMode; codeNumber++) {
        this->stepModeCode[codeNumber] = stepModeCode[codeNumber];
        Serial.print(stepModeCode[codeNumber]);
        Serial.print(", ");
    }
    Serial.println();
    
    this->anglePerStep = anglePerStep;
    
    currentMicroStep = 0;
    
    type = STEPPERDEVICE;
    state = 0;
    movePointer = 0;
    countOfMoves = 0;
    
    attachDevice();
    
    if (CCStepperDevice_VERBOSE & CCStepperDevice_MEMORYDEBUG) {
        Serial.print(F("[CCStepperDevice]: setup stepperDevice "));
        Serial.print(deviceName);
        Serial.print(F(" at $"));
        Serial.println((long)this, HEX);
    }
}

CCStepperDevice::~CCStepperDevice() {
    deleteMoves();
    detachDevice();
}

void CCStepperDevice::enableDevice() {
    digitalWrite(enable_pin, LOW);
}
void CCStepperDevice::disableDevice() {
    digitalWrite(enable_pin, HIGH);
    digitalWrite(microStep_0_pin, LOW);
    digitalWrite(microStep_1_pin, LOW);
    digitalWrite(microStep_2_pin, LOW);
}

void CCStepperDevice::attachDevice() {
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(enable_pin, OUTPUT);
    
    pinMode(microStep_0_pin, OUTPUT);
    pinMode(microStep_1_pin, OUTPUT);
    pinMode(microStep_2_pin, OUTPUT);
    
    digitalWrite(enable_pin, HIGH);
    
    if (CCStepperDevice_VERBOSE & CCStepperDevice_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: device "));
        Serial.print(deviceName);
        Serial.print(F(" attached: dir_pin: "));
        Serial.print(dir_pin);
        Serial.print(F(", step_pin: "));
        Serial.print(step_pin);
        Serial.print(F(", enable_pin: "));
        Serial.print(enable_pin);
        Serial.print(F(" steppingModes: "));
        Serial.print(highestSteppingMode + 1);
        Serial.print("stepModeCodes: ");
        for (unsigned char codeNumber = 0; codeNumber <= highestSteppingMode; codeNumber++) {
            Serial.print(stepModeCode[codeNumber]);
            Serial.print(", ");
        }
        Serial.println();
    }
}
void CCStepperDevice::detachDevice() {
    digitalWrite(enable_pin, HIGH);
    
    pinMode(dir_pin, INPUT);
    pinMode(step_pin, INPUT);
    pinMode(enable_pin, INPUT);
    
    pinMode(microStep_0_pin, INPUT);
    pinMode(microStep_1_pin, INPUT);
    pinMode(microStep_2_pin, INPUT);
    
    if (CCStepperDevice_VERBOSE & CCStepperDevice_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: device "));
        Serial.print(deviceName);
        Serial.println(F(" detached"));
    }
}



void CCStepperDevice::prepareNextMove() {
    // this takes 812us with calculating anglePerStep
    unsigned long t_prepMove = micros();
    if (state & MOVING) {
        // *** current velocity ***
        if (currentMicroStep < microStepsForAcceleration) {
            // v * v = 2 * a * gamma
            // gamma = n * anglePerStep = n_mic / microStepsPerStep * anglePerStep = n_mic / (1 << microSteppingMode) * anglePerStep
            // ==> v = sqrt(2 * a * n_mic / (1 << microSteppingMode) * anglePerStep)
            currentVelocity = sqrt(2 * acceleration * currentMicroStep / (1 << highestSteppingMode));
        }
        else if (currentMicroStep < microStepsForAccAndConstSpeed) {
            currentVelocity = velocity;
        }
        else {
            currentVelocity = sqrt(2 * acceleration * (microStepsToGo - currentMicroStep) / (1 << highestSteppingMode));
        }
    }
    else {
        currentVelocity = 0;
    }
    
    target = theMove[movePointer]->target;
    velocity = theMove[movePointer]->velocity;
    acceleration = theMove[movePointer]->acceleration;
    deceleration = theMove[movePointer]->deceleration;
    
    startEvent = theMove[movePointer]->startEvent;
    stopEvent = theMove[movePointer]->stopEvent;
    startDelay = theMove[movePointer]->startDelay;
    startTime = theMove[movePointer]->startTime;
    timeout = theMove[movePointer]->timeout;
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
    stopSharply = theMove[movePointer]->stopSharply;
    
    
    if (target < 0 || velocity < 0 || acceleration < 0) {
        target = fabs(target);
        velocity = fabs(velocity);
        acceleration = fabs(acceleration);
        countingDown = true;
        digitalWrite(dir_pin, HIGH);
    }
    else {
        countingDown = false;
        digitalWrite(dir_pin, LOW);
    }
    
    if (velocity == 0) {
        // lets say then: 20
        velocity = 20;
    }
    if (acceleration == 0) {
        // lets say then: wayForConstSpeed = 0 ==>
        // wayToGo / 2 = v * v / a ==> a = 2 * v * v / wayToGo
        acceleration = 2 * velocity * velocity / target;
    }
    
    if (deceleration == 0) {
        deceleration = acceleration;
    }
    else {
        deceleration = fabs(deceleration);
    }
    
    
    stepsToGo = (float)target / anglePerStep;
// v[steps/sec] = v[angle/sec] * stepPerAngle = v[angle/sec] / anglePerStep
    velocity /= anglePerStep;
    acceleration /= anglePerStep;
    deceleration /= anglePerStep;
    
    // *** steps for deceleration: ***
    // v * v = 2 * a * gamma
    // gamma = n * anglePerStep
    // ==> v * v = 2 * a * n * anglePerStep ==> n = v * v / (2 * a * anglePerStep)
    stepsForDeceleration = (0.5 * velocity * velocity / deceleration) + 0.5;
    
    // *** steps for acceleration: ***
    // v * v - v0 * v0 = 2 * a * gamma
    // gamma = n * anglePerStep
    // ==> v * v - v0 * v0 = 2 * a * n * anglePerStep
    // ==> n = (v * v - v0 * v0) / (2 * a * anglePerStep)
    stepsForAcceleration = fabs((0.5 * (velocity * velocity - currentVelocity * currentVelocity) / acceleration) + 0.5);
    
    // *** does acceleration and deceleration fit into the move? ***
    if (stepsForAcceleration + stepsForDeceleration > stepsToGo - 2) {
        // v * v - v0 * v0 = 2 * a_acc * s_acc ==> s_acc = (v * v - v0 * v0) / (2 * a_acc)
        // v * v = 2 * a_dec * s_dec => s_dec = v * v / (2 * a_dec)
        // ==> s_acc / s_dec = (v * v - v0 * v0) / (2 * a_acc) / (v * v / (2 * a_dec)) = a_dec * (v * v - v0 * v0) / (a_acc * v * v)
        // s = s_acc + s_dec;
        // s_acc / s_dec = c ==> s_acc = c * s_dec
        // ==> s = c * s_dec + s_dec = s_dec * (c + 1) ==> s_dec = s / (c + 1)
        // ==> s_dec = s / (a_dec * (v * v - v0 * v0) / (a_acc * v * v) + 1)
        stepsForDeceleration = abs(stepsToGo / ((velocity * velocity - currentVelocity * currentVelocity) * deceleration / acceleration / velocity / velocity + 1) - 1);
        stepsForAcceleration = stepsToGo - 2 - stepsForDeceleration;
    }
    
    bool adjustVelocity = false;
    
    if (adjustVelocity) {
        if (acceleration > deceleration) {
            // *** recalculate v: ***
            // v * v = 2 * a * n * anglePerStep ==> v = sqrt(2 * a * n * anglePerStep)
            velocity = sqrt(2 * deceleration * stepsForDeceleration);
        }
        else {
            // *** recalculate v: ***
            // v * v = 2 * a * n * anglePerStep ==> v = sqrt(2 * a * n * anglePerStep)
            velocity = sqrt(2 * acceleration * stepsForAcceleration);
        }
        deceleration = - deceleration;
    }
    else {
        // *** recalculate a: ***
        // v * v = 2 * a * n * anglePerStep ==> a = v * v / (2 * n * anglePerStep)
        deceleration = -0.5 * velocity * velocity / stepsForDeceleration;

        // *** recalculate a: ***
        // v * v - v0 * v0 = 2 * a * gamma ==> a = (v * v - v0 * v0) / 2 * gamma = (v * v - v0 * v0) / (2 * n * anglePerStep)
        acceleration = 0.5 * (velocity * velocity - currentVelocity * currentVelocity) / stepsForAcceleration;
    }

    // *** time for acceleration: ***
    // a = v0 / t0; a = v / t ==> t0 = v0 / a; t = v / a
    // tx = t - t0; ==> tx = v / a - v0 / a = (v - v0) / a
    timeForAcceleration = 1000000.0 * fabs((currentVelocity - velocity) / acceleration);
    
    timeForAccAndConstSpeed = timeForAcceleration + 1000000UL * (stepsToGo - stepsForAcceleration - stepsForDeceleration) / velocity;
    
    // *** time for the next step while acceleration: ***
    // gamma = 0.5 * a * t * t + v0 * t
    // n * anglePerStep = 0.5 * a * t * t + v0 * t
    // 0.5 * a * t * t + v0 * t - n * anglePerStep = 0
    // ==> t = (-v0 +/- sqrt(v0 * v0 + 2 * a * n * anglePerStep)) / a
    // ==> t = (-v0 +/- sqrt(v0 * v0 + 2 * a * microSteps / microStepsPerSteps * anglePerStep)) / a
    // 2 * a * anglePerStep / microStepsPerSteps = constant = c0_acc
    c0_acc = 2.0 * acceleration / (1 << highestSteppingMode);
    c0_dec = 2.0 * deceleration / (1 << highestSteppingMode);
    
    // *** time for next step while constant speed: ***
    // v = gamma / t ==> t = gamma / v
    // gamma = stepsToAngle(n) = n * stepsToAngle(1)
    // ==> t = n * stepsToAngle(1) / v [*1000000us/s]
    // stepsToAngle(1) / v = constant = c1
    // t = n * c1
    c1 = 1 / velocity / (1 << highestSteppingMode);
    
    microStepsToGo = stepsToGo << highestSteppingMode;
    microStepsForAcceleration = stepsForAcceleration << highestSteppingMode;
    microStepsForAccAndConstSpeed = microStepsToGo - (stepsForDeceleration << highestSteppingMode);
    
    if (state & MOVING) {
        t0 += lastStepTime;
        currentMicroStep = 1;
        stepExpiration = 1000000.0 * (-currentVelocity + sqrt(currentVelocity * currentVelocity + currentMicroStep * c0_acc)) / acceleration;
        
        if (CCStepperDevice_VERBOSE & CCStepperDevice_MOVEMENTDEBUG) {
            //        Serial.print(F("[CCStepperDevice]: "));
            //        Serial.print(deviceName);
            Serial.print(F("  ["));
            Serial.print(lastStepTime);
            Serial.print(F(" --> 0] next step at: "));
            Serial.print((unsigned long)stepExpiration);
            Serial.print(F(" --> "));
            Serial.println((unsigned long)stepExpiration + lastStepTime);
        }
    }



    if (CCStepperDevice_VERBOSE & CCStepperDevice_MEMORYDEBUG) {
//        Serial.print(F("[CCStepperDevice]: "));
//        Serial.print(deviceName);
//        Serial.print(F(": stepsToGo at $"));
//        Serial.print((long)&stepsToGo, HEX);
//        Serial.print(F(", velocity at $"));
//        Serial.print((long)&velocity, HEX);
//        Serial.print(F(", acceleration at $"));
//        Serial.println((long)&acceleration, HEX);
    }
    if (CCStepperDevice_VERBOSE & CCStepperDevice_CALCULATIONDEBUG) {
//        Serial.print(F("[CCStepperDevice]: "));
//        Serial.print(deviceName);
//        Serial.print(F(": prepare move "));
//        Serial.print(movePointer);
//        Serial.print(F(": "));
//        Serial.print(F("target: "));
//        Serial.print(target);
//        Serial.print(F(", stepsToGo: "));
//        Serial.print(stepsToGo);
//        Serial.print(F(" ("));
//        Serial.print(microStepsToGo);
//        Serial.print(F("), speed: "));
//        Serial.print(velocity);
//        Serial.print(F(", acceleration: "));
//        Serial.print(acceleration);
//        Serial.print(F(", deceleration: "));
//        Serial.print(deceleration);
//        Serial.print(F(", stepsForAcceleration: "));
//        Serial.print(stepsForAcceleration);
//        Serial.print(F(" ("));
//        Serial.print(microStepsForAcceleration);
//        Serial.print(F("), stepsForDeceleration: "));
//        Serial.print(stepsForDeceleration);
//        Serial.print(F(" ("));
//        Serial.print(microStepsToGo - microStepsForAccAndConstSpeed);
//        Serial.print(F("), timeForAcceleration: "));
//        Serial.println(timeForAcceleration);
    }
    Serial.println(micros() - t_prepMove);
}

void CCStepperDevice::startMove() {
    // lets start in highest stepping mode
    microSteppingMode = highestSteppingMode;
    (stepModeCode[microSteppingMode] & 1) ? digitalWrite(microStep_0_pin, HIGH) : digitalWrite(microStep_0_pin, LOW);
    (stepModeCode[microSteppingMode] & 2) ? digitalWrite(microStep_1_pin, HIGH) : digitalWrite(microStep_1_pin, LOW);
    (stepModeCode[microSteppingMode] & 4) ? digitalWrite(microStep_2_pin, HIGH) : digitalWrite(microStep_2_pin, LOW);
    
    enableDevice();
    
    
    state |= MOVING;
    t0 = micros() & 0x7fffffff;
    
    currentMicroStep = 0;
    stepExpiration = 0;
    driveDynamic();
    
    if (CCStepperDevice_VERBOSE & CCStepperDevice_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(", move: "));
        Serial.println((int)movePointer);
    }
}
void CCStepperDevice::initiateStop() {
    if (currentMicroStep < microStepsForAcceleration) {
        microStepsToGo = 2 * currentMicroStep;
        timeForAccAndConstSpeed = stepExpiration;
        microStepsForAccAndConstSpeed = currentMicroStep;
        return;
    }
    if (currentMicroStep < microStepsForAccAndConstSpeed) {
        microStepsToGo = currentMicroStep + microStepsForAcceleration;
        timeForAccAndConstSpeed = stepExpiration;
        microStepsForAccAndConstSpeed = currentMicroStep;
        return;
    }
}
void CCStepperDevice::switchToNextMove() {
    //currentTarget = target;
    
    
}

void CCStepperDevice::stopMoving() {
    state &= ~MOVING;
    state |= MOVE_DONE;
}
void CCStepperDevice::finishMove() {
    state &= ~MOVING & ~MOVE_DONE;
    
    disableDevice();
    
    if (CCStepperDevice_VERBOSE & CCStepperDevice_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": stop: move "));
        Serial.println((int)movePointer);
    }
}

void CCStepperDevice::driveDynamic() {
    long now = micros() & 0x7fffffff;
    if (t0 > now) t0 -= 0x80000000;

    elapsedTime = now - t0;
    
    signed long timeDif = stepExpiration - elapsedTime;
    
    if (timeDif <= 0) {
        digitalWrite(step_pin, LOW);
        digitalWrite(step_pin, HIGH);
        
        if (timeDif < -100) {
            if (currentMicroStep < microStepsToGo) {
                digitalWrite(I_AM_LATE_LED, HIGH);
            }
        }
        else {
            digitalWrite(I_AM_LATE_LED, LOW);
        }
        
        currentMicroStep += (1 << (highestSteppingMode - microSteppingMode));
        
        (countingDown) ? currentPosition = -(currentMicroStep >> highestSteppingMode) * anglePerStep : currentPosition = (currentMicroStep >> highestSteppingMode) * anglePerStep;
        
        // while ramping up
        if (currentMicroStep <= microStepsForAcceleration) {
            lastStepTime = stepExpiration;
            stepExpiration = 1000000.0 * (-currentVelocity + sqrt(currentVelocity * currentVelocity + currentMicroStep * c0_acc)) / acceleration;
            
            if (stepExpiration - elapsedTime < steppingPeriod_small) kickUp();
            
            if (CCStepperDevice_VERBOSE & CCStepperDevice_MOVEMENTDEBUG) {
                //        Serial.print(F("[CCStepperDevice]: "));
                //        Serial.print(deviceName);
                Serial.print((unsigned long)elapsedTime);
                Serial.print(F(": ramp up: cur: "));
                Serial.print(currentMicroStep);
                Serial.print(F(", next step at: "));
                Serial.println((unsigned long)stepExpiration);
            }
            return;
        }
        
        // while going with constant speed
        if (currentMicroStep < microStepsForAccAndConstSpeed) {
            lastStepTime = stepExpiration;
            stepExpiration = timeForAcceleration + (currentMicroStep - microStepsForAcceleration) * 1000000.0 * c1;
        
            if (CCStepperDevice_VERBOSE & CCStepperDevice_MOVEMENTDEBUG) {
                //        Serial.print(F("[CCStepperDevice]: "));
                //        Serial.print(deviceName);
                Serial.print(elapsedTime);
                Serial.print(F(": constant: cur: "));
                Serial.print(currentMicroStep);
                Serial.print(F(", next step at: "));
                Serial.println(stepExpiration);
            }
            return;
        }
        
        // while ramping down
        if (currentMicroStep < microStepsToGo) {
            lastStepTime = stepExpiration;
            stepExpiration = timeForAccAndConstSpeed + (sqrt((currentMicroStep - microStepsForAccAndConstSpeed) * c0_dec + velocity * velocity) - velocity) * 1000000.0 / deceleration;
            
            if (stepExpiration - elapsedTime > steppingPeriod_big) kickDown();
            
            if (CCStepperDevice_VERBOSE & CCStepperDevice_MOVEMENTDEBUG) {
                //        Serial.print(F("[CCStepperDevice]: "));
                //        Serial.print(deviceName);
                Serial.print(elapsedTime);
                Serial.print(F(": ramp down: cur: "));
                Serial.print(currentMicroStep);
                Serial.print(F(", next step at: "));
                Serial.println(stepExpiration);
            }
            return;
        }
        
        // very last step
        if (currentMicroStep == microStepsToGo) {
            stepExpiration = -velocity / deceleration;
            
            if (CCStepperDevice_VERBOSE & CCStepperDevice_MOVEMENTDEBUG) {
                //        Serial.print(F("[CCStepperDevice]: "));
                //        Serial.print(deviceName);
                Serial.print(elapsedTime);
                Serial.print(F(": last step: cur: "));
                Serial.print(currentMicroStep);
                Serial.print(F(", next step at: "));
                Serial.println(stepExpiration);
            }
            return;
        }
        
        // if we didnt return up to here, we are done!
        
        stopMoving();
        
    }
}

void CCStepperDevice::kickUp() {
    if (microSteppingMode > 0) {
        if (!(currentMicroStep & 0x0F)) {
            microSteppingMode--;
            
            (stepModeCode[microSteppingMode] & 1) ? digitalWrite(microStep_0_pin, HIGH) : digitalWrite(microStep_0_pin, LOW);
            (stepModeCode[microSteppingMode] & 2) ? digitalWrite(microStep_1_pin, HIGH) : digitalWrite(microStep_1_pin, LOW);
            (stepModeCode[microSteppingMode] & 4) ? digitalWrite(microStep_2_pin, HIGH) : digitalWrite(microStep_2_pin, LOW);
        }
    }
}
void CCStepperDevice::kickDown() {
    if (microSteppingMode < highestSteppingMode) {
        if ((currentMicroStep & 0x0F) == 0) {
            microSteppingMode++;
            
            (stepModeCode[microSteppingMode] & 1) ? digitalWrite(microStep_0_pin, HIGH) : digitalWrite(microStep_0_pin, LOW);
            (stepModeCode[microSteppingMode] & 2) ? digitalWrite(microStep_1_pin, HIGH) : digitalWrite(microStep_1_pin, LOW);
            (stepModeCode[microSteppingMode] & 4) ? digitalWrite(microStep_2_pin, HIGH) : digitalWrite(microStep_2_pin, LOW);
        }
    }
}




