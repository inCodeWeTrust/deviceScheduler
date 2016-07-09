//
//  CCDeviceScheduler.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//



#include "CCDeviceScheduler.h"





CCDeviceScheduler::CCDeviceScheduler(String schedulerName) {
    this->schedulerName = schedulerName;
    this->countOfDevices = 0;
    this->countOfControlButtons = 0;

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
//    for (int i = countOfControlButtons - 1; i >= 0; i--) {
//        delete controlButton[i];
//        controlButton[i] = NULL;
//    }

    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: destructed scheduler "));
        Serial.println(schedulerName);
    }
    
}


CCDevice* CCDeviceScheduler::addDcController(String deviceName, unsigned char switching_pin, boolean switchingPin_activ) {
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
    
    return device[countOfDevices - 1];
}
CCDevice* CCDeviceScheduler::addDcController_fullBridge(String deviceName, unsigned char switching_A_pin, boolean switchingPin_A_activ, unsigned char switching_B_pin, boolean switchingPin_B_activ) {
    
    device[countOfDevices] = new CCDcControllerDevice_fullBridge(deviceName, switching_A_pin, switchingPin_A_activ, switching_B_pin, switchingPin_B_activ);
    
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
        Serial.print(F(": CCDcControllerDevice_fullBridge constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return device[countOfDevices - 1];
}


CCDevice* CCDeviceScheduler::addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition) {
    
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
    
    return device[countOfDevices - 1];
}
CCDevice* CCDeviceScheduler::addServoWithCounterServo(String deviceName, unsigned char servo_00_pin, int minPosition_00, int midPosition_00, int maxPosition_00, unsigned char servo_01_pin, int minPosition_01, int midPosition_01, int maxPosition_01, int parkPosition) {
    
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
    
    return device[countOfDevices - 1];
}


CCDevice* CCDeviceScheduler::addStepper_A4988(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, unsigned int stepsPerRotation) {
    
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
    
    return device[countOfDevices - 1];
}


CCDevice* CCDeviceScheduler::addStepper_TMC260(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char chipSelect_pin, unsigned int currentMax, unsigned int stepsPerRotation) {
    
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
    
    return device[countOfDevices - 1];

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
        Serial.println(getNameOfDeviceType(device[i]->getType()));
    }
    Serial.println();
}


CCControlButton* CCDeviceScheduler::addControlButton(String buttonName, unsigned char button_pin, boolean buttonActiv, boolean pullup) {

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
    
    return controlButton[countOfControlButtons - 1];

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
        Serial.println(controlButton[i]->getButtonActiv());
    }
    Serial.println();
}





int CCDeviceScheduler::run(CCWorkflow* currentWorkflow) {
    
    unsigned long loopCounter = 0;
    unsigned char ongoingOperations = 0;
    boolean breakLoop = false;
    notificationCode = 0;
    notificationText = "";
    
    // prepare the loop
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.println(F(": prepare run ..."));
    }
    for (unsigned char df = 0; df < currentWorkflow->countOfDeviceFlows; df++) {
        currentDeviceFlow = currentWorkflow->deviceFlow[df];
        currentDeviceFlow->setTaskPointer(0);
        
        currentDevice = currentDeviceFlow->device;
        currentDevice->setState(PENDING_MOVES);
        currentDevice->attachDevice();
        
        //	prepare first Tasks
        currentDevice->prepareTask(currentDeviceFlow->task[0]);
        
        
        if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
            Serial.print(currentWorkflow->workflowName);
            Serial.print(F("->"));
            Serial.print(currentDeviceFlow->deviceFlowName);
            Serial.print(F(": setup first Task: current Position: "));
            Serial.print(currentDevice->getCurrentPosition());
            Serial.print(F(", target: "));
            Serial.println(currentDevice->getTarget());
        }
        delay(200);
    }
    
    for (schedulerControlButton b = 0; b < countOfControlButtons; b++) {
        controlButton[b]->readButtonState();
    }

    
    //  start the workflow
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": run workflow "));
        Serial.println(currentWorkflow->workflowName);
    }

    unsigned long taskTime, loopTime;
    unsigned long taskStartTime = millis();
    
    //    unsigned long lastPrintTime = taskStartTime;

    schedulerControlButton theButton = 0;
    
    do {
        loopTime = millis();
        taskTime = loopTime - taskStartTime;
        
        ongoingOperations = 0;
        
       
        for (unsigned char df = 0; df < currentWorkflow->countOfDeviceFlows; df++) {
            currentDeviceFlow = currentWorkflow->deviceFlow[df];
            currentDevice = currentDeviceFlow->device;
            currentTask = currentDeviceFlow->task[currentDeviceFlow->taskPointer];

            if (currentDevice->getState() > SLEEPING) {                                                             // (== MOVING || MOVE_DONE || PENDING_MOVES)
                if (currentDevice->getState() == MOVING) {                                                          // if device is moving...
                    currentDevice->operateTask();                              // so: move on!
                    
                    switch (currentDevice->getStopEvent()) {                                                        // is there a stopEvent defined?
                        case DATE:                                                                              // yes, device shall stop by date
                            if (taskTime > currentDevice->getStartTime() + currentDevice->getStartDelay() + currentDevice->getTimeout()) { // it's time to stop!
                                handleStopEvent(taskTime, currentDeviceFlow);
                            }
                            break;
                            
                        case BUTTON:                                                                            // device shall stop by button
                            if (controlButton[currentTask->getStopButton()]->readIfActiv()) {                         // it's time to stop!
                                handleStopEvent(taskTime, currentDeviceFlow);
                            }
                            break;
                            
                        case FOLLOW:                                                                            // device shall stop when a device reached a certain position
                            if (currentTask->getStopTriggerDevice()->currentTaskID > currentTask->getStopTriggerTaskID()) {          //  trigger device on trigger move?
                                handleStopEvent(taskTime, currentDeviceFlow);
                            }
                            break;
                            
                        case POSITION:                                                                          // device shall stop when a device reached a certain position
                            triggerDevice_stop = currentTask->getStopTriggerDevice();
                            if (triggerDevice_stop->currentTaskID >= currentTask->getStopTriggerTaskID()) {          //  trigger device on trigger move?
                                if ((triggerDevice_stop->getDirectionDown() && triggerDevice_stop->getCurrentPosition() <= currentTask->getStopTriggerPosition()) || (!triggerDevice_stop->getDirectionDown() && triggerDevice_stop->getCurrentPosition() >= currentTask->getStopTriggerPosition())) {                                                // trigger position reached?
                                    handleStopEvent(taskTime, currentDeviceFlow);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }                                                                                               // (currentDevice->getState() == MOVING)
                else {                                                                                          // if device is stopped
                    
                    if (currentDevice->getState() == MOVE_DONE) {                                                   // finished right now?
                        currentDevice->finishTask();
                        currentDevice->disableDevice();
                        
                        if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                            Serial.print(taskTime);
                            Serial.print(F(": "));
                            Serial.print(currentDeviceFlow->deviceFlowName);
                            Serial.print(F(" Task "));
                            Serial.print(currentDeviceFlow->getTaskPointer());
                            Serial.println(F(" done"));
                        }
                        
                        currentDeviceFlow->increaseTaskPointer();                                                       // go for next job!
                        
                        if (currentDeviceFlow->getTaskPointer() < currentDeviceFlow->getCountOfTasks()) {                       //  all tasks done? no!
                            currentDevice->setState(PENDING_MOVES);
                            currentDevice->prepareTask(currentDeviceFlow->task[currentDeviceFlow->taskPointer]);
                            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(currentDeviceFlow->deviceFlowName);
                                Serial.print(F(" prepare Task "));
                                Serial.print(currentDeviceFlow->getTaskPointer());
                                Serial.print(F(": current Position: "));
                                Serial.print(currentDevice->getCurrentPosition());
                                Serial.print(F(" target: "));
                                Serial.println(currentDevice->getTarget());
                            }
                            
                        }
                        else {                                                                                  // all tasks are done
                            currentDevice->setState(SLEEPING);
                            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(currentDeviceFlow->deviceFlowName);
                                Serial.println(F(" finished all Tasks"));
                            }
                        }
                    }
                    else {    //  (currentDevice->getState() == MOVE_DONE)                                          //  device is idle
                        switch (currentDevice->getStartEvent()) {                                                   //  what kind of startEvent is given?
                            case DATE:                                                                          //  start the next move by date
                                if (taskTime > currentDevice->getStartTime()) {
                                    handleStartEvent(taskTime, currentDeviceFlow);
                                }
                                break;
                                
                            case BUTTON:                                                                        //  start the next move by button
                                if (controlButton[currentTask->getStartButton()]->readIfActiv()) {
                                    handleStartEvent(taskTime, currentDeviceFlow);
                                }
                                break;
                                
                            case FOLLOW:                                                                        //  start the next move when a device reached a certain
                                if (currentTask->getStartTriggerDevice()->currentTaskID > currentTask->getStartTriggerTaskID()) {          //  is the trigger servo doing the trigger move?
                                
                                    handleStartEvent(taskTime, currentDeviceFlow);
                                }
                                break;
                                
                            case POSITION:                                                                      //  start the next move when a device reached a certain
                                triggerDevice_start = currentTask->getStartTriggerDevice();
                                if (triggerDevice_start->currentTaskID >= currentTask->getStartTriggerTaskID()) {        //  is the trigger servo doing the trigger move?
                                    
                                    if ((triggerDevice_start->getDirectionDown() && triggerDevice_start->getCurrentPosition() <= currentTask->getStartTriggerPosition()) || (!triggerDevice_start->getDirectionDown() && triggerDevice_start->getCurrentPosition() >= currentTask->getStartTriggerPosition())) {
                                        //  did the trigger servo pass the trigger position?
                                        handleStartEvent(taskTime, currentDeviceFlow);
                                    }
                                }
                                break;
                            default:
                                break;
                       }
                    }
                }
            }
            ongoingOperations += currentDevice->getState();                                                           // ongoing operations on any device?
            
        }

        //        if (taskTime - lastPrintTime > 1000) {
        //            device[6]->getReadOut(0);
        //            lastPrintTime = taskTime;
        //        }
        
        if (theButton < countOfControlButtons) {
            currentButton = controlButton[theButton];
            if (currentButton->readIfActiv()) {
                for (unsigned char theAction = 0; theAction < currentButton->getCountOfActions(); theAction++) {
                    currentAction = currentButton->action[theAction];
                    if (!currentAction->actionDone) {
                        if ((currentAction->validTaskID == -1) || (currentAction->validTaskID == currentAction->targetDeviceFlow->getTaskPointer())) {
                            
                            notificationCode = min(notificationCode, currentAction->notificationCode);
                            notificationText = notificationText + currentAction->notificationText + "\n";

                            if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(currentButton->getButtonName());
                                Serial.print(F(" rise Action "));
                                Serial.println(theAction);
                                Serial.print(F(" with notification: "));
                                Serial.print(currentAction->notificationCode);
                                Serial.print(F(" --> "));
                                Serial.println(currentAction->notificationText);
                            }

                            if (currentAction->targetAction == BREAK_LOOP) {
                                breakLoop = true;
                                break;
                            }
                            currentAction->targetDeviceFlow->setTaskPointer(currentAction->followingTaskID - 1);
                            switch (currentAction->targetAction) {
                                case START_NEXT_TASK:
                                    handleStartEvent(taskTime, currentAction->targetDeviceFlow);
                                    break;
                                case STOP_TASK_AND_SWITCH:
                                    currentAction->targetDeviceFlow->device->setSwitchTaskPromptly(true);
                                case STOP_TASK:
                                    currentAction->targetDeviceFlow->device->setStopping(STOP_NORMAL);
                                    handleStopEvent(taskTime, currentAction->targetDeviceFlow);
                                    break;
                                case STOP_TASK_SHARP_AND_SWITCH:
                                    currentAction->targetDeviceFlow->device->setSwitchTaskPromptly(true);
                                case STOP_TASK_SHARP:
                                    currentAction->targetDeviceFlow->device->setStopping(STOP_IMMEDIATELY);
                                    handleStopEvent(taskTime, currentAction->targetDeviceFlow);
                            }
                            currentAction->actionDone = true;
                        }
                    }
                }
            }
            theButton++;
        } else {
            theButton = 0;
        }
        
        
        if (breakLoop) {
            break;
        }


        loopCounter++;
        
       
    } while (ongoingOperations > 0);

    
    for (unsigned char df = 0; df < currentWorkflow->countOfDeviceFlows; df++) {
        currentDevice = currentWorkflow->deviceFlow[df]->device;
            currentDevice->disableDevice();
            currentDevice->detachDevice();
    }
    
    if (breakLoop) {
        Serial.print(F("scheduler canceled"));
    } else {
        Serial.print(F("scheduler ended normaly:"));
    }
    Serial.print(F("notificationCode:"));
    Serial.println(notificationCode);
    Serial.print(F("notificationText:"));
    Serial.println(notificationText);
    Serial.println();
    Serial.print(F("loops: "));
    Serial.print((signed long)loopCounter);
    Serial.print(F(", elapsed time: "));
    Serial.print((int)((millis() - taskStartTime) / 1000));
    Serial.print(F(", loops/second: "));
    Serial.print(1000.0 * loopCounter / (millis() - taskStartTime));
    Serial.println();
    Serial.println();

    return notificationCode;
}


void CCDeviceScheduler::handleStartEvent(unsigned long taskTime, CCDeviceFlow* currentDeviceFlow) {
    
    currentDevice = currentDeviceFlow->device;
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
        Serial.print(taskTime);
        Serial.print(F(": "));
        Serial.print(currentDeviceFlow->deviceFlowName);
        Serial.print(F(" start task "));
        Serial.print(currentDeviceFlow->getTaskPointer());
        Serial.print(" by ");
        Serial.println(getNameOfTaskEvent(currentDevice->getStartEvent()));
    }
    
    if (currentDevice->getStartDelay() > 0) {                                               // startDelay given?
        if (currentDevice->getStartEvent() == DATE) {
            currentDevice->setStartTime(currentDevice->getStartTime() + currentDevice->getStartDelay());
        } else {
            currentDevice->setStartTime(taskTime + currentDevice->getStartDelay());             // so start the move by date
            currentDevice->setStartEvent(DATE);
        }
        currentDevice->setStartDelay(0);
    }
    else {
        currentDevice->setStartTime(taskTime);
        currentDevice->enableDevice();
        currentDevice->startTask();
    }
}
void CCDeviceScheduler::handleStopEvent(unsigned long taskTime, CCDeviceFlow* currentDeviceFlow) {
    
    currentDevice = currentDeviceFlow->device;
    
    if (DEVICESCHEDULER_VERBOSE & DEVICESCHEDULER_SHOW_TASK_VIEW) {
        Serial.print(taskTime);
        Serial.print(F(": "));
        Serial.print(currentDeviceFlow->deviceFlowName);
        Serial.print(F(" stop task "));
        Serial.print(currentDeviceFlow->getTaskPointer());
        Serial.print(" by ");
        Serial.print(getNameOfTaskEvent(currentDevice->getStopEvent()));
        Serial.print(", switchTaskPromptly: ");
        Serial.print(currentDevice->getSwitchTaskPromptly());
        Serial.print(", stopping: ");
        Serial.println(getNameOfStoppingMode(currentDevice->getStopping()));
}
    
    if (currentDevice->getSwitchTaskPromptly()) {                                           // switch immediately to next move?
        currentDeviceFlow->increaseTaskPointer();                                               // go for next job! (if existing)
        if (currentDeviceFlow->getTaskPointer() < currentDeviceFlow->getCountOfTasks()) {               //  all tasks done? no!
            currentDevice->prepareTask(currentDeviceFlow->task[currentDeviceFlow->taskPointer]);
        } else {
            currentDevice->stopTask();
        }
    } else {                                                                            // just stop. but how?
        if (currentDevice->getStopping() == STOP_IMMEDIATELY) {
            currentDevice->stopTask();
        } else {
            currentDevice->initiateStop();
            currentDevice->setStopEvent(NONE);
        }
    }
}

void CCDeviceScheduler::getAllTasksOfAllDeviceFlowsOfWorkflow(CCWorkflow* workflow) {
    Serial.print(F("workflow "));
    Serial.println(workflow->workflowName);
    for (unsigned char df = 0; df < workflow->countOfDeviceFlows; df++) {
        Serial.print(F("   deviceFlow "));
        Serial.print(workflow->deviceFlow[df]->deviceFlowName);
        Serial.print(F(": with device: "));
        Serial.print(workflow->deviceFlow[df]->device->getDeviceName());
        Serial.print(F(", defaultVelocity: "));
        Serial.print(workflow->deviceFlow[df]->defaultVelocity);
        Serial.print(F(", defaultAcceleration: "));
        Serial.print(workflow->deviceFlow[df]->defaultAcceleration);
        Serial.print(F(", defaultDeceleration: "));
        Serial.print(workflow->deviceFlow[df]->defaultDeceleration);
        Serial.print(F(": tasks: "));
        Serial.println(workflow->deviceFlow[df]->countOfTasks);
        
        for (int i = 0; i < workflow->deviceFlow[df]->getCountOfTasks(); i++) {
            Serial.print(F("      # "));
            Serial.print(i);
            Serial.print(F(": target: "));
            Serial.print(workflow->deviceFlow[df]->task[i]->getTarget());
            Serial.print(F(", velocity: "));
            Serial.print(workflow->deviceFlow[df]->task[i]->getVelocity());
            Serial.print(F(", acceleration: "));
            Serial.print(workflow->deviceFlow[df]->task[i]->getAcceleration());
            if (workflow->deviceFlow[df]->task[i]->getMoveRelativ()) {
                Serial.print(F(", move relativ"));
            }
            if (workflow->deviceFlow[df]->task[i]->getWithPositionReset()) {
                Serial.print(F(", with position reset"));
            }
            Serial.print(F(", startDelay: "));
            Serial.print(workflow->deviceFlow[df]->task[i]->getStartDelay());
            Serial.print(F(", started by "));
            Serial.print(getNameOfTaskEvent(workflow->deviceFlow[df]->task[i]->getStartEvent()));
            switch (workflow->deviceFlow[df]->task[i]->getStartEvent()) {
                case DATE:
                    Serial.print(F(", startTime: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTime());
                    break;
                case BUTTON:
                    Serial.print(F(", startButton: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartButton());
                    //                Serial.print(controlButton[deviceFlow[df]->task[i]->getStartButton()]->getButtonName());
                    //                Serial.print(F(", at state: "));
                    //                Serial.print(controlButton[deviceFlow[df]->task[i]->getStartButton()]->getButtonActiv());
                    break;
                case FOLLOW:
                    Serial.print(F(", of: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTriggerDevice()->getDeviceName());
                    Serial.print(F(", after move: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTriggerTaskID());
                    break;
                case POSITION:
                    Serial.print(F(", of: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTriggerDevice()->getDeviceName());
                    Serial.print(F(", on move: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTriggerTaskID());
                    Serial.print(F(", at position: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTriggerPosition());
                    break;
                default:
                    Serial.print(F(", no start event given"));
            }
            Serial.print(F(", terminated by: "));
            Serial.print(getNameOfTaskEvent(workflow->deviceFlow[df]->task[i]->getStopEvent()));
            if ((workflow->deviceFlow[df]->task[i]->getStopEvent()) > 0) {
                Serial.print(", stopping: ");
                Serial.print(getNameOfStoppingMode(workflow->deviceFlow[df]->task[i]->getStopping()));
                
                switch (workflow->deviceFlow[df]->task[i]->getStopEvent()) {
                    case DATE:
                        Serial.print(F(", timeout: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getTimeout());
                        break;
                    case BUTTON:
                        Serial.print(F(", stopButton: "));
                        //                    Serial.print(controlButton[deviceFlow[df]->task[i]->getStopButton()]->getButtonName());
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopButton());
                        //                    Serial.print(F(", at state: "));
                        //                    Serial.print(controlButton[deviceFlow[df]->task[i]->getStopButton()]->getButtonActiv());
                        break;
                    case FOLLOW:
                        Serial.print(F(", of: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopTriggerDevice()->getDeviceName());
                        Serial.print(F(", after move: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopTriggerTaskID());
                        break;
                    case POSITION:
                        Serial.print(F(", of: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopTriggerDevice()->getDeviceName());
                        Serial.print(F(", on move: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopTriggerTaskID());
                        Serial.print(F(", at position: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopTriggerPosition());
                        break;
                    default:
                        Serial.print(F(", no stop event given"));
                }
                
                if (workflow->deviceFlow[df]->task[i]->getSwitchTaskPromptly()) {
                    Serial.print(F(" --> switch promptly to next move"));
                }
            }
            Serial.println();
            
        }
        Serial.println();
    }
}



/*
void CCWorkflow::getAllTasksForAllDeviceFlows() {
    for (unsigned char df = 0; df < countOfDeviceFlows; df++) {
        getTasksForDeviceFlow(df);
    }
}

void CCWorkflow::deleteAllDeviceFlows() {
    for (int i = 0; i < countOfDeviceFlows; i++) {
        delete deviceFlow[i];
        deviceFlow[i] = NULL;
    }
}
*/



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
    if (d == START_NEXT_TASK) return "start";
    if (d == STOP_TASK_AND_SWITCH) return "stop and switch";
    if (d == STOP_TASK) return "stop";
    if (d == STOP_TASK_SHARP_AND_SWITCH) return "stop immediately and switch";
    if (d == STOP_TASK_SHARP) return "stop immediately";
    return "unknown";
}








