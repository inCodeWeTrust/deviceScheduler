//
//  CCDevice.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//



#include "CCDevice.h"
#include "deviceScheduler.h"

CCDevice::~CCDevice() {}

void CCDevice::defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration) {
    this->defaultVelocity = defaultVelocity;
    this->defaultAcceleration = defaultAcceleration;
    this->defaultDeceleration = defaultDeceleration;
}

/*
 unsigned char CCDevice::addTask(float target) {
 return addMoveWithStartDelay(target, 0UL, defaultVelocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addTask(float target, float velocity) {
 return addMoveWithStartDelay(target, 0UL, velocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addTask(float target, float velocity, float acceleration) {
 return addMoveWithStartDelay(target, 0UL, velocity, acceleration, defaultDeceleration);
 }
 */
unsigned char CCDevice::addTask(float target, float velocity, float acceleration, float deceleration) {
    return addMoveWithStartDelay(target, 0UL, velocity, acceleration, deceleration);
}

/*
 unsigned char CCDevice::addMoveWithStartDelay(float target, unsigned long startDelay) {
 return addMoveWithStartDelay(target, startDelay, defaultVelocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addMoveWithStartDelay(float target, unsigned long startDelay, float velocity) {
 return addMoveWithStartDelay(target, startDelay, velocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration) {
 return addMoveWithStartDelay(target, startDelay, velocity, acceleration, defaultDeceleration);
 }
 */
unsigned char CCDevice::addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration) {
//    task[countOfTasks] = new onEventMove(target, velocity, acceleration, deceleration, startDelay);
    task[countOfTasks] = new CCTask(target, velocity, acceleration, deceleration, startDelay);
    
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCDevice]: add Move for "));
        Serial.print(deviceName);
        Serial.print(F(", target: "));
        Serial.print(task[countOfTasks]->target);
        Serial.print(F(", velocity: "));
        Serial.print(task[countOfTasks]->velocity);
        Serial.print(F(", acceleration: "));
        Serial.print(task[countOfTasks]->acceleration);
        Serial.print(F(", deceleration: "));
        Serial.print(task[countOfTasks]->deceleration);
        Serial.print(F(", startDelay: "));
        Serial.print(task[countOfTasks]->startDelay);
        Serial.print(F("; at $"));
        Serial.println((long)task[countOfTasks], HEX);
    }
    
    countOfTasks++;
    
    return countOfTasks - 1;
}

void CCDevice::deleteMoves() {
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCDevice]: delete tasks of "));
        Serial.print(deviceName);
        Serial.print(F(": move "));
    }
    for (int j = countOfTasks - 1; j >= 0; j--) {
        if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
            Serial.print(F(" #"));
            Serial.print(j);
        }
        delete task[j];
        task[j] = NULL;
    }
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.println();
        Serial.println();
    }
    
    state = SLEEPING;
    taskPointer = 0;
    countOfTasks = 0;
    
//    currentPosition = 0;
//    directionDown = 0;
    
}
