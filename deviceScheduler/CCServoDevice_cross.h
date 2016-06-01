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


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "deviceScheduler.h"
#include "CCServoDevice.h"

#ifndef CCServoDevice_cross_h
#define CCServoDevice_cross_h


//  verbosity:
#define CCSERVODEVICE_CROSS_BASICOUTPUT         0x01
#define CCSERVODEVICE_CROSS_MEMORYDEBUG         0x02
#define CCSERVODEVICE_CROSS_CALCULATIONDEBUG    0x04
#define CCSERVODEVICE_CROSS_MOVEMENTDEBUG       0x08

#define CCSERVODEVICE_CROSS_VERBOSE             0x00


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCServoDevice_cross
///
/// @brief Device class for two servo motor devices, working in counterrotation
///
///////////////////////////////////////////////////////////////////////////////////////////////////






class CCServoDevice_cross : public CCServoDevice {
    
public:
    CCServoDevice_cross(String deviceName, unsigned char servo_00_pin, int minPosition_00, int midPosition_00, int maxPosition_00, unsigned char servo_01_pin, int minPosition_01, int midPosition_01, int maxPosition_01, int parkPosition);
    ~CCServoDevice_cross();
    
    
    void attachDevice();
    void detachDevice();
    
    void operateTask();

    
    

    
private:
    
    Servo                secondServo;
    unsigned char        servo_01_pin;
    unsigned int         startPosition_01, targetPosition_01;
    unsigned int         minPosition_01, maxPosition_01;
    unsigned int         midPosition_00, midPosition_01;
    
    unsigned int         counterPosition(unsigned int position);

};

#endif
