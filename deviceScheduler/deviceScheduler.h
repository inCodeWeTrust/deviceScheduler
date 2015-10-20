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






#define VERBOSE_BUTTON                      A5

#define STEPPINGPERIOD_TO_KICK_UP           150
#define STEPPINGPERIOD_TO_KICK_DOWN         300



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

#define SERVO_TURN_INDEX                    3
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
#define TABLEDRIVE_ON                       HIGH
#define TABLEDRIVE_OFF                      LOW


#define SOLENOID_VACUUM_NAME                "vacuumSolenoid"
#define SOLENOID_VACUUM_PIN                 9
#define SOLENOID_ON                         HIGH
#define SOLENOID_OFF                        LOW


#define START_BUTTON                        A3
#define STOP_BUTTON                         A2
#define START_CUTTING_BUTTON                A1
#define STOP_CUTTING_BUTTON                 A0
#define CAT_PARK_BUTTON                     A4

//#define START_BUTTON                        A2
//#define LOADING_BUTTON                      A1
//#define RECORD_AVAILABLE_BUTTON             A0
//#define CAT_PARK_BUTTON                     A3

#define I_AM_LATE_LED                       12
#define SPEED_PIN                           A5


//  ################## MOVE DATA #############################################################################


#define LIFT_SPEED_FAST                 1600
#define LIFT_SPEED_SLOW                 600
#define LIFT_ACCEL_FAST                 1600
#define LIFT_ACCEL_SLOW                 400
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
#define HEAD_LEFT_CUT_POSITION          1580
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
#define CAT_GROOVE_START_POSITION       36000
#define CAT_SONG_START_POSITION         46000
#define CAT_SONG_END_POSITION           246000
#define CAT_GROOVE_END_POSITION         256000


#define SOLENOID_FREQUENCY                 10
#define SOLENOID_DUTYCYCLE           0.3 * 16

#define NO_START_DELAY                      0



