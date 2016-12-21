//
//  CCDcControllerDevice_fullBridge.h
//  deviceScheduler
//
//  Created by little Ikarus on 17.03.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCDcControllerDevice_fullBridge__
#define __deviceScheduler__CCDcControllerDevice_fullBridge__


#include "CCDcControllerDevice.h"



///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCDcControllerDevice_fullBridge
///
/// @brief Device class for full-controlled full-Bridge devices controlled by PWM-signals or spimply switched on and off
/// [target](@ref target): duty cycle of pwm signal [-1.0 ... 1.0]. If target > 0 the switchingPin is used, if target < 0 switching_B_pin is used for going in reversed direction.
/// [velocity](@ref velocity): pwm frequency in n/sec = Hz.
/// [acceleration](@ref acceleration): time for ramping up.
/// [deceleration](@ref deceleration): time for ramping down.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCDcControllerDevice_fullBridge : public CCDcControllerDevice {
    
private:
    
    const unsigned int  switching_B_pin;
    const bool          switchingPin_B_active;

    
    /// Variable is @c true when switched on, @c false when switched off.
    bool                isActive_B;

    
    
public:
    
    CCDcControllerDevice_fullBridge(const String deviceName, const unsigned int switching_A_pin, const bool switchingPin_A_active, const unsigned int switching_B_pin, const bool switchingPin_B_active);
    ~CCDcControllerDevice_fullBridge();
    
    
    deviceInfoCode      reviewValues(CCTask* nextTask);
    void                prepareNextTask();
    deviceInfoCode      prepareTask(CCTask* nextTask);
    void                startTask();
    void                operateTask();
    void                initiateStop();
    void                stopTask();
    void                finishTask();
    
    void                getReadOut(unsigned int theReadOut);
    
};


#endif // defined(__deviceScheduler__CCDcControllerDevice_fullBridge__)