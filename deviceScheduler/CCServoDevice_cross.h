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
    CCServoDevice_cross(const String deviceName, const unsigned int servo_00_pin, const int minPosition_00, const int midPosition_00, const int maxPosition_00, const unsigned int servo_01_pin, const int minPosition_01, const int midPosition_01, const int maxPosition_01, const int parkPosition);
    ~CCServoDevice_cross();
    
    
    void attachDevice();
    void detachDevice();
    
    void operateTask();

    
private:

    unsigned int         startPosition_01, targetPosition_01;

    Servo                secondServo;
    const unsigned int         midPosition_00;
    const unsigned int         servo_01_pin;
    const unsigned int         minPosition_01;
    const unsigned int         midPosition_01;
    const unsigned int         maxPosition_01;
    
    unsigned int         counterPosition(unsigned int position);

};

#endif // defined(__deviceScheduler__CCServoDevice_cross__)
