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




//  log visuals:
#define DEVICESCHEDULER_SHOW_TASK_VIEW                  0x10
#define DEVICESCHEDULER_SHOW_TAB_VIEW                   0x20



#define DEVICESCHEDULER_BASICOUTPUT                     0x01

#define DEVICESCHEDULER_VERBOSE         (DEVICESCHEDULER_SHOW_TASK_VIEW | DEVICESCHEDULER_BASICOUTPUT)




class CCDeviceScheduler {
    
    
    
public:
    
    unsigned char countOfDevices;
    
    CCDevice *device[3];
    
    CCDeviceScheduler();
    ~CCDeviceScheduler();
    
    unsigned char addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition);
    unsigned char addStepper(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, float anglePerStep);
    unsigned char addSolenoid(String deviceName, unsigned char solenoid_pin);
    
    void getAllDevices();
    void getAllMoves();
    void getMovesForDevice(unsigned char theDevice);
    void deleteAllMoves();
    
    void runTheLoop();
    
};



