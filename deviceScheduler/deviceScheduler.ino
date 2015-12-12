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

        
        // ########## setStartDateForMove(unsigned char moveIndex, unsigned long startTime)
        // ########## setStartButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState)
        // ########## setStartEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition)
        
        // ########## setDisposeMoveAfterMove(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove)
        
        // ########## setSwitchDateForMove(unsigned char moveIndex, unsigned long startTime);
        // ########## setSwitchButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState);
        // ########## setSwitchEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
        
        // ########## setTimeoutForMove(unsigned char moveIndex, unsigned long timeout, unsigned char stopMode)
        // ########## setStopButtonForMove(unsigned char moveIndex, unsigned char stopButton, boolean stopButtonState, unsigned char stopMode)
        // ########## setStopEventForMove(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, unsigned char stopMode)
        
        //  stopModes:  STOP_SHARPLY, STOP_NORMAL, STOP_DYNAMIC

        // ============================================================================================================================
        // ============================================================================================================================
        // ============================================================================================================================


        /*
        Serial.println("................................. initialisation .................................");
        unsigned char initCatStepper = scheduler->device[catStepper]->addMove(-400000, 6400, 3200.0, 3200.0);
        scheduler->device[catStepper]->setStartDateForMove(initCatStepper, 100);
        scheduler->device[catStepper]->setStopButtonForMove(initCatStepper, CAT_PARK_BUTTON, HIGH, 0);

        
        scheduler->getAllMoves();
        scheduler->reviewMoves();
        scheduler->getAllMoves();
        
        scheduler->runTheLoop();
        
        scheduler->deleteAllMoves();
        
        Serial.println("...................................... done ......................................");
        
        
        
        scheduler->device[catStepper]->currentPosition = 0;
        */
        
        
        
        freeRam();

        
        //  lift head before doing anything else: start now
        unsigned char liftHeadFirstLeft = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
        scheduler->device[headLeftServo]->setStartDateForMove(liftHeadFirstLeft, 100);
        
        unsigned char lowerHeadFirstLeft = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_SLOW);
        scheduler->device[headLeftServo]->setStartAfterCompletion(lowerHeadFirstLeft, headLeftServo, liftHeadFirstLeft);
        
//        unsigned char switchTableOn = scheduler->device[tableDrive]->addMove(TABLEDRIVE_ON, 0, 0, 0);
//        scheduler->device[tableDrive]->setStartAfterCompletion(switchTableOn, headLeftServo, liftHeadFirstLeft);
//        unsigned char switchTableOff = scheduler->device[tableDrive]->addMove(TABLEDRIVE_OFF, 0, 0, 0);
//        scheduler->device[tableDrive]->setStartDateForMove(switchTableOff, 10000);
        
        unsigned char riseHead = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
        scheduler->device[headLeftServo]->setStartAfterCompletion(riseHead, headLeftServo, lowerHeadFirstLeft);
        scheduler->device[headLeftServo]->setStopDynamicalForMove(riseHead, A5, 1000, 200, 0);
        scheduler->device[headLeftServo]->setTimeoutForMove(riseHead, 10000, true);

        unsigned char lowerHead = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
        scheduler->device[headLeftServo]->setStartAfterCompletion(lowerHead, headLeftServo, riseHead);
        scheduler->device[headLeftServo]->setStopDynamicalForMove(lowerHead, A5, 1000, 200, 0);
        scheduler->device[headLeftServo]->setTimeoutForMove(lowerHead, 10000, true);

        
        
        //  unsigned char liftHeadFirstRight = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
        //  scheduler->device[headRightServo]->setStartDateForMove(liftHeadFirstRight, 100);
        
        
        /*          my brandnew cutting schdule
        unsigned char driveToCuttingStartPosition = scheduler->device[catStepper]->addMove(CAT_CUTTING_START_POSITION, 6400, 3200, 3200);
        scheduler->device[catStepper]->setStartDateForMove(driveToCuttingStartPosition, 100);
        scheduler->device[catStepper]->setStopButtonForMove(driveToCuttingStartPosition, CAT_END_BUTTON, HIGH, 0);
        
        unsigned char makeStartGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, CAT_MOTOR_DEGREES_PER_SECOND_START, 3200, 3200);
        scheduler->device[catStepper]->setStartAfterCompletion(makeStartGroove, catStepper, driveToCuttingStartPosition);
        
        unsigned char makeMainGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, CAT_MOTOR_DEGREES_PER_SECOND, 600, 600);
        scheduler->device[catStepper]->setSwitchEventForMove(makeStartGroove, catStepper, makeStartGroove, CAT_SONG_START_POSITION);
        
        unsigned char makeEndGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, CAT_MOTOR_DEGREES_PER_SECOND_END, 800, 800);
        scheduler->device[catStepper]->setSwitchEventForMove(makeMainGroove, catStepper, makeMainGroove, CAT_SONG_END_POSITION);
        */
        
        // here may be a problem: preperation of next move before 'currentPosition' set to 0

        
        /*
        //  supply a new record from stock
        unsigned char supplyNewRecord = scheduler->device[stockStepper]->addMove(36000L, STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL, STOCK_SUPPLY_RECORD_ACCEL);
         //        scheduler->device[stockStepper]->setStartDateForMove(supplyNewRecord, 100);
        scheduler->device[stockStepper]->setStartButtonForMove(supplyNewRecord, START_BUTTON, LOW);
        scheduler->device[stockStepper]->setStopButtonForMove(supplyNewRecord, RECORD_AVAILABLE_BUTTON, HIGH, 0);

        //  lift grappler: start soon
        unsigned char liftFromParkPosition = scheduler->device[liftServo]->addMove(LIFT_UP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
         //        scheduler->device[liftServo]->setStartDateForMove(liftFromParkPosition, 100);
         //        scheduler->device[turnServo]->setStartButtonForMove(turnToStock, RECORD_AVAILABLE_BUTTON, HIGH);
        freeRam();
        
        //  turn grappler to stock: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
        unsigned char turnToStock = scheduler->device[turnServo]->addMove(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST, NO_START_DELAY);
        scheduler->device[turnServo]->setStartEventForMove(turnToStock, liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
        freeRam();
        
        //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
        unsigned char lowerToStock = scheduler->device[liftServo]->addMove(LIFT_STOCK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, NO_START_DELAY);
         //        scheduler->device[liftServo]->setStartDateForMove(lowerToStock, 3000);
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
        
        */
        /*
        //  move to start groove: start when grappler is in park position
        //        unsigned char moveToStartGroove = scheduler->device[catStepper]->addMove(CAT_GROOVE_START_POSITION, 6400, 3200.0, 3200.0);
        unsigned char moveToStartGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, 2400, 3200.0, 3200.0);
        scheduler->device[catStepper]->setStartButtonForMove(moveToStartGroove, START_BUTTON, LOW);
        scheduler->device[catStepper]->setStopButtonForMove(moveToStartGroove, STOP_BUTTON, LOW, 0);
        //            scheduler->device[catStepper]->setStartDateForMove(moveToStartGroove, 2000);
        //        scheduler->device[catStepper]->setStartEventForMove(moveToStartGroove, liftServo, lowerRecordToTable, LIFT_TABLE_POSITION);
    
        //  lower head to record surface: start when reached start position of start groove
        unsigned char lowerHeadForCuttingLeft = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
        unsigned char lowerHeadForCuttingRight = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
        //        scheduler->device[headLeftServo]->setStartEventForMove(lowerHeadForCuttingLeft, catStepper, moveToStartGroove, CAT_GROOVE_START_POSITION - 2);
        //        scheduler->device[headRightServo]->setStartEventForMove(lowerHeadForCuttingRight, catStepper, moveToStartGroove, CAT_GROOVE_START_POSITION - 2);
        scheduler->device[headLeftServo]->setStartButtonForMove(lowerHeadForCuttingLeft, START_CUTTING_BUTTON, LOW);
        scheduler->device[headRightServo]->setStartButtonForMove(lowerHeadForCuttingRight, START_CUTTING_BUTTON, LOW);
        
        unsigned char makeMainGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, 640, 600, 600);
        scheduler->device[catStepper]->setStartButtonForMove(makeMainGroove, START_CUTTING_BUTTON, LOW);
        scheduler->device[catStepper]->setStopButtonForMove(makeMainGroove, STOP_CUTTING_BUTTON, LOW, 0);

        
//        //  make the groves: start when cuttinghead is in cutting position
//        unsigned char makeStartGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, 2400, 1200.0, 1200.0);
//        scheduler->device[catStepper]->setStartEventForMove(makeStartGroove, headLeftServo, lowerHeadForCuttingLeft, HEAD_LEFT_CUT_POSITION);
//        scheduler->device[catStepper]->setSwitchEventForMove(makeStartGroove, catStepper, makeStartGroove, CAT_SONG_START_POSITION);
//        unsigned char makeMainGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, 420, 600, 600);
//        scheduler->device[catStepper]->setSwitchEventForMove(makeMainGroove, catStepper, makeMainGroove, CAT_SONG_END_POSITION);
//        unsigned char makeEndGroove = scheduler->device[catStepper]->addMove(CAT_CUTTING_END_POSITION, 2400, 800, 800);
        
        //  lift head after cutting: start, when end groove is finished
        unsigned char liftHeadAfterCuttingLeft = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
        unsigned char liftHeadAfterCuttingRight = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
//        scheduler->device[headLeftServo]->setStartEventForMove(liftHeadAfterCuttingLeft, catStepper, makeEndGroove, CAT_SONG_END_POSITION - 2);
//        scheduler->device[headRightServo]->setStartEventForMove(liftHeadAfterCuttingRight, catStepper, makeEndGroove, CAT_SONG_END_POSITION - 2);
        scheduler->device[headLeftServo]->setStartButtonForMove(liftHeadAfterCuttingLeft, STOP_CUTTING_BUTTON, LOW);
        scheduler->device[headRightServo]->setStartButtonForMove(liftHeadAfterCuttingRight, STOP_CUTTING_BUTTON, LOW);
        
        //  go back to park position: start when cutting head is lifted up
//        scheduler->device[catStepper]->setStopEventForMove(makeMainGroove, headLeftServo, liftHeadAfterCuttingLeft, HEAD_LEFT_TOP_POSITION, 0);
        scheduler->device[catStepper]->setDisposeFollowingMoveForMove(makeMainGroove);
//        scheduler->device[catStepper]->setSwitchButtonForMove(makeMainGroove, STOP_CUTTING_BUTTON, LOW);
        unsigned char returnCatToParkPosition = scheduler->device[catStepper]->addMove(CAT_PARK_POSITION, 6400, 3200.0, 3200.0);
        
        //  lower head to record surface: start when reached start position of start groove
        unsigned char lowerHeadForParkingLeft = scheduler->device[headLeftServo]->addMove(HEAD_LEFT_PARK_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
        unsigned char lowerHeadForParkingRight = scheduler->device[headRightServo]->addMove(HEAD_RIGHT_PARK_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW, NO_START_DELAY);
        scheduler->device[headLeftServo]->setStartEventForMove(lowerHeadForParkingLeft, catStepper, returnCatToParkPosition, CAT_PARK_POSITION + 200);
        scheduler->device[headRightServo]->setStartEventForMove(lowerHeadForParkingRight, catStepper, returnCatToParkPosition, CAT_PARK_POSITION + 200);
*/
        
        
        
        // ============================================================================================================================
        // ============================================================================================================================
        // ============================================================================================================================
        
        
        scheduler->getAllDevices();
        
        scheduler->getAllMoves();
        
        scheduler->reviewMoves();
        
        scheduler->getAllMoves();
        
        freeRam();
        
        
        
        Serial.print("*** Inputs: START_BUTTON: ");
        Serial.print(digitalRead(START_BUTTON));
        Serial.print(", STOP_BUTTON: ");
        Serial.print(digitalRead(STOP_BUTTON));
        Serial.print(", START_CUTTING_BUTTON: ");
        Serial.print(digitalRead(START_CUTTING_BUTTON));
        Serial.print(", STOP_CUTTING_BUTTON: ");
        Serial.print(digitalRead(STOP_CUTTING_BUTTON));
        Serial.print(", CAT_PARK_BUTTON: ");
        Serial.println(digitalRead(CAT_PARK_BUTTON));
        
        
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
//    pinMode(START_BUTTON, INPUT_PULLUP);
//    pinMode(LOADING_BUTTON, INPUT_PULLUP);
//    pinMode(RECORD_AVAILABLE_BUTTON, INPUT_PULLUP);
//    pinMode(CAT_PARK_BUTTON, INPUT_PULLUP);

    pinMode(START_BUTTON, INPUT_PULLUP);
    pinMode(STOP_BUTTON, INPUT_PULLUP);
    pinMode(START_CUTTING_BUTTON, INPUT_PULLUP);
    pinMode(STOP_CUTTING_BUTTON, INPUT_PULLUP);
    
    pinMode(CAT_PARK_BUTTON, INPUT_PULLUP);
    pinMode(CAT_END_BUTTON, INPUT_PULLUP);

    pinMode(I_AM_LATE_LED, OUTPUT);
    
    Serial.begin(115200);
    Serial.println(), Serial.println(), Serial.println();
    Serial.println(), Serial.println(), Serial.println();

//    pinMode(40, OUTPUT);
//    digitalWrite(40, HIGH);
//    delay(10000);
//    digitalWrite(40, LOW);
//    pinMode(40, INPUT);
    
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
    Serial.print("Estimation free Ram: ");
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

