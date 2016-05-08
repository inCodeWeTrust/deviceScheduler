//
//  CCDeviceScheduler.h
//  deviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCDeviceScheduler__
#define __deviceScheduler__CCDeviceScheduler__


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "deviceScheduler.h"

#include "CCDevice.h"
#include "CCServoDevice.h"
#include "CCStepperDevice.h"
#include "CCStepperDevice_A4988.h"
#include "CCStepperDevice_TMC260.h"
#include "CCDcControllerDevice.h"

#include "CCControlButton.h"



///////////////////////////////////////////////////////////////////////////////////////////////////
/// @mainpage	deviceScheduler
///
/// Diese Software wurde anlässlich des Baus eines Schallplattenschneide-Automaten entwickelt. Es ist ein Kunstprojekt. Dieser Apparat, der Voice-o-graph, ist eine Art Passbild-Automat, nur dass darin nicht das Bild, sondern der Ton analog aufgezeichnet wird. In einer kleinen Kabine hat man die Möglichkeit, 3 min Ton aufzunehmen, der simultan in einen 7inch-Schallplattenrohling geschnitten wird. Nachdem man also einen Gruß an einen Menschen in der Ferne, ein Liebesgedicht, seinen Lieblings-Popsong oder vielleicht ein großes Geheimnis ins Mikrofon gesprochen oder gesungen hat, erhält man es als Vinyl-Schallplatte. \n
/// Das gesamte Gerät wird durch einen ARM7-Controller gesteuert. Erstellt habe ich es auf einem Apple-Computer mit der IDE XCode und dem embedXcode+ plugin. Der Upload wird mit der Arduino-Toolchain bewerkstelligt.\n
/// Das Steuerungsprogramm ist ein Scheduler, der Geräte wie z.B. Stepper-Motoren, Modellbau-Servos, Gleichstrom-Motoren, Elektromagnete etc. starten, stoppen und führen kann. Diese Software ist in der Programmiersprache C++ geschrieben und nutzt einerseits deren Betriebssystemnähe und Schnelligkeit, andererseits deren Möglichkeit, objektorientiert zu programmieren: sie nutzt das Konzept der Klassen und der Polymorphie. Die Ansteuerung der Geräte wie z.B. die Erzeugung der Steuerimpulse für die Modellbau-Servos oder die Taktung der Schrittmotoren erfolgt ausschließlich in der Klasse der Geräte, während die Koordinierung des Arbeitsablaufes allein die Aufgabe der Schedulerklasse ist. Diese Trennung von Steuerung und Koordinierung bringt erhebliche Vorteile mit sich: der Code wird wesentlich reduzierter, übersichtlicher und strukturierter, wodurch Programmierfehler vermieden werden können bzw. schneller auffindbar werden. Der Code ist leicht lesbar und verständlich und an verschiedenste Anforderungen einfach anpassbar. Durch die Anwendung von aussagekräftigen Variablennamen wird die klare Struktur zusätzlich optimiert. Es können prinzipiell beliebig viele Geräte gleichzeitig gesteuert und überwacht und prinzipiell beliebig viele Aufgaben abgearbeitet und untereinander koordiniert werden. Programmiert ist sie wie eine Library, wodurch sie sehr leicht für andere Anlagen und Szenarios adaptiert werden kann. \n \n
///
/// @author		Christoph Freidhöfer
/// @author		(http://kunst-und-raederwerk.net)
/// @date		20.04.2016
///
/// @copyright	(c) little Ikarus, 2015
///
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCDeviceScheduler
///
/// @brief Scheduler engine to manage all devices and their tasks.
///
/// The scheduler holds all devices and controls them by calling their startTask(), operateTask(), stopTask() etc. routines.
///
///////////////////////////////////////////////////////////////////////////////////////////////////



class CCDeviceScheduler {
    
public:

    /// The Scheduler's device array.
    /// Array of all devices.
    CCDevice *device[8];
    /// Array of all control-inputs.
    CCControlButton *controlButton[8];
    
    /// Constructor creates a scheduler object
    CCDeviceScheduler();
    
    /// Destructor deletes the scheduler object.
    ~CCDeviceScheduler();
    


    /// Function adds a servo device to the device array and returns the index of the device. Any type of servo motor can be used, that works with a pulswidth of 600 thru 2400us at a pulsing frequency of 50Hz.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param servo_pin the pin number of the servo's pwm-pin.
    /// @param minPosition the minimum PCM pulse width in microseconds corresponding to one end position.
    /// @param maxPosition the maximum PCM pulse width in microseconds corresponding to other end position.
    /// @param parkPosition the PCM pulse width in microseconds corresponding to the default position.
    /// @return the device index to identify this device.
    schedulerDevice addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition);
    
    /// Function adds a stepper device to the device array and returns the index of the device. This routine is used for generic stepper driver circuits like A4988 or DRV7888. If micro stepping is available, the stepping mode is selected by microstepping pins.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param dir_pin the pin number of the stepper driver's direction pin.
    /// @param step_pin the pin number of the stepper driver's step pin.
    /// @param enable_pin the pin number of the stepper driver's enable pin.
    /// @param highestSteppingMode the number of available stepping modes (full step, half step, quarter step ...).
    /// @param stepModeCodesString a string containing the specific pin combinations for the different stepping modes.
    /// @param microStepPinsString a string containing the pin numbers of the micro stepping pins.
    /// i.e. "28, 30, 32": micro stepping pins are pin 28, pin 30 and pin 32
    /// @param stepsPerRotation the number of steps needed to make a full rotation.
    /// @return the device index to identify this device.
    schedulerDevice addStepper_A4988(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, unsigned int stepsPerRotation);
   
    
    /// Function adds a stepper device to the device array and returns the index of the device. Use this method to register a stepper driver circuit, that is controlled by SPI.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param dir_pin the pin number of the stepper driver's direction pin.
    /// @param step_pin the pin number of the stepper driver's step pin.
    /// @param enable_pin the pin number of the stepper driver's enable pin.
    /// @param chipSelect_pin the pin number of the stepper driver's chip select (CS) pin, to be addressed for SPI communication.
    /// @param currentMax the maximum coil current [in mA RMS].
    /// @param stepsPerRotation the number of steps needed to make a full rotation.
    /// @return the device index to identify this device.
    schedulerDevice addStepper_TMC260(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char chipSelect_pin, unsigned int currentMax, unsigned int stepsPerRotation);
    
    
    /// Function adds a dc-controller device to the device array and returns the index of the device.
    /// A dc-controller device is a device, that is simply switched on or off, respectivly driven by a PWM-pulse. Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param switching_pin the pin number of the device's controll pin.
    /// @param switchingPin_activ the state of the switching pin, where the device is activ.
    /// @return the device index.
    schedulerDevice addDcController(String deviceName, unsigned char switching_pin, boolean switchingPin_activ);

  
    


    /// Function lists all registered devices.
    /// A list with all devices and bare informations are presented.
    void getAllDevices();
    
    /// Function lists all tasks of all registered devices.
    /// A list with all tasks and bare informations are presented.
    void getAllTasks();
    
    /// Function lists all tasks of the specified device.
    /// A list with all tasks and bare informations are presented.
    void getTasksForDevice(schedulerDevice theDevice);
    
    /// Function calls the <reviewValues()> function of all devices.
    /// This causes the devices to approve, if all tasks are possible and do some basic calculations.
    /// This function needs to be called before the run loop is started.
    void reviewTasks();

    /// Function deletes all tasks of all registered devices.
    void deleteAllTasks();
    
    
    
    
    
    
    /// Function adds a control button to the control button array and returns the index of the button.
    /// A control button is a input device, that can provide either a HIGH or a LOW level at a input pin or simply connect the pin and GND using the internal inputPullup-function. Specific parameters are passed.
    /// @param buttonName the human-readable name of the device (used for verbose output).
    /// @param button_pin the pin number of the button's pin.
    /// @param button_activ the state of the pin, where the button should trigger actions. If "LOW" is passed, the input pullup is activated.
    /// @return the button index.
    schedulerControlButton addControlButton(String buttonName, unsigned char button_pin, boolean button_activ);

    
    /// Function lists all registered control buttons.
    /// A list with all buttons and bare informations are presented.
    void getAllControlButtons();
    
    /// Function lists all actions of all registered buttons.
    /// A list with all actions and bare informations are presented.
    void getAllActions();
    
    
    /// Function lists all actions of the specified button.
    /// A list with all actions and bare informations are presented.
    void getActionsForControlButton(unsigned char theButton);

    /// Function deletes all actions of all registered buttons.
    void deleteAllActions();
    
    
    /// Function starts the scheduler, all registered tasks are driven as specified.
    void run();
    

};



#endif /* defined(__deviceScheduler__CCDeviceScheduler__) */



/// \page page1 Coding Tutorial
/// \section createScheduler Creating a new scheduler
/// Zentrales Objekt ist der Scheduler, eine Instanz der Klasse mit dem Namen CCDeviceScheduler. Er steuert und koordiniert einen Arbeitsablauf, d.h. verschiedene Geräte führen einen Block von Aufgaben aus. Es können beliebig viele dieser Arbeitsabläufe definiert und in beliebiger Reihenfolge abgerufen werden. Zum Beispiel beim Plattenschneideautomat \c fetchNewRecord, \c cuttingProcess, \c ejectRecord etc.
/// \code {.cpp}
/// CCDeviceScheduler cuttingProcess = new CCDeviceScheduler;
/// \endcode
/// Ein neues Scheduler-Objekt mit dem Namen \c cuttingProcess wird erzeugt. Es soll z.B. die komplette Steuerung des Schneideprozesses übernehmen.
/// <br><br><br>
/// \section registerDevice Registering Devices
/// Für die Geräte existiert eine Basisklasse CCDevice. Der Scheduler enthält ein Array dieser Basisklasse mit dem namen \c device, in das alle Geräte eingetragen werden. Es können beliebig viele Geräte zum Scheduler hinzugefügt, d.h. in dieses Array eingetragen werden. Dies geschieht durch die Funktionen \c addServo(...), \c addStepper(...), \c addDcController(...) etc. des Schedulers, in der sämtliche Parameter dieses Geräts übergeben werden:
/// \code{.cpp}
/// cuttingProcess->addServo(SERVO_NAME, SERVO_PIN, SERVO_MIN_POSITION, SERVO_MAX_POSITION, SERVO_PARK_POSITION);
/// \endcode
/// Ein neues Device-Objekt vom Typ Modellbauservo wird dem Device-Array des Schedulers hinzugefügt. Übergeben wird ein Name, mit dem dieses Servo im Ablaufprotokoll erscheint, der Pin, mit dem es gesteuert wird, ein minimaler und ein maximaler Positionswert und die Parkposition.
/// Rückgabewert dieser Funktion ist der Index dieses Gerätes im Geräte-Array \c device des Schedulers. Es ist ein ganzzahliger Wert, sein Typname ist \c schedulerDevice. Mit diesem Geräteindex ist das Gerät dann eindeutig identifizierbar. Wird dieser Wert in einer aussagekräftigen Variable gespeichert, ist es sehr einfach, dieses Gerät anzusprechen:
/// \code{.cpp}
/// schedulerDevice liftServo = cuttingProcess->addServo(LIFTSERVO_NAME, LIFTSERVO_PIN, LIFTSERVO_MIN_POSITION, LIFTSERVO_MAX_POSITION, LIFTSERVO_PARK_POSITION);
///
/// cuttingProcess->device[liftServo]...
/// \endcode
/// Die implementierten Methoden sind:
/// - \code CCDeviceScheduler->addServo(...) \endcode
/// - \code CCDeviceScheduler->addDcController(...) \endcode
/// - \code CCDeviceScheduler->addStepper_A4988(...) \endcode
/// - \code CCDeviceScheduler->addStepper_TMC260(...) \endcode
/// <br><br>
/// \section registerTask Registering Tasks
/// Zu diesen Geräten des Schedulers können nun beliebig viele Aufgaben hinzugefügt werden. Für die Aufgaben existiert eine Klasse mit dem Namen CCTask. Die Klasse CCDevice enthält ein Aufgaben-Array mit dem Namen \c task. Die Eintragung von Aufgaben in dieses Array bewerkstelligt die Funktion \c addTask(...) des jeweiligen Gerätes: Übergeben werden sämtliche Parameter dieser Aufgabe:
/// \code{.cpp}
/// scheduledTask lowerHeadForCutting = cuttingProcess->device[liftServo]->addTask(CUTING_POSITION, SPEED_SLOW, ACCELERATION_NORMAL, DECELERATION_SLOW);
/// \endcode
/// Dem Gerät \c liftServo wird eine neue Aufgabe hinzugefügt: Fahre zur Position \c CUTTING_POSITION mit der Geschwindigkeit \c SPEED_SLOW, der Beschleunigung \c ACCELERATION_NORMAL bzw. \c DECELERATION_SLOW. Die Aufgabe startet ohne Startverzögerung.
/// Rückgabewert dieser Funktion ist wiederum der Index dieser Aufgabe im Aufgaben-Array, mit dem diese Aufgabe eindeutig identifiziert ist. Dieser Wert wird wiederum in einer aussagekräftigen Variable gespeichert, damit sie später leicht angesprochen werden kann. <br>
/// Die implementierten Methoden sind:
/// - \code CCDevice->addTask(...) \endcode
/// - \code CCDevice->addTaskWithStartDelay(...) \endcode
/// <br><br>
/// \section LinkTask Linking Tasks Together
/// Gestartet, gestoppt und manipuliert werden können die Geräte durch Zeitablauf, durch die Zustandsänderung angeschlossener Sensoren, durch das Erreichen eines Zustandes bzw. einer Position dieses Gerätes sowie durch das Erreichen eines Zustandes bzw. einer Position jedes anderen Gerätes.
/// \code{.cpp}
/// cuttingProcess->device[catStepper]->task[driveCatToCuttingStartPosition]->startByDate(100);
/// \endcode
/// Die Aufgabe \c driveCatToCuttingStartPosition des Gerätes \c catStepper soll 100 ms nachdem der Scheduler gestartet wurde, ausgeführt werden.
/// \code{.cpp}
/// cuttingProcess->device[liftServo]->task[lowerHeadForCutting]->startAfterCompletionOf(catStepper, driveCatToCuttingStartPosition);
/// \endcode
/// Die Aufgabe \c lowerHeadForCutting wird gestartet, nachdem die Aufgabe \c driveCatToCuttingStart des Schneideschlittenantriebs (\c catStepper) abgeschlossen ist.
/// \code{.cpp}
/// cuttingProcess->device[liftServo]->task[lowerHeadForCutting]->stopDynamicallyBySensor(DISTANCE_SENSOR, INITIATE_STOP_VALUE, TARGET_VALUE, STOP_PERFORMANCE, SKIP_APPROXIMATION_PRECISE);
/// \endcode
/// Die Aufgabe "lowerHeadForCutting" wird entsprechend der Sensorwerte des Distanzsensors im Schneidekopf dynamisch gestoppt. Die Fahrgeschwindigkeit wird entsprechend der Sensorwerte geregelt. <br>
/// Die implementierten Methoden sind:
/// - \code CCTask->startByDate(...) \endcode
/// - \code CCTask->startByButton(...) \endcode
/// - \code CCTask->startAfterCompletionOf(...) \endcode
/// - \code CCTask->startByTriggerpositionOf(...) \endcode
/// - \code CCTask->switchToNextTaskByDate(...) \endcode
/// - \code CCTask->switchToNextTaskByButton(...) \endcode
/// - \code CCTask->switchToNextTaskAfterCompletionOf(...) \endcode
/// - \code CCTask->switchToNextTaskByTriggerpositionOf(...) \endcode
/// - \code CCTask->stopByTimeout(...) \endcode
/// - \code CCTask->stopByButton(...) \endcode
/// - \code CCTask->stopAfterCompletionOf(...) \endcode
/// - \code CCTask->stopByTriggerpositionOf(...) \endcode
/// - \code CCTask->stopDynamicallyBySensor(...) \endcode
/// <br><br>
/// \section registerActions Registering Action Buttons
/// Zu dem Scheduler können nun noch beliebig viele Bedienungselemente hinzugefügt werden. Mittels dieser Schalter kann die Abarbeitung der Aufgaben manipuliert werden. Aufgaben können  gestoppt, übersprungen oder wiederholt werden. In der Klasse CCDeviceScheduler existiert ein Array der Klasse CCControlButton. Dort werden die Schalter eingetragen:
/// \code{.cpp}
/// schedulerControlButton songEndButton = cuttingProcess->addControlButton(SONG_END_BUTTON_NAME, SONG_END_PIN, SONG_END_ACTIV);
/// \endcode
/// Jedes Element dieser Klasse enthält ein Array von \c actions, die das Stoppen, Überspringen oder Wiederholen von Aufgaben veranlassen. Die Eintragung dieser \c actions bewerkstelligt die Funktion \c evokeTaskJump(...) bzw. \c evokeTaskJumpToTask(...). Übergeben werden sämtliche Parameter dieser Aufgabe:
/// \code{.cpp}
/// cuttingProcess->controlButton[songEndButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, makeEndGroove);
/// \endcode
/// Die Aufgabe \c makeMainGroove des Schneideschlittenantriebs \c catStepper, also der Schneideprozess, soll abgebrochen werden, wenn der aufzunehmende Song bereits vor dem Ablauf der eingestellten Aufnahmedauer zu ende ist. Dies wird mit dem Schalter \c songEndButton signalisiert. Der Schneideschlittenantrieb soll dann die Aufgabe \c makeMainGroove beenden und direkt zur Aufgabe \c makeEndGroove wechseln, also übergangslos zu der Geschwindigkeit beschleunigen, die zum Schneiden der Endrille notwendig ist. <br>
/// Die implementierten Methoden sind:
/// - \code CCControlButton->evokeTaskJump(...) \endcode
/// - \code CCControlButton->evokeTaskJumpToTask(...) \endcode
/// <br><br>
/// \section run Run the Process
/// Um den nun fertig konfigurierten Prozess zu starten dient der Befehl \c run.
/// \code{.cpp}
/// cuttingProcess->run();
/// \endcode
/// <br><br>
/// \page page2 Example Code
/// \htmlinclude exampleCode.html

/*
/// \page page3 My Code
/// \code{.cpp}
///
///
/// scheduler = new CCDeviceScheduler;
///
///
/// // ============================================================================================================================
/// // ============= devices ======================================================================================================
/// // ============================================================================================================================
///
/// schedulerDevice headLeftServo = scheduler->addServo(SERVO_HEAD_LEFT_NAME, SERVO_HEAD_LEFT_PIN, SERVO_HEAD_LEFT_MIN_POSITION, SERVO_HEAD_LEFT_MAX_POSITION, HEAD_LEFT_PARK_POSITION);
/// schedulerDevice headRightServo = scheduler->addServo(SERVO_HEAD_RIGHT_NAME, SERVO_HEAD_RIGHT_PIN, SERVO_HEAD_RIGHT_MIN_POSITION, SERVO_HEAD_RIGHT_MAX_POSITION, HEAD_RIGHT_PARK_POSITION);
/// schedulerDevice tableStepper = scheduler->addStepper_A4988(STEPPER_TABLE_NAME, STEPPER_TABLE_STEP_PIN, STEPPER_TABLE_DIR_PIN, STEPPER_TABLE_ENABLE_PIN, STEPPER_TABLE_HIGHEST_STEPPINGMODE, STEPPER_TABLE_STEPMODECODES, STEPPER_TABLE_MICROSTEPPINS, STEPPER_TABLE_STEPS_PER_ROTATION);
/// schedulerDevice catStepper = scheduler->addStepper_TMC260(STEPPER_CAT_NAME, STEPPER_CAT_STEP_PIN, STEPPER_CAT_DIR_PIN, STEPPER_CAT_ENABLE_PIN, STEPPER_CAT_CHIPSELECT_PIN, STEPPER_CAT_CURRENT, STEPPER_CAT_STEPS_PER_ROTATION);
///
///
/// scheduler->getAllDevices();
///
///
///
///
/// // ============================================================================================================================
/// // ============== control buttons =============================================================================================
/// // ============================================================================================================================
///
/// schedulerControlButton songEndButton = scheduler->addControlButton(SONG_END_BUTTON_NAME, SONG_END_PIN, SONG_END_ACTIV);
/// schedulerControlButton songCancelButton = scheduler->addControlButton(SONG_CANCEL_BUTTON_NAME, SONG_CANCEL_PIN, SONG_CANCEL_ACTIV);
///
///
/// scheduler->getAllControlButtons();
///
///
///
///
///
/// Serial.println("................................. initialisation .................................");
///
/// // ============================================================================================================================
/// // ============= initialisation tasks =========================================================================================
/// // ============================================================================================================================
///
/// scheduledTask initCatStepper = scheduler->device[catStepper]->addTask(-CAT_FULLRANGE, CAT_SPEED_VERY_HIGH, CAT_ACCEL_VERY_HIGH, CAT_ACCEL_VERY_HIGH);
/// scheduler->device[catStepper]->task[initCatStepper]->startByDate(100);
/// scheduler->device[catStepper]->task[initCatStepper]->stopByButton(CAT_PARK_BUTTON, HIGH, STOP_NORMAL);
///
///
/// scheduler->reviewTasks();
/// scheduler->getAllTasks();
///
///
///
/// // ============================================================================================================================
/// // ============== run initialisation ==========================================================================================
/// // ============================================================================================================================
///
/// scheduler->run();
///
///
///
/// // ============================================================================================================================
/// // ============== cleaning up =================================================================================================
/// // ============================================================================================================================
///
/// scheduler->deleteAllTasks();
/// scheduler->deleteAllActions();
///
///
/// Serial.println("...................................... done ......................................");
///
///
///
///
///
/// // ============================================================================================================================
/// // ============= main tasks ===================================================================================================
/// // ============================================================================================================================
///
/// //  move to start groove:
/// scheduledTask driveToCuttingStartPosition = scheduler->device[catStepper]->addTask(CAT_CUTTING_START_POSITION, CAT_SPEED_HIGH, CAT_ACCEL_HIGH, CAT_ACCEL_HIGH);
/// scheduler->device[catStepper]->task[driveToCuttingStartPosition]->startByDate(100);
/// scheduler->device[catStepper]->task[driveToCuttingStartPosition]->stopByButton(CAT_END_BUTTON, HIGH, STOP_NORMAL);
///
/// //  turn the table:
/// scheduledTask turnTheTable = scheduler->device[tableStepper]->addTask(grooves_all * 3600, turnTableStepperSpeed, TABLE_STEP_ACCEL, TABLE_STEP_ACCEL);
/// scheduler->device[tableDrive]->task[turnTheTable]->startByTriggerpositionOf(catStepper, driveToCuttingStartPosition, CAT_CUTTING_START_POSITION - 10000);
///
/// //  lower head to record surface: start when reached start position of start groove
/// scheduledTask lowerHeadLeftForCutting = scheduler->device[headLeftServo]->addTask(HEAD_LEFT_CUT_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_FAST, LIFT_ACCEL_FAST);
/// scheduler->device[headLeftServo]->task[lowerHeadLeftForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
///
/// //  lower head to record surface: start when reached start position of start groove
/// scheduledTask lowerHeadRightForCutting = scheduler->device[headRightServo]->addTask(HEAD_RIGHT_CUT_POSITION, LIFT_SPEED_VERY_SLOW, LIFT_ACCEL_VERY_SLOW, LIFT_ACCEL_VERY_SLOW);
/// scheduler->device[headRightServo]->task[lowerHeadRightForCutting]->startAfterCompletionOf(catStepper, driveToCuttingStartPosition);
/// scheduler->device[headRightServo]->task[lowerHeadRightForCutting]->stopDynamicallyBySensor(A5, 600, 460, 0.6, SKIP_APPROXIMATION_PRECISE);
///
/// //  make the start groove: start when cutting head reached cutting position
/// scheduledTask makeStartGroove = scheduler->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_startGroove, CAT_ACCEL_SLOW, CAT_ACCEL_SLOW);
/// scheduler->device[catStepper]->task[makeStartGroove]->startAfterCompletionOf(headRightServo, lowerHeadRightForCutting);
/// scheduler->device[catStepper]->task[makeStartGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeStartGroove, catSongStartPosition);
///
/// //  make the main groove, cut the song: change head speed to appropriate song-cutting-speed when start groove is done
/// scheduledTask makeMainGroove = scheduler->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_song, CAT_ACCEL_SLOW, CAT_ACCEL_SLOW);
/// scheduler->device[catStepper]->task[makeMainGroove]->switchToNextTaskByTriggerpositionOf(catStepper, makeMainGroove, catSongEndPosition);
///
/// //  make the end groove: change head speed to end groove speed, when song is done
/// scheduledTask makeEndGroove = scheduler->device[catStepper]->addTask(catCuttingEndPosition, catMotorSpeed_endGroove, CAT_ACCEL_NORMAL, CAT_ACCEL_NORMAL);
///
/// //  lift head after cutting: start when end groove is finished
/// scheduledTask liftHeadLeftAfterCutting = scheduler->device[headLeftServo]->addTask(HEAD_LEFT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
/// scheduler->device[headLeftServo]->task[liftHeadLeftAfterCutting]->startAfterCompletionOf(catStepper, makeEndGroove);
///
/// //  stop turning the table:
/// scheduler->device[tableStepper]->task[turnTheTable]->stopAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting, STOP_NORMAL);
///
/// //  lift head complete after cutting: start when other servo finished
/// scheduledTask liftHeadRightAfterCutting = scheduler->device[headRightServo]->addTask(HEAD_RIGHT_TOP_POSITION, LIFT_SPEED_FAST, LIFT_ACCEL_SLOW, LIFT_ACCEL_SLOW);
/// scheduler->device[headRightServo]->task[liftHeadRightAfterCutting]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
///
/// //  drive to park position after all
/// scheduledTask driveToParkPosition = scheduler->device[catStepper]->addTask(CAT_PARK_POSITION, CAT_SPEED_HIGH, CAT_ACCEL_HIGH, CAT_ACCEL_HIGH);
/// scheduler->device[catStepper]->task[driveToParkPosition]->startAfterCompletionOf(headLeftServo, liftHeadLeftAfterCutting);
/// scheduler->device[catStepper]->task[driveToParkPosition]->stopByButton(CAT_PARK_BUTTON, HIGH, STOP_NORMAL);
///
///
/// //  do the end groove when the button is pressed
/// scheduler->controlButton[songEndButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, makeEndGroove);
///
/// //  cancel recording, when button is pressed
/// scheduler->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeStartGroove, STOP_AND_SWITCH, driveToParkPosition);
/// scheduler->controlButton[songCancelButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_AND_SWITCH, driveToParkPosition);
/// scheduler->controlButton[songCancelButton]->evokeTaskJump(catStepper, makeEndGroove, STOP);
///
///
///
/// scheduler->reviewTasks();
/// scheduler->getAllTasks();
///
/// scheduler->getAllControlButtons();
/// scheduler->getAllActions();
///
///
///
/// // ============================================================================================================================
/// // ============== run it ======================================================================================================
/// // ============================================================================================================================
///
///
///
/// scheduler->run();
///
/// scheduler->deleteAllTasks();
/// scheduler->deleteAllActions();
///
/// \endcode
*/

