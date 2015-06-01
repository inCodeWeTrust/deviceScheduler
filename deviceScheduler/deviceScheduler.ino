/// 
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



// Prototypes
int freeRam ();


// Define variables and constants



// ------------- debug --------------------------------------------------------------------------------------------------
//static unsigned char CCDeviceSchedulerVerbose = SHOW_TAB_VIEW DEVICESCHEDULER_BASICPUTPUT | DEVICESCHEDULER_MEMORYDEBUG;
//static unsigned char CCDeviceSchedulerVerbose = SHOW_TASK_VIEW | DEVICESCHEDULER_BASICPUTPUT | DEVICESCHEDULER_MEMORYDEBUG;
//static unsigned char CCDeviceSchedulerVerbose = 0;

// ------------- globals --------------------------------------------------------------------------------------------------
CCDeviceScheduler *scheduler;

// ------------- prototypes --------------------------------------------------------------------------------------------------
void setup();




void loop() {
    
    freeRam();
    
    scheduler = new CCDeviceScheduler;
    
    freeRam();
    
    // ============================================================================================================================
    // ============= devices ======================================================================================================
    // ============================================================================================================================
    unsigned char liftServo = scheduler->addServo(SERVO_LIFT_NAME, SERVO_LIFT_PIN, SERVO_LIFT_MIN_POSITION, SERVO_LIFT_MAX_POSITION, LIFT_PARK_POSITION);
    freeRam();
    unsigned char turnServo = scheduler->addServo(SERVO_TURN_NAME, SERVO_TURN_PIN, SERVO_TURN_MIN_POSITION, SERVO_TURN_MAX_POSITION, TURN_PARK_POSITION);
    freeRam();
    unsigned char stockStepper = scheduler->addStepper(STEPPER_STOCK_NAME, STEPPER_STOCK_STEP_PIN, STEPPER_STOCK_DIR_PIN, STEPPER_STOCK_ENABLE_PIN, STEPPER_STOCK_HIGHEST_STEPPINGMODE, (unsigned char*)(unsigned char[])STEPPER_STOCK_STEPMODECODE, STEPPER_STOCK_MICROSTEP_0_PIN, STEPPER_STOCK_MICROSTEP_1_PIN, STEPPER_STOCK_MICROSTEP_2_PIN, STEPPER_STOCK_ANGLE_PER_STEP);
    freeRam();
    unsigned char vacuumSolenoid = scheduler->addSolenoid(SOLENOID_VACUUM_NAME, SOLENOID_VACUUM_PIN);
    freeRam();
    
    
    for (int n = 20; n < 2800; n+=40) {
        // ============================================================================================================================
        // ============= moves ========================================================================================================
        // ============================================================================================================================
        
        // ########## setStartDateForMove(unsigned char moveIndex, unsigned long startTime)
        // ########## setStartButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState)
        // ########## setStartEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition)
        //
        // ########## setTimeoutForMove(unsigned char moveIndex, unsigned long timeout, boolean stopSharply)
        // ########## setStopButtonForMove(unsigned char moveIndex, unsigned char stopButton, boolean stopButtonState, boolean stopSharply)
        // ########## setStopEventForMove(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply)
        
        int theValue = analogRead(A2) / 2;
        //  supply a new record: started by START-button, terminated by RECORD_AVAILABLE_BUTTON
        unsigned char supplyRecord = scheduler->device[stockStepper]->addMove((long)n * n / 80, n, 100, 103);
        //    unsigned char supplyRecord = scheduler->device[stockStepper]->addMove(320, 400, 0.4, 103);
        //    unsigned char supplyRecord_01 = scheduler->device[stockStepper]->addMove(320, 400, 1.4, 103);
        //    unsigned char supplyRecord = scheduler->device[stockStepper]->addMove(100UL * 360UL, STEPPER_SUPPLY_RECORD_SPEED, STEPPER_SUPPLY_RECORD_ACCEL, 2103);
        //    //      scheduler->device[stockStepper]->setStartButtonForMove(supplyRecord, START_BUTTON, LOW);
        scheduler->device[stockStepper]->setStartDateForMove(supplyRecord, 100);
        scheduler->device[stockStepper]->setStopButtonForMove(supplyRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);
        //            scheduler->device[stockStepper]->setStartEventForMove(supplyRecord_01, stockStepper, supplyRecord, 320);
        
        freeRam();
        /*
         //  turn grappler to stock: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
         unsigned char turnToStock = scheduler->device[turnServo]->addMove(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
         scheduler->device[turnServo]->setStartButtonForMove(turnToStock, RECORD_AVAILABLE_BUTTON, HIGH);
         freeRam();
         
         //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
         unsigned char lowerToStock = scheduler->device[liftServo]->addMove(LIFT_STOCK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
         scheduler->device[liftServo]->setStartEventForMove(lowerToStock, turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
         freeRam();
         
         //  grip new record: start when grappler reached stock (LIFT_STOCK_POSITION)
         unsigned char gripNewRecord = scheduler->device[vacuumSolenoid]->addMove(SOLENOID_DUTYCYCLE, SOLENOID_FREQUENCY, 0, 500);
         scheduler->device[vacuumSolenoid]->setStartEventForMove(gripNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
         freeRam();
         
         //  lift the new record: start with startDelay after stock was reached (LIFT_STOCK_POSITION)
         unsigned char liftNewRecord = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, 2666);
         scheduler->device[liftServo]->setStartEventForMove(liftNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
         freeRam();
         
         //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
         unsigned char turnRecordToTable = scheduler->device[turnServo]->addMove(TURN_TABLE_POSITION, TURN_SPEED_SLOW, TURN_ACCEL_SLOW, NO_START_DELAY);
         scheduler->device[turnServo]->setStartEventForMove(turnRecordToTable, liftServo, liftNewRecord, LIFT_FROM_STOCK_TRIGGER_TURN);
         freeRam();
         
         //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
         unsigned char lowerRecordToTable = scheduler->device[liftServo]->addMove(LIFT_TABLE_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
         scheduler->device[liftServo]->setStartEventForMove(lowerRecordToTable, turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
         scheduler->device[vacuumSolenoid]->setStopEventForMove(gripNewRecord, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION, 0);
         freeRam();
         
         //  lift for going to park position: start with startDelay after turn table was reached (LIFT_TABLE_POSITION)
         //    unsigned char liftForParkPosition = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, 903);
         unsigned char liftForParkPosition = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, 500);
         //      scheduler->device[liftServo]->setStopButtonForMove(liftForParkPosition, START_BUTTON, LOW, true);
         scheduler->device[liftServo]->setStartEventForMove(liftForParkPosition, liftServo, lowerToStock, LIFT_TABLE_POSITION);
         //scheduler->device[liftServo]->setStartEventForMove(liftForParkPosition, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
         freeRam();
         
         //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
         unsigned char turnToParkPosition = scheduler->device[turnServo]->addMove(TURN_PARK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
         scheduler->device[turnServo]->setStartEventForMove(turnToParkPosition, liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
         freeRam();
         
         //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
         //    unsigned char lowerForParkPosition = scheduler->device[liftServo]->addMove(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, 903);
         unsigned char lowerForParkPosition = scheduler->device[liftServo]->addMove(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
         scheduler->device[liftServo]->setStartEventForMove(lowerForParkPosition, liftServo, liftForParkPosition, LIFT_UP_POSITION);
         //      scheduler->device[liftServo]->setStopButtonForMove(lowerForParkPosition, START_BUTTON, LOW, 1);
         //    scheduler->device[liftServo]->setStartEventForMove(lowerForParkPosition, turnServo, turnToParkPosition, TURN_TO_PARK_TRIGGER_LIFT);
         freeRam();
         
         unsigned char goDownForLoading = scheduler->device[stockStepper]->addMove(-3600L, STEPPER_SUPPLY_RECORD_SPEED, STEPPER_SUPPLY_RECORD_ACCEL, 3003);
         scheduler->device[stockStepper]->setStartEventForMove(goDownForLoading, liftServo, lowerForParkPosition, LIFT_PARK_POSITION);
         //    scheduler->device[stockStepper]->setStartDateForMove(goDownForLoading, 100);
         //      scheduler->device[stockStepper]->setStartButtonForMove(supplyRecord, START_BUTTON, LOW);
         //      scheduler->device[stockStepper]->setStopButtonForMove(supplyRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);
         
         */
        // ============================================================================================================================
        // ============================================================================================================================
        // ============================================================================================================================
        
        
        scheduler->getAllDevices();
        
        scheduler->getAllMoves();
        
        
        
        freeRam();
        
        
        delay(2000);
        
        scheduler->runTheLoop();
        
        scheduler->deleteAllMoves();
    }
    
    delay(2000);
    
    
    Serial.println("delete scheduler");
    
    delete scheduler;
    scheduler = NULL;
    freeRam();
    
    delay(2000);
    
}



void setup() {
    pinMode(START_BUTTON, INPUT_PULLUP);
    pinMode(LOADING_BUTTON, INPUT_PULLUP);
    pinMode(RECORD_AVAILABLE_BUTTON, INPUT_PULLUP);
    pinMode(I_AM_LATE_LED, OUTPUT);
    
    Serial.begin(115200);
    Serial.println(), Serial.println(), Serial.println();
    for (int i = 0; i < 56; i++) Serial.print(F("#"));
    Serial.println(), Serial.println(), Serial.println();
}

int freeRam () {
    extern int __heap_start, *__brkval;
    int v;
    int fr = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    Serial.print("Free ram: ");
    Serial.println(fr);
    
}

