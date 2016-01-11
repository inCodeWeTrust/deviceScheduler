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
#include "CCSwitchDevice.h"




//  log visuals:
#define DEVICESCHEDULER_BASICOUTPUT                     0x01

#define DEVICESCHEDULER_SHOW_TASK_VIEW                  0x10
#define DEVICESCHEDULER_SHOW_TAB_VIEW                   0x20

#define DEVICESCHEDULER_VERBOSE                         (DEVICESCHEDULER_BASICOUTPUT | DEVICESCHEDULER_SHOW_TASK_VIEW)



class CCDeviceScheduler {
    
    
    
public:
    
    unsigned char countOfDevices;
    
    CCDevice *device[8];
    
    CCDeviceScheduler();
    ~CCDeviceScheduler();
    
    unsigned char addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition);
    unsigned char addStepper(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, unsigned int stepsPerRotation);
    unsigned char addSwitch(String deviceName, unsigned char switching_pin, boolean defaultState);

//    unsigned char addSolenoid(String deviceName, unsigned char solenoid_pin);
    
    void getAllDevices();
    void getAllMoves();
    void getMovesForDevice(unsigned char theDevice);
    void reviewMoves();

    void deleteAllMoves();
    
    void run();
    
};



#endif /* defined(__deviceScheduler__CCDeviceScheduler__) */


