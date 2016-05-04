//
//  CCDeviceScheduler.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//



#include "CCDeviceScheduler.h"





CCDeviceScheduler::CCDeviceScheduler() {
    countOfDevices = 0;
    countOfControlButtons = 0;

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
    for (int i = countOfControlButtons - 1; i >= 0; i--) {
        delete controlButton[i];
        controlButton[i] = NULL;
    }
}



schedulerDevice CCDeviceScheduler::addDcController(String deviceName, unsigned char switching_pin, boolean switchingPin_activ) {
    device[countOfDevices] = new CCDcControllerDevice(deviceName, switching_pin, switchingPin_activ);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}


schedulerDevice CCDeviceScheduler::addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition) {
    
    device[countOfDevices] = new CCServoDevice(deviceName, servo_pin, minPosition, maxPosition, parkPosition);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}


schedulerDevice CCDeviceScheduler::addStepper_A4988(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, unsigned int stepsPerRotation) {
    
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
    
    device[countOfDevices] = new CCStepperDevice_A4988(deviceName, dir_pin, step_pin, enable_pin, highestSteppingMode, stepModeCode, numberOfMicroStepPins, microStepPin, stepsPerRotation);
    
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
    }
    
    free(microStepPin);
    free(stepModeCode);
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}


schedulerDevice CCDeviceScheduler::addStepper_TMC260(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char chipSelect_pin, unsigned int currentMax, unsigned int stepsPerRotation) {
    
    device[countOfDevices] = new CCStepperDevice_TMC260(deviceName, step_pin, dir_pin, enable_pin, chipSelect_pin, currentMax, stepsPerRotation);
    
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
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
        Serial.print(device[i]->getDeviceName());
        Serial.print(F(", type: "));
        Serial.print(getNameOfDeviceType(device[i]->getType()));
        Serial.print(F(", tasks: "));
        Serial.println(device[i]->getCountOfTasks());
    }
    Serial.println();
}

void CCDeviceScheduler::getAllTasks() {
    for (int i = 0; i < countOfDevices; i++) {
        getTasksForDevice(i);
    }
}


void CCDeviceScheduler::getTasksForDevice(schedulerDevice theDevice) {
    Serial.print(F("[CCDeviceScheduler]: Tasks of Device "));
    Serial.print(device[theDevice]->getDeviceName());
    Serial.println(F(": "));
    for (int i = 0; i < device[theDevice]->getCountOfTasks(); i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(": target: "));
        Serial.print(device[theDevice]->task[i]->getTarget());
        Serial.print(F(", velocity: "));
        Serial.print(device[theDevice]->task[i]->getVelocity());
        Serial.print(F(", acceleration: "));
        Serial.print(device[theDevice]->task[i]->getAcceleration());
        Serial.print(F(", startDelay: "));
        Serial.print(device[theDevice]->task[i]->getStartDelay());
        Serial.print(F(", started by "));
        Serial.print(getNameOfTaskEvent(device[theDevice]->task[i]->getStartEvent()));
        switch (device[theDevice]->task[i]->getStartEvent()) {
            case DATE:
                Serial.print(F(", startTime: "));
                Serial.print(device[theDevice]->task[i]->getStartTime());
                break;
            case BUTTON:
                Serial.print(F(", startButton: "));
                Serial.print(controlButton[device[theDevice]->task[i]->getStartButton()]->getButtonName());
                Serial.print(F(", at state: "));
                Serial.print(controlButton[device[theDevice]->task[i]->getStartButton()]->getButtonActiv());
                break;
            case FOLLOW:
                Serial.print(F(", of: "));
                Serial.print(device[device[theDevice]->task[i]->getStartTriggerDevice()]->getDeviceName());
                Serial.print(F(", after move: "));
                Serial.print(device[theDevice]->task[i]->getStartTriggerTask());
                break;
            case POSITION:
                Serial.print(F(", of: "));
                Serial.print(device[device[theDevice]->task[i]->getStartTriggerDevice()]->getDeviceName());
                Serial.print(F(", on move: "));
                Serial.print(device[theDevice]->task[i]->getStartTriggerTask());
                Serial.print(F(", at position: "));
                Serial.print(device[theDevice]->task[i]->getStartTriggerPosition());
                break;
        }
        Serial.print(F(", terminated by: "));
        Serial.print(getNameOfTaskEvent(device[theDevice]->task[i]->getStopEvent()));
        if ((device[theDevice]->task[i]->getStopEvent()) > 0) {
            Serial.print(", stopping: ");
            Serial.print(getNameOfStoppingMode(device[theDevice]->task[i]->getStopping()));

            switch (device[theDevice]->task[i]->getStopEvent()) {
                case DATE:
                    Serial.print(F(", timeout: "));
                    Serial.print(device[theDevice]->task[i]->getTimeout());
                    break;
                case BUTTON:
                    Serial.print(F(", stopButton: "));
                    Serial.print(controlButton[device[theDevice]->task[i]->getStopButton()]->getButtonName());
                    Serial.print(F(", at state: "));
                    Serial.print(controlButton[device[theDevice]->task[i]->getStopButton()]->getButtonActiv());
                    break;
                case FOLLOW:
                    Serial.print(F(", of: "));
                    Serial.print(device[device[theDevice]->task[i]->getStopTriggerDevice()]->getDeviceName());
                    Serial.print(F(", after move: "));
                    Serial.print(device[theDevice]->task[i]->getStopTriggerTask());
                    break;
                case POSITION:
                    Serial.print(F(", of: "));
                    Serial.print(device[device[theDevice]->task[i]->getStopTriggerDevice()]->getDeviceName());
                    Serial.print(F(", on move: "));
                    Serial.print(device[theDevice]->task[i]->getStopTriggerTask());
                    Serial.print(F(", at position: "));
                    Serial.print(device[theDevice]->task[i]->getStopTriggerPosition());
                    break;
            }
        
            if (device[theDevice]->task[i]->getSwitchTaskPromptly()) {
                Serial.print(F(" --> switch promptly to next move"));
            }
        }
        Serial.println();
        
    }
    Serial.println();
}

void CCDeviceScheduler::deleteAllTasks() {
    for (int i = 0; i < countOfDevices; i++) {
        device[i]->deleteTasks();
    }
}

void CCDeviceScheduler::reviewTasks() {
    for (unsigned char d = 0; d < countOfDevices; d++) {
        device[d]->reviewValues();
    }
}

unsigned char CCDeviceScheduler::addControlButton(String buttonName, unsigned char button_pin, boolean buttonActiv) {
    controlButton[countOfControlButtons] = new CCControlButton(countOfControlButtons, buttonName, button_pin, buttonActiv);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: provided controlButton "));
        Serial.println(controlButton[countOfControlButtons]->getButtonName());
    }
    
    countOfControlButtons++;
    //	control button index = countOfControlButtons - 1 [8 buttons: index of first: 0, last: 7]
    
    return countOfControlButtons - 1;

}
void CCDeviceScheduler::getAllControlButtons() {
    Serial.println(F("[CCDeviceScheduler]: My ControlButtons: "));
    for (int i = 0; i < countOfControlButtons; i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(", name: "));
        Serial.print(controlButton[i]->getButtonName());
        Serial.print(F(", activ: "));
        Serial.print(controlButton[i]->getButtonActiv());
        Serial.print(F(", actions: "));
        Serial.println(controlButton[i]->getCountOfActions());
    }
    Serial.println();
}

void CCDeviceScheduler::getAllActions() {
    for (int i = 0; i < countOfControlButtons; i++) {
        getActionsForControlButton(i);
    }
}

void CCDeviceScheduler::getActionsForControlButton(unsigned char theButton) {
    Serial.print(F("[CCDeviceScheduler]: Actions of ControlButton "));
    Serial.print(controlButton[theButton]->getButtonName());
    Serial.println(F(": "));
    for (int i = 0; i < controlButton[theButton]->getCountOfActions(); i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(": targetDevice: "));
        Serial.print(controlButton[theButton]->getAction(i).targetDevice);
        Serial.print(F(", validTask: "));
        Serial.print(controlButton[theButton]->getAction(i).validTask);
        Serial.print(F(", targetAction: "));
        Serial.print(getNameOfDeviceAction(controlButton[theButton]->getAction(i).targetAction));
        Serial.print(F(", followingTask: "));
        Serial.println((int)controlButton[theButton]->getAction(i).followingTask);
    }
    Serial.println();
}

void CCDeviceScheduler::deleteAllActions() {
    for (int i = 0; i < countOfControlButtons; i++) {
        controlButton[i]->deleteActions();
    }
}




void CCDeviceScheduler::run() {
    
    unsigned long loopCounter = 0;
    unsigned char ongoingOperations;
    
    int i = 100;
    int c32, hend, hstrt, tof = 0;
    
    
    // prepare the loop
    for (unsigned char s = 0; s < countOfDevices; s++) {
        if (device[s]->getCountOfTasks() > 0) {                                                      // (== MOVING || MOVE_DONE || PENDING_MOVES)
            device[s]->setTaskPointer(0);
            device[s]->setState(PENDING_MOVES);
            
            
            //	prepare first Tasks
            
            device[s]->prepareNextTask();
            
            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                Serial.print(F("setup first Task for "));
                Serial.print(device[s]->getDeviceName());
                Serial.print(F(": current Position: "));
                Serial.print((int)device[s]->getCurrentPosition());
                Serial.print(F(", target: "));
                Serial.println((float)device[s]->getTarget());
            }
        }
    }
    delay(200);
    
    //  start the workflow
    long loopTime_min = 10000;
    long loopTime_max = 0;
    unsigned long taskTime, loopTime;
    unsigned long taskStartTime = millis();
    
    unsigned long lastPrintTime = taskStartTime;
    
    do {
        loopTime = millis();
        taskTime = loopTime - taskStartTime;
        
        ongoingOperations = 0;
        
        
        for (schedulerDevice s = 0; s < countOfDevices; s++) {
            if (device[s]->getState() > SLEEPING) {                                                         // (== MOVING || MOVE_DONE || PENDING_MOVES)
                if (device[s]->getState() == MOVING) {                                                // if device is moving...
                    device[s]->operateTask();                                                  // so: move on!
                    
                    switch (device[s]->getStopEvent()) {                                                      // is there a stopEvent defined?
                        case DATE:                                                                              // yes, device shall stop by date
                            if (taskTime > device[s]->getStartTime() + device[s]->getStartDelay() + device[s]->getTimeout()) { // it's time to stop!
                                handleStopEvent(taskTime, s, device[s]->getStopEvent());
                            }
                            break;
                            
                        case BUTTON:                                                                            // device shall stop by button
                            if (controlButton[device[s]->getStopButton()]->isActiv()) {             // it's time to stop!
                                handleStopEvent(taskTime, s, device[s]->getStopEvent());
                            }
                            break;
                            
                        case FOLLOW:                                                                          // device shall stop when a device reached a certain position
                            if (device[device[s]->getStopTriggerDevice()]->getTaskPointer() > device[s]->getStopTriggerTask()) {          //  trigger device on trigger move?
                                handleStopEvent(taskTime, s, device[s]->getStopEvent());
                            }
                            break;
                            
                        case POSITION:                                                                          // device shall stop when a device reached a certain position
                            if (device[s]->getStopTriggerTask() == device[device[s]->getStopTriggerDevice()]->getTaskPointer()) {          //  trigger device on trigger move?
                                if ((device[device[s]->getStopTriggerDevice()]->getDirectionDown() && device[device[s]->getStopTriggerDevice()]->getCurrentPosition() <= device[s]->getStopTriggerPosition()) || (!device[device[s]->getStopTriggerDevice()]->getDirectionDown() && device[device[s]->getStopTriggerDevice()]->getCurrentPosition() >= device[s]->getStopTriggerPosition())) { // trigger position reached?
                                    handleStopEvent(taskTime, s, device[s]->getStopEvent());
                                }
                            }
                            break;
                    }
                }                                                                                               // (device[s]->getState() == MOVING)
                else {                                                                                          // if device is stopped
                    
                    if (device[s]->getState() == MOVE_DONE) {                                                         // finished right now?
                        device[s]->finishTask();
                        
                        if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                            Serial.print(taskTime);
                            Serial.print(F(": "));
                            Serial.print(device[s]->getDeviceName());
                            Serial.print(F(" Task "));
                            Serial.print(device[s]->getTaskPointer());
                            Serial.println(F(" done"));
                        }
                        
                        device[s]->increaseTaskPointer();                                                               // go for next job!
                        
                        if (device[s]->getTaskPointer() < device[s]->getCountOfTasks()) {                                 //  all tasks done? no!
                            device[s]->setState(PENDING_MOVES);
                            device[s]->prepareNextTask();
                            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(device[s]->getDeviceName());
                                Serial.print(F(" prepare Task "));
                                Serial.print((int)device[s]->getTaskPointer());
                                Serial.print(F(": current Position: "));
                                Serial.print(device[s]->getCurrentPosition());
                                Serial.print(F(" target: "));
                                Serial.println((int)device[s]->getTarget());
                            }
                            
                        }
                        else {                                                                                  // all tasks are done
                            device[s]->setState(SLEEPING);
                            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(device[s]->getDeviceName());
                                Serial.println(F(" finished all Tasks"));
                            }
                        }
                    }
                    else {    //  (device[s]->getState() == MOVE_DONE)                                                //  device is idle
                        switch (device[s]->getStartEvent()) {                                                        //  what kind of startEvent is given?
                            case DATE:                                                                          //  start the next move by date
                                if (taskTime > device[s]->getStartTime()) {
                                    handleStartEvent(taskTime, s, device[s]->getStartEvent());
                                }
                                break;
                                
                            case BUTTON:                                                                        //  start the next move by button
                                if (controlButton[device[s]->getStartButton()]->isActiv()) {
                                    handleStartEvent(taskTime, s, device[s]->getStartEvent());
                                }
                                break;
                                
                            case FOLLOW:                                                                      //  start the next move when a device reached a certain
                                if (device[device[s]->getStartTriggerDevice()]->getTaskPointer() > device[s]->getStartTriggerTask()) {        //  is the trigger servo doing the trigger move?
                                    handleStartEvent(taskTime, s, device[s]->getStartEvent());
                                }
                                break;
                                
                            case POSITION:                                                                      //  start the next move when a device reached a certain
                                if (device[device[s]->getStartTriggerDevice()]->getTaskPointer() >= device[s]->getStartTriggerTask()) {        //  is the trigger servo doing the trigger move?
                                    
                                    if ((device[device[s]->getStartTriggerDevice()]->getDirectionDown() && device[device[s]->getStartTriggerDevice()]->getCurrentPosition() <= device[s]->getStartTriggerPosition()) || (!device[device[s]->getStartTriggerDevice()]->getDirectionDown() && device[device[s]->getStartTriggerDevice()]->getCurrentPosition() >= device[s]->getStartTriggerPosition())) {
                                        //  did the trigger servo pass the trigger position?
                                        handleStartEvent(taskTime, s, device[s]->getStartEvent());
                                    }
                                }
                        }
                    }
                }
            }
            ongoingOperations += device[s]->getState();                                                              // ongoing operations on any device?
            
        }
        /*
        if (taskTime & 0x1000) {
            i = analogRead(A8) >> 5;
            i = (c32 * 3 + i) >> 2;
            if (i != c32) {
                c32 = i;
                device[6]->setCurrentScale(c32);
                Serial.print(" *** c32: "), Serial.println(c32);
            }
        }
        if (taskTime & 0x1010) {
            i = (analogRead(A9) >> 6 ) - 3;
            i = (hend * 3 + i) >> 2;
            if (i != hend) {
                hend = i;
                device[6]->setChopperControlRegister_spreadCycle(0, 0, 1, hend, hstrt, tof);
                Serial.print("*** hend: "), Serial.print(hend);
            }
        }
        if (taskTime & 0x1100) {
            i = (analogRead(A10) >> 7) + 1;
            i = (hstrt * 3 + i) >> 2;
            if (i != hstrt) {
                hstrt = i;
                device[6]->setChopperControlRegister_spreadCycle(0, 0, 1, hend, hstrt, tof);
                Serial.print("*** hstrt: "), Serial.print(hstrt);
            }
        }
        if (taskTime & 0x1110) {
            i = analogRead(A11) >> 6;
            i = (tof * 3 + i) >> 2;
            if (i != tof) {
                tof = i;
                device[6]->setChopperControlRegister_spreadCycle(0, 0, 1, hend, hstrt, tof);
                Serial.print("*** t_off: "), Serial.println(tof);
            }
        }
         */
        /*
        if (taskTime - lastPrintTime > 200) {
            device[6]->getReadOut(2);
            lastPrintTime = taskTime;
        }
        */
        for (unsigned char b = 0; b < countOfControlButtons; b++) {
            controlButton[b]->readButtonState();
            if (controlButton[b]->isActiv()) {
                for (int theAction = 0; theAction < controlButton[b]->getCountOfActions(); theAction++) {
                    if (!controlButton[b]->getAction(theAction).actionDone) {
                        if (controlButton[b]->getAction(theAction).validTask == device[controlButton[b]->getAction(theAction).targetDevice]->getTaskPointer()) {
                            device[controlButton[b]->getAction(theAction).targetDevice]->setTaskPointer(controlButton[b]->getAction(theAction).followingTask - 1);
                            switch (controlButton[b]->getAction(theAction).targetAction) {
                                case START:
                                    handleStartEvent(taskTime, controlButton[b]->getAction(theAction).targetDevice, CONTROLBUTTON);
                                    break;
                                case STOP_AND_SWITCH:
                                    device[controlButton[b]->getAction(theAction).targetDevice]->setSwitchTaskPromptly(true);
                                case STOP:
                                    device[controlButton[b]->getAction(theAction).targetDevice]->setStopping(STOP_NORMAL);
                                    handleStopEvent(taskTime, controlButton[b]->getAction(theAction).targetDevice, CONTROLBUTTON);
                                    break;
                                case STOP_SHARP_AND_SWITCH:
                                    device[controlButton[b]->getAction(theAction).targetDevice]->setSwitchTaskPromptly(true);
                                case STOP_SHARP:
                                    device[controlButton[b]->getAction(theAction).targetDevice]->setStopping(STOP_IMMEDIATELY);
                                    handleStopEvent(taskTime, controlButton[b]->getAction(theAction).targetDevice, CONTROLBUTTON);
                            }
                            controlButton[b]->setActionDone(theAction);
                        }
                    }
                }
            }
        }
        
        
        
        loopCounter++;
        
        
//        loopTime -= millis();
//        loopTime_min = MIN(loopTime, loopTime_min);
//        loopTime_max = max(loopTime, loopTime_max);

        
    } while (ongoingOperations > 0);
    
    
    
    Serial.print(F("\n"));
    Serial.print(F("loops: "));
    Serial.print((signed long)loopCounter);
    Serial.print(F(", elapsed time: "));
    Serial.print((int)((millis() - taskStartTime) / 1000));
    Serial.print(F(", loops/second: "));
    Serial.print(1000.0 * loopCounter / (millis() - taskStartTime));
//    Serial.print(F(", minimal loop time: "));
//    Serial.print(loopTime_min);
//    Serial.print(F(", maximal loop time: "));
//    Serial.println(loopTime_max);
    Serial.println();
    Serial.println();
    
}


void CCDeviceScheduler::handleStartEvent(unsigned long taskTime, schedulerDevice s, event startEvent) {
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
        Serial.print(taskTime);
        Serial.print(F(": "));
        Serial.print(device[s]->getDeviceName());
        Serial.print(F(" start task "));
        Serial.print((int)device[s]->getTaskPointer());
        Serial.print(" by ");
        Serial.println(getNameOfTaskEvent(startEvent));
    }
    
    if (device[s]->getStartDelay() > 0) {                                        // startDelay given?
        if (startEvent == DATE) {
            device[s]->setStartTime(device[s]->getStartTime() + device[s]->getStartDelay());
        } else {
            device[s]->setStartTime(taskTime + device[s]->getStartDelay());            // so start the move by date
            device[s]->setStartEvent(DATE);
        }
        device[s]->setStartDelay(0);
    }
    else {
        device[s]->setStartTime(taskTime);
        device[s]->startTask();
    }
}
void CCDeviceScheduler::handleStopEvent(unsigned long taskTime, schedulerDevice s, event stopEvent) {
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
        Serial.print(taskTime);
        Serial.print(F(": "));
        Serial.print(device[s]->getDeviceName());
        Serial.print(F(" stop task "));
        Serial.print((int)device[s]->getTaskPointer());
        Serial.print(" by ");
        Serial.print(getNameOfTaskEvent(stopEvent));
        Serial.print(", switchTaskPromptly: ");
        Serial.print(device[s]->getSwitchTaskPromptly());
        Serial.print(", stopping: ");
        Serial.println(getNameOfStoppingMode(device[s]->getStopping()));
}
    
    if (device[s]->getSwitchTaskPromptly()) {                                              // switch immediately to next move?
        device[s]->increaseTaskPointer();                                                   // go for next job! (if existing)
        if (device[s]->getTaskPointer() < device[s]->getCountOfTasks()) {                     //  all tasks done? no!
            device[s]->prepareNextTask();
        } else {
            device[s]->stopTask();
        }
    } else {                                                                          // just stop. but how?
        if (device[s]->getStopping() == STOP_IMMEDIATELY) {
            device[s]->stopTask();
        } else {
            device[s]->initiateStop();
            device[s]->setStopEvent(NONE);
        }
    }
}

String CCDeviceScheduler::getNameOfDeviceType(deviceType t) {
    if (t == SERVODEVICE) return "Servo";
    if (t == STEPPERDEVICE) return "Stepper";
    if (t == DCCONTROLLERDEVICE) return "Switch";
    return "unknown";
}
String CCDeviceScheduler::getNameOfTaskEvent(event e) {
    if (e == NONE) return "none";
    if (e == DATE) return "date";
    if (e == BUTTON) return "button";
    if (e == FOLLOW) return "follow";
    if (e == POSITION) return "position";
    if (e == CONTROLBUTTON) return "control button";
    return "unknown";
}
String CCDeviceScheduler::getNameOfState(deviceState s) {
    if (s == SLEEPING) return "sleeping";
    if (s == MOVING) return "moving";
    if (s == MOVE_DONE) return "move done";
    if (s == PENDING_MOVES) return "pending moves";
    return "unknown";
}
String CCDeviceScheduler::getNameOfStoppingMode(stoppingMode s) {
    if (s == STOP_IMMEDIATELY) return "stop immediately";
    if (s == STOP_NORMAL) return "stop normal";
    if (s == STOP_DYNAMIC) return "stop dynamic";
    return "unknown";
}
String CCDeviceScheduler::getNameOfDeviceAction(deviceAction d) {
    if (d == START) return "start";
    if (d == STOP_AND_SWITCH) return "stop and switch";
    if (d == STOP) return "stop";
    if (d == STOP_SHARP_AND_SWITCH) return "stop immediately and switch";
    if (d == STOP_SHARP) return "stop immediately";
    return "unknown";
}


String CCDeviceScheduler::formatNumber(long theData, unsigned char len) {
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







