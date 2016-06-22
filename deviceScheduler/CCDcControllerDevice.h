//
//  CCDcControllerDevice.h
//  deviceScheduler
//
//  Created by little Ikarus on 17.03.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCDcControllerDevice__
#define __deviceScheduler__CCDcControllerDevice__


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
/// [target](@ref target): duty cycle of pwm signal.
/// [velocity](@ref velocity): pwm frequency in n/sec = Hz.
/// [acceleration](@ref acceleration): time for ramping up.
/// [deceleration](@ref deceleration): time for ramping down.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCDcControllerDevice : public CCDevice {
    
private:
    
    unsigned char        switching_pin, switchingPin_activ;

    // switchingInterval: periodendauer = 1 / frequency
    // acceleration: time for ramping up
    // deceleration: time for ramping down
    

    /// The current task's starting time.
    unsigned long        t0;
    
    /// The elapsed time since this task was started or switched.
    signed long          elapsedTime;
    
    /// The time for ramping up @<= acceleration@>.
    signed long          timeForAcceleration;

    /// The time for ramping down @<= deceleration@>.
    signed long          timeForDeceleration;
    
    /// This variable could be used when the task should be terminated automatically.
    signed long          timeForAccAndConstSpeed;
    
    /// The switching time interval @<1 / pwmFrequency = 1 / velocity@>.
    unsigned int         switchingInterval;

    /// The duration of onTime @< = target * switchingInterval@>.
    unsigned int         targetOnDuration;
    
    /// The effectiv date for switching on.
    signed long          switchOnTime;

    /// The effectiv date for switching off.
    signed long          switchOffTime;
    
    /// Variable is @c true when switched on, @c false when switched off.
    boolean              isActiv;
    
    /// The initial rate of duty cycle, when the task is switched, like v0.
    float                currentRatio;
    
    
public:
    
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
    
    void getReadOut(byte theReadOut);
    
};


#endif // defined(__deviceScheduler__CCDcControllerDevice__)