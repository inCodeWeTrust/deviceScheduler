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
unsigned long catSongStartPosition, catSongEndPosition, catCuttingEndPosition;
float catMotorSpeed_startGroove, catMotorSpeed_song, catMotorSpeed_endGroove;
unsigned long grooves_all;
float turnTableSpeed;

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
void calculateCuttingParameters();



void loop() {
    
    Serial.println(), Serial.println(), Serial.println();
    for (int i = 0; i < 56; i++) Serial.print(F("#"));
    Serial.println(), Serial.println(), Serial.println();
    
    freeRam();
    /* create a scheduler object */
    scheduler = new CCDeviceScheduler;
    
    freeRam();
    
    // ============================================================================================================================
    // ============= devices ======================================================================================================
    // ============================================================================================================================
    schedulerDevice liftServo = scheduler->addServo(SERVO_LIFT_NAME,
                                                  SERVO_LIFT_PIN,
                                                  SERVO_LIFT_MIN_POSITION,
                                                  SERVO_LIFT_MAX_POSITION,
                                                  LIFT_PARK_POSITION);
    freeRam();
    schedulerDevice turnServo = scheduler->addServo(SERVO_TURN_NAME,
                                                  SERVO_TURN_PIN,
                                                  SERVO_TURN_MIN_POSITION,
                                                  SERVO_TURN_MAX_POSITION,
                                                  TURN_PARK_POSITION);
    freeRam();
    schedulerDevice headLeftServo = scheduler->addServo(SERVO_HEAD_LEFT_NAME,
                                                      SERVO_HEAD_LEFT_PIN,
                                                      SERVO_HEAD_LEFT_MIN_POSITION,
                                                      SERVO_HEAD_LEFT_MAX_POSITION,
                                                      HEAD_LEFT_PARK_POSITION);
    freeRam();
    schedulerDevice headRightServo = scheduler->addServo(SERVO_HEAD_RIGHT_NAME,
                                                       SERVO_HEAD_RIGHT_PIN,
                                                       SERVO_HEAD_RIGHT_MIN_POSITION,
                                                       SERVO_HEAD_RIGHT_MAX_POSITION,
                                                       HEAD_RIGHT_PARK_POSITION);
    freeRam();
    schedulerDevice stockStepper = scheduler->addStepper_A4988(STEPPER_STOCK_NAME,
                                                       STEPPER_STOCK_STEP_PIN,
                                                       STEPPER_STOCK_DIR_PIN,
                                                       STEPPER_STOCK_ENABLE_PIN,
                                                       STEPPER_STOCK_HIGHEST_STEPPINGMODE,
                                                       STEPPER_STOCK_STEPMODECODES,
                                                       STEPPER_STOCK_MICROSTEPPINS,
                                                       STEPPER_STOCK_STEPS_PER_ROTATION);

    schedulerDevice tableStepper = scheduler->addStepper_A4988(STEPPER_TABLE_NAME,
                                                         STEPPER_TABLE_STEP_PIN,
                                                         STEPPER_TABLE_DIR_PIN,
                                                         STEPPER_TABLE_ENABLE_PIN,
                                                         STEPPER_TABLE_HIGHEST_STEPPINGMODE,
                                                         STEPPER_TABLE_STEPMODECODES,
                                                         STEPPER_TABLE_MICROSTEPPINS,
                                                         STEPPER_TABLE_STEPS_PER_ROTATION);
    
    schedulerDevice catStepper = scheduler->addStepper_TMC260(STEPPER_CAT_NAME,
                                                              STEPPER_CAT_STEP_PIN,
                                                              STEPPER_CAT_DIR_PIN,
                                                              STEPPER_CAT_ENABLE_PIN,
                                                              STEPPER_CAT_CHIPSELECT_PIN,
                                                              STEPPER_CAT_CURRENT,
                                                              STEPPER_CAT_RESISTOR,
                                                              STEPPER_CAT_STEPS_PER_ROTATION);

    freeRam();
    schedulerDevice tableDrive = scheduler->addDcController(TABLEDRIVE_NAME, TABLEDRIVE_PIN, TABLEDRIVE_ACTIV);
    freeRam();
    
    
    scheduler->getAllDevices();

    schedulerControlButton songEndButton = scheduler->addControlButton(SONG_END_BUTTON_NAME, SONG_END_PIN, SONG_END_ACTIV);
    schedulerControlButton songCancelButton = scheduler->addControlButton(SONG_CANCEL_BUTTON_NAME, SONG_CANCEL_PIN, SONG_CANCEL_ACTIV);
    
    scheduler->getAllControlButtons();
    
    
    
    for (;;) {
        
        // ============================================================================================================================
        // ============= moves ========================================================================================================
        // ============================================================================================================================
        
        // ########## scheduledTask addTaskWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration);
        // ########## scheduledTask addTask(float target, float velocity, float acceleration, float deceleration) {
        
        
        // ########## startByDate(unsigned long startTime)
        // ########## startByButton(unsigned char startButton, boolean startButtonState)
        // ########## startByTriggerpositionOf(unsigned char startTriggerDevice, unsigned char startTriggerTask, signed long startTriggerPosition)
        
        // ########## setDisposeAfterTask(unsigned char stopTriggerDevice, unsigned char stopTriggerTask)
        
        // ########## switchToNextTaskByDate(unsigned long startTime);
        // ########## switchToNextTaskByButton(unsigned char startButton, boolean startButtonState);
        // ########## switchToNextTaskByTriggerpositionOf(unsigned char startTriggerDevice, unsigned char startTriggerTask, signed long startTriggerPosition);
        
        // ########## stopByTimeout(unsigned long timeout, stoppingMode stopping)
        // ########## stopByButton(unsigned char stopButton, boolean stopButtonState, stoppingMode stopping)
        // ########## stopByTriggerpositionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping)
        
        //  stopping:  STOP_IMMEDIATELY, STOP_NORMAL
        
        // ============================================================================================================================
        // ============================================================================================================================
        // ============================================================================================================================
        
        
        
        
        Serial.println("................................. initialisation .................................");
        
        
        scheduledTask initCatStepper = scheduler->device[catStepper]->addTask(-400000, CAT_SPEED_VERY_HIGH, CAT_ACCEL_VERY_HIGH, CAT_ACCEL_VERY_HIGH);
        scheduler->device[catStepper]->task[initCatStepper]->startByDate(100);
        scheduler->device[catStepper]->task[initCatStepper]->stopByButton(CAT_PARK_BUTTON, HIGH, STOP_NORMAL);
        
        
        
        scheduler->reviewTasks();
        scheduler->getAllTasks();
        
        scheduler->run();
        
        scheduler->deleteAllTasks();
        scheduler->deleteAllActions();
        
        Serial.println("...................................... done ......................................");

        
        scheduler->device[catStepper]->currentPosition = 0;
        scheduler->device[tableStepper]->currentPosition = 0;
        
        if (digitalRead(NORMAL_CUTTING) == HIGH) {
            
            freeRam();
            
            //  move to start groove:
            scheduledTask driveToCuttingStartPosition = scheduler->device[catStepper]->addTask(CAT_CUTTING_START_POSITION, CAT_SPEED_HIGH, CAT_ACCEL_HIGH, CAT_ACCEL_HIGH);
            scheduler->device[catStepper]->task[driveToCuttingStartPosition]->startByDate(100);
            scheduler->device[catStepper]->task[driveToCuttingStartPosition]->stopByButton(CAT_END_BUTTON, HIGH, STOP_NORMAL);
            
            //  turn the table:
            scheduledTask turnTheTable = scheduler->device[tableStepper]->addTask(grooves_all * 360, turnTableSpeed, TABLE_STEP_ACCEL, TABLE_STEP_ACCEL);
            scheduler->device[tableStepper]->task[turnTheTable]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
//            scheduler->device[tableDrive]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
            
            //  lower head to record surface: start when reached start position of start groove
            scheduledTask lowerHeadLeftForCutting = scheduler->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
            scheduler->device[headLeftServo]->task[lowerHeadLeftForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
            
            //  lower head to record surface: start when reached start position of start groove
            scheduledTask lowerHeadRightForCutting = scheduler->device[headRightServo]->addTask(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
            scheduler->device[headRightServo]->task[lowerHeadRightForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
            scheduler->device[headRightServo]->task[lowerHeadRightForCutting]->stopDynamicallyBySensor(A5, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
            
            scheduledTask makeStartGroove = scheduler->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_startGroove, CAT_ACCEL_SLOW, CAT_ACCEL_SLOW);
            scheduler->device[catStepper]->task[makeStartGroove]->startAfterCompletionOf(headRightServo, lowerHeadRightForCutting);
            scheduler->device[catStepper]->task[makeStartGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeStartGroove, catSongStartPosition);
            
            scheduledTask makeMainGroove = scheduler->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_song, CAT_ACCEL_SLOW, CAT_ACCEL_SLOW);
            scheduler->device[catStepper]->task[makeMainGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition);
            
            scheduledTask makeEndGroove = scheduler->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_endGroove, CAT_ACCEL_NORMAL, CAT_ACCEL_NORMAL);
            
            scheduledTask liftHeadLeftAfterCutting = scheduler->device[headLeftServo]->addTask(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
            scheduler->device[headLeftServo]->task[liftHeadLeftAfterCutting]->startAfterCompletionOf(catStepper, makeEndGroove);
            
            scheduler->device[tableStepper]->task[turnTheTable]->stopAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting, STOP_NORMAL);
            
            scheduledTask liftHeadRightAfterCutting = scheduler->device[headRightServo]->addTask(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
            scheduler->device[headRightServo]->task[liftHeadRightAfterCutting]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
            
            scheduledTask driveToParkPosition = scheduler->device[catStepper]->addTask(CAT_PARK_POSITION, CAT_SPEED_HIGH, CAT_ACCEL_HIGH, CAT_ACCEL_HIGH);
            scheduler->device[catStepper]->task[driveToParkPosition]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
            scheduler->device[catStepper]->task[driveToParkPosition]->stopByButton(CAT_PARK_BUTTON, HIGH, STOP_NORMAL);
            
            
            scheduler->controlButton[songEndButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, makeEndGroove);
            
            scheduler->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeStartGroove, STOP_AND_SWITCH, driveToParkPosition);
            scheduler->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, driveToParkPosition);
            scheduler->controlButton[songCancelButton]->evokeTaskJump(catStepper, makeEndGroove, STOP);

            scheduler->getAllControlButtons();
            scheduler->getAllActions();
            
            // ============================================================================================================================
            // ============================================================================================================================
            // ============================================================================================================================
            
            
           scheduler->reviewTasks();
            
            scheduler->getAllTasks();
            
            freeRam();
            
            
            
             
            delay(2000);
            
            scheduler->run();
            
            scheduler->deleteAllTasks();
            scheduler->deleteAllActions();

        } else {
            
            //  move to start groove:
            scheduledTask driveToCuttingStartPositionMan = scheduler->device[catStepper]->addTask(CAT_CUTTING_START_POSITION, 4800, 3200, 3200);
            scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(100);
            scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(CAT_END_BUTTON, HIGH, STOP_NORMAL);
            
            scheduler->reviewTasks();
            scheduler->run();
            scheduler->deleteAllTasks();
            
            while (digitalRead(START_CUTTING_BUTTON) == HIGH) {
                if (digitalRead(CAT_FWD) == LOW) {
                    driveToCuttingStartPositionMan = scheduler->device[catStepper]->addTask(catCuttingEndPosition, 4800, 3200, 3200);
                    scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
                    scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(CAT_FWD, HIGH, STOP_NORMAL);
                    
                    scheduler->reviewTasks();
                    scheduler->run();
                    scheduler->deleteAllTasks();
                }
                if (digitalRead(CAT_RWD) == LOW) {
                    driveToCuttingStartPositionMan = scheduler->device[catStepper]->addTask(CAT_PARK_POSITION, 4800, 3200, 3200);
                    scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
                    scheduler->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(CAT_RWD, HIGH, STOP_NORMAL);
                    
                    scheduler->reviewTasks();
                    scheduler->run();
                    scheduler->deleteAllTasks();
                }
            }
            
            
            scheduler->reviewTasks();
            scheduler->run();
            scheduler->deleteAllTasks();
            
            
            
        }
    }
    
    delay(2000);
    
    
    Serial.println("delete scheduler");
    
    delete scheduler;
    scheduler = NULL;
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
    Serial.print("mm, max. record grooves: ");
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
    turnTableSpeed = RECORD_TURNS_PER_MINUTE / 60.0 * TABLE_DRIVE_RATIO * STEPPER_TABLE_STEPS_PER_ROTATION;
    
    
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

    Serial.println();
    Serial.println("################################################################################");
    Serial.println();
    Serial.println();
    
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
    
    calculateCuttingParameters();
    
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



//        unsigned char switchTableOn = scheduler->device[tableDrive]->addTask(TABLEDRIVE_ON, 0, 0, 0);
//        scheduler->device[tableDrive]->setStartAfterCompletion(switchTableOn, headLeftServo, liftHeadFirstLeft);
//        unsigned char switchTableOff = scheduler->device[tableDrive]->addTask(TABLEDRIVE_ACTIV, 0, 0, 0);
//        scheduler->device[tableDrive]->startByDate(switchTableOff, 10000);

//        unsigned char riseHead = scheduler->device[headLeftServo]->addTask(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//        scheduler->device[headLeftServo]->setStartAfterCompletion(riseHead, headLeftServo, lowerHeadFirstLeft);
//        scheduler->device[headLeftServo]->stopByTimeout(riseHead, 10000, true);
//
//        unsigned char lowerHead = scheduler->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//        scheduler->device[headLeftServo]->setStartAfterCompletion(lowerHead, headLeftServo, riseHead);
//        scheduler->device[headLeftServo]->stopDynamicallyBySensor(lowerHead, A5, 1000, 200, 0);
//        scheduler->device[headLeftServo]->stopByTimeout(lowerHead, 10000, true);



//  unsigned char liftHeadFirstRight = scheduler->device[headRightServo]->addTask(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
//  scheduler->device[headRightServo]->startByDate(liftHeadFirstRight, 100);


// here may be a problem: preperation of next move before 'currentPosition' set to 0


/*
 //  supply a new record from stock
 unsigned char supplyNewRecord = scheduler->device[stockStepper]->addTask(36000L, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, STOCK_SUPPLY_RECORD_ACCEL);
 //        scheduler->device[stockStepper]->startByDate(supplyNewRecord, 100);
 scheduler->device[stockStepper]->startByButton(supplyNewRecord, CAT_FWD, LOW);
 scheduler->device[stockStepper]->stopByButton(supplyNewRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);
 
 //  lift grappler: start soon
 unsigned char liftFromParkPosition = scheduler->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
 //        scheduler->device[liftServo]->startByDate(liftFromParkPosition, 100);
 //        scheduler->device[turnServo]->startByButton(turnToStock, RECORD_AVAILABLE_BUTTON, HIGH);
 freeRam();
 
 //  turn grappler to stock: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnToStock = scheduler->device[turnServo]->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
 scheduler->device[turnServo]->startByTriggerpositionOf(turnToStock, liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
 unsigned char lowerToStock = scheduler->device[liftServo]->addTask(LIFT_STOCK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
 //        scheduler->device[liftServo]->startByDate(lowerToStock, 3000);
 scheduler->device[liftServo]->startByTriggerpositionOf(lowerToStock, turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
 freeRam();
 
 //         //  grip new record: start when grappler reached stock (LIFT_STOCK_POSITION)
 //         unsigned char gripNewRecord = scheduler->device[vacuumSolenoid]->addTask(SOLENOID_DUTYCYCLE, SOLENOID_FREQUENCY, 0, 500);
 //         scheduler->device[vacuumSolenoid]->startByTriggerpositionOf(gripNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
 //         freeRam();
 
 //  lift the new record: start with startDelay after stock was reached (LIFT_STOCK_POSITION)
 unsigned char liftNewRecord = scheduler->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, 2666);
 scheduler->device[liftServo]->startByTriggerpositionOf(liftNewRecord, liftServo, lowerToStock, LIFT_STOCK_POSITION);
 freeRam();
 
 //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnRecordToTable = scheduler->device[turnServo]->addTask(TURN_TABLE_POSITION, TURN_SPEED_SLOW, TURN_ACCEL_SLOW, NO_START_DELAY);
 scheduler->device[turnServo]->startByTriggerpositionOf(turnRecordToTable, liftServo, liftNewRecord, LIFT_FROM_STOCK_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
 unsigned char lowerRecordToTable = scheduler->device[liftServo]->addTask(LIFT_TABLE_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
 scheduler->device[liftServo]->startByTriggerpositionOf(lowerRecordToTable, turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
 //        scheduler->device[vacuumSolenoid]->stopByTriggerpositionOf(gripNewRecord, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION, 0);
 freeRam();
 
 //  lift for going to park position: start with startDelay after turn table was reached (LIFT_TABLE_POSITION)
 unsigned char liftForParkPosition = scheduler->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, 500);
 scheduler->device[liftServo]->startByTriggerpositionOf(liftForParkPosition, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
 freeRam();
 
 //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
 unsigned char turnToParkPosition = scheduler->device[turnServo]->addTask(TURN_PARK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
 scheduler->device[turnServo]->startByTriggerpositionOf(turnToParkPosition, liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
 freeRam();
 
 //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
 unsigned char lowerForParkPosition = scheduler->device[liftServo]->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
 scheduler->device[liftServo]->startByTriggerpositionOf(lowerForParkPosition, turnServo, turnToParkPosition, TURN_TO_PARK_TRIGGER_LIFT);
 freeRam();
 
 scheduledTask turnTheTable = scheduler->device[tableDrive]->addTask(PWM_FULL_POWER, PWM_FREQUENCY_LOW, PWM_NO_RAMPING, PWM_NO_RAMPING);
 scheduler->device[tableDrive]->task[turnTheTable]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
 //            scheduler->device[tableDrive]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
 

 
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

