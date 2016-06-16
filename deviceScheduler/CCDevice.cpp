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
void CCDevice::defineDefaults(float defaultVelocity, float defaultAcceleration) {
    this->defaultVelocity = defaultVelocity;
    this->defaultAcceleration = defaultAcceleration;
    this->defaultDeceleration = defaultAcceleration;
}




String CCDevice::getDeviceName(){return deviceName;}
deviceType CCDevice::getType(){return type;}
unsigned char CCDevice::getCountOfTasks(){return countOfTasks;}
void CCDevice::setCountOfTasks(unsigned char count){countOfTasks = count;}
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
schedulerControlButton CCDevice::getStartButton(){return startButton;}
schedulerControlButton CCDevice::getStopButton(){return stopButton;}
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


