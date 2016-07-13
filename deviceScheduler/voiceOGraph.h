
#ifndef __deviceScheduler__voiceOGraph__
#define __deviceScheduler__voiceOGraph__





///////////////////////////////////////////////////////////////////////////////////////////////////
/// @page pinmap pin mapping on the Arduino Due
///
/// | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|
/// |   A0  |               |                               |
/// |   A1  |               |                               |
/// |   A2  |               |                               |
/// |   A3  |               |  move cat manually switch     |
/// |   A4  |               |  move cat forward button      |
/// |   A5  |               |  move cat back button         |
/// |   A6  |               |  match head impact switch     |
/// |   A7  |               |  loading button               |
/// |   A8  |  <header 01>  |  head inclination sensor      |
/// |   A9  |  <header 01>  |                               |
/// |  A10  |  <header 02>  |                               |
/// |  A11  |  <header 02>  |                               |
/// |  A12  |  <header 02>  |                               |
/// |  A13  |  <header 02>  |                               |
/// |  A14  |               |                               |
/// |  A15  |               |                               |
///
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
///
/// | pin   | header        | function on shields           | function on board             |
/// |-------|---------------|-------------------------------|-------------------------------|
/// |    8  |               |  TMC260 step                  |                               |
/// |    9  |               |  TMC260 dir                   |                               |
/// |   10  |               |  TMC260 chipSelect            |                               |
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
/// |   21  |  <header 03>  |  vacuumclaner                 |
///
/// | pin   | header        | function                      | | pin   | header        | function                      |
/// |-------|---------------|-------------------------------|-|-------|---------------|-------------------------------|
/// |   22  |   A4988 01    |  stockStepper dir             | |   23  |   A4988 02    |  conveyorStepper enable       |
/// |   24  |   A4988 01    |  stockStepper step            | |   25  |   A4988 02    |  conveyorStepper microstep 00 |
/// |   26  |   A4988 01    |  stockStepper enable          | |   27  |   A4988 02    |  conveyorStepper microstep 01 |
/// |   28  |   A4988 01    |  stockStepper microstep 00    | |   29  |   A4988 02    |  conveyorStepper microstep 02 |
/// |   30  |   A4988 01    |  stockStepper microstep 01    | |   31  |   A4988 02    |  conveyorStepper step         |
/// |   32  |   A4988 01    |  stockStepper microstep 02    | |   33  |   A4988 02    |  conveyorStepper dir          |
/// |   34  |   DRV8825     |  tableStepper dir             | |   35  |               |  Controller Lamp yellow       |
/// |   36  |   DRV8825     |  tableStepper step            | |   37  |               |  Controller Lamp red          |
/// |   38  |   DRV8825     |  tableStepper enable          | |   39  |               |                               |
/// |   40  |   DRV8825     |  tableStepper microstep 00    | |   41  |               |                               |
/// |   42  |   DRV8825     |  tableStepper microstep 01    | |   43  |  <header 10>  |                               |
/// |   44  |   DRV8825     |  tableStepper microstep 02    | |   45  |  <header 10>  |  stockBottomButton            |
/// |   46  |               |                               | |   47  |  <header 10>  |  stockTopButton               |
/// |   48  |               |  songEndButton                | |   49  |  <header 10>  |  recordAvaliableButton        |
/// |   50  |               |  startCuttingButton           | |   51  |  <header 10>  |  bridgeEndButton              |
/// |   52  |               |  fetchRecordButton            | |   53  |  <header 10>  |  bridgeParkButton             |
///////////////////////////////////////////////////////////////////////////////////////////////////






//  ################## Hardware Cutting Parameters #########################################################

#define SONGGROOVE_PITCH                    0.2
#define STARTGROOVE_PITCH                   1
#define STARTGROOVE_RANGE                   4
#define ENDGROOVE_PITCH                     4.8
#define ENDGROOVE_RANGE_MIN                 10
#define CUTTING_RANGE                       38
#define PLAYTIME_MINUTES                    3

#define ADJUSTGROOVEPITCH_TO_PLAYTIME       false




//  ################## Hardware #############################################################################

#define STEPPER_STOCK_NAME                      "stockStepper"

#define STEPPER_STOCK_DIR_PIN                   22
#define STEPPER_STOCK_STEP_PIN                  24
#define STEPPER_STOCK_ENABLE_PIN                26
#define STEPPER_STOCK_MICROSTEPPINS             "28, 30, 32"

#define STEPPER_STOCK_HIGHEST_STEPPINGMODE      4
#define STEPPER_STOCK_STEPMODECODES             "0x00, 0x01, 0x02, 0x03, 0x07"
#define STEPPER_STOCK_STEPS_PER_ROTATION        48
#define STEPPER_STOCK_PPS_MAX                   800
#define STEPPER_STOCK_ACC_MAX                   2000



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



#define STEPPER_TABLE_NAME                      "tableStepper"

#define STEPPER_TABLE_DIR_PIN                   34
#define STEPPER_TABLE_STEP_PIN                  36
#define STEPPER_TABLE_ENABLE_PIN                38
#define STEPPER_TABLE_MICROSTEPPINS             "40, 42, 44"
#define STEPPER_TABLE_HIGHEST_STEPPINGMODE      5
#define STEPPER_TABLE_STEPMODECODES             "0x00, 0x01, 0x02, 0x03, 0x04, 0x05"
#define STEPPER_TABLE_STEPS_PER_ROTATION        48
#define STEPPER_TABLE_PPS_MAX                   800
#define STEPPER_TABLE_ACC_MAX                   2000



#define STEPPER_CAT_NAME                        "catStepper"

#define STEPPER_CAT_CHIPSELECT_PIN              10
#define STEPPER_CAT_DIR_PIN                     9
#define STEPPER_CAT_STEP_PIN                    8
#define STEPPER_CAT_ENABLE_PIN                  7
#define STEPPER_CAT_CURRENT                     1100
#define STEPPER_CAT_STEPS_PER_ROTATION          200
#define STEPPER_CAT_PPS_MAX                     1000
#define STEPPER_CAT_ACC_MAX                     4000



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

#define VACCUUMCLEANER_PIN                      21
#define VACCUUMCLEANER_ACTIV                    HIGH



#define CONTROLLER_LAMP_YELLOW_NAME             "yellowLamp"

#define CONTROLLER_LAMP_YELLOW_PIN              35
//#define CONTROLLER_LAMP_YELLOW_ACTIV            LOW
#define CONTROLLER_LAMP_YELLOW_ON               HIGH
#define CONTROLLER_LAMP_YELLOW_OFF              LOW



#define CONTROLLER_LAMP_RED_NAME                "redLamp"

#define CONTROLLER_LAMP_RED_PIN                 37
//#define CONTROLLER_LAMP_RED_ACTIV               LOW
#define CONTROLLER_LAMP_RED_ACTIV               HIGH



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
#define RECORDAVAILABLE_BUTTON_ACTIV        LOW
#define RECORDAVAILABLE_BUTTON_PULLUP       true

#define RECORDNOTAVAILABLE_BUTTON_NAME      "recordNotAvailableButton"
#define RECORDNOTAVAILABLE_BUTTON_ACTIV     HIGH

#define STOCKBOTTOM_BUTTON_NAME             "stockBottomButton"
#define STOCKBOTTOM_BUTTON_PIN              47
//#define STOCKBOTTOM_BUTTON_ACTIV            HIGH
#define STOCKBOTTOM_BUTTON_ACTIV            LOW
#define STOCKBOTTOM_BUTTON_PULLUP           true

#define STOCKTOP_BUTTON_NAME                "stockTopButton"
#define STOCKTOP_BUTTON_PIN                 45
//#define STOCKTOP_BUTTON_ACTIV               HIGH
#define STOCKTOP_BUTTON_ACTIV               LOW
#define STOCKTOP_BUTTON_PULLUP              true



#define START_CUTTING_BUTTON                50
#define FETCH_RECORD_BUTTON                 52

#define MOVE_MANUALLY_SWITCH                A3
#define MOVE_CAT_FWD_BUTTON                 A4
#define MOVE_CAT_RWD_BUTTON                 A5
#define MATCH_HEADIMPACT_SWITCH             A6
#define LOADING_BUTTON                      A7


#define HEAD_INCLINATION_SENSOR             A8






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
#define LIFT_UP_TRIGGER_TURN            1800
#define LIFT_TABLE_POSITION             1000
#define LIFT_STOCK_POSITION             1000
#define LIFT_CONVEYER_POSITION          1000
#define LIFT_PARK_POSITION              1000



#define TURN_SPEED_FAST                 320
#define TURN_SPEED_SLOW                 680
#define TURN_ACCEL_FAST                 280
#define TURN_ACCEL_SLOW                 180

#define TURN_TABLE_POSITION             1960
#define TURN_CONVEYOR_POSITION          1480
#define TURN_STOCK_POSITION             1054
#define TURN_TO_CONVEYER_TRIGGER_LIFT   TURN_CONVEYOR_POSITION + 60
#define TURN_TO_TABLE_TRIGGER_LIFT      TURN_TABLE_POSITION - 60
#define TURN_TO_STOCK_TRIGGER_LIFT      TURN_STOCK_POSITION + 60


#define HEAD_LEFT_PARK_POSITION         2100
#define HEAD_LEFT_CUT_POSITION          1260
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

#define CONVEYOR_DISTANCE               9600
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
/// @page sketchpage voiceOGraph.ino
/// voiceOGraph.ino is the main sketch to control the cutting machine
/// @code {.cpp}

/// #include <Arduino.h>
/// #include <Servo.h>
///
/// #include "voiceOGraph.h"
/// #include "CCDeviceScheduler.h"
///
///
/// #include <malloc.h>
///
///
///
///
///
/// // ------------- globals -----------------------------------------------------------------------------------------------------
/// CCDeviceScheduler   *cuttingProcess;
/// CCDeviceScheduler   *fetchingRecord;
/// CCDeviceScheduler   *ejectingRecord;
///
/// unsigned long       catSongStartPosition, catSongEndPosition, catCuttingEndPosition;
/// float               catMotorSpeed_startGroove, catMotorSpeed_song, catMotorSpeed_endGroove;
/// unsigned long       grooves_all;
/// float               turnTableStepperDegrees, turnTableStepperSpeed;
///
///
///
/// // ------------- prototypes --------------------------------------------------------------------------------------------------
/// void setup();
/// void calculateCuttingParameters();
///
/// void freeRam ();
///
/// extern char _end;
/// extern "C" char *sbrk(int i);
///
///
/// // ------------- main loop --------------------------------------------------------------------------------------------------
/// void loop() {
///
///     Serial.println(), Serial.println(), Serial.println();
///     for (int i = 0; i < 56; i++) Serial.print(F("#"));
///     Serial.println(), Serial.println(), Serial.println();
///
///
///     freeRam();
///
///     // ================================================================================================================================
///     // ============= create fetchingRecord scheduler object: ==========================================================================
///     // ================================================================================================================================
///
///     fetchingRecord = new CCDeviceScheduler("fetchingRecord");
///
///
///     {
///         // ============================================================================================================================
///         // ============= devices of fetchingRecord: ===================================================================================
///         // ============================================================================================================================
///
///         schedulerDevice liftServo = fetchingRecord->addServoWithCounterServo(SERVO_LIFT_NAME,
///                                                                              SERVO_LIFT_LEFT_PIN,
///                                                                              SERVO_LIFT_LEFT_MIN_POSITION,
///                                                                              SERVO_LIFT_LEFT_MID_POSITION,
///                                                                              SERVO_LIFT_LEFT_MAX_POSITION,
///                                                                              SERVO_LIFT_RIGHT_PIN,
///                                                                              SERVO_LIFT_RIGHT_MIN_POSITION,
///                                                                              SERVO_LIFT_RIGHT_MID_POSITION,
///                                                                              SERVO_LIFT_RIGHT_MAX_POSITION,
///                                                                              LIFT_PARK_POSITION);
///         fetchingRecord->device[liftServo]->defineDefaults(LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
///
///
///         schedulerDevice turnServo = fetchingRecord->addServo(SERVO_TURN_NAME,
///                                                              SERVO_TURN_PIN,
///                                                              SERVO_TURN_MIN_POSITION,
///                                                              SERVO_TURN_MAX_POSITION,
///                                                              TURN_STOCK_POSITION);
///         fetchingRecord->device[turnServo]->defineDefaults(TURN_SPEED_SLOW, TURN_ACCEL_SLOW);
///
///
///         schedulerDevice pumpServo = fetchingRecord->addServo(SERVO_PUMP_NAME,
///                                                              SERVO_PUMP_PIN,
///                                                              SERVO_PUMP_MIN_POSITION,
///                                                              SERVO_PUMP_MAX_POSITION,
///                                                              PUMP_PARK_POSITION);
///         fetchingRecord->device[pumpServo]->defineDefaults(PUMP_SPEED, PUMP_ACCEL);
///
///
///         schedulerDevice stockStepper = fetchingRecord->addStepper_A4988(STEPPER_STOCK_NAME,
///                                                                         STEPPER_STOCK_STEP_PIN,
///                                                                         STEPPER_STOCK_DIR_PIN,
///                                                                         STEPPER_STOCK_ENABLE_PIN,
///                                                                         STEPPER_STOCK_HIGHEST_STEPPINGMODE,
///                                                                         STEPPER_STOCK_STEPMODECODES,
///                                                                         STEPPER_STOCK_MICROSTEPPINS,
///                                                                         STEPPER_STOCK_STEPS_PER_ROTATION);
///         fetchingRecord->device[stockStepper]->defineDefaults(STOCK_SUPPLY_RECORD_SPEED, STOCK_SUPPLY_RECORD_ACCEL);
///
///
///
///
///         schedulerControlButton recordAvailableButton = fetchingRecord->addControlButton(RECORDAVAILABLE_BUTTON_NAME,
///                                                                                         RECORDAVAILABLE_BUTTON_PIN,
///                                                                                         RECORDAVAILABLE_BUTTON_ACTIV,
///                                                                                         RECORDAVAILABLE_BUTTON_PULLUP);
///
///         schedulerControlButton stockTopButton = fetchingRecord->addControlButton(STOCKTOP_BUTTON_NAME,
///                                                                                  STOCKTOP_BUTTON_PIN,
///                                                                                  STOCKTOP_BUTTON_ACTIV,
///                                                                                  STOCKTOP_BUTTON_PULLUP);
///
///         schedulerControlButton stockBottomButton = fetchingRecord->addControlButton(STOCKBOTTOM_BUTTON_NAME,
///                                                                                     STOCKBOTTOM_BUTTON_PIN,
///                                                                                     STOCKBOTTOM_BUTTON_ACTIV,
///                                                                                     STOCKBOTTOM_BUTTON_PULLUP);
///
///         fetchingRecord->listDevices();
///         fetchingRecord->listControlButtons();
///
///
///
///         // ============================================================================================================================
///         // ============= initialisation of fetchingRecord: ============================================================================
///         // ============================================================================================================================
///
///         //  not needed
///
///
///         // ============================================================================================================================
///         // ============= tasks of fetchingRecord: =====================================================================================
///         // ============================================================================================================================
///
///         //  lift grappler
///         scheduledTask liftFromParkPosition = fetchingRecord->device[liftServo]->addTask(LIFT_PARK_POSITION + 500, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
///         fetchingRecord->device[liftServo]->task[liftFromParkPosition]->startByDate(100);
///
///         //  move stock down first
///         scheduledTask moveStockStepperDown = fetchingRecord->device[stockStepper]->addTaskMoveRelativ(-4000);
///         fetchingRecord->device[stockStepper]->task[moveStockStepperDown]->startByDate(100);
///         fetchingRecord->device[stockStepper]->task[moveStockStepperDown]->stopByButton(stockBottomButton, STOP_NORMAL);
///
///         //  supply a new record, terminated by recordAvailableButton
///         scheduledTask supplyRecord = fetchingRecord->device[stockStepper]->addTaskMoveRelativ(14000);
///         fetchingRecord->device[stockStepper]->task[supplyRecord]->startAfterCompletionOf(stockStepper, moveStockStepperDown);
///         fetchingRecord->device[stockStepper]->task[supplyRecord]->stopByButton(recordAvailableButton, STOP_NORMAL);
///
///         //  lower grappler to stock: start when record is available
///         scheduledTask lowerToStock = fetchingRecord->device[liftServo]->addTask(LIFT_STOCK_POSITION);
///         fetchingRecord->device[liftServo]->task[lowerToStock]->startByButton(recordAvailableButton);
///
///         //  pump: prepare
///         scheduledTask pumpForGrip_down = fetchingRecord->device[pumpServo]->addTask(PUMP_DOWN_POSITION);
///         fetchingRecord->device[pumpServo]->task[pumpForGrip_down]->startByTriggerpositionOf(liftServo, lowerToStock, LIFT_STOCK_POSITION + 60);
///
///         //  pump: make vaccum
///         scheduledTask pumpForGrip_up = fetchingRecord->device[pumpServo]->addTask(PUMP_PARK_POSITION);
///         fetchingRecord->device[pumpServo]->task[pumpForGrip_up]->startAfterCompletionOf(pumpServo, pumpForGrip_down);
///
///         //  lift the new record
///         scheduledTask liftNewRecord = fetchingRecord->device[liftServo]->addTask(LIFT_UP_POSITION);
///         fetchingRecord->device[liftServo]->task[liftNewRecord]->startAfterCompletionOf(pumpServo, pumpForGrip_up);
///
///         //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
///         scheduledTask turnRecordToTable = fetchingRecord->device[turnServo]->addTask(TURN_TABLE_POSITION);
///         fetchingRecord->device[turnServo]->task[turnRecordToTable]->startAfterCompletionOf(liftServo, liftNewRecord);
///
///         //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
///         scheduledTask lowerRecordToTable = fetchingRecord->device[liftServo]->addTask(LIFT_TABLE_POSITION);
///         fetchingRecord->device[liftServo]->task[lowerRecordToTable]->startByTriggerpositionOf(turnServo, turnRecordToTable, TURN_TO_TABLE_TRIGGER_LIFT);
///
///         //  release new record: release vacuum
///         scheduledTask pumpForRelease_down = fetchingRecord->device[pumpServo]->addTask(PUMP_DOWN_POSITION);
///         fetchingRecord->device[pumpServo]->task[pumpForRelease_down]->startAfterCompletionOf(liftServo, lowerRecordToTable);
///
///         //  lift for going to park position: start when vacuum was released
///         scheduledTask liftForParkPosition = fetchingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
///         fetchingRecord->device[liftServo]->task[liftForParkPosition]->startAfterCompletionOf(pumpServo, pumpForRelease_down);
///
///         //  release pump when record is placed
///         scheduledTask pumpForRelease_up = fetchingRecord->device[pumpServo]->addTask(PUMP_PARK_POSITION);
///         fetchingRecord->device[pumpServo]->task[pumpForRelease_up]->startAfterCompletionOf(liftServo, liftForParkPosition);
///
///         //  turn grappler to park position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
///         scheduledTask turnToStockPosition = fetchingRecord->device[turnServo]->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
///         fetchingRecord->device[turnServo]->task[turnToStockPosition]->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
///
///         //  lower grappler to park position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
///         scheduledTask lowerForParkPosition = fetchingRecord->device[liftServo]->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
///         fetchingRecord->device[liftServo]->task[lowerForParkPosition]->startByTriggerpositionOf(turnServo, turnToStockPosition, TURN_TO_STOCK_TRIGGER_LIFT);
///
///
///
///         //  cancel loading if stockBottomButton is pressed
///         fetchingRecord->controlButton[stockTopButton]->evokeJumpToNextTask(stockStepper, supplyRecord, STOP);
///         fetchingRecord->controlButton[stockTopButton]->evokeJumpToTask(liftServo, liftFromParkPosition, STOP_SHARP_AND_SWITCH, lowerForParkPosition + 1);
///         fetchingRecord->controlButton[stockTopButton]->evokeJumpToTask(turnServo, turnRecordToTable, STOP_SHARP_AND_SWITCH, turnToStockPosition + 1);
///
///
///
///         fetchingRecord->reviewTasks();
///         fetchingRecord->getAllTasks();
///         fetchingRecord->getAllActions();
///
///
///         freeRam();
///     }
///
///
///
///
///
///     // ================================================================================================================================
///     // ============= create cuttingProcess scheduler object: ==========================================================================
///     // ================================================================================================================================
///
///     cuttingProcess = new CCDeviceScheduler("cuttingProcess");
///
///
///     {
///         // ============================================================================================================================
///         // ============= devices of cuttingProcess: ===================================================================================
///         // ============================================================================================================================
///
///         schedulerDevice catStepper = cuttingProcess->addStepper_TMC260(STEPPER_CAT_NAME,
///                                                                        STEPPER_CAT_STEP_PIN,
///                                                                        STEPPER_CAT_DIR_PIN,
///                                                                        STEPPER_CAT_ENABLE_PIN,
///                                                                        STEPPER_CAT_CHIPSELECT_PIN,
///                                                                        STEPPER_CAT_CURRENT,
///                                                                        STEPPER_CAT_STEPS_PER_ROTATION);
///         cuttingProcess->device[catStepper]->defineDefaults(CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
///
///         schedulerDevice tableStepper = cuttingProcess->addStepper_A4988(STEPPER_TABLE_NAME,
///                                                                         STEPPER_TABLE_STEP_PIN,
///                                                                         STEPPER_TABLE_DIR_PIN,
///                                                                         STEPPER_TABLE_ENABLE_PIN,
///                                                                         STEPPER_TABLE_HIGHEST_STEPPINGMODE,
///                                                                         STEPPER_TABLE_STEPMODECODES,
///                                                                         STEPPER_TABLE_MICROSTEPPINS,
///                                                                         STEPPER_TABLE_STEPS_PER_ROTATION);
///
///         schedulerDevice headLeftServo = cuttingProcess->addServo(SERVO_HEAD_LEFT_NAME,
///                                                                  SERVO_HEAD_LEFT_PIN,
///                                                                  SERVO_HEAD_LEFT_MIN_POSITION,
///                                                                  SERVO_HEAD_LEFT_MAX_POSITION,
///                                                                  HEAD_LEFT_PARK_POSITION);
///
///         schedulerDevice headRightServo = cuttingProcess->addServo(SERVO_HEAD_RIGHT_NAME,
///                                                                   SERVO_HEAD_RIGHT_PIN,
///                                                                   SERVO_HEAD_RIGHT_MIN_POSITION,
///                                                                   SERVO_HEAD_RIGHT_MAX_POSITION,
///                                                                   HEAD_RIGHT_PARK_POSITION);
///
///
///
///
///
///         schedulerControlButton bridgeParkButton = cuttingProcess->addControlButton(CAT_PARKBUTTON_NAME,
///                                                                                    CAT_PARKBUTTON_PIN,
///                                                                                    CAT_PARKBUTTON_ACTIV,
///                                                                                    CAT_PARKBUTTON_PULLUP);
///
///         schedulerControlButton bridgeEndButton = cuttingProcess->addControlButton(CAT_ENDBUTTON_NAME,
///                                                                                   CAT_ENDBUTTON_PIN,
///                                                                                   CAT_ENDBUTTON_ACTIV,
///                                                                                   CAT_ENDBUTTON_PULLUP);
///
///         schedulerControlButton songEndButton = cuttingProcess->addControlButton(SONG_ENDBUTTON_NAME,
///                                                                                 SONG_ENDBUTTON_PIN,
///                                                                                 SONG_ENDBUTTON_ACTIV,
///                                                                                 SONG_ENDBUTTON_PULLUP);
///
///         schedulerControlButton songCancelButton = cuttingProcess->addControlButton(SONG_CANCELBUTTON_NAME,
///                                                                                    SONG_CANCELBUTTON_PIN,
///                                                                                    SONG_CANCELBUTTON_ACTIV,
///                                                                                    SONG_CANCELBUTTON_PULLUP);
///
///
///         cuttingProcess->listDevices();
///         cuttingProcess->listControlButtons();
///
///
///
///         // ============================================================================================================================
///         // ============= initialisation of cuttingProcess: =====================================================================================
///         // ============================================================================================================================
///
///
///
///         Serial.println("................................. initialisation of catStepper ...................................");
///
///
///         scheduledTask initCatStepper = cuttingProcess->device[catStepper]->addTaskWithPositionReset(-400000);
///         cuttingProcess->device[catStepper]->task[initCatStepper]->startByDate(100);
///         cuttingProcess->device[catStepper]->task[initCatStepper]->stopByButton(bridgeParkButton, STOP_NORMAL);
///
///         cuttingProcess->reviewTasks();
///         cuttingProcess->getAllTasks();
///
///
///         cuttingProcess->run();
///
///         cuttingProcess->device[catStepper]->setCurrentPosition(0.0);
///
///         cuttingProcess->deleteAllTasks();
///         cuttingProcess->deleteAllActions();
///
///
///         Serial.println("...................................... done ......................................................");
///
///
///
///
///
///         // ============================================================================================================================
///         // ============= tasks of cuttingProcess: =====================================================================================
///         // ============================================================================================================================
///
///         //  move to start groove:
///         scheduledTask driveToCuttingStartPosition = cuttingProcess->device[catStepper]->addTask(CAT_CUTTING_START_POSITION);
///         cuttingProcess->device[catStepper]->task[driveToCuttingStartPosition]->startByDate(100);
///
///         //  turn the table:
///         scheduledTask turnTheTable = cuttingProcess->device[tableStepper]->addTaskWithPositionReset(turnTableStepperDegrees + 7200.0, turnTableStepperSpeed, TABLE_STEP_ACCEL);
///         cuttingProcess->device[tableStepper]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
///
///         //  lower head close to record surface (left servo): start when reached start position of start groove
///         scheduledTask lowerHeadLeftForCutting = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
///         cuttingProcess->device[headLeftServo]->task[lowerHeadLeftForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
///
///         //  approximate head to record surface (right servo): start when left servo reached cutting position
///         scheduledTask approximateHeadRightForCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
///         cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
///         cuttingProcess->device[headRightServo]->task[approximateHeadRightForCutting]->stopDynamicallyBySensor(HEAD_INCLINATION_SENSOR, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
///
///         //  make start groove:
///         scheduledTask makeStartGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_startGroove, CAT_ACCEL_SLOW);
///         cuttingProcess->device[catStepper]->task[makeStartGroove]->startAfterCompletionOf(headRightServo, approximateHeadRightForCutting);
///         cuttingProcess->device[catStepper]->task[makeStartGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeStartGroove, catSongStartPosition);
///
///         //  make song groove:
///         scheduledTask makeMainGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_song, CAT_ACCEL_SLOW);
///         cuttingProcess->device[catStepper]->task[makeMainGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition);
///
///         //  make end groove:
///         scheduledTask makeEndGroove = cuttingProcess->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_endGroove, CAT_ACCEL_NORMAL);
///
///         //  lift head after cutting above the record surface:
///         scheduledTask liftHeadRightAfterCutting = cuttingProcess->device[headRightServo]->addTask(HEAD_RIGHT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW);
///         cuttingProcess->device[headRightServo]->task[liftHeadRightAfterCutting]->startAfterCompletionOf(catStepper, makeEndGroove);
///
///         //  lift head into park position:
///         scheduledTask liftHeadLeftAfterCutting = cuttingProcess->device[headLeftServo]->addTask(HEAD_LEFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW);
///         cuttingProcess->device[headLeftServo]->task[liftHeadLeftAfterCutting]->startAfterCompletionOf(headRightServo, liftHeadRightAfterCutting);
///
///         //  switch off turn table
///         cuttingProcess->device[tableStepper]->task[turnTheTable]->stopAfterCompletionOf(headRightServo, liftHeadRightAfterCutting, STOP_NORMAL);
///
///         //  drive cat back home
///         scheduledTask driveToParkPosition = cuttingProcess->device[catStepper]->addTask(CAT_PARK_POSITION);
///         cuttingProcess->device[catStepper]->task[driveToParkPosition]->startAfterCompletionOf(headRightServo, liftHeadRightAfterCutting);
///         cuttingProcess->device[catStepper]->task[driveToParkPosition]->stopByButton(bridgeParkButton, STOP_NORMAL);
///
///
///
///         //  jump over to end groove if songEndButton is pressed
///         cuttingProcess->controlButton[songEndButton]->evokeJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, makeEndGroove);
///
///         //  cancel cutting if songCancelButton is pressed
///         cuttingProcess->controlButton[songCancelButton]->evokeJumpToTask(catStepper, makeStartGroove, STOP_AND_SWITCH, driveToParkPosition);
///         cuttingProcess->controlButton[songCancelButton]->evokeJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, driveToParkPosition);
///         cuttingProcess->controlButton[songCancelButton]->evokeJumpToNextTask(catStepper, makeEndGroove, STOP);
///
///
///
///         cuttingProcess->reviewTasks();
///         cuttingProcess->getAllTasks();
///         cuttingProcess->getAllActions();
///
///
///         freeRam();
///
///     }
///
///
///
///     // ================================================================================================================================
///     // ============= create ejectingRecord scheduler object: ==========================================================================
///     // ================================================================================================================================
///
///     ejectingRecord = new CCDeviceScheduler("ejectingRecord");
///
///
///     {
///         // ============================================================================================================================
///         // ============= devices of ejectingRecord: ===================================================================================
///         // ============================================================================================================================
///
///         schedulerDevice liftServo = ejectingRecord->addServoWithCounterServo(SERVO_LIFT_NAME,
///                                                                              SERVO_LIFT_LEFT_PIN,
///                                                                              SERVO_LIFT_LEFT_MIN_POSITION,
///                                                                              SERVO_LIFT_LEFT_MID_POSITION,
///                                                                              SERVO_LIFT_LEFT_MAX_POSITION,
///                                                                              SERVO_LIFT_RIGHT_PIN,
///                                                                              SERVO_LIFT_RIGHT_MIN_POSITION,
///                                                                              SERVO_LIFT_RIGHT_MID_POSITION,
///                                                                              SERVO_LIFT_RIGHT_MAX_POSITION,
///                                                                              LIFT_PARK_POSITION);
///         ejectingRecord->device[liftServo]->defineDefaults(LIFT_SPEED_SLOW, LIFT_ACCEL_SLOW);
///
///
///         schedulerDevice turnServo = ejectingRecord->addServo(SERVO_TURN_NAME,
///                                                              SERVO_TURN_PIN,
///                                                              SERVO_TURN_MIN_POSITION,
///                                                              SERVO_TURN_MAX_POSITION,
///                                                              TURN_STOCK_POSITION);
///         ejectingRecord->device[turnServo]->defineDefaults(TURN_SPEED_SLOW, TURN_ACCEL_SLOW);
///
///
///         schedulerDevice pumpServo = ejectingRecord->addServo(SERVO_PUMP_NAME,
///                                                              SERVO_PUMP_PIN,
///                                                              SERVO_PUMP_MIN_POSITION,
///                                                              SERVO_PUMP_MAX_POSITION,
///                                                              PUMP_PARK_POSITION);
///         ejectingRecord->device[pumpServo]->defineDefaults(PUMP_SPEED, PUMP_ACCEL);
///
///
///         schedulerDevice conveyStepper = ejectingRecord->addStepper_A4988(STEPPER_CONVEYOR_NAME,
///                                                                          STEPPER_CONVEYOR_STEP_PIN,
///                                                                          STEPPER_CONVEYOR_DIR_PIN,
///                                                                          STEPPER_CONVEYOR_ENABLE_PIN,
///                                                                          STEPPER_CONVEYOR_HIGHEST_STEPPINGMODE,
///                                                                          STEPPER_CONVEYOR_STEPMODECODES,
///                                                                          STEPPER_CONVEYOR_MICROSTEPPINS,
///                                                                          STEPPER_CONVEYOR_STEPS_PER_ROTATION);
///         ejectingRecord->device[conveyStepper]->defineDefaults(CONVEYOR_SPEED, CONVEYOR_ACCEL);
///
///
///
///
///         ejectingRecord->listDevices();
///         ejectingRecord->listControlButtons();
///
///
///
///         // ============================================================================================================================
///         // ============= initialisation of ejectingRecord: ============================================================================
///         // ============================================================================================================================
///
///         //  not needed
///
///
///         // ============================================================================================================================
///         // ============= tasks of ejectingRecord: =====================================================================================
///         // ============================================================================================================================
///
///         //  remove record from turntable: started after cuttingProcess is finished
///
///         // lift grappler: start when cuttingProcess is finished
///         scheduledTask liftFromParkPosition = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
///         ejectingRecord->device[liftServo]->task[liftFromParkPosition]->startByDate(100);
///
///         //  turn grappler to turn table: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
///         scheduledTask turnToTable = ejectingRecord->device[turnServo]->addTask(TURN_TABLE_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
///         ejectingRecord->device[turnServo]->task[turnToTable]->startByTriggerpositionOf(liftServo, liftFromParkPosition, LIFT_UP_TRIGGER_TURN);
///
///         //  lower grappler to turn table: start when turning reached trigger position (TURN_TO_TABLE_TRIGGER_LIFT)
///         scheduledTask lowerToTable = ejectingRecord->device[liftServo]->addTask(LIFT_TABLE_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
///         ejectingRecord->device[liftServo]->task[lowerToTable]->startByTriggerpositionOf(turnServo, turnToTable, TURN_TO_TABLE_TRIGGER_LIFT);
///
///         // pump prepare
///         scheduledTask pumpForGrip_down = ejectingRecord->device[pumpServo]->addTask(PUMP_DOWN_POSITION);
///         ejectingRecord->device[pumpServo]->task[pumpForGrip_down]->startAfterCompletionOf(liftServo, lowerToTable);
///
///         // pump make vaccum
///         scheduledTask pumpForGrip_up = ejectingRecord->device[pumpServo]->addTask(PUMP_PARK_POSITION);
///         ejectingRecord->device[pumpServo]->task[pumpForGrip_up]->startAfterCompletionOf(pumpServo, pumpForGrip_down);
///
///         //  lift the cutted record: start with startDelay after table was reached (LIFT_TABLE_POSITION)
///         scheduledTask liftCuttedRecord = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION);
///         ejectingRecord->device[liftServo]->task[liftCuttedRecord]->startAfterCompletionOf(pumpServo, pumpForGrip_up);
///
///         //  turn grappler to conveyer position: start when lifting reached triggerPosition (LIFT_UP_TRIGGER_TURN)
///         scheduledTask turnToConveyor = ejectingRecord->device[turnServo]->addTask(TURN_CONVEYOR_POSITION);
///         ejectingRecord->device[turnServo]->task[turnToConveyor]->startByTriggerpositionOf(liftServo, liftCuttedRecord, LIFT_UP_TRIGGER_TURN);
///
///         //  lower grappler to conveyer position: start when turning reached trigger position (TURN_TO_PARK_TRIGGER_LIFT)
///         scheduledTask lowerToConveyor = ejectingRecord->device[liftServo]->addTask(LIFT_CONVEYER_POSITION);
///         ejectingRecord->device[liftServo]->task[lowerToConveyor]->startByTriggerpositionOf(turnServo, turnToConveyor, TURN_TO_CONVEYER_TRIGGER_LIFT);
///
///         //  release new record, release vacuum
///         scheduledTask pumpForRelease_down = ejectingRecord->device[pumpServo]->addTask(PUMP_DOWN_POSITION);
///         ejectingRecord->device[pumpServo]->task[pumpForRelease_down]->startAfterCompletionOf(liftServo, lowerToConveyor);
///
///         //  lift for parking
///         scheduledTask liftForParkPosition = ejectingRecord->device[liftServo]->addTask(LIFT_UP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
///         ejectingRecord->device[liftServo]->task[liftForParkPosition]->startAfterCompletionOf(liftServo, lowerToConveyor);
///
///         //  move it to the user
///         scheduledTask driveIt = ejectingRecord->device[conveyStepper]->addTaskWithPositionReset(CONVEYOR_DISTANCE, CONVEYOR_SPEED, CONVEYOR_ACCEL);
///         ejectingRecord->device[conveyStepper]->task[driveIt]->startAfterCompletionOf(liftServo, liftForParkPosition);
///
///         //  release pump
///         scheduledTask pumpForRelease_up = ejectingRecord->device[pumpServo]->addTask(PUMP_PARK_POSITION);
///         ejectingRecord->device[pumpServo]->task[pumpForRelease_up]->startAfterCompletionOf(liftServo, liftForParkPosition);
///
///         //  turn grappler for parking
///         scheduledTask turnToStock = ejectingRecord->device[turnServo]->addTask(TURN_STOCK_POSITION, TURN_SPEED_FAST, TURN_ACCEL_FAST);
///         ejectingRecord->device[turnServo]->task[turnToStock]->startByTriggerpositionOf(liftServo, liftForParkPosition, LIFT_UP_TRIGGER_TURN);
///
///         //  lower grappler to stock: start when turning reached trigger position (TURN_TO_STOCK_TRIGGER_LIFT)
///         scheduledTask lowerToPark = ejectingRecord->device[liftServo]->addTask(LIFT_PARK_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST);
///         ejectingRecord->device[liftServo]->task[lowerToPark]->startByTriggerpositionOf(turnServo, turnToStock, TURN_TO_STOCK_TRIGGER_LIFT);
///
///
///
///         ejectingRecord->reviewTasks();
///         ejectingRecord->getAllTasks();
///         ejectingRecord->getAllActions();
///
///
///         freeRam();
///
///     }
///
///
///
///
///
///
///
///
///
///
///     // ============================================================================================================================
///     // ============================================================================================================================
///     // ============= operation: ===================================================================================================
///     // ============================================================================================================================
///     // ============================================================================================================================
///
///
///     Serial.println(F("ready to run!"));
///
///     boolean initNeeded = false;
///     while (!initNeeded) {
///
///         if (digitalRead(FETCH_RECORD_BUTTON) == LOW) {
///
///             fetchingRecord->run();
///
///         }
///
///
///         if (digitalRead(START_CUTTING_BUTTON) == LOW) {
///
///             cuttingProcess->run();
///
///             ejectingRecord->run();
///
///         }
///
///         if (digitalRead(MOVE_MANUALLY_SWITCH) == LOW) {
///             {
///                 CCDeviceScheduler *manualDrive = new CCDeviceScheduler("manualDrive");
///
///                 schedulerDevice catStepper = manualDrive->addStepper_TMC260(STEPPER_CAT_NAME,
///                                                                             STEPPER_CAT_STEP_PIN,
///                                                                             STEPPER_CAT_DIR_PIN,
///                                                                             STEPPER_CAT_ENABLE_PIN,
///                                                                             STEPPER_CAT_CHIPSELECT_PIN,
///                                                                             STEPPER_CAT_CURRENT,
///                                                                             STEPPER_CAT_STEPS_PER_ROTATION);
///                 manualDrive->device[catStepper]->defineDefaults(CAT_SPEED_HIGH, CAT_ACCEL_HIGH);
///
///                 schedulerControlButton catForwardButton = manualDrive->addControlButton("catForward", MOVE_CAT_FWD_BUTTON, HIGH, true);
///                 schedulerControlButton catRewindButton = manualDrive->addControlButton("catRewind", MOVE_CAT_RWD_BUTTON, HIGH, true);
///
///
///                 //  move to start groove:
///                 scheduledTask driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(CAT_CUTTING_START_POSITION);
///                 manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(100);
///
///                 manualDrive->reviewTasks();
///                 manualDrive->run();
///                 manualDrive->deleteAllTasks();
///
///                 while (digitalRead(START_CUTTING_BUTTON) == HIGH) {
///                     if (digitalRead(MOVE_CAT_FWD_BUTTON) == LOW) {
///                         driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(catCuttingEndPosition);
///                         manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
///                         manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(catForwardButton, STOP_NORMAL);
///
///                         manualDrive->reviewTasks();
///                         manualDrive->run();
///                         manualDrive->deleteAllTasks();
///                     }
///                     if (digitalRead(MOVE_CAT_RWD_BUTTON) == LOW) {
///                         driveToCuttingStartPositionMan = manualDrive->device[catStepper]->addTask(CAT_PARK_POSITION);
///                         manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->startByDate(10);
///                         manualDrive->device[catStepper]->task[driveToCuttingStartPositionMan]->stopByButton(catRewindButton, STOP_NORMAL);
///
///                         manualDrive->reviewTasks();
///                         manualDrive->run();
///                         manualDrive->deleteAllTasks();
///                     }
///                 }
///
///
///                 cuttingProcess->device[0]->setCurrentPosition(manualDrive->device[0]->getCurrentPosition());
///                 cuttingProcess->device[0]->increaseTaskPointer();
///                 cuttingProcess->run();
///
///                 delete manualDrive;
///                 manualDrive = NULL;
///
///             }
///         }
///
///         if (digitalRead(MATCH_HEADIMPACT_SWITCH) == LOW) {
///
///             cuttingProcess->device[0]->setCountOfTasks(0);
///             cuttingProcess->device[1]->setCountOfTasks(0);
///             cuttingProcess->device[3]->task[1]->setStartDelay(3000);
///
///             while (digitalRead(MATCH_HEADIMPACT_SWITCH == LOW)) {
///
///                 cuttingProcess->run();
///
///                 delay(3000);
///
///             }
///             initNeeded = true;
///         }
///     }
///
///
///     delay(2000);
///
///
///     delete fetchingRecord;
///     fetchingRecord = NULL;
///
///     delete cuttingProcess;
///     cuttingProcess = NULL;
///
///     delete ejectingRecord;
///     ejectingRecord = NULL;
///
///
///     freeRam();
///
///     delay(2000);
///
/// }
///
/// void calculateCuttingParameters() {
///
///     unsigned int songRange = CUTTING_RANGE - STARTGROOVE_RANGE - ENDGROOVE_RANGE_MIN;
///     unsigned int songGrooves = songRange / (float)SONGGROOVE_PITCH;
///     unsigned int playTime_seconds = songGrooves * 60 / RECORD_TURNS_PER_MINUTE;
///     unsigned int endGrooveRange = ENDGROOVE_RANGE_MIN;
///
///
///
///     Serial.println();
///     Serial.println("################################################################################");
///     Serial.println("############################## CUTTING PARAMETERS ##############################");
///     Serial.println("################################################################################");
///     Serial.println();
///
///     Serial.print(F("groove pitch is set to "));
///     Serial.print(SONGGROOVE_PITCH);
///     Serial.print(F("mm, start groove pitch is set to "));
///     Serial.print(STARTGROOVE_PITCH);
///     Serial.print(F("mm, start groove width is set to "));
///     Serial.print(STARTGROOVE_RANGE);
///     Serial.print(F("mm, end groove pitch is set to "));
///     Serial.print(ENDGROOVE_PITCH);
///     Serial.print(F("mm, minimal end groove width is set to "));
///     Serial.print(ENDGROOVE_RANGE_MIN);
///     Serial.println(F("mm"));
///     Serial.print(F("max. song width: "));
///     Serial.print(songRange);
///     Serial.print(F("mm, max. song grooves: "));
///     Serial.print(songGrooves);
///     Serial.print(F(", max. playing time: "));
///     Serial.print(playTime_seconds / 60);
///     Serial.print(F(":"));
///     Serial.println(playTime_seconds % 60);
///
///
///     if ((float)PLAYTIME_MINUTES * 60 < playTime_seconds) {
///         songGrooves = PLAYTIME_MINUTES * RECORD_TURNS_PER_MINUTE;
///         songRange = songGrooves * SONGGROOVE_PITCH;
///         endGrooveRange = CUTTING_RANGE - songRange - STARTGROOVE_RANGE;
///         playTime_seconds = PLAYTIME_MINUTES * 60;
///
///         Serial.print(F("playing time is set to "));
///         Serial.print(playTime_seconds / 60);
///         Serial.print(F(":"));
///         Serial.print(playTime_seconds % 60);
///         Serial.print(F(", song width: "));
///         Serial.print(songRange);
///         Serial.print(F("mm, record grooves: "));
///         Serial.println(songGrooves);
///     } else {
///         Serial.println(F("desired playing time is too long, using maximal playing time!"));
///     }
///
///
///     grooves_all = STARTGROOVE_RANGE / (float)STARTGROOVE_PITCH + songGrooves + endGrooveRange / (float)ENDGROOVE_PITCH;
///     turnTableStepperDegrees = grooves_all * TABLE_DRIVE_RATIO * 360.0;
///     //    turnTableStepperSpeed = RECORD_TURNS_PER_MINUTE / 60.0 * TABLE_DRIVE_RATIO * 360.0;
///     turnTableStepperSpeed = 6.0 * RECORD_TURNS_PER_MINUTE * TABLE_DRIVE_RATIO;
///
///
///     float catSpeed_start_mmPerSecond = STARTGROOVE_PITCH * RECORD_TURNS_PER_MINUTE / 60.0;
///     float catMotorDegrees_start = 360.0 * STARTGROOVE_RANGE / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
///     catMotorSpeed_startGroove = 360.0 * catSpeed_start_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
///
///     float catSpeed_song_mmPerSecond = SONGGROOVE_PITCH * RECORD_TURNS_PER_MINUTE / 60.0;
///     float catMotorDegrees_song = 360.0 * songRange / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
///     catMotorSpeed_song = 360.0 * catSpeed_song_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
///
///     float catSpeed_end_mmPerSecond = ENDGROOVE_PITCH * RECORD_TURNS_PER_MINUTE / 60.0;
///     float catMotorDegrees_end = 360.0 * endGrooveRange / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
///     catMotorSpeed_endGroove = 360.0 * catSpeed_end_mmPerSecond / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
///
///
///     catSongStartPosition = CAT_CUTTING_START_POSITION + catMotorDegrees_start;
///     catSongEndPosition = catSongStartPosition + catMotorDegrees_song;
///     catCuttingEndPosition = CAT_CUTTING_START_POSITION + CUTTING_RANGE * 360.0 / SPIN_PITCH_M6 / (float)(CAT_DRIVE_RATIO);
///
///
///     Serial.print("Cutting the start groove: cutting progress [mm/sec]: ");
///     Serial.print(catSpeed_start_mmPerSecond);
///     Serial.print(", degrees: ");
///     Serial.print(catMotorDegrees_start);
///     Serial.print(", degrees per second: ");
///     Serial.println(catMotorSpeed_startGroove);
///
///     Serial.print("Cutting the song: cutting progress [mm/sec]: ");
///     Serial.print(catSpeed_song_mmPerSecond);
///     Serial.print(", degrees: ");
///     Serial.print(catMotorDegrees_song);
///     Serial.print(", degrees per second: ");
///     Serial.println(catMotorSpeed_song);
///
///     Serial.print("Cutting the end groove: cutting progress [mm/sec]: ");
///     Serial.print(catSpeed_end_mmPerSecond);
///     Serial.print(", degrees: ");
///     Serial.print(catMotorDegrees_end);
///     Serial.print(", degrees per second: ");
///     Serial.println(catMotorSpeed_endGroove);
///
///     Serial.print("Turning the table: table rotations: ");
///     Serial.print(grooves_all);
///     Serial.print(", motor degrees: ");
///     Serial.print(turnTableStepperDegrees);
///     Serial.print(", degrees per second: ");
///     Serial.println(turnTableStepperSpeed);
///
///     Serial.println();
///     Serial.println("################################################################################");
///     Serial.println();
///     Serial.println();
///
/// }
///
/// void setup() {
///
///     pinMode(FETCH_RECORD_BUTTON, INPUT_PULLUP);
///     pinMode(START_CUTTING_BUTTON, INPUT_PULLUP);
///     pinMode(MATCH_HEADIMPACT_SWITCH, INPUT_PULLUP);
///     pinMode(MOVE_MANUALLY_SWITCH, INPUT_PULLUP);
///     pinMode(MOVE_CAT_FWD_BUTTON, INPUT_PULLUP);
///     pinMode(MOVE_CAT_RWD_BUTTON, INPUT_PULLUP);
///
///     pinMode(I_AM_LATE_LED, OUTPUT);
///
///     Serial.begin(115200);
///     Serial.println(), Serial.println(), Serial.println();
///     Serial.println(), Serial.println(), Serial.println();
///
///     calculateCuttingParameters();
///
/// }
///
/// void freeRam() {
///     char *ramstart = (char *) 0x20070000;
///     char *ramend = (char *) 0x20088000;
///     char *heapend = sbrk(0);
///     register char * stack_ptr asm( "sp" );
///     struct mallinfo mi = mallinfo();
///     Serial.print("Ram used (bytes): dynamic: ");
///     Serial.print(mi.uordblks);
///     Serial.print("  static:  ");
///     Serial.print(&_end - ramstart);
///     Serial.print("  stack:   ");
///     Serial.print(ramend - stack_ptr);
///     Serial.print(", estimation free Ram: ");
///     Serial.println(stack_ptr - heapend + mi.fordblks);
/// }
///
///
///
/// @endcode
///////////////////////////////////////////////////////////////////////////////////////////////////





