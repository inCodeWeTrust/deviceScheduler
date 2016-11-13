//
//  CCTask.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 07.01.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCTask.h"


CCTask::CCTask(float target, float velocity, float acceleration, float deceleration, bool moveRelativ, positionResetMode positionReset, unsigned int taskID) {

    this->taskID = taskID;
    
    this->target = target;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->deceleration = deceleration;
    this->moveRelativ = moveRelativ;
    this->positionReset = positionReset;
    this->startDelay = 0;
    this->startEvent = NONE;
    this->stopEvent = NONE;
    this->switchTaskPromptly = NO_SWITCHING;
    this->startTime = 0;
    this->timeout = 0;
    this->startControl = 0;
    this->stopControl = 0;
    this->startTriggerDevice = NULL;
    this->stopTriggerDevice = NULL;
    this->startTriggerTaskID = 0;
    this->stopTriggerTaskID = 0;
    this->startTriggerPosition = 0;
    this->stopTriggerPosition = 0;
    this->stopping = STOP_NORMAL;
    this->sensor = 0;
    this->initiatePerformanceValue = 0;
    this->targetValue = 0;
    this->stopPerformance = 0;
    this->approximationCurve = 0;
    this->gap = 0;
    this->reversedApproximation = false;

    if (TASK_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCTask]: setup task "));
        Serial.print((int)taskID);
        if (TASK_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.print((long)this, HEX);
        }
        Serial.println();
    }

}

void CCTask::setStartDelay(unsigned long startDelay) {
    this->startDelay = startDelay;
}

void CCTask::startByDate(unsigned long startTime) {
    this->startEvent = DATE;
    this->startTime = startTime;
}
void CCTask::startByControl(CCControl* startControl, comparingMode comparing, int target) {
    this->startEvent = CONTROL;
    this->startControl = startControl;
    this->startControlComparing = comparing;
    this->startControlTarget = target;
}
void CCTask::startAfterCompletion() {
    this->startEvent = FOLLOW_ME;
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
    this->switchTaskPromptly = SWITCH_PROMPTLY;
}
void CCTask::switchToNextTaskByControl(CCControl* switchingControl, comparingMode comparing, int target) {
    this->stopEvent = CONTROL;
    this->stopControl = switchingControl;
    this->stopControlComparing = comparing;
    this->stopControlTarget = target;
    this->switchTaskPromptly = SWITCH_PROMPTLY;
}
void CCTask::switchToNextTaskAfterCompletionOf(CCDevice* switchingTriggerDevice, CCTask* switchingTriggerTask) {
    this->stopEvent = FOLLOW;
    this->stopTriggerDevice = switchingTriggerDevice;
    this->stopTriggerTaskID = switchingTriggerTask->taskID;
    this->switchTaskPromptly = SWITCH_PROMPTLY;
}
void CCTask::switchToNextTaskAtPosition(signed long switchingTriggerPosition) {
    this->stopEvent = MY_POSITION;
    this->stopTriggerPosition = switchingTriggerPosition;
    this->switchTaskPromptly = SWITCH_PROMPTLY;
}
void CCTask::switchToNextTaskByTriggerpositionOf(CCDevice* switchingTriggerDevice, CCTask* switchingTriggerTask, signed long switchingTriggerPosition) {
    this->stopEvent = POSITION;
    this->stopTriggerDevice = switchingTriggerDevice;
    this->stopTriggerTaskID = switchingTriggerTask->taskID;
    this->stopTriggerPosition = switchingTriggerPosition;
    this->switchTaskPromptly = SWITCH_PROMPTLY;
}

void CCTask::stopByTimeout(unsigned long timeout, stoppingMode stopping) {
    this->stopEvent = DATE;
    this->timeout = timeout;
    this->stopping = stopping;
}
void CCTask::stopByControl(CCControl* stopControl, comparingMode comparing, int target, stoppingMode stopping) {
    this->stopEvent = CONTROL;
    this->stopControl = stopControl;
    this->stopControlComparing = comparing;
    this->stopControlTarget = target;
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
void CCTask::stopAtPosition(signed long stopTriggerPosition, stoppingMode stopping) {
    this->stopEvent = MY_POSITION;
    this->stopTriggerPosition = stopTriggerPosition;
    this->stopping = stopping;
}


void CCTask::stopDynamicallyBySensor(unsigned int sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, approximationMode approximation) {
    this->stopping = STOP_DYNAMIC;
    this->sensor = sensor;
    this->initiatePerformanceValue = initiatePerformanceValue;
    this->targetValue = targetValue;
    this->stopPerformance = stopPerformance;
    this->approximation = approximation;
}

void CCTask::stopDynamicallyBySensor_new(unsigned int sensor, unsigned int targetValue, float approximationCurve, float gap, approximationMode approximation) {
    this->stopping = STOP_DYNAMIC;
    this->sensor = sensor;
    this->targetValue = targetValue;
    this->approximationCurve = approximationCurve;
    this->gap = gap;
    this->approximation = approximation;
}


unsigned int CCTask::getTaskID(){return taskID;}
float CCTask::getTarget(){return target;}
void CCTask::setTarget(float target){this->target = target;}
float CCTask::getVelocity(){return velocity;}
void CCTask::setVelocity(float velocity){this->velocity = velocity;}
float CCTask::getAcceleration(){return acceleration;}
void CCTask::setAcceleration(float acceleration){this->acceleration = acceleration;}
float CCTask::getDeceleration(){return deceleration;}
void CCTask::setDeceleration(float deceleration){this->deceleration = deceleration;}
bool CCTask::getMoveRelativ() {return moveRelativ;}
positionResetMode CCTask::getPositionReset(){return positionReset;}
unsigned long CCTask::getStartDelay(){return startDelay;}
event CCTask::getStartEvent(){return startEvent;}
event CCTask::getStopEvent(){return stopEvent;}
unsigned long CCTask::getStartTime(){return startTime;}
unsigned long CCTask::getTimeout(){return timeout;}
CCControl* CCTask::getStartControl(){return startControl;}
CCControl* CCTask::getStopControl(){return stopControl;}
int CCTask::getStartControlTarget(){return startControlTarget;}
int CCTask::getStopControlTarget(){return stopControlTarget;}
comparingMode CCTask::getStartControlComparing(){return startControlComparing;}
comparingMode CCTask::getStopControlComparing(){return stopControlComparing;}
CCDevice* CCTask::getStartTriggerDevice(){return startTriggerDevice;}
CCDevice* CCTask::getStopTriggerDevice(){return stopTriggerDevice;}
unsigned int CCTask::getStartTriggerTaskID(){return startTriggerTaskID;}
unsigned int CCTask::getStopTriggerTaskID(){return stopTriggerTaskID;}
signed long CCTask::getStartTriggerPosition(){return startTriggerPosition;}
signed long CCTask::getStopTriggerPosition(){return stopTriggerPosition;}
stoppingMode CCTask::getStopping(){return stopping;}
switchingMode CCTask::getSwitchTaskPromptly(){return switchTaskPromptly;}
unsigned int CCTask::getSensor(){return sensor;}
signed int CCTask::getInitiatePerformanceValue(){return initiatePerformanceValue;}
signed int CCTask::getTargetValue(){return targetValue;}
float CCTask::getStopPerformance(){return stopPerformance;}
approximationMode CCTask::getApproximation(){return approximation;}
unsigned int CCTask::getApproximationCurve(){return approximationCurve;}
unsigned int CCTask::getGap(){return gap;}
bool CCTask::getReversedApproximation(){return reversedApproximation;}



