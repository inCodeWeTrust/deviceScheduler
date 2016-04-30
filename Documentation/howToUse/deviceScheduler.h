
#ifndef __deviceScheduler__deviceScheduler__
#define __deviceScheduler__deviceScheduler__






//  ################## Hardware #############################################################################


#define STEPPER_TABLE_NAME                  "tableStepper"

#define STEPPER_TABLE_STEP_PIN              22
#define STEPPER_TABLE_DIR_PIN               24
#define STEPPER_TABLE_ENABLE_PIN            26
#define STEPPER_TABLE_MICROSTEPPINS         "28, 30, 32"
#define STEPPER_TABLE_HIGHEST_STEPPINGMODE  4
#define STEPPER_TABLE_STEPMODECODES         "0x00, 0x01, 0x02, 0x03, 0x07"
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



//  ################## BUTTONS AND SENSORS #########################################################################


#define SONG_END_BUTTON_NAME                "songEndButton"
#define SONG_END_PIN                        50
#define SONG_END_ACTIV                      LOW

#define SONG_CANCEL_BUTTON_NAME             "songCancelButton"
#define SONG_CANCEL_PIN                     51
#define SONG_CANCEL_ACTIV                   LOW




#define CAT_PARK_BUTTON                     52
#define CAT_END_BUTTON                      53

#define HEAD_INCLINATION_SENSOR             A5
#define HEAD_INCLINATION_INIT_STOP          600
#define HEAD_INCLINATION_TARGET             460
#define HEAD_INCLINATION_STOP_PERFORM       0.6




//  ################## MOVE DATA #############################################################################


#define HEAD_LEFT_PARK_POSITION         2100
#define HEAD_LEFT_CUT_POSITION          1460
#define HEAD_LEFT_MID_POSITION          1520
#define HEAD_LEFT_TOP_POSITION          2100

#define HEAD_RIGHT_PARK_POSITION        900
#define HEAD_RIGHT_CUT_POSITION         2100
#define HEAD_RIGHT_MID_POSITION         1520
#define HEAD_RIGHT_TOP_POSITION         900


#define TABLE_STEP_ACCEL                200

#define CAT_SPEED_HIGH                  4800
#define CAT_SPEED_VERY_HIGH             5600
#define CAT_ACCEL_SLOW                  800
#define CAT_ACCEL_NORMAL                1800
#define CAT_ACCEL_HIGH                  3200
#define CAT_ACCEL_VERY_HIGH             4800

#define CAT_PARK_POSITION               0
#define CAT_CUTTING_START_POSITION      10000



//  ################## PROGRAMMATICAL DEFINITIONS AND CONSTANTS #############################################################################

/// \fn typedef unsigned char schedulerDevice, scheduledTask, schedulerControlButton
/// \brief using a special variable types will make the coding easier!
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






#endif /* defined(__deviceScheduler__deviceScheduler__) */

