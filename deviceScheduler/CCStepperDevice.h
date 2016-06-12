//
//  CCStepperDevice.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCStepperDevice__
#define __deviceScheduler__CCStepperDevice__


#include "CCDevice.h"


//  verbosity:
#define CCSTEPPERDEVICE_BASICOUTPUT         0x01
#define CCSTEPPERDEVICE_MEMORYDEBUG         0x02
#define CCSTEPPERDEVICE_CALCULATIONDEBUG    0x04
#define CCSTEPPERDEVICE_MOVEMENTDEBUG       0x08

#define CCSTEPPERDEVICE_VERBOSE             0x00


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCStepperDevice
///
/// @brief Device class for stepper motor devices
///
///////////////////////////////////////////////////////////////////////////////////////////////////



class CCStepperDevice : public CCDevice {

public:

    virtual ~CCStepperDevice();
    
    
    void attachDevice();
    void detachDevice();
    void enableDevice();
    void disableDevice();
    
    void reviewValues();
    void prepareNextTask();
    void startTask();
    void operateTask();
    void initiateStop();
    void stopTask();
    void finishTask();

    virtual void getReadOut(byte theReadOut) = 0;
    
    
protected:
    
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
    unsigned int         *steppingUnit;
    float                stepsPerDegree, degreesPerMicroStep;
    unsigned long        currentMicroStep;
    float                startPosition;
    bool                 prepareAndStartNextTaskWhenFinished, switchAtNextFullStep;
    
    void                 kickDown();
    void                 kickUp();
    
    virtual void         setupMicroSteppingMode() = 0;
    virtual void         setupMicroSteppingMode(unsigned char data) = 0;
    
    
    unsigned char        dir_pin, step_pin, enable_pin;
    unsigned char        highestSteppingMode;
    unsigned int         stepsPerRotation;
    int                  acceleration_max;
    
};


#endif // defined(__deviceScheduler__CCStepperDevice__)
