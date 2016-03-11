//
//  CCStepperDevice.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCSolenoidDevice__
#define __deviceScheduler__CCSolenoidDevice__


#include <Arduino.h>
#include "deviceScheduler.h"
#include "CCDevice.h"


//  verbosity:
#define CCSolenoidDevice_BASICOUTPUT         0x01
#define CCSolenoidDevice_MEMORYDEBUG         0x02
#define CCSolenoidDevice_CALCULATIONDEBUG    0x04
#define CCSolenoidDevice_MOVEMENTDEBUG       0x08

//static unsigned char CCSolenoidDevice_VERBOSE = 0;
#define CCSolenoidDevice_VERBOSE            0x0F
//#define CCSolenoidDevice_VERBOSE CCSolenoidDevice_BASICOUTPUT | CCSolenoidDevice_MEMORYDEBUG | CCSolenoidDevice_CALCULATIONDEBUG



///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCSolenoidDevice
///
/// @brief Device class for solenoid devices
///////////////////////////////////////////////////////////////////////////////////////////////////



class CCSolenoidDevice : public CCDevice {
    unsigned long        t0;
    signed long          elapsedTime;
    unsigned int         switchingInterval, onDuration;
    unsigned long        switchOnTime, switchOffTime;
    boolean              isActiv;



  public:

    unsigned char        solenoid_pin;


    CCSolenoidDevice(unsigned int deviceIndex, String deviceName, unsigned char solenoid_pin);
    ~CCSolenoidDevice();


    void attachDevice();
    void detachDevice();
    void enableDevice();
    void disableDevice();
    
    void reviewValues();
    void prepareNextTask();
    void startTask();
    void operateTask();
    void initiateStop();
    void stopTask();
    void finishTask();
};
#endif /* defined(__deviceScheduler__CCSolenoidDevice__) */

