//
//  CCDevice.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include "CCDevice.h"



CCDevice::CCDevice(const String deviceName, const deviceType type) : deviceName(deviceName), type(type), currentTaskID(0), currentPosition(0), directionDown(false), state(SLEEPING), target(0), velocity(0), acceleration(0), deceleration(0), moveRelativ(false), positionReset(NO_RESET), stopping(STOP_NORMAL), switching(NO_SWITCHING), approximationTarget(0), approximationControl(NULL), initiatePerformanceValue(0), stopPerformance(0), approximationCurve(0), gap(0), reversedApproximation(false), approximation(SKIP_APPROXIMATION_MODERATE), targetReachedCounter(0) {


    this->verbosity = NO_OUTPUT;
}

CCDevice::~CCDevice() {}



const String CCDevice::getName() const {return deviceName;}
deviceType CCDevice::getType() const {return (deviceType)type;}

unsigned int CCDevice::getCurrentTaskID(){return currentTaskID;}
void CCDevice::setCurrentTaskID(unsigned int taskID) {currentTaskID = taskID;}
float CCDevice::getCurrentPosition(){return currentPosition;}
void CCDevice::setCurrentPosition(float position){currentPosition = position;}
float CCDevice::getTarget(){return target;}
float CCDevice::getVelocity(){return velocity;}
float CCDevice::getAcceleration(){return acceleration;}
float CCDevice::getDeceleration(){return deceleration;}
bool CCDevice::getDirectionDown(){return directionDown;}
deviceState CCDevice::getState(){return state;}
void CCDevice::setState(deviceState state){this->state = state;}
stoppingMode CCDevice::getStopping(){return stopping;}
void CCDevice::setStopping(stoppingMode mode){stopping = mode;}
switchingMode CCDevice::getSwitching(){return switching;}
void CCDevice::setSwitching(switchingMode switchPromptly){switching = switchPromptly;}
signed int CCDevice::getInitiatePerformanceValue(){return initiatePerformanceValue;}
float CCDevice::getStopPerformance(){return stopPerformance;}
unsigned int CCDevice::getApproximationCurve(){return approximationCurve;}
unsigned int CCDevice::getGap(){return gap;}
bool CCDevice::getReversedApproximation(){return reversedApproximation;}
approximationMode CCDevice::getApproximation(){return approximation;}

void CCDevice::increaseTargetReachedCounter() {targetReachedCounter++;}
void CCDevice::decreaseTargetReachedCounter() {targetReachedCounter--;}



void CCDevice::setVerbosity(int verbosity) {this->verbosity = verbosity;}

