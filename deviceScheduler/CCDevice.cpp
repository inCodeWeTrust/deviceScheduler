//
//  CCDevice.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include "CCDevice.h"



CCDevice::~CCDevice() {
    deleteTasks();
}

void CCDevice::defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration) {
    this->defaultVelocity = defaultVelocity;
    this->defaultAcceleration = defaultAcceleration;
    this->defaultDeceleration = defaultDeceleration;
}

/*
 unsigned char CCDevice::addTask(float target) {
 return addTaskWithStartDelay(target, 0UL, defaultVelocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addTask(float target, float velocity) {
 return addTaskWithStartDelay(target, 0UL, velocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addTask(float target, float velocity, float acceleration) {
 return addTaskWithStartDelay(target, 0UL, velocity, acceleration, defaultDeceleration);
 }
 */
unsigned char CCDevice::addTask(float target, float velocity, float acceleration, float deceleration) {
    return addTaskWithStartDelay(target, 0UL, velocity, acceleration, deceleration);
}

/*
 unsigned char CCDevice::addTaskWithStartDelay(float target, unsigned long startDelay) {
 return addTaskWithStartDelay(target, startDelay, defaultVelocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addTaskWithStartDelay(float target, unsigned long startDelay, float velocity) {
 return addTaskWithStartDelay(target, startDelay, velocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addTaskWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration) {
 return addTaskWithStartDelay(target, startDelay, velocity, acceleration, defaultDeceleration);
 }
 */
unsigned char CCDevice::addTaskWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration) {
//    task[countOfTasks] = new onEventTask(target, velocity, acceleration, deceleration, startDelay);
    task[countOfTasks] = new CCTask(target, velocity, acceleration, deceleration, startDelay);
    
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCDevice]: add Task for "));
        Serial.print(deviceName);
        Serial.print(F(", target: "));
        Serial.print(task[countOfTasks]->getTarget());
        Serial.print(F(", velocity: "));
        Serial.print(task[countOfTasks]->getVelocity());
        Serial.print(F(", acceleration: "));
        Serial.print(task[countOfTasks]->getAcceleration());
        Serial.print(F(", deceleration: "));
        Serial.print(task[countOfTasks]->getDeceleration());
        Serial.print(F(", startDelay: "));
        Serial.print(task[countOfTasks]->getStartDelay());
        Serial.print(F("; at $"));
        Serial.println((long)task[countOfTasks], HEX);
    }
    
    countOfTasks++;
    
    return countOfTasks - 1;
}

void CCDevice::deleteTasks() {
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


String CCDevice::getDeviceName(){return deviceName;}
deviceType CCDevice::getType(){return type;}
unsigned char CCDevice::getCountOfTasks(){return countOfTasks;}
unsigned char CCDevice::getTaskPointer(){return taskPointer;}
void CCDevice::setTaskPointer(unsigned char pointer){taskPointer = pointer;}
void CCDevice::increaseTaskPointer(){taskPointer++;}
float CCDevice::getCurrentPosition(){return currentPosition;}
void CCDevice::setCurrentPosition(float position){currentPosition = position;}
float CCDevice::getTarget(){return target;}
float CCDevice::getVelocity(){return velocity;}
float CCDevice::getAcceleration(){return acceleration;}
float CCDevice::getDeceleration(){return deceleration;}
bool CCDevice::getDirectionDown(){return directionDown;}
deviceState CCDevice::getState(){return state;}
void CCDevice::setState(deviceState state){this->state = state;}
unsigned long CCDevice::getStartDelay(){return startDelay;}
void CCDevice::setStartDelay(unsigned long delay){startDelay = delay;}
event CCDevice::getStartEvent(){return startEvent;}
void CCDevice::setStartEvent(event startEvent){this->startEvent = startEvent;}
event CCDevice::getStopEvent(){return stopEvent;}
void CCDevice::setStopEvent(event stopEvent){this->stopEvent = stopEvent;}
unsigned long CCDevice::getStartTime(){return startTime;}
void CCDevice::setStartTime(unsigned long startTime){this->startTime = startTime;};
unsigned long CCDevice::getTimeout(){return timeout;}
boolean CCDevice::getStartButton(){return startButton;}
boolean CCDevice::getStopButton(){return stopButton;}
boolean CCDevice::getStartButtonState(){return startButtonState;}
boolean CCDevice::getStopButtonState(){return stopButtonState;}
schedulerDevice CCDevice::getStartTriggerDevice(){return startTriggerDevice;}
schedulerDevice CCDevice::getStopTriggerDevice(){return stopTriggerDevice;}
scheduledTask CCDevice::getStartTriggerTask(){return startTriggerTask;}
scheduledTask CCDevice::getStopTriggerTask(){return stopTriggerTask;}
signed long CCDevice::getStartTriggerPosition(){return startTriggerPosition;}
signed long CCDevice::getStopTriggerPosition(){return stopTriggerPosition;}
stoppingMode CCDevice::getStopping(){return stopping;}
void CCDevice::setStopping(stoppingMode mode){stopping = mode;}
boolean CCDevice::getSwitchTaskPromptly(){return switchTaskPromptly;}
void CCDevice::setSwitchTaskPromptly(boolean switchPromptly){switchTaskPromptly = switchPromptly;}
unsigned char CCDevice::getSensor(){return sensor;}
signed int CCDevice::getInitiatePerformanceValue(){return initiatePerformanceValue;}
signed int CCDevice::getTargetValue(){return targetValue;}
float CCDevice::getStopPerformance(){return stopPerformance;}
approximationMode CCDevice::getApproximation(){return approximation;}


