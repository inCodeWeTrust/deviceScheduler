//
//  CCStepperDevice_A4988.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCStepperDevice_A4988__
#define __deviceScheduler__CCStepperDevice_A4988__


#include "CCStepperDevice.h"


//  verbosity:
#define CCSTEPPERDEVICE_A4988_BASICOUTPUT         0x01
#define CCSTEPPERDEVICE_A4988_MEMORYDEBUG         0x02
#define CCSTEPPERDEVICE_A4988_CALCULATIONDEBUG    0x04
#define CCSTEPPERDEVICE_A4988_MOVEMENTDEBUG       0x08

#define CCSTEPPERDEVICE_A4988_VERBOSE             0x00


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCStepperDevice_A4988
///
/// @brief Device class for stepper motor devices using a A4988 or similar stepper driver.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCStepperDevice_A4988 : public CCStepperDevice {
    
public:
    CCStepperDevice_A4988(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCodes, unsigned char numberOfMicroStepPins, unsigned char *microStepPin, unsigned int stepsPerRotation);
    
    ~CCStepperDevice_A4988();
    

    void getReadOut(byte theReadOut);

    
private:
    unsigned char        numberOfMicroStepPins;
    unsigned char        *microStepPin;
    
    ///    i.e. "0x00, 0x01, 0x02, 0x03, 0x07":
    /// stepModeCode | bin | pin 2 | pin 1 | pin 0 | description
    /// -------------|-----|-------|-------|-------|---------------------------------------------
    /// 0x00         | 000 |   LOW |   LOW |   LOW | full step mode
    /// 0x01         | 001 |   LOW |   LOW |  HIGH | half step mode
    /// 0x02         | 010 |   LOW |  HIGH |   LOW | quarter step mode
    /// 0x03         | 011 |   LOW |  HIGH |  HIGH | eighth step mode
    /// 0x07         | 111 |  HIGH |  HIGH |  HIGH | sixteenth step mode
    unsigned char        *stepModeCode;

    void setupMicroSteppingMode();
    void setupMicroSteppingMode(unsigned char data);


 };

#endif // defined(__deviceScheduler__CCStepperDevice_A4988__)
