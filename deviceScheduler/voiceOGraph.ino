//
//  deviceScheduler.ino
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//



//  #define ARDUINO_SIMULATION


#ifdef ARDUINO_SIMULATION
#include "SIMPatchRunner.h"
#else
#include <Arduino.h>
#include <Servo.h>
#include <malloc.h>
#endif




#include "voiceOGraph.h"
#include "CCDeviceScheduler.h"





// ------------- globals -----------------------------------------------------------------------------------------------------

/// The Scheduler and it's device array

CCDeviceScheduler   *scheduler;




/// parameters for cutting

unsigned long       catSongStartPosition, catSongEndPosition, catCuttingEndPosition;
float               catMotorSpeed_startGroove, catMotorSpeed_song, catMotorSpeed_endGroove;
unsigned long       grooves_all;
float               turnTableStepperDegrees, turnTableStepperSpeed;



// ------------- prototypes --------------------------------------------------------------------------------------------------
void setup();
void calculateCuttingParameters();
void evaluateButtons();
void freeRam ();

#ifndef ARDUINO_SIMULATION
extern char _end;
extern "C" char *sbrk(int i);
#endif



// ------------- main loop --------------------------------------------------------------------------------------------------
void loop() {
    
    Serial.println(), Serial.println(), Serial.println();
    for (int i = 0; i < 56; i++) Serial.print(F("#"));
    Serial.println(), Serial.println(), Serial.println();
    
    
//    unsigned int k = -15;
    
//    k++;
    
    
    
    
    freeRam();
    
    // ================================================================================================================================
    // ============= create scheduler object: ==========================================================================
    // ================================================================================================================================
    
    scheduler = new CCDeviceScheduler("scheduler");
    
    
    //============================================================================================================================
    // ============= register devices: ================================================================================================
    // ============================================================================================================================
    
    CCDevice* liftServo = scheduler->addServoWithCounterServo(SERVO_LIFT_NAME,
                                                              SERVO_LIFT_LEFT_PIN,
                                                              SERVO_LIFT_LEFT_MIN_POSITION,
                                                              SERVO_LIFT_LEFT_MID_POSITION,
                                                              SERVO_LIFT_LEFT_MAX_POSITION,
                                                              SERVO_LIFT_RIGHT_PIN,
                                                              SERVO_LIFT_RIGHT_MIN_POSITION,
                                                              SERVO_LIFT_RIGHT_MID_POSITION,
                                                              SERVO_LIFT_RIGHT_MAX_POSITION,
                                                              LIFT_PARK_POSITION);
    
    
    CCDevice* turnServo = scheduler->addServo(SERVO_TURN_NAME,
                                              SERVO_TURN_PIN,
                                              SERVO_TURN_MIN_POSITION,
                                              SERVO_TURN_MAX_POSITION,
                                              TURN_STOCK_POSITION);
    
    CCDevice* turnMotor = scheduler->addDcController_fullBridge("turnMotor", 39, HIGH, 41, HIGH);
    
    
    CCDevice* pumpServo = scheduler->addServo(SERVO_PUMP_NAME,
                                              SERVO_PUMP_PIN,
                                              SERVO_PUMP_MIN_POSITION,
                                              SERVO_PUMP_MAX_POSITION,
                                              PUMP_PARK_POSITION);
    
    
    CCDevice* headLeftServo = scheduler->addServo(SERVO_HEAD_LEFT_NAME,
                                                  SERVO_HEAD_LEFT_PIN,
                                                  SERVO_HEAD_LEFT_MIN_POSITION,
                                                  SERVO_HEAD_LEFT_MAX_POSITION,
                                                  HEAD_LEFT_PARK_POSITION);
    
    CCDevice* headRightServo = scheduler->addServo(SERVO_HEAD_RIGHT_NAME,
                                                   SERVO_HEAD_RIGHT_PIN,
                                                   SERVO_HEAD_RIGHT_MIN_POSITION,
                                                   SERVO_HEAD_RIGHT_MAX_POSITION,
                                                   HEAD_RIGHT_PARK_POSITION);
    
    
    CCDevice* stockStepper = scheduler->addStepper_A4988(STEPPER_STOCK_NAME,
                                                         STEPPER_STOCK_STEP_PIN,
                                                         STEPPER_STOCK_DIR_PIN,
                                                         STEPPER_STOCK_ENABLE_PIN,
                                                         STEPPER_STOCK_STEPS_PER_ROTATION,
                                                         STEPPER_STOCK_MICROSTEPPIN_00,
                                                         STEPPER_STOCK_MICROSTEPPIN_01,
                                                         STEPPER_STOCK_MICROSTEPPIN_02,
                                                         STEPPER_STOCK_STEPMODECODE_00,
                                                         STEPPER_STOCK_STEPMODECODE_01,
                                                         STEPPER_STOCK_STEPMODECODE_02,
                                                         STEPPER_STOCK_STEPMODECODE_03,
                                                         STEPPER_STOCK_STEPMODECODE_04);
    
    
    CCDevice* catStepper = scheduler->addStepper_TMC260(STEPPER_CAT_NAME,
                                                        STEPPER_CAT_STEP_PIN,
                                                        STEPPER_CAT_DIR_PIN,
                                                        STEPPER_CAT_ENABLE_PIN,
                                                        STEPPER_CAT_STEPS_PER_ROTATION,
                                                        STEPPER_CAT_CHIPSELECT_PIN,
                                                        STEPPER_CAT_CURRENT);
    
    
    CCDevice* tableStepper = scheduler->addStepper_A4988(STEPPER_TABLE_NAME,
                                                         STEPPER_TABLE_STEP_PIN,
                                                         STEPPER_TABLE_DIR_PIN,
                                                         STEPPER_TABLE_ENABLE_PIN,
                                                         STEPPER_TABLE_STEPS_PER_ROTATION,
                                                         STEPPER_TABLE_MICROSTEPPIN_00,
                                                         STEPPER_TABLE_MICROSTEPPIN_01,
                                                         STEPPER_TABLE_MICROSTEPPIN_02,
                                                         STEPPER_TABLE_STEPMODECODE_00,
                                                         STEPPER_TABLE_STEPMODECODE_01,
                                                         STEPPER_TABLE_STEPMODECODE_02,
                                                         STEPPER_TABLE_STEPMODECODE_03,
                                                         STEPPER_TABLE_STEPMODECODE_04,
                                                         STEPPER_TABLE_STEPMODECODE_05);
    
    
    
    CCDevice* conveyStepper = scheduler->addStepper_A4988(STEPPER_CONVEYOR_NAME,
                                                          STEPPER_CONVEYOR_STEP_PIN,
                                                          STEPPER_CONVEYOR_DIR_PIN,
                                                          STEPPER_CONVEYOR_ENABLE_PIN,
                                                          STEPPER_CONVEYOR_STEPS_PER_ROTATION,
                                                          STEPPER_CONVEYOR_MICROSTEPPIN_00,
                                                          STEPPER_CONVEYOR_MICROSTEPPIN_01,
                                                          STEPPER_CONVEYOR_MICROSTEPPIN_02,
                                                          STEPPER_CONVEYOR_STEPMODECODE_00,
                                                          STEPPER_CONVEYOR_STEPMODECODE_01,
                                                          STEPPER_CONVEYOR_STEPMODECODE_02,
                                                          STEPPER_CONVEYOR_STEPMODECODE_03,
                                                          STEPPER_CONVEYOR_STEPMODECODE_04);
    
    
    CCDevice* vacuumCleaner = scheduler->addDcController(VACCUUMCLEANER_NAME,
                                                         VACCUUMCLEANER_PIN,
                                                         VACCUUMCLEANER_ACTIVE);
    
    
    CCDevice* startingSoonLamp = scheduler->addDcController(CONTROLLER_LAMP_RED_NAME,
                                                            CONTROLLER_LAMP_RED_PIN,
                                                            CONTROLLER_LAMP_RED_ON);
    
    
    
    //============================================================================================================================
    // ============= register sensors: ================================================================================================
    // ============================================================================================================================
    
    
    
    CCControl* recordAvailableButton = scheduler->addControlButton(RECORDAVAILABLE_BUTTON_NAME,
                                                                   RECORDAVAILABLE_BUTTON_PIN,
                                                                   RECORDAVAILABLE_BUTTON_PULLUP);
    
    CCControl* stockTopButton = scheduler->addControlButton(STOCKTOP_BUTTON_NAME,
                                                            STOCKTOP_BUTTON_PIN,
                                                            STOCKTOP_BUTTON_PULLUP);
    
    CCControl* stockBottomButton = scheduler->addControlButton(STOCKBOTTOM_BUTTON_NAME,
                                                               STOCKBOTTOM_BUTTON_PIN,
                                                               STOCKBOTTOM_BUTTON_PULLUP);
    
    CCControl* bridgeParkButton = scheduler->addControlButton(CAT_PARKBUTTON_NAME,
                                                              CAT_PARKBUTTON_PIN,
                                                              CAT_PARKBUTTON_PULLUP);
    
    CCControl* bridgeEndButton = scheduler->addControlButton(CAT_ENDBUTTON_NAME,
                                                             CAT_ENDBUTTON_PIN,
                                                             CAT_ENDBUTTON_PULLUP);
    
    CCControl* songEndButton = scheduler->addControlButton(SONG_ENDBUTTON_NAME,
                                                           SONG_ENDBUTTON_PIN,
                                                           SONG_ENDBUTTON_PULLUP);
    
    CCControl* songCancelButton = scheduler->addControlButton(SONG_CANCELBUTTON_NAME,
                                                              SONG_CANCELBUTTON_PIN,
                                                              SONG_CANCELBUTTON_PULLUP);
    
    CCControl* headInclinationSensor = scheduler->addControlSensor(HEAD_INCLINATION_SENSOR_NAME,
                                                                   HEAD_INCLINATION_SENSOR_PIN);
    
    CCControl* armTurnSensor = scheduler->addControlSensor(ARM_TURN_SENSOR_NAME,
                                                           ARM_TURN_SENSOR_PIN);
    
    
    
    scheduler->listDevices();
    scheduler->listControls();
    
    delay(1000);
    Serial.println("now the work flowing");
    
    
    // =================================================================================================================================================
    // =================================================================================================================================================
    // ============= initialisation workflow ===========================================================================================================
    // =================================================================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* initTheMachine = new CCWorkflow("initMachine");
    
    {
        // ============= devices of initMachine =======================================================================================
        
        CCDeviceFlow* turnMotorFlow = initTheMachine->addDeviceFlow("turnMotorFlow", turnMotor, 20, 2000);
        
        CCControl* turnControl = initTheMachine->addControl(armTurnSensor);
        
        // ============= tasks of initMachine =========================================================================================
        
        CCTask* driveLeft;
        driveLeft =turnMotorFlow->addTask(0.6);
        driveLeft->startByDate(1000);
        driveLeft->stopDynamicallyBySensor_new(turnControl, 300, 8000, 4, SKIP_APPROXIMATION_VERY_PRECISE);
        
        CCTask* driveRight;
        driveRight =turnMotorFlow->addTask(0.25);
        driveRight->startAfterCompletion();
        driveRight->setStartDelay(1000);
        driveRight->stopDynamicallyBySensor_new(turnControl, 200, 1000, 4, SKIP_APPROXIMATION_VERY_PRECISE);
        
        
        scheduler->reviewTasks(initTheMachine);
        scheduler->listAllTasks(initTheMachine);
     
        for (; ; ) {
            scheduler->run(initTheMachine);
        }

    }
    

    
    
/*
    {
        // ============= devices of initMachine =======================================================================================
        
        CCDeviceFlow* catStepperFlow = initTheMachine->addDeviceFlow("catStepperFlow", catStepper, CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
        CCDeviceFlow* headLeftServoFlow = initTheMachine->addDeviceFlow("headLeftServoFlow", headLeftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* headRightServoFlow = initTheMachine->addDeviceFlow("headRightServoFlow", headRightServo, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);

        CCControl* bridgeParkControl = initTheMachine->addControl(bridgeParkButton);
        
        // ============= tasks of initMachine =========================================================================================
        
        CCTask* driveHeadLeftToParkPosition;
        driveHeadLeftToParkPosition = headLeftServoFlow->addTask(HEAD_LEFT_PARK_POSITION);
        driveHeadLeftToParkPosition->startByDate(10);
        
        CCTask* driveHeadRightToParkPosition;
        driveHeadRightToParkPosition = headRightServoFlow->addTask(HEAD_RIGHT_PARK_POSITION);
        driveHeadRightToParkPosition->startByDate(20);
        
        CCTask* driveCatToParkPosition;
        driveCatToParkPosition = catStepperFlow->addTaskWithPositionResetOnCompletion(-400000);
        driveCatToParkPosition->startByDate(100);
        driveCatToParkPosition->stopByControl(bridgeParkControl, IS, CAT_PARKBUTTON_REACHED, STOP_NORMAL);
        
        scheduler->reviewTasks(initTheMachine);
        scheduler->listAllTasks(initTheMachine);
        
        scheduler->run(initTheMachine);
        catStepper->setCurrentPosition(0.0);
    }
  */
    
    
    
    
    // =================================================================================================================================================
    // =================================================================================================================================================
    // ============= fetchingRecord workflow ===========================================================================================================
    // =================================================================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* fetchingRecord = new CCWorkflow("fetchingRecord");
    
    
    {
        // ============= devices of fetchingRecord: =====================================================================================
        
        CCDeviceFlow* liftServoFlow = fetchingRecord->addDeviceFlow("liftServoFlow", liftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* turnServoFlow = fetchingRecord->addDeviceFlow("turnServoFlow", turnServo, TURN_SPEED_SLOW, TURN_ACCEL_SLOW);
        CCDeviceFlow* pumpServoFlow = fetchingRecord->addDeviceFlow("pumpServoFlow", pumpServo, PUMP_SPEED, PUMP_ACCEL);
        CCDeviceFlow* stockStepperFlow = fetchingRecord->addDeviceFlow("stockStepperFlow", stockStepper, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL);
        
        CCControl* recordAvailableControl = fetchingRecord->addControl(recordAvailableButton);
        CCControl* stockTopControl = fetchingRecord->addControl(stockTopButton);
        CCControl* stockBottomControl = fetchingRecord->addControl(stockBottomButton);

        CCFlowControl* stockTopFlowControl = fetchingRecord->addFlowControl("stockTopObserver", stockTopControl, IS, STOCKTOP_BUTTON_IS_REACHED);
        CCFlowControl* stockBottomFlowControl = fetchingRecord->addFlowControl("stockBottomObserver", stockBottomControl, IS, STOCKBOTTOM_BUTTON_IS_REACHED);
        
        
        // ============= tasks of fetchingRecord: =====================================================================================
        
        //  lift grappler
        CCTask* liftFromParkPosition;
        liftFromParkPosition = liftServoFlow->addTask(LIFT_PARK_POSITION + 500, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
        liftFromParkPosition->startByDate(100);
        
        //  move stock down first
        CCTask* moveStockStepperDown;
        moveStockStepperDown = stockStepperFlow->addTaskMoveRelativ(-4000);
        moveStockStepperDown->startByDate(200);
        moveStockStepperDown->stopByControl(recordAvailableControl, IS_NOT, RECORD_IS_AVAILABLE, STOP_NORMAL);
        
        //  supply a new record, terminated by recordAvailableButton
        CCTask* supplyRecord;
        supplyRecord = stockStepperFlow->addTaskMoveRelativ(4000);
        supplyRecord->startAfterCompletionOf(stockStepper, moveStockStepperDown);
        supplyRecord->stopByControl(recordAvailableControl, IS, RECORD_IS_AVAILABLE, STOP_NORMAL);
        
        //  lower grappler to stock: start when record is available
        CCTask* lowerToStock;
        lowerToStock = liftServoFlow->addTask(LIFT_STOCK_POSITION);
//        lowerToStock->startByControl(recordAvailableButton);
        lowerToStock->startAfterCompletionOf(stockStepper, moveStockStepperDown);
        
        //  pump: prepare
        CCTask* pumpForGrip_down;
        pumpForGrip_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
        pumpForGrip_down->startByTriggerpositionOf(liftServo, lowerToStock, LIFT_STOCK_POSITION + 60);
        
        //  pump: make vaccum
        CCTask* pumpForGrip_up;
        pumpForGrip_up = pumpServoFlow->addTask(PUMP_PARK_POSITION);
        pumpForGrip_up->startAfterCompletionOf(liftServo, lowerToStock);
        
        //  lift the new record
        CCTask* liftNewRecord;
        liftNewRecord = liftServoFlow->addTask(LIFT_UP_POSITION);
        liftNewRecord->startAfterCompletionOf(pumpServo, pumpForGrip_up);
        
        //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        CCTask* turnRecordToTable;
        turnRecordToTable = turnServoFlow->addTask(TURN_TABLE_POSITION);
        turnRecordToTable->startByTriggerpositionOf(liftServo, liftNewRecord, LIFT_UP_TRIGGER_TURN);
        
        //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
        CCTask* lowerRecordToTable = liftServoFlow->addTask(LIFT_TABLE_POSITION);
        lowerRecordToTable->startByTriggerpositionOf(turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
        
        //  release new record: release vacuum
        CCTask* pumpForRelease_down;
        pumpForRelease_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
        pumpForRelease_down->startAfterCompletionOf(pumpServo, pumpForGrip_up);
        pumpForRelease_down->startByTriggerpositionOf(liftServo, lowerRecordToTable, LIFT_TABLE_POSITION + 60);
        
        //  lift for going to park position: start when vacuum was released
        CCTask* liftForParkPosition;
        liftForParkPosition = liftServoFlow->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
        liftForParkPosition->startAfterCompletionOf(pumpServo, pumpForRelease_down);
        
        //  release pump when record is placed
        CCTask* pumpForRelease_up;
        pumpForRelease_up = pumpServoFlow->addTask(PUMP_PARK_POSITION);
        pumpForRelease_up->startAfterCompletionOf(liftServo, liftForParkPosition);
        
        //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        CCTask* turnToStockPosition;
        turnToStockPosition = turnServoFlow->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
        turnToStockPosition->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
        
        //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
        CCTask* lowerForParkPosition;
        lowerForParkPosition = liftServoFlow->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
        lowerForParkPosition->startByTriggerpositionOf(turnServo, turnToStockPosition, TURN_TO_STOCK_TRIGGER_LIFT);
        
        
        
        //  cancel loading if stockBottomButton is pressed while lowering
        
        CCAction* postWorkflowInfo = stockTopFlowControl->addAction("lastRecordInfo",  WORKFLOW_DISABLED_ON_ENDBUTTON_REACHED);
//        postWorkflowInfo->notificationCode = CONTINUE_ON_ENDBUTTON_REACHED;
//        postWorkflowInfo->notificationText = "running out of stock!\n";
        
        CCAction* stopStockAtBottom = stockBottomFlowControl->addAction("stopWhenStockBottomReached", WORKFLOW_CANCELED_ON_ENDBUTTON_REACHED);
        //        stopStockAtBottom->notificationCode = BREAK_ON_ENDBUTTON_REACHED;
        //        stopStockAtBottom->notificationText = "too many records in stock!\n";
        
        
        
        scheduler->reviewTasks(fetchingRecord);
        scheduler->listAllTasks(fetchingRecord);
        scheduler->listAllActions(fetchingRecord);
        
        
        freeRam();
    }
    
    
    
    
    
    // =================================================================================================================================================
    // =================================================================================================================================================
    // ============= cuttingProcess workflow ===========================================================================================================
    // =================================================================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* cuttingProcess = new CCWorkflow("cuttingProcess");
    
    
    {
        // ============= devices of cuttingProcess: =====================================================================================
        
        CCDeviceFlow* catStepperFlow = cuttingProcess->addDeviceFlow("catStepperFlow", catStepper, CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
        CCDeviceFlow* tableStepperFlow = cuttingProcess->addDeviceFlow("tableStepperFlow", tableStepper);
        CCDeviceFlow* startingSoonLampFlow = cuttingProcess->addDeviceFlow("startingSoonLampFlow", startingSoonLamp);
        CCDeviceFlow* headLeftServoFlow = cuttingProcess->addDeviceFlow("headLeftServoFlow", headLeftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* headRightServoFlow = cuttingProcess->addDeviceFlow("headRightServoFlow", headRightServo, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
        CCDeviceFlow* vacuumCleanerFlow = cuttingProcess->addDeviceFlow("vacuumCleanerFlow", vacuumCleaner, 1.0, 0.0);
        
        CCControl* headInclinationControl = cuttingProcess->addControl(headInclinationSensor);
        CCControl* bridgeParkControl = cuttingProcess->addControl(bridgeParkButton);
        CCControl* songEndControl = cuttingProcess->addControl(songEndButton);
        CCControl* songCancelControl = cuttingProcess->addControl(songCancelButton);
        
        CCFlowControl* songEndFlowControl = cuttingProcess->addFlowControl("songEndObserver", songEndControl, IS, SONG_ENDBUTTON_PRESSED);
        CCFlowControl* songCancelFlowControl = cuttingProcess->addFlowControl("songCancelControl", songCancelControl, IS, SONG_CANCELBUTTON_PRESSED);
        
        
        // ============= tasks of cuttingProcess: =====================================================================================
        
        //  (0) blink lamp for start cutting soon:
        CCTask* blinkForCuttingSoon;
        blinkForCuttingSoon = startingSoonLampFlow->addTask(0.5, 0.3, 0.0);
        blinkForCuttingSoon->startByDate(200);
        
        //  move to start groove:
        CCTask* driveToCuttingStartPosition;
        driveToCuttingStartPosition = catStepperFlow->addTask(CAT_CUTTING_START_POSITION);
        driveToCuttingStartPosition->startByDate(100);
        
        // switch on vacuum device
        CCTask* hooverTheFlake;
        hooverTheFlake = vacuumCleanerFlow->addTask(1.0);
        hooverTheFlake->startByDate(1500);
        
        //  turn the table:
        CCTask* turnTheTable;
        turnTheTable = tableStepperFlow->addTaskWithPositionReset(turnTableStepperDegrees + 17200.0, turnTableStepperSpeed, TABLE_STEP_ACCEL);
        turnTheTable->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
        
        //  (1) blink lamp for start cutting very soon:
        CCTask* blinkForCuttingVerySoon;
        blinkForCuttingVerySoon = startingSoonLampFlow->addTask(0.5, 1.0, 0.0);
        blinkForCuttingSoon->switchToNextTaskAfterCompletionOf(catStepper, driveToCuttingStartPosition);
        
        //  lower head close to record surface (left servo): start when reached start position of start groove
        CCTask* lowerHeadLeftForCutting;
        lowerHeadLeftForCutting = headLeftServoFlow->addTask(HEAD_LEFT_CUT_POSITION);
        lowerHeadLeftForCutting->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
        
        //  approximate head to record surface (right servo): start when left servo reached cutting position
        CCTask* approximateHeadRightForCutting;
        approximateHeadRightForCutting = headRightServoFlow->addTask(HEAD_RIGHT_CUT_POSITION);
        approximateHeadRightForCutting->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
        approximateHeadRightForCutting->stopDynamicallyBySensor(headInclinationControl, HEAD_INCLINATION_INIT_STOP, HEAD_INCLINATION_TARGET, 0.6, SKIP_APPROXIMATION_PRECISE);
        //        cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->stopDynamicallyBySensor(HEAD_INCLINATION_SENSOR, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
        
        //  (2) blink lamp for start cutting very very soon:
        CCTask* blinkForCuttingVeryVerySoon;
        blinkForCuttingVeryVerySoon = startingSoonLampFlow->addTask(0.5, 3.0, 0.0);
        blinkForCuttingVerySoon->switchToNextTaskAfterCompletionOf(headRightServo, approximateHeadRightForCutting);
        
        //  make start groove:
        CCTask* makeStartGroove;
        makeStartGroove = catStepperFlow->addTask(catCuttingEndPosition, catMotorSpeed_startGroove, CAT_ACCEL_SLOW);
//        makeStartGroove->startAfterCompletionOf(headRightServo, approximateHeadRightForCutting);
        makeStartGroove->startByControl(headInclinationControl, IS_SMALLER_THEN, HEAD_INCLINATION_START_CAT);
//        makeStartGroove->switchToNextTaskByTriggerpositionOf(catStepper, makeStartGroove, catSongStartPosition);
        makeStartGroove->switchToNextTaskAtPosition(catSongStartPosition);
        //  (3) keep blinking lamp on for whole cutting process:
        CCTask* keepLampOn;
        keepLampOn = startingSoonLampFlow->addTask(1.0, 1.0, 0.0);
        blinkForCuttingVeryVerySoon->switchToNextTaskAfterCompletionOf(catStepper, makeStartGroove);
        
        //  make song groove:
        CCTask* makeMainGroove;
        makeMainGroove = catStepperFlow->addTask(catCuttingEndPosition, catMotorSpeed_song, CAT_ACCEL_SLOW);
        makeMainGroove->switchToNextTaskAtPosition(catSongEndPosition);
        
        //  (4) blink lamp for finishing soon:
        CCTask* blinkForFinishingSoon;
        blinkForFinishingSoon = startingSoonLampFlow->addTask(0.9, 0.3, 0.0);
        keepLampOn->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition - 10000);
        
        //  (5) blink lamp for finishing very soon:
        CCTask* blinkForFinishingVerySoon;
        blinkForFinishingVerySoon = startingSoonLampFlow->addTask(0.9, 1.0, 0.0);
        blinkForFinishingSoon->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition - 5000);
        
        //  (5) blink lamp for finishing very soon:
        CCTask* blinkForFinishingVeryVerySoon;
        blinkForFinishingVeryVerySoon = startingSoonLampFlow->addTask(0.9, 3.0, 0.0);
        blinkForFinishingVerySoon->switchToNextTaskAfterCompletionOf(catStepper, makeMainGroove);
        
        //  make end groove:
        CCTask* makeEndGroove;
        makeEndGroove = catStepperFlow->addTask(catCuttingEndPosition, catMotorSpeed_endGroove, CAT_ACCEL_NORMAL);
        
        //  switch off blinking lamp:
        blinkForFinishingVeryVerySoon->stopAfterCompletionOf(catStepper, makeEndGroove, STOP_IMMEDIATELY);
        
        //  switch off vacuum cleaner
        hooverTheFlake->stopAfterCompletionOf(catStepper, makeEndGroove, STOP_IMMEDIATELY);
        
        //  lift head after cutting above the record surface:
        CCTask* liftHeadRightAfterCutting;
        liftHeadRightAfterCutting = headRightServoFlow->addTask(HEAD_RIGHT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW);
        liftHeadRightAfterCutting->startAfterCompletionOf(catStepper, makeEndGroove);
        
        //  lift head into park position:
        CCTask* liftHeadLeftAfterCutting;
        liftHeadLeftAfterCutting = headLeftServoFlow->addTask(HEAD_LEFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW);
        liftHeadLeftAfterCutting->startAfterCompletionOf(headRightServo, liftHeadRightAfterCutting);
        
        //  switch off turn table
        turnTheTable->stopAfterCompletionOf(headRightServo, liftHeadRightAfterCutting, STOP_NORMAL);
        
        //  drive cat back home
        CCTask* driveToParkPosition;
        driveToParkPosition = catStepperFlow->addTaskWithPositionResetOnCompletion(CAT_PARK_POSITION - 600);
        driveToParkPosition->startAfterCompletionOf(headRightServo, liftHeadRightAfterCutting);
        driveToParkPosition->stopByControl(bridgeParkControl, IS, CAT_PARKBUTTON_REACHED, STOP_NORMAL);
        
        
        
        //  jump over to end groove if songEndButton is pressed
        CCAction* songEndAction = songEndFlowControl->addAction("songEndAction", SONGENDBUTTON_PRESSED);
        songEndAction->evokeJumpToTask(catStepperFlow, makeMainGroove, SWITCH_PROMPTLY, makeEndGroove);
        
        //         cuttingProcess->controlButton[songEndButton]->evokeJumpToTask(catStepper, makeMainGroove, TASK_STOP_ACTION_IMMEDIATELY_AND_SWITCH, makeEndGroove, 32, "songEndButton was pressed");
        
        //  cancel cutting if songCancelButton is pressed
        CCAction* songCancelAction_cat = songCancelFlowControl->addAction("songCancelAction", SONGCANCELBUTTON_PRESSED);
        songCancelAction_cat->evokeJumpToTask(catStepperFlow, NULL, SWITCH_PROMPTLY, driveToParkPosition);

        CCAction* songCancelAction_leftHead = songCancelFlowControl->addAction("songCancelAction", SONGCANCELBUTTON_PRESSED);
        songCancelAction_leftHead->evokeJumpToTask(headLeftServoFlow, NULL, SWITCH_PROMPTLY, liftHeadLeftAfterCutting);

        CCAction* songCancelAction_rightHead = songCancelFlowControl->addAction("songCancelAction", SONGCANCELBUTTON_PRESSED);
        songCancelAction_rightHead->evokeJumpToTask(headRightServoFlow, NULL, SWITCH_PROMPTLY, liftHeadRightAfterCutting);

        
        
        scheduler->reviewTasks(cuttingProcess);
        scheduler->listAllTasks(cuttingProcess);
        scheduler->listAllActions(cuttingProcess);
        
        freeRam();
        
    }
    
    
    
    // =================================================================================================================================================
    // =================================================================================================================================================
    // ============= ejectingRecord workflow ===========================================================================================================
    // =================================================================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* ejectingRecord = new CCWorkflow("ejectingRecord");
    
    
    {
        // ============= devices of ejectingRecord ====================================================================================
        
        CCDeviceFlow* liftServoFlow = ejectingRecord->addDeviceFlow("liftServoFlow", liftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* turnServoFlow = ejectingRecord->addDeviceFlow("turnServoFlow", turnServo, TURN_SPEED_SLOW, TURN_ACCEL_SLOW);
        CCDeviceFlow* pumpServoFlow = ejectingRecord->addDeviceFlow("pumpServoFlow", pumpServo, PUMP_SPEED, PUMP_ACCEL);
        CCDeviceFlow* conveyStepperFlow = ejectingRecord->addDeviceFlow("conveyStepperFlow", conveyStepper, CONVEYOR_SPEED, CONVEYOR_ACCEL);
        
        
        // ============= tasks of ejectingRecord ======================================================================================
      
         //  remove record from turntable: started after cuttingProcess is finished
         
         // lift grappler: start when cuttingProcess is finished
        CCTask* liftFromParkPosition;
        liftFromParkPosition = liftServoFlow->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         liftFromParkPosition->startByDate(100);
         
         //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        CCTask* turnToTable;
        turnToTable = turnServoFlow->addTask(TURN_TABLE_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
         turnToTable->startByTriggerpositionOf(liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
         
         //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
        CCTask* lowerToTable;
        lowerToTable = liftServoFlow->addTask(LIFT_TABLE_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         lowerToTable->startByTriggerpositionOf(turnServo, turnToTable, TURN_TO_TABLE_TRIGGER_LIFT);
         
         // pump prepare
        CCTask* pumpForGrip_down;
        pumpForGrip_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
         pumpForGrip_down->startAfterCompletionOf(liftServo, lowerToTable);
         
         // pump make vaccum
        CCTask* pumpForGrip_up;
        pumpForGrip_up = pumpServoFlow->addTask(PUMP_PARK_POSITION);
         pumpForGrip_up->startAfterCompletionOf(pumpServo, pumpForGrip_down);
         
         //  lift the cutted record: start with startDelay after table was reached (LIFT_TABLE_POSITION)
        CCTask* liftCuttedRecord;
        liftCuttedRecord = liftServoFlow->addTask(LIFT_UP_POSITION);
         liftCuttedRecord->startAfterCompletionOf(pumpServo, pumpForGrip_up);
         
         //  turn grappler to conveyer position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        CCTask* turnToConveyor;
        turnToConveyor = turnServoFlow->addTask(TURN_CONVEYOR_POSITION);
         turnToConveyor->startByTriggerpositionOf(liftServo, liftCuttedRecord, LIFT_UP_TRIGGER_TURN);
         
         //  lower grappler to conveyer position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
        CCTask* lowerToConveyor;
        lowerToConveyor = liftServoFlow->addTask(LIFT_CONVEYER_POSITION);
         lowerToConveyor->startByTriggerpositionOf(turnServo, turnToConveyor, TURN_TO_CONVEYER_TRIGGER_LIFT);
         
         //  release new record, release vacuum
        CCTask* pumpForRelease_down;
        pumpForRelease_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
         pumpForRelease_down->startAfterCompletionOf(liftServo, lowerToConveyor);
         
         //  lift for parking
        CCTask* liftForParkPosition;
        liftForParkPosition = liftServoFlow->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         liftForParkPosition->startAfterCompletionOf(liftServo, lowerToConveyor);
         
         //  move it to the user
        CCTask* driveIt;
        driveIt = conveyStepperFlow->addTaskWithPositionReset(CONVEYOR_DISTANCE);
         driveIt->startAfterCompletionOf(liftServo, liftForParkPosition);
         
         //  release pump
        CCTask* pumpForRelease_up;
        pumpForRelease_up = pumpServoFlow->addTask(PUMP_PARK_POSITION);
         pumpForRelease_up->startAfterCompletionOf(liftServo, liftForParkPosition);
         
         //  turn grappler for parking
        CCTask* turnToStock;
        turnToStock = turnServoFlow->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
         turnToStock->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
         
         //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
        CCTask* lowerToPark;
        lowerToPark = liftServoFlow->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         lowerToPark->startByTriggerpositionOf(turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
         
         
        scheduler->reviewTasks(ejectingRecord);
        scheduler->listAllTasks(ejectingRecord);
        
        
        freeRam();
    }
    
    
    
    
    
    
    
    
    
    
    // ============================================================================================================================
    // ============================================================================================================================
    // ============= operation: ===================================================================================================
    // ============================================================================================================================
    // ============================================================================================================================
    
    
    Serial.println(F("ready to run!"));
    
    bool initNeeded = false;
    long blinkTimer = millis();
    while (!initNeeded) {
        if (digitalRead(FETCH_RECORD_BUTTON) == LOW) {
            Serial.println("go for a brand new record");
            
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
            
            scheduler->run(fetchingRecord);
            
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
            
        }
        
        
        if (digitalRead(START_CUTTING_BUTTON) == LOW) {
            Serial.println("go for cut and scratch");
            
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
            
            scheduler->run(cuttingProcess);
            
            scheduler->run(ejectingRecord);
            
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
            
        }
        
//        if (digitalRead(LOADING_BUTTON) == LOW) {
//            Serial.println("go for loading");
            /*
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
            
            {
                CCDeviceScheduler *loading = new CCDeviceScheduler("loading");
                
                
                
                unsigned int driveDown = loading->device[stockStepper]->addTask(-80000);
                loading->device[stockStepper]->task[driveDown]->startByDate(100);
                loading->device[stockStepper]->task[driveDown]->stopByControl(stockBottomButton);
                
                unsigned int comeUpAgain = loading->device[stockStepper]->addTask(80000);
                loading->device[stockStepper]->task[comeUpAgain]->startAfterCompletionOf(stockStepper, driveDown);
                loading->device[stockStepper]->task[comeUpAgain]->stopByControl(stockTopButton);
                
                loading->controlButton[loadingButton]->evokeJumpToNextTask(stockStepper, driveDown, TASK_STOP_ACTION);
                loading->controlButton[recordAvailableButton]->evokeJumpToNextTask(stockStepper, comeUpAgain, TASK_STOP_ACTION);
                
                loading->reviewTasks();
                loading->run();
                
                delete loading;
                loading = NULL;
            }
            
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
            */
//        }
        
//        if (digitalRead(MOVE_MANUALLY_SWITCH) == LOW) {
//            Serial.println("go for mamual moving");
            /*
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
            
            {
                CCDeviceScheduler *manualDrive = new CCDeviceScheduler("manualDrive");
                
                
                CCControl* catForwardButton = manualDrive->addControl("catForward", MOVE_CAT_FWD_BUTTON, HIGH, true);
                CCControl* catRewindButton = manualDrive->addControl("catRewind", MOVE_CAT_RWD_BUTTON, HIGH, true);
                
                
                //  move to start groove:
                unsigned int driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(CAT_CUTTING_START_POSITION);
                manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(100);
                
                manualDrive->reviewTasks();
                manualDrive->run();
                manualDrive->deleteAllTasks();
                
                while (digitalRead(START_CUTTING_BUTTON) == HIGH) {
                    if (digitalRead(MOVE_CAT_FWD_BUTTON) == LOW) {
                        driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(catCuttingEndPosition);
                        manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
                        manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByControl(catForwardButton, STOP_NORMAL);
                        
                        manualDrive->reviewTasks();
                        manualDrive->run();
                        manualDrive->deleteAllTasks();
                    }
                    if (digitalRead(MOVE_CAT_RWD_BUTTON) == LOW) {
                        driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(CAT_PARK_POSITION);
                        manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
                        manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByControl(catRewindButton, STOP_NORMAL);
                        
                        manualDrive->reviewTasks();
                        manualDrive->run();
                        manualDrive->deleteAllTasks();
                    }
                }
                
                
                cuttingProcess->device[0]->setCurrentPosition(manualDrive->device[0]->getCurrentPosition());
                cuttingProcess->device[0]->increaseTaskPointer();
                cuttingProcess->run();
                
                delete manualDrive;
                manualDrive = NULL;
                
            }
            
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
            */
//        }
        
//        if (digitalRead(MATCH_HEADIMPACT_SWITCH) == LOW) {
//            Serial.println("go for matching head impact");
            /*
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
            
            cuttingProcess->device[0]->setCountOfTasks(0);
            cuttingProcess->device[1]->setCountOfTasks(0);
            cuttingProcess->device[3]->task[1]->setStartDelay(3000);
            
            while (digitalRead(MATCH_HEADIMPACT_SWITCH == LOW)) {
                
                cuttingProcess->run();
                
                delay(3000);
                
            }
            
            initNeeded = true;
            
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
            */
//        }

//        if (digitalRead(EVALUATE_BUTTONS_SWITCH) == LOW) {
//            Serial.println("go for evaluatin the buttons");
//            scheduler->evaluateButtons();
//        }

        if (millis() - blinkTimer > 2000) {
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
            if (millis() - blinkTimer > 2200) {
                digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
                blinkTimer = millis();
                Serial.print(".");
            }
        }
    }
    
    
    Serial.println(F("done!"));

    delay(2000);
    
    
    delete fetchingRecord;
    fetchingRecord = NULL;
    
    delete cuttingProcess;
    cuttingProcess = NULL;
    
    delete ejectingRecord;
    ejectingRecord = NULL;
    

    delete scheduler;
    scheduler = NULL;
    
    freeRam();
    
    delay(2000);
    
}




/*
CCDeviceFlow* turnMotorFlow = cuttingProcess->addDeviceFlow("turnMotorFlow", turnMotor, 20, 200);

CCTask* turnRight;
turnRight = turnMotorFlow->addTask(1.0, 20, 8000, 8000);
turnRight->startByDate(100);
turnRight->stopByTimeout(10000, STOP_NORMAL);

CCTask* turnLeft;
turnLeft = turnMotorFlow->addTask(-1.0, 20, 8000, 8000);
turnLeft->startAfterCompletionOf(turnMotor, turnRight);
turnLeft->stopByTimeout(10000, STOP_NORMAL);

CCTask* turnRight1;
turnRight1 = turnMotorFlow->addTask(1.0, 20, 8000, 8000);
turnRight1->startAfterCompletionOf(turnMotor, turnLeft);
turnRight1->stopByTimeout(6000, STOP_IMMEDIATELY);

CCTask* turnLeft1;
turnLeft1 = turnMotorFlow->addTask(-1.0, 20, 8000, 8000);
turnLeft1->startAfterCompletionOf(turnMotor, turnRight1);
turnLeft1->stopByTimeout(3000, STOP_NORMAL);

CCTask* turnRecordToTable;
turnRecordToTable = turnMotorFlow->addTask(0.5);
turnRecordToTable->startAfterCompletionOf(turnMotor, turnLeft1);
turnRecordToTable->stopDynamicallyBySensor_new(A0, 300, 20, 2, SKIP_APPROXIMATION_VERY_PRECISE);
*/

void calculateCuttingParameters() {
    
    unsigned int songRange = CUTTING_RANGE - STARTGROOVE_RANGE - ENDGROOVE_RANGE_MIN;
    unsigned int songGrooves = songRange / (float)SONGGROOVE_PITCH;
    unsigned int playTime_seconds = songGrooves * 60 / RECORD_TURNS_PER_MINUTE;
    unsigned int endGrooveRange = ENDGROOVE_RANGE_MIN;
    
    
    
    Serial.println();
    Serial.println("################################################################################");
    Serial.println("############################## CUTTING PARAMETERS ##############################");
    Serial.println("################################################################################");
    Serial.println();
    
    Serial.print(F("groove pitch is set to "));
    Serial.print((float)SONGGROOVE_PITCH);
    Serial.print(F("mm, start groove pitch is set to "));
    Serial.print((float)STARTGROOVE_PITCH);
    Serial.print(F("mm, start groove width is set to "));
    Serial.print((float)STARTGROOVE_RANGE);
    Serial.print(F("mm, end groove pitch is set to "));
    Serial.print((float)ENDGROOVE_PITCH);
    Serial.print(F("mm, minimal end groove width is set to "));
    Serial.print((float)ENDGROOVE_RANGE_MIN);
    Serial.println(F("mm"));
    Serial.print(F("max. song width: "));
    Serial.print(songRange);
    Serial.print(F("mm, max. song grooves: "));
    Serial.print(songGrooves);
    Serial.print(F(", max. playing time: "));
    Serial.print(playTime_seconds / 60);
    Serial.print(F(":"));
    Serial.println(playTime_seconds % 60);
    
    
    if ((float)PLAYTIME_MINUTES * 60 < playTime_seconds) {
        songGrooves = PLAYTIME_MINUTES * RECORD_TURNS_PER_MINUTE;
        songRange = songGrooves * SONGGROOVE_PITCH;
        endGrooveRange = CUTTING_RANGE - songRange - STARTGROOVE_RANGE;
        playTime_seconds = PLAYTIME_MINUTES * 60;
        
        Serial.print(F("playing time is set to "));
        Serial.print(playTime_seconds / 60);
        Serial.print(F(":"));
        Serial.print(playTime_seconds % 60);
        Serial.print(F(", song width: "));
        Serial.print(songRange);
        Serial.print(F("mm, record grooves: "));
        Serial.println(songGrooves);
    } else {
        Serial.println(F("desired playing time is too long, using maximal playing time!"));
    }
    
    
    grooves_all = STARTGROOVE_RANGE / (float)STARTGROOVE_PITCH + songGrooves + endGrooveRange / (float)ENDGROOVE_PITCH;
    turnTableStepperDegrees = grooves_all * TABLE_DRIVE_RATIO * 360.0;
    //    turnTableStepperSpeed = RECORD_TURNS_PER_MINUTE / 60.0 * TABLE_DRIVE_RATIO * 360.0;
    turnTableStepperSpeed = 6.0 * RECORD_TURNS_PER_MINUTE * TABLE_DRIVE_RATIO;
    
    
    float catSpeed_start_mmPerSecond = STARTGROOVE_PITCH * RECORD_TURNS_PER_MINUTE / 60.0;
    float catMotorDegrees_start = 360.0 * STARTGROOVE_RANGE / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    catMotorSpeed_startGroove = 360.0 * catSpeed_start_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    
    float catSpeed_song_mmPerSecond = SONGGROOVE_PITCH * RECORD_TURNS_PER_MINUTE / 60.0;
    float catMotorDegrees_song = 360.0 * songRange / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    catMotorSpeed_song = 360.0 * catSpeed_song_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    
    float catSpeed_end_mmPerSecond = ENDGROOVE_PITCH * RECORD_TURNS_PER_MINUTE / 60.0;
    float catMotorDegrees_end = 360.0 * endGrooveRange / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    catMotorSpeed_endGroove = 360.0 * catSpeed_end_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    
    
    catSongStartPosition = CAT_CUTTING_START_POSITION + catMotorDegrees_start;
    catSongEndPosition = catSongStartPosition + catMotorDegrees_song;
    catCuttingEndPosition = CAT_CUTTING_START_POSITION + CUTTING_RANGE * 360.0 / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    
    
    Serial.print("Cutting the start groove: cutting progress [mm/sec]: ");
    Serial.print(catSpeed_start_mmPerSecond);
    Serial.print(", degrees: ");
    Serial.print(catMotorDegrees_start);
    Serial.print(", degrees per second: ");
    Serial.println(catMotorSpeed_startGroove);
    
    Serial.print("Cutting the song: cutting progress [mm/sec]: ");
    Serial.print(catSpeed_song_mmPerSecond);
    Serial.print(", degrees: ");
    Serial.print(catMotorDegrees_song);
    Serial.print(", degrees per second: ");
    Serial.println(catMotorSpeed_song);
    
    Serial.print("Cutting the end groove: cutting progress [mm/sec]: ");
    Serial.print(catSpeed_end_mmPerSecond);
    Serial.print(", degrees: ");
    Serial.print(catMotorDegrees_end);
    Serial.print(", degrees per second: ");
    Serial.println(catMotorSpeed_endGroove);
    
    Serial.print("Turning the table: table rotations: ");
    Serial.print(grooves_all);
    Serial.print(", motor degrees: ");
    Serial.print(turnTableStepperDegrees);
    Serial.print(", degrees per second: ");
    Serial.println(turnTableStepperSpeed);
    
    Serial.println();
    Serial.println("################################################################################");
    Serial.println();
    Serial.println();
    
}

void setup() {
    
    pinMode(FETCH_RECORD_BUTTON, INPUT_PULLUP);
    pinMode(START_CUTTING_BUTTON, INPUT_PULLUP);
    pinMode(LOADING_BUTTON, INPUT_PULLUP);
    pinMode(MATCH_HEADIMPACT_SWITCH, INPUT_PULLUP);
    pinMode(MOVE_MANUALLY_SWITCH, INPUT_PULLUP);
    pinMode(MOVE_CAT_FWD_BUTTON, INPUT_PULLUP);
    pinMode(MOVE_CAT_RWD_BUTTON, INPUT_PULLUP);
    pinMode(EVALUATE_BUTTONS_SWITCH, INPUT_PULLUP);
    
    pinMode(I_AM_LATE_LED, OUTPUT);
    
    pinMode(CONTROLLER_LAMP_YELLOW_PIN, OUTPUT);
    
    
    Serial.begin(115200);
    Serial.println(), Serial.println(), Serial.println();
    Serial.println(), Serial.println(), Serial.println();
    
    calculateCuttingParameters();
    
}

void freeRam() {
#ifndef ARDUINO_SIMULATION
    char *ramstart = (char *) 0x20070000;
    char *ramend = (char *) 0x20088000;
    char *heapend = sbrk(0);
    register char * stack_ptr asm( "sp" );
    struct mallinfo mi = mallinfo();
    Serial.print("Ram used (bytes): dynamic: ");
    Serial.print(mi.uordblks);
    Serial.print("  static:  ");
    Serial.print(&_end - ramstart);
    Serial.print("  stack:   ");
    Serial.print(ramend - stack_ptr);
    Serial.print(", estimation free Ram: ");
    Serial.println(stack_ptr - heapend + mi.fordblks);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @page mainProgramPage
/// The whole program sceme look as follows:
/// @image html scheduler__sceme.png
/// The main program voiceOGraph.ino holds the complete workflow and task structure. It consists of one or several workflows (CCWorkFlow objects). This workflows holds one deviceflow (CCDeviceFlow object) for every involved device. The deviceflow contains then an array that includes all tasks, to be executed by this device.
/// To run a workflow, the scheduler's run method is to be called, and a pointer to this workflow needs to be passed.
/// First, the user interaction is to be setup
///
/// @image html setupUserInteraction.png
///
/// @internal
/// @startuml{setupUserInteraction.png}
/// start
/// :setup user interaction;
/// stop
/// @enduml
/// @endinternal
///
///
/// Second, all parameters for the cutting process are to be calculated, like the number of grooves on the record, the playtime, the width of the end groove, the motor speed for cat stepper etc.
///
/// @image html calculateCuttingParameters.png
///
/// @internal
/// @startuml{calculateCuttingParameters.png}
/// start
/// :calculate all cuttingParameters;
/// stop
/// @enduml
/// @endinternal
///
///
/// Then all devices and their tasks and movements are to be defined.
///
/// @image html prepareRunLoop.png
///
/// @internal
/// @startuml{prepareRunLoop.png}
/// title preperation of run loop
/// start
/// partition initialisation {
/// 	:instantiate scheduler object;
/// 	:instantiate and define array of devices;
/// 	:instantiate and define array of controllButtons;
/// }
/// partition "define the work" {
/// 	:instantiate workflow object;
/// 	:instantiate deviceFlows of workflow;
/// 	:instantiate and define tasks of deviceFlows of workflow;
/// 	:instantiate actions of workflow;
/// }
/// partition "prepare running" {
/// 	:reviewTasks
/// all values are evaluated and converted;
/// 	:listAllTasks;
/// 	:listAllActions;
/// }
/// stop
/// @enduml
/// @endinternal
///
///
/// Now we're ready to run!
///
/// @image html runTheMachine.png
///
/// @internal
/// @startuml{runTheMachine.png}
/// title run loop of the machine
/// start
/// repeat
///     :user interaction;
///     :do ordered job;
///     if (everything ok?) then (yes)
///     else (no)
///         if (solution available?) then (yes)
///             :solve problem;
///         else (no)
///         	stop
/// 	    endif
///     endif
/// repeat while()
/// @enduml
/// @endinternal
///
///////////////////////////////////////////////////////////////////////////////////////////////////









/*
 osascript -e '
 tell application "System Events"
 try
 set serialMonitorWindow to first window of process "Arduino" whose name begins with "/dev/tty.usbmodem"
 click button 1 of serialMonitorWindow
 end try
 end tell
 '
 
 osascript -e '
 tell application "System Events"
 try
 tell application "Arduino" to activate
 set serialMonitorMenuItem to menu item "Serieller Monitor" of menu "Werkzeuge" of menu bar item "Werkzeuge" of menu bar 1 of process "Arduino"
 click serialMonitorMenuItem
 end try
 end tell
 '
 */

