//
//  CCDeviceScheduler.cpp
//  CCDeviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//



#include "CCDeviceScheduler.h"


String getNameOfDeviceType(deviceType t);
String getNameOfMoveEvent(event e);
String getNameOfState(deviceState s);
String formatNumber(long theData, unsigned char len);



CCDeviceScheduler::CCDeviceScheduler() {
    countOfDevices = 0;
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: CCDeviceScheduler constructed at $"));
        Serial.println((long)this, HEX);
    }
}
CCDeviceScheduler::~CCDeviceScheduler() {
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.println(F("[CCDeviceScheduler]: destruct CCDeviceScheduler"));
    }
    
    for (int i = countOfDevices - 1; i >= 0; i--) {
        delete device[i];
        device[i] = NULL;
    }
}

unsigned char CCDeviceScheduler::addSwitch(String deviceName, unsigned char switching_pin, boolean defaultState) {
    
    device[countOfDevices] = new CCSwitchDevice(countOfDevices, deviceName, switching_pin, defaultState);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->type));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->deviceName);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}

unsigned char CCDeviceScheduler::addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition) {
    
    device[countOfDevices] = new CCServoDevice(countOfDevices, deviceName, servo_pin, minPosition, maxPosition, parkPosition);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->type));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->deviceName);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}

unsigned char CCDeviceScheduler::addStepper(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, unsigned int stepsPerRotation) {
    
    unsigned char numberOfMicroStepPins = 0;                                                    // first find how many pins are given
    signed char elementBegin = 1;
    signed char elementEnd = microStepPinsString.indexOf(',', elementBegin);                    // examine comma seperated list (String)
    while (elementBegin > 0) {
        numberOfMicroStepPins++;
        elementBegin = elementEnd + 1;
        elementEnd = microStepPinsString.indexOf(',', elementBegin);
    }
    
    unsigned char *microStepPin = new unsigned char[numberOfMicroStepPins];                     // prepare an array for the pins
    
    elementBegin = 0;
    for (unsigned char pinIndex = 0; pinIndex < numberOfMicroStepPins; pinIndex++) {            // once again examine the list (String)
        elementEnd = microStepPinsString.indexOf(',', elementBegin);
        
        String theElement = microStepPinsString.substring(elementBegin, elementEnd);
        theElement.trim();
        
        if (theElement.indexOf('x') > 0 || theElement.indexOf('X') > 0) {                       // is it a hex-value (0x...)? then use strtol
            char elementBuffer[theElement.length() + 1];                                        // propare a buffer for a charArray
            theElement.toCharArray(elementBuffer, sizeof(elementBuffer));                       // and convert String to charArray
            
            microStepPin[pinIndex] = strtol(elementBuffer, NULL, 16);                           // then convert to int (long)
        }
        else                                                                                    // else use .toInt()
        {
            microStepPin[pinIndex] = microStepPinsString.substring(elementBegin, elementEnd).toInt();
        }
        elementBegin = elementEnd + 1;
    }
    
    
    unsigned char *stepModeCode = new unsigned char[highestSteppingMode + 1];                   // prepare array for stepModeCodes
    
    elementBegin = 0;
    for (unsigned char codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
        elementEnd = stepModeCodesString.indexOf(',', elementBegin);                            // examine comma seperated list (String)
        
        String theElement = stepModeCodesString.substring(elementBegin, elementEnd);
        theElement.trim();
        
        if (theElement.indexOf('x') > 0 || theElement.indexOf('X') > 0) {                       // is it a hex-value (0x...)? then use strtol
            char elementBuffer[theElement.length() + 1];                                        // propare a buffer for a charArray
            theElement.toCharArray(elementBuffer, sizeof(elementBuffer));                       // and convert String to charArray
            
            stepModeCode[codeIndex] = strtol(elementBuffer, NULL, 16);                          // then convert to int (long)
        }
        else                                                                                    // else use .toInt()
        {
            stepModeCode[codeIndex] = stepModeCodesString.substring(elementBegin, elementEnd).toInt();
        }
        elementBegin = elementEnd + 1;
    }
    
    device[countOfDevices] = new CCStepperDevice(countOfDevices, deviceName, dir_pin, step_pin, enable_pin, highestSteppingMode, stepModeCode, numberOfMicroStepPins, microStepPin, stepsPerRotation);
    
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->type));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->deviceName);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}


void CCDeviceScheduler::getAllDevices() {
    Serial.println(F("[CCDeviceScheduler]: My Devices: "));
    for (int i = 0; i < countOfDevices; i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(", name: "));
        Serial.print(device[i]->deviceName);
        Serial.print(F(", type: "));
        Serial.print(getNameOfDeviceType(device[i]->type));
        Serial.print(F(", moves: "));
        Serial.println(device[i]->countOfTasks);
    }
    Serial.println();
}

void CCDeviceScheduler::getAllMoves() {
    for (int i = 0; i < countOfDevices; i++) {
        getMovesForDevice(i);
    }
}


void CCDeviceScheduler::getMovesForDevice(unsigned char theDevice) {
    Serial.print(F("[CCDeviceScheduler]: Moves of Device "));
    Serial.print(device[theDevice]->deviceName);
    Serial.println(F(": "));
    for (int i = 0; i < device[theDevice]->countOfTasks; i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(": target: "));
        Serial.print(device[theDevice]->task[i]->target);
        Serial.print(F(", velocity: "));
        Serial.print(device[theDevice]->task[i]->velocity);
        Serial.print(F(", acceleration: "));
        Serial.print(device[theDevice]->task[i]->acceleration);
        Serial.print(F(", startDelay: "));
        Serial.print(device[theDevice]->task[i]->startDelay);
        Serial.print(F(", started by "));
        Serial.print(getNameOfMoveEvent(device[theDevice]->task[i]->startEvent));
        if (device[theDevice]->task[i]->startEvent == DATE) {
            Serial.print(F(", startTime: "));
            Serial.print(device[theDevice]->task[i]->startTime);
        }
        if (device[theDevice]->task[i]->startEvent == BUTTON) {
            Serial.print(F(", startButton: "));
            Serial.print(device[theDevice]->task[i]->startButton);
            Serial.print(F(", at state: "));
            Serial.print(device[theDevice]->task[i]->startButtonState);
        }
        if (device[theDevice]->task[i]->startEvent == FOLLOW) {
            Serial.print(F(", of: "));
            Serial.print(device[device[theDevice]->task[i]->startTriggerDevice]->deviceName);
            Serial.print(F(", after move: "));
            Serial.print(device[theDevice]->task[i]->startTriggerMove);
        }
        if (device[theDevice]->task[i]->startEvent == POSITION) {
            Serial.print(F(", of: "));
            Serial.print(device[device[theDevice]->task[i]->startTriggerDevice]->deviceName);
            Serial.print(F(", on move: "));
            Serial.print(device[theDevice]->task[i]->startTriggerMove);
            Serial.print(F(", at position: "));
            Serial.print(device[theDevice]->task[i]->startTriggerPosition);
        }
        Serial.print(F(", terminated by: "));
        Serial.print(getNameOfMoveEvent(device[theDevice]->task[i]->stopEvent));
        if ((device[theDevice]->task[i]->stopEvent) == DATE) {
            Serial.print(F(", timeout: "));
            Serial.print(device[theDevice]->task[i]->timeout);
        }
        if ((device[theDevice]->task[i]->stopEvent) == BUTTON) {
            Serial.print(F(", stopButton: "));
            Serial.print(device[theDevice]->task[i]->stopButton);
            Serial.print(F(", at state: "));
            Serial.print(device[theDevice]->task[i]->stopButtonState);
        }
        if (device[theDevice]->task[i]->stopEvent == FOLLOW) {
            Serial.print(F(", of: "));
            Serial.print(device[device[theDevice]->task[i]->stopTriggerDevice]->deviceName);
            Serial.print(F(", after move: "));
            Serial.print(device[theDevice]->task[i]->stopTriggerMove);
        }
        if ((device[theDevice]->task[i]->stopEvent) == POSITION) {
            Serial.print(F(", of: "));
            Serial.print(device[device[theDevice]->task[i]->stopTriggerDevice]->deviceName);
            Serial.print(F(", on move: "));
            Serial.print(device[theDevice]->task[i]->stopTriggerMove);
            Serial.print(F(", at position: "));
            Serial.print(device[theDevice]->task[i]->stopTriggerPosition);
        }
        if (device[theDevice]->task[i]->switchMovePromptly) {
            Serial.print(F(" --> switch promptly to next move"));
        }
        Serial.println();
        
    }
    Serial.println();
}

void CCDeviceScheduler::deleteAllMoves() {
    for (int i = 0; i < countOfDevices; i++) {
        device[i]->deleteMoves();
    }
}

void CCDeviceScheduler::reviewMoves() {
    for (unsigned char d = 0; d < countOfDevices; d++) {
        device[d]->reviewValues();
    }
}

void CCDeviceScheduler::run() {
    
    unsigned long loopCounter = 0;
    unsigned char ongoingOperations;
    
    
    
    // prepare the loop
    for (unsigned char s = 0; s < countOfDevices; s++) {
        if (device[s]->countOfTasks > 0) {                                                      // (== MOVING || MOVE_DONE || PENDING_MOVES)
            device[s]->taskPointer = 0;
            device[s]->state = PENDING_MOVES;
            
            
            //	prepare first Moves
            
            device[s]->prepareNextMove();
            
            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                Serial.print(F("setup first Move for "));
                Serial.print(device[s]->deviceName);
                Serial.print(F(": current: "));
                Serial.print((int)device[s]->currentPosition);
                Serial.print(F(", target: "));
                Serial.println((int)device[s]->target);
            }
        }
    }
    
    
    //  start the task
    int loopTime_min = 10000;
    int loopTime_max = 0;
    unsigned long taskTime, loopTime;
    unsigned long taskStartTime = millis();
    
    
    do {
        loopTime = millis();
        taskTime = loopTime - taskStartTime;
        
        ongoingOperations = 0;
        
        
        for (unsigned char s = 0; s < countOfDevices; s++) {
            if (device[s]->state > SLEEPING) {                                                         // (== MOVING || MOVE_DONE || PENDING_MOVES)
                if (device[s]->state == MOVING) {                                                // if device is moving...
                    device[s]->driveDynamic();                                                  // so: move on!
                    
                    switch (device[s]->stopEvent) {                                                      // is there a stopEvent defined?
                        case DATE:                                                                              // yes, device shall stop by date
                            if (taskTime > device[s]->startTime + device[s]->startDelay + device[s]->timeout) { // it's time to stop!
                                if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                    Serial.print(taskTime);
                                    Serial.print(F(": "));
                                    Serial.print(device[s]->deviceName);
                                    Serial.print(F(" stop/switch "));
                                    Serial.println((int)device[s]->taskPointer);
                                }
                                
                                if (device[s]->switchMovePromptly) {                                              // switch immediately to next move?
                                    device[s]->taskPointer++;                                                   // go for next job! (if existing)
                                    if (device[s]->taskPointer < device[s]->countOfTasks) {                     //  all tasks done? no!
                                        device[s]->prepareNextMove();
                                    } else {
                                        device[s]->stopMoving();
                                    }
                                } else {                                                                          // just stop. but how?
                                    if (device[s]->stopSharply) {
                                        device[s]->stopMoving();
                                    } else {
                                        device[s]->initiateStop();
                                        device[s]->stopEvent = NONE;
                                    }
                                }
                            }
                            break;
                            
                        case BUTTON:                                                                            // device shall stop by button
                            if (digitalRead(device[s]->stopButton) == device[s]->stopButtonState) {             // it's time to stop!
                                if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                    Serial.print(taskTime);
                                    Serial.print(F(": "));
                                    Serial.print(device[s]->deviceName);
                                    Serial.print(F(" stop/switch "));
                                    Serial.println((int)device[s]->taskPointer);
                                }
                                
                                if (device[s]->switchMovePromptly) {                                              // switch immediately to next move?
                                    device[s]->taskPointer++;                                                   // go for next job! (if existing)
                                    if (device[s]->taskPointer < device[s]->countOfTasks) {                     //  all tasks done? no!
                                        device[s]->prepareNextMove();
                                    } else {
                                        device[s]->stopMoving();
                                    }
                                } else {                                                                          // just stop. but how?
                                    if (device[s]->stopSharply) {
                                        device[s]->stopMoving();
                                    } else {
                                        device[s]->initiateStop();
                                        device[s]->stopEvent = NONE;
                                    }
                                }
                            }
                            break;
                            
                        case FOLLOW:                                                                          // device shall stop when a device reached a certain position
                            if (device[device[s]->stopTriggerDevice]->taskPointer > device[s]->stopTriggerMove) {          //  trigger device on trigger move?
                                if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                    Serial.print(taskTime);
                                    Serial.print(F(": "));
                                    Serial.print(device[s]->deviceName);
                                    Serial.print(F(" stop/switch "));
                                    Serial.println((int)device[s]->taskPointer);
                                }
                                
                                if (device[s]->switchMovePromptly) {                                          // switch immediately to next move?
                                    device[s]->taskPointer++;                                               // go for next job! (if existing)
                                    if (device[s]->taskPointer < device[s]->countOfTasks) {                 // all tasks done? no!
                                        device[s]->prepareNextMove();
                                    } else {
                                        device[s]->stopMoving();
                                    }
                                } else {                                                                      // just stop. but how?
                                    if (device[s]->stopSharply) {
                                        device[s]->stopMoving();
                                    } else {
                                        device[s]->initiateStop();
                                        device[s]->stopEvent = NONE;
                                    }
                                }
                            }
                            break;
                            
                        case POSITION:                                                                          // device shall stop when a device reached a certain position
                            if (device[s]->stopTriggerMove == device[device[s]->stopTriggerDevice]->taskPointer) {          //  trigger device on trigger move?
                                if ((device[device[s]->stopTriggerDevice]->directionDown && device[device[s]->stopTriggerDevice]->currentPosition <= device[s]->stopTriggerPosition) || (!device[device[s]->stopTriggerDevice]->directionDown && device[device[s]->stopTriggerDevice]->currentPosition >= device[s]->stopTriggerPosition)) { // trigger position reached?
                                    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                        Serial.print(taskTime);
                                        Serial.print(F(": "));
                                        Serial.print(device[s]->deviceName);
                                        Serial.print(F(" stop/switch "));
                                        Serial.println((int)device[s]->taskPointer);
                                    }
                                    
                                    if (device[s]->switchMovePromptly) {                                          // switch immediately to next move?
                                        device[s]->taskPointer++;                                               // go for next job! (if existing)
                                        if (device[s]->taskPointer < device[s]->countOfTasks) {                 // all tasks done? no!
                                            device[s]->prepareNextMove();
                                        } else {
                                            device[s]->stopMoving();
                                        }
                                    } else {                                                                      // just stop. but how?
                                        if (device[s]->stopSharply) {
                                            device[s]->stopMoving();
                                        } else {
                                            device[s]->initiateStop();
                                            device[s]->stopEvent = NONE;
                                        }
                                    }
                                }
                            }
                            break;
                    }
                }                                                                                               // (device[s]->state == MOVING)
                else {                                                                                          // if device is stopped
                    
                    if (device[s]->state == MOVE_DONE) {                                                         // finished right now?
                        device[s]->finishMove();
                        
                        if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                            Serial.print(taskTime);
                            Serial.print(F(": "));
                            Serial.print(device[s]->deviceName);
                            Serial.print(F(" Move "));
                            Serial.print(device[s]->taskPointer);
                            Serial.println(F(" done"));
                        }
                        
                        device[s]->taskPointer++;                                                               // go for next job!
                        
                        if (device[s]->taskPointer < device[s]->countOfTasks) {                                 //  all tasks done? no!
                            device[s]->prepareNextMove();
                            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(device[s]->deviceName);
                                Serial.print(F(" prepare Move "));
                                Serial.print((int)device[s]->taskPointer);
                                Serial.print(F(": current: "));
                                Serial.print(device[s]->currentPosition);
                                Serial.print(F(" target: "));
                                Serial.println((int)device[s]->target);
                            }
                            
                        }
                        else {                                                                                  // all tasks are done
                            device[s]->state = SLEEPING;
                            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(device[s]->deviceName);
                                Serial.println(F(" finished all Moves\n"));
                            }
                        }
                    }
                    else {    //  (device[s]->state == MOVE_DONE)                                                //  device is idle
                        switch (device[s]->startEvent) {                                                        //  what kind of startEvent is given?
                            case DATE:                                                                          //  start the next move by date
                                if (taskTime > device[s]->startTime) {
                                    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                        Serial.print(taskTime);
                                        Serial.print(F(": "));
                                        Serial.print(device[s]->deviceName);
                                        Serial.print(F(" start by Date Move "));
                                        Serial.println((int)device[s]->taskPointer);
                                    }
                                    if (device[s]->startDelay > 0) {                                            // startDelay given?
                                        device[s]->startTime += device[s]->startDelay;
                                        device[s]->startDelay = 0;
                                    }
                                    else {
                                        device[s]->startMove();
                                    }
                                }
                                break;
                                
                            case BUTTON:                                                                        //  start the next move by button
                                if (digitalRead(device[s]->startButton) == device[s]->startButtonState) {
                                    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                        Serial.print(taskTime);
                                        Serial.print(F(": "));
                                        Serial.print(device[s]->deviceName);
                                        Serial.print(F(" start by Button Move "));
                                        Serial.println((int)device[s]->taskPointer);
                                    }
                                    if (device[s]->startDelay > 0) {                                            // startDelay given?
                                        device[s]->startTime = taskTime + device[s]->startDelay;                // so start the move by date
                                        device[s]->startDelay = 0;
                                        device[s]->startEvent = DATE;
                                    }
                                    else {
                                        device[s]->startTime = taskTime;
                                        device[s]->startMove();
                                    }
                                }
                                break;
                                
                            case FOLLOW:                                                                      //  start the next move when a device reached a certain
                                if (device[s]->taskPointer > device[s]->startTriggerMove) {        //  is the trigger servo doing the trigger move?
                                    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                        Serial.print(taskTime);
                                        Serial.print(F(": "));
                                        Serial.print(device[s]->deviceName);
                                        Serial.print(F(" start by Completion Move "));
                                        Serial.println((int)device[s]->taskPointer);
                                    }
                                    
                                    if (device[s]->startDelay > 0) {                                        // startDelay given?
                                        device[s]->startTime = taskTime + device[s]->startDelay;            // so start the move by date
                                        device[s]->startDelay = 0;
                                        device[s]->startEvent = DATE;
                                    } else {
                                        device[s]->startTime = taskTime;
                                        device[s]->startMove();
                                    }
                                }
                                break;
                                
                            case POSITION:                                                                      //  start the next move when a device reached a certain
                                if (device[s]->startTriggerMove <= device[device[s]->startTriggerDevice]->taskPointer) {        //  is the trigger servo doing the trigger move?
                                    
                                    if ((device[device[s]->startTriggerDevice]->directionDown && device[device[s]->startTriggerDevice]->currentPosition <= device[s]->startTriggerPosition) || (!device[device[s]->startTriggerDevice]->directionDown && device[device[s]->startTriggerDevice]->currentPosition >= device[s]->startTriggerPosition)) {
                                        //  did the trigger servo pass the trigger position?
                                        if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                            Serial.print(taskTime);
                                            Serial.print(F(": "));
                                            Serial.print(device[s]->deviceName);
                                            Serial.print(F(" start by Position Move "));
                                            Serial.println((int)device[s]->taskPointer);
                                        }
                                        
                                        if (device[s]->startDelay > 0) {                                        // startDelay given?
                                            device[s]->startTime = taskTime + device[s]->startDelay;            // so start the move by date
                                            device[s]->startDelay = 0;
                                            device[s]->startEvent = DATE;
                                        }
                                        else {
                                            device[s]->startTime = taskTime;
                                            device[s]->startMove();
                                        }
                                    }
                                }
                        }
                    }
                }
            }
            ongoingOperations += device[s]->state;                                                              // ongoing operations on any device?
            
        }
        
        loopCounter++;
        
        
        loopTime -= millis();
        loopTime_min = fmin(loopTime, loopTime_min);
        loopTime_max = fmax(loopTime, loopTime_max);
        
        
    }
    while (ongoingOperations > 0);
    
    
    
    Serial.print(F("\n"));
    Serial.print(F("loops: "));
    Serial.print((signed long)loopCounter);
    Serial.print(F(", elapsed time: "));
    Serial.print((int)((millis() - taskStartTime) / 1000));
    Serial.print(F(", loops second: "));
    Serial.print(1000.0 * loopCounter / (millis() - taskStartTime));
    Serial.print(F(", minimal loop time: "));
    Serial.print((int)loopTime_min);
    Serial.print(F(", maximal loop time: "));
    Serial.println((int)loopTime_max);
    Serial.println();
    Serial.println();
    
}


String getNameOfDeviceType(deviceType t) {
    if (t == SERVODEVICE) return "Servo";
    if (t == STEPPERDEVICE) return "Stepper";
    if (t == SWITCHINGDEVICE) return "Switch";
    return "unknown";
}
String getNameOfMoveEvent(event e) {
    if (e == NONE) return "none";
    if (e == DATE) return "date";
    if (e == BUTTON) return "button";
    if (e == FOLLOW) return "follow";
    if (e == POSITION) return "position";
    return "unknown";
}
String getNameOfState(deviceState s) {
    if (s == 0) return "sleeping";
    if (s == 1) return "moving";
    if (s == 2) return "move done";
    if (s == 4) return "pending moves";
    return "unknown";
}

String formatNumber(long theData, unsigned char len) {
    char theString[len + 1];
    theString[len] = 0;
    theString[len - 1] = (theData % 10) + 48;
    for (signed char i = len - 2; i >= 0; i--) {
        theData /= 10;
        if (theData > 0) {
            theString[i] = (theData % 10) + 48;
        }
        else {
            theString[i] = 32;
        }
    }
    return (String)theString;
}







