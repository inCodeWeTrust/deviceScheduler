#include <Arduino.h>

#include "deviceScheduler.h"


#include "CCDevice.h"


//  verbosity:
#define CCSTEPPERDEVICE_BASICOUTPUT         0x01
#define CCSTEPPERDEVICE_MEMORYDEBUG         0x02
#define CCSTEPPERDEVICE_CALCULATIONDEBUG    0x04
#define CCSTEPPERDEVICE_MOVEMENTDEBUG       0x08

#define CCSTEPPERDEVICE_VERBOSE             (CCSTEPPERDEVICE_BASICOUTPUT | CCSTEPPERDEVICE_MEMORYDEBUG | CCSTEPPERDEVICE_CALCULATIONDEBUG | CCSTEPPERDEVICE_MOVEMENTDEBUG)




class CCStepperDevice : public CCDevice {
    
    signed long          stepsToGo;
    unsigned long        stepsForAcceleration, stepsForDeceleration, stepsForAccAndConstSpeed;
    unsigned long        timeForAcceleration, timeForAccAndConstSpeed;
    unsigned long        microStepsToGo, microStepsForAcceleration, microStepsForAccAndConstSpeed, microStepsTillZero;
    signed long          t0;
    unsigned long        stepExpiration, elapsedTime, lastStepTime;
    float                c0_acc, c0_dec, c1;
//    long                c0_acc, c0_dec, c1;
    unsigned char        microSteppingMode;       // 0: none, 1: halfStep, 2: quarterStep, 3: eigthStep, 4: sixteenthStep
    unsigned long        currentMicroStep;
    float                currentVelocity, currVeloBySquare, veloBySquare;
//    long                currentVelocity, currVeloBySquare, veloBySquare;
    bool                 changeDirection, targetDirectionDown, prepareNextMoveWhenFinished;
    unsigned char        *steppingUnit;
    float                *positionUnit;
    
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
    float                anglePerStep;
    int                  acceleration_max;
    
    
    CCStepperDevice(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCodes, unsigned char numberOfMicroStepPins, unsigned char *microStepPin, float anglePerStep);
    ~CCStepperDevice();
    
    
    void attachDevice();
    void detachDevice();
    void enableDevice();
    void disableDevice();
    void prepareNextMove();
    void startMove();
    void driveDynamic();
    void switchToNextMove();
    void initiateStop();
    void stopMoving();
    void finishMove();
    
};

