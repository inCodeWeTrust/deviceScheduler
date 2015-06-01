//
//  CCSolenoidDevice.cpp
//  moveServos
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <avr/pgmspace.h>

#include "CCSolenoidDevice.h"




CCSolenoidDevice::CCSolenoidDevice(String deviceName, unsigned char solenoid_pin) : CCDevice() {
  this->deviceName = deviceName;
  this->solenoid_pin = solenoid_pin;

  type = SOLENOIDDEVICE;
  state = 0;
  movePointer = 0;
  countOfMoves = 0;

  attachDevice();

  if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_MEMORYDEBUG) {
    Serial.print(F("[CCSolenoidDevice]: setup solenoidDevice "));
    Serial.print(deviceName);
    Serial.print(F(" at $"));
    Serial.println((long)this, HEX);
  }
}

CCSolenoidDevice::~CCSolenoidDevice() {
  deleteMoves();
  detachDevice();
}

void CCSolenoidDevice::enableDevice() {}
void CCSolenoidDevice::disableDevice() {}

void CCSolenoidDevice::attachDevice() {
  pinMode(solenoid_pin, OUTPUT);
}
void CCSolenoidDevice::detachDevice() {
  pinMode(solenoid_pin, INPUT);
}



void CCSolenoidDevice::prepareNextMove() {

  target = theMove[movePointer]->target;
  velocity = theMove[movePointer]->velocity;
  //  acceleration = theMove[movePointer]->acceleration;

  startEvent = theMove[movePointer]->startEvent;
  stopEvent = theMove[movePointer]->stopEvent;
  startDelay = theMove[movePointer]->startDelay;
  startTime = theMove[movePointer]->startTime;
  timeout = theMove[movePointer]->timeout;
  startButton = theMove[movePointer]->startButton;
  stopButton = theMove[movePointer]->stopButton;
  startButtonState = theMove[movePointer]->startButtonState;
  stopButtonState = theMove[movePointer]->stopButtonState;
  startTriggerDevice = theMove[movePointer]->startTriggerDevice;
  startTriggerMove = theMove[movePointer]->startTriggerMove;
  startTriggerPosition = theMove[movePointer]->startTriggerPosition;
  stopTriggerDevice = theMove[movePointer]->stopTriggerDevice;
  stopTriggerMove = theMove[movePointer]->stopTriggerMove;
  stopTriggerPosition = theMove[movePointer]->stopTriggerPosition;
  stopSharply = theMove[movePointer]->stopSharply;


  target = min(16, fabs(target));

  if (velocity == 0) {
    switchingInterval = 0xFFFE;
    onDuration = 0xFFFF;
  }
  else {
    // minimum onDuration = 1ms, having 16 dutyCycles mninimum switchOnTime = 16ms
    onDuration = max(1000 / (velocity * 16), 1);
    switchingInterval = onDuration << 4;
    onDuration *= target;
  }

  if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_MEMORYDEBUG) {
    Serial.print(F("[CCSolenoidDevice]: "));
    Serial.print(deviceName);
    Serial.print(F(": power at $"));
    Serial.print((long)&target, HEX);
    Serial.print(F(", frequency at $"));
    Serial.println((long)&velocity, HEX);
  }
  if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_CALCULATIONDEBUG) {
    Serial.print(F("[CCSolenoidDevice]: "));
    Serial.print(deviceName);
    Serial.print(F(": prepare move "));
    Serial.print(movePointer);
    Serial.print(F(": power (1...16): "));
    Serial.print(target);
    Serial.print(F(", frequency (n/sec): "));
    Serial.print(velocity);
    Serial.print(F(", switchingInterval: "));
    Serial.print(switchingInterval);
    Serial.print(F(", onDuration: "));
    Serial.println(onDuration);
  }
}

void CCSolenoidDevice::startMove() {
  state |= MOVING;
  t0 = millis();

  switchOnTime = 0;
  switchOffTime = switchOnTime + onDuration;
  isActiv = false;

  if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_BASICOUTPUT) {
    Serial.print(F("[CCSolenoidDevice]: "));
    Serial.print(deviceName);
    Serial.print(F(", start move: "));
    Serial.print((int)movePointer);
    Serial.print(F(", switchOnTime: "));
    Serial.print(switchOnTime);
    Serial.print(F(", switchOffTime: "));
    Serial.println(switchOffTime);
  }
}

void CCSolenoidDevice::initiateStop() {
  digitalWrite(solenoid_pin, SOLENOID_OFF);
  stopMoving();
}

void CCSolenoidDevice::switchToNextMove() {}

void CCSolenoidDevice::stopMoving() {
  digitalWrite(solenoid_pin, SOLENOID_OFF);
  state &= ~MOVING;
  state |= MOVE_DONE;
}

void CCSolenoidDevice::finishMove() {
  state &= ~MOVING & ~MOVE_DONE;

  disableDevice();

  if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_BASICOUTPUT) {
    Serial.print(F("[CCSolenoidDevice]: "));
    Serial.print(deviceName);
    Serial.print(F(": stop: move "));
    Serial.println((int)movePointer);
  }
}

void CCSolenoidDevice::driveDynamic() {
  elapsedTime = millis() - t0;

  if (elapsedTime > switchOnTime) {
    digitalWrite(solenoid_pin, SOLENOID_ON);
    switchOffTime = switchOnTime + onDuration;
    switchOnTime += switchingInterval;
    isActiv = true;

    if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_MOVEMENTDEBUG) {
      Serial.print(elapsedTime);
      Serial.print(F(": switch on, switch off at "));
      Serial.print(switchOffTime);
      Serial.print(F(", next cycle at "));
      Serial.println(switchOnTime);
    }
  }
  if (isActiv) {
    if (elapsedTime > switchOffTime) {
      digitalWrite(solenoid_pin, SOLENOID_OFF);
      isActiv = false;
    }
  }
}







