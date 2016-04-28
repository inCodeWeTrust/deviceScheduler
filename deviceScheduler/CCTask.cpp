//
//  CCTask.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 07.01.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCTask.h"
#include "deviceScheduler.h"





CCTask::CCTask(float target, float velocity, float acceleration, float deceleration, unsigned long startDelay) {

    this->target = target;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->deceleration = deceleration;
    this->startDelay = startDelay;
    this->startEvent = NONE;
    this->stopEvent = NONE;
    this->switchTaskPromptly = false;
    this->startTime = 0;
    this->timeout = 0;
    this->startButton = 0;
    this->stopButton = 0;
    this->startButtonState = 0;
    this->stopButtonState = 0;
    this->startTriggerDevice = 0;
    this->startTriggerTask = 0;
    this->startTriggerPosition = 0;
    this->stopTriggerDevice = 0;
    this->stopTriggerTask = 0;
    this->stopTriggerPosition = 0;
    this->stopping = STOP_NORMAL;
    this->sensor = 0;
    this->initiatePerformanceValue = 0;
    this->targetValue = 0;
    this->stopPerformance = 0;
 
}

void CCTask::startByDate(unsigned long startTime) {
    this->startEvent = DATE;
    this->startTime = startTime;
}
void CCTask::startByButton(unsigned char startButton, boolean startButtonState) {
    this->startEvent = BUTTON;
    this->startButton = startButton;
    this->startButtonState = startButtonState;
}
void CCTask::startAfterCompletionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask) {
    this->startEvent = FOLLOW;
    this->startTriggerDevice = startTriggerDevice;
    this->startTriggerTask = startTriggerTask;
}
void CCTask::startByTriggerpositionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask, signed long startTriggerPosition) {
    this->startEvent = POSITION;
    this->startTriggerDevice = startTriggerDevice;
    this->startTriggerTask = startTriggerTask;
    this->startTriggerPosition = startTriggerPosition;
}

void CCTask::switchToNextTaskByDate(unsigned long switchingTimeout) {
    this->stopEvent = DATE;
    this->timeout = switchingTimeout;
    this->switchTaskPromptly = true;
}
void CCTask::switchToNextTaskByButton(unsigned char switchingButton, boolean switchingButtonState) {
    this->stopEvent = BUTTON;
    this->stopButton = switchingButton;
    this->stopButtonState = switchingButtonState;
    this->switchTaskPromptly = true;
}
void CCTask::switchToNextTaskAfterCompletionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask) {
    this->stopEvent = FOLLOW;
    this->stopTriggerDevice = switchingTriggerDevice;
    this->stopTriggerTask = switchingTriggerTask;
    this->switchTaskPromptly = true;
}
void CCTask::switchToNextTaskByTriggerpositionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask, signed long switchingTriggerPosition) {
    this->stopEvent = POSITION;
    this->stopTriggerDevice = switchingTriggerDevice;
    this->stopTriggerTask = switchingTriggerTask;
    this->stopTriggerPosition = switchingTriggerPosition;
    this->switchTaskPromptly = true;
}

void CCTask::stopByTimeout(unsigned long timeout, stoppingMode stopping) {
    this->stopEvent = DATE;
    this->timeout = timeout;
    this->stopping = stopping;
}
void CCTask::stopByButton(unsigned char stopButton, boolean stopButtonState, stoppingMode stopping) {
    this->stopEvent = BUTTON;
    this->stopButton = stopButton;
    this->stopButtonState = stopButtonState;
    this->stopping = stopping;
}
void CCTask::stopAfterCompletionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, stoppingMode stopping) {
    this->stopEvent = FOLLOW;
    this->stopTriggerDevice = stopTriggerDevice;
    this->stopTriggerTask = stopTriggerTask;
    this->stopping = stopping;
}
void CCTask::stopByTriggerpositionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping) {
    this->stopEvent = POSITION;
    this->stopTriggerDevice = stopTriggerDevice;
    this->stopTriggerTask = stopTriggerTask;
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

