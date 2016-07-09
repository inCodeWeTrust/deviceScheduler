//
//  deviceScheduler.ino
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//



#include <Arduino.h>
#include <Servo.h>

#include "voiceOGraph.h"
#include "CCDeviceScheduler.h"


#include <malloc.h>


// any chamges may happen here!


// ------------- globals -----------------------------------------------------------------------------------------------------
/// The Scheduler's device array.
/// Array of all devices.
CCDevice *device[8];
/// Array of all control-inputs.
CCControlButton *controlButton[8];

CCDeviceScheduler   *scheduler;

unsigned long       catSongStartPosition, catSongEndPosition, catCuttingEndPosition;
float               catMotorSpeed_startGroove, catMotorSpeed_song, catMotorSpeed_endGroove;
unsigned long       grooves_all;
float               turnTableStepperDegrees, turnTableStepperSpeed;



// ------------- prototypes --------------------------------------------------------------------------------------------------
void setup();
void calculateCuttingParameters();

void freeRam ();

extern char _end;
extern "C" char *sbrk(int i);


// ------------- main loop --------------------------------------------------------------------------------------------------
void loop() {
    
    Serial.println(), Serial.println(), Serial.println();
    for (int i = 0; i < 56; i++) Serial.print(F("#"));
    Serial.println(), Serial.println(), Serial.println();
    
    
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
    
    
    CCDevice* pumpServo = scheduler->addServo(SERVO_PUMP_NAME,
                                                    SERVO_PUMP_PIN,
                                                    SERVO_PUMP_MIN_POSITION,
                                                    SERVO_PUMP_MAX_POSITION,
                                                    PUMP_PARK_POSITION);
    
    /*
     schedulerDevice headLeftServo = scheduler->addServo(SERVO_HEAD_LEFT_NAME,
     SERVO_HEAD_LEFT_PIN,
     SERVO_HEAD_LEFT_MIN_POSITION,
     SERVO_HEAD_LEFT_MAX_POSITION,
     HEAD_LEFT_PARK_POSITION);
     
     
     schedulerDevice headRightServo = scheduler->addServo(SERVO_HEAD_RIGHT_NAME,
     SERVO_HEAD_RIGHT_PIN,
     SERVO_HEAD_RIGHT_MIN_POSITION,
     SERVO_HEAD_RIGHT_MAX_POSITION,
     HEAD_RIGHT_PARK_POSITION);
     
     
     
     
     schedulerDevice stockStepper = scheduler->addStepper_A4988(STEPPER_STOCK_NAME,
     STEPPER_STOCK_STEP_PIN,
     STEPPER_STOCK_DIR_PIN,
     STEPPER_STOCK_ENABLE_PIN,
     STEPPER_STOCK_HIGHEST_STEPPINGMODE,
     STEPPER_STOCK_STEPMODECODES,
     STEPPER_STOCK_MICROSTEPPINS,
     STEPPER_STOCK_STEPS_PER_ROTATION);
     scheduler->device[stockStepper]->defineDefaults(STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL);
     
     
     schedulerDevice catStepper = scheduler->addStepper_TMC260(STEPPER_CAT_NAME,
     STEPPER_CAT_STEP_PIN,
     STEPPER_CAT_DIR_PIN,
     STEPPER_CAT_ENABLE_PIN,
     STEPPER_CAT_CHIPSELECT_PIN,
     STEPPER_CAT_CURRENT,
     STEPPER_CAT_STEPS_PER_ROTATION);
     scheduler->device[catStepper]->defineDefaults(CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
     
     
     schedulerDevice tableStepper = scheduler->addStepper_A4988(STEPPER_TABLE_NAME,
     STEPPER_TABLE_STEP_PIN,
     STEPPER_TABLE_DIR_PIN,
     STEPPER_TABLE_ENABLE_PIN,
     STEPPER_TABLE_HIGHEST_STEPPINGMODE,
     STEPPER_TABLE_STEPMODECODES,
     STEPPER_TABLE_MICROSTEPPINS,
     STEPPER_TABLE_STEPS_PER_ROTATION);
     
     
     schedulerDevice conveyStepper = scheduler->addStepper_A4988(STEPPER_CONVEYOR_NAME,
     STEPPER_CONVEYOR_STEP_PIN,
     STEPPER_CONVEYOR_DIR_PIN,
     STEPPER_CONVEYOR_ENABLE_PIN,
     STEPPER_CONVEYOR_HIGHEST_STEPPINGMODE,
     STEPPER_CONVEYOR_STEPMODECODES,
     STEPPER_CONVEYOR_MICROSTEPPINS,
     STEPPER_CONVEYOR_STEPS_PER_ROTATION);
     scheduler->device[conveyStepper]->defineDefaults(CONVEYOR_SPEED, CONVEYOR_ACCEL);
     
     
     
     
     schedulerDevice vacuumCleaner = scheduler->addDcController(VACCUUMCLEANER_NAME,
     VACCUUMCLEANER_PIN,
     VACCUUMCLEANER_ACTIV);
     
     
     schedulerDevice startingSoonLamp = scheduler->addDcController(CONTROLLER_LAMP_RED_NAME,
     CONTROLLER_LAMP_RED_PIN,
     CONTROLLER_LAMP_RED_ACTIV);
     
     
     */
     
     CCControlButton* recordAvailableButton = scheduler->addControlButton(RECORDAVAILABLE_BUTTON_NAME,
     RECORDAVAILABLE_BUTTON_PIN,
     RECORDAVAILABLE_BUTTON_ACTIV,
     RECORDAVAILABLE_BUTTON_PULLUP);
     
     CCControlButton* stockTopButton = scheduler->addControlButton(STOCKTOP_BUTTON_NAME,
     STOCKTOP_BUTTON_PIN,
     STOCKTOP_BUTTON_ACTIV,
     STOCKTOP_BUTTON_PULLUP);
     
     CCControlButton* stockBottomButton = scheduler->addControlButton(STOCKBOTTOM_BUTTON_NAME,
     STOCKBOTTOM_BUTTON_PIN,
     STOCKBOTTOM_BUTTON_ACTIV,
     STOCKBOTTOM_BUTTON_PULLUP);
     
     /*
     
     schedulerControlButton bridgeParkButton = scheduler->addControlButton(CAT_PARKBUTTON_NAME,
     CAT_PARKBUTTON_PIN,
     CAT_PARKBUTTON_ACTIV,
     CAT_PARKBUTTON_PULLUP);
     
     schedulerControlButton bridgeEndButton = scheduler->addControlButton(CAT_ENDBUTTON_NAME,
     CAT_ENDBUTTON_PIN,
     CAT_ENDBUTTON_ACTIV,
     CAT_ENDBUTTON_PULLUP);
     
     schedulerControlButton songEndButton = scheduler->addControlButton(SONG_ENDBUTTON_NAME,
     SONG_ENDBUTTON_PIN,
     SONG_ENDBUTTON_ACTIV,
     SONG_ENDBUTTON_PULLUP);
     
     schedulerControlButton songCancelButton = scheduler->addControlButton(SONG_CANCELBUTTON_NAME,
     SONG_CANCELBUTTON_PIN,
     SONG_CANCELBUTTON_ACTIV,
     SONG_CANCELBUTTON_PULLUP);
     
     
     */
    scheduler->getAllDevices();
    scheduler->getAllControlButtons();
    
    
        // ============================================================================================================================
        // ============= initialisation of fetchingRecord: ============================================================================
        // ============================================================================================================================
        
        //  not needed
        
        
        // ============================================================================================================================
        // ============= tasks of fetchingRecord: =====================================================================================
        // ============================================================================================================================
        
        
        CCWorkflow *fetchingRecord = new CCWorkflow("fetchingRecord");
    {
       CCDeviceFlow *liftServoFlow = fetchingRecord->addDeviceFlow("liftServoFlow", liftServo, 100, 101, 102);
//        CCDeviceFlow *turnServoFlow = fetchingRecord->addDeviceFlow("turnServoFlow", turnServo, 200, 201, 202);
        CCDeviceFlow *pumpServoFlow = fetchingRecord->addDeviceFlow("pumpServoFlow", pumpServo, 300, 301, 302);
        
        
        //  lift grappler
        CCTask* liftFromParkPosition;
        liftFromParkPosition = liftServoFlow->addTask(LIFT_PARK_POSITION + 500, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
        liftFromParkPosition->startByDate(100);
        
        /*
         //  move stock down first
         scheduledTask moveStockStepperDown = fetchingRecord->device[stockStepper]->addTaskMoveRelativ(-4000);
         fetchingRecord->device[stockStepper]->task[moveStockStepperDown]->startByDate(100);
         fetchingRecord->device[stockStepper]->task[moveStockStepperDown]->stopByButton(stockBottomButton, STOP_NORMAL);
         
         //  supply a new record, terminated by recordAvailableButton
         scheduledTask supplyRecord = fetchingRecord->device[stockStepper]->addTaskMoveRelativ(16000);
         fetchingRecord->device[stockStepper]->task[supplyRecord]->startAfterCompletionOf(stockStepper, moveStockStepperDown);
         fetchingRecord->device[stockStepper]->task[supplyRecord]->stopByButton(recordAvailableButton, STOP_NORMAL);
         */
        
        //  lower grappler to stock: start when record is available
        CCTask* lowerToStock;
        lowerToStock = liftServoFlow->addTask(LIFT_STOCK_POSITION);
//        lowerToStock->startByButton(recordAvailableButton);
        lowerToStock->startAfterCompletionOf(liftServo, liftFromParkPosition);
        
         //  pump: prepare
        CCTask* pumpForGrip_down;
        pumpForGrip_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
        pumpForGrip_down->startByTriggerpositionOf(liftServo, lowerToStock, LIFT_STOCK_POSITION + 60);
//        pumpForGrip_down->startAfterCompletionOf(liftServo, lowerToStock);
        
         //  pump: make vaccum
        CCTask* pumpForGrip_up;
        pumpForGrip_up = pumpServoFlow->addTask(PUMP_PARK_POSITION);
        pumpForGrip_up->startAfterCompletionOf(pumpServo, pumpForGrip_down);
         
         //  lift the new record
        CCTask* liftNewRecord;
        liftNewRecord = liftServoFlow->addTask(LIFT_UP_POSITION);
        liftNewRecord->startAfterCompletionOf(pumpServo, pumpForGrip_up);
        
        /*

         //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
         scheduledTask turnRecordToTable = fetchingRecord->device[turnServo]->addTask(TURN_TABLE_POSITION);
         fetchingRecord->device[turnServo]->task[turnRecordToTable]->startAfterCompletionOf(liftServo, liftNewRecord);
         
         //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
         scheduledTask lowerRecordToTable = fetchingRecord->device[liftServo]->addTask(LIFT_TABLE_POSITION);
         fetchingRecord->device[liftServo]->task[lowerRecordToTable]->startByTriggerpositionOf(turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
         */
         //  release new record: release vacuum
        CCTask* pumpForRelease_down;
        pumpForRelease_down = pumpServoFlow->addTask(PUMP_DOWN_POSITION);
        pumpForRelease_down->startAfterCompletionOf(pumpServo, pumpForGrip_up);
        pumpForRelease_down->setStartDelay(500);
//        pumpForRelease_down->startAfterCompletionOf(liftServo, lowerRecordToTable);
         /*
         //  lift for going to park position: start when vacuum was released
         scheduledTask liftForParkPosition = fetchingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         fetchingRecord->device[liftServo]->task[liftForParkPosition]->startAfterCompletionOf(pumpServo, pumpForRelease_down);
         
         //  release pump when record is placed
         scheduledTask pumpForRelease_up = fetchingRecord->device[pumpServo]->addTask(PUMP_PARK_POSITION);
         fetchingRecord->device[pumpServo]->task[pumpForRelease_up]->startAfterCompletionOf(liftServo, liftForParkPosition);
         
         //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
         scheduledTask turnToStockPosition = fetchingRecord->device[turnServo]->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
         fetchingRecord->device[turnServo]->task[turnToStockPosition]->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
         
         //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
         scheduledTask lowerForParkPosition = fetchingRecord->device[liftServo]->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         fetchingRecord->device[liftServo]->task[lowerForParkPosition]->startByTriggerpositionOf(turnServo, turnToStockPosition, TURN_TO_STOCK_TRIGGER_LIFT);
         

         
         //  cancel loading if stockBottomButton is pressed while lowering
         fetchingRecord->controlButton[stockTopButton]->evokeBreak(stockStepper, moveStockStepperDown, 66, "stock empty!");
         
         
         */
        
        fetchingRecord->addControllButton(stockBottomButton);
        CCAction* stockBottomAction = stockBottomButton->addAction("stockBottomAction");
        stockBottomAction->evokeBreak();
        
        
//         fetchingRecord->reviewTasks();
        scheduler->getAllTasksOfAllDeviceFlowsOfWorkflow(fetchingRecord);
//         fetchingRecord->getAllActions();
        
        freeRam();
    }
    
    
    
    
    
    // ================================================================================================================================
    // ============= create cuttingProcess scheduler object: ==========================================================================
    // ================================================================================================================================
    
    //    cuttingProcess = new CCDeviceScheduler("cuttingProcess");
    
    
    {
        // ============================================================================================================================
        // ============= devices of cuttingProcess: ===================================================================================
        // ============================================================================================================================
        
        
        
        
        // ============================================================================================================================
        // ============= initialisation of cuttingProcess: =====================================================================================
        // ============================================================================================================================
        
        
        /*
         Serial.println("................................. initialisation of catStepper ...................................");
         
         
         scheduledTask initCatStepper = cuttingProcess->device[catStepper]->addTaskWithPositionReset(-400000);
         cuttingProcess->device[catStepper]->task[initCatStepper]->startByDate(100);
         cuttingProcess->device[catStepper]->task[initCatStepper]->stopByButton(bridgeParkButton, STOP_NORMAL);
         
         cuttingProcess->reviewTasks();
         cuttingProcess->getAllTasks();
         
         
         cuttingProcess->run();
         
         cuttingProcess->device[catStepper]->setCurrentPosition(0.0);
         
         cuttingProcess->deleteAllTasks();
         cuttingProcess->deleteAllActions();
         
         
         Serial.println("...................................... done ......................................................");
         
         
         */
        
        
        // ============================================================================================================================
        // ============= tasks of cuttingProcess: =====================================================================================
        // ============================================================================================================================
        
        /*
         //  (0) blink lamp for start cutting soon:
         scheduledTask blinkForCuttingSoon = cuttingProcess->device[startingSoonLamp]->addTask(0.5, 0.3, 0.0);
         cuttingProcess->device[startingSoonLamp]->task[blinkForCuttingSoon]->startByDate(100);
         
         //  move to start groove:
         scheduledTask driveToCuttingStartPosition = cuttingProcess->device[catStepper]->addTask(CAT_CUTTING_START_POSITION);
         cuttingProcess->device[catStepper]->task[driveToCuttingStartPosition]->startByDate(100);
         
         // switch on vacuum device
         scheduledTask hooverTheFlake = cuttingProcess->device[vacuumCleaner]->addTask(1.0, 1.0, 0.0);
         cuttingProcess->device[vacuumCleaner]->task[hooverTheFlake]->startByDate(500);
         
         //  turn the table:
         scheduledTask turnTheTable = cuttingProcess->device[tableStepper]->addTaskWithPositionReset(turnTableStepperDegrees + 17200.0, turnTableStepperSpeed, TABLE_STEP_ACCEL);
         cuttingProcess->device[tableStepper]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
         
         //  (1) blink lamp for start cutting very soon:
         scheduledTask blinkForCuttingVerySoon = cuttingProcess->device[startingSoonLamp]->addTask(0.5, 1.0, 0.0);
         cuttingProcess->device[startingSoonLamp]->task[blinkForCuttingSoon]->switchToNextTaskAfterCompletionOf(catStepper, driveToCuttingStartPosition);
         
         //  lower head close to record surface (left servo): start when reached start position of start groove
         scheduledTask lowerHeadLeftForCutting = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
         cuttingProcess->device[headLeftServo]->task[lowerHeadLeftForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
         
         //  approximate head to record surface (right servo): start when left servo reached cutting position
         scheduledTask approximateHeadRightForCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
         cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
         //        cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->stopDynamicallyBySensor(HEAD_INCLINATION_SENSOR, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
         
         //  (2) blink lamp for start cutting very very soon:
         scheduledTask blinkForCuttingVeryVerySoon = cuttingProcess->device[startingSoonLamp]->addTask(0.5, 3.0, 0.0);
         cuttingProcess->device[startingSoonLamp]->task[blinkForCuttingVerySoon]->switchToNextTaskAfterCompletionOf(headRightServo, approximateHeadRightForCutting);
         
         //  make start groove:
         scheduledTask makeStartGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_startGroove, CAT_ACCEL_SLOW);
         cuttingProcess->device[catStepper]->task[makeStartGroove]->startAfterCompletionOf(headRightServo, approximateHeadRightForCutting);
         cuttingProcess->device[catStepper]->task[makeStartGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeStartGroove, catSongStartPosition);
         
         //  (3) keep blinking lamp on for whole cutting process:
         scheduledTask keepLampOn = cuttingProcess->device[startingSoonLamp]->addTask(1.0, 1.0, 0.0);
         cuttingProcess->device[startingSoonLamp]->task[blinkForCuttingVeryVerySoon]->switchToNextTaskAfterCompletionOf(catStepper, makeStartGroove);
         
         //  make song groove:
         scheduledTask makeMainGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_song, CAT_ACCEL_SLOW);
         cuttingProcess->device[catStepper]->task[makeMainGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition);
         
         //  (4) blink lamp for finishing soon:
         scheduledTask blinkForFinishingSoon = cuttingProcess->device[startingSoonLamp]->addTask(0.9, 0.3, 0.0);
         cuttingProcess->device[startingSoonLamp]->task[keepLampOn]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition - 10000);
         
         //  (5) blink lamp for finishing very soon:
         scheduledTask blinkForFinishingVerySoon = cuttingProcess->device[startingSoonLamp]->addTask(0.9, 1.0, 0.0);
         cuttingProcess->device[startingSoonLamp]->task[blinkForFinishingSoon]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition - 5000);
         
         //  (5) blink lamp for finishing very soon:
         scheduledTask blinkForFinishingVeryVerySoon = cuttingProcess->device[startingSoonLamp]->addTask(0.9, 3.0, 0.0);
         cuttingProcess->device[startingSoonLamp]->task[blinkForFinishingVerySoon]->switchToNextTaskAfterCompletionOf(catStepper, makeMainGroove);
         
         //  make end groove:
         scheduledTask makeEndGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_endGroove, CAT_ACCEL_NORMAL);
         
         //  switch off blinking lamp:
         cuttingProcess->device[startingSoonLamp]->task[blinkForFinishingVeryVerySoon]->stopAfterCompletionOf(catStepper, makeEndGroove, STOP_IMMEDIATELY);
         
         //  switch off vacuum cleaner
         cuttingProcess->device[vacuumCleaner]->task[hooverTheFlake]->stopAfterCompletionOf(catStepper, makeEndGroove, STOP_IMMEDIATELY);
         
         //  lift head after cutting above the record surface:
         scheduledTask liftHeadRightAfterCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW);
         cuttingProcess->device[headRightServo]->task[liftHeadRightAfterCutting]->startAfterCompletionOf(catStepper, makeEndGroove);
         
         //  lift head into park position:
         scheduledTask liftHeadLeftAfterCutting = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW);
         cuttingProcess->device[headLeftServo]->task[liftHeadLeftAfterCutting]->startAfterCompletionOf(headRightServo, liftHeadRightAfterCutting);
         
         //  switch off turn table
         cuttingProcess->device[tableStepper]->task[turnTheTable]->stopAfterCompletionOf(headRightServo, liftHeadRightAfterCutting, STOP_NORMAL);
         
         //  drive cat back home
         scheduledTask driveToParkPosition = cuttingProcess->device[catStepper]->addTask(CAT_PARK_POSITION);
         cuttingProcess->device[catStepper]->task[driveToParkPosition]->startAfterCompletionOf(headRightServo, liftHeadRightAfterCutting);
         cuttingProcess->device[catStepper]->task[driveToParkPosition]->stopByButton(bridgeParkButton, STOP_NORMAL);
         
         
         
         //  jump over to end groove if songEndButton is pressed
         cuttingProcess->controlButton[songEndButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_TASK_AND_SWITCH, makeEndGroove, 32, "songEndButton was pressed");
         
         //  cancel cutting if songCancelButton is pressed
         cuttingProcess->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeStartGroove, STOP_TASK_AND_SWITCH, driveToParkPosition);
         cuttingProcess->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_TASK_AND_SWITCH, driveToParkPosition);
         cuttingProcess->controlButton[songCancelButton]->evokeTaskJump(catStepper, makeEndGroove, STOP_TASK);
         
         
         
         cuttingProcess->reviewTasks();
         cuttingProcess->getAllTasks();
         cuttingProcess->getAllActions();
         
         */
        freeRam();
        
    }
    
    
    
    // ================================================================================================================================
    // ============= create ejectingRecord scheduler object: ==========================================================================
    // ================================================================================================================================
    
    //    ejectingRecord = new CCDeviceScheduler("ejectingRecord");
    
    
    {
        // ============================================================================================================================
        // ============= devices of ejectingRecord: ===================================================================================
        // ============================================================================================================================
        /*
         schedulerDevice liftServo = ejectingRecord->addServoWithCounterServo(SERVO_LIFT_NAME,
         SERVO_LIFT_LEFT_PIN,
         SERVO_LIFT_LEFT_MIN_POSITION,
         SERVO_LIFT_LEFT_MID_POSITION,
         SERVO_LIFT_LEFT_MAX_POSITION,
         SERVO_LIFT_RIGHT_PIN,
         SERVO_LIFT_RIGHT_MIN_POSITION,
         SERVO_LIFT_RIGHT_MID_POSITION,
         SERVO_LIFT_RIGHT_MAX_POSITION,
         LIFT_PARK_POSITION);
         ejectingRecord->device[liftServo]->defineDefaults(LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
         
         
         schedulerDevice turnServo = ejectingRecord->addServo(SERVO_TURN_NAME,
         SERVO_TURN_PIN,
         SERVO_TURN_MIN_POSITION,
         SERVO_TURN_MAX_POSITION,
         TURN_STOCK_POSITION);
         ejectingRecord->device[turnServo]->defineDefaults(TURN_SPEED_SLOW, TURN_ACCEL_SLOW);
         
         
         schedulerDevice pumpServo = ejectingRecord->addServo(SERVO_PUMP_NAME,
         SERVO_PUMP_PIN,
         SERVO_PUMP_MIN_POSITION,
         SERVO_PUMP_MAX_POSITION,
         PUMP_PARK_POSITION);
         ejectingRecord->device[pumpServo]->defineDefaults(PUMP_SPEED, PUMP_ACCEL);
         
         
         
         
         ejectingRecord->getAllDevices();
         ejectingRecord->getAllControlButtons();
         
         */
        
        // ============================================================================================================================
        // ============= initialisation of ejectingRecord: ============================================================================
        // ============================================================================================================================
        
        //  not needed
        
        
        // ============================================================================================================================
        // ============= tasks of ejectingRecord: =====================================================================================
        // ============================================================================================================================
        /*
         //  remove record from turntable: started after cuttingProcess is finished
         
         // lift grappler: start when cuttingProcess is finished
         scheduledTask liftFromParkPosition = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         ejectingRecord->device[liftServo]->task[liftFromParkPosition]->startByDate(100);
         
         //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
         scheduledTask turnToTable = ejectingRecord->device[turnServo]->addTask(TURN_TABLE_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
         ejectingRecord->device[turnServo]->task[turnToTable]->startByTriggerpositionOf(liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
         
         //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
         scheduledTask lowerToTable = ejectingRecord->device[liftServo]->addTask(LIFT_TABLE_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         ejectingRecord->device[liftServo]->task[lowerToTable]->startByTriggerpositionOf(turnServo, turnToTable, TURN_TO_TABLE_TRIGGER_LIFT);
         
         // pump prepare
         scheduledTask pumpForGrip_down = ejectingRecord->device[pumpServo]->addTask(PUMP_DOWN_POSITION);
         ejectingRecord->device[pumpServo]->task[pumpForGrip_down]->startAfterCompletionOf(liftServo, lowerToTable);
         
         // pump make vaccum
         scheduledTask pumpForGrip_up = ejectingRecord->device[pumpServo]->addTask(PUMP_PARK_POSITION);
         ejectingRecord->device[pumpServo]->task[pumpForGrip_up]->startAfterCompletionOf(pumpServo, pumpForGrip_down);
         
         //  lift the cutted record: start with startDelay after table was reached (LIFT_TABLE_POSITION)
         scheduledTask liftCuttedRecord = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION);
         ejectingRecord->device[liftServo]->task[liftCuttedRecord]->startAfterCompletionOf(pumpServo, pumpForGrip_up);
         
         //  turn grappler to conveyer position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
         scheduledTask turnToConveyor = ejectingRecord->device[turnServo]->addTask(TURN_CONVEYOR_POSITION);
         ejectingRecord->device[turnServo]->task[turnToConveyor]->startByTriggerpositionOf(liftServo, liftCuttedRecord, LIFT_UP_TRIGGER_TURN);
         
         //  lower grappler to conveyer position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
         scheduledTask lowerToConveyor = ejectingRecord->device[liftServo]->addTask(LIFT_CONVEYER_POSITION);
         ejectingRecord->device[liftServo]->task[lowerToConveyor]->startByTriggerpositionOf(turnServo, turnToConveyor, TURN_TO_CONVEYER_TRIGGER_LIFT);
         
         //  release new record, release vacuum
         scheduledTask pumpForRelease_down = ejectingRecord->device[pumpServo]->addTask(PUMP_DOWN_POSITION);
         ejectingRecord->device[pumpServo]->task[pumpForRelease_down]->startAfterCompletionOf(liftServo, lowerToConveyor);
         
         //  lift for parking
         scheduledTask liftForParkPosition = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         ejectingRecord->device[liftServo]->task[liftForParkPosition]->startAfterCompletionOf(liftServo, lowerToConveyor);
         
         //  move it to the user
         scheduledTask driveIt = ejectingRecord->device[conveyStepper]->addTaskWithPositionReset(CONVEYOR_DISTANCE, CONVEYOR_SPEED, CONVEYOR_ACCEL);
         ejectingRecord->device[conveyStepper]->task[driveIt]->startAfterCompletionOf(liftServo, liftForParkPosition);
         
         //  release pump
         scheduledTask pumpForRelease_up = ejectingRecord->device[pumpServo]->addTask(PUMP_PARK_POSITION);
         ejectingRecord->device[pumpServo]->task[pumpForRelease_up]->startAfterCompletionOf(liftServo, liftForParkPosition);
         
         //  turn grappler for parking
         scheduledTask turnToStock = ejectingRecord->device[turnServo]->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
         ejectingRecord->device[turnServo]->task[turnToStock]->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
         
         //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
         scheduledTask lowerToPark = ejectingRecord->device[liftServo]->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
         ejectingRecord->device[liftServo]->task[lowerToPark]->startByTriggerpositionOf(turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
         
         
         
         ejectingRecord->reviewTasks();
         ejectingRecord->getAllTasks();
         ejectingRecord->getAllActions();
         
         */
        freeRam();
        
    }
    
    
    
    
    
    
    
    
    
    
    // ============================================================================================================================
    // ============================================================================================================================
    // ============= operation: ===================================================================================================
    // ============================================================================================================================
    // ============================================================================================================================
    
    
    Serial.println(F("ready to run!"));
    scheduler->run(fetchingRecord);
    /*
     boolean initNeeded = false;
     int i = 0;
     while (!initNeeded) {
     if (digitalRead(FETCH_RECORD_BUTTON) == LOW) {
     Serial.println("go for a brand new record");
     
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
     
     fetchingRecord->run();
     
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
     
     }
     
     
     if (digitalRead(START_CUTTING_BUTTON) == LOW) {
     Serial.println("go for cut and scratch");
     
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
     
     cuttingProcess->run();
     
     ejectingRecord->run();
     
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
     
     }
     
     if (digitalRead(LOADING_BUTTON) == LOW) {
     Serial.println("go for loading");
     
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
     
     {
     CCDeviceScheduler *loading = new CCDeviceScheduler("loading");
     
     schedulerDevice stockStepper = loading->addStepper_A4988(STEPPER_STOCK_NAME,
     STEPPER_STOCK_STEP_PIN,
     STEPPER_STOCK_DIR_PIN,
     STEPPER_STOCK_ENABLE_PIN,
     STEPPER_STOCK_HIGHEST_STEPPINGMODE,
     STEPPER_STOCK_STEPMODECODES,
     STEPPER_STOCK_MICROSTEPPINS,
     STEPPER_STOCK_STEPS_PER_ROTATION);
     loading->device[stockStepper]->defineDefaults(STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL);
     
     
     schedulerControlButton stockBottomButton = loading->addControlButton(STOCKBOTTOM_BUTTON_NAME,
     STOCKBOTTOM_BUTTON_PIN,
     STOCKBOTTOM_BUTTON_ACTIV,
     STOCKBOTTOM_BUTTON_PULLUP);
     
     
     schedulerControlButton recordAvailableButton = loading->addControlButton(RECORDAVAILABLE_BUTTON_NAME,
     RECORDAVAILABLE_BUTTON_PIN,
     RECORDAVAILABLE_BUTTON_ACTIV,
     RECORDAVAILABLE_BUTTON_PULLUP);
     
     schedulerControlButton stockTopButton = loading->addControlButton(STOCKTOP_BUTTON_NAME,
     STOCKTOP_BUTTON_PIN,
     STOCKTOP_BUTTON_ACTIV,
     STOCKTOP_BUTTON_PULLUP);
     
     schedulerControlButton loadingButton = loading->addControlButton("loadingButton", LOADING_BUTTON, HIGH, true);
     
     
     scheduledTask driveDown = loading->device[stockStepper]->addTask(-80000);
     loading->device[stockStepper]->task[driveDown]->startByDate(100);
     loading->device[stockStepper]->task[driveDown]->stopByButton(stockBottomButton);
     
     scheduledTask comeUpAgain = loading->device[stockStepper]->addTask(80000);
     loading->device[stockStepper]->task[comeUpAgain]->startAfterCompletionOf(stockStepper, driveDown);
     loading->device[stockStepper]->task[comeUpAgain]->stopByButton(stockTopButton);
     
     loading->controlButton[loadingButton]->evokeTaskJump(stockStepper, driveDown, STOP_TASK);
     loading->controlButton[recordAvailableButton]->evokeTaskJump(stockStepper, comeUpAgain, STOP_TASK);
     
     loading->reviewTasks();
     loading->run();
     
     delete loading;
     loading = NULL;
     }
     
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
     
     }
     
     if (digitalRead(MOVE_MANUALLY_SWITCH) == LOW) {
     Serial.println("go for mamual moving");
     
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
     
     {
     CCDeviceScheduler *manualDrive = new CCDeviceScheduler("manualDrive");
     
     schedulerDevice catStepper = manualDrive->addStepper_TMC260(STEPPER_CAT_NAME,
     STEPPER_CAT_STEP_PIN,
     STEPPER_CAT_DIR_PIN,
     STEPPER_CAT_ENABLE_PIN,
     STEPPER_CAT_CHIPSELECT_PIN,
     STEPPER_CAT_CURRENT,
     STEPPER_CAT_STEPS_PER_ROTATION);
     manualDrive->device[catStepper]->defineDefaults(CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
     
     schedulerControlButton catForwardButton = manualDrive->addControlButton("catForward", MOVE_CAT_FWD_BUTTON, HIGH, true);
     schedulerControlButton catRewindButton = manualDrive->addControlButton("catRewind", MOVE_CAT_RWD_BUTTON, HIGH, true);
     
     
     //  move to start groove:
     scheduledTask driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(CAT_CUTTING_START_POSITION);
     manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(100);
     
     manualDrive->reviewTasks();
     manualDrive->run();
     manualDrive->deleteAllTasks();
     
     while (digitalRead(START_CUTTING_BUTTON) == HIGH) {
     if (digitalRead(MOVE_CAT_FWD_BUTTON) == LOW) {
     driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(catCuttingEndPosition);
     manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
     manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(catForwardButton, STOP_NORMAL);
     
     manualDrive->reviewTasks();
     manualDrive->run();
     manualDrive->deleteAllTasks();
     }
     if (digitalRead(MOVE_CAT_RWD_BUTTON) == LOW) {
     driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(CAT_PARK_POSITION);
     manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
     manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(catRewindButton, STOP_NORMAL);
     
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
     
     }
     
     if (digitalRead(MATCH_HEADIMPACT_SWITCH) == LOW) {
     Serial.println("go for matching head impact");
     
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
     
     }
     
     if (i > 400000) {
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_ON);
     if (i > 410000) {
     digitalWrite(CONTROLLER_LAMP_YELLOW_PIN, CONTROLLER_LAMP_YELLOW_OFF);
     i = 0;
     Serial.print(".");
     }
     }
     i++;
     
     }
     */
    
     delay(2000);
     
     
     delete fetchingRecord;
     fetchingRecord = NULL;
     
    /*
     delete cuttingProcess;
     cuttingProcess = NULL;
     
     delete ejectingRecord;
     ejectingRecord = NULL;
     
     */
    freeRam();
    
    delay(2000);
    
}

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
    Serial.print(SONGGROOVE_PITCH);
    Serial.print(F("mm, start groove pitch is set to "));
    Serial.print(STARTGROOVE_PITCH);
    Serial.print(F("mm, start groove width is set to "));
    Serial.print(STARTGROOVE_RANGE);
    Serial.print(F("mm, end groove pitch is set to "));
    Serial.print(ENDGROOVE_PITCH);
    Serial.print(F("mm, minimal end groove width is set to "));
    Serial.print(ENDGROOVE_RANGE_MIN);
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
    
    pinMode(I_AM_LATE_LED, OUTPUT);
    
    pinMode(CONTROLLER_LAMP_YELLOW_PIN, OUTPUT);
    
    
    Serial.begin(115200);
    Serial.println(), Serial.println(), Serial.println();
    Serial.println(), Serial.println(), Serial.println();
    
    calculateCuttingParameters();
    
}

void freeRam() {
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
}



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

