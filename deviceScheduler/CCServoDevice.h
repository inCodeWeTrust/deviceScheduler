//
//  CCServoDevice.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCServoDevice__
#define __deviceScheduler__CCServoDevice__


#include <Arduino.h>
#include "CCDevice.h"

#include <Servo.h>


#define CCSERVODEVICE_BASICOUTPUT           0x01
#define CCSERVODEVICE_MEMORYDEBUG           0x02
#define CCSERVODEVICE_CALCULATIONDEBUG      0x04
#define CCSERVODEVICE_MOVEMENTDEBUG         0x08

#define CCSERVODEVICE_VERBOSE               CCSERVODEVICE_BASICOUTPUT | CCSERVODEVICE_CALCULATIONDEBUG





class CCServoDevice : public CCDevice {
    unsigned long        t0;
    signed long          elapsedTime;
    signed long          wayToGo, wayForAcceleration, wayForConstantSpeed;
    unsigned long        timeForAcceleration, timeForConstantSpeed;
    signed long          deltaS;
    boolean              dynamicalStop, sensorValuesFalling;
    float                deltaDeltaNorm, deltaDelta, performanceFactor, c_perform;
    signed int           valueCounter;
    long                 lastCycleTime;
    signed int           sensorValue;
    float                sensorTreshold;

public:

    Servo                theServo;
    unsigned char        servo_pin;
    unsigned int         startPosition, targetPosition;
    unsigned int         minPosition, maxPosition, parkPosition;



    CCServoDevice(unsigned int deviceIndex, String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition);
    ~CCServoDevice();


    void attachDevice();
    void detachDevice();
    void enableDevice();
    void disableDevice();
    
    void reviewValues();
    void prepareNextMove();
    void startMove();
    void drive();
    void initiateStop();
    void stopMoving();
    void finishMove();
};


#endif /* defined(__deviceScheduler__CCServoDevice__) */

