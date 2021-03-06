
#ifndef __deviceScheduler__voiceOGraph__
#define __deviceScheduler__voiceOGraph__




///////////////////////////////////////////////////////////////////////////////////////////////////
/// @page pinmap pin mapping on the Arduino Due
///
/// | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|
/// |   A0  |               |                               |
/// |   A1  |               |                               |
/// |   A2  |               |  evaluateButtons switch       |
/// |   A3  |               |  move cat manually switch     |
/// |   A4  |               |  move cat forward button      |
/// |   A5  |               |  move cat back button         |
/// |   A6  |               |  match head impact switch     |
/// |   A7  |               |  SCHEDULERS_TASKVIEW_SHOW_PIN |
/// |   A8  |  <header 01>  |  head inclination sensor      |
/// |   A9  |  <header 01>  |  arm turn sensor              |
/// |  A10  |  <header 02>  |                               |
/// |  A11  |  <header 02>  |                               |
/// |  A12  |  <header 02>  |                               |
/// |  A13  |  <header 02>  |                               |
/// |  A14  |               |                               |
/// |  A15  |               |                               |
///
/// | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|
/// |    0  |               |  <serial communication>       |
/// |    1  |               |  <serial communication>       |
/// |    2  |               |                               |
/// |    3  |               |  TMC260 table enable          |
/// |    4  |               |  TMC260 table step            |
/// |    5  |               |  TMC260 table dir             |
/// |    6  |               |  TMC260 table chip select     |
/// |    7  |               |  TMC260 cat enable            |
///
/// | pin   | header        | function on shields           | function on board             |
/// |-------|---------------|-------------------------------|-------------------------------|
/// |    8  |               |  TMC260 cat step              |                               |
/// |    9  |               |  TMC260 cat dir               |                               |
/// |   10  |               |  TMC260 cat chipSelect        |                               |
/// |   11  |  SPI          |  TMC260                       |                               |
/// |   12  |  SPI          |  TMC260                       |  i_am_late pin                |
/// |   13  |  SPI          |  TMC260                       |                               |
///
/// | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|
/// |   14  |  <header 03>  |  liftServoLeft                |
/// |   15  |  <header 03>  |  liftServoRight               |
/// |   16  |  <header 03>  |  turnServo                    |
/// |   17  |  <header 03>  |  pumpServo                    |
/// |   18  |  <header 03>  |  headLeftServo                |
/// |   19  |  <header 03>  |  headRightServo               |
/// |   20  |  <header 03>  |                               |
/// |   21  |  <header 03>  |                               |
///
/// | pin   | header        | function                      | | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|-|-------|---------------|-------------------------------|
/// |   22  |   A4988 01    |  stockStepper dir             | |   23  |   A4988 02    |  conveyorStepper enable       |
/// |   24  |   A4988 01    |  stockStepper step            | |   25  |   A4988 02    |  conveyorStepper microstep 00 |
/// |   26  |   A4988 01    |  stockStepper enable          | |   27  |   A4988 02    |  conveyorStepper microstep 01 |
/// |   28  |   A4988 01    |  stockStepper microstep 00    | |   29  |   A4988 02    |  conveyorStepper microstep 02 |
/// |   30  |   A4988 01    |  stockStepper microstep 01    | |   31  |   A4988 02    |  conveyorStepper step         |
/// |   32  |   A4988 01    |  stockStepper microstep 02    | |   33  |   A4988 02    |  conveyorStepper dir          |
/// |   34  |   DRV8825     |  tableStepper dir             | |   35  |  <header 20>  |  vacuumclaner                 |
/// |   36  |   DRV8825     |  tableStepper step            | |   37  |  <header 20>  |                               |
/// |   38  |   DRV8825     |  tableStepper enable          | |   39  |  <header 21>  |  Controller Lamp red          |
/// |   40  |   DRV8825     |  tableStepper microstep 00    | |   41  |  <header 21>  |  Controller Lamp yellow       |
/// |   42  |   DRV8825     |  tableStepper microstep 01    | |   43  |  <header 10>  |                               |
/// |   44  |   DRV8825     |  tableStepper microstep 02    | |   45  |  <header 10>  |  stockBottomButton            |
/// |   46  |               |                               | |   47  |  <header 10>  |  stockTopButton               |
/// |   48  |  <header 21>  |  songEndButton                | |   49  |  <header 10>  |  recordAvaliableButton        |
/// |   50  |  <header 21>  |  startCuttingButton           | |   51  |  <header 10>  |  bridgeEndButton              |
/// |   52  |  <header 21>  |  fetchRecordButton            | |   53  |  <header 10>  |  bridgeParkButton             |
///
/// header 21:
/// upper: [left] | GND (bl)            | powerOn/reset (or) | yellow lamp (ye) | red lamp (gr)       | [right]
/// lower: [left] | loading button (wh) | start button (gy)  | stop button (pi) | emergency stop (rd) | [right]
///
///////////////////////////////////////////////////////////////////////////////////////////////////



#define SCHEDULERS_TASKVIEW_SHOW_PIN        A7
#define SCHEDULERS_TASKVIEW_SHOW            LOW
#define SCHEDULERS_TASKVIEW_HIDE            HIGH


//  ################## Hardware Cutting Parameters #########################################################

#define SONGGROOVE_PITCH                    0.1
#define STARTGROOVE_PITCH                   1
#define STARTGROOVE_RANGE                   4
#define ENDGROOVE_PITCH                     4.8
#define ENDGROOVE_RANGE_MIN                 10
#define CUTTING_RANGE                       38
#define PLAYTIME_MINUTES                    6

#define ADJUSTGROOVEPITCH_TO_PLAYTIME       false




//  ################## Hardware #############################################################################

//  stockStepper's driver is located next to pin 14 - 21
#define STEPPER_STOCK_NAME                      "stockStepper"

#define STEPPER_STOCK_DIR_PIN                   22
#define STEPPER_STOCK_STEP_PIN                  24
#define STEPPER_STOCK_ENABLE_PIN                26
#define STEPPER_STOCK_MICROSTEPPIN_00           28
#define STEPPER_STOCK_MICROSTEPPIN_01           30
#define STEPPER_STOCK_MICROSTEPPIN_02           32

#define STEPPER_STOCK_STEPMODECODE_00           0x00
#define STEPPER_STOCK_STEPMODECODE_01           0x01
#define STEPPER_STOCK_STEPMODECODE_02           0x02
#define STEPPER_STOCK_STEPMODECODE_03           0x03
#define STEPPER_STOCK_STEPMODECODE_04           0x07
#define STEPPER_STOCK_STEPS_PER_ROTATION        48


//  conveyer's driver is located next to pin 5 - 13
#define STEPPER_CONVEYOR_NAME                  "conveyorStepper"

#define STEPPER_CONVEYOR_DIR_PIN                33
#define STEPPER_CONVEYOR_STEP_PIN               31
#define STEPPER_CONVEYOR_ENABLE_PIN             23
#define STEPPER_CONVEYOR_MICROSTEPPIN_00        25
#define STEPPER_CONVEYOR_MICROSTEPPIN_01        27
#define STEPPER_CONVEYOR_MICROSTEPPIN_02        29

#define STEPPER_CONVEYOR_STEPMODECODE_00        0x00
#define STEPPER_CONVEYOR_STEPMODECODE_01        0x01
#define STEPPER_CONVEYOR_STEPMODECODE_02        0x02
#define STEPPER_CONVEYOR_STEPMODECODE_03        0x03
#define STEPPER_CONVEYOR_STEPMODECODE_04        0x07
#define STEPPER_CONVEYOR_STEPS_PER_ROTATION     48


//  turnStepper's driver is located next to pin A8 - CANTX
#define STEPPER_TURN_NAME                       "turnStepper"

#define STEPPER_TURN_DIR_PIN                   34
#define STEPPER_TURN_STEP_PIN                  36
#define STEPPER_TURN_ENABLE_PIN                38
#define STEPPER_TURN_MICROSTEPPIN_00           40
#define STEPPER_TURN_MICROSTEPPIN_01           42
#define STEPPER_TURN_MICROSTEPPIN_02           44

#define STEPPER_TURN_STEPMODECODE_00           0x00
#define STEPPER_TURN_STEPMODECODE_01           0x01
#define STEPPER_TURN_STEPMODECODE_02           0x02
#define STEPPER_TURN_STEPMODECODE_03           0x03
#define STEPPER_TURN_STEPMODECODE_04           0x04
#define STEPPER_TURN_STEPMODECODE_05           0x05
#define STEPPER_TURN_STEPS_PER_ROTATION        200


#define STEPPER_TABLE_NAME                      "tableStepper"

#define STEPPER_TABLE_CHIPSELECT_PIN            6
#define STEPPER_TABLE_DIR_PIN                   5
#define STEPPER_TABLE_STEP_PIN                  4
#define STEPPER_TABLE_ENABLE_PIN                3
#define STEPPER_TABLE_CURRENT                   1100
#define STEPPER_TABLE_STEPS_PER_ROTATION        48




#define STEPPER_CAT_NAME                        "catStepper"

#define STEPPER_CAT_CHIPSELECT_PIN              10
#define STEPPER_CAT_DIR_PIN                     9
#define STEPPER_CAT_STEP_PIN                    8
#define STEPPER_CAT_ENABLE_PIN                  7
#define STEPPER_CAT_CURRENT                     1100
#define STEPPER_CAT_STEPS_PER_ROTATION          200



#define SERVO_LIFT_NAME                         "liftDoubleServo"

#define SERVO_LIFT_LEFT_PIN                     14
#define SERVO_LIFT_LEFT_MIN_POSITION            800
#define SERVO_LIFT_LEFT_MID_POSITION            1500
#define SERVO_LIFT_LEFT_MAX_POSITION            2200

#define SERVO_LIFT_RIGHT_PIN                    15
#define SERVO_LIFT_RIGHT_MIN_POSITION           150
#define SERVO_LIFT_RIGHT_MID_POSITION           1500
#define SERVO_LIFT_RIGHT_MAX_POSITION           600



#define SERVO_TURN_NAME                         "turnServo"

#define SERVO_TURN_PIN                          16
#define SERVO_TURN_MIN_POSITION                 800
#define SERVO_TURN_MAX_POSITION                 2200


#define SERVO_PUMP_NAME                         "pumpServo"

#define SERVO_PUMP_PIN                          17
#define SERVO_PUMP_MIN_POSITION                 800
#define SERVO_PUMP_MAX_POSITION                 2200




#define SERVO_HEAD_LEFT_NAME                    "headLeftServo"

#define SERVO_HEAD_LEFT_PIN                     18
#define SERVO_HEAD_LEFT_MIN_POSITION            900
#define SERVO_HEAD_LEFT_MAX_POSITION            2100



#define SERVO_HEAD_RIGHT_NAME                   "headRightServo"

#define SERVO_HEAD_RIGHT_PIN                    19
#define SERVO_HEAD_RIGHT_MIN_POSITION           900
#define SERVO_HEAD_RIGHT_MAX_POSITION           2100



#define VACCUUMCLEANER_NAME                     "vacuumCleaner"

//#define VACCUUMCLEANER_PIN                      21
#define VACCUUMCLEANER_PIN                      35
#define VACCUUMCLEANER_ACTIVE                    HIGH



#define CONTROLLER_LAMP_RED_NAME                "redLamp"

#define CONTROLLER_LAMP_RED_PIN                 39
#define CONTROLLER_LAMP_RED_ON                  HIGH



#define CONTROLLER_LAMP_YELLOW_NAME             "yellowLamp"

#define CONTROLLER_LAMP_YELLOW_PIN              41
#define CONTROLLER_LAMP_YELLOW_ON               HIGH
#define CONTROLLER_LAMP_YELLOW_OFF              LOW



//  ################## BUTTONS AND SENSORS #########################################################################


#define SONG_ENDBUTTON_NAME                 "songEndButton"
#define SONG_ENDBUTTON_PIN                  48
#define SONG_ENDBUTTON_MODE                 INPUT_PULLUP
#define SONG_ENDBUTTON_PRESSED              LOW

#define SONG_CANCELBUTTON_NAME              "songCancelButton"
#define SONG_CANCELBUTTON_PIN               46
#define SONG_CANCELBUTTON_MODE              INPUT_PULLUP
#define SONG_CANCELBUTTON_PRESSED           LOW

#define CAT_PARKBUTTON_NAME                 "bridgeParkButton"
#define CAT_PARKBUTTON_PIN                  53
#define CAT_PARKBUTTON_MODE                 INPUT_PULLUP
// for simulation: #define CAT_PARKBUTTON_MODE               false
#define CAT_PARKBUTTON_REACHED              HIGH

#define CAT_ENDBUTTON_NAME                  "bridgeEndButton"
#define CAT_ENDBUTTON_PIN                   51
#define CAT_ENDBUTTON_MODE                  INPUT_PULLUP
#define CAT_ENDBUTTON_REACHED               HIGH


#define RECORDAVAILABLE_BUTTON_NAME         "recordAvailableButton"
#define RECORDAVAILABLE_BUTTON_PIN          49
#define RECORDAVAILABLE_BUTTON_MODE         INPUT_PULLUP
// for simulation: #define RECORDAVAILABLE_BUTTON_MODE       false
#define RECORD_IS_AVAILABLE                 HIGH

#define STOCKBOTTOM_BUTTON_NAME             "stockBottomButton"
#define STOCKBOTTOM_BUTTON_PIN              47
#define STOCKBOTTOM_BUTTON_MODE             INPUT_PULLUP
#define STOCKBOTTOM_BUTTON_IS_REACHED       HIGH

#define STOCKTOP_BUTTON_NAME                "stockTopButton"
#define STOCKTOP_BUTTON_PIN                 45
#define STOCKTOP_BUTTON_MODE                INPUT_PULLUP
#define STOCKTOP_BUTTON_IS_REACHED          HIGH



#define START_CUTTING_BUTTON                50

#define FETCH_RECORD_BUTTON_NAME            "fetchRecordButton"
#define FETCH_RECORD_BUTTON_PIN             52
#define FETCH_RECORD_BUTTON_MODE            INPUT_PULLUP
#define FETCH_RECORD_BUTTON_PRESSED         LOW

#define EVALUATE_BUTTONS_SWITCH             A4
#define MATCH_HEADIMPACT_SWITCH             A5


#define HEAD_INCLINATION_SENSOR_NAME        "headInclinationSensor"
#define HEAD_INCLINATION_SENSOR_PIN         A8
// for simulation: #define HEAD_INCLINATION_SENSOR_PIN         A4
#define HEAD_INCLINATION_INIT_STOP          600
#define HEAD_INCLINATION_START_CAT          580
#define HEAD_INCLINATION_TARGET             460


#define ARM_SENSOR_NAME                     "armTurnSensor"
// #define ARM_SENSOR_PIN                      A9
#define ARM_SENSOR_PIN                      A10





//  ################## CUTTING TOOL #########################################################################

#define SPIN_PITCH_M6                       1
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

#define LIFT_UP_POSITION                2200
#define LIFT_UP_TRIGGER_TURN            1400
#define LIFT_TABLE_POSITION             1000
#define LIFT_STOCK_POSITION             1000
#define LIFT_CONVEYER_POSITION          1000
#define LIFT_LIFTED_POSITION            1400
#define LIFT_PARK_POSITION              1000


#define TURN_SPEED_FAST                 120
#define TURN_SPEED_SLOW                 60
#define TURN_ACCEL_FAST                 120
#define TURN_ACCEL_SLOW                 60

#define TURN_TABLE_POSITION             -563
#define TURN_CONVEYOR_POSITION          -264
#define TURN_STOCK_POSITION             0
#define TURN_TO_CONVEYER_TRIGGER_LIFT   -360
#define TURN_TO_TABLE_TRIGGER_LIFT      -460
#define TURN_TO_STOCK_TRIGGER_LIFT      -70

#define ARM_STOCK_POSITION              76
#define ARM_CONVEYOR_POSITION           248
#define ARM_TABLE_POSITION              483

#define HEAD_LEFT_PARK_POSITION         2100
//#define HEAD_LEFT_CUT_POSITION          1260
#define HEAD_LEFT_CUT_POSITION          1060
#define HEAD_LEFT_MID_POSITION          1520
#define HEAD_LEFT_TOP_POSITION          2100

#define HEAD_RIGHT_PARK_POSITION        900
#define HEAD_RIGHT_CUT_POSITION         2100
#define HEAD_RIGHT_MID_POSITION         1520
#define HEAD_RIGHT_TOP_POSITION         900

#define PUMP_SPEED                      1800
#define PUMP_ACCEL                      1800
#define PUMP_PARK_POSITION              2000
#define PUMP_DOWN_POSITION              800


#define STOCK_SUPPLY_RECORD_SPEED       1000
#define STOCK_SUPPLY_RECORD_ACCEL       320
#define STOCK_LOADING_RECORDS_SPEED     1800
#define STOCK_LOADING_RECORDS_ACCEL     1800

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

#define CONVEYOR_DISTANCE               8400
#define CONVEYOR_SPEED                  600
#define CONVEYOR_ACCEL                  400



#endif // defined(__deviceScheduler__voiceOGraph__)







///////////////////////////////////////////////////////////////////////////////////////////////////
/// @mainpage	deviceScheduler
///
/// Diese Software wurde anlässlich des Baus eines Schallplattenschneide-Automaten entwickelt. Es ist ein Kunstprojekt. Dieser Apparat, der Voice-o-graph, ist eine Art Passbild-Automat, nur dass darin nicht das Bild, sondern der Ton analog aufgezeichnet wird. In einer kleinen Kabine hat man die Möglichkeit, 3 min Ton aufzunehmen, der simultan in einen 7inch-Schallplattenrohling geschnitten wird. Nachdem man also einen Gruß an einen Menschen in der Ferne, ein Liebesgedicht, seinen Lieblings-Popsong oder vielleicht ein großes Geheimnis ins Mikrofon gesprochen oder gesungen hat, erhält man es als Vinyl-Schallplatte. @n
/// Das gesamte Gerät wird durch den 32-bit ARM core microcontroller SAM3X8E ARM Cortex-M3 CPU (Arduino Due) gesteuert. Erstellt habe ich es auf einem Apple-Computer mit der IDE XCode und dem embedXcode+ plugin. Der Upload wird mit der Arduino-Toolchain bewerkstelligt.@n
/// Das Steuerungsprogramm ist ein Scheduler, der Geräte wie z.B. Stepper-Motoren, Modellbau-Servos, Gleichstrom-Motoren, Elektromagnete etc. starten, stoppen und führen kann. Diese Software ist in der Programmiersprache C++ geschrieben und nutzt einerseits deren Betriebssystemnähe und Schnelligkeit, andererseits deren Möglichkeit, objektorientiert zu programmieren: sie nutzt das Konzept der Klassen und der Polymorphie. Die Ansteuerung der Geräte wie z.B. die Erzeugung der Steuerimpulse für die Modellbau-Servos oder die Taktung der Schrittmotoren erfolgt ausschließlich in der Klasse der Geräte, während die Koordinierung des Arbeitsablaufes allein die Aufgabe der Schedulerklasse ist. Diese Trennung von Steuerung und Koordinierung bringt erhebliche Vorteile mit sich: der Code wird wesentlich reduzierter, übersichtlicher und strukturierter, wodurch Programmierfehler vermieden werden können bzw. schneller auffindbar werden. Der Code ist leicht lesbar und verständlich und an verschiedenste Anforderungen einfach anpassbar. Durch die Anwendung von aussagekräftigen Variablennamen wird die klare Struktur zusätzlich optimiert. Es können prinzipiell beliebig viele Geräte gleichzeitig gesteuert und überwacht und prinzipiell beliebig viele Aufgaben abgearbeitet und untereinander koordiniert werden. Programmiert ist sie wie eine Library, wodurch sie sehr leicht für andere Anlagen und Szenarios adaptiert werden kann. @n @n
///
/// @author		Christoph Freidhöfer
/// @author		(http://kunst-und-raederwerk.net)
/// @date		30.05.2016
///
/// @copyright	(c) little Ikarus, 2015
///
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @page mainProgramPage voiceOGraph.ino
/// voiceOGraph.ino is the main programm to control the cutting machine.
///
///////////////////////////////////////////////////////////////////////////////////////////////////



