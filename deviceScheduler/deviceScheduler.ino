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
#include <malloc.h>



// Prototypes
void freeRam ();


// Define variables and constants
extern char _end;
extern "C" char *sbrk(int i);




// ------------- debug --------------------------------------------------------------------------------------------------
//static unsigned char DEVICESCHEDULER_VERBOSE = DEVICESCHEDULER_SHOW_TAB_VIEW DEVICESCHEDULER_BASICPUTPUT | DEVICESCHEDULER_MEMORYDEBUG;
//static unsigned char DEVICESCHEDULER_VERBOSE = DEVICESCHEDULER_SHOW_TASK_VIEW | DEVICESCHEDULER_BASICPUTPUT | DEVICESCHEDULER_MEMORYDEBUG;
//static unsigned char DEVICESCHEDULER_VERBOSE = 0;

// ------------- globals --------------------------------------------------------------------------------------------------
CCDeviceScheduler *scheduler;

// ------------- prototypes --------------------------------------------------------------------------------------------------
void setup();




void loop() {
    
    Serial.println(), Serial.println(), Serial.println();
    for (int i = 0; i < 56; i++) Serial.print(F("#"));
    Serial.println(), Serial.println(), Serial.println();
    
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
    unsigned char headLeftServo = scheduler->addServo(SERVO_HEAD_LEFT_NAME, SERVO_HEAD_LEFT_PIN, SERVO_HEAD_LEFT_MIN_POSITION, SERVO_HEAD_LEFT_MAX_POSITION, HEAD_LEFT_PARK_POSITION);
    freeRam();
    unsigned char headRightServo = scheduler->addServo(SERVO_HEAD_RIGHT_NAME, SERVO_HEAD_RIGHT_PIN, SERVO_HEAD_RIGHT_MIN_POSITION, SERVO_HEAD_RIGHT_MAX_POSITION, HEAD_RIGHT_PARK_POSITION);
    freeRam();
    unsigned char stockStepper = scheduler->addStepper(STEPPER_STOCK_NAME, STEPPER_STOCK_STEP_PIN, STEPPER_STOCK_DIR_PIN, STEPPER_STOCK_ENABLE_PIN, STEPPER_STOCK_HIGHEST_STEPPINGMODE, STEPPER_STOCK_STEPMODECODES, STEPPER_STOCK_MICROSTEPPINS, STEPPER_STOCK_STEPS_PER_ROTATION);
    unsigned char catStepper = scheduler->addStepper(STEPPER_CAT_NAME, STEPPER_CAT_STEP_PIN, STEPPER_CAT_DIR_PIN, STEPPER_CAT_ENABLE_PIN, STEPPER_CAT_HIGHEST_STEPPINGMODE, STEPPER_CAT_STEPMODECODES, STEPPER_CAT_MICROSTEPPINS, STEPPER_CAT_STEPS_PER_ROTATION);
    freeRam();
    unsigned char tableDrive = scheduler->addSwitch(TABLEDRIVE_NAME, TABLEDRIVE_PIN, TABLEDRIVE_OFF);
    //    unsigned char vacuumSolenoid = scheduler->addSolenoid(SOLENOID_VACUUM_NAME, SOLENOID_VACUUM_PIN);
    //    freeRam();
    
    
    scheduler->getAllDevices();
    
    
    
    
    
    
    for (;;) {
        
        // ============================================================================================================================
        // ============= moves ========================================================================================================
        // ============================================================================================================================
        
        // ########## unsigned char addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration);
        // ########## unsigned char addMove(float target, float velocity, float acceleration, float deceleration) {
        
        
        // ########## startByDate(unsigned long startTime)
        // ########## startByButton(unsigned char startButton, boolean startButtonState)
        // ########## startByTriggerpositionOf(unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition)
        
        // ########## setDisposeAfterMove(unsigned char stopTriggerDevice, unsigned char stopTriggerMove)
        
        // ########## switchToNextTaskByDate(unsigned long startTime);
        // ########## switchToNextTaskByButton(unsigned char startButton, boolean startButtonState);
        // ########## switchToNextTaskByTriggerpositionOf(unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
        
        // ########## stopByTimeout(unsigned long timeout, unsigned char stopMode)
        // ########## stopByButton(unsigned char stopButton, boolean stopButtonState, unsigned char stopMode)
        // ########## stopByTriggerpositionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, unsigned char stopMode)
        
        //  stopModes:  STOP_SHARPLY, STOP_NORMAL, STOP_DYNAMIC
        
        // ============================================================================================================================
        // ============================================================================================================================
        // ============================================================================================================================
        
        
        
        Serial.println("................................. initialisation .................................");
        
        
        unsigned char initCatStepper = scheduler->device[catStepper]->addMove(-400000, 6400, 3200.0, 3200.0);
        scheduler->device[catStepper]->task[initCatStepper]->startByDate(100);
        scheduler->device[catStepper]->task[initCatStepper]->stopByButton(CAT_PARK_BUTTON, HIGH, 0);
        
        
        
        scheduler->getAllMoves();
        scheduler->reviewMoves();
        scheduler->getAllMoves();
        
        scheduler->runTheLoop();
        
        scheduler->deleteAllMoves();
        
        Serial.println("...................................... done ......................................");
        
        
        
        scheduler->device[catStepper]->currentPosition = 0;
        
        
        if (digitalRead(NORMAL_CUTTING) == HIGH) {
            
            freeRam();
            
            
            //  move to start groove:
            unsigned char driveToCuttingStartPosition = scheduler->device[catStepper]->addMove(CAT_CUTTING_START_POSITION, 4800, 3200, 3200);
            scheduler->device[catStepper]->task[driveToCuttingStartPosition]->startByDate(100);
            scheduler->device[catStepper]->task[driveToCuttingStartPosition]->stopByButton(CAT_END_BUTTON, HIGH, 0);
            
            
            unsigned char turnTheTable = scheduler->device[tableDrive]->addMove(TABLEDRIVE_ON, 0, 0, 0);
            scheduler->device[tableDrive]->task[turnTheTable]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
            
            
            //  lower head to record surface: start when reached start position of start groove
            unsigned char lowerHeadLeftForCutting = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
            scheduler->device[headLeftServo]->task[lowerHeadLeftForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
            scheduler->device[headLeftServo]->task[lowerHeadLeftForCutting]->stopDynamicallyBySensor(A5, 660, 220, 1.0, STOP_VERY_FAST);
            
            //  lower head to record surface: start when reached start position of start groove
            unsigned char lowerHeadRightForCutting = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
            scheduler->device[headRightServo]->task[lowerHeadRightForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
            scheduler->device[headRightServo]->task[lowerHeadRightForCutting]->stopDynamicallyBySensor(A5, 660, 220, 0.6, STOP_PRECISE);
            
            unsigned char makeStartGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, CAT_MOTOR_DEGREES_PER_SECOND_START, 800, 800);
            scheduler->device[catStepper]->task[makeStartGroove]->startAfterCompletionOf(headRightServo, lowerHeadRightForCutting);
            scheduler->device[catStepper]->task[makeStartGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeStartGroove, CAT_SONG_START_POSITION);
            
            unsigned char makeMainGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, CAT_MOTOR_DEGREES_PER_SECOND, 600, 600);
            scheduler->device[catStepper]->task[makeMainGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, CAT_SONG_END_POSITION);
            
            unsigned char makeEndGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, CAT_MOTOR_DEGREES_PER_SECOND_END, 800, 800);
            
            unsigned char liftHeadLeftAfterCutting = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
            scheduler->device[headLeftServo]->task[liftHeadLeftAfterCutting]->startAfterCompletionOf(catStepper, makeEndGroove);
            
            scheduler->device[tableDrive]->task[turnTheTable]->stopAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
            
            unsigned char liftHeadRightAfterCutting = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
            scheduler->device[headRightServo]->task[liftHeadRightAfterCutting]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
            
            unsigned char driveToParkPosition = scheduler->device[catStepper]->addMove(CAT_PARK_POSITION, 4800, 3200, 3200);
            scheduler->device[catStepper]->task[driveToParkPosition]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
            scheduler->device[catStepper]->task[driveToParkPosition]->stopByButton(CAT_PARK_BUTTON, HIGH, 0);
            
            
            
            
            // ============================================================================================================================
            // ============================================================================================================================
            // ============================================================================================================================
            
            
            scheduler->getAllDevices();
            
            scheduler->getAllMoves();
            
            scheduler->reviewMoves();
            
            scheduler->getAllMoves();
            
            freeRam();
            
            
            
            Serial.print("*** Inputs: CAT_FWD: ");
            Serial.print(digitalRead(CAT_FWD));
            Serial.print(", CAT_RWD: ");
            Serial.print(digitalRead(CAT_RWD));
            Serial.print(", START_CUTTING_BUTTON: ");
            Serial.print(digitalRead(START_CUTTING_BUTTON));
            Serial.print(", STOP_CUTTING_BUTTON: ");
            Serial.print(digitalRead(STOP_CUTTING_BUTTON));
            Serial.print(", CAT_PARK_BUTTON: ");
            Serial.println(digitalRead(CAT_PARK_BUTTON));
            
            
            delay(2000);
            
            scheduler->runTheLoop();
            
            scheduler->deleteAllMoves();
        } else {
            
            //  move to start groove:
            unsigned char driveToCuttingStartPositionMan = scheduler->device[catStepper]->addMove(CAT_CUTTING_START_POSITION, 4800, 3200, 3200);
            scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(100);
            scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(CAT_END_BUTTON, HIGH, 0);
            
            scheduler->reviewMoves();
            scheduler->runTheLoop();
            scheduler->deleteAllMoves();
            
            while (digitalRead(START_CUTTING_BUTTON) == HIGH) {
                if (digitalRead(CAT_FWD) == LOW) {
                    driveToCuttingStartPositionMan = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, 4800, 3200, 3200);
                    scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
                    scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(CAT_FWD, HIGH, 0);
                    
                    scheduler->reviewMoves();
                    scheduler->runTheLoop();
                    scheduler->deleteAllMoves();
                }
                if (digitalRead(CAT_RWD) == LOW) {
                    driveToCuttingStartPositionMan = scheduler->device[catStepper]->addMove(CAT_PARK_POSITION, 4800, 3200, 3200);
                    scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
                    scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(CAT_RWD, HIGH, 0);
                    
                    scheduler->reviewMoves();
                    scheduler->runTheLoop();
                    scheduler->deleteAllMoves();
                }
            }
            
            
            unsigned char turnTheTableMan = scheduler->device[tableDrive]->addMove(TABLEDRIVE_ON, 0, 0, 0);
            scheduler->device[tableDrive]->task[turnTheTableMan]->startByDate(100);
            
            //  lower head to record surface: start when reached start position of start groove
            unsigned char lowerHeadLeftForCuttingMan = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
            scheduler->device[headLeftServo]->task[lowerHeadLeftForCuttingMan]->startByDate(200);
            scheduler->device[headLeftServo]->task[lowerHeadLeftForCuttingMan]->stopDynamicallyBySensor(A5, 660, 220, 1.0, STOP_VERY_FAST);
            
            //  lower head to record surface: start when reached start position of start groove
            unsigned char lowerHeadRightForCuttingMan = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
            scheduler->device[headRightServo]->task[lowerHeadRightForCuttingMan]->startByDate(220);
            scheduler->device[headRightServo]->task[lowerHeadRightForCuttingMan]->stopDynamicallyBySensor(A5, 660, 220, 0.6, STOP_PRECISE);
            
            unsigned char makeMainGrooveMan = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, CAT_MOTOR_DEGREES_PER_SECOND, 600, 600);
            scheduler->device[catStepper]->task[makeMainGrooveMan]->startAfterCompletionOf(headRightServo, lowerHeadRightForCuttingMan);
            scheduler->device[catStepper]->task[makeMainGrooveMan]->stopByButton(STOP_CUTTING_BUTTON, LOW, 0);
            
            unsigned char liftHeadLeftAfterCuttingMan = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
            scheduler->device[headLeftServo]->task[liftHeadLeftAfterCuttingMan]->startAfterCompletionOf(catStepper, makeMainGrooveMan);
            
            unsigned char liftHeadRightAfterCuttingMan = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
            scheduler->device[headRightServo]->task[liftHeadRightAfterCuttingMan]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCuttingMan);
            
            scheduler->device[tableDrive]->task[liftHeadRightAfterCuttingMan]->stopAfterCompletionOf(headLeftServo, liftHeadLeftAfterCuttingMan);
            
            unsigned char driveToParkPositionMan = scheduler->device[catStepper]->addMove(CAT_PARK_POSITION, 4800, 3200, 3200);
            scheduler->device[catStepper]->task[driveToParkPositionMan]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCuttingMan);
            scheduler->device[catStepper]->task[driveToParkPositionMan]->stopByButton(CAT_PARK_BUTTON, HIGH, 0);
            
       
            scheduler->reviewMoves();
            scheduler->runTheLoop();
            scheduler->deleteAllMoves();
            
            
            
        }
    }
    
    delay(2000);
    
    
    Serial.println("delete scheduler");
    
    delete scheduler;
    scheduler = NULL;
    freeRam();
    
    delay(2000);
    
}



void setup() {
    //    pinMode(CAT_FWD, INPUT_PULLUP);
    //    pinMode(LOADING_BUTTON, INPUT_PULLUP);
    //    pinMode(RECORD_AVAILABLE_BUTTON, INPUT_PULLUP);
    //    pinMode(CAT_PARK_BUTTON, INPUT_PULLUP);
    
    
    pinMode(NORMAL_CUTTING, INPUT_PULLUP);
    pinMode(CAT_FWD, INPUT_PULLUP);
    pinMode(CAT_RWD, INPUT_PULLUP);
    pinMode(START_CUTTING_BUTTON, INPUT_PULLUP);
    pinMode(STOP_CUTTING_BUTTON, INPUT_PULLUP);
    
    pinMode(CAT_PARK_BUTTON, INPUT_PULLUP);
    pinMode(CAT_END_BUTTON, INPUT_PULLUP);
    
    pinMode(I_AM_LATE_LED, OUTPUT);
    
    Serial.begin(115200);
    Serial.println(), Serial.println(), Serial.println();
    Serial.println(), Serial.println(), Serial.println();
        
    Serial.println();
    Serial.println();
    Serial.println();
    
    Serial.print("groovePitch is set to ");
    Serial.print(RECORD_GROOVE_PITCH);
    Serial.print("mm, startGroovePitch is set to ");
    Serial.print(START_GROOVE_PITCH);
    Serial.print("mm, startGrooveWidth is set to ");
    Serial.print(STARTGROOVE_WIDTH);
    Serial.print("mm, endGroovePitch is set to ");
    Serial.print(END_GROOVE_PITCH);
    Serial.print("mm, endGrooveWidthMin is set to ");
    Serial.print(ENDGROOVE_WIDTH_MIN);
    Serial.println("mm:");
    Serial.print("maxPlayRange: ");
    Serial.print(MAX_PLAY_RANGE);
    Serial.print("mm, maxRecordGrooves: ");
    Serial.print(MAX_RECORDGROOVES);
    Serial.print(", max. playTime: ");
    Serial.print(MAX_PLAY_TIME_MINUTES);
    Serial.println("min");
    Serial.println();
    
    Serial.print("playTime is set to ");
    Serial.print(PLAYTIME_MINUTES);
    Serial.println("min:");
    Serial.print("grooves [ = recordTurns]: ");
    Serial.print(RECORDTURNS);
    Serial.print(", playingRange: ");
    Serial.print(PLAYING_RANGE);
    Serial.print("mm, endGrooveWidth: ");
    Serial.print(ENDGROOVE_WIDTH);
    Serial.println("mm");
    Serial.println();
    
    Serial.print("secondsPerTableRotation: ");
    Serial.println(SECONDS_PER_TABLE_ROTATION);
    
    Serial.print("Cutting the song: catSpeed: ");
    Serial.print(CAT_SPEED);
    Serial.print("catmotorTurns: ");
    Serial.print(CAT_MOTOR_TURNS_PER_SONG);
    Serial.print(", catMotorDegrees: ");
    Serial.print(CAT_MOTOR_DEGREES_PER_SONG);
    Serial.print(", catMotorTurnsPerSecond: ");
    Serial.print(CAT_MOTOR_TURNS_PER_SECOND);
    Serial.print(", catMotorDegreesPerSecond: ");
    Serial.println(CAT_MOTOR_DEGREES_PER_SECOND);
    
    Serial.print("StartGroove: catSpeed: ");
    Serial.print(CAT_SPEED_START);
    Serial.print(", catmotorTurns: ");
    Serial.print(CAT_MOTOR_TURNS_PER_START);
    Serial.print(", catMotorDegrees: ");
    Serial.print(CAT_MOTOR_DEGREES_PER_START);
    Serial.print(", catMotorTurnsPerSecond: ");
    Serial.print(CAT_MOTOR_TURNS_PER_SECOND_START);
    Serial.print(", catMotorDegreesPerSecond: ");
    Serial.println(CAT_MOTOR_DEGREES_PER_SECOND_START);
    
    Serial.print("EndGroove: catSpeed: ");
    Serial.print(CAT_SPEED_END);
    Serial.print("catmotorTurns: ");
    Serial.print(CAT_MOTOR_TURNS_PER_END);
    Serial.print(", catMotorDegrees: ");
    Serial.print(CAT_MOTOR_DEGREES_PER_END);
    Serial.print(", catMotorTurnsPerSecond: ");
    Serial.print(CAT_MOTOR_TURNS_PER_SECOND_END);
    Serial.print(", catMotorDegreesPerSecond: ");
    Serial.println(CAT_MOTOR_DEGREES_PER_SECOND_END);
    
    
    Serial.print("CAT_CUTTING_START_POSITION: ");
    Serial.print(CAT_CUTTING_START_POSITION);
    Serial.print(", CAT_SONG_START_POSITION: ");
    Serial.print(CAT_SONG_START_POSITION);
    Serial.print(", CAT_SONG_END_POSITION: ");
    Serial.print(CAT_SONG_END_POSITION);
    Serial.print(", CAT_CUTTING_END_POSITION: ");
    Serial.print(CAT_CUTTING_END_POSITION);
    
    
    Serial.println();
    Serial.println();
    Serial.println();
    
    
}

//int freeRam () {
//    extern int __heap_start, *__brkval;
//    int v;
//    int fr = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
//    Serial.print("Free ram: ");
//    Serial.println(fr);
//}


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



//        unsigned char switchTableOn = scheduler->device[tableDrive]->addMove(TABLEDRIVE_ON, 0, 0, 0);
//        scheduler->device[tableDrive]->setStartAfterCompletion(switchTableOn, headLeftServo, liftHeadFirstLeft);
//        unsigned char switchTableOff = scheduler->device[tableDrive]->addMove(TABLEDRIVE_OFF, 0, 0, 0);
//        scheduler->device[tableDrive]->startByDate(switchTableOff, 10000);

//        unsigned char riseHead = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//        scheduler->device[headLeftServo]->setStartAfterCompletion(riseHead, headLeftServo, lowerHeadFirstLeft);
//        scheduler->device[headLeftServo]->stopByTimeout(riseHead, 10000, true);
//
//        unsigned char lowerHead = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//        scheduler->device[headLeftServo]->setStartAfterCompletion(lowerHead, headLeftServo, riseHead);
//        scheduler->device[headLeftServo]->stopDynamicallyBySensor(lowerHead, A5, 1000, 200, 0);
//        scheduler->device[headLeftServo]->stopByTimeout(lowerHead, 10000, true);



//  unsigned char liftHeadFirstRight = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//  scheduler->device[headRightServo]->startByDate(liftHeadFirstRight, 100);


// here may be a problem: preperation of next move before 'currentPosition' set to 0


/*
 //  supply a new record from stock
 unsigned char supplyNewRecord = scheduler->device[stockStepper]->addMove(36000L, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, STOCK_SUPPLY_RECORD_ACCEL);
 //        scheduler->device[stockStepper]->startByDate(supplyNewRecord, 100);
 scheduler->device[stockStepper]->startByButton(supplyNewRecord, CAT_FWD, LOW);
 scheduler->device[stockStepper]->stopByButton(supplyNewRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);
 
 //  lift grappler: start soon
 unsigned char liftFromParkPosition = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
 //        scheduler->device[liftServo]->startByDate(liftFromParkPosition, 100);
 //        scheduler->device[turnServo]->startByButton(turnToStock, RECORD_AVAILABLE_BUTTON, HIGH);
 freeRam();
 
 //  turn grappler to stock: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnToStock = scheduler->device[turnServo]->addMove(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
 scheduler->device[turnServo]->startByTriggerpositionOf(turnToStock, liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
 unsigned char lowerToStock = scheduler->device[liftServo]->addMove(LIFT_STOCK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
 //        scheduler->device[liftServo]->startByDate(lowerToStock, 3000);
 scheduler->device[liftServo]->startByTriggerpositionOf(lowerToStock, turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
 freeRam();
 
 //         //  grip new record: start when grappler reached stock (LIFT_STOCK_POSITION)
 //         unsigned char gripNewRecord = scheduler->device[vacuumSolenoid]->addMove(SOLENOID_DUTYCYCLE, SOLENOID_FREQUENCY, 0, 500);
 //         scheduler->device[vacuumSolenoid]->startByTriggerpositionOf(gripNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
 //         freeRam();
 
 //  lift the new record: start with startDelay after stock was reached (LIFT_STOCK_POSITION)
 unsigned char liftNewRecord = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, 2666);
 scheduler->device[liftServo]->startByTriggerpositionOf(liftNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
 freeRam();
 
 //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnRecordToTable = scheduler->device[turnServo]->addMove(TURN_TABLE_POSITION, TURN_SPEED_SLOW, TURN_ACCEL_SLOW, NO_START_DELAY);
 scheduler->device[turnServo]->startByTriggerpositionOf(turnRecordToTable, liftServo, liftNewRecord, LIFT_FROM_STOCK_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
 unsigned char lowerRecordToTable = scheduler->device[liftServo]->addMove(LIFT_TABLE_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
 scheduler->device[liftServo]->startByTriggerpositionOf(lowerRecordToTable, turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
 //        scheduler->device[vacuumSolenoid]->stopByTriggerpositionOf(gripNewRecord, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION, 0);
 freeRam();
 
 //  lift for going to park position: start with startDelay after turn table was reached (LIFT_TABLE_POSITION)
 unsigned char liftForParkPosition = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, 500);
 scheduler->device[liftServo]->startByTriggerpositionOf(liftForParkPosition, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
 freeRam();
 
 //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnToParkPosition = scheduler->device[turnServo]->addMove(TURN_PARK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
 scheduler->device[turnServo]->startByTriggerpositionOf(turnToParkPosition, liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
 unsigned char lowerForParkPosition = scheduler->device[liftServo]->addMove(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
 scheduler->device[liftServo]->startByTriggerpositionOf(lowerForParkPosition, turnServo, turnToParkPosition, TURN_TO_PARK_TRIGGER_LIFT);
 freeRam();
 
 
 
 //  make the groves: start when cuttinghead is in cutting position
 
 //  lift head after cutting: start, when end groove is finished
 
 //  go back to park position: start when cutting head is lifted up
 
 //  lower head to record surface: start when reached start position of start groove
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

