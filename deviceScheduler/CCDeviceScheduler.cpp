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
    
    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: setup scheduler "));
        Serial.print(schedulerName);
        if (SCHEDULER_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(" at $"));
            Serial.println((long)this, HEX);
        }
        Serial.println();
    }
}

CCDeviceScheduler::~CCDeviceScheduler() {    
    for (int i = countOfDevices - 1; i >= 0; i--) {
        if (SCHEDULER_VERBOSE & BASICOUTPUT) {
            Serial.print(F("[CCDeviceScheduler]: "));
            Serial.print(schedulerName);
            Serial.print(F(": delete device: "));
            Serial.println(device[i]->getName());
        }
        delete device[i];
        device[i] = NULL;
    }
    
    for (int i = countOfControlButtons - 1; i >= 0; i--) {
        if (SCHEDULER_VERBOSE & BASICOUTPUT) {
            Serial.print(F("[CCDeviceScheduler]: "));
            Serial.print(schedulerName);
            Serial.print(F(": delete button: "));
            Serial.println(controlButton[i]->getName());
        }
        delete controlButton[i];
        controlButton[i] = NULL;
    }

    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.println();
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.println(F(": destructed"));
    }
    
}


CCDevice* CCDeviceScheduler::addDcController(String deviceName, unsigned char switching_pin, bool switchingPin_activ) {
    device[countOfDevices] = new CCDcControllerDevice(deviceName, switching_pin, switchingPin_activ);
    
    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getName());
    }
    if (SCHEDULER_VERBOSE & MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCDcControllerDevice constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }

    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return device[countOfDevices - 1];
}
CCDevice* CCDeviceScheduler::addDcController_fullBridge(String deviceName, unsigned char switching_A_pin, bool switchingPin_A_activ, unsigned char switching_B_pin, bool switchingPin_B_activ) {
    
    device[countOfDevices] = new CCDcControllerDevice_fullBridge(deviceName, switching_A_pin, switchingPin_A_activ, switching_B_pin, switchingPin_B_activ);
    
    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getName());
    }
    if (SCHEDULER_VERBOSE & MEMORYDEBUG) {
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
    
    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getName());
    }
    if (SCHEDULER_VERBOSE & MEMORYDEBUG) {
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
    
    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getName());
    }
    if (SCHEDULER_VERBOSE & MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCServoDevice constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return device[countOfDevices - 1];
}


CCDevice* CCDeviceScheduler::addStepper_A4988(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned int stepsPerRotation, unsigned char microStep_00_pin, unsigned char microStep_01_pin, unsigned char microStep_02_pin, signed char steppingCode_00, signed char steppingCode_01, signed char steppingCode_02, signed char steppingCode_03, signed char steppingCode_04, signed char steppingCode_05, signed char steppingCode_06, signed char steppingCode_07) {

    device[countOfDevices] = new CCStepperDevice_A4988(deviceName, step_pin, dir_pin, enable_pin, stepsPerRotation, microStep_00_pin, microStep_01_pin, microStep_02_pin, steppingCode_00, steppingCode_01, steppingCode_02, steppingCode_03, steppingCode_04, steppingCode_05, steppingCode_06, steppingCode_07);
    
    
    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getName());
    }
    if (SCHEDULER_VERBOSE & MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCStepperDevice_A4988 constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }


    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return device[countOfDevices - 1];
}


CCDevice* CCDeviceScheduler::addStepper_TMC260(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned int stepsPerRotation, unsigned char chipSelect_pin, unsigned int currentMax) {
    
    device[countOfDevices] = new CCStepperDevice_TMC260(deviceName, step_pin, dir_pin, enable_pin, stepsPerRotation, chipSelect_pin, currentMax);
    
    
    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided "));
        Serial.print(getNameOfDeviceType(device[countOfDevices]->getType()));
        Serial.print(F(": "));
        Serial.println(device[countOfDevices]->getName());
    }
    if (SCHEDULER_VERBOSE & MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCStepperDevice_TMC260 constructed at $"));
        Serial.println((long)device[countOfDevices], HEX);
    }
    
    countOfDevices++;
    //	Device index = countOfDevices - 1 [8 Devices: index of first: 0, last: 7]
    
    return device[countOfDevices - 1];

}


void CCDeviceScheduler::listDevices() {
    Serial.print(F("[CCDeviceScheduler]: "));
    Serial.print(schedulerName);
    Serial.println(F(": Devices: "));
    for (int i = 0; i < countOfDevices; i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(", name: "));
        Serial.print(device[i]->getName());
        Serial.print(F(", type: "));
        Serial.println(getNameOfDeviceType(device[i]->getType()));
    }
    Serial.println();
}


CCControlButton* CCDeviceScheduler::addControlButton(String buttonName, unsigned char button_pin, bool buttonActiv, bool pullup) {

    controlButton[countOfControlButtons] = new CCControlButton(countOfControlButtons, buttonName, button_pin, buttonActiv, pullup);
    
    if (SCHEDULER_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": provided controlButton "));
        Serial.println(controlButton[countOfControlButtons]->getName());
    }
    if (SCHEDULER_VERBOSE & MEMORYDEBUG) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": CCDcControlButton constructed at $"));
        Serial.println((long)controlButton[countOfControlButtons], HEX);
    }

    countOfControlButtons++;
    //	control button index = countOfControlButtons - 1 [8 buttons: index of first: 0, last: 7]
    
    return controlButton[countOfControlButtons - 1];

}

void CCDeviceScheduler::listControlButtons() {
    Serial.print(F("[CCDeviceScheduler]: "));
    Serial.print(schedulerName);
    Serial.println(F(": ControlButtons: "));
    for (int i = 0; i < countOfControlButtons; i++) {
        Serial.print(F("   # "));
        Serial.print(i);
        Serial.print(F(", name: "));
        Serial.print(controlButton[i]->getName());
        Serial.print(F(", activ: "));
        Serial.println(controlButton[i]->getButtonActiv());
    }
    Serial.println();
}


void CCDeviceScheduler::reviewTasks(CCWorkflow* currentWorkflow) {
    for (unsigned char df = 0; df < currentWorkflow->getCountOfDeviceFlows(); df++) {
        for (unsigned char t = 0; t < currentWorkflow->deviceFlow[df]->getCountOfTasks(); t++) {
            
            infoCode i = currentWorkflow->deviceFlow[df]->device->reviewValues(currentWorkflow->deviceFlow[df]->task[t]);

            Serial.print(currentWorkflow->getName());
            Serial.print(F(" > "));
            Serial.print(currentWorkflow->deviceFlow[df]->getName());
            Serial.print(F(" task #"));
            Serial.print(t);
            Serial.print(F(": "));
            Serial.println(getLiteralOfWorkflowInfo(i));
        }
    }
}



int CCDeviceScheduler::run(CCWorkflow* currentWorkflow) {
    
    unsigned long loopCounter = 0;
    unsigned char ongoingOperations = 0;
    bool breakLoop = false;
    notificationCode = 0;
    notificationText = "";
    
    
    if ((char)currentWorkflow->getWorkflowInfo() >= 0) {
        
    // prepare the loop
    
    if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.println(F(": prepare run ..."));
    }
    for (unsigned char df = 0; df < currentWorkflow->getCountOfDeviceFlows(); df++) {
        currentDeviceFlow = currentWorkflow->deviceFlow[df];
        currentDeviceFlow->setTaskPointer(0);
        
        currentDevice = currentDeviceFlow->device;
        currentDevice->setState(PENDING_MOVES);
        currentDevice->attachDevice();
        
        //	prepare first Tasks
        currentDevice->prepareTask(currentDeviceFlow->task[0]);
        
        
        if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
            Serial.print(currentWorkflow->getName());
            Serial.print(F("->"));
            Serial.print(currentDeviceFlow->getName());
            Serial.print(F(": setup first Task: current Position: "));
            Serial.print(currentDevice->getCurrentPosition());
            Serial.print(F(", target: "));
            Serial.println(currentDevice->getTarget());
        }
        delay(200);
    }
    
    for (unsigned char fc = 0; fc < currentWorkflow->getCountOfFlowControls(); fc++) {
        currentWorkflow->flowControl[fc]->button->readButtonState();
    }

    
    //  start the workflow
    if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
        Serial.print(F("[CCDeviceScheduler]: "));
        Serial.print(schedulerName);
        Serial.print(F(": run workflow "));
        Serial.println(currentWorkflow->getName());
    }

    unsigned long taskTime, loopTime;
    unsigned long taskStartTime = millis();
    
    //    unsigned long lastPrintTime = taskStartTime;

    schedulerControlButton theButtonID = 0;
    
    do {
        loopTime = millis();
        taskTime = loopTime - taskStartTime;

        ongoingOperations = 0;
        
       
        for (unsigned char df = 0; df < currentWorkflow->getCountOfDeviceFlows(); df++) {
            currentDeviceFlow = currentWorkflow->deviceFlow[df];
            currentDevice = currentDeviceFlow->device;
            currentTask = currentDeviceFlow->task[currentDeviceFlow->getTaskPointer()];

//            Serial.print(currentDeviceFlow->getName());
//            Serial.println(currentDevice->getName());
            
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
                            if (currentTask->getStopButton()->readIfActiv()) {                         // it's time to stop!
                                handleStopEvent(taskTime, currentDeviceFlow);
                            }
                            break;
                            
                        case FOLLOW:                                                                            // device shall stop when a device reached a certain position
                            if (currentTask->getStopTriggerDevice()->getCurrentTaskID() > currentTask->getStopTriggerTaskID()) {          //  trigger device on trigger move?
                                handleStopEvent(taskTime, currentDeviceFlow);
                            }
                            break;
                            
                        case POSITION:                                                                          // device shall stop when a device reached a certain position
                            triggerDevice_stop = currentTask->getStopTriggerDevice();
                            if (triggerDevice_stop->getCurrentTaskID() >= currentTask->getStopTriggerTaskID()) {          //  trigger device on trigger move?
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
                        
                        if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                            Serial.print(taskTime);
                            Serial.print(F(": "));
                            Serial.print(currentDeviceFlow->getName());
                            Serial.print(F(" Task "));
                            Serial.print(currentDeviceFlow->getTaskPointer());
                            Serial.println(F(" done"));
                        }
                        
                        currentDeviceFlow->increaseTaskPointer();                                                       // go for next job!
                        
                        if (currentDeviceFlow->getTaskPointer() < currentDeviceFlow->getCountOfTasks()) {                       //  all tasks done? no!
                            currentDevice->setState(PENDING_MOVES);
                            currentDevice->prepareTask(currentDeviceFlow->task[currentDeviceFlow->getTaskPointer()]);
                            if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(currentDeviceFlow->getName());
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
                            if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(currentDeviceFlow->getName());
                                Serial.println(F(" finished all Tasks"));
                            }
                        }
                    }
                    else {    //  (currentDevice->getState() == MOVE_DONE)                                          //  device is idle
                        
//                        Serial.print(taskTime);
//                        Serial.print(": ");
//                        Serial.print(currentDevice->getStartEvent() == DATE);
//                        Serial.print(": ");
//                        Serial.println(currentDevice->getStartTime());
//                        

                        
                        switch (currentDevice->getStartEvent()) {                                                   //  what kind of startEvent is given?
                            case DATE:                                                                          //  start the next move by date
                                if (taskTime > currentDevice->getStartTime()) {
                                    handleStartEvent(taskTime, currentDeviceFlow);
                                }
                                break;
                                
                            case BUTTON:                                                                        //  start the next move by button
                                if (currentTask->getStartButton()->readIfActiv()) {
                                    handleStartEvent(taskTime, currentDeviceFlow);
                                }
                                break;
                                
                            case FOLLOW:                                                                        //  start the next move when a device reached a certain
                                if (currentTask->getStartTriggerDevice()->getCurrentTaskID() > currentTask->getStartTriggerTaskID()) {          //  is the trigger servo doing the trigger move?
                                
                                    handleStartEvent(taskTime, currentDeviceFlow);
                                }
                                break;
                                
                            case POSITION:                                                                      //  start the next move when a device reached a certain
                                triggerDevice_start = currentTask->getStartTriggerDevice();
                                if (triggerDevice_start->getCurrentTaskID() >= currentTask->getStartTriggerTaskID()) {        //  is the trigger servo doing the trigger move?
                                    
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

        if (theButtonID < currentWorkflow->getCountOfFlowControls()) {
            currentFlowControl = currentWorkflow->flowControl[theButtonID];
            if (currentFlowControl->button->readIfActiv()) {
                for (unsigned char theActionID = 0; theActionID < currentFlowControl->getCountOfActions(); theActionID++) {
                    currentAction = currentFlowControl->action[theActionID];
                    if (!currentAction->getActionDone()) {
                        if ((currentAction->getValidTaskID() == -1) || (currentAction->getValidTaskID() == currentAction->targetDeviceFlow->getTaskPointer())) {
                            
                            currentWorkflow->setWorkflowInfo(currentAction->getWorkflowInfoCode());

//                            notificationCode = min(notificationCode, currentAction->notificationCode);
//                            notificationText = notificationText + currentAction->notificationText + "\n";

                            if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
                                Serial.print(taskTime);
                                Serial.print(F(": "));
                                Serial.print(currentButton->getName());
                                Serial.print(F(" rise Action "));
                                Serial.println(theActionID);
//                                Serial.print(F(" with notification: "));
//                                Serial.print(currentAction->notificationCode);
//                                Serial.print(F(" --> "));
//                                Serial.println(currentAction->notificationText);
                            }

                            if (currentAction->getTargetAction() == BREAK_LOOP) {
                                breakLoop = true;
                                break;
                            }
                            
                            if (currentAction->getFollowingTaskID() > -1) {
                                currentAction->targetDeviceFlow->setTaskPointer(currentAction->getFollowingTaskID() - 1);
                                switch (currentAction->getTargetAction()) {
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
                                    default:
                                        break;
                                }
                            } else {
                                if (currentAction->getValidTaskID() > -1) {
                                    currentWorkflow->setWorkflowInfo(WORKFLOW_CANCELED_ON_FLOWCONTROL_ERROR);
                                    breakLoop = true;
                                    break;
                                }
                            }
                            currentAction->setActionDone(true);
                        }
                    }
                }
            }
            theButtonID++;
        } else {
            theButtonID = 0;
        }
        
        
        if (breakLoop) {
            break;
        }

        
        loopCounter++;
        
       
    } while (ongoingOperations > 0);

    
    for (unsigned char df = 0; df < currentWorkflow->getCountOfDeviceFlows(); df++) {
        currentDevice = currentWorkflow->deviceFlow[df]->device;
            currentDevice->disableDevice();
            currentDevice->detachDevice();
    }
    
    if (breakLoop) {
        Serial.println(F("workflow canceled"));
    } else {
        Serial.println(F("workflow ended normaly:"));
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

    } else {
        Serial.println(F("workflow was disabled"));
        
    }
    
    
    return notificationCode;
}


void CCDeviceScheduler::handleStartEvent(unsigned long taskTime, CCDeviceFlow* currentDeviceFlow) {
    
    currentDevice = currentDeviceFlow->device;
    
    if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
        Serial.print(taskTime);
        Serial.print(F(": "));
        Serial.print(currentDeviceFlow->getName());
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
    
    if (SCHEDULER_VERBOSE & SHOW_TASK_VIEW) {
        Serial.print(taskTime);
        Serial.print(F(": "));
        Serial.print(currentDeviceFlow->getName());
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
            currentDevice->prepareTask(currentDeviceFlow->task[currentDeviceFlow->getTaskPointer()]);
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

void CCDeviceScheduler::listAllTasks(CCWorkflow* workflow) {
    Serial.print(F("workflow "));
    Serial.println(workflow->getName());
    for (unsigned char df = 0; df < workflow->getCountOfDeviceFlows(); df++) {
        Serial.print(F("   deviceFlow "));
        Serial.print(workflow->deviceFlow[df]->getName());
        Serial.print(F(": with device: "));
        Serial.print(workflow->deviceFlow[df]->device->getName());
        Serial.print(F(", defaultVelocity: "));
        Serial.print(workflow->deviceFlow[df]->getDefaultVelocity());
        Serial.print(F(", defaultAcceleration: "));
        Serial.print(workflow->deviceFlow[df]->getDefaultAcceleration());
        Serial.print(F(", defaultDeceleration: "));
        Serial.print(workflow->deviceFlow[df]->getDefaultDeceleration());
        Serial.print(F(": tasks: "));
        Serial.println(workflow->deviceFlow[df]->getCountOfTasks());
        
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
            if (workflow->deviceFlow[df]->task[i]->getPositionReset() == RESET_ON_START) {
                Serial.print(F(", with position reset on start"));
            } else if (workflow->deviceFlow[df]->task[i]->getPositionReset() == RESET_ON_COMPLETION) {
                Serial.print(F(", with position reset on completion"));
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
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartButton()->getName());
                Serial.print(F(", at state: "));
                Serial.print(workflow->deviceFlow[df]->task[i]->getStartButton()->getButtonActiv());
                    break;
                case FOLLOW:
                    Serial.print(F(", of: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTriggerDevice()->getName());
                    Serial.print(F(", after move: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTriggerTaskID());
                    break;
                case POSITION:
                    Serial.print(F(", of: "));
                    Serial.print(workflow->deviceFlow[df]->task[i]->getStartTriggerDevice()->getName());
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
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopButton()->getName());
                        Serial.print(F(", at state: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopButton()->getButtonActiv());
                        break;
                    case FOLLOW:
                        Serial.print(F(", of: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopTriggerDevice()->getName());
                        Serial.print(F(", after move: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopTriggerTaskID());
                        break;
                    case POSITION:
                        Serial.print(F(", of: "));
                        Serial.print(workflow->deviceFlow[df]->task[i]->getStopTriggerDevice()->getName());
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
void CCDeviceScheduler::listAllActions(CCWorkflow* workflow) {
    Serial.print(F("workflow "));
    Serial.println(workflow->getName());
    for (unsigned char fc = 0; fc < workflow->getCountOfFlowControls(); fc++) {
        Serial.print(F("   flowControl "));
        Serial.print(workflow->flowControl[fc]->getName());
        Serial.print(F(": with device: "));
        Serial.print(workflow->flowControl[fc]->button->getName());
        Serial.print(F(": actions: "));
        Serial.println(workflow->flowControl[fc]->getCountOfActions());
        
        for (int i = 0; i < workflow->flowControl[fc]->getCountOfActions(); i++) {
            Serial.print(F("      # "));
            Serial.print(i);
            Serial.print(F(": action: "));
            Serial.print(workflow->flowControl[fc]->action[i]->getName());
            Serial.print(F(", on deviceFlow: "));
            if (workflow->flowControl[fc]->action[i]->targetDeviceFlow == NULL) {
                Serial.print(F(" any "));
            } else {
                Serial.print(workflow->flowControl[fc]->action[i]->targetDeviceFlow->getName());
            }
            Serial.print(F(", on task: "));
            if (workflow->flowControl[fc]->action[i]->getValidTaskID() < 0) {
                Serial.print(F(" any "));
            } else {
                Serial.print(workflow->flowControl[fc]->action[i]->getValidTaskID());
            }
            Serial.print(F(", following: "));
            if (workflow->flowControl[fc]->action[i]->getFollowingTaskID() < 0) {
                Serial.print(F(" any "));
            } else {
                Serial.print(workflow->flowControl[fc]->action[i]->getFollowingTaskID());
            }
            Serial.print(F(", do: "));
            Serial.print(getNameOfDeviceAction(workflow->flowControl[fc]->action[i]->getTargetAction()));
            Serial.print(F(", infoCode: ["));
            Serial.print(getLiteralOfWorkflowInfo(workflow->flowControl[fc]->action[i]->getWorkflowInfoCode()));
            Serial.println(F("]"));
            
        }
        Serial.println();
    }
}

void CCDeviceScheduler::evaluateButtons() {
    bool lastButtonState[20];
    Serial.print(F("************************************* evaluate buttons *************************************"));
    Serial.print(F("[CCDeviceScheduler]: "));
    Serial.print(schedulerName);
    Serial.println(F(": ControlButtons: "));
    for (int i = 0; i < countOfControlButtons; i++) {
        Serial.print(controlButton[i]->getName());
        Serial.print(F(", activ at: "));
        Serial.print(controlButton[i]->getButtonActiv());
        Serial.print(F(", current: "));
        controlButton[i]->readButtonState();
        Serial.print(getNameOfButtonState(controlButton[i]->getState()));
        Serial.print(F(", now activ: "));
        if (controlButton[i]->readIfActiv()) {
            Serial.println(F("yes"));
        } else {
            Serial.println(F("no"));
        }
        Serial.println();
        
        lastButtonState[i] = controlButton[i]->getState();
    }
    
    while (digitalRead(EVALUATE_BUTTONS_SWITCH) == LOW) {
        for (int i = 0; i < countOfControlButtons; i++) {
            controlButton[i]->readButtonState();
            bool buttonState = controlButton[i]->getState();
            if (buttonState != lastButtonState[i]) {
                Serial.print(controlButton[i]->getName());
                Serial.print(F(", activ at: "));
                Serial.print(controlButton[i]->getButtonActiv());
                Serial.print(F(", current: "));
                controlButton[i]->readButtonState();
                Serial.print(getNameOfButtonState(controlButton[i]->getState()));
                Serial.print(F(", now activ: "));
                if (controlButton[i]->readIfActiv()) {
                    Serial.println(F("yes"));
                } else {
                    Serial.println(F("no"));
                }
                lastButtonState[i] = buttonState;
            }
        }
    }
}


/*
 void CCWorkflow::getAllTasksForAllDeviceFlows() {
 for (unsigned char df = 0; df < getCountOfDeviceFlows(); df++) {
        getTasksForDeviceFlow(df);
    }
}

void CCWorkflow::deleteAllDeviceFlows() {
    for (int i = 0; i < getCountOfDeviceFlows(); i++) {
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
String CCDeviceScheduler::getNameOfButtonState(bool s) {
    if (s) return "HIGH";
    return "LOW";
}
String CCDeviceScheduler::getNameOfDeviceAction(deviceAction d) {
    if (d == BREAK_LOOP) return "break loop";
    if (d == START_NEXT_TASK) return "start";
    if (d == STOP_TASK_AND_SWITCH) return "stop and switch";
    if (d == STOP_TASK) return "stop";
    if (d == STOP_TASK_SHARP_AND_SWITCH) return "stop immediately and switch";
    if (d == STOP_TASK_SHARP) return "stop immediately";
    return "unknown";
}
String CCDeviceScheduler::getLiteralOfWorkflowInfo(infoCode i) {
    if (i == WORKFLOW_CANCELED_ON_ERROR) return "workflow canceled on error";
    if (i == WORKFLOW_CANCELED_ON_ENDBUTTON_REACHED) return "workflow canceled on endpoint reached";
    if (i == WORKFLOW_CANCELED_ON_BUTTON_NOT_REACHED) return "workflow canceled on target not reached";
    if (i == WORKFLOW_CANCELED_ON_PARAMETER_ERROR) return "workflow canceled on parameter error";
    if (i == WORKFLOW_DISABLED_ON_ERROR) return "workflow disabled on error";
    if (i == WORKFLOW_DISABLED_ON_ENDBUTTON_REACHED) return "workflow disabled on endpoint reached";
    if (i == WORKFLOW_DISABLED_ON_BUTTON_NOT_REACHED) return "workflow disabled on target not reached";
    if (i == EVERYTHING_OK) return "everything ok";
    if (i == SONGCANCELBUTTON_PRESSED) return "songCancelButtonPressed";
    if (i == SONGENDBUTTON_PRESSED) return "songEndButtonPressed";
    if (i == BUTTON_NOT_REACHED) return "target not reached";
    if (i == ENDBUTTON_REACHED) return "endpoint reached";
    return "unknown";
}






