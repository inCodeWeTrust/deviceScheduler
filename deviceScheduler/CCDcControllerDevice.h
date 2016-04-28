//
//  CCDcControllerDevice.h
//  deviceScheduler
//
//  Created by little Ikarus on 17.03.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCDcControllerDevice__
#define __deviceScheduler__CCDcControllerDevice__


#include <Arduino.h>
#include "deviceScheduler.h"
#include "CCDevice.h"


//  verbosity:
#define CCDcControllerDevice_BASICOUTPUT         0x01
#define CCDcControllerDevice_MEMORYDEBUG         0x02
#define CCDcControllerDevice_CALCULATIONDEBUG    0x04
#define CCDcControllerDevice_MOVEMENTDEBUG       0x08

#define CCDcControllerDevice_VERBOSE             0



///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCDcControllerDevice
///
/// @brief Device class for devices controlled by PWM-signals or spimply switched on and off
///
///////////////////////////////////////////////////////////////////////////////////////////////////



class CCDcControllerDevice : public CCDevice {
    

    
    unsigned long        t0;
    signed long          elapsedTime, timeForAcceleration, timeForDeceleration, timeForAccAndConstSpeed;
    unsigned int         switchingInterval, targetOnDuration;
    signed long          switchOnTime, switchOffTime;
    boolean              isActiv;
    float                currentRatio;
    
    
    
public:
    
    unsigned char        switching_pin, switchingPin_activ;
    
    
    CCDcControllerDevice(String deviceName, unsigned char switching_pin, boolean switchingPin_activ);
    ~CCDcControllerDevice();
    
    
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


#endif /* defined(__deviceScheduler__CCDcControllerDevice__) */