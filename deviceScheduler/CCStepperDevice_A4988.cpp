//
// CCStepperDevice_A4988.cpp 
// C++ code
// ----------------------------------
// Developed with embedXcode+ 
// http://embedXcode.weebly.com
//
// Project 		deviceScheduler
//
// Created by 	Little Abakus, 14.04.16 22:42
// 				Little Abakus
//
// Copyright 	(c) Little Abakus, 2016
// License		<#license#>
//
// See 			CCStepperDevice_A4988.h and ReadMe.txt for references
//



#include "CCStepperDevice_A4988.h"

CCStepperDevice_A4988::CCStepperDevice_A4988(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCode, unsigned char numberOfMicroStepPins, unsigned char *microStepPin, unsigned int stepsPerRotation) : CCStepperDevice() {
    
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
    this->steppingUnit = new unsigned int[highestSteppingMode + 1];                        // create array for increment of microSteps according to microSteppingMode

    for (unsigned char codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
        this->stepModeCode[codeIndex] = stepModeCode[codeIndex];
        this->steppingUnit[codeIndex] = (1 << (highestSteppingMode - codeIndex));
    }
    
    this->stepsPerDegree = stepsPerRotation / 360.0;                                              // save time executing prepareNextTask()
    this->degreesPerMicroStep = 360.0 / stepsPerRotation / (1 << highestSteppingMode);            // save time when calculatin currentPosition in operateTask()
    
    this->acceleration_max = 4000;
    
    this->type = STEPPERDEVICE;
    this->state = SLEEPING;
    this->taskPointer = 0;
    this->countOfTasks = 0;
    
    this->defaultVelocity = 0;
    this->defaultAcceleration = 0;
    this->defaultDeceleration = 0;
    
    this->currentMicroStep = 0;
    this->currentPosition = 0;
    
    this->prepareAndStartNextTaskWhenFinished = false;
    
    if (CCSTEPPERDEVICE_A4988_VERBOSE & CCSTEPPERDEVICE_A4988_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice_A4988]: setup "));
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


CCStepperDevice_A4988::~CCStepperDevice_A4988() {
    detachDevice();
    free(stepModeCode);
    free(microStepPin);
}



void CCStepperDevice_A4988::attachDevice() {
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(enable_pin, OUTPUT);
    digitalWrite(enable_pin, HIGH);
    
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        pinMode(microStepPin[pinIndex], OUTPUT);
        digitalWrite(microStepPin[pinIndex], LOW);
    }
    
    if (CCSTEPPERDEVICE_A4988_VERBOSE & CCSTEPPERDEVICE_A4988_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice_A4988]: "));
        Serial.print(deviceName);
        Serial.println(F(" attached"));
    }
}
void CCStepperDevice_A4988::detachDevice() {
    digitalWrite(enable_pin, HIGH);
    
    pinMode(dir_pin, INPUT);
    pinMode(step_pin, INPUT);
    pinMode(enable_pin, INPUT);
    
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        pinMode(microStepPin[pinIndex], INPUT);
    }
    
    if (CCSTEPPERDEVICE_A4988_VERBOSE & CCSTEPPERDEVICE_A4988_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice_A4988]: device "));
        Serial.print(deviceName);
        Serial.println(F(" detached"));
    }
}



void CCStepperDevice_A4988::setupMicroSteppingMode() {
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        digitalWrite(microStepPin[pinIndex], stepModeCode[microSteppingMode] & (1 << pinIndex));
    }
}
void CCStepperDevice_A4988::setupMicroSteppingMode(unsigned char data) {
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {
        digitalWrite(microStepPin[pinIndex], data & (1 << pinIndex));
    }
}

void CCStepperDevice_A4988::getReadOut(byte theReadOut) {}


