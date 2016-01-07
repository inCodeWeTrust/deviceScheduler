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
    this->startEvent = 0;
    this->stopEvent = 0;
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
    this->stopValue = 0;
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
//void CCTask::startAfterCompletion(unsigned char moveIndex) {
//    this->startEvent |= FOLLOW;
//    this->startTriggerDevice = deviceIndex;
//    this->startTriggerMove = moveIndex;
//}
void CCTask::startAfterCompletionOf(unsigned char startTriggerDevice, unsigned char startTriggerMove) {
    this->startEvent |= FOLLOW;
    this->startTriggerDevice = startTriggerDevice;
    this->startTriggerMove = startTriggerMove;
}
void CCTask::startByTriggerposition(unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition) {
    this->startEvent = POSITION;
    this->startTriggerDevice = startTriggerDevice;
    this->startTriggerMove = startTriggerMove;
    this->startTriggerPosition = startTriggerPosition;
}

void CCTask::switchByDate(unsigned long timeout) {
    this->stopEvent = DATE | SWITCH;
    this->timeout = timeout;
}
void CCTask::switchByButton(unsigned char stopButton, boolean stopButtonState) {
    this->stopEvent = BUTTON | SWITCH;
    this->stopButton = stopButton;
    this->stopButtonState = stopButtonState;
}
void CCTask::switchByTriggerposition(unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition) {
    this->stopEvent = POSITION | SWITCH;
    this->stopTriggerDevice = stopTriggerDevice;
    this->stopTriggerMove = stopTriggerMove;
    this->stopTriggerPosition = stopTriggerPosition;
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
void CCTask::stopByTriggerposition(unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply) {
    this->stopEvent = POSITION;
    this->stopTriggerDevice = stopTriggerDevice;
    this->stopTriggerMove = stopTriggerMove;
    this->stopTriggerPosition = stopTriggerPosition;
    this->stopSharply = stopSharply;
}
void CCTask::stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int stopValue, float stopPerformance, unsigned char stopMode) {
    this->stopDynamically = true;
    this->sensor = sensor;
    this->initiatePerformanceValue = initiatePerformanceValue;
    this->stopValue = stopValue;
    this->stopPerformance = stopPerformance;
    this->stopMode = stopMode;
}

