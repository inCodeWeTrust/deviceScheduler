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

//  verbosity:
#define CCDcControllerDevice_fullBridge_BASICOUTPUT         0x01
#define CCDcControllerDevice_fullBridge_MEMORYDEBUG         0x02
#define CCDcControllerDevice_fullBridge_CALCULATIONDEBUG    0x04
#define CCDcControllerDevice_fullBridge_MOVEMENTDEBUG       0x08

#define CCDcControllerDevice_fullBridge_VERBOSE             0


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
    
    unsigned char        switching_B_pin, switchingPin_B_activ;

    
    /// Variable is @c true when switched on, @c false when switched off.
    boolean              isActiv_B;
    
    
    
public:
    
    CCDcControllerDevice_fullBridge(String deviceName, unsigned char switching_A_pin, boolean switchingPin_A_activ, unsigned char switching_B_pin, boolean switchingPin_B_activ);
    ~CCDcControllerDevice_fullBridge();
    
    
    void prepareNextTask();
    void prepareTask(CCTask* nextTask);
    void startTask();
    void operateTask();
    void initiateStop();
    void stopTask();
    void finishTask();
    
    void getReadOut(byte theReadOut);
    
};


#endif // defined(__deviceScheduler__CCDcControllerDevice_fullBridge__)