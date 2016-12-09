//
//  CCControl.cpp
//  ArduinoSimulator
//
//  Created by Little Abakus on 12.10.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//

#include "CCControl.h"



CCControl::~CCControl() {}


int CCControl::value() {return sensorValue;}


bool CCControl::isGreaterThen(int minimum) {
    return sensorValue > minimum;
}
bool CCControl::isSmallerThen(int maximum) {
    return sensorValue < maximum;
}
bool CCControl::isAbout(int fix, int tolerance) {
    return fabs(sensorValue - fix) < tolerance;
}
bool CCControl::is(int fix) {
    return sensorValue == fix;
}
bool CCControl::isNot(int fix) {
    return !(sensorValue == fix);
}









bool CCControl::getDigitalValue() {return digitalRead(pin);}
int CCControl::getAnalogValue() {return analogRead(pin);}
String CCControl::getName() {return controlName;}
unsigned int CCControl::getCountOfActions() {return countOfActions;}

void CCControl::setVerbosity(int verbosity) {this->verbosity = verbosity;}

