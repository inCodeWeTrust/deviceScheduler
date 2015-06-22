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

#define STEPPINGPERIOD_TO_KICK_UP           600
#define STEPPINGPERIOD_TO_KICK_DOWN         1200



//  ################## Hardware #############################################################################

#define STEPPER_STOCK_NAME                  "stockStepper"

#define STEPPER_STOCK_DIR_PIN               5
#define STEPPER_STOCK_STEP_PIN              6
#define STEPPER_STOCK_ENABLE_PIN            7
#define STEPPER_STOCK_MICROSTEPPINS         "2, 3, 4"

#define STEPPER_STOCK_HIGHEST_STEPPINGMODE  4        
#define STEPPER_STOCK_STEPMODECODES         "0x00, 0x01, 0x02, 0x03, 0x07"
#define STEPPER_STOCK_STEPS_PER_ROTATION    400
#define STEPPER_STOCK_ACC_MAX               200



#define SERVO_LIFT_NAME                     "liftServo"

#define SERVO_LIFT_PIN                      10
#define SERVO_LIFT_MIN_POSITION             800
#define SERVO_LIFT_MAX_POSITION             2400



#define SERVO_TURN_NAME                     "turnServo"

#define SERVO_TURN_PIN                      11
#define SERVO_TURN_MIN_POSITION             800
#define SERVO_TURN_MAX_POSITION             2400


#define SOLENOID_VACUUM_NAME                "vacuumSolenoid"
#define SOLENOID_VACUUM_PIN                 9
#define SOLENOID_ON                         HIGH
#define SOLENOID_OFF                        LOW


#define START_BUTTON                        A4
#define LOADING_BUTTON                      A1
#define RECORD_AVAILABLE_BUTTON             A0

#define I_AM_LATE_LED                       12
#define SPEED_PIN                           A2


//  ################## MOVE DATA #############################################################################


#define LIFT_SPEED_FAST                  1600
#define LIFT_SPEED_SLOW                   600
#define LIFT_ACCEL_FAST                  1600
#define LIFT_ACCEL_SLOW                   400
#define LIFT_UP_POSITION                  840
#define LIFT_FROM_STOCK_TRIGGER_TURN     1400
#define LIFT_UP_TRIGGER_TURN             1240
#define LIFT_TABLE_POSITION              1460
#define LIFT_STOCK_POSITION              1480
#define LIFT_PARK_POSITION                840

#define TURN_SPEED_FAST                   320
#define TURN_SPEED_SLOW                   180
#define TURN_ACCEL_FAST                   280
#define TURN_ACCEL_SLOW                   150
#define TURN_TABLE_POSITION              1800
#define TURN_TO_PARK_TRIGGER_LIFT        1600
#define TURN_TO_TABLE_TRIGGER_LIFT       1600
#define TURN_PARK_POSITION               1300
#define TURN_TO_STOCK_TRIGGER_LIFT        900
#define TURN_STOCK_POSITION               800

#define STEPPER_SUPPLY_RECORD_SPEED       640
#define STEPPER_SUPPLY_RECORD_ACCEL       320
#define STEPPER_LOADING_SPEED             520
#define STEPPER_LOADING_ACCEL             260

#define SOLENOID_FREQUENCY                 10
#define SOLENOID_DUTYCYCLE           0.3 * 16

#define NO_START_DELAY                      0



