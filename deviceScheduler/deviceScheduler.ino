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

//#include <Streaming.h>
//#include <malloc.h>



// Prototypes
int freeRam ();


// Define variables and constants
//extern char _end;
//extern "C" char *sbrk(int i);




// ------------- debug --------------------------------------------------------------------------------------------------
//static unsigned char DEVICESCHEDULER_VERBOSE = DEVICESCHEDULER_SHOW_TAB_VIEW DEVICESCHEDULER_BASICPUTPUT | DEVICESCHEDULER_MEMORYDEBUG;
//static unsigned char DEVICESCHEDULER_VERBOSE = DEVICESCHEDULER_SHOW_TASK_VIEW | DEVICESCHEDULER_BASICPUTPUT | DEVICESCHEDULER_MEMORYDEBUG;
//static unsigned char DEVICESCHEDULER_VERBOSE = 0;

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
    unsigned char turnServo = scheduler->addServo(SERVO_TURN_NAME, SERVO_TURN_INDEX, SERVO_TURN_MIN_POSITION, SERVO_TURN_MAX_POSITION, TURN_PARK_POSITION);
    freeRam();
    unsigned char headLeftServo = scheduler->addServo(SERVO_HEAD_LEFT_NAME, SERVO_HEAD_LEFT_PIN, SERVO_HEAD_LEFT_MIN_POSITION, SERVO_HEAD_LEFT_MAX_POSITION, HEAD_LEFT_PARK_POSITION);
    freeRam();
    unsigned char headRightServo = scheduler->addServo(SERVO_HEAD_RIGHT_NAME, SERVO_HEAD_RIGHT_PIN, SERVO_HEAD_RIGHT_MIN_POSITION, SERVO_HEAD_RIGHT_MAX_POSITION, HEAD_RIGHT_PARK_POSITION);
    freeRam();
    unsigned char stockStepper = scheduler->addStepper(STEPPER_STOCK_NAME, STEPPER_STOCK_STEP_PIN, STEPPER_STOCK_DIR_PIN, STEPPER_STOCK_ENABLE_PIN, STEPPER_STOCK_HIGHEST_STEPPINGMODE, STEPPER_STOCK_STEPMODECODES, STEPPER_STOCK_MICROSTEPPINS, STEPPER_STOCK_STEPS_PER_ROTATION);
    unsigned char catStepper = scheduler->addStepper(STEPPER_CAT_NAME, STEPPER_CAT_STEP_PIN, STEPPER_CAT_DIR_PIN, STEPPER_CAT_ENABLE_PIN, STEPPER_CAT_HIGHEST_STEPPINGMODE, STEPPER_CAT_STEPMODECODES, STEPPER_CAT_MICROSTEPPINS, STEPPER_CAT_STEPS_PER_ROTATION);
    freeRam();
    //    unsigned char vacuumSolenoid = scheduler->addSolenoid(SOLENOID_VACUUM_NAME, SOLENOID_VACUUM_PIN);
    //    freeRam();
    
    
    for (int n = 0; n < 2000; n+=100) {
        // ============================================================================================================================
        // ============= moves ========================================================================================================
        // ============================================================================================================================
        
        // ########## setStartDateForMove(unsigned char moveIndex, unsigned long startTime)
        // ########## setStartButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState)
        // ########## setStartEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition)
        
        // ########## setDisposeFollowingMoveForMove(unsigned char moveIndex)
        
        // ########## setSwitchDateForMove(unsigned char moveIndex, unsigned long startTime);
        // ########## setSwitchButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState);
        // ########## setSwitchEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
        
        // ########## setTimeoutForMove(unsigned char moveIndex, unsigned long timeout, boolean stopSharply)
        // ########## setStopButtonForMove(unsigned char moveIndex, unsigned char stopButton, boolean stopButtonState, boolean stopSharply)
        // ########## setStopEventForMove(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply)
        

        //        scheduler->device[stockStepper]->setStartDateForMove(test, 100);
//        scheduler->device[stockStepper]->setSwitchEventForMove(supplyRecord, stockStepper, supplyRecord, -(2 + 10 * n));
//        unsigned char supplyRecord1 = scheduler->device[stockStepper]->addMove(-3 * 360, 100, 50.0, 50.0);
//        
//        unsigned char supplyRecord2 = scheduler->device[stockStepper]->addMove(-2080, 160, 60.0, 60.0);
//        scheduler->device[stockStepper]->setStartDateForMove(supplyRecord2, 16000);
//        scheduler->device[stockStepper]->setStopEventForMove(supplyRecord2, stockStepper, supplyRecord2, -(1100 + 10 * n), false);
//        scheduler->device[stockStepper]->setSwitchEventForMove(supplyRecord2, stockStepper, supplyRecord2, n + 40);
//        unsigned char supplyRecord3 = scheduler->device[stockStepper]->addMove(0, 20, 8.0, 8.0);

//        scheduler->device[stockStepper]->setSwitchEventForMove(supplyRecord3, stockStepper, supplyRecord3, 180);
//        unsigned char supplyRecord4 = scheduler->device[stockStepper]->addMove(0, 80, 8.0, 8.0);
        
//        scheduler->device[stockStepper]->setStartEventForMove(supplyRecord4, stockStepper, supplyRecord3, n + 360 + 72);
        
//        
//        unsigned char supplyRecord4 = scheduler->device[stockStepper]->addMove(-2 * 36, 10, 5.0, 5.0);
//        scheduler->device[stockStepper]->setStartDateForMove(supplyRecord4, 35000);
//        scheduler->device[stockStepper]->setSwitchEventForMove(supplyRecord4, stockStepper, supplyRecord4, -n);
//        unsigned char supplyRecord5 = scheduler->device[stockStepper]->addMove(-2 * 36, 20, 5.0, 5.0);
//        
//
//        unsigned char supplyRecord6 = scheduler->device[stockStepper]->addMove(2 * 36, 10, 5.0, 5.0);
//        scheduler->device[stockStepper]->setStartDateForMove(supplyRecord6, 50000);
//        scheduler->device[stockStepper]->setSwitchEventForMove(supplyRecord6, stockStepper, supplyRecord6, n);
//        unsigned char supplyRecord7 = scheduler->device[stockStepper]->addMove(-2 * 36, 12, 6.0, 6.0);
//
//        unsigned char supplyRecord8 = scheduler->device[stockStepper]->addMove(-2 * 36, 10, 5.0, 5.0);
//        scheduler->device[stockStepper]->setStartDateForMove(supplyRecord8, 65000);
//        scheduler->device[stockStepper]->setSwitchEventForMove(supplyRecord8, stockStepper, supplyRecord8, -n);
//        unsigned char supplyRecord9 = scheduler->device[stockStepper]->addMove(2 * 36, 18, 6.0, 6.0);
//
        
        //    unsigned char supplyRecord = scheduler->device[stockStepper]->addMove(320, 400, 0.4, 103);
        //    unsigned char supplyRecord_01 = scheduler->device[stockStepper]->addMove(320, 400, 1.4, 103);
        //    unsigned char supplyRecord = scheduler->device[stockStepper]->addMove(100UL * 360UL, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, 2103);
        //    //      scheduler->device[stockStepper]->setStartButtonForMove(supplyRecord, START_BUTTON, LOW);
//        scheduler->device[stockStepper]->setStartDateForMove(supplyRecord, 100);
//        scheduler->device[stockStepper]->setStopButtonForMove(supplyRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);
        //            scheduler->device[stockStepper]->setStartEventForMove(supplyRecord_01, stockStepper, supplyRecord, 320);
//        scheduler->device[stockStepper]->setSwitchDateForMove(supplyRecord, 3000);
//        unsigned char supplyRecord2 = scheduler->device[stockStepper]->addMove(n * 720, n * 100, 100.0, 50.0);
//        
        freeRam();

        
        
        //  supply a new record from stock
        unsigned char supplyNewRecord = scheduler->device[stockStepper]->addMove(360L, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, 3003);
        scheduler->device[stockStepper]->setStartDateForMove(supplyNewRecord, 100);
        //        //      scheduler->device[stockStepper]->setStartButtonForMove(supplyRecord, START_BUTTON, LOW);
        //        //      scheduler->device[stockStepper]->setStopButtonForMove(supplyRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);

        
        //  lift grappler: start soon
        unsigned char liftFromParkPosition = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
        scheduler->device[liftServo]->setStartDateForMove(liftFromParkPosition, 100);
        freeRam();
        
        //  turn grappler to stock: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        unsigned char turnToStock = scheduler->device[turnServo]->addMove(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
        scheduler->device[turnServo]->setStartButtonForMove(turnToStock, RECORD_AVAILABLE_BUTTON, HIGH);
        freeRam();
        
        //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
        unsigned char lowerToStock = scheduler->device[liftServo]->addMove(LIFT_STOCK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
        scheduler->device[liftServo]->setStartEventForMove(lowerToStock, turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
        freeRam();
        
        //         //  grip new record: start when grappler reached stock (LIFT_STOCK_POSITION)
        //         unsigned char gripNewRecord = scheduler->device[vacuumSolenoid]->addMove(SOLENOID_DUTYCYCLE, SOLENOID_FREQUENCY, 0, 500);
        //         scheduler->device[vacuumSolenoid]->setStartEventForMove(gripNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
        //         freeRam();
        
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
//        scheduler->device[vacuumSolenoid]->setStopEventForMove(gripNewRecord, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION, 0);
        freeRam();
        
        //  lift for going to park position: start with startDelay after turn table was reached (LIFT_TABLE_POSITION)
        unsigned char liftForParkPosition = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, 500);
        scheduler->device[liftServo]->setStartEventForMove(liftForParkPosition, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
        freeRam();
        
        //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        unsigned char turnToParkPosition = scheduler->device[turnServo]->addMove(TURN_PARK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
        scheduler->device[turnServo]->setStartEventForMove(turnToParkPosition, liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
        freeRam();
        
        //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
        unsigned char lowerForParkPosition = scheduler->device[liftServo]->addMove(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
        scheduler->device[liftServo]->setStartEventForMove(lowerForParkPosition, turnServo, turnToParkPosition, TURN_TO_PARK_TRIGGER_LIFT);
        freeRam();
        
        //  move to start groove: start when grappler is in park position
        unsigned char moveToStartGroove = scheduler->device[catStepper]->addMove(CAT_GROOVE_START_POSITION, 2400, 800.0, 800.0);
        scheduler->device[catStepper]->setStartEventForMove(moveToStartGroove, liftServo, lowerForParkPosition, LIFT_PARK_POSITION);
        
        //  lower head to record surface: start when reached start position of start groove
        unsigned char lowerHeadForCuttingLeft = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        unsigned char lowerHeadForCuttingRight = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        scheduler->device[headLeftServo]->setStartEventForMove(lowerHeadForCuttingLeft, catStepper, moveToStartGroove, CAT_GROOVE_START_POSITION);
        scheduler->device[headRightServo]->setStartEventForMove(lowerHeadForCuttingRight, catStepper, moveToStartGroove, CAT_GROOVE_START_POSITION);
        
        //  make the groves: start when cuttinghead is in cutting position
        unsigned char makeStartGroove = scheduler->device[catStepper]->addMove(CAT_GROOVE_END_POSITION, 2400, 1200.0, 1200.0);
        scheduler->device[catStepper]->setStartEventForMove(makeStartGroove, headLeftServo, lowerHeadForCuttingLeft, HEAD_LEFT_CUT_POSITION);
        scheduler->device[catStepper]->setSwitchEventForMove(makeStartGroove, catStepper, makeStartGroove, CAT_SONG_START_POSITION);
        unsigned char makeMainGroove = scheduler->device[catStepper]->addMove(CAT_GROOVE_END_POSITION, 420, 600, 600);
        scheduler->device[catStepper]->setSwitchEventForMove(makeMainGroove, catStepper, makeMainGroove, CAT_SONG_END_POSITION);
        unsigned char makeEndGroove = scheduler->device[catStepper]->addMove(CAT_GROOVE_END_POSITION, 2400, 800, 800);
        
        //  lift head after cutting: start, when end groove is finished
        unsigned char liftHeadAfterCuttingLeft = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        unsigned char liftHeadAfterCuttingRight = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
        scheduler->device[headLeftServo]->setStartEventForMove(liftHeadAfterCuttingLeft, catStepper, makeEndGroove, CAT_SONG_END_POSITION);
        scheduler->device[headRightServo]->setStartEventForMove(liftHeadAfterCuttingRight, catStepper, makeEndGroove, CAT_SONG_END_POSITION);
        
        //  go back to park position: start when cutting head is lifted up
        unsigned char returnCatToParkPosition = scheduler->device[catStepper]->addMove(CAT_PARK_POSITION, 2400, 1200.0, 1200.0);
        scheduler->device[catStepper]->setStartEventForMove(returnCatToParkPosition, headLeftServo, liftHeadAfterCuttingLeft, HEAD_LEFT_PARK_POSITION);

        


        
        
        
//        unsigned char goDownForLoading = scheduler->device[stockStepper]->addMove(-3600L, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, 3003);
//        scheduler->device[stockStepper]->setStartEventForMove(goDownForLoading, liftServo, lowerForParkPosition, LIFT_PARK_POSITION);
//        //    scheduler->device[stockStepper]->setStartDateForMove(goDownForLoading, 100);
//        //      scheduler->device[stockStepper]->setStartButtonForMove(supplyRecord, START_BUTTON, LOW);
//        //      scheduler->device[stockStepper]->setStopButtonForMove(supplyRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);
//        
        // ============================================================================================================================
        // ============================================================================================================================
        // ============================================================================================================================
        
        
        scheduler->getAllDevices();
        
        scheduler->getAllMoves();
        
        scheduler->reviewMoves();

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
//    extern int __heap_start, *__brkval;
//    int v;
//    int fr = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
//    Serial.print("Free ram: ");
//    Serial.println(fr);
}

/*
void freeRam() {
    char *ramstart = (char *) 0x20070000;
    char *ramend = (char *) 0x20088000;
    char *heapend = sbrk(0);
    register char * stack_ptr asm( "sp" );
    struct mallinfo mi = mallinfo();
    Serial << "Ram used (bytes): " << endl
    << "  dynamic: " << mi.uordblks << endl
    << "  static:  " << &_end - ramstart << endl
    << "  stack:   " << ramend - stack_ptr << endl;
    Serial << "Estimation free Ram: " << stack_ptr - heapend + mi.fordblks << endl;
}
*/

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

