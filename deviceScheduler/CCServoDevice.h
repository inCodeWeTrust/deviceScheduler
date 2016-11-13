//
//  CCServoDevice.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCServoDevice__
#define __deviceScheduler__CCServoDevice__

#include "CCDevice.h"

#ifndef ARDUINO_SIMULATION
#include <Servo.h>
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCServoDevice
///
/// @brief Device class for servo motor devices
///
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCServoDevice : public CCDevice {

protected:
    
    unsigned long        t0;
    signed long          elapsedTime;
    signed long          wayToGo, wayForAcceleration, wayForConstantSpeed;
    unsigned long        timeForAcceleration, timeForConstantSpeed;
    signed long          deltaS;
    bool                 dynamicalStop, sensorValuesFalling;
    float                deltaDeltaNorm, deltaDelta, performanceFactor, c_perform;
    signed int           valueCounter;
    long                 lastCycleTime;
    signed int           sensorValue;
    float                sensorTreshold;


    Servo                theServo;
    unsigned int         servo_pin;
    unsigned int         startPosition, targetPosition;
    unsigned int         minPosition, maxPosition, parkPosition;

    
public:
    
    CCServoDevice(String deviceName, unsigned int servo_pin, int minPosition, int maxPosition, int parkPosition);
    ~CCServoDevice();


    void attachDevice();
    void detachDevice();
    void enableDevice();
    void disableDevice();
    
    deviceInfoCode reviewValues(CCTask* nextTask);
    void prepareNextTask();
    deviceInfoCode prepareTask(CCTask* nextTask);
    void startTask();
    void operateTask();
    void initiateStop();
    void stopTask();
    void finishTask();
    
    void getReadOut(unsigned int theReadOut);


};

#endif // defined(__deviceScheduler__CCServoDevice__)

