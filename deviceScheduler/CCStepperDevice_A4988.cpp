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

CCStepperDevice_A4988::CCStepperDevice_A4988(const String deviceName, const unsigned int step_pin, const unsigned int dir_pin, const unsigned int enable_pin, const unsigned int stepsPerRotation, const unsigned int microStep_00_pin, const unsigned int microStep_01_pin, const unsigned int microStep_02_pin, const signed char steppingCode_00, const signed char steppingCode_01, const signed char steppingCode_02, const signed char steppingCode_03, const signed char steppingCode_04, const signed char steppingCode_05, const signed char steppingCode_06, const signed char steppingCode_07) : CCStepperDevice(deviceName, step_pin, dir_pin, enable_pin, stepsPerRotation) {

//    microStepPin{microStep_00_pin, microStep_01_pin, microStep_02_pin};
//    this->microStepPin = new unsigned int[NUMBER_OF_MICROSTEPPING_PINS] {microStep_00_pin, microStep_01_pin, microStep_02_pin};
    
    this->microStepPin[0] = microStep_00_pin;                              // and fill it
    this->microStepPin[1] = microStep_01_pin;                              // and fill it
    this->microStepPin[2] = microStep_02_pin;                              // and fill it
    
    this->highestSteppingMode = 7;
    this->stepModeCode = new unsigned int[highestSteppingMode + 1];                        // create array for microStep pin configuration
    this->stepModeCode[0] = steppingCode_00;
    this->stepModeCode[1] = steppingCode_01;
    this->stepModeCode[2] = steppingCode_02;
    if (steppingCode_03 == -1) {
        this->highestSteppingMode = 2;
    } else {
        this->stepModeCode[3] = steppingCode_03;
        if (steppingCode_04 == -1) {
            this->highestSteppingMode = 3;
        } else {
            this->stepModeCode[4] = steppingCode_04;
            if (steppingCode_05 == -1) {
                this->highestSteppingMode = 4;
            } else {
                this->stepModeCode[5] = steppingCode_05;
                if (steppingCode_06 == -1) {
                    this->highestSteppingMode = 5;
                } else {
                    this->stepModeCode[6] = steppingCode_06;
                    if (steppingCode_07 == -1) {
                        this->highestSteppingMode = 6;
                    } else {
                        this->stepModeCode[7] = steppingCode_07;
                        this->highestSteppingMode = 7;
                    }
                }
            }
        }
    }
    
    for (unsigned int codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
        this->steppingUnit[codeIndex] = (1 << (highestSteppingMode - codeIndex));
    }
    
    for (unsigned int pinIndex = 0; pinIndex < NUMBER_OF_MICROSTEPPING_PINS; pinIndex++) {
        pinMode(microStepPin[pinIndex], OUTPUT);
        digitalWrite(microStepPin[pinIndex], LOW);
    }
    
    
    this->stepsPerDegree = stepsPerRotation / 360.0;                                        // save time executing prepareNextTask()
    this->degreesPerMicroStep = 360.0 / stepsPerRotation / (1 << highestSteppingMode);      // save time when calculatin currentPosition in operateTask()
    
    this->acceleration_max = 4000;
    
    this->state = SLEEPING;
    
    this->currentMicroStep = 0;
    this->currentPosition = 0;
    
    this->prepareAndStartNextTaskWhenFinished = false;
    
    
    //        Serial.print(F("[CCStepperDevice_A4988]: setup stepper "));
    //        Serial.print(deviceName);
    //        Serial.print(F(": NUMBER_OF_MICROSTEPPING_PINS: "));
    //        Serial.print(NUMBER_OF_MICROSTEPPING_PINS);
    //        Serial.print(F(", pins: "));
    //        for (unsigned int pinIndex = 0; pinIndex < NUMBER_OF_MICROSTEPPING_PINS; pinIndex++) {
    //            Serial.print(microStepPin[pinIndex]);
    //            Serial.print(F(", "));
    //        }
    //        Serial.print(F("steppingModes: "));
    //        Serial.print(highestSteppingMode + 1);
    //        Serial.print(F(", stepModeCodes: "));
    //        for (unsigned int codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
    //            Serial.print(stepModeCode[codeIndex]);
    //            Serial.print(F(", "));
    //        }
    //        if (verbosity & MEMORYDEBUG) {
    //            Serial.print(F(", at $"));
    //            Serial.print((long) this, HEX);
    //        }
    //        Serial.println();
}


CCStepperDevice_A4988::~CCStepperDevice_A4988() {
    digitalWrite(enable_pin, HIGH);
    
    pinMode(dir_pin, INPUT);
    pinMode(step_pin, INPUT);
    pinMode(enable_pin, INPUT_PULLUP);
    
    for (unsigned int pinIndex = 0; pinIndex < NUMBER_OF_MICROSTEPPING_PINS; pinIndex++) {
        pinMode(microStepPin[pinIndex], INPUT);
    }
    
    delete [] stepModeCode;
    stepModeCode = NULL;
    
    if (verbosity & BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice_A4988]: device "));
        Serial.print(deviceName);
        Serial.println(F(" destructed"));
    }
}


void CCStepperDevice_A4988::setupMicroSteppingMode() {
    for (unsigned int pinIndex = 0; pinIndex < NUMBER_OF_MICROSTEPPING_PINS; pinIndex++) {
        digitalWrite(microStepPin[pinIndex], stepModeCode[microSteppingMode] & (1 << pinIndex));
    }
}
void CCStepperDevice_A4988::setupMicroSteppingMode(unsigned int data) {
    for (unsigned int pinIndex = 0; pinIndex < NUMBER_OF_MICROSTEPPING_PINS; pinIndex++) {
        digitalWrite(microStepPin[pinIndex], data & (1 << pinIndex));
    }
}

void CCStepperDevice_A4988::getReadOut(unsigned int theReadOut) {}


