//
//  CCServoDevice_cross.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCServoDevice_cross__
#define __deviceScheduler__CCServoDevice_cross__


#include "CCServoDevice.h"




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

#endif // defined(__deviceScheduler__CCServoDevice_cross__)
