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




///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCStepperDevice_A4988
///
/// @brief Device class for stepper motor devices using a A4988 or similar stepper driver.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCStepperDevice_A4988 : public CCStepperDevice {
    
public:
    CCStepperDevice_A4988(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned int stepsPerRotation, unsigned char microStep_00_pin, unsigned char microStep_01_pin, unsigned char microStep_02_pin, signed char steppingCode_00, signed char steppingCode_01, signed char steppingCode_02, signed char steppingCode_03, signed char steppingCode_04, signed char steppingCode_05, signed char steppingCode_06, signed char steppingCode_07);
    
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
