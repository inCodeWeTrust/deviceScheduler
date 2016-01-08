///
/// @file		motorController.h
/// @brief		Header
/// @details	<#details#>
/// @n
/// @n @b		Project motorController
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Little Abakus
/// @author		Little Abakus
///
/// @date		17.09.14 03:23
/// @version	<#version#>
///
/// @copyright	(c) Little Abakus, 2014
/// @copyright	<#license#>
///
/// @see		ReadMe.txt for references
///


#ifndef DEVICESCHEDULER
#define DEVICESCHEDULER 1



//  device type
enum deviceType {
    SERVODEVICE,
    STEPPERDEVICE,
    SWITCHINGDEVICE
};

//#define SERVODEVICE                     0x01
//#define STEPPERDEVICE                   0x02
//#define SWITCHINGDEVICE                 0x03
//#define SOLENOIDDEVICE                  0x04



// start/stopEvent:
#define NONE                            0x00
#define DATE                            0x01
#define BUTTON                          0x02
#define FOLLOW                          0x04
#define POSITION                        0x08
#define SWITCH                          0x10

//  device state:
#define SLEEPING                        0x00
#define MOVING                          0x01
#define MOVE_DONE                       0x02
#define PENDING_MOVES	                0x04




//  stopModes:
#define STOP_SHARPLY                    0x00
#define STOP_VERY_FAST                  0x01
#define STOP_FAST                       0x40
#define STOP_PRECISE                    0x80
#define STOP_VERY_PRECISE               0xC0
#define STOP_NEVER                      0xFF




#define STEPPINGPERIOD_TO_KICK_UP           150        //150
#define STEPPINGPERIOD_TO_KICK_DOWN         300        //300


#define RECORD_GROOVE_PITCH                 0.2
#define START_GROOVE_PITCH                  1
#define STARTGROOVE_WIDTH                   4
#define END_GROOVE_PITCH                    5
#define ENDGROOVE_WIDTH_MIN                 10
#define CUTTING_RANGE                       40
#define PLAYTIME_MINUTES                    3
#define DIAMETER_SONG_START                 166
#define DIAMETER_SONG_END                   116






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



#define STEPPER_CAT_NAME                    "catStepper"

#define STEPPER_CAT_STEP_PIN                23
#define STEPPER_CAT_DIR_PIN                 25
#define STEPPER_CAT_ENABLE_PIN              27
#define STEPPER_CAT_MICROSTEPPINS           "29, 31, 33"

#define STEPPER_CAT_HIGHEST_STEPPINGMODE    4
#define STEPPER_CAT_STEPMODECODES           "0x00, 0x01, 0x02, 0x03, 0x07"
#define STEPPER_CAT_STEPS_PER_ROTATION      400
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
#define SERVO_HEAD_LEFT_MIN_POSITION        150
#define SERVO_HEAD_LEFT_MAX_POSITION        600



#define SERVO_HEAD_RIGHT_NAME               "headRightServo"

#define SERVO_HEAD_RIGHT_PIN                5
#define SERVO_HEAD_RIGHT_MIN_POSITION       150
#define SERVO_HEAD_RIGHT_MAX_POSITION       600



#define TABLEDRIVE_NAME                     "tableDrive"
#define TABLEDRIVE_PIN                      40
#define TABLEDRIVE_READY_PIN                41
#define TABLEDRIVE_ON                       HIGH
#define TABLEDRIVE_OFF                      LOW


#define SOLENOID_VACUUM_NAME                "vacuumSolenoid"
#define SOLENOID_VACUUM_PIN                 9
#define SOLENOID_ON                         HIGH
#define SOLENOID_OFF                        LOW



#define NORMAL_CUTTING                      A4
#define CAT_FWD                             A3
#define CAT_RWD                             A2
#define START_CUTTING_BUTTON                A1
#define STOP_CUTTING_BUTTON                 A0

#define CAT_PARK_BUTTON                     52
#define CAT_END_BUTTON                      53

#define HEAD_DISTANCE_SENSOR

//#define START_BUTTON                        A2
//#define LOADING_BUTTON                      A1
//#define RECORD_AVAILABLE_BUTTON             A0
//#define CAT_PARK_BUTTON                     A3

#define I_AM_LATE_LED                       12
#define HEADDISTANCE_SENSE_PIN              A5

#define PLAYTIME_MINUTES                    3


//  ################## MOVE DATA #############################################################################

#define SPIN_PITCH_M6                       1
#define CAT_DRIVE_RATIO                     (float) 36.0 / 60.0 * 22.0 / 60.0
#define RECORD_TURNS_PER_MINUTE             45

#define MAX_PLAY_RANGE                      CUTTING_RANGE - STARTGROOVE_WIDTH - ENDGROOVE_WIDTH_MIN
#define MAX_RECORDGROOVES                   (MAX_PLAY_RANGE) / RECORD_GROOVE_PITCH
#define MAX_PLAY_TIME_MINUTES               MAX_RECORDGROOVES / RECORD_TURNS_PER_MINUTE

#define RECORDTURNS                         PLAYTIME_MINUTES * RECORD_TURNS_PER_MINUTE
#define PLAYING_RANGE                       RECORDTURNS * RECORD_GROOVE_PITCH
#define ENDGROOVE_WIDTH                     CUTTING_RANGE - PLAYING_RANGE - STARTGROOVE_WIDTH

#define SECONDS_PER_TABLE_ROTATION          60.0 / RECORD_TURNS_PER_MINUTE
#define CAT_SPEED                           RECORD_GROOVE_PITCH / (SECONDS_PER_TABLE_ROTATION)

#define CAT_MOTOR_TURNS_PER_SONG            PLAYING_RANGE / SPIN_PITCH_M6 / (CAT_DRIVE_RATIO)
#define CAT_MOTOR_DEGREES_PER_SONG          CAT_MOTOR_TURNS_PER_SONG * 360.0

#define CAT_MOTOR_TURNS_PER_SECOND          (CAT_SPEED) / SPIN_PITCH_M6 / (CAT_DRIVE_RATIO)
#define CAT_MOTOR_DEGREES_PER_SECOND        CAT_MOTOR_TURNS_PER_SECOND * 360.0

#define CAT_SPEED_START                     START_GROOVE_PITCH / (SECONDS_PER_TABLE_ROTATION)

#define CAT_MOTOR_TURNS_PER_START           STARTGROOVE_WIDTH / SPIN_PITCH_M6 / (CAT_DRIVE_RATIO)
#define CAT_MOTOR_DEGREES_PER_START         CAT_MOTOR_TURNS_PER_START * 360.0

#define CAT_MOTOR_TURNS_PER_SECOND_START    (CAT_SPEED_START) / SPIN_PITCH_M6 / (CAT_DRIVE_RATIO)
#define CAT_MOTOR_DEGREES_PER_SECOND_START  CAT_MOTOR_TURNS_PER_SECOND_START * 360.0

#define CAT_SPEED_END                       END_GROOVE_PITCH / (SECONDS_PER_TABLE_ROTATION)

#define CAT_MOTOR_TURNS_PER_END             (ENDGROOVE_WIDTH) / SPIN_PITCH_M6 / (CAT_DRIVE_RATIO)
#define CAT_MOTOR_DEGREES_PER_END           CAT_MOTOR_TURNS_PER_END * 360.0

#define CAT_MOTOR_TURNS_PER_SECOND_END      (CAT_SPEED_END) / SPIN_PITCH_M6 / (CAT_DRIVE_RATIO)
#define CAT_MOTOR_DEGREES_PER_SECOND_END    CAT_MOTOR_TURNS_PER_SECOND_END * 360.0





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
#define HEAD_LEFT_CUT_POSITION          900
//#define HEAD_LEFT_CUT_POSITION          1610
//#define HEAD_LEFT_CUT_POSITION          1810
#define HEAD_LEFT_MID_POSITION          1520
#define HEAD_LEFT_TOP_POSITION          2100

#define HEAD_RIGHT_PARK_POSITION        900
//#define HEAD_RIGHT_CUT_POSITION         1810
//#define HEAD_RIGHT_CUT_POSITION         1520
#define HEAD_RIGHT_CUT_POSITION         1460
//#define HEAD_RIGHT_CUT_POSITION         1430
//#define HEAD_RIGHT_CUT_POSITION         1210
#define HEAD_RIGHT_MID_POSITION         1520
#define HEAD_RIGHT_TOP_POSITION         900

#define STOCK_SUPPLY_RECORD_SPEED       640
#define STOCK_SUPPLY_RECORD_ACCEL       320
#define STOCK_LOADING_SPEED             520
#define STOCK_LOADING_ACCEL             260

#define CAT_PARK_POSITION               0
#define CAT_CUTTING_START_POSITION      98000
#define CAT_SONG_START_POSITION         CAT_CUTTING_START_POSITION + (CAT_MOTOR_DEGREES_PER_START)
#define CAT_SONG_END_POSITION           CAT_SONG_START_POSITION + (CAT_MOTOR_DEGREES_PER_SONG)
#define CAT_CUTTING_END_POSITION        CAT_CUTTING_START_POSITION + (float)CUTTING_RANGE * 360.0 / SPIN_PITCH_M6 / (CAT_DRIVE_RATIO)


#define SOLENOID_FREQUENCY                 10
#define SOLENOID_DUTYCYCLE           0.3 * 16

#define NO_START_DELAY                      0

#endif

