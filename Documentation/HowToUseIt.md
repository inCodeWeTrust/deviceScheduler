<h1>Prinzipielle Programmierung</h1>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* create a scheduler object */
cuttingProcess = new CCDeviceScheduler;


/* add the devices */
unsigned char catStepper = cuttingProcess->addStepper(CATSTEPPER_NAME, CATSTEPPER_STEP_PIN, CATSTEPPER_DIR_PIN, CATSTEPPER_ENABLE_PIN, CATSTEPPER_HIGHEST_STEPPINGMODE, CATSTEPPER_STEPMODECODES, CATSTEPPER_MICROSTEPPINS, CATSTEPPER_STEPS_PER_ROTATION);
unsigned char liftServo = cuttingProcess->addServo(LIFTSERVO_NAME, LIFTSERVO_PIN, LIFTSERVO_MIN_POSITION, LIFTSERVO_MAX_POSITION, LIFTSERVO_PARK_POSITION);
	...


/* add the tasks */
unsigned char lowerHeadForCutting = cuttingProcess->device[liftServo]->addTask(CUTING_POSITION, SPEED_SLOW, ACCELERATION_NORMAL, DECELERATION_SLOW);
unsigned char driveCatToCuttingStartPosition = cuttingProcess->device[catStepper]->addTask(CUTTING_START_POSITION, SPEED_HIGH, ACCELERATION_HIGH, DECELERATION_HIGH);


	...


/* link the tasks together */
cuttingProcess->device[catStepper]->task[driveCatToCuttingStartPosition]->startByDate(100);
cuttingProcess->device[liftServo]->task[lowerHeadForCutting]->startAfterCompletionOf(catStepper, driveCatToCuttingStartPosition);
cuttingProcess->device[liftServo]->task[lowerHeadForCutting]->stopDynamicallyBySensor(DISTANCE_SENSOR, INITIATE_STOP_VALUE, TARGET_VALUE, ATTENUATION, STOP_VERY_PRECISE);
	...


/* run the tasks */
cuttingProcess->run();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  
  
    
# Funktionsweise
Zentrales Objekt ist der Scheduler, eine Instanz der Klasse mit dem Namen CCDeviceScheduler. Er steuert und koordiniert einen Arbeitsablauf, d.h. verschiedene Geräte führen einen Block von Aufgaben aus. Es können beliebig viele dieser Arbeitsabläufe definiert und in beliebiger Reihenfolge abgerufen werden. Zum Beispiel „fetchNewRecord“, „cuttingProcess“, „ejectRecord“ etc.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cuttingProcess = new CCDeviceScheduler;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Ein neues Scheduler-Objekt mit dem Namen „cuttingProcess“ wird erzeugt. Es soll die komplette Steuerung des Schneideprozesses übernehmen.*

<br>

Für die Geräte existiert eine Basisklasse „CCDevice“ bzw. die eigens entwickelten abgeleiteten Klassen „CCServoDevice“, „CCStepperDevice“ „CCSwitchingDevice“. Als abgeleitete Geräteklassen haben sie alle das gleiche Interface, verfügen also alle über die gleichen Methoden, z.B. „prepareMove()“, „startMove()“, „drive()“, „stopMove()“ etc. Sie sind insofern alle auf die gleiche Weise zu handhaben, wenngleich die Implementierung dieser Funktionen freilich in jeder abgeleiteten Klasse völlig unterschiedlich ist. Diese richtet sich nach dem zu steuernden Gerät und dessen Anforderungen, ob beispielsweise bei der Schrittmotor-Klasse Impulse genau zum richtigen Zeitpunkt oder bei der Servo-Klasse Impulse der exakt richtigen Länge generiert werden müssen. 
An die Geräteklasse der Motoren werden der Endpunkt, die gewünschte Beschleunigung sowie die gewünschte Geschwindigkeit übergeben. Die Geräteklassen beschleunigen den Motor mit konstanter Beschleunigung vom Stillstand bis zur gewünschten Geschwindigkeit und bremsen ihn mit konstanter Beschleunigung bis zum Stillstand an der Zielposition ab.
Wenn notwendig, könnten hier weitere abgeleitete Klassen z.B. zur gepulsten Steuerung von Elektromagneten o.ä. hinzugefügt werden. 

Der Scheduler enthält ein Array dieser Basisklasse „CCDevice“, in das alle Geräte eingetragen werden. Es können beliebig viele Geräte (Instanzen der abgeleiteten Klassen dieser Klasse) zum Scheduler hinzugefügt, d.h. in dieses Array eingetragen werden. Dies geschieht durch die Funktionen „addServo(...)“, „addStepper(...)“, „addSwitch(...)“ etc. des Schedulers, in der sämtliche Parameter dieses Geräts übergeben werden:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned char liftServo = cuttingProcess->addServo(LIFTSERVO_NAME, LIFTSERVO_PIN, LIFTSERVO_MIN_POSITION, LIFTSERVO_MAX_POSITION, LIFTSERVO_PARK_POSITION);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Ein neues Device-Objekt vom Typ Modellbauservo wird dem Device-Array des Schedulers hinzugefügt. Das Device-Array des Schedulers heißt „device“. Übergeben wird ein Name, mit dem dieses Servo im Ablaufprotokoll erscheint, der Pin, mit dem es gesteuert wird, ein minimaler und ein maximaler Positionswert und die Parkposition.*

<br>

Rückgabewert dieser Funktion ist der Index dieses Gerätes im Geräte-Array („device[]“) des Schedulers. Mit diesem Geräteindex ist das Gerät dann eindeutig identifizierbar. Wird dieser Wert in einer aussagekräftigen Variable gespeichert, ist es sehr einfach, dieses Gerät anzusprechen: 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cuttingProcess->device[liftServo]...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<br>

Zu diesen Geräten des Schedulers können nun beliebig viele Aufgaben hinzugefügt werden. Die Klasse „CCDevice“ enthält ein Aufgaben-Array mit dem Namen „task[]“. Die Eintragung von Aufgaben in dieses Array bewerkstelligt die Funktion addTask(...) des jeweiligen Gerätes: Übergeben werden sämtliche Parameter dieser Aufgabe:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned char lowerHeadForCutting = cuttingProcess->device[liftServo]->addTask(CUTING_POSITION, SPEED_SLOW, ACCELERATION_NORMAL, DECELERATION_SLOW, NO_DELAY);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Dem Gerät "liftServo" wird eine neue Aufgabe hinzugefügt: Fahre zur Position "CUTTING_POSITION" mit der Geschwindigkeit "SPEED_SLOW", der Beschleunigung "ACCELERATION_NORMAL" bzw. "DECELERATION_SLOW". Die Aufgabe startet ohne Startverzögerung. Sein Index wird in der Variable „lowerHeadForCutting“ abgelegt.*

<br>

Rückgabewert dieser Funktion ist wiederum der Index dieser Aufgabe im Aufgaben-Array, mit dem diese Aufgabe eindeutig identifiziert ist. Dieser Wert wird wiederum in einer aussagekräftigen Variable gespeichert, damit sie später leicht angesprochen werden kann.

Gestartet, gestoppt und manipuliert werden können die Geräte durch Zeitablauf, durch die Zustandsänderung angeschlossener Sensoren, durch das Erreichen eines Zustandes bzw. einer Position dieses Gerätes sowie durch das Erreichen eines Zustandes bzw. einer Position jedes anderen Gerätes. Entsprechend lauten die Methoden der Aufgaben („task[]“): startByDate(...), startByButton(...), startAfterMy(...), startAfterCompletionOf(...), startByTriggerpositionOf(...), switchToNextTaskByDate(...), switchToNextTaskByButton(...), switchToNextTaskAfterCompletionOf(...), switchToNextTaskByTriggerpositionOf(...), stopByTimeout(...), stopByButton(...), stopAfterCompletionOf(...), stopByTriggerpositionOf(...), stopDynamicallyBySensor(...);

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cuttingProcess->device[catStepper]->task[driveCatToCuttingStartPosition]->startByDate(100);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Die Aufgabe "driveCatToCuttingStartPosition" soll 100 ms nachdem der Scheduler („cuttingProcess“) gestartet wurde, ausgeführt werden.*

<br>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cuttingProcess->device[liftServo]->task[lowerHeadForCutting]->startAfterCompletionOf(catStepper, driveCatToCuttingStartPosition);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Die Aufgabe "lowerHeadForCutting" wird gestartet, nachdem die Aufgabe "driveCatToCuttingStart" des Schneideschlittenantriebs abgeschlossen ist.*

<br>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cuttingProcess->device[liftServo]->task[lowerHeadForCutting]->stopDynamicallyBySensor(DISTANCE_SENSOR, INITIATE_STOP_VALUE, TARGET_VALUE, ATTENUATION, STOP_VERY_PRECISE);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Die Aufgabe "lowerHeadForCutting" wird entsprechend der Sensorwerte des Distanzsensors im Schneidekopf dynamisch gestoppt.*

<br>

Nachdem die Parameter aller Aufgaben definiert sind, kann der Prozess gestartet werden. Jetzt startet, koordiniert und beendet der Scheduler alle Aufgaben entsprechend der Eingabe.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cuttingProcess->run();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



