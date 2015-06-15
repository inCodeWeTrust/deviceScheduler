//
//  CCDeviceScheduler.cpp
//  CCDeviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//



#include "CCDeviceScheduler.h"


String getNameOfDeviceType(unsigned char t);
String getNameOfMoveEvent(unsigned char e);
String getNameOfState(unsigned char s);
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

unsigned char CCDeviceScheduler::addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition) {
    device[countOfDevices] = new CCServoDevice(deviceName, servo_pin, minPosition, maxPosition, parkPosition);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: add Servo: device #"));
        Serial.print(countOfDevices);
        Serial.print(F(", name: "));
        Serial.print(device[countOfDevices]->deviceName);
        Serial.print(F(", type: "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->type));
        //        Serial.print(F(", servo_pin: "));
        //        Serial.print(device[countOfDevices]->servo_pin);
        //        Serial.print(F(", park: "));
        //        Serial.print(device[countOfDevices]->parkPosition);
        Serial.print(F(", current: "));
        Serial.print(device[countOfDevices]->currentPosition);
        Serial.print(F(", at $"));
        Serial.println((long)device[countOfDevices], HEX);
        //    Serial.print(F(", with Servo at "));
        //    Serial.println((long)&device[countOfDevices]->theServo, HEX);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}

unsigned char CCDeviceScheduler::addStepper(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, float anglePerStep) {
    
    unsigned char numberOfMicroStepPins = 0;
    unsigned char elementBegin = 0;
    unsigned char elementEnd = microStepPinsString.indexOf(',', elementBegin);
    while (elementEnd > elementBegin) {
        numberOfMicroStepPins++;
        elementBegin = elementEnd + 1;
        elementEnd = microStepPinsString.indexOf(',', elementBegin);
    }

    unsigned char *microStepPin = new unsigned char[numberOfMicroStepPins + 1];
    
    elementBegin = 0;
    for (unsigned char pinIndex = 0; pinIndex <= numberOfMicroStepPins; pinIndex++) {
        elementEnd = microStepPinsString.indexOf(',', elementBegin);
        
        String theElement = microStepPinsString.substring(elementBegin, elementEnd);
        theElement.trim();
        
        if (theElement.indexOf('x') > 0 || theElement.indexOf('X') > 0) {
            char elementBuffer[theElement.length() + 1];
            theElement.toCharArray(elementBuffer, sizeof(elementBuffer));
            
            microStepPin[pinIndex] = strtol(elementBuffer, NULL, 16);
        }
        else
        {
            microStepPin[pinIndex] = microStepPinsString.substring(elementBegin, elementEnd).toInt();
        }
        elementBegin = elementEnd + 1;
    }
    
    
    unsigned char *stepModeCode = new unsigned char[highestSteppingMode + 1];
    
    elementBegin = 0;
    for (unsigned char codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
        elementEnd = stepModeCodesString.indexOf(',', elementBegin);
        
        String theElement = stepModeCodesString.substring(elementBegin, elementEnd);
        theElement.trim();
        
        if (theElement.indexOf('x') > 0 || theElement.indexOf('X') > 0) {
            char elementBuffer[theElement.length() + 1];
            theElement.toCharArray(elementBuffer, sizeof(elementBuffer));
            
            stepModeCode[codeIndex] = strtol(elementBuffer, NULL, 16);
        }
        else
        {
            stepModeCode[codeIndex] = stepModeCodesString.substring(elementBegin, elementEnd).toInt();
        }
        elementBegin = elementEnd + 1;
    }
    
    device[countOfDevices] = new CCStepperDevice(deviceName, dir_pin, step_pin, enable_pin, highestSteppingMode, stepModeCode, numberOfMicroStepPins, microStepPin, anglePerStep);
    
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: add Stepper: device #"));
        Serial.print(countOfDevices);
        Serial.print(F(", name: "));
        Serial.print(device[countOfDevices]->deviceName);
        Serial.print(F(", type: "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->type));
        //        Serial.print(F(", dir_pin: "));
        //        Serial.print(device[countOfDevices]->dir_pin);
        //        Serial.print(F(", step_pin: "));
        //        Serial.print(device[countOfDevices]->step_pin);
        //        Serial.print(F(", enable_pin: "));
        //        Serial.print(device[countOfDevices]->enable_pin);
        //        Serial.print(F(", anglePerStep: "));
        //        Serial.print(device[countOfDevices]->anglePerStep);
        Serial.print(F(", at $"));
        Serial.println((long) & (device[countOfDevices]), HEX);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}

unsigned char CCDeviceScheduler::addSolenoid(String deviceName, unsigned char solenoid_pin) {
    device[countOfDevices] = new CCSolenoidDevice(deviceName, solenoid_pin);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: add Solenoid: device #"));
        Serial.print(countOfDevices);
        Serial.print(F(", name: "));
        Serial.print(device[countOfDevices]->deviceName);
        Serial.print(F(", type: "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->type));
        //    Serial.print(F(", solenoid_pin: "));
        //    Serial.print(device[countOfDevices]->solenoid_pin);
        Serial.print(F(" at "));
        Serial.println((long) & (device[countOfDevices]), HEX);
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
        Serial.print(", moves: ");
        Serial.println(device[i]->countOfMoves);
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
    for (int i = 0; i < device[theDevice]->countOfMoves; i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(": target: "));
        Serial.print(device[theDevice]->theMove[i]->target);
        Serial.print(F(", velocity: "));
        Serial.print(device[theDevice]->theMove[i]->velocity);
        Serial.print(F(", acceleration: "));
        Serial.print(device[theDevice]->theMove[i]->acceleration);
        Serial.print(F(", startDelay: "));
        Serial.print(device[theDevice]->theMove[i]->startDelay);
        Serial.print(F(", started by "));
        Serial.print(getNameOfMoveEvent(device[theDevice]->theMove[i]->startEvent));
        if (device[theDevice]->theMove[i]->startEvent == DATE) {
            Serial.print(F(", startTime: "));
            Serial.print(device[theDevice]->theMove[i]->startTime);
        }
        if (device[theDevice]->theMove[i]->startEvent == BUTTON) {
            Serial.print(F(", startButton: "));
            Serial.print(device[theDevice]->theMove[i]->startButton);
            Serial.print(F(", at state: "));
            Serial.print(device[theDevice]->theMove[i]->startButtonState);
        }
        if (device[theDevice]->theMove[i]->startEvent == POSITION) {
            Serial.print(F(", of: "));
            Serial.print(device[device[theDevice]->theMove[i]->startTriggerDevice]->deviceName);
            Serial.print(F(", on move: "));
            Serial.print(device[theDevice]->theMove[i]->startTriggerMove);
            Serial.print(F(", at position: "));
            Serial.print(device[theDevice]->theMove[i]->startTriggerPosition);
        }
        Serial.print(F(", terminated by: "));
        Serial.print(getNameOfMoveEvent(device[theDevice]->theMove[i]->stopEvent));
        if (device[theDevice]->theMove[i]->stopEvent & 0x0F == DATE) {
            Serial.print(F(", timeout: "));
            Serial.print(device[theDevice]->theMove[i]->timeout);
        }
        if (device[theDevice]->theMove[i]->stopEvent & 0x0F == BUTTON) {
            Serial.print(F(", stopButton: "));
            Serial.print(device[theDevice]->theMove[i]->stopButton);
            Serial.print(F(", at state: "));
            Serial.print(device[theDevice]->theMove[i]->stopButtonState);
        }
        if (device[theDevice]->theMove[i]->stopEvent & 0x0F == POSITION) {
            Serial.print(F(", of: "));
            Serial.print(device[device[theDevice]->theMove[i]->stopTriggerDevice]->deviceName);
            Serial.print(F(", on move: "));
            Serial.print(device[theDevice]->theMove[i]->stopTriggerMove);
            Serial.print(F(", at position: "));
            Serial.print(device[theDevice]->theMove[i]->stopTriggerPosition);
        }
        if (device[theDevice]->theMove[i]->stopEvent & 0x10) {
            Serial.print(F(" --> switch to next move"));
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

void CCDeviceScheduler::runTheLoop() {
    
    unsigned long loopCounter = 0;
    unsigned char ongoingOperations;
    
    const char emptyTaskTime[] = {"        "};
    const char emptyPosition[] = {"      "};
    const char emptyMessage[] = {"                        "};
    
    // prepare the loop
    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
        Serial.println(F("setup the devices: "));
        Serial.print(emptyTaskTime);
    }
    for (unsigned char s = 0; s < countOfDevices; s++) {
        if (device[s]->countOfMoves > 0) {						// (== MOVING || MOVE_DONE || PENDING_MOVES)
            device[s]->movePointer = 0;
            device[s]->state = PENDING_MOVES;
            
            //	prepare first Moves
            
            device[s]->prepareNextMove();
            
            if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                Serial.print(F("setup first Move for "));
                Serial.print(device[s]->deviceName);
                Serial.print(F(": current: "));
                Serial.print((int)device[s]->currentPosition);
                Serial.print(F(", target: "));
                Serial.println((int)device[s]->target);
            }
            if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                Serial.print(emptyTaskTime);
                Serial.print(F(" | "));
                Serial.print(emptyPosition);
                Serial.print(F(" "));
                Serial.print(F("setup move "));
                Serial.print(formatNumber(device[s]->movePointer + 1, 2));
                Serial.print(F("/"));
                Serial.print(formatNumber(device[s]->countOfMoves, 2));
                Serial.print(F("       ."));
            }
            
        }
        else {
            if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                Serial.print(F("  [no tasks]            "));
            }
        }
    }
    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
        Serial.println();
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
        
        if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
            // ran till 211800
            Serial.print(formatNumber(taskTime, 8));
        }
        
        for (unsigned char s = 0; s < countOfDevices; s++) {
            if (device[s]->state > 0) {						// (== MOVING || MOVE_DONE || PENDING_MOVES)
                if (device[s]->state & MOVING) {			                // if device is moving...
                    device[s]->driveDynamic();                                            // so: move on!
                    
                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                        Serial.print(F(" | "));
                        Serial.print(formatNumber(device[s]->currentPosition, 6));
                        Serial.print(F(" "));
                    }
                    
                    switch (device[s]->stopEvent & 0x0F) {                                                     // is there a stopEvent defined?
                        case DATE:                                                                             // yes, device shall stop by date
                            if (taskTime > device[s]->startTime + device[s]->startDelay + device[s]->timeout) {      // it's time to stop!
                                if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                    Serial.print(F("->| time: "));
                                    Serial.print(formatNumber(device[s]->startTime + device[s]->timeout, 8));
                                    Serial.print(F("      "));
                                }
                                if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                    //                                    Serial.print(taskTime);
                                    //                                    Serial.print(F(": "));
                                    //                                    Serial.print(device[s]->deviceName);
                                    //                                    Serial.print(F(" initiate stop "));
                                    //                                    Serial.println((int)device[s]->movePointer);
                                }
                                
                                if (device[s]->stopEvent & 0x10) {                                          // switch immediately to next move?
                                    device[s]->movePointer++;                                               // go for next job! (if existing)
                                    if (device[s]->movePointer < device[s]->countOfMoves) {                 //  all tasks done? no!
                                        device[s]->prepareNextMove();
                                    }
                                }
                                else {                                                                      // just stop. but how?
                                    if (device[s]->stopSharply) {
                                        device[s]->stopMoving();
                                    }
                                    else {
                                        device[s]->initiateStop();
                                        device[s]->stopEvent = 0;
                                    }
                                }
                            }
                            else {                                                             //  stopEvent DATE given, timeout not yet expired
                                if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                    Serial.print(emptyMessage);
                                }
                            }
                            break;
                            
                        case BUTTON:                                                                         // device shall stop by button
                            if (digitalRead(device[s]->stopButton) == device[s]->stopButtonState) {          // it's time to stop!
                                if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                    Serial.print(F("->| button: "));
                                    Serial.print(formatNumber(device[s]->stopButton, 2));
                                    Serial.print(F("          "));
                                }
                                if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                    Serial.print(taskTime);
                                    Serial.print(F(": "));
                                    Serial.print(device[s]->deviceName);
                                    Serial.print(F(" initiate stop "));
                                    Serial.println((int)device[s]->movePointer);
                                }
                                
                                if (device[s]->stopEvent & 0x10) {                                           // switch immediately to next move?
                                    device[s]->movePointer++;                                                // go for next job! (if existing)
                                    if (device[s]->movePointer < device[s]->countOfMoves) {                  //  all tasks done? no!
                                        device[s]->prepareNextMove();
                                    }
                                }
                                else {                                                                       // just stop. but how?
                                    if (device[s]->stopSharply) {
                                        device[s]->stopMoving();
                                    }
                                    else {
                                        device[s]->initiateStop();
                                        device[s]->stopEvent = 0;
                                    }
                                }
                            }
                            else {                                                            // stopEvent BUTTON given, but was not yet pressed
                                if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                    Serial.print(emptyMessage);
                                }
                            }
                            break;
                            
                        case POSITION:                                                        // device shall stop by position of another device
                            if (device[s]->stopTriggerMove == device[device[s]->stopTriggerDevice]->movePointer) {  //  trig dev on trigger move?
                                if ((device[device[s]->stopTriggerDevice]->directionDown && device[device[s]->stopTriggerDevice]->currentPosition <= device[s]->stopTriggerPosition) || (!device[device[s]->stopTriggerDevice]->directionDown && device[device[s]->stopTriggerDevice]->currentPosition >= device[s]->stopTriggerPosition)) { // trigger position reached?
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                        Serial.print(F("->| from: "));
                                        Serial.print(formatNumber(device[s]->stopTriggerDevice, 2));
                                        Serial.print(F(" ->"));
                                        Serial.print(formatNumber((int)device[s]->stopTriggerPosition, 6));
                                        Serial.print(F("   "));
                                    }
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                        Serial.print(taskTime);
                                        Serial.print(device[s]->deviceName);
                                        Serial.print(F(" initiate stop move "));
                                        Serial.println((int)device[s]->movePointer);
                                    }
                                    
                                    if (device[s]->stopEvent & 0x10) {                                        // switch immediately to next move?
                                        device[s]->movePointer++;                                             // go for next job! (if existing)
                                        if (device[s]->movePointer < device[s]->countOfMoves) {               //  all tasks done? no!
                                            device[s]->prepareNextMove();
                                        }
                                    }
                                    else {                                                                    // just stop. but how?
                                        if (device[s]->stopSharply) {
                                            device[s]->stopMoving();
                                        }
                                        else {
                                            device[s]->initiateStop();
                                            device[s]->stopEvent = 0;
                                        }
                                    }
                                }
                                else {
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                        Serial.print(emptyMessage);
                                    }
                                }
                            }
                            else {                                                          // stopEvent POSITION given, but was not yet reached
                                if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                    Serial.print(emptyMessage);
                                }
                            }
                            break;
                            
                        default:
                            if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                Serial.print(emptyMessage);
                            }
                    }
                }    // (device[s]->state & MOVING)
                
                else {                                                                          // if device is stopped
                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                        Serial.print(F(" | "));
                        Serial.print(emptyPosition);
                        Serial.print(F(" "));
                    }
                    
                    if (device[s]->state & MOVE_DONE) {                                          // finished right now?
                        device[s]->finishMove();
                        
                        if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                            Serial.print(taskTime);
                            Serial.print(F(": "));
                            Serial.print(device[s]->deviceName);
                            Serial.print(F(" Move "));
                            Serial.print(device[s]->movePointer);
                            Serial.println(F(" done"));
                        }
                        
                        device[s]->movePointer++;                                                  // go for next job!
                        
                        if (device[s]->movePointer < device[s]->countOfMoves) {                    //  all tasks done? no!
                            
                            device[s]->prepareNextMove();
                            
                            if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                Serial.print(F(" setup move "));
                                Serial.print(formatNumber(device[s]->movePointer, 2));
                                Serial.print(F("/"));
                                Serial.print(formatNumber(device[s]->countOfMoves, 2));
                                Serial.print(F("       "));
                            }
                            if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(device[s]->deviceName);
                                Serial.print(F(" prepare Move "));
                                Serial.print((int)device[s]->movePointer);
                                Serial.print(F(": current: "));
                                Serial.print(device[s]->currentPosition);
                                Serial.print(F(" target: "));
                                Serial.println((int)device[s]->target);
                            }
                            
                        }
                        else {                                                              // all tasks are done
                            device[s]->state = 0;
                            if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                Serial.print(F("all done                "));
                            }
                            if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(device[s]->deviceName);
                                Serial.println(F(" finished all Moves\n"));
                            }
                        }
                    }
                    else {    //  (device[s]->state & MOVE_DONE)
                        //  device is idle
                        switch (device[s]->startEvent) {                                                //  what kind of startEvent is given?
                            case DATE:                                                                    //  start the next move by date
                                if (taskTime > device[s]->startTime) {
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                        Serial.print(F("|-> time: "));
                                        Serial.print(formatNumber(device[s]->startTime, 8));
                                        Serial.print(F("      "));
                                    }
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                        Serial.print(taskTime);
                                        Serial.print(F(": "));
                                        Serial.print(device[s]->deviceName);
                                        Serial.print(F(" start by Date Move "));
                                        Serial.println((int)device[s]->movePointer);
                                    }
                                    if (device[s]->startDelay > 0) {                                          // startDelay given?
                                        device[s]->startTime += device[s]->startDelay;
                                        device[s]->startDelay = 0;
                                    }
                                    else {
                                        device[s]->startMove();
                                    }
                                }
                                else {
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                        Serial.print(emptyMessage);
                                    }
                                }
                                break;
                                
                            case BUTTON:                                                                //  start the next move by button
                                if (digitalRead(device[s]->startButton) == device[s]->startButtonState) {
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                        Serial.print(F("|-> button: "));
                                        Serial.print(formatNumber(device[s]->startButton, 2));
                                        Serial.print(F("          "));
                                    }
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                        Serial.print(taskTime);
                                        Serial.print(F(": "));
                                        Serial.print(device[s]->deviceName);
                                        Serial.print(F(" start by Button Move "));
                                        Serial.println((int)device[s]->movePointer);
                                    }
                                    if (device[s]->startDelay > 0) {                                          // startDelay given?
                                        device[s]->startTime = taskTime + device[s]->startDelay;                // so start the move by date
                                        device[s]->startDelay = 0;
                                        device[s]->startEvent = DATE;
                                    }
                                    else {
                                        device[s]->startTime = taskTime;
                                        device[s]->startMove();
                                    }
                                }
                                else {
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                        Serial.print(emptyMessage);
                                    }
                                }
                                break;
                                
                            case POSITION:                                                               //  start the next move by position of another device
                                if (device[s]->startTriggerMove <= device[device[s]->startTriggerDevice]->movePointer) {                //  is the trigger servo doing the trigger move?
                                    
                                    if ((device[device[s]->startTriggerDevice]->directionDown && device[device[s]->startTriggerDevice]->currentPosition <= device[s]->startTriggerPosition) || (!device[device[s]->startTriggerDevice]->directionDown && device[device[s]->startTriggerDevice]->currentPosition >= device[s]->startTriggerPosition)) {
                                        //  did the trigger servo pass the trigger position?
                                        if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                            Serial.print(F("|-> from: "));
                                            Serial.print(formatNumber(device[s]->startTriggerDevice, 2));
                                            Serial.print(F(" ->"));
                                            Serial.print(formatNumber((int)device[s]->startTriggerPosition, 6));
                                            (device[device[s]->startTriggerDevice]->directionDown) ? Serial.print(F("-")) : Serial.print(F("+"));
                                            Serial.print(F("  "));
                                        }
                                        if (DEVICESCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                            Serial.print(taskTime);
                                            Serial.print(F(": "));
                                            Serial.print(device[s]->deviceName);
                                            Serial.print(F(" start by Position Move "));
                                            Serial.println((int)device[s]->movePointer);
                                        }
                                        
                                        if (device[s]->startDelay > 0) {                                          // startDelay given?
                                            device[s]->startTime = taskTime + device[s]->startDelay;                // so start the move by date
                                            device[s]->startDelay = 0;
                                            device[s]->startEvent = DATE;
                                        }
                                        else {
                                            device[s]->startTime = taskTime;
                                            device[s]->startMove();
                                        }
                                    }
                                    else {
                                        if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                            Serial.print(emptyMessage);
                                        }
                                    }
                                }
                                else {
                                    if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                                        Serial.print(emptyMessage);
                                    }
                                }
                        }
                    }
                }
            }
            else {
                if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
                    Serial.print(F(" | "));
                    Serial.print(emptyPosition);
                    Serial.print(F(" "));
                    Serial.print(emptyMessage);
                }
                
            }
            ongoingOperations += device[s]->state;
            
        }
        
        if (DEVICESCHEDULER_VERBOSE & SHOW_TAB_VIEW) {
            Serial.println();
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


String getNameOfDeviceType(unsigned char t) {
    if (t == 1) return "Servo";
    if (t == 2) return "Stepper";
    if (t == 3) return "Solenoid";
    return "unknown";
}
String getNameOfMoveEvent(unsigned char e) {
    if ((e & 0x0F) == 0) return "none";
    if ((e & 0x0F) == 1) return "date";
    if ((e & 0x0F) == 2) return "button";
    if ((e & 0x0F) == 4) return "position";
    return "unknown";
}
String getNameOfState(unsigned char s) {
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







