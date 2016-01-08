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
    this->switchMovePromptly = false;
    this->startTime = 0;
    this->timeout = 0;
    this->startButton = 0;
    this->stopButton = 0;
    this->startButtonState = 0;
    this->stopButtonState = 0;
    this->startTriggerDevice = 0;
    this->startTriggerMove = 0;
    this->startTriggerPosition = 0;
    this->stopTriggerDevice = 0;
    this->stopTriggerMove = 0;
    this->stopTriggerPosition = 0;
    this->stopSharply = 0;
    this->stopDynamically = 0;
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
void CCTask::startAfterMy(unsigned char startTriggerMove) {
    this->startEvent = FOLLOW;
    this->startTriggerMove = startTriggerMove;
}
void CCTask::startAfterCompletionOf(unsigned char startTriggerDevice, unsigned char startTriggerMove) {
    this->startEvent = FOLLOW;
    this->startTriggerDevice = startTriggerDevice;
    this->startTriggerMove = startTriggerMove;
}
void CCTask::startByTriggerpositionOf(unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition) {
    this->startEvent = POSITION;
    this->startTriggerDevice = startTriggerDevice;
    this->startTriggerMove = startTriggerMove;
    this->startTriggerPosition = startTriggerPosition;
}

void CCTask::switchToNextTaskByDate(unsigned long switchingTimeout) {
    this->stopEvent = DATE;
    this->timeout = switchingTimeout;
    this->switchMovePromptly = true;
}
void CCTask::switchToNextTaskByButton(unsigned char switchingButton, boolean switchingButtonState) {
    this->stopEvent = BUTTON;
    this->stopButton = switchingButton;
    this->stopButtonState = switchingButtonState;
    this->switchMovePromptly = true;
}
void CCTask::switchToNextTaskAfterCompletionOf(unsigned char switchingTriggerDevice, unsigned char switchingTriggerMove) {
    this->stopEvent = FOLLOW;
    this->stopTriggerDevice = switchingTriggerDevice;
    this->stopTriggerMove = switchingTriggerMove;
    this->switchMovePromptly = true;
}
void CCTask::switchToNextTaskByTriggerpositionOf(unsigned char switchingTriggerDevice, unsigned char switchingTriggerMove, signed long switchingTriggerPosition) {
    this->stopEvent = POSITION;
    this->stopTriggerDevice = switchingTriggerDevice;
    this->stopTriggerMove = switchingTriggerMove;
    this->stopTriggerPosition = switchingTriggerPosition;
    this->switchMovePromptly = true;
}

void CCTask::stopByTimeout(unsigned long timeout, boolean stopSharply) {
    this->stopEvent = DATE;
    this->timeout = timeout;
    this->stopSharply = stopSharply;
}
void CCTask::stopByButton(unsigned char stopButton, boolean stopButtonState, boolean stopSharply) {
    this->stopEvent = BUTTON;
    this->stopButton = stopButton;
    this->stopButtonState = stopButtonState;
    this->stopSharply = stopSharply;
}
void CCTask::stopAfterCompletionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerMove) {
    this->stopEvent = FOLLOW;
    this->stopTriggerDevice = stopTriggerDevice;
    this->stopTriggerMove = stopTriggerMove;
}
void CCTask::stopByTriggerpositionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply) {
    this->stopEvent = POSITION;
    this->stopTriggerDevice = stopTriggerDevice;
    this->stopTriggerMove = stopTriggerMove;
    this->stopTriggerPosition = stopTriggerPosition;
    this->stopSharply = stopSharply;
}
void CCTask::stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, unsigned char stopMode) {
    this->stopDynamically = true;
    this->sensor = sensor;
    this->initiatePerformanceValue = initiatePerformanceValue;
    this->targetValue = targetValue;
    this->stopPerformance = stopPerformance;
    this->stopMode = stopMode;
}

