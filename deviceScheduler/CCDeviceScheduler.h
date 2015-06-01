//
//  CCDeviceScheduler.h
//  CCDeviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "deviceScheduler.h"

#include "CCDevice.h"
#include "CCServoDevice.h"
#include "CCStepperDevice.h"
#include "CCSolenoidDevice.h"



//  device type
#define SERVODEVICE                     0x01
#define STEPPERDEVICE                   0x02
#define SOLENOIDDEVICE                  0x03

// start/stopEvent:
#define NONE                            0x00
#define DATE                            0x01
#define BUTTON                          0x02
#define POSITION                        0x04

//  device state:
#define SLEEPING                        0x00
#define MOVING		                0x01
#define MOVE_DONE	                0x02
#define PENDING_MOVES	                0x04


//  log operations:
#define SHOW_TASK_VIEW                  0x10
#define SHOW_TAB_VIEW                   0x20



#define DEVICESCHEDULER_BASICDEBUG      0x01
#define DEVICESCHEDULER_MEMORYDEBUG     0x02

//static unsigned char CCDeviceSchedulerVerbose = DEVICESCHEDULER_BASICDEBUG | DEVICESCHEDULER_MEMORYDEBUG;
//static unsigned char CCDeviceSchedulerVerbose = SHOW_TASK_VIEW | DEVICESCHEDULER_BASICDEBUG | DEVICESCHEDULER_MEMORYDEBUG;
static unsigned char CCDeviceSchedulerVerbose = 0x13;




class CCDeviceScheduler {



public:

  unsigned char countOfDevices;

  CCDevice *device[10];
  CCDevice *tempDevice;

  CCDeviceScheduler();
  ~CCDeviceScheduler();

  unsigned char addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition);
//  unsigned char addStepper(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char microStep_0_pin, unsigned char microStep_1_pin, unsigned char microStep_2_pin, float anglePerStep);
    unsigned char addStepper(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, unsigned char microStep_0_pin, unsigned char microStep_1_pin, unsigned char microStep_2_pin, float anglePerStep);
  unsigned char addSolenoid(String deviceName, unsigned char solenoid_pin);

  void getAllDevices();
  void getAllMoves();
  void getMovesForDevice(unsigned char theDevice);
  void deleteAllMoves();

  void runTheLoop();
  
};



