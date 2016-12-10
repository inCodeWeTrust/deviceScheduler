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


#define NUMBER_OF_MICROSTEPPING_PINS        3

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCStepperDevice_A4988
///
/// @brief Device class for stepper motor devices using a A4988 or similar stepper driver.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCStepperDevice_A4988 : public CCStepperDevice {
    
public:
    CCStepperDevice_A4988(const String deviceName, const unsigned int step_pin, const unsigned int dir_pin, const unsigned int enable_pin, const unsigned int stepsPerRotation, const unsigned int microStep_00_pin, const unsigned int microStep_01_pin, const unsigned int microStep_02_pin, const signed char steppingCode_00, const signed char steppingCode_01, const signed char steppingCode_02, const signed char steppingCode_03, const signed char steppingCode_04, const signed char steppingCode_05, const signed char steppingCode_06, const signed char steppingCode_07);
    
    ~CCStepperDevice_A4988();
    

    void getReadOut(unsigned int theReadOut);

    
private:
    unsigned int        microStepPin[NUMBER_OF_MICROSTEPPING_PINS];
    
    ///    i.e. "0x00, 0x01, 0x02, 0x03, 0x07":
    /// stepModeCode | bin | pin 2 | pin 1 | pin 0 | description
    /// -------------|-----|-------|-------|-------|---------------------------------------------
    /// 0x00         | 000 |   LOW |   LOW |   LOW | full step mode
    /// 0x01         | 001 |   LOW |   LOW |  HIGH | half step mode
    /// 0x02         | 010 |   LOW |  HIGH |   LOW | quarter step mode
    /// 0x03         | 011 |   LOW |  HIGH |  HIGH | eighth step mode
    /// 0x07         | 111 |  HIGH |  HIGH |  HIGH | sixteenth step mode
    unsigned int        *stepModeCode;

    void setupMicroSteppingMode();
    void setupMicroSteppingMode(unsigned int data);


 };

#endif // defined(__deviceScheduler__CCStepperDevice_A4988__)
