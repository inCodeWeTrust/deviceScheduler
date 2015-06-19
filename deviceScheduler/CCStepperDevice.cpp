//
//  CCStepperDevice.cpp
//  moveServos
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <avr/pgmspace.h>

#include "CCStepperDevice.h"


CCStepperDevice::CCStepperDevice(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCode, unsigned char numberOfMicroStepPins, unsigned char *microStepPin, float anglePerStep) : CCDevice() {
    
    this->deviceName = deviceName;
    this->dir_pin = dir_pin;
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
    
    this->numberOfMicroStepPins = numberOfMicroStepPins;
    this->microStepPin = new unsigned char[numberOfMicroStepPins];
    
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        this->microStepPin[pinIndex] = microStepPin[pinIndex];
    }
    
    this->highestSteppingMode = highestSteppingMode;
    this->stepModeCode = new unsigned char[highestSteppingMode + 1];
    this->steppingUnit = new unsigned char[highestSteppingMode + 1];
    this->positionUnit = new float[highestSteppingMode + 1];
    
    for (unsigned char codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
        this->stepModeCode[codeIndex] = stepModeCode[codeIndex];
        this->steppingUnit[codeIndex] = (1 << (highestSteppingMode - codeIndex));
        this->positionUnit[codeIndex] = anglePerStep / (1 << codeIndex);
    }
    
    this->anglePerStep = anglePerStep;
    this->acceleration_max = 200;
    
    type = STEPPERDEVICE;
    state = 0;
    movePointer = 0;
    countOfMoves = 0;
    
    defaultVelocity = 0;
    defaultAcceleration = 0;
    defaultDeceleration = 0;
    
    currentMicroStep = 0;
    currentPosition = 0;
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: setup "));
        Serial.print(deviceName);
        Serial.print(F(": currentPosition: "));
        Serial.print(currentPosition);
        Serial.print(F(", dir_pin: "));
        Serial.print(dir_pin);
        Serial.print(F(", step_pin: "));
        Serial.print(step_pin);
        Serial.print(F(", enable_pin: "));
        Serial.print(enable_pin);
        Serial.print(F(", numberOfMicroStepPins: "));
        Serial.print(numberOfMicroStepPins);
        Serial.print(F(", pins: "));
        for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
            Serial.print(microStepPin[pinIndex]);
            Serial.print(", ");
        }
        Serial.print(F("steppingModes: "));
        Serial.print(highestSteppingMode + 1);
        Serial.print(F(", stepModeCodes: "));
        for (unsigned char codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
            Serial.print(stepModeCode[codeIndex]);
            Serial.print(", ");
        }
        Serial.print(F("anglePerStep: "));
        Serial.print(anglePerStep);
        Serial.print(F(", at $"));
        Serial.println((long) this, HEX);
    }
    
    attachDevice();
    
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
    setMicroStepPins(0);
}

void CCStepperDevice::attachDevice() {
    pinMode(dir_pin, OUTPUT);
    
    pinMode(step_pin, OUTPUT);
    
    pinMode(enable_pin, OUTPUT);
    digitalWrite(enable_pin, HIGH);
    
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        pinMode(microStepPin[pinIndex], OUTPUT);
    }
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: "));
        Serial.print(deviceName);
        Serial.println(F(" attached"));
    }
}
void CCStepperDevice::detachDevice() {
    digitalWrite(enable_pin, HIGH);
    
    pinMode(dir_pin, INPUT);
    pinMode(step_pin, INPUT);
    pinMode(enable_pin, INPUT);
    
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        pinMode(microStepPin[pinIndex], INPUT);
    }
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: device "));
        Serial.print(deviceName);
        Serial.println(F(" detached"));
    }
}



void CCStepperDevice::prepareNextMove() {
    // this takes 900us 1500us with calculating velocity * velocity
    unsigned long t_prepMove = micros();
    unsigned long t_stop;
    
    if (state & MOVING) {
        // *** current velocity ***
        if (currentMicroStep < microStepsForAcceleration) {
            // v * v = 2 * a * gamma
            // gamma = n * anglePerStep = n_mic / microStepsPerStep * anglePerStep = n_mic / (1 << microSteppingMode) * anglePerStep
            // ==> v = sqrt(2 * a * n_mic / (1 << microSteppingMode) * anglePerStep)
            currentVelocity = sqrt(2 * abs(acceleration) * currentMicroStep / (1 << highestSteppingMode));
        }
        else if (currentMicroStep < microStepsForAccAndConstSpeed) {
            currentVelocity = velocity;
        }
        else {
            currentVelocity = sqrt(2 * abs(deceleration) * (microStepsToGo - currentMicroStep) / (1 << highestSteppingMode));
        }
    }
    else {
        currentVelocity = 0;
    }
 
    t_stop = micros() - t_prepMove;
    Serial.print("== currentVelocity: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    
    
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

    t_stop = micros() - t_prepMove;
    Serial.print("== copy parameters: ");
    Serial.println(t_stop);
    t_prepMove = micros();

    stepsToGo = (float)(target - currentPosition) / anglePerStep;
//    stepsToGo = (target - currentPosition) * 200 / 360;

    
    t_stop = micros() - t_prepMove;
    Serial.print("== stepsToGo: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    
    if (velocity == 0) velocity = defaultVelocity;
    if (acceleration == 0) acceleration = defaultAcceleration;
    if (deceleration == 0) deceleration = defaultDeceleration;
    
    if (stepsToGo == 0 || velocity == 0 || acceleration == 0) {
        return;
    }
    
    targetDirectionDown = false;
    changeDirection = false;
    
    
    if (stepsToGo < 0 || velocity < 0 || acceleration < 0) {
        stepsToGo = abs(stepsToGo);
        velocity = abs(velocity);
        acceleration = abs(acceleration);
        
        if (state & MOVING) {
            if (!directionDown) {
                changeDirection = true;
                targetDirectionDown = true;
            }
        }
        else {
            changeDirection = false;
            directionDown = true;
            digitalWrite(dir_pin, HIGH);
        }
    }
    else {
        if (state & MOVING) {
            if (directionDown) {
                changeDirection = true;
                targetDirectionDown = false;
            }
        }
        else {
            changeDirection = false;
            directionDown = false;
            digitalWrite(dir_pin, LOW);
        }
    }
    
    
    if (deceleration == 0) {
        deceleration = acceleration;
    }
    else {
        deceleration = abs(deceleration);
    }
    
    t_stop = micros() - t_prepMove;
    Serial.print("== set flags: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    

    
    // v[steps/sec] = v[angle/sec] * stepPerAngle = v[angle/sec] / anglePerStep
    velocity /= anglePerStep;
    acceleration /= anglePerStep;
    acceleration_inv = 1 / acceleration;
    deceleration /= anglePerStep;
    deceleration_inv = 1 / deceleration;
    
    currVeloBySquare = currentVelocity * currentVelocity;
    veloBySquare = velocity * velocity;

    t_stop = micros() - t_prepMove;
    Serial.print("== velos to square: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    

    // *** steps for deceleration: ***
    // v * v = 2 * a * gamma
    // gamma = n * anglePerStep
    // ==> v * v = 2 * a * n * anglePerStep ==> n = v * v / (2 * a * anglePerStep)
    /// v[DEG/65536/sec] * 65535 * v[DEG/65536/sec] * 65535 / (2 * a[DEG/65535/sec/sec] * 65535 *)
//    stepsForDeceleration = 0.5 * velocity * velocity / deceleration;
    stepsForDeceleration = (unsigned long)(veloBySquare / deceleration) >> 1;
    
    t_stop = micros() - t_prepMove;
    Serial.print("== stepsForDeceleration: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    
// *** steps for acceleration: ***
    // v * v - v0 * v0 = 2 * a * gamma
    // gamma = n * anglePerStep
    // ==> v * v - v0 * v0 = 2 * a * n * anglePerStep
    // ==> n = (v * v - v0 * v0) / (2 * a * anglePerStep)
    if (changeDirection) {
        stepsForAcceleration = (unsigned long)((veloBySquare + currVeloBySquare) * acceleration_inv) >> 1;
//        stepsForAcceleration = (0.5 * (veloBySquare + currVeloBySquare) / acceleration / anglePerStep) + 0.5;
    }
    else {
        stepsForAcceleration = (unsigned long)(abs(veloBySquare - currVeloBySquare) * acceleration_inv) >> 1;
//        stepsForAcceleration = (0.5 * abs(veloBySquare - currVeloBySquare) / acceleration / anglePerStep) + 0.5;
    }
    t_stop = micros() - t_prepMove;
    Serial.print("== stepsForAcceleration: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    

    
    // *** does acceleration and deceleration fit into the move? ***
    if (stepsForAcceleration + stepsForDeceleration > stepsToGo - 2) {
        // v * v - v0 * v0 = 2 * a_acc * s_acc ==> s_acc = (v * v - v0 * v0) / (2 * a_acc)
        // v * v = 2 * a_dec * s_dec => s_dec = v * v / (2 * a_dec)
        // ==> s_acc / s_dec = (v * v - v0 * v0) / (2 * a_acc) / (v * v / (2 * a_dec)) = a_dec * (v * v - v0 * v0) / (a_acc * v * v)
        // s = s_acc + s_dec;
        // s_acc / s_dec = c ==> s_acc = c * s_dec
        // ==> s = c * s_dec + s_dec = s_dec * (c + 1) ==> s_dec = s / (c + 1)
        // ==> s_dec = s / (a_dec * (v * v - v0 * v0) / (a_acc * v * v) + 1)
        if (changeDirection) {
//            stepsForDeceleration = stepsToGo / ((velocity * velocity + currentVelocity * currentVelocity) * deceleration / acceleration / velocity / velocity + 1) - 1;
            stepsForDeceleration = stepsToGo / ((veloBySquare + currVeloBySquare) * deceleration * acceleration_inv / veloBySquare + 1) - 1;
        }
        else {
//            stepsForDeceleration = stepsToGo / (abs(velocity * velocity - currentVelocity * currentVelocity) * deceleration / acceleration / velocity / velocity + 1) - 1;
            stepsForDeceleration = stepsToGo / (abs(veloBySquare - currVeloBySquare) * deceleration * acceleration_inv / veloBySquare + 1) - 1;
        }
        stepsForAcceleration = stepsToGo - 2 - stepsForDeceleration;
    }
    
    
    // *** recalculate a: ***
    // v * v = 2 * a * n * anglePerStep ==> a = v * v / (2 * n * anglePerStep)
    deceleration = -veloBySquare / (stepsForDeceleration << 1);
//    deceleration = -0.5 * veloBySquare / stepsForDeceleration / anglePerStep;
    
    if (deceleration < -acceleration_max) {
        // *** recalculate v: ***
        // v * v = 2 * a * n * anglePerStep ==> v = sqrt(2 * a * n * anglePerStep)
        deceleration = -acceleration_max;
        veloBySquare = 2 * acceleration_max * stepsForDeceleration;
        velocity = sqrt(veloBySquare);
    }
    
    if (stepsForAcceleration > 0) {
        
        // *** recalculate a: ***
        // v * v - v0 * v0 = 2 * a * gamma ==> a = (v * v - v0 * v0) / 2 * gamma = (v * v - v0 * v0) / (2 * n * anglePerStep)
        if (changeDirection) {
            if (targetDirectionDown) {
                acceleration = -(veloBySquare + currVeloBySquare) / (stepsForAcceleration << 1);
                acceleration_inv = 1 / acceleration;
                if (acceleration < -acceleration_max) {
                    acceleration = -acceleration_max;
                    // acceleration = -0.5 * (velocity * velocity + currentVelocity * currentVelocity) / stepsForAcceleration / anglePerStep
                    // -2 * acceleration * stepsForAcceleration * anglePerStep = velocity * velocity + currentVelocity * currentVelocity
                    // -2 * acceleration * stepsForAcceleration * anglePerStep - currentVelocity * currentVelocity = velocity * velocity
                    // acceleration = -acceleration_max ==>
                    // 2 * acceleration_max * stepsForAcceleration * anglePerStep - currentVelocity * currentVelocity = velocity * velocity
                    // ==> velocity = sqrt(2 * acceleration_max * stepsForAcceleration * anglePerStep - currentVelocity * currentVelocity)
//                    velocity = sqrt(2 * acceleration_max * stepsForAcceleration - currentVelocity * currentVelocity);
                    veloBySquare = (stepsForAcceleration << 1) * acceleration_max - currVeloBySquare;
                    velocity = sqrt(veloBySquare);
                    deceleration = -veloBySquare / (stepsForDeceleration << 1);
//                    deceleration = -0.5 * veloBySquare / stepsForDeceleration / anglePerStep;
                }
            }
            else {
                acceleration = (veloBySquare + currVeloBySquare) / (stepsForAcceleration << 1);
//                acceleration = 0.5 * (veloBySquare + currVeloBySquare) / stepsForAcceleration / anglePerStep;
                if (acceleration > acceleration_max) {
                    acceleration = acceleration_max;
                    // acceleration = 0.5 * (velocity * velocity + currentVelocity * currentVelocity) / stepsForAcceleration / anglePerStep;
                    // 2 * acceleration * stepsForAcceleration * anglePerStep = velocity * velocity + currentVelocity * currentVelocity
                    // 2 * acceleration * stepsForAcceleration * anglePerStep - currentVelocity * currentVelocity = velocity * velocity
                    // acceleration = acceleration_max ==>
                    // 2 * acceleration_max * stepsForAcceleration * anglePerStep - currentVelocity * currentVelocity = velocity * velocity
                    // ==> velocity = sqrt(2 * acceleration_max * stepsForAcceleration * anglePerStep - currentVelocity * currentVelocity)
//                    velocity = sqrt(2 * acceleration_max * stepsForAcceleration - currentVelocity * currentVelocity);
                    veloBySquare = (stepsForAcceleration << 1) * acceleration_max - currVeloBySquare;
                    velocity = sqrt(veloBySquare);
//                    deceleration = -0.5 * velocity * velocity / stepsForDeceleration;
                    deceleration = -veloBySquare / (stepsForDeceleration << 1);
                }
            }
        }
        else {
//            acceleration = 0.5 * abs(velocity * velocity - currentVelocity * currentVelocity) / stepsForAcceleration;
            acceleration = abs(veloBySquare - currVeloBySquare) / (stepsForAcceleration << 1);
            if (acceleration > acceleration_max) {
                acceleration = acceleration_max;
                // acceleration = 0.5 * (velocity * velocity - currentVelocity * currentVelocity) / stepsForAcceleration / anglePerStep;
                // 2 * acceleration * stepsForAcceleration * anglePerStep = velocity * velocity - currentVelocity * currentVelocity
                // 2 * acceleration * stepsForAcceleration * anglePerStep + currentVelocity * currentVelocity = velocity * velocity
                // ==> velocity = sqrt(2 * acceleration_max * stepsForAcceleration * anglePerStep + currentVelocity * currentVelocity)
//                velocity = sqrt(2 * acceleration_max * stepsForAcceleration + currentVelocity * currentVelocity);
                veloBySquare = (stepsForAcceleration << 2) * acceleration_max + currVeloBySquare;
                velocity = sqrt(veloBySquare);
//                deceleration = -0.5 * velocity * velocity / stepsForDeceleration;
                deceleration = -veloBySquare / (stepsForDeceleration << 1);
            }
        }
    }
    
    t_stop = micros() - t_prepMove;
    Serial.print("== recalculations: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    

//    microStepsTillZero = (unsigned long)abs(0.5 * currentVelocity * currentVelocity / acceleration) << highestSteppingMode;
    microStepsTillZero = (unsigned long)abs(currVeloBySquare / acceleration) << highestSteppingMode << 1;
    

    t_stop = micros() - t_prepMove;
    Serial.print("== microStepsTillZero: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    

    // *** time for acceleration: ***
    // a = v0 / t0; a = v / t ==> t0 = v0 / a; t = v / a
    // tx = t - t0; ==> tx = v / a - v0 / a = (v - v0) / a
    if (changeDirection) {
        timeForAcceleration = abs(1000000.0 * (currentVelocity + velocity) / acceleration);
    }
    else {
        timeForAcceleration = abs(1000000.0 * (currentVelocity - velocity) / acceleration);
    }
    timeForAccAndConstSpeed = timeForAcceleration + 1000000UL * (stepsToGo - stepsForAcceleration - stepsForDeceleration) / velocity;
    
    t_stop = micros() - t_prepMove;
    Serial.print("== times: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    
    
    
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
    
    t_stop = micros() - t_prepMove;
    Serial.print("== constants: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    

    
    microStepsToGo = stepsToGo << highestSteppingMode;
    microStepsForAcceleration = stepsForAcceleration << highestSteppingMode;
    microStepsForAccAndConstSpeed = microStepsToGo - (stepsForDeceleration << highestSteppingMode);

    t_stop = micros() - t_prepMove;
    Serial.print("== steps to microSteps: ");
    Serial.println(t_stop);
    t_prepMove = micros();
    

    if (state & MOVING) {
        t0 += lastStepTime;
        currentMicroStep = 1;
        stepExpiration = 1000000.0 * (-currentVelocity + sqrt(currVeloBySquare + currentMicroStep * c0_acc)) * acceleration_inv;
        
        
        t_stop = micros() - t_prepMove;
        Serial.print("== stepExpiration when moving: ");
        Serial.println(t_stop);
        
        if (digitalRead(VERBOSE_BUTTON)) {
//        if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
            //        Serial.print(F("[CCStepperDevice]: "));
            //        Serial.print(deviceName);
            Serial.print(F("currentVelocity: "));
            Serial.print(currentVelocity);
            Serial.print(F("  ["));
            Serial.print(lastStepTime);
            Serial.print(F(" --> 0] next step at: "));
            Serial.print((unsigned long)stepExpiration);
            Serial.print(F(" --> "));
            Serial.println((unsigned long)stepExpiration + lastStepTime);
        }
    }
    else {
//        Serial.print("== prepareMove: ");
//        Serial.println(t_stop);
    }
    
    
//    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MEMORYDEBUG) {
//        Serial.print(F("[CCStepperDevice]: "));
//        Serial.print(deviceName);
//        Serial.print(F(": stepsToGo at $"));
//        Serial.print((long)&stepsToGo, HEX);
//        Serial.print(F(", velocity at $"));
//        Serial.print((long)&velocity, HEX);
//        Serial.print(F(", acceleration at $"));
//        Serial.println((long)&acceleration, HEX);
//    }

    //    if (digitalRead(VERBOSE_BUTTON)) {
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_CALCULATIONDEBUG) {
        Serial.print(F("[CCStepperDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": prepare move "));
        Serial.print(movePointer);
        Serial.print(F(": "));
        Serial.print(F("target: "));
        Serial.print(target);
        Serial.print(F(", stepsToGo: "));
        Serial.print(stepsToGo);
        Serial.print(F(" ("));
        Serial.print(microStepsToGo);
        Serial.print(F("), directionDown: "));
        Serial.print(directionDown);
        Serial.print(F(", targetDirectionDown: "));
        Serial.print(targetDirectionDown);
        Serial.print(F(", changeDirection: "));
        Serial.print(changeDirection);
        Serial.print(F(", currentVelocity: "));
        Serial.print(currentVelocity);
        Serial.print(F(", velocity: "));
        Serial.print(velocity);
        Serial.print(F(", acceleration: "));
        Serial.print(acceleration);
        Serial.print(F(", deceleration: "));
        Serial.print(deceleration);
        Serial.print(F(", stepsForAcceleration: "));
        Serial.print(stepsForAcceleration);
        Serial.print(F(" ("));
        Serial.print(microStepsForAcceleration);
        Serial.print(F("), stepsForDeceleration: "));
        Serial.print(stepsForDeceleration);
        Serial.print(F(" ("));
        Serial.print(microStepsToGo - microStepsForAccAndConstSpeed);
        Serial.print(F("), stepsTillZero: "));
        Serial.print(microStepsTillZero >> highestSteppingMode);
        Serial.print(F(" ("));
        Serial.print(microStepsTillZero);
        Serial.print(F("), timeForAcceleration: "));
        Serial.print(timeForAcceleration);
        Serial.print(F(", c0_acc: "));
        Serial.print(c0_acc);
        Serial.print(F(", c0_dec: "));
        Serial.print(c0_dec);
        Serial.print(F(", c1: "));
        Serial.println(c1);
    }
}

void CCStepperDevice::startMove() {
    if (stepsToGo == 0 || velocity == 0 || acceleration == 0) {
        stopMoving();
    }
    else {
        // lets start in highest stepping mode
        microSteppingMode = highestSteppingMode;
        setMicroStepPins();
        
        enableDevice();
        
        if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
            Serial.print(F("[CCStepperDevice]: "));
            Serial.print(deviceName);
            Serial.print(F(", start move: "));
            Serial.println((int)movePointer);
        }
                
        state |= MOVING;
        currentMicroStep = 0;
        stepExpiration = 0;
        t0 = micros() & 0x7fffffff;
        
        driveDynamic();
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
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": stop move "));
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
        
        currentMicroStep += steppingUnit[microSteppingMode];
        
        if (directionDown) {
            currentPosition -= positionUnit[microSteppingMode];
        }
        else {
            currentPosition += positionUnit[microSteppingMode];
        }
        
        
        // while ramping up
        if (currentMicroStep <= microStepsForAcceleration) {
            lastStepTime = stepExpiration;
            if (changeDirection && (currentMicroStep > microStepsTillZero)) {
                if (directionDown != targetDirectionDown) {
                    directionDown = targetDirectionDown;
                    digitalWrite(dir_pin, directionDown);
                }
//                stepExpiration = 1000000.0 * (-currentVelocity - sqrt(-currentVelocity * currentVelocity - currentMicroStep * c0_acc)) / acceleration;
                stepExpiration = 1000000.0 * (-sqrt(-currVeloBySquare - currentMicroStep * c0_acc) - currentVelocity) * acceleration_inv;
            }
            else {
//                stepExpiration = 1000000.0 * (-currentVelocity + sqrt(currentVelocity * currentVelocity + currentMicroStep * c0_acc)) / acceleration;
                stepExpiration = 1000000.0 * (sqrt(currVeloBySquare + currentMicroStep * c0_acc) - currentVelocity) * acceleration_inv;
            }
            
            if (stepExpiration - lastStepTime < STEPPINGPERIOD_TO_KICK_UP) kickUp();
            
            if (digitalRead(VERBOSE_BUTTON)) {
//            if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
                //        Serial.print(F("[CCStepperDevice]: "));
                //        Serial.print(deviceName);
                Serial.print((unsigned long)elapsedTime);
                Serial.print(F(": up   , # "));
                Serial.print(currentMicroStep);
                Serial.print(F(", Pos: "));
                Serial.print(currentPosition);
                Serial.print(F(", next step: "));
                Serial.println((unsigned long)stepExpiration);
            }
            return;
        }
        
        // while going with constant speed
        if (currentMicroStep < microStepsForAccAndConstSpeed) {
            lastStepTime = stepExpiration;
            stepExpiration = timeForAcceleration + (currentMicroStep - microStepsForAcceleration) * 1000000.0 * c1;
            
            if (digitalRead(VERBOSE_BUTTON)) {
//            if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
                //        Serial.print(F("[CCStepperDevice]: "));
                //        Serial.print(deviceName);
                Serial.print((unsigned long)elapsedTime);
                Serial.print(F(": const, # "));
                Serial.print(currentMicroStep);
                Serial.print(F(", Pos: "));
                Serial.print(currentPosition);
                Serial.print(F(", next step: "));
                Serial.println((unsigned long)stepExpiration);
            }
            return;
        }
        
        // while ramping down
        if (currentMicroStep < microStepsToGo) {
            lastStepTime = stepExpiration;
            stepExpiration = timeForAccAndConstSpeed + 1000000.0 * (sqrt(veloBySquare + (currentMicroStep - microStepsForAccAndConstSpeed) * c0_dec) - velocity) * deceleration_inv;
            
            if (stepExpiration - lastStepTime > STEPPINGPERIOD_TO_KICK_DOWN) kickDown();
            
            if (digitalRead(VERBOSE_BUTTON)) {
//          if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
                //        Serial.print(F("[CCStepperDevice]: "));
                //        Serial.print(deviceName);
                Serial.print((unsigned long)elapsedTime);
                Serial.print(F(": down , # "));
                Serial.print(currentMicroStep);
                Serial.print(F(", Pos: "));
                Serial.print(currentPosition);
                Serial.print(F(", next step: "));
                Serial.println((unsigned long)stepExpiration);
            }
            return;
        }
        
        // very last step
        if (currentMicroStep == microStepsToGo) {
            stepExpiration = -velocity * deceleration_inv;
            
            if (digitalRead(VERBOSE_BUTTON)) {
//            if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
                //        Serial.print(F("[CCStepperDevice]: "));
                //        Serial.print(deviceName);
                Serial.print((unsigned long)elapsedTime);
                Serial.print(F(": last , # "));
                Serial.print(currentMicroStep);
                Serial.print(F(", Pos: "));
                Serial.print(currentPosition);
                Serial.print(F(", very last: "));
                Serial.println((unsigned long)stepExpiration);
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
            
            setMicroStepPins();
            
        }
    }
}

void CCStepperDevice::kickDown() {
    if (microSteppingMode < highestSteppingMode) {
        if ((currentMicroStep & 0x0F) == 0) {
            microSteppingMode++;
            
            setMicroStepPins();
            
        }
    }
}

void CCStepperDevice::setMicroStepPins() {
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        digitalWrite(microStepPin[pinIndex], stepModeCode[microSteppingMode] & (1 << pinIndex));
    }
}
void CCStepperDevice::setMicroStepPins(unsigned char data) {
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        digitalWrite(microStepPin[pinIndex], data & (1 << pinIndex));
    }
}


/*
 
 osascript -e '
 tell application "System Events"
 try
 say "try to open serial monitor"
 click menu item "Serieller Monitor" of menu "Werkzeuge" of menu bar item "Werkzeuge" of menu bar 1 of process "Arduino"
 end try
 end tell
 '
 
 
 if (velocity == 0) {
 // lets say then: 20
 velocity = 20;
 }
 if (acceleration == 0) {
 // lets say then: wayForConstSpeed = 0 ==>
 // wayToGo / 2 = v * v / a ==> a = 2 * v * v / wayToGo
 acceleration = 2 * velocity * velocity / target;
 }
 
 */
/*
void CCStepperDevice::prepareNextMove() {
    currentVelocity = sqrt(2 * abs(deceleration) * (microStepsToGo - currentMicroStep / (1 << highestSteppingMode));
    stepsToGo = (target - currentPosition) * 200 / 360;
    stepsForAcceleration = (0.5 * abs(veloBySquare - currVeloBySquare) / acceleration) + 0.5;
    stepsForDeceleration = stepsToGo / (abs(veloBySquare - currVeloBySquare) * deceleration / acceleration / veloBySquare + 1) - 1;
    stepsForAcceleration = stepsToGo - 2 - stepsForDeceleration;
    acceleration = -0.5 * (veloBySquare + currVeloBySquare) / stepsForAcceleration;
    deceleration = -0.5 * veloBySquare / stepsForDeceleration;
    velocity = sqrt(2 * acceleration_max * stepsForAcceleration - currVeloBySquare);
    microStepsTillZero = (unsigned long)abs(0.5 * currVeloBySquare / acceleration) << highestSteppingMode;
    timeForAcceleration = abs(1000000.0 * (currentVelocity - velocity) / acceleration);
    timeForAccAndConstSpeed = timeForAcceleration + 1000000UL * (stepsToGo - stepsForAcceleration - stepsForDeceleration) / velocity;
    c0_acc = 2.0 * acceleration / (1 << highestSteppingMode);
    c1 = 1 / velocity / (1 << highestSteppingMode);
    stepExpiration = 1000000.0 * (-currentVelocity + sqrt(currVeloBySquare + currentMicroStep * c0_acc)) / acceleration;
}
*/