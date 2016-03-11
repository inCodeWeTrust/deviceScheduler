//
//  CCStepperDevice.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <avr/pgmspace.h>
#include "CCStepperDevice.h"



CCStepperDevice::CCStepperDevice(unsigned int deviceIndex, String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCode, unsigned char numberOfMicroStepPins, unsigned char *microStepPin, unsigned int stepsPerRotation) : CCDevice() {
    
    this->deviceIndex = deviceIndex;
    this->deviceName = deviceName;
    this->dir_pin = dir_pin;
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
    
    this->numberOfMicroStepPins = numberOfMicroStepPins;
    this->microStepPin = new unsigned char[numberOfMicroStepPins];                          // create array for the pins
    
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        this->microStepPin[pinIndex] = microStepPin[pinIndex];                              // and fill it
    }
    
    this->highestSteppingMode = highestSteppingMode;
    this->stepModeCode = new unsigned char[highestSteppingMode + 1];                        // create array for microStep pin configuration
    this->steppingUnit = new unsigned char[highestSteppingMode + 1];                        // create array for increment of microSteps according to microSteppingMode
    
    for (unsigned char codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
        this->stepModeCode[codeIndex] = stepModeCode[codeIndex];
        this->steppingUnit[codeIndex] = (1 << (highestSteppingMode - codeIndex));
    }
    
    stepsPerDegree = stepsPerRotation / 360.0;                                              // save time executing prepareNextTask()
    degreesPerMicroStep = 360.0 / stepsPerRotation / (1 << highestSteppingMode);            // save time when calculatin currentPosition in operateTask()
    
    acceleration_max = 4000;
    
    type = STEPPERDEVICE;
    state = SLEEPING;
    taskPointer = 0;
    countOfTasks = 0;
    
    defaultVelocity = 0;
    defaultAcceleration = 0;
    defaultDeceleration = 0;
    
    currentMicroStep = 0;
    currentPosition = 0;
    
    prepareAndStartNextTaskWhenFinished = false;
    
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
            Serial.print(F(", "));
        }
        Serial.print(F("steppingModes: "));
        Serial.print(highestSteppingMode + 1);
        Serial.print(F(", stepModeCodes: "));
        for (unsigned char codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
            Serial.print(stepModeCode[codeIndex]);
            Serial.print(F(", "));
        }
        Serial.print(F("stepsPerDegree: "));
        Serial.print(stepsPerDegree);
        Serial.print(F(", at $"));
        Serial.println((long) this, HEX);
    }
    
    attachDevice();
    
}

CCStepperDevice::~CCStepperDevice() {
    deleteTasks();
    detachDevice();
}

void CCStepperDevice::enableDevice() {
    digitalWrite(enable_pin, LOW);
}
void CCStepperDevice::disableDevice() {
    digitalWrite(enable_pin, HIGH);
    digitalWrite(dir_pin, LOW);
    setMicroStepPins(0);
}

void CCStepperDevice::attachDevice() {
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(enable_pin, OUTPUT);
    digitalWrite(enable_pin, HIGH);
    
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        pinMode(microStepPin[pinIndex], OUTPUT);
        digitalWrite(microStepPin[pinIndex], LOW);
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


void CCStepperDevice::reviewValues() {
    Serial.print(F("[CCStepperDevice]: device "));
    Serial.print(deviceName);
    Serial.print(F(" review Values for move "));
    for (unsigned char m = 0; m < countOfTasks; m++) {
        Serial.print(" #");
        Serial.print(m);

        if (task[m]->velocity == 0) {
            task[m]->velocity = defaultVelocity;
        }
        if (task[m]->acceleration == 0) {
            task[m]->acceleration = defaultAcceleration;
        }
        if (task[m]->deceleration == 0) {
            task[m]->deceleration = defaultDeceleration;
        }

        if (task[m]->deceleration == 0) {
            task[m]->deceleration = task[m]->acceleration;
        }

        // v[steps/sec] = v[angle/sec] * stepsPerAngle
        task[m]->velocity *= stepsPerDegree;

        // a[steps/sec/sec] = a[angle/sec/sec] * stepsPerAngle
        task[m]->acceleration *= stepsPerDegree;
        task[m]->deceleration *= stepsPerDegree;
    }
    Serial.println("   done");

}

void CCStepperDevice::prepareNextTask() {
    unsigned long t_prepTask = micros();
    unsigned long t_stop, t_sum = 0;

    if (state == MOVING) {
        if (switchTaskPromptly) {
            if ((task[taskPointer]->target - currentPosition) < 0) {             // when switching to move in different direction
                if (!directionDown) {
                    prepareAndStartNextTaskWhenFinished = true;
                    initiateStop();
                    return;
                }
            }
            else {
                if (directionDown) {
                    prepareAndStartNextTaskWhenFinished = true;
                    initiateStop();
                    return;
                }
            }
        }
        
        if (currentMicroStep & 0x0f) {
            switchAtNextFullStep = true;
            return;
        }
        // *** current velocity ***
        if (currentMicroStep < microStepsForAcceleration) {
            // v = a * t
            currentVelocity = elapsedTime * 0.000001 * acceleration;
        }
        else if (currentMicroStep < microStepsForAccAndConstSpeed) {
            currentVelocity = velocity;
        }
        else {
            // v - v0 = a * t ==> v = a * t + v0
            currentVelocity = (elapsedTime - timeForAccAndConstSpeed) * 0.000001 * deceleration + velocity;
        }
    }
    else {
        currentVelocity = 0;
    }

//    this takes ca 30us
//    t_stop = micros() - t_prepTask;
//    Serial.print("== currentVelocity: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();

    
    startPosition = currentPosition;
    
    target = task[taskPointer]->target;
    velocity = task[taskPointer]->velocity;
    acceleration = task[taskPointer]->acceleration;
    deceleration = task[taskPointer]->deceleration;
    
    
//    Serial.print(F("### currentPosition: "));
//    Serial.println(currentPosition);
//
//    Serial.print(F("### target: "));
//    Serial.print(this->target);
//    Serial.print(F(", velocity: "));
//    Serial.print(this->velocity);
//    Serial.print(F(", acceleration: "));
//    Serial.print(this->acceleration);
//    Serial.print(F(", deceleration: "));
//    Serial.println(this->deceleration);

    
    startEvent = task[taskPointer]->startEvent;
    stopEvent = task[taskPointer]->stopEvent;
    switchTaskPromptly = task[taskPointer]->switchTaskPromptly;
    startDelay = task[taskPointer]->startDelay;
    startTime = task[taskPointer]->startTime;
    timeout = task[taskPointer]->timeout;
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
    stopping = task[taskPointer]->stopping;
    
    //    this takes ca 24us
//    t_stop = micros() - t_prepTask;
//    Serial.print("== copy parameters: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();

    stepsToGo = (float)(target - currentPosition) * stepsPerDegree;

    //    this takes ca 4us
//    t_stop = micros() - t_prepTask;
//    Serial.print("== stepsToGo: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();
    
    if (stepsToGo < 0) {
        stepsToGo = -stepsToGo;
        directionDown = true;
    }
    else {
        directionDown = false;
    }
    
    //    this takes ca 4us
//    t_stop = micros() - t_prepTask;
//    Serial.print("== stepsToGo < 0: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();
    
    
    veloBySquare = velocity * velocity;
    currVeloBySquare = currentVelocity * currentVelocity;
    
    
    // *** steps for deceleration: ***
    // v * v = 2 * a * n
    // ==> v * v = 2 * a * n ==> n = v * v / (2 * a)
    stepsForDeceleration = (unsigned long)(veloBySquare / deceleration) >> 1;
    
    //    this takes ca 40us
//    t_stop = micros() - t_prepTask;
//    Serial.print("== stepsForDeceleration: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();
    
    

    accelerateDown = (currentVelocity > velocity);
    
    // *** steps for acceleration: ***
    // v * v - v0 * v0 = 2 * a * n
    // ==> n = (v * v - v0 * v0) / (2 * a)
    if (accelerateDown) {
        stepsForAcceleration = (unsigned long)((currVeloBySquare - veloBySquare) / acceleration) >> 1;
    }
    else {
        stepsForAcceleration = (unsigned long)((veloBySquare - currVeloBySquare) / acceleration) >> 1;
    }
    
    //    this takes ca 60us
//    t_stop = micros() - t_prepTask;
//    Serial.print("== stepsForAcceleration: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();
    
    /*
    Serial.print(F("### stepsToGo: "));
    Serial.print(this->stepsToGo);
    Serial.print(F(" directionDown: "));
    Serial.print(this->directionDown);
    Serial.print(F(" stepsForDeceleration: "));
    Serial.print(this->stepsForDeceleration);
    Serial.print(F(", accelerateDown: "));
    Serial.print(this->accelerateDown);
    Serial.print(F(", stepsForAcceleration: "));
    Serial.println(this->stepsForAcceleration);
    */
    
    
    // *** does acceleration and deceleration fit into the move? ***
    if (stepsForAcceleration + stepsForDeceleration > stepsToGo - 2) {
        
        Serial.println(F("!!!! too less steps !!!!"));

        // v * v - v0 * v0 = 2 * a_acc * s_acc ==> s_acc = (v * v - v0 * v0) / (2 * a_acc)
        // v * v = 2 * a_dec * s_dec => s_dec = v * v / (2 * a_dec)
        // ==> s_acc / s_dec = (v * v - v0 * v0) / (2 * a_acc) / (v * v / (2 * a_dec)) = a_dec * (v * v - v0 * v0) / (a_acc * v * v)
        // s = s_acc + s_dec;
        // s_acc / s_dec = c ==> s_acc = c * s_dec
        // ==> s = c * s_dec + s_dec = s_dec * (c + 1) ==> s_dec = s / (c + 1)
        // ==> s_dec = s / (a_dec * (v * v - v0 * v0) / (a_acc * v * v) + 1)
        if (accelerateDown) {
            stepsForDeceleration = (float) stepsToGo / ((currVeloBySquare - veloBySquare) * deceleration / (acceleration * veloBySquare) + 1);
        }
        else {
            stepsForDeceleration = (float) stepsToGo / ((veloBySquare - currVeloBySquare) * deceleration / (acceleration * veloBySquare) + 1);
        }
        stepsForAcceleration = stepsToGo - 2 - stepsForDeceleration--;
        
        Serial.print(F(" stepsForDeceleration: "));
        Serial.print(this->stepsForDeceleration);
        Serial.print(F(", stepsForAcceleration: "));
        Serial.println(this->stepsForAcceleration);

    }
    
    //    this takes ca 4us (no recalculation)
//    t_stop = micros() - t_prepTask;
//    Serial.print("== recalculate steps: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();
    
    

    // *** recalculate a: ***
    // v * v = 2 * a * n ==> a = v * v / (2 * n)
    deceleration = -veloBySquare / (stepsForDeceleration << 1);
    
    if (deceleration < -acceleration_max) {                             // !!!! too much deceleration !!!!
        deceleration = -acceleration_max;
        // *** recalculate v: ***
        // v * v = 2 * a * n ==> v = sqrt(2 * a * n)
        //        veloBySquare = 2 * acceleration_max * stepsForDeceleration;
        veloBySquare = acceleration_max * (stepsForDeceleration << 1);
        velocity = sqrt(veloBySquare);
    }
    
    // *** recalculate a: ***
    if (stepsForAcceleration > 0) {                                     // !!!! too much acceleration !!!!
        // v * v - v0 * v0 = 2 * a * n ==> a = (v * v - v0 * v0) / (2 * n)
        acceleration = (veloBySquare - currVeloBySquare) / (stepsForAcceleration << 1);
        if (acceleration > acceleration_max) {
            Serial.println(F("!!!! too much acceleration !!!!"));
            acceleration = acceleration_max;
            // *** recalculate v: ***
            // v * v - v0 * v0 = 2 * a * n ==> v * v = 2 * a * n + v0 * v0
            veloBySquare = (stepsForAcceleration << 1) * acceleration_max + currVeloBySquare;
            velocity = sqrt(veloBySquare);
            
            deceleration = -veloBySquare / (stepsForDeceleration << 1);
        }
        else if (acceleration < -acceleration_max){
            Serial.println(F("!!!! too much acceleration !!!!"));
            acceleration = -acceleration_max;
            // *** recalculate v: ***
            // v * v - v0 * v0 = 2 * a * n ==> v * v = 2 * a * n + v0 * v0
            veloBySquare = (stepsForAcceleration << 1) * acceleration_max + currVeloBySquare;
            velocity = sqrt(veloBySquare);
            
            deceleration = -veloBySquare / (stepsForDeceleration << 1);
        }
        acceleration_inv = 1 / acceleration;
    }
    deceleration_inv = 1 / deceleration;
    
    
    // *** time for acceleration: ***
    // a = v0 / t0; a = v / t ==> t0 = v0 / a; t = v / a
    // tx = t - t0; ==> tx = v / a - v0 / a = (v - v0) / a
    // in case accelerateUp: (velocity - currentVelocity) > 0 ==> acceleration_inv > 0 ==> timeForAcceleration > 0
    // in case accelerateDown: (velocity - currentVelocity) < 0 ==> acceleration_inv < 0 ==> timeForAcceleration > 0
    timeForAcceleration = 1000000.0 * (velocity - currentVelocity) * acceleration_inv;
    timeForAccAndConstSpeed = 1000000.0 * (stepsToGo - stepsForAcceleration - stepsForDeceleration) / velocity + timeForAcceleration;
    

    // *** time for the next step while acceleration: ***
    // n = 0.5 * a * t * t + v0 * t
    // 0.5 * a * t * t + v0 * t - n = 0
    // ==> t = (-v0 +/- sqrt(v0 * v0 + 2 * a * n)) / a [* 1000000us/s]
    // ==> t = (-v0 +/- sqrt(v0 * v0 + 2 * a * microSteps / microStepsPerStep)) / a [* 1000000us/s]
    // ==> t = (-v0 +/- sqrt(v0 * v0 + microSteps * 2 * a / microStepsPerStep)) / a [* 1000000us/s]
    // 2 * a / microStepsPerStep = constant = c0
    // t = (-v0 +/- sqrt(v0 * v0 + microSteps * c0)) / a [* 1000000us/s]
    c0_acc = 2.0 * acceleration / (1 << highestSteppingMode);
    c0_dec = 2.0 * deceleration / (1 << highestSteppingMode);
    
    
    
    // *** time for next step while constant speed: ***
    // v = n / t ==> t = n / v
    // t = microSteps / microStepsPerStep / v
    // ==> t = microSteps / (microStepsPerStep * v) [*1000000us/s]
    // 1000000.0 / (microStepsPerStep * v) = constant = c1
    // t = microSteps * c1
    c1 = 1000000.0 / (velocity * (1 << highestSteppingMode));
        
    //    this takes ca 132us
//    t_stop = micros() - t_prepTask;
//    Serial.print("== constants: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();
    
    

    microStepsToGo = stepsToGo << highestSteppingMode;
    microStepsForAcceleration = stepsForAcceleration << highestSteppingMode;
    microStepsForAccAndConstSpeed = microStepsToGo - (stepsForDeceleration << highestSteppingMode);
    
    //    this takes ca 12us
//    t_stop = micros() - t_prepTask;
//    Serial.print("== steps to microSteps: ");
//    Serial.println(t_stop);
//    t_sum += t_stop;
//    t_prepTask = micros();
    
    
    
    if (state == MOVING) {
        t0 += lastStepTime;
        currentMicroStep = steppingUnit[microSteppingMode];
        stepExpiration = 1000000.0 * (sqrt(currVeloBySquare + currentMicroStep * c0_acc) - currentVelocity) * acceleration_inv;
        
        //    this takes ca 88us
//        t_stop = micros() - t_prepTask;
//        Serial.print("== stepExpiration: ");
//        Serial.println(t_stop);
//        t_sum += t_stop;
//        t_prepTask = micros();
        

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
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_CALCULATIONDEBUG) {
        Serial.print(F("[CCStepperDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": prepare move "));
        Serial.print(this->taskPointer);
        Serial.print(F(": currentPosition: "));
        Serial.print(currentPosition);
        Serial.print(F(", target: "));
        Serial.print(this->target);
        Serial.print(F(", stepsToGo: "));
        Serial.print(this->stepsToGo);
        Serial.print(F(" ("));
        Serial.print(this->microStepsToGo);
        Serial.print(F("), currentVelocity: "));
        Serial.print(this->currentVelocity);
        Serial.print(F(", velocity: "));
        Serial.print(this->velocity);
        Serial.print(F(", acceleration: "));
        Serial.print(this->acceleration);
        Serial.print(F(", deceleration: "));
        Serial.println(this->deceleration);
    }

//    t_stop = micros() - t_prepTask;
//    Serial.print("== whole preperation: ");
//    Serial.println(t_stop);

}


void CCStepperDevice::startTask() {                                 // start this move
    if (stepsToGo == 0 || velocity == 0 || acceleration == 0) {     // values valid?
        stopTask();
    }
    else {
        digitalWrite(dir_pin, directionDown);                       // setup DIR-pin of stepper driver
        
        // lets start in highest stepping mode
        microSteppingMode = highestSteppingMode;                    // setup stepper driver's highest steppingMode
        setMicroStepPins();
        
        enableDevice();                                             // setup ENABLE-pin of stepper driver
        
        if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
            Serial.print(F("[CCStepperDevice]: "));
            Serial.print(deviceName);
            Serial.print(F(": start move: "));
            Serial.println((int)taskPointer);
        }
        
        state = MOVING;                                             // tag device as MOVING
        currentMicroStep = 0;                                       // start counting at 0
        stepExpiration = 0;                                         // set time for next step to 0 (= now)
        t0 = micros() & 0x7fffffff;                                 // remember starting time (but be aware of overflows)
        
        operateTask();                                                    // do step, if time expired and calculate time for next step
    }
}

void CCStepperDevice::initiateStop() {                              // irregular stopping
    if (currentMicroStep < microStepsForAcceleration) {             // if stop request appears while accelerating
                                                                    // stop acceleration immediately and decelerate symmetrical
        // what is my speed?
        // v * v [steps/s] = 2 * a * steps = 2 * a * microStep / microStepsPerFullStep
        veloBySquare = 2 * abs(acceleration) * currentMicroStep / (1 << highestSteppingMode);
        velocity = sqrt(veloBySquare);
        
        // do deceleration symetrical to acceleration: take same amount of microSteps for deceleration as used for acceleration till here
        microStepsToGo = 2 * currentMicroStep;                      // stepsToGo = stepsForAcceleration + stepsForDeceleration
        microStepsForAcceleration = currentMicroStep;               // stepsForAcceleration = currentStep = stepsForDeceleration
        microStepsForAccAndConstSpeed = currentMicroStep;           // stepsForConstSpeed = 0
        timeForAccAndConstSpeed = stepExpiration;                   // time for start of deceleration is time of next step
        return;
    }
    if (currentMicroStep < microStepsForAccAndConstSpeed) {         // when going with constant speed
        microStepsToGo = currentMicroStep + (stepsForDeceleration << highestSteppingMode);  // use nominated deceration values
        microStepsForAccAndConstSpeed = currentMicroStep;           // no more stepsForConstSpeed
        timeForAccAndConstSpeed = stepExpiration;                   // time for start of deceleration is time of next step
        return;
    }
}

void CCStepperDevice::stopTask() {
    state = MOVE_DONE;
}
void CCStepperDevice::finishTask() {
    state = SLEEPING;
    
    disableDevice();
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice]: "));
        Serial.print(deviceName);
        Serial.print(F(": stop move "));
        Serial.println((int)taskPointer);
    }
}

void CCStepperDevice::operateTask() {
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
            currentPosition = startPosition - currentMicroStep * degreesPerMicroStep;
        }
        else {
            currentPosition = startPosition + currentMicroStep * degreesPerMicroStep;
        }
        
        
        // while ramping up
        if (currentMicroStep <= microStepsForAcceleration) {
            lastStepTime = stepExpiration;
            stepExpiration = 1000000.0 * (sqrt(currVeloBySquare + currentMicroStep * c0_acc) - currentVelocity) * acceleration_inv;
            
            if (accelerateDown) {
                if (stepExpiration - lastStepTime > STEPPINGPERIOD_TO_KICK_DOWN) kickDown();
            }
            else {
                if (stepExpiration - lastStepTime < STEPPINGPERIOD_TO_KICK_UP) kickUp();
            }
            
            if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
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
            stepExpiration = timeForAcceleration + (currentMicroStep - microStepsForAcceleration) * c1;
            
            
            if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
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
            stepExpiration = timeForAccAndConstSpeed + (sqrt((currentMicroStep - microStepsForAccAndConstSpeed) * c0_dec + veloBySquare) - velocity) * deceleration_inv * 1000000.0;
            
            if (stepExpiration - lastStepTime > STEPPINGPERIOD_TO_KICK_DOWN) kickDown();
            
            if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
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
            stepExpiration = timeForAccAndConstSpeed - velocity * deceleration_inv;         // deceleration_inv < 0
            
            if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_MOVEMENTDEBUG) {
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
        
        if (prepareAndStartNextTaskWhenFinished) {
            prepareAndStartNextTaskWhenFinished = false;
            state = SLEEPING;
            prepareNextTask();
            startTask();
        }
        else {
            stopTask();
        }
    }
    else {
        if (switchAtNextFullStep) {
            if ((currentMicroStep & 0xF) == 0) {
                prepareNextTask();
                switchAtNextFullStep = false;
            }
        }
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
