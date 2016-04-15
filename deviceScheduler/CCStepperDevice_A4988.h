///
/// @file		CCStepperDevice_A4988.h
/// @brief		Header
/// @details	<#details#>
/// @n	
/// @n @b		Project deviceScheduler
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		Little Abakus
/// @author		Little Abakus
///
/// @date		14.04.16 22:43
/// @version	<#version#>
/// 
/// @copyright	(c) Little Abakus, 2016
/// @copyright	<#license#>
///
/// @see		ReadMe.txt for references
///



#include "Arduino.h"
#include "deviceScheduler.h"
#include "CCStepperDevice.h"
#include "CCDevice.h"


#ifndef CCStepperDevice_A4988_h
#define CCStepperDevice_A4988_h


////  verbosity:
//#define CCSTEPPERDEVICE_BASICOUTPUT         0x01
//#define CCSTEPPERDEVICE_MEMORYDEBUG         0x02
//#define CCSTEPPERDEVICE_CALCULATIONDEBUG    0x04
//#define CCSTEPPERDEVICE_MOVEMENTDEBUG       0x08
//
//#define CCSTEPPERDEVICE_VERBOSE             0


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCStepperDevice
///
/// @brief Device class for stepper motor devices
///
///////////////////////////////////////////////////////////////////////////////////////////////////





class CCStepperDevice_A4988 : public CCStepperDevice {
    
        
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

    
    
public:
    CCStepperDevice_A4988(unsigned int deviceIndex, String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCodes, unsigned char numberOfMicroStepPins, unsigned char *microStepPin, unsigned int stepsPerRotation);

    
    void attachDevice();
    void detachDevice();


};

#endif
