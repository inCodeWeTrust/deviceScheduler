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

unsigned long       catCuttingStartPosition, catSongStartPosition, catSongEndPosition, catCuttingEndPosition;
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
    
    
    freeRam();
    
    // ================================================================================================================================
    // ================================================================================================================================
    // ============= create scheduler object: =========================================================================================
    // ================================================================================================================================
    // ================================================================================================================================
    
    Serial.print(F("create scheduler: "));
    
    scheduler = new CCDeviceScheduler("scheduler");
    scheduler->setVerbosity(SHOW_TASK_VIEW | BASICOUTPUT);
    
    Serial.println(scheduler->getName());
    
    
    freeRam();
    
    // ================================================================================================================================
    // ================================================================================================================================
    // ============= register devices: ================================================================================================
    // ================================================================================================================================
    // ================================================================================================================================
    
    Serial.print(F("register devices of scheduler "));
    Serial.println(scheduler->getName());
    
    
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
    
    
    CCDevice* turnStepper = scheduler->addStepper_A4988(STEPPER_TURN_NAME,
                                                        STEPPER_TURN_STEP_PIN,
                                                        STEPPER_TURN_DIR_PIN,
                                                        STEPPER_TURN_ENABLE_PIN,
                                                        STEPPER_TURN_STEPS_PER_ROTATION,
                                                        STEPPER_TURN_MICROSTEPPIN_00,
                                                        STEPPER_TURN_MICROSTEPPIN_01,
                                                        STEPPER_TURN_MICROSTEPPIN_02,
                                                        STEPPER_TURN_STEPMODECODE_00,
                                                        STEPPER_TURN_STEPMODECODE_01,
                                                        STEPPER_TURN_STEPMODECODE_02,
                                                        STEPPER_TURN_STEPMODECODE_03,
                                                        STEPPER_TURN_STEPMODECODE_04,
                                                        STEPPER_TURN_STEPMODECODE_05);
    
    
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
    
    
    CCDevice* catStepper = scheduler->addStepper_TMC260(STEPPER_CAT_NAME,
                                                        STEPPER_CAT_STEP_PIN,
                                                        STEPPER_CAT_DIR_PIN,
                                                        STEPPER_CAT_ENABLE_PIN,
                                                        STEPPER_CAT_STEPS_PER_ROTATION,
                                                        STEPPER_CAT_CHIPSELECT_PIN,
                                                        STEPPER_CAT_CURRENT);
    
    
    CCDevice* tableStepper = scheduler->addStepper_TMC260(STEPPER_TABLE_NAME,
                                                          STEPPER_TABLE_STEP_PIN,
                                                          STEPPER_TABLE_DIR_PIN,
                                                          STEPPER_TABLE_ENABLE_PIN,
                                                          STEPPER_TABLE_STEPS_PER_ROTATION,
                                                          STEPPER_TABLE_CHIPSELECT_PIN,
                                                          STEPPER_TABLE_CURRENT);
    
    
    CCDevice* vacuumCleaner = scheduler->addDcController(VACCUUMCLEANER_NAME,
                                                         VACCUUMCLEANER_PIN,
                                                         VACCUUMCLEANER_ACTIVE);
    
    
    CCDevice* startingSoonLamp = scheduler->addDcController(CONTROLLER_LAMP_RED_NAME,
                                                            CONTROLLER_LAMP_RED_PIN,
                                                            CONTROLLER_LAMP_RED_ON);
    
    
    freeRam();
    
    
    // ================================================================================================================================
    //=================================================================================================================================
    // ============= register sensors: ================================================================================================
    // ================================================================================================================================
    // ================================================================================================================================
    
    Serial.print(F("register controls of scheduler "));
    Serial.println(scheduler->getName());
    
    
    CCControl* bridgeParkButton = scheduler->addControlButton(CAT_PARKBUTTON_NAME,
                                                              CAT_PARKBUTTON_PIN,
                                                              CAT_PARKBUTTON_MODE);
    
    
    CCControl* bridgeEndButton = scheduler->addControlButton(CAT_ENDBUTTON_NAME,
                                                             CAT_ENDBUTTON_PIN,
                                                             CAT_ENDBUTTON_MODE);
    
    
    CCControl* songEndButton = scheduler->addControlButton(SONG_ENDBUTTON_NAME,
                                                           SONG_ENDBUTTON_PIN,
                                                           SONG_ENDBUTTON_MODE);
    
    
    CCControl* songCancelButton = scheduler->addControlButton(SONG_CANCELBUTTON_NAME,
                                                              SONG_CANCELBUTTON_PIN,
                                                              SONG_CANCELBUTTON_MODE);
    
    
    CCControl* headInclinationSensor = scheduler->addControlSensor(HEAD_INCLINATION_SENSOR_NAME,
                                                                   HEAD_INCLINATION_SENSOR_PIN);
    
    
    CCControl* armTurnSensor = scheduler->addControlSensor(ARM_SENSOR_NAME,
                                                           ARM_SENSOR_PIN);
    
    
    CCControl* loadingButton = scheduler->addControlButton(FETCH_RECORD_BUTTON_NAME,
                                                           FETCH_RECORD_BUTTON_PIN,
                                                           FETCH_RECORD_BUTTON_MODE);
    
    
    CCControl* recordAvailableButton = scheduler->addControlButton(RECORDAVAILABLE_BUTTON_NAME,
                                                                   RECORDAVAILABLE_BUTTON_PIN,
                                                                   RECORDAVAILABLE_BUTTON_MODE);
    
    
    CCControl* stockTopButton = scheduler->addControlButton(STOCKTOP_BUTTON_NAME,
                                                            STOCKTOP_BUTTON_PIN,
                                                            STOCKTOP_BUTTON_MODE);
    
    
    CCControl* stockBottomButton = scheduler->addControlButton(STOCKBOTTOM_BUTTON_NAME,
                                                               STOCKBOTTOM_BUTTON_PIN,
                                                               STOCKBOTTOM_BUTTON_MODE);
    
    
    scheduler->listDevices();
    scheduler->listControls();
    
    
    freeRam();
    
    // =================================================================================================================================================
    // =================================================================================================================================================
    // ============= register workflows ===========================================================================================================
    // =================================================================================================================================================
    // =================================================================================================================================================
    
    Serial.print(F("register workflows of scheduler "));
    Serial.println(scheduler->getName());
    
    
    // =================================================================================================================================================
    // ============= initialisation workflow ===========================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* initTheMachine = new CCWorkflow("initMachine");
    
    
    {
        // ============= devices of initMachine =======================================================================================
        
        CCDeviceFlow* catStepperFlow = initTheMachine->addDeviceFlow("catStepperFlow", catStepper, CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
        CCDeviceFlow* headLeftServoFlow = initTheMachine->addDeviceFlow("headLeftServoFlow", headLeftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* headRightServoFlow = initTheMachine->addDeviceFlow("headRightServoFlow", headRightServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* liftServoFlow = initTheMachine->addDeviceFlow("liftServoFlow", liftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* turnStepperFlow = initTheMachine->addDeviceFlow("turnStepperFlow", turnStepper, TURN_SPEED_SLOW, TURN_ACCEL_SLOW);
        
        CCControl* bridgeParkControl = initTheMachine->addControl(bridgeParkButton);
        CCControl* armControl = initTheMachine->addControl(armTurnSensor);
        
        // ============= tasks of initMachine =========================================================================================
        
        //  drive head (left) to parking position
        CCTask* driveHeadLeftToParkPosition;
        driveHeadLeftToParkPosition = headLeftServoFlow->addTask(HEAD_LEFT_PARK_POSITION);
        driveHeadLeftToParkPosition->startByDate(10);
        
        //  drive head (right) to parking position
        CCTask* driveHeadRightToParkPosition;
        driveHeadRightToParkPosition = headRightServoFlow->addTask(HEAD_RIGHT_PARK_POSITION);
        driveHeadRightToParkPosition->startByDate(20);
        
        //  drive cat to parking position
        CCTask* driveCatToParkPosition;
        driveCatToParkPosition = catStepperFlow->addTaskWithPositionResetOnCompletion(-400000);
        driveCatToParkPosition->startByDate(30);
        driveCatToParkPosition->stopByControl(bridgeParkControl, IS, CAT_PARKBUTTON_REACHED, STOP_NORMAL);
        
        
        //  lift grappler
        CCTask* liftForCalibratingArm;
        liftForCalibratingArm = liftServoFlow->addTask(LIFT_LIFTED_POSITION);
        liftForCalibratingArm->startAfterCompletionOf(catStepper, driveCatToParkPosition);
        
        //  find correct parking position of arm
        CCTask* calibrateArmBack;
        calibrateArmBack = turnStepperFlow->addTaskMoveRelativ(600, 10, 10);
        calibrateArmBack->startAfterCompletionOf(liftServo, liftForCalibratingArm);
        calibrateArmBack->stopByControl(armControl, IS_SMALLER_THEN, ARM_STOCK_POSITION, STOP_IMMEDIATELY, SKIP_APPROXIMATION_VERY_PRECISE);
        
        //  find correct parking position of arm
        CCTask* calibrateArmForth;
        calibrateArmForth = turnStepperFlow->addTaskMoveRelativWithPositionResetOnCompletion(-600, 10, 10);
        calibrateArmForth->startAfterCompletion();
        calibrateArmForth->stopByControl(armControl, IS_GREATER_THEN, ARM_STOCK_POSITION, STOP_IMMEDIATELY, SKIP_APPROXIMATION_VERY_PRECISE);
        
        //  park arm
        CCTask* lowerInParkPosition;
        lowerInParkPosition = liftServoFlow->addTask(LIFT_STOCK_POSITION);
        lowerInParkPosition->startAfterCompletionOf(turnStepper, calibrateArmForth);
        
        
        scheduler->reviewTasks(initTheMachine);
        scheduler->listAllTasks(initTheMachine);
        scheduler->listAllActions(initTheMachine);
        
        
        freeRam();
    }
    
    // =================================================================================================================================================
    // ============= fetchingRecord workflow ===========================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* fetchingRecord = new CCWorkflow("fetchingRecord");
    
    
    {
        // ============= devices of fetchingRecord: =====================================================================================
        
        CCDeviceFlow* liftServoFlow = fetchingRecord->addDeviceFlow("liftServoFlow", liftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* turnStepperFlow = fetchingRecord->addDeviceFlow("turnStepperFlow", turnStepper, TURN_SPEED_SLOW, TURN_ACCEL_SLOW);
        CCDeviceFlow* pumpServoFlow = fetchingRecord->addDeviceFlow("pumpServoFlow", pumpServo, PUMP_SPEED, PUMP_ACCEL);
        CCDeviceFlow* stockStepperFlow = fetchingRecord->addDeviceFlow("stockStepperFlow", stockStepper, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL);
        
        
        CCControl* armControl = fetchingRecord->addControl(armTurnSensor);
        CCControl* recordAvailableControl = fetchingRecord->addControl(recordAvailableButton);
        CCControl* stockTopControl = fetchingRecord->addControl(stockTopButton);
        CCControl* stockBottomControl = fetchingRecord->addControl(stockBottomButton);
        
        CCFlowControl* stockTopFlowControl = fetchingRecord->addFlowControl("stockTopObserver", stockTopControl, IS, STOCKTOP_BUTTON_IS_REACHED);
        CCFlowControl* stockBottomFlowControl = fetchingRecord->addFlowControl("stockBottomObserver", stockBottomControl, IS, STOCKBOTTOM_BUTTON_IS_REACHED);
        
        
        
        // ============= tasks of fetchingRecord: =====================================================================================
        
        //  move stock down first
        CCTask* moveStockStepperDown;
        moveStockStepperDown = stockStepperFlow->addTaskMoveRelativ(-400000);
        moveStockStepperDown->startByDate(100);
        moveStockStepperDown->stopByControl(recordAvailableControl, IS_NOT, RECORD_IS_AVAILABLE, STOP_NORMAL, SKIP_APPROXIMATION_VERY_PRECISE);
        
        //  lift grappler
        CCTask* liftForCalibratingArm;
        liftForCalibratingArm = liftServoFlow->addTask(LIFT_LIFTED_POSITION);
        liftForCalibratingArm->startAfterCompletionOf(stockStepper, moveStockStepperDown);
        
        //  find correct parking position of arm
        CCTask* calibrateArmBack;
        calibrateArmBack = turnStepperFlow->addTaskMoveRelativ(600, 10, 10);
        calibrateArmBack->startAfterCompletionOf(liftServo, liftForCalibratingArm);
        calibrateArmBack->stopByControl(armControl, IS_SMALLER_THEN, ARM_STOCK_POSITION, STOP_IMMEDIATELY, SKIP_APPROXIMATION_VERY_PRECISE);
        
        //  find correct parking position of arm
        CCTask* calibrateArmForth;
        calibrateArmForth = turnStepperFlow->addTaskMoveRelativWithPositionResetOnCompletion(-600, 10, 10);
        calibrateArmForth->startAfterCompletion();
        calibrateArmForth->stopByControl(armControl, IS_GREATER_THEN, ARM_STOCK_POSITION, STOP_IMMEDIATELY, SKIP_APPROXIMATION_VERY_PRECISE);
        
        //  supply a new record, terminated by recordAvailableButton
        CCTask* supplyRecord;
        supplyRecord = stockStepperFlow->addTaskMoveRelativ(400000);
        supplyRecord->startAfterCompletion();
        supplyRecord->stopByControl(recordAvailableControl, IS, RECORD_IS_AVAILABLE);
        
        //  lower grappler to stock: start when record is available
        CCTask* lowerToStock;
        lowerToStock = liftServoFlow->addTask(LIFT_STOCK_POSITION);
        lowerToStock->startAfterCompletionOf(stockStepper, supplyRecord);
        
        //  pump: prepare
        CCTask* pumpForGrip_down;
        pumpForGrip_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
        pumpForGrip_down->startByTriggerpositionOf(liftServo, lowerToStock, LIFT_STOCK_POSITION + 200);
        
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
        turnRecordToTable = turnStepperFlow->addTask(TURN_TABLE_POSITION);
        turnRecordToTable->startByTriggerpositionOf(liftServo, liftNewRecord, LIFT_UP_TRIGGER_TURN);
        
        //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
        CCTask* lowerRecordToTable;
        lowerRecordToTable = liftServoFlow->addTask(LIFT_TABLE_POSITION);
        lowerRecordToTable->startByTriggerpositionOf(turnStepper, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
        
        //  release new record: release vacuum
        CCTask* pumpForRelease_down;
        pumpForRelease_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
        pumpForRelease_down->startByTriggerpositionOf(liftServo, lowerRecordToTable, LIFT_TABLE_POSITION + 200);
        
        //  lift for going to park position: start when vacuum was released
        CCTask* liftForParkPosition;
        liftForParkPosition = liftServoFlow->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
        liftForParkPosition->startAfterCompletionOf(pumpServo, pumpForRelease_down);
        
        //  release pump when record is placed
        CCTask* pumpForRelease_up;
        pumpForRelease_up = pumpServoFlow->addTask(PUMP_PARK_POSITION);
        pumpForRelease_up->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_TABLE_POSITION + 200);
        
        //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        CCTask* turnToStockPosition;
        turnToStockPosition = turnStepperFlow->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
        turnToStockPosition->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
        
        //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
        CCTask* lowerForParkPosition;
        lowerForParkPosition = liftServoFlow->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
        lowerForParkPosition->startByTriggerpositionOf(turnStepper, turnToStockPosition, TURN_TO_STOCK_TRIGGER_LIFT);
        
        
        
        // ============= actions of fetchingRecord: =====================================================================================
        
        //  cancel fetching if stockBottomButton is pressed while lowering
        CCAction* cancelLoweringOnBottomAction = stockBottomFlowControl->addAction("cancelLoweringOnBottomAction", STOCKBOTTOM_BUTTON_REACHED);
        cancelLoweringOnBottomAction->evokeTaskStop(stockStepperFlow, moveStockStepperDown, STOP_NORMAL);
        
        //  stop supplying when stockTopButton is reached
        CCAction* stockEmptyAction = stockTopFlowControl->addAction("stockEmptyAction", STOCKTOP_BUTTON_REACHED);
        stockEmptyAction->evokeTaskStop(stockStepperFlow, supplyRecord, STOP_NORMAL);
        
        
        
        
        scheduler->reviewTasks(fetchingRecord);
        scheduler->listAllTasks(fetchingRecord);
        scheduler->listAllActions(fetchingRecord);
        
        
        freeRam();
    }
    
    
    
    // =================================================================================================================================================
    // ============= driveToStartPositionOnly workflow =================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* driveToStartPositionOnly = new CCWorkflow("driveToStartPositionOnly");
    
    
    {
        // ============= devices of driveToStartPositionOnly: =====================================================================================
        
        CCDeviceFlow* catStepperFlow = driveToStartPositionOnly->addDeviceFlow("catStepperFlow", catStepper, CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
        CCDeviceFlow* headLeftServoFlow = driveToStartPositionOnly->addDeviceFlow("headRightServoFlow", headRightServo, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
        
        // ============= tasks of driveToStartPositionOnly: =====================================================================================
        
        //  move to start groove:
        CCTask* driveToCuttingStartPosition;
        driveToCuttingStartPosition = catStepperFlow->addTaskWithPositionReset(catCuttingStartPosition);
        driveToCuttingStartPosition->startByDate(100);
        
        //  lower head close to record surface (left servo): start when reached start position of start groove
        CCTask* lowerHeadLeftForCutting;
        lowerHeadLeftForCutting = headLeftServoFlow->addTask(HEAD_LEFT_CUT_POSITION);
        lowerHeadLeftForCutting->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
        
        
        scheduler->reviewTasks(driveToStartPositionOnly);
        scheduler->listAllTasks(driveToStartPositionOnly);
        scheduler->listAllActions(driveToStartPositionOnly);
        
        
        freeRam();
    }
    
    
    // =================================================================================================================================================
    // ============= songCuttingOnly workflow ===========================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* songCuttingOnly = new CCWorkflow("songCuttingOnly");
    
    
    {
        // ============= devices of songCuttingOnly: =====================================================================================
        
        CCDeviceFlow* catStepperFlow = songCuttingOnly->addDeviceFlow("catStepperFlow", catStepper, CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
        CCDeviceFlow* tableStepperFlow = songCuttingOnly->addDeviceFlow("tableStepperFlow", tableStepper);
        CCDeviceFlow* startingSoonLampFlow = songCuttingOnly->addDeviceFlow("startingSoonLampFlow", startingSoonLamp);
        CCDeviceFlow* headLeftServoFlow = songCuttingOnly->addDeviceFlow("headLeftServoFlow", headLeftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* headRightServoFlow = songCuttingOnly->addDeviceFlow("headRghtServoFlow", headRightServo, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
        CCDeviceFlow* vacuumCleanerFlow = songCuttingOnly->addDeviceFlow("vacuumCleanerFlow", vacuumCleaner, 1.0, 0.0);
        
        CCControl* headInclinationControl = songCuttingOnly->addControl(headInclinationSensor);
        CCControl* bridgeParkControl = songCuttingOnly->addControl(bridgeParkButton);
        CCControl* songEndControl = songCuttingOnly->addControl(songEndButton);
        CCControl* songCancelControl = songCuttingOnly->addControl(songCancelButton);
        
        CCFlowControl* songEndFlowControl = songCuttingOnly->addFlowControl("songEndObserver", songEndControl, IS, SONG_ENDBUTTON_PRESSED);
        CCFlowControl* songCancelFlowControl = songCuttingOnly->addFlowControl("songCancelControl", songCancelControl, IS, SONG_CANCELBUTTON_PRESSED);
        
        
        
        // ============= tasks of songCuttingOnly: =====================================================================================
        
        // switch on vacuum device
        CCTask* hooverTheFlake;
        hooverTheFlake = vacuumCleanerFlow->addTask(1.0);
        hooverTheFlake->startByDate(10);
        
        //  turn the table:
        CCTask* turnTheTable;
        turnTheTable = tableStepperFlow->addTaskWithPositionReset(turnTableStepperDegrees + 17200.0, turnTableStepperSpeed, TABLE_STEP_ACCEL);
        turnTheTable->startByDate(10);
        
        //  (1) blink lamp for start cutting very soon:
        CCTask* blinkForCuttingVerySoon;
        blinkForCuttingVerySoon = startingSoonLampFlow->addTask(0.5, 1.0, 0.0);
        blinkForCuttingVerySoon->startByDate(10);
        
        //  approximate head to record surface (right servo): start when left servo reached cutting position
        CCTask* approximateHeadRightForCutting;
        approximateHeadRightForCutting = headRightServoFlow->addTask(HEAD_RIGHT_CUT_POSITION);
        approximateHeadRightForCutting->startByDate(10000);
        approximateHeadRightForCutting->stopDynamicallyBySensor(headInclinationControl, HEAD_INCLINATION_INIT_STOP, HEAD_INCLINATION_TARGET, 0.6, SKIP_APPROXIMATION_PRECISE);
        //        cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->stopDynamicallyBySensor(HEAD_INCLINATION_SENSOR, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
        
        //  (2) blink lamp for start cutting very very soon:
        CCTask* blinkForCuttingVeryVerySoon;
        blinkForCuttingVeryVerySoon = startingSoonLampFlow->addTask(0.5, 3.0, 0.0);
        blinkForCuttingVerySoon->switchToNextTaskAfterCompletionOf(headRightServo, approximateHeadRightForCutting);
        
        //  make start groove:
        CCTask* makeStartGroove;
        makeStartGroove = catStepperFlow->addTask(catCuttingEndPosition, catMotorSpeed_startGroove, CAT_ACCEL_SLOW);
        makeStartGroove->startByControl(headInclinationControl, IS_SMALLER_THEN, HEAD_INCLINATION_START_CAT);
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
        
        
        
        // ============= actions of cuttingProcess: =====================================================================================
        
        //  jump over to end groove if songEndButton is pressed
        CCAction* speedUpCatForEndGroove = songEndFlowControl->addAction("speedUpCatAtSongEnd", SONGENDBUTTON_PRESSED);
        speedUpCatForEndGroove->evokeJumpToTask(catStepperFlow, makeMainGroove, SWITCH_PROMPTLY, makeEndGroove);
        
        CCAction* songEndBlinking_00 = songEndFlowControl->addAction("songEndBlinking_00", SONGENDBUTTON_PRESSED);
        songEndBlinking_00->evokeJumpToTask(startingSoonLampFlow, keepLampOn, SWITCH_PROMPTLY, blinkForFinishingVeryVerySoon);
        
        CCAction* songEndBlinking_01 = songEndFlowControl->addAction("songEndBlinking_01", SONGENDBUTTON_PRESSED);
        songEndBlinking_01->evokeJumpToTask(startingSoonLampFlow, blinkForFinishingSoon, SWITCH_PROMPTLY, blinkForFinishingVeryVerySoon);
        
        CCAction* songEndBlinking_02 = songEndFlowControl->addAction("songEndBlinking_02", SONGENDBUTTON_PRESSED);
        songEndBlinking_02->evokeJumpToTask(startingSoonLampFlow, blinkForFinishingVerySoon, SWITCH_PROMPTLY, blinkForFinishingVeryVerySoon);
        
        
        //  cancel cutting if songCancelButton is pressed
        CCAction* songCancelAction_cat = songCancelFlowControl->addAction("songCancelAction", SONGCANCELBUTTON_PRESSED);
        songCancelAction_cat->evokeJumpToTask(catStepperFlow, NULL, SWITCH_PROMPTLY, driveToParkPosition);
        
        CCAction* songCancelAction_leftHead = songCancelFlowControl->addAction("songCancelAction", SONGCANCELBUTTON_PRESSED);
        songCancelAction_leftHead->evokeJumpToTask(headLeftServoFlow, NULL, SWITCH_PROMPTLY, liftHeadLeftAfterCutting);
        
        CCAction* songCancelAction_rightHead = songCancelFlowControl->addAction("songCancelAction", SONGCANCELBUTTON_PRESSED);
        songCancelAction_rightHead->evokeJumpToTask(headRightServoFlow, NULL, SWITCH_PROMPTLY, liftHeadRightAfterCutting);
        
        
        
        scheduler->reviewTasks(songCuttingOnly);
        scheduler->listAllTasks(songCuttingOnly);
        scheduler->listAllActions(songCuttingOnly);
        
        
        freeRam();
    }
    
    
    
    
    
    
    
    
    
    
    // =================================================================================================================================================
    // ============= cuttingProcess workflow ===========================================================================================================
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
        driveToCuttingStartPosition = catStepperFlow->addTaskWithPositionReset(catCuttingStartPosition);
        driveToCuttingStartPosition->startByDate(100);
        
        // switch on vacuum device
        CCTask* hooverTheFlake;
        hooverTheFlake = vacuumCleanerFlow->addTask(1.0);
        hooverTheFlake->startByDate(1500);
        
        //  turn the table:
        CCTask* turnTheTable;
        turnTheTable = tableStepperFlow->addTaskWithPositionReset(turnTableStepperDegrees + 17200.0, turnTableStepperSpeed, TABLE_STEP_ACCEL);
        turnTheTable->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, catCuttingStartPosition - 10000);
        
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
        makeStartGroove->startByControl(headInclinationControl, IS_SMALLER_THEN, HEAD_INCLINATION_START_CAT);
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
        
        
        
        // ============= actions of cuttingProcess: =====================================================================================
        
        //  jump over to end groove if songEndButton is pressed
        CCAction* speedUpCatForEndGroove = songEndFlowControl->addAction("speedUpCatAtSongEnd", SONGENDBUTTON_PRESSED);
        speedUpCatForEndGroove->evokeJumpToTask(catStepperFlow, makeMainGroove, SWITCH_PROMPTLY, makeEndGroove);
        
        CCAction* songEndBlinking_00 = songEndFlowControl->addAction("songEndBlinking_00", SONGENDBUTTON_PRESSED);
        songEndBlinking_00->evokeJumpToTask(startingSoonLampFlow, keepLampOn, SWITCH_PROMPTLY, blinkForFinishingVeryVerySoon);
        
        CCAction* songEndBlinking_01 = songEndFlowControl->addAction("songEndBlinking_01", SONGENDBUTTON_PRESSED);
        songEndBlinking_01->evokeJumpToTask(startingSoonLampFlow, blinkForFinishingSoon, SWITCH_PROMPTLY, blinkForFinishingVeryVerySoon);
        
        CCAction* songEndBlinking_02 = songEndFlowControl->addAction("songEndBlinking_02", SONGENDBUTTON_PRESSED);
        songEndBlinking_02->evokeJumpToTask(startingSoonLampFlow, blinkForFinishingVerySoon, SWITCH_PROMPTLY, blinkForFinishingVeryVerySoon);
        
        
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
    // ============= ejectingRecord workflow ===========================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* ejectingRecord = new CCWorkflow("ejectingRecord");
    
    
    {
        // ============= devices of ejectingRecord ====================================================================================
        
        CCDeviceFlow* liftServoFlow = ejectingRecord->addDeviceFlow("liftServoFlow", liftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        CCDeviceFlow* turnStepperFlow = ejectingRecord->addDeviceFlow("turnStepperFlow", turnStepper, TURN_SPEED_SLOW, TURN_ACCEL_SLOW);
        CCDeviceFlow* pumpServoFlow = ejectingRecord->addDeviceFlow("pumpServoFlow", pumpServo, PUMP_SPEED, PUMP_ACCEL);
        CCDeviceFlow* conveyStepperFlow = ejectingRecord->addDeviceFlow("conveyStepperFlow", conveyStepper, CONVEYOR_SPEED, CONVEYOR_ACCEL);
        
        CCControl* armControl = ejectingRecord->addControl(armTurnSensor);
        
        
        
        // ============= tasks of ejectingRecord ======================================================================================
        
        //  lift grappler for calibrating arm
        CCTask* liftForCalibratingArm;
        liftForCalibratingArm = liftServoFlow->addTask(LIFT_LIFTED_POSITION);
        liftForCalibratingArm->startByDate(100);
        
        //  find correct parking position of arm
        CCTask* calibrateArmBack;
        calibrateArmBack = turnStepperFlow->addTaskMoveRelativ(600, 10, 10);
        calibrateArmBack->startAfterCompletionOf(liftServo, liftForCalibratingArm);
        calibrateArmBack->stopByControl(armControl, IS_SMALLER_THEN, ARM_STOCK_POSITION, STOP_IMMEDIATELY, SKIP_APPROXIMATION_PRECISE);
        
        //  find correct parking position of arm
        CCTask* calibrateArmForth;
        calibrateArmForth = turnStepperFlow->addTaskMoveRelativWithPositionResetOnCompletion(-600, 10, 10);
        calibrateArmForth->startAfterCompletion();
        calibrateArmForth->stopByControl(armControl, IS_GREATER_THEN, ARM_STOCK_POSITION, STOP_IMMEDIATELY, SKIP_APPROXIMATION_PRECISE);
        
        // lift grappler: start when arm is in correct position
        CCTask* liftForTable;
        liftForTable = liftServoFlow->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
        liftForTable->startAfterCompletionOf(turnStepper, calibrateArmForth);
        
        //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        CCTask* turnToTable;
        turnToTable = turnStepperFlow->addTask(TURN_TABLE_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
        turnToTable->startByTriggerpositionOf(liftServo, liftForTable, LIFT_LIFTED_POSITION + 60);
        
        //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
        CCTask* lowerToTable;
        lowerToTable = liftServoFlow->addTask(LIFT_TABLE_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
        lowerToTable->startByTriggerpositionOf(turnStepper, turnToTable, TURN_TO_TABLE_TRIGGER_LIFT);
        
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
        turnToConveyor = turnStepperFlow->addTask(TURN_CONVEYOR_POSITION);
        turnToConveyor->startByTriggerpositionOf(liftServo, liftCuttedRecord, LIFT_UP_TRIGGER_TURN);
        
        //  lower grappler to conveyer position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
        CCTask* lowerToConveyor;
        lowerToConveyor = liftServoFlow->addTask(LIFT_CONVEYER_POSITION);
        lowerToConveyor->startByTriggerpositionOf(turnStepper, turnToConveyor, TURN_TO_CONVEYER_TRIGGER_LIFT);
        
        //  release new record, release vacuum
        CCTask* pumpForRelease_down;
        pumpForRelease_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
        pumpForRelease_down->startAfterCompletionOf(liftServo, lowerToConveyor);
        
        //  lift for parking
        CCTask* liftForParkPosition;
        liftForParkPosition = liftServoFlow->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
        liftForParkPosition->startAfterCompletionOf(pumpServo, pumpForRelease_down);
        
        //  move it to the user
        CCTask* driveIt;
        driveIt = conveyStepperFlow->addTaskWithPositionReset(CONVEYOR_DISTANCE);
        driveIt->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_CONVEYER_POSITION + 60);
        
        //  release pump
        CCTask* pumpForRelease_up;
        pumpForRelease_up = pumpServoFlow->addTask(PUMP_PARK_POSITION);
        pumpForRelease_up->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_CONVEYER_POSITION + 160);
        
        //  turn grappler for parking
        CCTask* turnToStock;
        turnToStock = turnStepperFlow->addTask(TURN_STOCK_POSITION);
        turnToStock->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
        
        //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
        CCTask* lowerToPark;
        lowerToPark = liftServoFlow->addTask(LIFT_PARK_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        lowerToPark->startByTriggerpositionOf(turnStepper, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
        
        
        
        
        scheduler->reviewTasks(ejectingRecord);
        scheduler->listAllTasks(ejectingRecord);
        
        
        freeRam();
    }
    
    
    
    // =================================================================================================================================================
    // ============= loading workflow ==================================================================================================================
    // =================================================================================================================================================
    
    CCWorkflow* loading = new CCWorkflow("loading");
    
    
    {
        
        // ============= devices of loading ====================================================================================
        
        CCDeviceFlow* liftServoFlow = loading->addDeviceFlow("liftServoFlow", liftServo, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
        CCDeviceFlow* turnStepperFlow = loading->addDeviceFlow("turnStepperFlow", turnStepper, TURN_SPEED_FAST, TURN_ACCEL_FAST);
        CCDeviceFlow* stockStepperFlow = loading->addDeviceFlow("stockStepperFlow", stockStepper, STOCK_LOADING_RECORDS_SPEED, STOCK_LOADING_RECORDS_ACCEL);
        
        CCControl* loadingButtonControl = loading->addControl(loadingButton);
        CCControl* recordAvailableControl = loading->addControl(recordAvailableButton);
        CCControl* stockTopControl = loading->addControl(stockTopButton);
        CCControl* stockBottomControl = loading->addControl(stockBottomButton);
        CCControl* armControl = loading->addControl(armTurnSensor);
        
        CCFlowControl* stockTopFlowControl = loading->addFlowControl("stockTopObserver", stockTopControl, IS, STOCKTOP_BUTTON_IS_REACHED);
        
        
        
        // ============= tasks of loading: =====================================================================================
        
        //  lift grappler
        CCTask* liftForCalibratingArm;
        liftForCalibratingArm = liftServoFlow->addTask(LIFT_LIFTED_POSITION);
        liftForCalibratingArm->startByDate(100);
        
        //  find correct parking position of arm
        CCTask* calibrateArmBack;
        calibrateArmBack = turnStepperFlow->addTaskMoveRelativ(600, 10, 10);
        calibrateArmBack->startAfterCompletionOf(liftServo, liftForCalibratingArm);
        calibrateArmBack->stopByControl(armControl, IS_SMALLER_THEN, ARM_STOCK_POSITION, STOP_IMMEDIATELY, SKIP_APPROXIMATION_VERY_PRECISE);
        
        //  find correct parking position of arm
        CCTask* calibrateArmForth;
        calibrateArmForth = turnStepperFlow->addTaskMoveRelativWithPositionResetOnCompletion(-600, 10, 10);
        calibrateArmForth->startAfterCompletion();
        calibrateArmForth->stopByControl(armControl, IS_GREATER_THEN, ARM_STOCK_POSITION, STOP_IMMEDIATELY, SKIP_APPROXIMATION_VERY_PRECISE);
        
        //  lift for parking on turnTable
        CCTask* liftForTurnTable;
        liftForTurnTable = liftServoFlow->addTask(LIFT_UP_POSITION);
        liftForTurnTable->startAfterCompletionOf(turnStepper, calibrateArmForth);
        
        //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        CCTask* turnToTable;
        turnToTable = turnStepperFlow->addTask(TURN_TABLE_POSITION);
        turnToTable->startByTriggerpositionOf(liftServo, liftForTurnTable, LIFT_UP_TRIGGER_TURN);
        
        //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
        CCTask* lowerToTable;
        lowerToTable = liftServoFlow->addTask(LIFT_TABLE_POSITION);
        lowerToTable->startByTriggerpositionOf(turnStepper, turnToTable, TURN_TO_TABLE_TRIGGER_LIFT);
        
        //  move stock down first
        CCTask* moveStockStepperDown;
        moveStockStepperDown = stockStepperFlow->addTaskMoveRelativ(-400000);
        moveStockStepperDown->startByTriggerpositionOf(turnStepper, turnToTable, TURN_TO_STOCK_TRIGGER_LIFT);
        moveStockStepperDown->stopByControl(stockBottomControl, IS, STOCKBOTTOM_BUTTON_IS_REACHED);
        
        //  wait while the new records are loaded
        
        //  lift up the new records, terminated by recordAvailableButton
        CCTask* supplyNewRecords;
        supplyNewRecords = stockStepperFlow->addTaskMoveRelativ(800000, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL);
        supplyNewRecords->startByControl(loadingButtonControl, IS, FETCH_RECORD_BUTTON_PRESSED);
        supplyNewRecords->stopByControl(recordAvailableControl, IS, RECORD_IS_AVAILABLE);
        
        //  bring lift back for normal parking
        CCTask* liftForParkPosition;
        liftForParkPosition = liftServoFlow->addTask(LIFT_UP_POSITION);
        liftForParkPosition->startAfterCompletionOf(stockStepper, supplyNewRecords);
        
        //  turn grappler for parking
        CCTask* turnToStock;
        turnToStock = turnStepperFlow->addTask(TURN_STOCK_POSITION);
        turnToStock->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
        
        //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
        CCTask* lowerToPark;
        lowerToPark = liftServoFlow->addTask(LIFT_PARK_POSITION);
        lowerToPark->startByTriggerpositionOf(turnStepper, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
        
        
        
        // ============= actions of loading: =====================================================================================
        
        //  stop supplying when stockTopButton is reached
        CCAction* stockEmptyAction = stockTopFlowControl->addAction("stockEmptyAction", STOCKTOP_BUTTON_REACHED);
        stockEmptyAction->evokeTaskStop(stockStepperFlow, supplyNewRecords, STOP_NORMAL);
        
        
        
        
        scheduler->reviewTasks(loading);
        scheduler->listAllTasks(loading);
        scheduler->listAllActions(loading);
        
        
        freeRam();
    }
    
    
    
    
    // ============================================================================================================================
    // ============= evaluate evaluation switches =====================================================================================
    // ============================================================================================================================
    
    // ============= evaluate evaluateButtons switch: =====================================================================================
    
    if (scheduler->getValueOfTriStateSwitch(EVALUATE_BUTTONS_SWITCH, EVALUATE_BUTTONS_ACTIVE)) {
        Serial.println();
        Serial.print(F("evaluate the buttons"));
        
        scheduler->evaluateButtons();
        
        // this will not return...
        
    }
    
    
    // ============= evaluate runTable switch: =====================================================================================
    
    if (scheduler->getValueOfTriStateSwitch(RUN_TABLE_SWITCH, RUN_TABLE_ACTIVE)) {
        Serial.println();
        Serial.print(F("run the table"));
        
        {
            CCWorkflow* turnTableOnly = new CCWorkflow("turnTableOnly");
            
            CCDeviceFlow* tableStepperFlow = turnTableOnly->addDeviceFlow("tableStepperFlow", tableStepper);
            CCDeviceFlow* vacuumCleanerFlow = turnTableOnly->addDeviceFlow("vacuumCleanerFlow", vacuumCleaner, 1.0, 0.0);
            
            
            // switch on vacuum device
            CCTask* hooverTheFlake;
            hooverTheFlake = vacuumCleanerFlow->addTask(1.0);
            hooverTheFlake->startByDate(150);
            
            //  turn the table:
            CCTask* turnTheTable;
            turnTheTable = tableStepperFlow->addTaskWithPositionReset(turnTableStepperDegrees + 17200.0, turnTableStepperSpeed, TABLE_STEP_ACCEL);
            turnTheTable->startByDate(150);
            
            
            scheduler->reviewTasks(turnTableOnly);
            scheduler->listAllTasks(turnTableOnly);
            
            
            freeRam();
            
            
            while (true) {
                scheduler->run(turnTableOnly);
            }
        }
    }
    
    
    // ============= evaluate matchHeadImpactButton =====================================================================================
    
    if (scheduler->getValueOfTriStateSwitch(MATCH_HEADIMPACT_SWITCH, MATCH_HEADIMPACT_ACTIVE)) {
        Serial.println();
        Serial.println(F("matching head impact ... "));
        
        Serial.print(F("init the machine first ... "));
        scheduler->run(initTheMachine);
        Serial.println("done");
        
        {
            CCWorkflow* matchHeadImpact = new CCWorkflow("matchHeadImpact");
            
            CCDeviceFlow* catStepperFlow = matchHeadImpact->addDeviceFlow("catStepperFlow", catStepper, CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
            CCDeviceFlow* headLeftServoFlow = matchHeadImpact->addDeviceFlow("headLeftServoFlow", headLeftServo, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
            CCDeviceFlow* headRightServoFlow = matchHeadImpact->addDeviceFlow("headRightServoFlow", headRightServo, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
            CCDeviceFlow* startingSoonLampFlow = matchHeadImpact->addDeviceFlow("startingSoonLampFlow", startingSoonLamp);
            
            CCControl* headInclinationControl = matchHeadImpact->addControl(headInclinationSensor);
            
            
            
            // ============= tasks of matchHeadImpact: =====================================================================================
            
            //  move to matchingPosition:
            CCTask* driveToMatchingPosition;
            driveToMatchingPosition = catStepperFlow->addTask(3000);
            driveToMatchingPosition->startByDate(100);
            
            //  blink lamp for approximating:
            CCTask* blinkForApproximation;
            blinkForApproximation = startingSoonLampFlow->addTask(0.5, 4.0, 0.0);
            blinkForApproximation->startAfterCompletionOf(catStepper, driveToMatchingPosition);
            
            //  lower head close to record surface (left servo): start when reached matchingPosition
            CCTask* lowerHeadLeftForMatching;
            lowerHeadLeftForMatching = headLeftServoFlow->addTask(HEAD_LEFT_CUT_POSITION);
            lowerHeadLeftForMatching->startAfterCompletionOf(catStepper, driveToMatchingPosition);
            
            //  approximate head to record surface (right servo): start when left servo reached cutting position
            CCTask* approximateHeadRightForMatching;
            approximateHeadRightForMatching = headRightServoFlow->addTask(HEAD_RIGHT_CUT_POSITION);
            approximateHeadRightForMatching->startAfterCompletionOf(catStepper, driveToMatchingPosition);
            approximateHeadRightForMatching->stopDynamicallyBySensor(headInclinationControl, HEAD_INCLINATION_INIT_STOP, HEAD_INCLINATION_TARGET, 0.6, SKIP_APPROXIMATION_PRECISE);
            
            //  keep lamp constantly on when target position is reaached
            CCTask* keepLampOn;
            keepLampOn = startingSoonLampFlow->addTask(1.0, 4.0, 0.0);
            blinkForApproximation->switchToNextTaskAfterCompletionOf(headRightServo, approximateHeadRightForMatching);
            
            //  give some time for reading...
            
            //  lift head after matching
            CCTask* liftHeadRightAfterMatching;
            liftHeadRightAfterMatching = headRightServoFlow->addTask(HEAD_RIGHT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW);
            liftHeadRightAfterMatching->startAfterCompletionOf(headRightServo, approximateHeadRightForMatching);
            liftHeadRightAfterMatching->setStartDelay(3000);
            
            //  switch lamp off after matching
            keepLampOn->stopAfterCompletionOf(headRightServo, liftHeadRightAfterMatching, STOP_NORMAL);
            
            //  lift head into park position:
            CCTask* liftHeadLeftAfterMatching;
            liftHeadLeftAfterMatching = headLeftServoFlow->addTask(HEAD_LEFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW);
            liftHeadLeftAfterMatching->startAfterCompletionOf(headRightServo, liftHeadRightAfterMatching);
            
            
            
            scheduler->reviewTasks(matchHeadImpact);
            scheduler->listAllTasks(matchHeadImpact);
            
            
            freeRam();
            
            
            while (true) {
                scheduler->run(matchHeadImpact);
                delay(1000);
            }
        }
    }
    
    
    
    
    // ============================================================================================================================
    // ============================================================================================================================
    // ============= operation: ===================================================================================================
    // ============================================================================================================================
    // ============================================================================================================================
    
    scheduler->run(initTheMachine);
    Serial.println(F("initialisation complete!"));
    
    bool initNeeded = false;
    long blinkTimer = millis();
    
    while (!initNeeded) {
        
        // ============= evaluate fetchingRecordButton =====================================================================================
        
        if (digitalRead(FETCH_RECORD_BUTTON_PIN) == FETCH_RECORD_BUTTON_PRESSED) {
            long buttonPressedTime = millis();
            delay(100);
            
            while (digitalRead(FETCH_RECORD_BUTTON_PIN) == FETCH_RECORD_BUTTON_PRESSED) {
                if (millis() - buttonPressedTime > 4000) break;
            }
            
            if (millis() - buttonPressedTime > 4000) {
                Serial.println();
                Serial.print(F("loading new records ... "));
                
                digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
                scheduler->run(loading);
                digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
                
                Serial.println(F("done"));
                Serial.println();
                
            } else {
                Serial.println();
                Serial.print(F("fetch new record for cutting ... "));
                
                digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
                scheduler->run(fetchingRecord);
                digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
                
                Serial.println(F("done"));
                Serial.println();
                
            }
        }
        
        
        // ============= evaluate startCuttingButton =====================================================================================
        
        if (digitalRead(START_CUTTING_BUTTON) == START_CUTTING_BUTTON_PRESSED) {
            if (digitalRead(MOVE_CAT_MANUALLY_SWITCH) != MOVE_CAT_MANUALLY_ACTIVE) {
                
                Serial.println();
                Serial.print(F("cut and scratch the record ... "));
                
                digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
                scheduler->run(cuttingProcess);
                
            } else {
                Serial.println("go for mamual moving");
                
                digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
                scheduler->run(driveToStartPositionOnly);
                
                while (digitalRead(START_CUTTING_BUTTON) == START_CUTTING_BUTTON_PRESSED);
                
                {
                    CCWorkflow *manualDrive = new CCWorkflow("manualDrive");
                    
                    CCDeviceFlow *catStepperFlow = manualDrive->addDeviceFlow("catStepperDeviceFlow", catStepper);
                    
                    CCControl* catForwardControl = manualDrive->addControl(loadingButton);
                    CCControl* catRewindControl = manualDrive->addControl(songEndButton);
                    
                    while (digitalRead(START_CUTTING_BUTTON) != START_CUTTING_BUTTON_PRESSED) {
                        if (catForwardControl->getDigitalValue() == FETCH_RECORD_BUTTON_PRESSED) {
                            {
                                CCTask* driveToCuttinEndPosition;
                                driveToCuttinEndPosition = catStepperFlow->addTask(catCuttingEndPosition);
                                driveToCuttinEndPosition->startByDate(100);
                                driveToCuttinEndPosition->stopByControl(catForwardControl, IS_NOT, FETCH_RECORD_BUTTON_PRESSED, STOP_NORMAL);
                            }
                            scheduler->reviewTasks(manualDrive);
                            scheduler->run(manualDrive);
                            
                            catStepperFlow->deleteAllTasks();
                            
                            Serial.print(F("cat position: "));
                            Serial.println(catStepper->getCurrentPosition());
                            
                        }
                        
                        if (catRewindControl->getDigitalValue() == SONG_ENDBUTTON_PRESSED) {
                            {
                                CCTask* driveToParkPosition;
                                driveToParkPosition = catStepperFlow->addTask(0);
                                driveToParkPosition->startByDate(100);
                                driveToParkPosition->stopByControl(catRewindControl, IS_NOT, SONG_ENDBUTTON_PRESSED, STOP_NORMAL);
                            }
                            scheduler->reviewTasks(manualDrive);
                            scheduler->run(manualDrive);
                            
                            catStepperFlow->deleteAllTasks();
                            
                            Serial.print(F("cat position: "));
                            Serial.println(catStepper->getCurrentPosition());
                            
                        }
                    }
                }
                
                scheduler->run(songCuttingOnly);
                
            }
            
            Serial.println(F("done"));
            
            if (digitalRead(SKIP_UNLOADING_SWITCH) != SKIP_UNLOADING_ACTIVE) {
                Serial.print(F("eject the record ... "));
                scheduler->run(ejectingRecord);
            }
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
            
            Serial.println(F("done"));
            Serial.println();
            
            
        }
        
        
        
        // ============= or just blink =======================================================================================================
        
        if (millis() - blinkTimer > 2000) {
            digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
            if (millis() - blinkTimer > 2200) {
                digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
                blinkTimer = millis();
                Serial.print(F("."));
                
                if (digitalRead(SCHEDULERS_TASKVIEW_SHOW_PIN) == SCHEDULERS_TASKVIEW_HIDE) {
                    scheduler->setVerbosity(NO_OUTPUT);
                }
                
            }
        }
    }
    
    
    Serial.println(F("exectuion is terminated!"));
    
    delay(2000);
    
    freeRam();
    
    Serial.println();
    Serial.print(F("delete all objects ... "));
    
    delete initTheMachine;
    initTheMachine = NULL;
    
    delete fetchingRecord;
    fetchingRecord = NULL;
    
    delete cuttingProcess;
    cuttingProcess = NULL;
    
    delete ejectingRecord;
    ejectingRecord = NULL;
    
    delete loading;
    loading = NULL;
    
    delete scheduler;
    scheduler = NULL;
    
    Serial.println(F("done!"));
    
    freeRam();
    
    delay(2000);
    
}





void calculateCuttingParameters() {
    
    
    float songGroove_pitch = SONGGROOVE_PITCH_7INCH;
    float startGroove_pitch = STARTGROOVE_PITCH_7INCH;
    float startGroove_range = STARTGROOVE_RANGE_7INCH;
    float endGroove_pitch = ENDGROOVE_PITCH_7INCH;
    float endGroove_range_min = ENDGROOVE_RANGE_MIN_7INCH;
    float cuttingRange = CUTTING_RANGE_7INCH;
    float playTime_minutes = PLAYTIME_MINUTES_7INCH;
    
    catCuttingStartPosition = CAT_CUTTING_START_POSITION_7INCH;
    
    if (digitalRead(CUTTING_POSTCARDS_SWITCH) == CUTTING_POSTCARDS_ACTIVE) {
        songGroove_pitch = SONGGROOVE_PITCH_CARD;
        startGroove_pitch = STARTGROOVE_PITCH_CARD;
        startGroove_range = STARTGROOVE_RANGE_CARD;
        endGroove_pitch = ENDGROOVE_PITCH_CARD;
        endGroove_range_min = ENDGROOVE_RANGE_MIN_CARD;
        cuttingRange = CUTTING_RANGE_CARD;
        playTime_minutes = PLAYTIME_MINUTES_CARD;
        catCuttingStartPosition = CAT_CUTTING_START_POSITION_CARD;
    }
    
    unsigned int recordTurnsPerMinute = 45;
    if (digitalRead(ROTATIONSPEED_33_SWITCH) == ROTATIONSPEED_33_ACTIVE) {
        recordTurnsPerMinute = 33;
    }
    
    unsigned int songRange = cuttingRange - startGroove_range - endGroove_range_min;
    unsigned int songGrooves = songRange / songGroove_pitch;
    unsigned int playTime_seconds = songGrooves * 60 / recordTurnsPerMinute;
    unsigned int endGrooveRange = endGroove_range_min;
    
    
    
    Serial.println();
    Serial.println("################################################################################");
    Serial.println("############################## CUTTING PARAMETERS ##############################");
    Serial.println("################################################################################");
    Serial.println();
    
    Serial.print(F("groove pitch is set to "));
    Serial.print(songGroove_pitch);
    Serial.print(F("mm, start groove pitch is set to "));
    Serial.print(startGroove_pitch);
    Serial.print(F("mm, start groove width is set to "));
    Serial.print(startGroove_range);
    Serial.print(F("mm, end groove pitch is set to "));
    Serial.print(endGroove_pitch);
    Serial.print(F("mm, minimal end groove width is set to "));
    Serial.print(endGroove_range_min);
    Serial.println(F("mm"));
    Serial.print(F("max. song width: "));
    Serial.print(songRange);
    Serial.print(F("mm, max. song grooves: "));
    Serial.print(songGrooves);
    Serial.print(F(", max. playing time: "));
    Serial.print(playTime_seconds / 60);
    Serial.print(F(":"));
    Serial.println(playTime_seconds % 60);
    
    
    if (playTime_minutes * 60 < playTime_seconds) {
        songGrooves = playTime_minutes * recordTurnsPerMinute;
        songRange = songGrooves * songGroove_pitch;
        endGrooveRange = cuttingRange - songRange - startGroove_range;
        playTime_seconds = playTime_minutes * 60;
        
        Serial.print(F("playing time is set to "));
        Serial.print(playTime_seconds / 60);
        Serial.print(F(":"));
        Serial.print(playTime_seconds % 60);
        Serial.print(F(", song width: "));
        Serial.print(songRange);
        Serial.print(F("mm, record grooves: "));
        Serial.println(songGrooves);
    } else {
        Serial.println(F("using maximal playing time!"));
    }
    
    
    grooves_all = startGroove_range / startGroove_pitch + songGrooves + endGrooveRange / endGroove_pitch;
    turnTableStepperDegrees = grooves_all * TABLE_DRIVE_RATIO * 360.0;
    //    turnTableStepperSpeed = recordTurnsPerMinute / 60.0 * TABLE_DRIVE_RATIO * 360.0;
    turnTableStepperSpeed = 6.0 * recordTurnsPerMinute * TABLE_DRIVE_RATIO;
    
    
    float catSpeed_start_mmPerSecond = startGroove_pitch * recordTurnsPerMinute / 60.0;
    float catMotorDegrees_start = 360.0 * startGroove_range / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    catMotorSpeed_startGroove = 360.0 * catSpeed_start_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    
    float catSpeed_song_mmPerSecond = songGroove_pitch * recordTurnsPerMinute / 60.0;
    float catMotorDegrees_song = 360.0 * songRange / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    catMotorSpeed_song = 360.0 * catSpeed_song_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    
    float catSpeed_end_mmPerSecond = endGroove_pitch * recordTurnsPerMinute / 60.0;
    float catMotorDegrees_end = 360.0 * endGrooveRange / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    catMotorSpeed_endGroove = 360.0 * catSpeed_end_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    
    
    catSongStartPosition = catCuttingStartPosition + catMotorDegrees_start;
    catSongEndPosition = catSongStartPosition + catMotorDegrees_song;
    catCuttingEndPosition = catCuttingStartPosition + cuttingRange * 360.0 / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
    
    
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
    // ============= pin configuration =======================================================================================================
    
    // FETCH_RECORD_BUTTON is initializes by scheduler
    pinMode(START_CUTTING_BUTTON, INPUT_PULLUP);
    // SONG_ENDBUTTON is initialized by scheduler
    
    pinMode(SKIP_UNLOADING_SWITCH, INPUT_PULLUP);
    pinMode(CUTTING_POSTCARDS_SWITCH, INPUT_PULLUP);
    pinMode(ROTATIONSPEED_33_SWITCH, INPUT_PULLUP);
    pinMode(MOVE_CAT_MANUALLY_SWITCH, INPUT_PULLUP);
    
    pinMode(EVALUATE_BUTTONS_SWITCH, INPUT_PULLUP);
    pinMode(RUN_TABLE_SWITCH, INPUT_PULLUP); // is the same as:
    pinMode(MATCH_HEADIMPACT_SWITCH, INPUT_PULLUP);
    
    pinMode(SCHEDULERS_TASKVIEW_SHOW_PIN, INPUT_PULLUP);
    
    pinMode(I_AM_LATE_LED, OUTPUT);
    
    pinMode(CONTROLLER_LAMP_YELLOW_PIN, OUTPUT);
    // CONTROLLER_LAMP_RED is initialized by scheduler
    
    
    
    // ============= prepare output =======================================================================================================
    
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

