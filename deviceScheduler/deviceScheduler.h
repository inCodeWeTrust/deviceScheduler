
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

/// | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|
/// |   A0  |               |                               |
/// |   A1  |               |                               |
/// |   A2  |               |                               |
/// |   A3  |               |                               |
/// |   A4  |               |                               |
/// |   A5  |               |                               |
/// |   A6  |               |  match head impact button     |
/// |   A7  |               |                               |
/// |   A8  |  <header 01>  |  head inclination sensor      |
/// |   A9  |  <header 01>  |                               |
/// |  A10  |  <header 02>  |                               |
/// |  A11  |  <header 02>  |                               |
/// |  A12  |  <header 02>  |                               |
/// |  A13  |  <header 02>  |                               |
/// |  A14  |               |                               |
/// |  A15  |               |                               |

/// | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|
/// |    0  |               |                               |
/// |    1  |               |                               |
/// |    2  |               |                               |
/// |    3  |               |                               |
/// |    4  |               |                               |
/// |    5  |               |                               |
/// |    6  |               |                               |
/// |    7  |               |  TMC260 enable                |

/// | pin   | header        | function on shields           | function on board             |
/// |-------|---------------|-------------------------------|-------------------------------|
/// |    8  |               |  TMC260 step                  |   */*                         |
/// |    9  |               |  TMC260 dir                   |   */*                         |
/// |   10  |               |  TMC260 chipSelect            |   */*                         |
/// |   11  |  SPI / nc     |  TMC260                       |                               |
/// |   12  |  SPI / nc     |  TMC260                       |  i_am_late pin                |
/// |   13  |  SPI / nc     |  TMC260                       |                               |

/// | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|
/// |   14  |  <header 03>  |  liftServo                    |
/// |   15  |  <header 03>  |  turnServo                    |
/// |   16  |  <header 03>  |  headLeftServo                |
/// |   17  |  <header 03>  |  headRightServo               |
/// |   18  |  <header 03>  |                               |
/// |   19  |  <header 03>  |                               |
/// |   20  |  <header 03>  |                               |
/// |   21  |  <header 03>  |                               |

/// | pin   | header        | function                      || pin   | header        | function                      |
/// |-------|---------------|-------------------------------||-------|---------------|-------------------------------|
/// |   22  |   A4988 01    |  stockStepper dir             ||   23  |   A4988 02    |  conveyorStepper enable       |
/// |   24  |   A4988 01    |  stockStepper step            ||   25  |   A4988 02    |  conveyorStepper microstep 00 |
/// |   26  |   A4988 01    |  stockStepper enable          ||   27  |   A4988 02    |  conveyorStepper microstep 01 |
/// |   28  |   A4988 01    |  stockStepper microstep 00    ||   29  |   A4988 02    |  conveyorStepper microstep 02 |
/// |   30  |   A4988 01    |  stockStepper microstep 01    ||   31  |   A4988 02    |  conveyorStepper step         |
/// |   32  |   A4988 01    |  stockStepper microstep 02    ||   33  |   A4988 02    |  conveyorStepper dir          |
/// |   34  |   DRV8825     |  tableStepper dir             ||   35  |               |                               |
/// |   36  |   DRV8825     |  tableStepper step            ||   37  |               |                               |
/// |   38  |   DRV8825     |  tableStepper enable          ||   39  |               |                               |
/// |   40  |   DRV8825     |  tableStepper microstep 00    ||   41  |               |                               |
/// |   42  |   DRV8825     |  tableStepper microstep 01    ||   43  |  <header 10>  |                               |
/// |   44  |   DRV8825     |  tableStepper microstep 02    ||   45  |  <header 10>  |  stockBottomButton            |
/// |   46  |               |  songCancelButton             ||   47  |  <header 10>  |  stockTopButton               |
/// |   48  |               |  songEndButton                ||   49  |  <header 10>  |  recordAvaliableButton        |
/// |   50  |               |  startCuttingButton           ||   51  |  <header 10>  |  bridgeEndButton              |
/// |   52  |               |  fetchRecordButton            ||   53  |  <header 10>  |  bridgeParkButton             |
///


#define STEPPER_STOCK_NAME                  "stockStepper"

#define STEPPER_STOCK_DIR_PIN               22
#define STEPPER_STOCK_STEP_PIN              24
#define STEPPER_STOCK_ENABLE_PIN            26
#define STEPPER_STOCK_MICROSTEPPINS         "28, 30, 32"

#define STEPPER_STOCK_HIGHEST_STEPPINGMODE  4        
#define STEPPER_STOCK_STEPMODECODES         "0x00, 0x01, 0x02, 0x03, 0x07"
#define STEPPER_STOCK_STEPS_PER_ROTATION    48
#define STEPPER_STOCK_PPS_MAX               800
#define STEPPER_STOCK_ACC_MAX               2000


#define STEPPER_CONVEYOR_NAME                  "conveyorStepper"

#define STEPPER_CONVEYOR_DIR_PIN               33
#define STEPPER_CONVEYOR_STEP_PIN              31
#define STEPPER_CONVEYOR_ENABLE_PIN            23
#define STEPPER_CONVEYOR_MICROSTEPPINS         "25, 27, 29"

#define STEPPER_CONVEYOR_HIGHEST_STEPPINGMODE  4
#define STEPPER_CONVEYOR_STEPMODECODES         "0x00, 0x01, 0x02, 0x03, 0x07"
#define STEPPER_CONVEYOR_STEPS_PER_ROTATION    48
#define STEPPER_CONVEYOR_PPS_MAX               800
#define STEPPER_CONVEYOR_ACC_MAX               2000



#define STEPPER_TABLE_NAME                  "tableStepper"

#define STEPPER_TABLE_DIR_PIN               34
#define STEPPER_TABLE_STEP_PIN              36
#define STEPPER_TABLE_ENABLE_PIN            38
#define STEPPER_TABLE_MICROSTEPPINS         "40, 42, 44"
#define STEPPER_TABLE_HIGHEST_STEPPINGMODE  5
#define STEPPER_TABLE_STEPMODECODES         "0x00, 0x01, 0x02, 0x03, 0x04, 0x05"
#define STEPPER_TABLE_STEPS_PER_ROTATION    48
#define STEPPER_TABLE_PPS_MAX               800
#define STEPPER_TABLE_ACC_MAX               2000



#define STEPPER_CAT_NAME                    "catStepper"

#define STEPPER_CAT_CHIPSELECT_PIN          10
#define STEPPER_CAT_DIR_PIN                 9
#define STEPPER_CAT_STEP_PIN                8
#define STEPPER_CAT_ENABLE_PIN              7
#define STEPPER_CAT_CURRENT                 1100
#define STEPPER_CAT_STEPS_PER_ROTATION      200
#define STEPPER_CAT_PPS_MAX                 1000
#define STEPPER_CAT_ACC_MAX                 4000



#define SERVO_LIFT_NAME                     "liftDoubleServo"

#define SERVO_LIFT_LEFT_PIN                      14
#define SERVO_LIFT_LEFT_MIN_POSITION            800
#define SERVO_LIFT_LEFT_MID_POSITION            1500
#define SERVO_LIFT_LEFT_MAX_POSITION            2200

#define SERVO_LIFT_RIGHT_PIN                      15
#define SERVO_LIFT_RIGHT_MIN_POSITION             150
#define SERVO_LIFT_RIGHT_MID_POSITION            1500
#define SERVO_LIFT_RIGHT_MAX_POSITION             600



#define SERVO_TURN_NAME                     "turnServo"

#define SERVO_TURN_PIN                      16
#define SERVO_TURN_MIN_POSITION             150
#define SERVO_TURN_MAX_POSITION             600


#define SERVO_PUMP_NAME                     "pumpServo"

#define SERVO_PUMP_PIN                      17
#define SERVO_PUMP_MIN_POSITION             150
#define SERVO_PUMP_MAX_POSITION             600




#define SERVO_HEAD_LEFT_NAME                "headLeftServo"

#define SERVO_HEAD_LEFT_PIN                 18
#define SERVO_HEAD_LEFT_MIN_POSITION        900
#define SERVO_HEAD_LEFT_MAX_POSITION        2100



#define SERVO_HEAD_RIGHT_NAME               "headRightServo"

#define SERVO_HEAD_RIGHT_PIN                19
#define SERVO_HEAD_RIGHT_MIN_POSITION       900
#define SERVO_HEAD_RIGHT_MAX_POSITION       2100





// TMC260 sensing resistor value in mOhm
#define R_SENSE                             150

//  ################## BUTTONS AND SENSORS #########################################################################


#define SONG_ENDBUTTON_NAME                 "songEndButton"
#define SONG_ENDBUTTON_PIN                  48
#define SONG_ENDBUTTON_ACTIV                LOW
#define SONG_ENDBUTTON_PULLUP               true

#define SONG_CANCELBUTTON_NAME              "songCancelButton"
#define SONG_CANCELBUTTON_PIN               46
#define SONG_CANCELBUTTON_ACTIV             LOW
#define SONG_CANCELBUTTON_PULLUP            true

#define CAT_PARKBUTTON_NAME                 "bridgeParkButton"
#define CAT_PARKBUTTON_PIN                  53
#define CAT_PARKBUTTON_ACTIV                HIGH
#define CAT_PARKBUTTON_PULLUP               true

#define CAT_ENDBUTTON_NAME                  "bridgeEndButton"
#define CAT_ENDBUTTON_PIN                   51
#define CAT_ENDBUTTON_ACTIV                 HIGH
#define CAT_ENDBUTTON_PULLUP                true


#define RECORDAVAILABLE_BUTTON_NAME         "recordAvailableButton"
#define RECORDAVAILABLE_BUTTON_PIN          49
#define RECORDAVAILABLE_BUTTON_ACTIV        HIGH
#define RECORDAVAILABLE_BUTTON_PULLUP       true

#define STOCKBOTTOM_BUTTON_NAME      "stockBottomButton"
#define STOCKBOTTOM_BUTTON_PIN       47
#define STOCKBOTTOM_BUTTON_ACTIV     HIGH
#define STOCKBOTTOM_BUTTON_PULLUP    true

#define STOCKTOP_BUTTON_NAME         "stockTopButton"
#define STOCKTOP_BUTTON_PIN          45
#define STOCKTOP_BUTTON_ACTIV        HIGH
#define STOCKTOP_BUTTON_PULLUP       true



//#define CAT_FWD                             A3
//#define CAT_RWD                             A2
#define START_CUTTING_BUTTON                50
#define FETCH_RECORD_BUTTON                 52

#define MATCH_HEADIMPACT_BUTTON             A6

#define HEAD_INCLINATION_SENSOR             A8


#define I_AM_LATE_LED                       12




//  ################## CUTTING TOOL #########################################################################

#define SPIN_PITCH_M6                       1
//#define CAT_DRIVE_RATIO                     22.0 / 60.0 * 22.0 / 60.0
#define CAT_DRIVE_RATIO                     36.0 / 60.0 * 22.0 / 60.0
#define RECORD_TURNS_PER_MINUTE             45

#define TABLE_DRIVE_RATIO                   123.0 / 28.0


//  ################## MOVE DATA #############################################################################


#define LIFT_SPEED_FAST                 1600
#define LIFT_SPEED_SLOW                 400
#define LIFT_SPEED_VERY_SLOW            120
#define LIFT_ACCEL_FAST                 1600
#define LIFT_ACCEL_SLOW                 400
#define LIFT_ACCEL_VERY_SLOW            200

#define LIFT_UP_POSITION               2200
#define LIFT_UP_TRIGGER_TURN           1800
#define LIFT_TABLE_POSITION            1000
#define LIFT_STOCK_POSITION            1000
#define LIFT_CONVEYER_POSITION         1000
#define LIFT_PARK_POSITION             1000


#define TURN_SPEED_FAST                 320
#define TURN_SPEED_SLOW                 480
#define TURN_ACCEL_FAST                 280
#define TURN_ACCEL_SLOW                 150

#define TURN_TABLE_POSITION             2022
#define TURN_CONVEYOR_POSITION          1520
#define TURN_STOCK_POSITION             1010
#define TURN_TO_CONVEYER_TRIGGER_LIFT   TURN_CONVEYOR_POSITION + 60
#define TURN_TO_TABLE_TRIGGER_LIFT      TURN_TABLE_POSITION - 60
#define TURN_TO_STOCK_TRIGGER_LIFT      TURN_STOCK_POSITION + 60


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

#define PUMP_PARK_POSITION              2000
#define PUMP_DOWN_POSITION              800


#define STOCK_SUPPLY_RECORD_SPEED       1000
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
#define CAT_CUTTING_START_POSITION      131600

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

