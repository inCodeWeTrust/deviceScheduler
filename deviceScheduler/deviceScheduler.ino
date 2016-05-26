
/// @mainpage	deviceScheduler
///
/// @details	schedule and drive stepper-, servo- and other devices
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		little Ikarus
/// @author		___FULLUSERNAME___
/// @date		01.06.15 12:52
/// @version	<#version#>
///
/// @copyright	(c) little Ikarus, 2015
/// @copyright	License
///
/// @see		ReadMe.txt for references
///


///
/// @file		deviceScheduler.ino
/// @brief		Main sketch
///
/// @details	<#details#>
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		little Ikarus
/// @author		___FULLUSERNAME___
/// @date		01.06.15 12:52
/// @version	<#version#>
///
/// @copyright	(c) little Ikarus, 2015
/// @copyright	License
///
/// @see		ReadMe.txt for references
/// @n
///


#include <Arduino.h>

// Include application, user and local libraries
#include <Servo.h>

#include "deviceScheduler.h"
#include "CCDeviceScheduler.h"


#include "freeRam.h"

// Prototypes


// Define variables and constants
unsigned long catSongStartPosition, catSongEndPosition, catCuttingEndPosition;
float catMotorSpeed_startGroove, catMotorSpeed_song, catMotorSpeed_endGroove;
unsigned long grooves_all;
float turnTableStepperDegrees, turnTableStepperSpeed;





// ------------- globals -----------------------------------------------------------------------------------------------------
CCDeviceScheduler *cuttingProcess;
CCDeviceScheduler *fetchingRecord;
CCDeviceScheduler *ejectingRecord;

// ------------- prototypes --------------------------------------------------------------------------------------------------
void setup();
void calculateCuttingParameters();



void loop() {
    
    Serial.println(), Serial.println(), Serial.println();
    for (int i = 0; i < 56; i++) Serial.print(F("#"));
    Serial.println(), Serial.println(), Serial.println();
    
    
    freeRam();
    
    // ============================================================================================================================
    // ============= create fetchingRecord scheduler object: ======================================================================
    // ============================================================================================================================
    
    fetchingRecord = new CCDeviceScheduler("fetchingRecord");
    
    
    {
    // ============================================================================================================================
    // ============= devices of fetchingRecord: ===================================================================================
    // ============================================================================================================================
    
    schedulerDevice liftServo = fetchingRecord->addServo(SERVO_LIFT_NAME,
                                                    SERVO_LIFT_PIN,
                                                    SERVO_LIFT_MIN_POSITION,
                                                    SERVO_LIFT_MAX_POSITION,
                                                    LIFT_PARK_POSITION);
    
    schedulerDevice turnServo = fetchingRecord->addServo(SERVO_TURN_NAME,
                                                    SERVO_TURN_PIN,
                                                    SERVO_TURN_MIN_POSITION,
                                                    SERVO_TURN_MAX_POSITION,
                                                    TURN_PARK_POSITION);
    
    schedulerDevice stockStepper = fetchingRecord->addStepper_A4988(STEPPER_STOCK_NAME,
                                                               STEPPER_STOCK_STEP_PIN,
                                                               STEPPER_STOCK_DIR_PIN,
                                                               STEPPER_STOCK_ENABLE_PIN,
                                                               STEPPER_STOCK_HIGHEST_STEPPINGMODE,
                                                               STEPPER_STOCK_STEPMODECODES,
                                                               STEPPER_STOCK_MICROSTEPPINS,
                                                               STEPPER_STOCK_STEPS_PER_ROTATION);
    
   
    
    schedulerControlButton recordAvailableButton = fetchingRecord->addControlButton(RECORDAVAILABLE_BUTTON_NAME, RECORDAVAILABLE_BUTTON_PIN, RECORDAVAILABLE_BUTTON_ACTIV, RECORDAVAILABLE_BUTTON_PULLUP);
    schedulerControlButton stockTopButton = fetchingRecord->addControlButton(STOCKTOP_BUTTON_NAME, STOCKTOP_BUTTON_PIN, STOCKTOP_BUTTON_ACTIV, STOCKTOP_BUTTON_PULLUP);
    schedulerControlButton stockBottomButton = fetchingRecord->addControlButton(STOCKBOTTOM_BUTTON_NAME, STOCKBOTTOM_BUTTON_PIN, STOCKBOTTOM_BUTTON_ACTIV, STOCKBOTTOM_BUTTON_PULLUP);
    
    fetchingRecord->getAllDevices();
    fetchingRecord->getAllControlButtons();

    
    
    // ============================================================================================================================
    // ============= initialisation of fetchingRecord: ============================================================================
    // ============================================================================================================================
    
    if (true) {
        
        Serial.println("................................. initialisation of stockStepper .................................");

        scheduledTask initStockStepper_down = fetchingRecord->device[stockStepper]->addTask(-10000, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, STOCK_SUPPLY_RECORD_ACCEL);
        fetchingRecord->device[stockStepper]->task[initStockStepper_down]->startByDate(100);
        fetchingRecord->device[stockStepper]->task[initStockStepper_down]->stopByButton(stockBottomButton, STOP_NORMAL);
        
//        scheduledTask initStockStepper_up = fetchingRecord->device[stockStepper]->addTaskWithPositionReset(140000, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, STOCK_SUPPLY_RECORD_ACCEL);
//        fetchingRecord->device[stockStepper]->task[initStockStepper_up]->startAfterCompletionOf(stockStepper, initStockStepper_down);
//        fetchingRecord->device[stockStepper]->task[initStockStepper_up]->stopByButton(recordAvailableButton, STOP_NORMAL);
        

        
        fetchingRecord->reviewTasks();
        fetchingRecord->getAllTasks();
        
        
        fetchingRecord->run();
        
        
        fetchingRecord->deleteAllTasks();
        fetchingRecord->deleteAllActions();
        
        
        Serial.println("...................................... done ......................................................");
    }
    
    freeRam();
    
    
    
    
    
    // ============================================================================================================================
    // ============= tasks of fetchingRecord: =====================================================================================
    // ============================================================================================================================
    
    //  supply a new record: started by START-button, terminated by RECORD_AVAILABLE_BUTTON
    scheduledTask supplyRecord = fetchingRecord->device[stockStepper]->addTask(140000, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, STOCK_SUPPLY_RECORD_ACCEL);
    fetchingRecord->device[stockStepper]->task[supplyRecord]->startByDate(100);
    fetchingRecord->device[stockStepper]->task[supplyRecord]->stopByButton(recordAvailableButton, STOP_NORMAL);
    
    
    // lift grappler: start when RECORD_AVAILABLE_BUTTON indicates new record available
    scheduledTask liftFromParkPosition = fetchingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    fetchingRecord->device[liftServo]->task[liftFromParkPosition]->startByButton(recordAvailableButton);
    
    //  turn grappler to stock: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
    scheduledTask turnToStock = fetchingRecord->device[turnServo]->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, TURN_ACCEL_FAST);
    fetchingRecord->device[turnServo]->task[turnToStock]->startByTriggerpositionOf(liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
    
    //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
    scheduledTask lowerToStock = fetchingRecord->device[liftServo]->addTask(LIFT_STOCK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    fetchingRecord->device[liftServo]->task[lowerToStock]->startByTriggerpositionOf(turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
    
    
    //             //  grip new record: start when grappler reached stock (LIFT_STOCK_POSITION)
    //             unsigned char gripNewRecord = fetchingRecord->device[vacuumSolenoid]->addMove(1, SOLENOID_FREQUENCY, SOLENOID_DUTYCYCLE, 555);
    //             fetchingRecord->device[vacuumSolenoid]->setStartEventForMove(gripNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
    //             freeRam();
    
    
    //  lift the new record: start with startDelay after stock was reached (LIFT_STOCK_POSITION)
    scheduledTask liftNewRecord = fetchingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    fetchingRecord->device[liftServo]->task[liftNewRecord]->startByTriggerpositionOf(liftServo, lowerToStock, LIFT_STOCK_POSITION);
    
    //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
    scheduledTask turnRecordToTable = fetchingRecord->device[turnServo]->addTask(TURN_TABLE_POSITION, TURN_SPEED_SLOW, TURN_ACCEL_SLOW, TURN_ACCEL_SLOW);
    fetchingRecord->device[turnServo]->task[turnRecordToTable]->startByTriggerpositionOf(liftServo, liftNewRecord, LIFT_UP_TRIGGER_TURN);
    
    //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
    scheduledTask lowerRecordToTable = fetchingRecord->device[liftServo]->addTask(LIFT_TABLE_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    fetchingRecord->device[liftServo]->task[lowerRecordToTable]->startByTriggerpositionOf(turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
    //  fetchingRecord->device[vacuumSolenoid]->setStopEventForMove(gripNewRecord, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
    
    //  lift for going to park position: start with startDelay after turn table was reached (LIFT_TABLE_POSITION)
    scheduledTask liftForParkPosition = fetchingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    fetchingRecord->device[liftServo]->task[liftForParkPosition]->startByTriggerpositionOf(liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
    
    //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
    scheduledTask turnToParkPosition = fetchingRecord->device[turnServo]->addTask(TURN_PARK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, TURN_ACCEL_FAST);
    fetchingRecord->device[turnServo]->task[turnToParkPosition]->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
    
    //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
    scheduledTask lowerForParkPosition = fetchingRecord->device[liftServo]->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    fetchingRecord->device[liftServo]->task[lowerForParkPosition]->startByTriggerpositionOf(turnServo, turnToParkPosition, TURN_TO_PARK_TRIGGER_LIFT);

    
    
    //  cancel cutting if songCancelButton is pressed
    fetchingRecord->controlButton[stockTopButton]->evokeTaskJump(stockStepper, supplyRecord, STOP);
    fetchingRecord->controlButton[stockTopButton]->evokeTaskJumpToTask(liftServo, liftFromParkPosition, STOP_SHARP_AND_SWITCH, lowerForParkPosition + 1);
    fetchingRecord->controlButton[stockTopButton]->evokeTaskJumpToTask(turnServo, turnToStock, STOP_SHARP_AND_SWITCH, turnToParkPosition + 1);
    
    
    
    fetchingRecord->reviewTasks();
    fetchingRecord->getAllTasks();
    fetchingRecord->getAllActions();
    

    freeRam();
}
    
    
    // ============================================================================================================================
    // ============= create cuttingProcess scheduler object: ======================================================================
    // ============================================================================================================================
    
    cuttingProcess = new CCDeviceScheduler("cuttingProcess");
    
    
    {
    // ============================================================================================================================
    // ============= devices of cuttingProcess: ===================================================================================
    // ============================================================================================================================
    
    schedulerDevice headLeftServo = cuttingProcess->addServo(SERVO_HEAD_LEFT_NAME,
                                                             SERVO_HEAD_LEFT_PIN,
                                                             SERVO_HEAD_LEFT_MIN_POSITION,
                                                             SERVO_HEAD_LEFT_MAX_POSITION,
                                                             HEAD_LEFT_PARK_POSITION);
    
    schedulerDevice headRightServo = cuttingProcess->addServo(SERVO_HEAD_RIGHT_NAME,
                                                              SERVO_HEAD_RIGHT_PIN,
                                                              SERVO_HEAD_RIGHT_MIN_POSITION,
                                                              SERVO_HEAD_RIGHT_MAX_POSITION,
                                                              HEAD_RIGHT_PARK_POSITION);
    
    schedulerDevice tableStepper = cuttingProcess->addStepper_A4988(STEPPER_TABLE_NAME,
                                                                    STEPPER_TABLE_STEP_PIN,
                                                                    STEPPER_TABLE_DIR_PIN,
                                                                    STEPPER_TABLE_ENABLE_PIN,
                                                                    STEPPER_TABLE_HIGHEST_STEPPINGMODE,
                                                                    STEPPER_TABLE_STEPMODECODES,
                                                                    STEPPER_TABLE_MICROSTEPPINS,
                                                                    STEPPER_TABLE_STEPS_PER_ROTATION);
    
    schedulerDevice catStepper = cuttingProcess->addStepper_TMC260(STEPPER_CAT_NAME,
                                                                   STEPPER_CAT_STEP_PIN,
                                                                   STEPPER_CAT_DIR_PIN,
                                                                   STEPPER_CAT_ENABLE_PIN,
                                                                   STEPPER_CAT_CHIPSELECT_PIN,
                                                                   STEPPER_CAT_CURRENT,
                                                                   STEPPER_CAT_STEPS_PER_ROTATION);
    
    
    
    
    schedulerControlButton bridgeParkButton = cuttingProcess->addControlButton(CAT_PARKBUTTON_NAME, CAT_PARKBUTTON_PIN, CAT_PARKBUTTON_ACTIV, CAT_PARKBUTTON_PULLUP);
    schedulerControlButton bridgeEndButton = cuttingProcess->addControlButton(CAT_ENDBUTTON_NAME, CAT_ENDBUTTON_PIN, CAT_ENDBUTTON_ACTIV, CAT_ENDBUTTON_PULLUP);
    
    schedulerControlButton songEndButton = cuttingProcess->addControlButton(SONG_ENDBUTTON_NAME, SONG_ENDBUTTON_PIN, SONG_ENDBUTTON_ACTIV, SONG_ENDBUTTON_PULLUP);
    schedulerControlButton songCancelButton = cuttingProcess->addControlButton(SONG_CANCELBUTTON_NAME, SONG_CANCELBUTTON_PIN, SONG_CANCELBUTTON_ACTIV, SONG_CANCELBUTTON_PULLUP);
    
    cuttingProcess->getAllDevices();
    cuttingProcess->getAllControlButtons();
    
    freeRam();
    
//    
//        scheduledTask turn = cuttingProcess->device[tableStepper]->addTaskWithPositionReset(turnTableStepperDegrees + 7200.0, turnTableStepperSpeed, TABLE_STEP_ACCEL, TABLE_STEP_ACCEL);
//        cuttingProcess->device[tableStepper]->task[turn]->startByDate(100);
//        // cuttingProcess->device[tableDrive]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
//        cuttingProcess->reviewTasks();
//        cuttingProcess->getAllTasks();
//        
//    while (true) {
//        
//        cuttingProcess->run();
//
//
//    }
//    
    
    // ============================================================================================================================
    // ============= initialisation of cuttingProcess: =====================================================================================
    // ============================================================================================================================
  
    cuttingProcess->device[catStepper]->setCurrentPosition(0.0);
    
    if (true) {
        
        Serial.println("................................. initialisation of catStepper ...................................");

        
        scheduledTask initCatStepper = cuttingProcess->device[catStepper]->addTask(-400000, CAT_SPEED_VERY_HIGH, CAT_ACCEL_VERY_HIGH, CAT_ACCEL_VERY_HIGH);
        cuttingProcess->device[catStepper]->task[initCatStepper]->startByDate(100);
        cuttingProcess->device[catStepper]->task[initCatStepper]->stopByButton(bridgeParkButton, STOP_NORMAL);
        
        cuttingProcess->reviewTasks();
        cuttingProcess->getAllTasks();
        
        
        cuttingProcess->run();
        
        
        cuttingProcess->deleteAllTasks();
        cuttingProcess->deleteAllActions();
        
        
        Serial.println("...................................... done ......................................................");
    }
    
    freeRam();
    
    
    cuttingProcess->device[catStepper]->setCurrentPosition(0.0);
    cuttingProcess->device[tableStepper]->setCurrentPosition(0.0);
    
    
    
    
    // ============================================================================================================================
    // ============= tasks of cuttingProcess: =====================================================================================
    // ============================================================================================================================
    
    //  move to start groove:
    scheduledTask driveToCuttingStartPosition = cuttingProcess->device[catStepper]->addTask(CAT_CUTTING_START_POSITION, CAT_SPEED_HIGH, CAT_ACCEL_HIGH, CAT_ACCEL_HIGH);
    cuttingProcess->device[catStepper]->task[driveToCuttingStartPosition]->startByDate(100);
    
    //  turn the table:
    scheduledTask turnTheTable = cuttingProcess->device[tableStepper]->addTaskWithPositionReset(turnTableStepperDegrees + 7200.0, turnTableStepperSpeed, TABLE_STEP_ACCEL, TABLE_STEP_ACCEL);
    cuttingProcess->device[tableStepper]->task[turnTheTable]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
    // cuttingProcess->device[tableDrive]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
    
    //  lower head close to record surface (left servo): start when reached start position of start groove
    scheduledTask lowerHeadLeftForCutting = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    cuttingProcess->device[headLeftServo]->task[lowerHeadLeftForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
    
    //  approximate head to record surface (right servo): start when left servo reached cutting position
    scheduledTask approximateHeadRightForCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
    cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
    cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->stopDynamicallyBySensor(HEAD_INCLINATION_SENSOR, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
    
    //  make start groove:
    scheduledTask makeStartGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_startGroove, CAT_ACCEL_SLOW, CAT_ACCEL_SLOW);
    cuttingProcess->device[catStepper]->task[makeStartGroove]->startAfterCompletionOf(headRightServo, approximateHeadRightForCutting);
    cuttingProcess->device[catStepper]->task[makeStartGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeStartGroove, catSongStartPosition);
    
    //  make song groove:
    scheduledTask makeMainGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_song, CAT_ACCEL_SLOW, CAT_ACCEL_SLOW);
    cuttingProcess->device[catStepper]->task[makeMainGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition);
    
    //  make end groove:
    scheduledTask makeEndGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_endGroove, CAT_ACCEL_NORMAL, CAT_ACCEL_NORMAL);
    
    //  lift head after cutting above the record surface:
    scheduledTask liftHeadRightAfterCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    cuttingProcess->device[headRightServo]->task[liftHeadRightAfterCutting]->startAfterCompletionOf(catStepper, makeEndGroove);
    
    //  lift head into park position:
    scheduledTask liftHeadLeftAfterCutting = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    cuttingProcess->device[headLeftServo]->task[liftHeadLeftAfterCutting]->startAfterCompletionOf(headRightServo, liftHeadRightAfterCutting);
    
    //  switch off turn table
    cuttingProcess->device[tableStepper]->task[turnTheTable]->stopAfterCompletionOf(headRightServo, liftHeadRightAfterCutting, STOP_NORMAL);
    
    //  drive cat back home
    scheduledTask driveToParkPosition = cuttingProcess->device[catStepper]->addTask(CAT_PARK_POSITION, CAT_SPEED_HIGH, CAT_ACCEL_HIGH, CAT_ACCEL_HIGH);
    cuttingProcess->device[catStepper]->task[driveToParkPosition]->startAfterCompletionOf(headRightServo, liftHeadRightAfterCutting);
    cuttingProcess->device[catStepper]->task[driveToParkPosition]->stopByButton(bridgeParkButton, STOP_NORMAL);
    
    
    //  jump over to end groove if songEndButton is pressed
    cuttingProcess->controlButton[songEndButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, makeEndGroove);
    
    //  cancel cutting if songCancelButton is pressed
    cuttingProcess->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeStartGroove, STOP_AND_SWITCH, driveToParkPosition);
    cuttingProcess->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, driveToParkPosition);
    cuttingProcess->controlButton[songCancelButton]->evokeTaskJump(catStepper, makeEndGroove, STOP);
    
    
    
    cuttingProcess->reviewTasks();
    cuttingProcess->getAllTasks();
    cuttingProcess->getAllActions();
}


    
    // ============================================================================================================================
    // ============= create ejectingRecord scheduler object: ======================================================================
    // ============================================================================================================================
    
    ejectingRecord = new CCDeviceScheduler("ejectingRecord");
    
    
    {
    // ============================================================================================================================
    // ============= devices of ejectingRecord: ===================================================================================
    // ============================================================================================================================
    
    schedulerDevice liftServo = ejectingRecord->addServo(SERVO_LIFT_NAME,
                                                         SERVO_LIFT_PIN,
                                                         SERVO_LIFT_MIN_POSITION,
                                                         SERVO_LIFT_MAX_POSITION,
                                                         LIFT_PARK_POSITION);
    
    schedulerDevice turnServo = ejectingRecord->addServo(SERVO_TURN_NAME,
                                                         SERVO_TURN_PIN,
                                                         SERVO_TURN_MIN_POSITION,
                                                         SERVO_TURN_MAX_POSITION,
                                                         TURN_PARK_POSITION);
    
    schedulerDevice stockStepper = ejectingRecord->addStepper_A4988(STEPPER_STOCK_NAME,
                                                                    STEPPER_STOCK_STEP_PIN,
                                                                    STEPPER_STOCK_DIR_PIN,
                                                                    STEPPER_STOCK_ENABLE_PIN,
                                                                    STEPPER_STOCK_HIGHEST_STEPPINGMODE,
                                                                    STEPPER_STOCK_STEPMODECODES,
                                                                    STEPPER_STOCK_MICROSTEPPINS,
                                                                    STEPPER_STOCK_STEPS_PER_ROTATION);
        
        schedulerDevice conveyStepper = ejectingRecord->addStepper_A4988(STEPPER_CONVEYOR_NAME,
                                                                         STEPPER_CONVEYOR_STEP_PIN,
                                                                         STEPPER_CONVEYOR_DIR_PIN,
                                                                         STEPPER_CONVEYOR_ENABLE_PIN,
                                                                         STEPPER_CONVEYOR_HIGHEST_STEPPINGMODE,
                                                                         STEPPER_CONVEYOR_STEPMODECODES,
                                                                         STEPPER_CONVEYOR_MICROSTEPPINS,
                                                                         STEPPER_CONVEYOR_STEPS_PER_ROTATION);
    
    
    
    schedulerControlButton recordAvailableButton = ejectingRecord->addControlButton(RECORDAVAILABLE_BUTTON_NAME, RECORDAVAILABLE_BUTTON_PIN, RECORDAVAILABLE_BUTTON_ACTIV, RECORDAVAILABLE_BUTTON_PULLUP);
    schedulerControlButton stockTopButton = ejectingRecord->addControlButton(STOCKTOP_BUTTON_NAME, STOCKTOP_BUTTON_PIN, STOCKTOP_BUTTON_ACTIV, STOCKTOP_BUTTON_PULLUP);
    schedulerControlButton stockBottomButton = ejectingRecord->addControlButton(STOCKBOTTOM_BUTTON_NAME, STOCKBOTTOM_BUTTON_PIN, STOCKBOTTOM_BUTTON_ACTIV, STOCKBOTTOM_BUTTON_PULLUP);
    
    ejectingRecord->getAllDevices();
    ejectingRecord->getAllControlButtons();
    
    
    
    // ============================================================================================================================
    // ============= initialisation of ejectingRecord: ============================================================================
    // ============================================================================================================================
    

    
    
    // ============================================================================================================================
    // ============= tasks of ejectingRecord: =====================================================================================
    // ============================================================================================================================
    

        scheduledTask drive = ejectingRecord->device[conveyStepper]->addTask(20000ul, 400, 200, 200);
        ejectingRecord->device[conveyStepper]->task[drive]->startByDate(100);
            /*
    //  supply a new record: started by START-button, terminated by RECORD_AVAILABLE_BUTTON
    scheduledTask supplyRecord = ejectingRecord->device[stockStepper]->addTask(140000, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, STOCK_SUPPLY_RECORD_ACCEL);
    ejectingRecord->device[stockStepper]->task[supplyRecord]->startByDate(100);
    ejectingRecord->device[stockStepper]->task[supplyRecord]->stopByButton(recordAvailableButton, STOP_NORMAL);
    
    
    // lift grappler: start when RECORD_AVAILABLE_BUTTON indicates new record available
    scheduledTask liftFromParkPosition = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    ejectingRecord->device[liftServo]->task[liftFromParkPosition]->startByButton(recordAvailableButton);
    
    //  turn grappler to stock: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
    scheduledTask turnToStock = ejectingRecord->device[turnServo]->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, TURN_ACCEL_FAST);
    ejectingRecord->device[turnServo]->task[turnToStock]->startByTriggerpositionOf(liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
    
    //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
    scheduledTask lowerToStock = ejectingRecord->device[liftServo]->addTask(LIFT_STOCK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    ejectingRecord->device[liftServo]->task[lowerToStock]->startByTriggerpositionOf(turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
    
    
    //             //  grip new record: start when grappler reached stock (LIFT_STOCK_POSITION)
    //             unsigned char gripNewRecord = ejectingRecord->device[vacuumSolenoid]->addMove(1, SOLENOID_FREQUENCY, SOLENOID_DUTYCYCLE, 555);
    //             ejectingRecord->device[vacuumSolenoid]->setStartEventForMove(gripNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
    //             freeRam();
    
    
    //  lift the new record: start with startDelay after stock was reached (LIFT_STOCK_POSITION)
    scheduledTask liftNewRecord = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    ejectingRecord->device[liftServo]->task[liftNewRecord]->startByTriggerpositionOf(liftServo, lowerToStock, LIFT_STOCK_POSITION);
    
    //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
    scheduledTask turnRecordToTable = ejectingRecord->device[turnServo]->addTask(TURN_TABLE_POSITION, TURN_SPEED_SLOW, TURN_ACCEL_SLOW, TURN_ACCEL_SLOW);
    ejectingRecord->device[turnServo]->task[turnRecordToTable]->startByTriggerpositionOf(liftServo, liftNewRecord, LIFT_UP_TRIGGER_TURN);
    
    //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
    scheduledTask lowerRecordToTable = ejectingRecord->device[liftServo]->addTask(LIFT_TABLE_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    ejectingRecord->device[liftServo]->task[lowerRecordToTable]->startByTriggerpositionOf(turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
    //  ejectingRecord->device[vacuumSolenoid]->setStopEventForMove(gripNewRecord, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
    
    //  lift for going to park position: start with startDelay after turn table was reached (LIFT_TABLE_POSITION)
    scheduledTask liftForParkPosition = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    ejectingRecord->device[liftServo]->task[liftForParkPosition]->startByTriggerpositionOf(liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
    
    //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
    scheduledTask turnToParkPosition = ejectingRecord->device[turnServo]->addTask(TURN_PARK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, TURN_ACCEL_FAST);
    ejectingRecord->device[turnServo]->task[turnToParkPosition]->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
    
    //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
    scheduledTask lowerForParkPosition = ejectingRecord->device[liftServo]->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    ejectingRecord->device[liftServo]->task[lowerForParkPosition]->startByTriggerpositionOf(turnServo, turnToParkPosition, TURN_TO_PARK_TRIGGER_LIFT);
    
    
    
    //  cancel cutting if songCancelButton is pressed
    ejectingRecord->controlButton[stockTopButton]->evokeTaskJump(stockStepper, supplyRecord, STOP);
    ejectingRecord->controlButton[stockTopButton]->evokeTaskJumpToTask(liftServo, liftFromParkPosition, STOP_SHARP_AND_SWITCH, lowerForParkPosition + 1);
    ejectingRecord->controlButton[stockTopButton]->evokeTaskJumpToTask(turnServo, turnToStock, STOP_SHARP_AND_SWITCH, turnToParkPosition + 1);
    
    */
    
    ejectingRecord->reviewTasks();
    ejectingRecord->getAllTasks();
    ejectingRecord->getAllActions();
    
    
    freeRam();

    }
    
    
    
    
    // ============================================================================================================================
    // ============================================================================================================================
    // ============= operation: ===================================================================================================
    // ============================================================================================================================
    // ============================================================================================================================

    
    Serial.println(F("ready to run!"));
    
    while (true) {
        
        if (digitalRead(FETCH_RECORD_BUTTON) == LOW) {
            
            fetchingRecord->run();
            ejectingRecord->run();
            
        }
        
        
        if (digitalRead(START_CUTTING_BUTTON) == LOW) {
            
//            cuttingProcess->device[catStepper]->setCurrentPosition(0.0);
//            cuttingProcess->device[tableStepper]->setCurrentPosition(0.0);
//            
           cuttingProcess->run();
            
        }
        
        if (false) {
            
//            schedulerControlButton catForwardButton = cuttingProcess->addControlButton("catForward", CAT_FWD, HIGH, true);
//            schedulerControlButton catRewindButton = cuttingProcess->addControlButton("catRewind", CAT_RWD, HIGH, true);
//            
//            
//            //  move to start groove:
//            scheduledTask driveToCuttingStartPositionMan = cuttingProcess->device[catStepper]->addTask(CAT_CUTTING_START_POSITION, 4800, 3200, 3200);
//            cuttingProcess->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(100);
//            cuttingProcess->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(bridgeEndButton, STOP_NORMAL);
//            
//            cuttingProcess->reviewTasks();
//            cuttingProcess->run();
//            cuttingProcess->deleteAllTasks();
//            
//            while (digitalRead(START_CUTTING_BUTTON) == HIGH) {
//                if (digitalRead(CAT_FWD) == LOW) {
//                    driveToCuttingStartPositionMan = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, 4800, 3200, 3200);
//                    cuttingProcess->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
//                    cuttingProcess->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(catForwardButton, STOP_NORMAL);
//                    
//                    cuttingProcess->reviewTasks();
//                    cuttingProcess->run();
//                    cuttingProcess->deleteAllTasks();
//                }
//                if (digitalRead(CAT_RWD) == LOW) {
//                    driveToCuttingStartPositionMan = cuttingProcess->device[catStepper]->addTask(CAT_PARK_POSITION, 4800, 3200, 3200);
//                    cuttingProcess->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
//                    cuttingProcess->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(catRewindButton, STOP_NORMAL);
//                    
//                    cuttingProcess->reviewTasks();
//                    cuttingProcess->run();
//                    cuttingProcess->deleteAllTasks();
//                }
//            }
//            
//            
//            cuttingProcess->reviewTasks();
//            cuttingProcess->run();
//            cuttingProcess->deleteAllTasks();
            
        }
        
        if (digitalRead(MATCH_HEADIMPACT_BUTTON) == LOW) {
            /*
            scheduledTask liftLeft = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
            cuttingProcess->device[headLeftServo]->task[liftLeft]->startByDate(100);
            
            scheduledTask liftRight = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
            cuttingProcess->device[headRightServo]->task[liftRight]->startByDate(100);
            
            scheduledTask lowerLeft = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
            cuttingProcess->device[headLeftServo]->task[lowerLeft]->startAfterCompletionOf(headRightServo, liftRight);
            
            //  approximate head to record surface (right servo): start when left servo reached cutting position
            scheduledTask approximateHeadRightForCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
            cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->startAfterCompletionOf(headRightServo, liftRight);
            cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->stopDynamicallyBySensor(HEAD_INCLINATION_SENSOR, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
            
            cuttingProcess->run();
            
            delay(3000);
            
            cuttingProcess->deleteAllTasks();
            */
        }
    }
    
    
    delay(2000);
    
    
    delete fetchingRecord;
    fetchingRecord = NULL;
    
    delete cuttingProcess;
    cuttingProcess = NULL;
    
    
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

    Serial.print("groove pitch is set to ");
    Serial.print(SONGGROOVE_PITCH);
    Serial.print("mm, start groove pitch is set to ");
    Serial.print(STARTGROOVE_PITCH);
    Serial.print("mm, start groove width is set to ");
    Serial.print(STARTGROOVE_RANGE);
    Serial.print("mm, end groove pitch is set to ");
    Serial.print(ENDGROOVE_PITCH);
    Serial.print("mm, minimal end groove width is set to ");
    Serial.print(ENDGROOVE_RANGE_MIN);
    Serial.println("mm");
    Serial.print("max. song width: ");
    Serial.print(songRange);
    Serial.print("mm, max. song grooves: ");
    Serial.print(songGrooves);
    Serial.print(", max. playing time: ");
    Serial.print(playTime_seconds / 60);
    Serial.print(":");
    Serial.println(playTime_seconds % 60);

    
    if ((float)PLAYTIME_MINUTES * 60 < playTime_seconds) {
        songGrooves = PLAYTIME_MINUTES * RECORD_TURNS_PER_MINUTE;
        songRange = songGrooves * SONGGROOVE_PITCH;
        endGrooveRange = CUTTING_RANGE - songRange - STARTGROOVE_RANGE;
        playTime_seconds = PLAYTIME_MINUTES * 60;

        Serial.print("playing time is set to ");
        Serial.print(playTime_seconds / 60);
        Serial.print(":");
        Serial.print(playTime_seconds % 60);
        Serial.print(", song width: ");
        Serial.print(songRange);
        Serial.print("mm, record grooves: ");
        Serial.println(songGrooves);
    } else {
        Serial.println("desired playing time is too long, using maximal playing time!");
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
    //    pinMode(CAT_FWD, INPUT_PULLUP);
    //    pinMode(LOADING_BUTTON, INPUT_PULLUP);
    //    pinMode(RECORD_AVAILABLE_BUTTON, INPUT_PULLUP);
    //    pinMode(CAT_PARKBUTTON_PIN, INPUT_PULLUP);
    
    
//    pinMode(CAT_FWD, INPUT_PULLUP);
//    pinMode(CAT_RWD, INPUT_PULLUP);
    pinMode(START_CUTTING_BUTTON, INPUT_PULLUP);
    pinMode(FETCH_RECORD_BUTTON, INPUT_PULLUP);
    pinMode(MATCH_HEADIMPACT_BUTTON, INPUT_PULLUP);
    
    pinMode(I_AM_LATE_LED, OUTPUT);
    
    Serial.begin(115200);
    Serial.println(), Serial.println(), Serial.println();
    Serial.println(), Serial.println(), Serial.println();
        
    Serial.println();
    Serial.println();
    Serial.println();
    
    calculateCuttingParameters();
    
}

//int freeRam () {
//    extern int __heap_start, *__brkval;
//    int v;
//    int fr = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
//    Serial.print("Free ram: ");
//    Serial.println(fr);
//}


//void freeRam() {
//    char *ramstart = (char *) 0x20070000;
//    char *ramend = (char *) 0x20088000;
//    char *heapend = sbrk(0);
//    register char * stack_ptr asm( "sp" );
//    struct mallinfo mi = mallinfo();
//    Serial.print("Ram used (bytes): dynamic: ");
//    Serial.print(mi.uordblks);
//    Serial.print("  static:  ");
//    Serial.print(&_end - ramstart);
//    Serial.print("  stack:   ");
//    Serial.print(ramend - stack_ptr);
//    Serial.print(", estimation free Ram: ");
//    Serial.println(stack_ptr - heapend + mi.fordblks);
//}


/*
void anything() {
    
    
    scheduler = new CCDeviceScheduler;
    
    
    // ============================================================================================================================
    // ============= devices ======================================================================================================
    // ============================================================================================================================
    
    schedulerDevice headLeftServo = cuttingProcess->addServo(SERVO_HEAD_LEFT_NAME, SERVO_HEAD_LEFT_PIN, SERVO_HEAD_LEFT_MIN_POSITION, SERVO_HEAD_LEFT_MAX_POSITION, HEAD_LEFT_PARK_POSITION);
    schedulerDevice headRightServo = cuttingProcess->addServo(SERVO_HEAD_RIGHT_NAME, SERVO_HEAD_RIGHT_PIN, SERVO_HEAD_RIGHT_MIN_POSITION, SERVO_HEAD_RIGHT_MAX_POSITION, HEAD_RIGHT_PARK_POSITION);
    schedulerDevice tableStepper = cuttingProcess->addStepper_A4988(STEPPER_TABLE_NAME, STEPPER_TABLE_STEP_PIN, STEPPER_TABLE_DIR_PIN, STEPPER_TABLE_ENABLE_PIN, STEPPER_TABLE_HIGHEST_STEPPINGMODE, STEPPER_TABLE_STEPMODECODES, STEPPER_TABLE_MICROSTEPPINS, STEPPER_TABLE_STEPS_PER_ROTATION);
    schedulerDevice catStepper = cuttingProcess->addStepper_TMC260(STEPPER_CAT_NAME, STEPPER_CAT_STEP_PIN, STEPPER_CAT_DIR_PIN, STEPPER_CAT_ENABLE_PIN, STEPPER_CAT_CHIPSELECT_PIN, STEPPER_CAT_CURRENT, STEPPER_CAT_STEPS_PER_ROTATION);
    
    
    cuttingProcess->getAllDevices();
    
    
    
    
    // ============================================================================================================================
    // ============== control buttons =============================================================================================
    // ============================================================================================================================
    
    schedulerControlButton songEndButton = cuttingProcess->addControlButton(SONG_ENDBUTTON_NAME, SONG_ENDBUTTON_PIN, SONG_ENDBUTTON_ACTIV);
    schedulerControlButton songCancelButton = cuttingProcess->addControlButton(SONG_CANCELBUTTON_NAME, SONG_CANCELBUTTON_PIN, SONG_CANCELBUTTON_ACTIV);
    
    
    cuttingProcess->getAllControlButtons();
    
    
    
    
    
    Serial.println("................................. initialisation .................................");
    
    // ============================================================================================================================
    // ============= initialisation tasks =========================================================================================
    // ============================================================================================================================
    
    scheduledTask initCatStepper = cuttingProcess->device[catStepper]->addTask(-CAT_FULLRANGE, CAT_SPEED_VERY_HIGH, CAT_ACCEL_VERY_HIGH, CAT_ACCEL_VERY_HIGH);
    cuttingProcess->device[catStepper]->task[initCatStepper]->startByDate(100);
    cuttingProcess->device[catStepper]->task[initCatStepper]->stopByButton(CAT_PARKBUTTON_PIN, HIGH, STOP_NORMAL);
    
    
    cuttingProcess->reviewTasks();
    cuttingProcess->getAllTasks();
    
    
    
    // ============================================================================================================================
    // ============== run initialisation ==========================================================================================
    // ============================================================================================================================
    
    cuttingProcess->run();
    
    
    
    // ============================================================================================================================
    // ============== cleaning up =================================================================================================
    // ============================================================================================================================
    
    cuttingProcess->deleteAllTasks();
    cuttingProcess->deleteAllActions();
    
    
    Serial.println("...................................... done ......................................");
    
    
    
    
    
    // ============================================================================================================================
    // ============= main tasks ===================================================================================================
    // ============================================================================================================================
    
    //  move to start groove:
    scheduledTask driveToCuttingStartPosition = cuttingProcess->device[catStepper]->addTask(CAT_CUTTING_START_POSITION, CAT_SPEED_HIGH, CAT_ACCEL_HIGH, CAT_ACCEL_HIGH);
    cuttingProcess->device[catStepper]->task[driveToCuttingStartPosition]->startByDate(100);
    cuttingProcess->device[catStepper]->task[driveToCuttingStartPosition]->stopByButton(CAT_ENDBUTTON, HIGH, STOP_NORMAL);
    
    //  turn the table:
    scheduledTask turnTheTable = cuttingProcess->device[tableStepper]->addTask(grooves_all * 3600, turnTableStepperSpeed, TABLE_STEP_ACCEL, TABLE_STEP_ACCEL);
    cuttingProcess->device[tableDrive]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
    
    //  lower head to record surface: start when reached start position of start groove
    scheduledTask lowerHeadLeftForCutting = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
    cuttingProcess->device[headLeftServo]->task[lowerHeadLeftForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
    
    //  lower head to record surface: start when reached start position of start groove
    scheduledTask lowerHeadRightForCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
    cuttingProcess->device[headRightServo]->task[lowerHeadRightForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
    cuttingProcess->device[headRightServo]->task[lowerHeadRightForCutting]->stopDynamicallyBySensor(A5, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
    
    //  make the start groove: start when cutting head reached cutting position
    scheduledTask makeStartGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_startGroove, CAT_ACCEL_SLOW, CAT_ACCEL_SLOW);
    cuttingProcess->device[catStepper]->task[makeStartGroove]->startAfterCompletionOf(headRightServo, lowerHeadRightForCutting);
    cuttingProcess->device[catStepper]->task[makeStartGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeStartGroove, catSongStartPosition);
    
    //  make the main groove, cut the song: change head speed to appropriate song-cutting-speed when start groove is done
    scheduledTask makeMainGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_song, CAT_ACCEL_SLOW, CAT_ACCEL_SLOW);
    cuttingProcess->device[catStepper]->task[makeMainGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition);
    
    //  make the end groove: change head speed to end groove speed, when song is done
    scheduledTask makeEndGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_endGroove, CAT_ACCEL_NORMAL, CAT_ACCEL_NORMAL);
    
    //  lift head after cutting: start when end groove is finished
    scheduledTask liftHeadLeftAfterCutting = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    cuttingProcess->device[headLeftServo]->task[liftHeadLeftAfterCutting]->startAfterCompletionOf(catStepper, makeEndGroove);
    
    //  stop turning the table:
    cuttingProcess->device[tableStepper]->task[turnTheTable]->stopAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting, STOP_NORMAL);
    
    //  lift head complete after cutting: start when other servo finished
    scheduledTask liftHeadRightAfterCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
    cuttingProcess->device[headRightServo]->task[liftHeadRightAfterCutting]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
    
    //  drive to park position after all
    scheduledTask driveToParkPosition = cuttingProcess->device[catStepper]->addTask(CAT_PARK_POSITION, CAT_SPEED_HIGH, CAT_ACCEL_HIGH, CAT_ACCEL_HIGH);
    cuttingProcess->device[catStepper]->task[driveToParkPosition]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
    cuttingProcess->device[catStepper]->task[driveToParkPosition]->stopByButton(CAT_PARKBUTTON_PIN, HIGH, STOP_NORMAL);
    
    
    //  do the end groove when the button is pressed
    cuttingProcess->controlButton[songEndButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, makeEndGroove);
    
    //  cancel recording, when button is pressed
    cuttingProcess->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeStartGroove, STOP_AND_SWITCH, driveToParkPosition);
    cuttingProcess->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, driveToParkPosition);
    cuttingProcess->controlButton[songCancelButton]->evokeTaskJump(catStepper, makeEndGroove, STOP);
    
    
    
    cuttingProcess->reviewTasks();
    cuttingProcess->getAllTasks();
    
    cuttingProcess->getAllControlButtons();
    cuttingProcess->getAllActions();
    
    
    
    // ============================================================================================================================
    // ============== run it ======================================================================================================
    // ============================================================================================================================
    
    
    
    cuttingProcess->run();
    
    cuttingProcess->deleteAllTasks();
    cuttingProcess->deleteAllActions();
    
    
    
}




*/








//        unsigned char switchTableOn = cuttingProcess->device[tableDrive]->addTask(TABLEDRIVE_ON, 0, 0, 0);
//        cuttingProcess->device[tableDrive]->setStartAfterCompletion(switchTableOn, headLeftServo, liftHeadFirstLeft);
//        unsigned char switchTableOff = cuttingProcess->device[tableDrive]->addTask(TABLEDRIVE_ACTIV, 0, 0, 0);
//        cuttingProcess->device[tableDrive]->startByDate(switchTableOff, 10000);

//        unsigned char riseHead = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//        cuttingProcess->device[headLeftServo]->setStartAfterCompletion(riseHead, headLeftServo, lowerHeadFirstLeft);
//        cuttingProcess->device[headLeftServo]->stopByTimeout(riseHead, 10000, true);
//
//        unsigned char lowerHead = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//        cuttingProcess->device[headLeftServo]->setStartAfterCompletion(lowerHead, headLeftServo, riseHead);
//        cuttingProcess->device[headLeftServo]->stopDynamicallyBySensor(lowerHead, A5, 1000, 200, 0);
//        cuttingProcess->device[headLeftServo]->stopByTimeout(lowerHead, 10000, true);



//  unsigned char liftHeadFirstRight = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//  cuttingProcess->device[headRightServo]->startByDate(liftHeadFirstRight, 100);


// here may be a problem: preperation of next move before 'currentPosition' set to 0


/*
 //  supply a new record from stock
 unsigned char supplyNewRecord = cuttingProcess->device[stockStepper]->addTask(36000L, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, STOCK_SUPPLY_RECORD_ACCEL);
 //        cuttingProcess->device[stockStepper]->startByDate(supplyNewRecord, 100);
 cuttingProcess->device[stockStepper]->startByButton(supplyNewRecord, CAT_FWD, LOW);
 cuttingProcess->device[stockStepper]->stopByButton(supplyNewRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);
 
 //  lift grappler: start soon
 unsigned char liftFromParkPosition = cuttingProcess->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
 //        cuttingProcess->device[liftServo]->startByDate(liftFromParkPosition, 100);
 //        cuttingProcess->device[turnServo]->startByButton(turnToStock, RECORD_AVAILABLE_BUTTON, HIGH);
 freeRam();
 
 //  turn grappler to stock: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnToStock = cuttingProcess->device[turnServo]->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
 cuttingProcess->device[turnServo]->startByTriggerpositionOf(turnToStock, liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
 unsigned char lowerToStock = cuttingProcess->device[liftServo]->addTask(LIFT_STOCK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
 //        cuttingProcess->device[liftServo]->startByDate(lowerToStock, 3000);
 cuttingProcess->device[liftServo]->startByTriggerpositionOf(lowerToStock, turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
 freeRam();
 
 //         //  grip new record: start when grappler reached stock (LIFT_STOCK_POSITION)
 //         unsigned char gripNewRecord = cuttingProcess->device[vacuumSolenoid]->addTask(SOLENOID_DUTYCYCLE, SOLENOID_FREQUENCY, 0, 500);
 //         cuttingProcess->device[vacuumSolenoid]->startByTriggerpositionOf(gripNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
 //         freeRam();
 
 //  lift the new record: start with startDelay after stock was reached (LIFT_STOCK_POSITION)
 unsigned char liftNewRecord = cuttingProcess->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, 2666);
 cuttingProcess->device[liftServo]->startByTriggerpositionOf(liftNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
 freeRam();
 
 //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnRecordToTable = cuttingProcess->device[turnServo]->addTask(TURN_TABLE_POSITION, TURN_SPEED_SLOW, TURN_ACCEL_SLOW, NO_START_DELAY);
 cuttingProcess->device[turnServo]->startByTriggerpositionOf(turnRecordToTable, liftServo, liftNewRecord, LIFT_UP_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
 unsigned char lowerRecordToTable = cuttingProcess->device[liftServo]->addTask(LIFT_TABLE_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
 cuttingProcess->device[liftServo]->startByTriggerpositionOf(lowerRecordToTable, turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
 //        cuttingProcess->device[vacuumSolenoid]->stopByTriggerpositionOf(gripNewRecord, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION, 0);
 freeRam();
 
 //  lift for going to park position: start with startDelay after turn table was reached (LIFT_TABLE_POSITION)
 unsigned char liftForParkPosition = cuttingProcess->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, 500);
 cuttingProcess->device[liftServo]->startByTriggerpositionOf(liftForParkPosition, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
 freeRam();
 
 //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnToParkPosition = cuttingProcess->device[turnServo]->addTask(TURN_PARK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
 cuttingProcess->device[turnServo]->startByTriggerpositionOf(turnToParkPosition, liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
 unsigned char lowerForParkPosition = cuttingProcess->device[liftServo]->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
 cuttingProcess->device[liftServo]->startByTriggerpositionOf(lowerForParkPosition, turnServo, turnToParkPosition, TURN_TO_PARK_TRIGGER_LIFT);
 freeRam();
 
 scheduledTask turnTheTable = cuttingProcess->device[tableDrive]->addTask(PWM_FULL_POWER, PWM_FREQUENCY_LOW, PWM_NO_RAMPING, PWM_NO_RAMPING);
 cuttingProcess->device[tableDrive]->task[turnTheTable]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
 //            cuttingProcess->device[tableDrive]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
 

 
 //  make the groves: start when cuttinghead is in cutting position
 
 //  lift head after cutting: start, when end groove is finished
 
 //  go back to park position: start when cutting head is lifted up
 
 //  lower head to record surface: start when reached start position of start groove
 */



// ============================================================================================================================
// ============= moves ========================================================================================================
// ============================================================================================================================

// ########## scheduledTask addTaskWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration);
// ########## scheduledTask addTask(float target, float velocity, float acceleration, float deceleration) {


// ########## startByDate(unsigned long startTime)
// ########## startByButton(schedulerControlButton startButton)
// ########## startByTriggerpositionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask, signed long startTriggerPosition)

// ########## switchToNextTaskByDate(unsigned long startTime);
// ########## switchToNextTaskByButton(schedulerControlButton switchingButton);
// ########## switchToNextTaskByTriggerpositionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask, signed long startTriggerPosition);

// ########## stopByTimeout(unsigned long timeout, stoppingMode stopping)
// ########## stopByButton(schedulerControlButton stopButton, stoppingMode stopping)
// ########## stopByTriggerpositionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping)

//  stopping:  STOP_IMMEDIATELY, STOP_NORMAL

// ============================================================================================================================
// ============================================================================================================================
// ============================================================================================================================





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

