//
//  CCSolenoidDevice.cpp
//  moveServos
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <avr/pgmspace.h>
#include "CCSolenoidDevice.h"




CCSolenoidDevice::CCSolenoidDevice(unsigned int deviceIndex, String deviceName, unsigned char solenoid_pin) : CCDevice() {
    this->deviceIndex = deviceIndex;
    this->deviceName = deviceName;
  this->solenoid_pin = solenoid_pin;

  type = SOLENOIDDEVICE;
    state = SLEEPING;
    taskPointer = 0;
    countOfTasks = 0;

  attachDevice();

  if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_MEMORYDEBUG) {
    Serial.print(F("[CCSolenoidDevice]: setup solenoidDevice "));
    Serial.print(deviceName);
    Serial.print(F(" at $"));
    Serial.println((long)this, HEX);
  }
}

CCSolenoidDevice::~CCSolenoidDevice() {
    deleteTasks();
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



void CCSolenoidDevice::prepareNextTask() {

    target = task[taskPointer]->target;
    velocity = task[taskPointer]->velocity;
    acceleration = task[taskPointer]->acceleration;
    deceleration = task[taskPointer]->deceleration;
 
    startEvent = task[taskPointer]->startEvent;
    stopEvent = task[taskPointer]->stopEvent;
    switchTaskPromptly = task[taskPointer]->switchTaskPromptly;
    startDelay = task[taskPointer]->startDelay;
    startTime = task[taskPointer]->startTime;
    timeout = task[taskPointer]->timeout;
    startButton = task[taskPointer]->startButton;
    stopButton = task[taskPointer]->stopButton;
    startButtonState = task[taskPointer]->startButtonState;
    stopButtonState = task[taskPointer]->stopButtonState;
    startTriggerDevice = task[taskPointer]->startTriggerDevice;
    startTriggerTask = task[taskPointer]->startTriggerTask;
    startTriggerPosition = task[taskPointer]->startTriggerPosition;
    stopTriggerDevice = task[taskPointer]->stopTriggerDevice;
    stopTriggerTask = task[taskPointer]->stopTriggerTask;
    stopTriggerPosition = task[taskPointer]->stopTriggerPosition;
    stopping = task[taskPointer]->stopping;


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
    Serial.print(taskPointer);
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

void CCSolenoidDevice::startTask() {
    if (false) {     // values valid?
        stopTask();
    }
    else {
        enableDevice();                                             // setup ENABLE-pin of stepper driver
        
        state = MOVING;                                             // tag device as MOVING
        t0 = millis();
        
        switchOnTime = 0;
        switchOffTime = switchOnTime + onDuration;
        isActiv = false;
        
        if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_BASICOUTPUT) {
            Serial.print(F("[CCSolenoidDevice]: "));
            Serial.print(deviceName);
            Serial.print(F(", start move: "));
            Serial.print((int)taskPointer);
            Serial.print(F(", switchOnTime: "));
            Serial.print(switchOnTime);
            Serial.print(F(", switchOffTime: "));
            Serial.println(switchOffTime);
        }
    }
}

void CCSolenoidDevice::initiateStop() {
  digitalWrite(solenoid_pin, SOLENOID_OFF);
  stopTask();
}


void CCSolenoidDevice::stopTask() {
  digitalWrite(solenoid_pin, SOLENOID_OFF);
  state = MOVE_DONE;
}

void CCSolenoidDevice::finishTask() {
    state = SLEEPING;

  disableDevice();

  if (CCSolenoidDevice_VERBOSE & CCSolenoidDevice_BASICOUTPUT) {
    Serial.print(F("[CCSolenoidDevice]: "));
    Serial.print(deviceName);
    Serial.print(F(": stop: move "));
    Serial.println((int)taskPointer);
  }
}

void CCSolenoidDevice::operateTask() {
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







