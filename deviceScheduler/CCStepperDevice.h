//
//  CCStepperDevice.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCStepperDevice__
#define __deviceScheduler__CCStepperDevice__


#include <Arduino.h>
#include "deviceScheduler.h"
#include "CCDevice.h"


//  verbosity:
#define CCSTEPPERDEVICE_BASICOUTPUT         0x01
#define CCSTEPPERDEVICE_MEMORYDEBUG         0x02
#define CCSTEPPERDEVICE_CALCULATIONDEBUG    0x04
#define CCSTEPPERDEVICE_MOVEMENTDEBUG       0x08

#define CCSTEPPERDEVICE_VERBOSE             CCSTEPPERDEVICE_BASICOUTPUT




class CCStepperDevice : public CCDevice {
    
    signed long          stepsToGo;
    unsigned long        stepsForAcceleration, stepsForDeceleration, stepsForAccAndConstSpeed;
    unsigned long        timeForAcceleration, timeForAccAndConstSpeed;
    unsigned long        microStepsToGo, microStepsForAcceleration, microStepsForAccAndConstSpeed;
    bool                 accelerateDown;
    float                currentVelocity, currVeloBySquare, veloBySquare;
    signed long          t0;
    unsigned long        stepExpiration, elapsedTime, lastStepTime;
    float                c0_acc, c0_dec, c1, acceleration_inv, deceleration_inv;
    unsigned char        microSteppingMode;       // 0: none, 1: halfStep, 2: quarterStep, 3: eigthStep, 4: sixteenthStep
    unsigned char        *steppingUnit;
    float                stepsPerDegree, degreesPerMicroStep;
    unsigned long        currentMicroStep;
    float                startPosition;
    bool                 prepareAndStartNextMoveWhenFinished, switchAtNextFullStep;
    
    void                 kickDown();
    void                 kickUp();
    void                 setMicroStepPins();
    void                 setMicroStepPins(unsigned char data);

    
public:
    
    unsigned char        dir_pin, step_pin, enable_pin;
    unsigned char        numberOfMicroStepPins;
    unsigned char        *microStepPin;
    unsigned char        highestSteppingMode;
    unsigned char        *stepModeCode;
    unsigned int         stepsPerRoation;
    int                  acceleration_max;
    
    
    CCStepperDevice(unsigned int deviceIndex, String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCodes, unsigned char numberOfMicroStepPins, unsigned char *microStepPin, unsigned int stepsPerRotation);
    ~CCStepperDevice();
    
    
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


#endif /* defined(__deviceScheduler__CCStepperDevice__) */
