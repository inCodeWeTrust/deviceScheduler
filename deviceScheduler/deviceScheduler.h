//
//  deviceScheduler.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__deviceScheduler__
#define __deviceScheduler__deviceScheduler__


#include <Arduino.h>
#include <avr/pgmspace.h>


//  ################## PROGRAMMATICAL DEFINITIONS AND CONSTANTS #############################################################################

typedef signed char schedulerDevice, scheduledTask, schedulerControlButton;

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
    BREAK_LOOP,
    START_NEXT_TASK,
    STOP_TASK_AND_SWITCH,
    STOP_TASK,
    STOP_TASK_SHARP_AND_SWITCH,
    STOP_TASK_SHARP
};

//  stop mode:
enum stoppingMode {
    STOP_IMMEDIATELY,
    STOP_NORMAL,
    STOP_DYNAMIC
};

//  notification code:
enum notificationCode {
    BREAK_ON_ENDBUTTON_PRESSED = -0x22,
    ENDBUTTON_PRESSED = 0x22
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



#define I_AM_LATE_LED                       12




#endif // defined(__deviceScheduler__deviceScheduler__)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @page coding Coding Tutorial
/// @section createScheduler Creating a new scheduler
/// Zentrales Objekt ist der Scheduler, eine Instanz der Klasse mit dem Namen CCDeviceScheduler. Er steuert und koordiniert einen Arbeitsablauf, d.h. verschiedene Geräte führen einen Block von Aufgaben aus. Es können beliebig viele dieser Arbeitsabläufe definiert und in beliebiger Reihenfolge abgerufen werden. Zum Beispiel beim Plattenschneideautomat @c fetchNewRecord, @c cuttingProcess, @c ejectRecord etc.
/// @code {.cpp}
/// CCDeviceScheduler *cuttingProcess = new CCDeviceScheduler("cutting");
/// @endcode
/// Ein neues Scheduler-Objekt mit dem Namen @c cuttingProcess wird erzeugt. Im logging erscheint ess mit dem Namen @c cutting. Es soll z.B. die komplette Steuerung des Schneideprozesses übernehmen.
/// <br><br><br>
/// @section registerDevice Registering Devices
/// Für die Geräte existiert eine Basisklasse CCDevice. Der Scheduler enthält ein Array dieser Basisklasse mit dem namen @c device, in das alle Geräte eingetragen werden. Es können beliebig viele Geräte zum Scheduler hinzugefügt, d.h. in dieses Array eingetragen werden. Dies geschieht durch die Funktionen @c addServo(...), @c addStepper(...), @c addDcController(...) etc. des Schedulers, in der sämtliche Parameter dieses Geräts übergeben werden:
/// @code{.cpp}
/// cuttingProcess->addServo("lift", 12, 800, 2100, 1800);
/// @endcode
/// Ein neues Device-Objekt vom Typ Modellbauservo wird dem Device-Array des Schedulers hinzugefügt. Übergeben wird ein Name, mit dem dieses Servo im Ablaufprotokoll erscheint, hier @c lift, der Pin, mit dem es gesteuert wird, ein minimaler und ein maximaler Positionswert und die Parkposition.
/// Rückgabewert dieser Funktion ist der Index dieses Gerätes im Geräte-Array @c device des Schedulers. Es ist ein ganzzahliger Wert, sein Typname ist @c schedulerDevice. Mit diesem Geräteindex ist das Gerät dann eindeutig identifizierbar.
/// Zur besseren Übersichtlichkeit können die Parameter freilich als Makros in einem Headerfile notiert werden.
/// Die Registrierung eines Geräts kann dann zB. so aussehen:
/// @code{.cpp}
/// cuttingProcess->addServo(SERVO_NAME, SERVO_PIN, SERVO_MIN_POSITION, SERVO_MAX_POSITION, SERVO_PARK_POSITION);
/// @endcode
/// Wird dieser Wert in einer aussagekräftigen Variable gespeichert, ist es sehr einfach, dieses Gerät anzusprechen:
/// @code{.cpp}
/// schedulerDevice liftServo = cuttingProcess->addServo(LIFTSERVO_NAME, LIFTSERVO_PIN, LIFTSERVO_MIN_POSITION, LIFTSERVO_MAX_POSITION, LIFTSERVO_PARK_POSITION);
///
/// cuttingProcess->device[liftServo]...
/// @endcode
/// Die implementierten Methoden sind:
/// - @code addServo(...) @endcode
/// - @code addServoWithCounterServo(...) @endcode
/// - @code addDcController(...) @endcode
/// - @code addStepper_A4988(...) @endcode
/// - @code addStepper_TMC260(...) @endcode
/// <br><br>
/// @section registerTask Registering Tasks
/// Zu diesen Geräten des Schedulers können nun beliebig viele Aufgaben hinzugefügt werden. Für die Aufgaben existiert eine Klasse mit dem Namen CCTask. Die Klasse CCDevice enthält ein Aufgaben-Array mit dem Namen @c task. Die Eintragung von Aufgaben in dieses Array bewerkstelligt die Funktion @c addTask(...) des jeweiligen Gerätes: Übergeben werden sämtliche Parameter dieser Aufgabe:
/// @code{.cpp}
/// scheduledTask lowerHeadForCutting = cuttingProcess->device[liftServo]->addTask(CUTING_POSITION, SPEED_SLOW, ACCELERATION_NORMAL, DECELERATION_SLOW);
/// @endcode
/// Dem Gerät @c liftServo wird eine neue Aufgabe hinzugefügt: Fahre zur Position @c CUTTING_POSITION mit der Geschwindigkeit @c SPEED_SLOW, der Beschleunigung @c ACCELERATION_NORMAL bzw. @c DECELERATION_SLOW. Die Aufgabe startet ohne Startverzögerung.
/// Rückgabewert dieser Funktion ist wiederum der Index dieser Aufgabe im Aufgaben-Array, mit dem diese Aufgabe eindeutig identifiziert ist. Dieser Wert wird wiederum in einer aussagekräftigen Variable gespeichert, damit sie später leicht angesprochen werden kann. <br>
/// Die implementierten Methoden sind:
/// - @code addTask(...) @endcode
/// - @code addTaskMoveRelativ(...) @endcode
/// - @code addTaskWithPositionReset(...) @endcode
/// <br><br>
/// @section LinkTask Linking Tasks Together
/// Gestartet, gestoppt und manipuliert werden können die Geräte durch Zeitablauf, durch die Zustandsänderung angeschlossener Sensoren, durch das Erreichen eines Zustandes bzw. einer Position dieses Gerätes sowie durch das Erreichen eines Zustandes bzw. einer Position jedes anderen Gerätes.
/// @code{.cpp}
/// cuttingProcess->device[catStepper]->task[driveCatToCuttingStartPosition]->startByDate(100);
/// @endcode
/// Die Aufgabe @c driveCatToCuttingStartPosition des Gerätes @c catStepper soll 100 ms nachdem der Scheduler gestartet wurde, ausgeführt werden.
/// @code{.cpp}
/// cuttingProcess->device[liftServo]->task[lowerHeadForCutting]->startAfterCompletionOf(catStepper, driveCatToCuttingStartPosition);
/// @endcode
/// Die Aufgabe @c lowerHeadForCutting wird gestartet, nachdem die Aufgabe @c driveCatToCuttingStart des Schneideschlittenantriebs (@c catStepper) abgeschlossen ist.
/// @code{.cpp}
/// cuttingProcess->device[liftServo]->task[lowerHeadForCutting]->stopDynamicallyBySensor(DISTANCE_SENSOR, INITIATE_STOP_VALUE, TARGET_VALUE, STOP_PERFORMANCE, SKIP_APPROXIMATION_PRECISE);
/// @endcode
/// Die Aufgabe "lowerHeadForCutting" wird entsprechend der Sensorwerte des Distanzsensors im Schneidekopf dynamisch gestoppt. Die Fahrgeschwindigkeit wird entsprechend der Sensorwerte geregelt. <br>
/// Die implementierten Methoden sind:
/// - @code startByDate(...) @endcode
/// - @code startByButton(...) @endcode
/// - @code startAfterCompletionOf(...) @endcode
/// - @code startByTriggerpositionOf(...) @endcode
/// - @code switchToNextTaskByDate(...) @endcode
/// - @code switchToNextTaskByButton(...) @endcode
/// - @code switchToNextTaskAfterCompletionOf(...) @endcode
/// - @code switchToNextTaskByTriggerpositionOf(...) @endcode
/// - @code stopByTimeout(...) @endcode
/// - @code stopByButton(...) @endcode
/// - @code stopAfterCompletionOf(...) @endcode
/// - @code stopByTriggerpositionOf(...) @endcode
/// - @code stopDynamicallyBySensor(...) @endcode
/// <br><br>
/// @section registerActions Registering Action Buttons
/// Zu dem Scheduler können nun noch beliebig viele Bedienungselemente hinzugefügt werden. Mittels dieser Schalter kann die Abarbeitung der Aufgaben manipuliert werden. Aufgaben können  gestoppt, übersprungen oder wiederholt werden. In der Klasse CCDeviceScheduler existiert ein Array der Klasse CCControlButton. Dort werden die Schalter eingetragen:
/// @code{.cpp}
/// schedulerControlButton songEndButton = cuttingProcess->addControlButton(SONG_END_BUTTON_NAME, SONG_END_PIN, SONG_END_ACTIV);
/// @endcode
/// Jedes Element dieser Klasse enthält ein Array von @c actions, die das Stoppen, Überspringen oder Wiederholen von Aufgaben veranlassen. Die Eintragung dieser @c actions bewerkstelligt die Funktion @c evokeTaskJump(...) bzw. @c evokeTaskJumpToTask(...). Übergeben werden sämtliche Parameter dieser Aufgabe:
/// @code{.cpp}
/// cuttingProcess->controlButton[songEndButton]->evokeTaskJumpToTask(catStepper, makeMainGroove, STOP_TASK_AND_SWITCH, makeEndGroove);
/// @endcode
/// Die Aufgabe @c makeMainGroove des Schneideschlittenantriebs @c catStepper, also der Schneideprozess, soll abgebrochen werden, wenn der aufzunehmende Song bereits vor dem Ablauf der eingestellten Aufnahmedauer zu ende ist. Dies wird mit dem Schalter @c songEndButton signalisiert. Der Schneideschlittenantrieb soll dann die Aufgabe @c makeMainGroove beenden und direkt zur Aufgabe @c makeEndGroove wechseln, also übergangslos zu der Geschwindigkeit beschleunigen, die zum Schneiden der Endrille notwendig ist. <br>
/// Die implementierten Methoden sind:
/// - @code evokeTaskJump(...) @endcode
/// - @code evokeTaskJumpToTask(...) @endcode
/// <br><br>
/// @section run Run the Process
/// Um den nun fertig konfigurierten Prozess zu starten dient der Befehl @c run.
/// @code{.cpp}
/// cuttingProcess->run();
/// @endcode
/// <br><br>
