
#ifndef __deviceScheduler__deviceScheduler__
#define __deviceScheduler__deviceScheduler__





#define SONGGROOVE_PITCH                 0.2
#define STARTGROOVE_PITCH                  1
#define STARTGROOVE_RANGE                   4
#define ENDGROOVE_PITCH                     4.8
#define ENDGROOVE_RANGE_MIN                 10
#define CUTTING_RANGE                       38
#define PLAYTIME_MINUTES                    3

#define ADJUSTGROOVEPITCH_TO_PLAYTIME      false




//  ################## Hardware #############################################################################

#define STEPPER_STOCK_NAME                  "stockStepper"

#define STEPPER_STOCK_STEP_PIN              22
#define STEPPER_STOCK_DIR_PIN               24
#define STEPPER_STOCK_ENABLE_PIN            26
#define STEPPER_STOCK_MICROSTEPPINS         "28, 30, 32"

#define STEPPER_STOCK_HIGHEST_STEPPINGMODE  4        
#define STEPPER_STOCK_STEPMODECODES         "0x00, 0x01, 0x02, 0x03, 0x07"
#define STEPPER_STOCK_STEPS_PER_ROTATION    48
#define STEPPER_STOCK_PPS_MAX               800
#define STEPPER_STOCK_ACC_MAX               2000



#define STEPPER_TABLE_NAME                  "tableStepper"

#define STEPPER_TABLE_STEP_PIN              22
#define STEPPER_TABLE_DIR_PIN               24
#define STEPPER_TABLE_ENABLE_PIN            26
#define STEPPER_TABLE_MICROSTEPPINS         "28, 30, 32"
#define STEPPER_TABLE_HIGHEST_STEPPINGMODE  4
#define STEPPER_TABLE_STEPMODECODES         "0x00, 0x01, 0x02, 0x03, 0x07"
//#define STEPPER_TABLE_STEP_PIN              23
//#define STEPPER_TABLE_DIR_PIN               25
//#define STEPPER_TABLE_ENABLE_PIN            27
//#define STEPPER_TABLE_MICROSTEPPINS         "29, 31, 33"
//#define STEPPER_TABLE_HIGHEST_STEPPINGMODE  5
//#define STEPPER_TABLE_STEPMODECODES         "0x00, 0x01, 0x02, 0x03, 0x04, 0x05"
#define STEPPER_TABLE_STEPS_PER_ROTATION    200
#define STEPPER_TABLE_PPS_MAX               800
#define STEPPER_TABLE_ACC_MAX               2000



#define STEPPER_CAT_NAME                    "catStepper"

#define STEPPER_CAT_DIR_PIN                 9
#define STEPPER_CAT_STEP_PIN                8
#define STEPPER_CAT_ENABLE_PIN              7
#define STEPPER_CAT_CHIPSELECT_PIN          10
#define STEPPER_CAT_CURRENT                 1600
#define STEPPER_CAT_STEPS_PER_ROTATION      200
#define STEPPER_CAT_PPS_MAX                 1000
#define STEPPER_CAT_ACC_MAX                 4000



#define SERVO_LIFT_NAME                     "liftServo"

#define SERVO_LIFT_PIN                      2
#define SERVO_LIFT_MIN_POSITION             150
#define SERVO_LIFT_MAX_POSITION             600



#define SERVO_TURN_NAME                     "turnServo"

#define SERVO_TURN_PIN                      3
#define SERVO_TURN_MIN_POSITION             150
#define SERVO_TURN_MAX_POSITION             600



#define SERVO_HEAD_LEFT_NAME                "headLeftServo"

#define SERVO_HEAD_LEFT_PIN                 4
#define SERVO_HEAD_LEFT_MIN_POSITION        900
#define SERVO_HEAD_LEFT_MAX_POSITION        2100



#define SERVO_HEAD_RIGHT_NAME               "headRightServo"

#define SERVO_HEAD_RIGHT_PIN                5
#define SERVO_HEAD_RIGHT_MIN_POSITION       900
#define SERVO_HEAD_RIGHT_MAX_POSITION       2100



#define TABLEDRIVE_NAME                     "tableDrive"

#define TABLEDRIVE_PIN                      40
#define TABLEDRIVE_ACTIV                    HIGH


#define VACUUMSOLENOID_NAME                 "vacuumSolenoid"

#define VACUUMSOLENOID_PIN                  9
#define VACUUMSOLENOID_ACTIV                LOW


// TMC260 sensing resistor value in mOhm
#define R_SENSE                             150

//  ################## BUTTONS AND SENSORS #########################################################################


#define SONG_ENDBUTTON_NAME                "songEndButton"
#define SONG_ENDBUTTON_PIN                        50
#define SONG_ENDBUTTON_ACTIV                      LOW
#define SONG_ENDBUTTON_PULLUP                     true

#define SONG_CANCELBUTTON_NAME             "songCancelButton"
#define SONG_CANCELBUTTON_PIN                     51
#define SONG_CANCELBUTTON_ACTIV                   LOW
#define SONG_CANCELBUTTON_PULLUP                  true

#define CAT_PARKBUTTON_NAME                "bridgeParkButton"
#define CAT_PARKBUTTON_PIN                     52
#define CAT_PARKBUTTON_ACTIV               HIGH
#define CAT_PARKBUTTON_PULLUP                     true

#define CAT_ENDBUTTON_NAME                 "bridgeEndButton"
#define CAT_ENDBUTTON_PIN                      53
#define CAT_ENDBUTTON_ACTIV                HIGH
#define CAT_ENDBUTTON_PULLUP                     true


#define NORMAL_CUTTING                      A4
#define CAT_FWD                             A3
#define CAT_RWD                             A2
#define START_CUTTING_BUTTON                A1
#define STOP_CUTTING_BUTTON                 A0


#define HEAD_INCLINATION_SENSOR             A5

//#define START_BUTTON                        A2
//#define LOADING_BUTTON                      A1
//#define RECORD_AVAILABLE_BUTTON             A0
//#define CAT_PARKBUTTON_PIN                     A3

#define I_AM_LATE_LED                       12




//  ################## CUTTING TOOL #########################################################################

#define SPIN_PITCH_M6                       1
//#define CAT_DRIVE_RATIO                     22.0 / 60.0 * 22.0 / 60.0
#define CAT_DRIVE_RATIO                     36.0 / 60.0 * 22.0 / 60.0
#define RECORD_TURNS_PER_MINUTE             45

#define TABLE_DRIVE_RATIO                   122.0 / 28.0


//  ################## MOVE DATA #############################################################################


#define LIFT_SPEED_FAST                 1600
#define LIFT_SPEED_SLOW                 600
#define LIFT_SPEED_VERY_SLOW            200
#define LIFT_ACCEL_FAST                 1600
#define LIFT_ACCEL_SLOW                 400
#define LIFT_ACCEL_VERY_SLOW            200

#define LIFT_UP_POSITION                840
#define LIFT_FROM_STOCK_TRIGGER_TURN    1400
#define LIFT_UP_TRIGGER_TURN            1240
#define LIFT_TABLE_POSITION             1460
#define LIFT_STOCK_POSITION             1480
#define LIFT_PARK_POSITION              1484


#define TURN_SPEED_FAST                 320
#define TURN_SPEED_SLOW                 180
#define TURN_ACCEL_FAST                 280
#define TURN_ACCEL_SLOW                 150

#define TURN_TABLE_POSITION             1800
#define TURN_TO_PARK_TRIGGER_LIFT       1600
#define TURN_TO_TABLE_TRIGGER_LIFT      1600
#define TURN_PARK_POSITION              1300
#define TURN_TO_STOCK_TRIGGER_LIFT      900
#define TURN_STOCK_POSITION             800


#define HEAD_LEFT_PARK_POSITION         2100
//#define HEAD_LEFT_CUT_POSITION          1210
//#define HEAD_LEFT_CUT_POSITION          1520
#define HEAD_LEFT_CUT_POSITION          1460
//#define HEAD_LEFT_CUT_POSITION          1610
//#define HEAD_LEFT_CUT_POSITION          1810
#define HEAD_LEFT_MID_POSITION          1520
#define HEAD_LEFT_TOP_POSITION          2100

#define HEAD_RIGHT_PARK_POSITION        900
//#define HEAD_RIGHT_CUT_POSITION         2000
//#define HEAD_RIGHT_CUT_POSITION         1520
//#define HEAD_RIGHT_CUT_POSITION         1460
//#define HEAD_RIGHT_CUT_POSITION         1430
#define HEAD_RIGHT_CUT_POSITION         2100
#define HEAD_RIGHT_MID_POSITION         1520
#define HEAD_RIGHT_TOP_POSITION         900


#define STOCK_SUPPLY_RECORD_SPEED       640
#define STOCK_SUPPLY_RECORD_ACCEL       320

#define STOCK_LOADING_SPEED             520
#define STOCK_LOADING_ACCEL             260

#define TABLE_STEP_ACCEL                200

#define CAT_SPEED_HIGH                  4800
#define CAT_SPEED_VERY_HIGH             5600
#define CAT_ACCEL_SLOW                  800
#define CAT_ACCEL_NORMAL                1800
#define CAT_ACCEL_HIGH                  3200
#define CAT_ACCEL_VERY_HIGH             4800

#define CAT_PARK_POSITION               0
#define CAT_CUTTING_START_POSITION      88600

#define VACUUMSOLENOID_FREQUENCY           20
#define VACUUMSOLENOID_DUTYCYCLE           0.4




#define NO_START_DELAY                      0
#define PWM_FULL_POWER                  1
#define PWM_FREQUENCY_LOW               1
#define PWM_NO_RAMPING                  0


//  ################## PROGRAMMATICAL DEFINITIONS AND CONSTANTS #############################################################################

typedef unsigned char schedulerDevice, scheduledTask, schedulerControlButton;

//  device type
enum deviceType {
    SERVODEVICE,
    STEPPERDEVICE,
    DCCONTROLLERDEVICE
};


// start/stopEvent:
enum event {
    NONE,
    DATE,
    BUTTON,
    FOLLOW,
    POSITION,
    CONTROLBUTTON
};


//  device state:
enum deviceState {
    SLEEPING,
    MOVING,
    MOVE_DONE,
    PENDING_MOVES
};


//  device action:
enum deviceAction {
    START,
    STOP_AND_SWITCH,
    STOP,
    STOP_SHARP_AND_SWITCH,
    STOP_SHARP
};

//  stop mode:
enum stoppingMode {
    STOP_IMMEDIATELY,
    STOP_NORMAL,
    STOP_DYNAMIC
};


//  appriximation mode:
enum approximationMode {
    SKIP_APPROXIMATION_IMMEDIATELY = 0x00,
    SKIP_APPROXIMATION_VERY_FAST = 0x01,
    SKIP_APPROXIMATION_FAST = 0x20,
    SKIP_APPROXIMATION_PRECISE = 0x40,
    SKIP_APPROXIMATION_VERY_PRECISE = 0x60,
    SKIP_APPROXIMATION_NEVER = 0xFF
};





#define STEPPINGPERIOD_TO_KICK_UP           150        //150
#define STEPPINGPERIOD_TO_KICK_DOWN         300        //300



#endif /* defined(__deviceScheduler__deviceScheduler__) */

