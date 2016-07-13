//
//  CCTask.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 07.01.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCTask.h"


CCTask::CCTask(float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, boolean withPositionReset, unsigned char taskID) {

    this->taskID = taskID;
    
    this->target = target;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->deceleration = deceleration;
    this->moveRelativ = moveRelativ;
    this->withPositionReset = withPositionReset;
    this->startDelay = 0;
    this->startEvent = NONE;
    this->stopEvent = NONE;
    this->switchTaskPromptly = false;
    this->startTime = 0;
    this->timeout = 0;
    this->startButton = 0;
    this->stopButton = 0;
    this->startTriggerDevice = NULL;
    this->startTriggerTaskID = 0;
    this->startTriggerPosition = 0;
    this->stopTriggerDevice = NULL;
    this->stopTriggerTaskID = 0;
    this->stopTriggerPosition = 0;
    this->stopping = STOP_NORMAL;
    this->sensor = 0;
    this->initiatePerformanceValue = 0;
    this->targetValue = 0;
    this->stopPerformance = 0;
    this->approximationCurve = 0;
    this->gap = 0;
    this->reversedApproximation = false;

}

void CCTask::setStartDelay(unsigned long startDelay) {
    this->startDelay = startDelay;
}

void CCTask::startByDate(unsigned long startTime) {
    this->startEvent = DATE;
    this->startTime = startTime;
}
void CCTask::startByButton(CCControlButton* startButton) {
    this->startEvent = BUTTON;
    this->startButton = startButton;
}
void CCTask::startAfterCompletionOf(CCDevice* startTriggerDevice, CCTask* startTriggerTask) {
    this->startEvent = FOLLOW;
    this->startTriggerDevice = startTriggerDevice;
    this->startTriggerTaskID = startTriggerTask->taskID;
}
void CCTask::startByTriggerpositionOf(CCDevice* startTriggerDevice, CCTask* startTriggerTask, signed long startTriggerPosition) {
    this->startEvent = POSITION;
    this->startTriggerDevice = startTriggerDevice;
    this->startTriggerTaskID = startTriggerTask->taskID;
    this->startTriggerPosition = startTriggerPosition;
}

void CCTask::switchToNextTaskByDate(unsigned long switchingTimeout) {
    this->stopEvent = DATE;
    this->timeout = switchingTimeout;
    this->switchTaskPromptly = true;
}
void CCTask::switchToNextTaskByButton(CCControlButton* switchingButton) {
    this->stopEvent = BUTTON;
    this->stopButton = switchingButton;
    this->switchTaskPromptly = true;
}
void CCTask::switchToNextTaskAfterCompletionOf(CCDevice* switchingTriggerDevice, CCTask* switchingTriggerTask) {
    this->stopEvent = FOLLOW;
    this->stopTriggerDevice = switchingTriggerDevice;
    this->stopTriggerTaskID = switchingTriggerTask->taskID;
    this->switchTaskPromptly = true;
}
void CCTask::switchToNextTaskByTriggerpositionOf(CCDevice* switchingTriggerDevice, CCTask* switchingTriggerTask, signed long switchingTriggerPosition) {
    this->stopEvent = POSITION;
    this->stopTriggerDevice = switchingTriggerDevice;
    this->stopTriggerTaskID = switchingTriggerTask->taskID;
    this->stopTriggerPosition = switchingTriggerPosition;
    this->switchTaskPromptly = true;
}

void CCTask::stopByTimeout(unsigned long timeout, stoppingMode stopping) {
    this->stopEvent = DATE;
    this->timeout = timeout;
    this->stopping = stopping;
}
void CCTask::stopByButton(CCControlButton* stopButton, stoppingMode stopping) {
    this->stopEvent = BUTTON;
    this->stopButton = stopButton;
    this->stopping = stopping;
}
void CCTask::stopAfterCompletionOf(CCDevice* stopTriggerDevice, CCTask* stopTriggerTask, stoppingMode stopping) {
    this->stopEvent = FOLLOW;
    this->stopTriggerDevice = stopTriggerDevice;
    this->stopTriggerTaskID = stopTriggerTask->taskID;
    this->stopping = stopping;
}
void CCTask::stopByTriggerpositionOf(CCDevice* stopTriggerDevice, CCTask* stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping) {
    this->stopEvent = POSITION;
    this->stopTriggerDevice = stopTriggerDevice;
    this->stopTriggerTaskID = stopTriggerTask->taskID;
    this->stopTriggerPosition = stopTriggerPosition;
    this->stopping = stopping;
}
void CCTask::stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, approximationMode approximation) {
    this->stopping = STOP_DYNAMIC;
    this->sensor = sensor;
    this->initiatePerformanceValue = initiatePerformanceValue;
    this->targetValue = targetValue;
    this->stopPerformance = stopPerformance;
    this->approximation = approximation;
}

void CCTask::stopDynamicallyBySensor_new(unsigned char sensor, unsigned int targetValue, float approximationCurve, float gap, approximationMode approximation) {
    this->stopping = STOP_DYNAMIC;
    this->sensor = sensor;
    this->targetValue = targetValue;
    this->approximationCurve = approximationCurve;
    this->gap = gap;
    this->approximation = approximation;
}


float CCTask::getTarget(){return target;}
void CCTask::setTarget(float target){this->target = target;}
float CCTask::getVelocity(){return velocity;}
void CCTask::setVelocity(float velocity){this->velocity = velocity;}
float CCTask::getAcceleration(){return acceleration;}
void CCTask::setAcceleration(float acceleration){this->acceleration = acceleration;}
float CCTask::getDeceleration(){return deceleration;}
void CCTask::setDeceleration(float deceleration){this->deceleration = deceleration;}
boolean CCTask::getMoveRelativ() {return moveRelativ;}
boolean CCTask::getWithPositionReset(){return withPositionReset;}
unsigned long CCTask::getStartDelay(){return startDelay;}
event CCTask::getStartEvent(){return startEvent;}
event CCTask::getStopEvent(){return stopEvent;}
unsigned long CCTask::getStartTime(){return startTime;}
unsigned long CCTask::getTimeout(){return timeout;}
CCControlButton* CCTask::getStartButton(){return startButton;}
CCControlButton* CCTask::getStopButton(){return stopButton;}

CCDevice* CCTask::getStartTriggerDevice(){return startTriggerDevice;}
CCDevice* CCTask::getStopTriggerDevice(){return stopTriggerDevice;}
scheduledTask CCTask::getStartTriggerTaskID(){return startTriggerTaskID;}
scheduledTask CCTask::getStopTriggerTaskID(){return stopTriggerTaskID;}

signed long CCTask::getStartTriggerPosition(){return startTriggerPosition;}
signed long CCTask::getStopTriggerPosition(){return stopTriggerPosition;}
stoppingMode CCTask::getStopping(){return stopping;}
boolean CCTask::getSwitchTaskPromptly(){return switchTaskPromptly;}
unsigned char CCTask::getSensor(){return sensor;}
signed int CCTask::getInitiatePerformanceValue(){return initiatePerformanceValue;}
signed int CCTask::getTargetValue(){return targetValue;}
float CCTask::getStopPerformance(){return stopPerformance;}
unsigned int CCTask::getApproximationCurve(){return approximationCurve;}
unsigned int CCTask::getGap(){return gap;}
boolean CCTask::getReversedApproximation(){return reversedApproximation;}
approximationMode CCTask::getApproximation(){return approximation;}



