//
//  CCDeviceScheduler.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//



#include "CCDeviceScheduler.h"





CCDeviceScheduler::CCDeviceScheduler(String name) {
    schedulerName = name;
    countOfDevices = 0;
    countOfControlButtons = 0;

    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: constructed scheduler "));
        Serial.println(schedulerName);
    }
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: constructed scheduler "));
        Serial.print(schedulerName);
        Serial.print(F(" at $"));
        Serial.println((long)this, HEX);
    }
}

CCDeviceScheduler::~CCDeviceScheduler() {
    for (int i = countOfDevices - 1; i >= 0; i--) {
        delete device[i];
        device[i] = NULL;
    }
    for (int i = countOfControlButtons - 1; i >= 0; i--) {
        delete controlButton[i];
        controlButton[i] = NULL;
    }

    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: destructed scheduler "));
        Serial.println(schedulerName);
    }
    
}


schedulerDevice CCDeviceScheduler::addDcController(String deviceName, unsigned char switching_pin, boolean switchingPin_activ) {
    device[countOfDevices] = new CCDcControllerDevice(deviceName, switching_pin, switchingPin_activ);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
    }
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCDcControllerDevice constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }

    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}


schedulerDevice CCDeviceScheduler::addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition) {
    
    device[countOfDevices] = new CCServoDevice(deviceName, servo_pin, minPosition, maxPosition, parkPosition);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
    }
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCServoDevice constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}
schedulerDevice CCDeviceScheduler::addServoWithCounterServo(String deviceName, unsigned char servo_00_pin, int minPosition_00, int midPosition_00, int maxPosition_00, unsigned char servo_01_pin, int minPosition_01, int midPosition_01, int maxPosition_01, int parkPosition) {
    
    device[countOfDevices] = new CCServoDevice_cross(deviceName, servo_00_pin, minPosition_00, midPosition_00, maxPosition_00, servo_01_pin, minPosition_01, midPosition_01, maxPosition_01, parkPosition);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
    }
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCServoDevice constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;
}


schedulerDevice CCDeviceScheduler::addStepper_A4988(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, unsigned int stepsPerRotation) {
    
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
    
    device[countOfDevices] = new CCStepperDevice_A4988(deviceName, step_pin, dir_pin, enable_pin, highestSteppingMode, stepModeCode, numberOfMicroStepPins, microStepPin, stepsPerRotation);
    
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
    }
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCStepperDevice_A4988 constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
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
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getDeviceName());
    }
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCStepperDevice_TMC260 constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return countOfDevices - 1;

}


void CCDeviceScheduler::getAllDevices() {
    Serial.print(F("[CCDeviceScheduler]: "));
    Serial.print(schedulerName);
    Serial.println(F(": Devices: "));
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
    Serial.print(F("[CCDeviceScheduler]: "));
    Serial.print(schedulerName);
    Serial.print(F(": Tasks of Device "));
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
        if (device[theDevice]->task[i]->getMoveRelativ()) {
            Serial.print(F(", move relativ"));
        }
        if (device[theDevice]->task[i]->getWithPositionReset()) {
            Serial.print(F(", with position reset"));
        }
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
            default:
                Serial.print(F(", no start event given"));
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
                default:
                    Serial.print(F(", no stop event given"));
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


unsigned char CCDeviceScheduler::addControlButton(String buttonName, unsigned char button_pin, boolean buttonActiv, boolean pullup) {
    controlButton[countOfControlButtons] = new CCControlButton(countOfControlButtons, buttonName, button_pin, buttonActiv, pullup);
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided controlButton "));
        Serial.println(controlButton[countOfControlButtons]->getButtonName());
    }
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCDcControlButton constructed at $"));
        Serial.println((long)controlButton[countOfControlButtons], HEX);
    }

    countOfControlButtons++;
    //	control button index = countOfControlButtons - 1 [8 buttons: index of first: 0, last: 7]
    
    return countOfControlButtons - 1;

}

void CCDeviceScheduler::getAllControlButtons() {
    Serial.print(F("[CCDeviceScheduler]: "));
    Serial.print(schedulerName);
    Serial.println(F(": ControlButtons: "));
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
    Serial.print(F("[CCDeviceScheduler]: "));
    Serial.print(schedulerName);
    Serial.print(F(": Actions of ControlButton "));
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
    

    // prepare the loop
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.println(F(": prepare run ..."));
    }
 
    for (unsigned char s = 0; s < countOfDevices; s++) {
        if (device[s]->getCountOfTasks() > 0) {                                                      // (== MOVING || MOVE_DONE || PENDING_MOVES)
            
            device[s]->attachDevice();
            
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

    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.println(F(": run ..."));
    }

    for (schedulerControlButton b = 0; b < countOfControlButtons; b++) {
        controlButton[b]->readButtonState();
    }
    
    //  start the workflow

    unsigned long taskTime, loopTime;
    unsigned long taskStartTime = millis();
    
    //    unsigned long lastPrintTime = taskStartTime;
    
    schedulerControlButton theButton = 0;
    
    do {
        loopTime = millis();
        taskTime = loopTime - taskStartTime;
        
        ongoingOperations = 0;
        
       
        for (schedulerDevice s = 0; s < countOfDevices; s++) {
            if (device[s]->getState() > SLEEPING) {                                                             // (== MOVING || MOVE_DONE || PENDING_MOVES)
                if (device[s]->getState() == MOVING) {                                                          // if device is moving...
                    device[s]->operateTask();                                                                   // so: move on!
                    
                    switch (device[s]->getStopEvent()) {                                                        // is there a stopEvent defined?
                        case DATE:                                                                              // yes, device shall stop by date
                            if (taskTime > device[s]->getStartTime() + device[s]->getStartDelay() + device[s]->getTimeout()) { // it's time to stop!
                                handleStopEvent(taskTime, s, device[s]->getStopEvent());
                            }
                            break;
                            
                        case BUTTON:                                                                            // device shall stop by button
                            controlButton[device[s]->getStopButton()]->readButtonState();
                            if ((controlButton[device[s]->getStopButton()]->isActiv())) {                         // it's time to stop!
                                
                                Serial.print("stop device ");
                                Serial.print(s);
                                Serial.print(", button: ");
                                Serial.print(device[s]->getStopButton());
                                Serial.print(", at state: ");
                                Serial.println(controlButton[device[s]->getStopButton()]->readButtonState());
                                
                                handleStopEvent(taskTime, s, device[s]->getStopEvent());
                            }
                            break;
                            
                        case FOLLOW:                                                                            // device shall stop when a device reached a certain position
                            if (device[device[s]->getStopTriggerDevice()]->getTaskPointer() > device[s]->getStopTriggerTask()) {          //  trigger device on trigger move?
                                handleStopEvent(taskTime, s, device[s]->getStopEvent());
                            }
                            break;
                            
                        case POSITION:                                                                          // device shall stop when a device reached a certain position
                            if (device[s]->getStopTriggerTask() == device[device[s]->getStopTriggerDevice()]->getTaskPointer()) {          //  trigger device on trigger move?
                                if ((device[device[s]->getStopTriggerDevice()]->getDirectionDown() && device[device[s]->getStopTriggerDevice()]->getCurrentPosition() <= device[s]->getStopTriggerPosition()) || (!device[device[s]->getStopTriggerDevice()]->getDirectionDown() && device[device[s]->getStopTriggerDevice()]->getCurrentPosition() >= device[s]->getStopTriggerPosition())) {                                                // trigger position reached?
                                    handleStopEvent(taskTime, s, device[s]->getStopEvent());
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }                                                                                               // (device[s]->getState() == MOVING)
                else {                                                                                          // if device is stopped
                    
                    if (device[s]->getState() == MOVE_DONE) {                                                   // finished right now?
                        device[s]->finishTask();
                        device[s]->disableDevice();
                        
                        if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                            Serial.print(taskTime);
                            Serial.print(F(": "));
                            Serial.print(device[s]->getDeviceName());
                            Serial.print(F(" Task "));
                            Serial.print(device[s]->getTaskPointer());
                            Serial.println(F(" done"));
                        }
                        
                        device[s]->increaseTaskPointer();                                                       // go for next job!
                        
                        if (device[s]->getTaskPointer() < device[s]->getCountOfTasks()) {                       //  all tasks done? no!
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
                    else {    //  (device[s]->getState() == MOVE_DONE)                                          //  device is idle
                        switch (device[s]->getStartEvent()) {                                                   //  what kind of startEvent is given?
                            case DATE:                                                                          //  start the next move by date
                                if (taskTime > device[s]->getStartTime()) {
                                    handleStartEvent(taskTime, s, device[s]->getStartEvent());
                                }
                                break;
                                
                            case BUTTON:                                                                        //  start the next move by button
                                if ((controlButton[device[s]->getStartButton()]->readButtonState()) == true) {
                                    handleStartEvent(taskTime, s, device[s]->getStartEvent());
                                }
                                break;
                                
                            case FOLLOW:                                                                        //  start the next move when a device reached a certain
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
                                break;
                            default:
                                break;
                       }
                    }
                }
            }
            ongoingOperations += device[s]->getState();                                                           // ongoing operations on any device?
            
        }

        //        if (taskTime - lastPrintTime > 1000) {
        //            device[6]->getReadOut(0);
        //            lastPrintTime = taskTime;
        //        }
        
        if (theButton < countOfControlButtons) {
            if (controlButton[theButton]->readButtonState()) {
//                Serial.print(controlButton[theButton]->isActiv());
                for (unsigned char theAction = 0; theAction < controlButton[theButton]->getCountOfActions(); theAction++) {
                    if (!controlButton[theButton]->getAction(theAction).actionDone) {
                        if (controlButton[theButton]->getAction(theAction).validTask == device[controlButton[theButton]->getAction(theAction).targetDevice]->getTaskPointer()) {
                            
                            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(controlButton[theButton]->getButtonName());
                                Serial.print(F(" rise Action "));
                                Serial.print(theAction);
                                Serial.print(F(", pin 45: "));
                                Serial.print(digitalRead(45));
                                Serial.print(F(", pin 47: "));
                                Serial.print(digitalRead(47));
                                Serial.print(F(", pin 49: "));
                                Serial.println(digitalRead(49));
                            }

                            device[controlButton[theButton]->getAction(theAction).targetDevice]->setTaskPointer(controlButton[theButton]->getAction(theAction).followingTask - 1);
                            switch (controlButton[theButton]->getAction(theAction).targetAction) {
                                case START:
                                    handleStartEvent(taskTime, controlButton[theButton]->getAction(theAction).targetDevice, CONTROLBUTTON);
                                    break;
                                case STOP_AND_SWITCH:
                                    device[controlButton[theButton]->getAction(theAction).targetDevice]->setSwitchTaskPromptly(true);
                                case STOP:
                                    device[controlButton[theButton]->getAction(theAction).targetDevice]->setStopping(STOP_NORMAL);
                                    handleStopEvent(taskTime, controlButton[theButton]->getAction(theAction).targetDevice, CONTROLBUTTON);
                                    break;
                                case STOP_SHARP_AND_SWITCH:
                                    device[controlButton[theButton]->getAction(theAction).targetDevice]->setSwitchTaskPromptly(true);
                                case STOP_SHARP:
                                    device[controlButton[theButton]->getAction(theAction).targetDevice]->setStopping(STOP_IMMEDIATELY);
                                    handleStopEvent(taskTime, controlButton[theButton]->getAction(theAction).targetDevice, CONTROLBUTTON);
                            }
                            controlButton[theButton]->setActionDone(theAction);
                        }
                    }
                }
            }
            theButton++;
        } else {
            theButton = 0;
        }
        
        
        loopCounter++;
        
       
    } while (ongoingOperations > 0);
    
    for (unsigned char s = 0; s < countOfDevices; s++) {
        if (device[s]->getCountOfTasks() > 0) {                                                      // (== MOVING || MOVE_DONE || PENDING_MOVES)
            device[s]->disableDevice();
            device[s]->detachDevice();
        }
    }
    
   
    
    Serial.println();
    Serial.print(F("loops: "));
    Serial.print((signed long)loopCounter);
    Serial.print(F(", elapsed time: "));
    Serial.print((int)((millis() - taskStartTime) / 1000));
    Serial.print(F(", loops/second: "));
    Serial.print(1000.0 * loopCounter / (millis() - taskStartTime));
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
    
    if (device[s]->getStartDelay() > 0) {                                               // startDelay given?
        if (startEvent == DATE) {
            device[s]->setStartTime(device[s]->getStartTime() + device[s]->getStartDelay());
        } else {
            device[s]->setStartTime(taskTime + device[s]->getStartDelay());             // so start the move by date
            device[s]->setStartEvent(DATE);
        }
        device[s]->setStartDelay(0);
    }
    else {
        device[s]->setStartTime(taskTime);
        device[s]->enableDevice();
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
    
    if (device[s]->getSwitchTaskPromptly()) {                                           // switch immediately to next move?
        device[s]->increaseTaskPointer();                                               // go for next job! (if existing)
        if (device[s]->getTaskPointer() < device[s]->getCountOfTasks()) {               //  all tasks done? no!
            device[s]->prepareNextTask();
        } else {
            device[s]->stopTask();
        }
    } else {                                                                            // just stop. but how?
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








