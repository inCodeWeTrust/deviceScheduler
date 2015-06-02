#include <Arduino.h>

#include "deviceScheduler.h"


#include "CCDevice.h"

#define CCStepperDevice_BASICOUTPUT         0x01
#define CCStepperDevice_MEMORYDEBUG         0x02
#define CCStepperDevice_CALCULATIONDEBUG    0x04
#define CCStepperDevice_MOVEMENTDEBUG       0x08

//static unsigned char CCStepperDevice_VERBOSE = 0;
#define CCStepperDevice_VERBOSE             0x07

//#define CCStepperDevice_VERBOSE CCStepperDevice_BASICOUTPUT | CCStepperDevice_MEMORYDEBUG | CCStepperDevice_CALCULATIONDEBUG




class CCStepperDevice : public CCDevice {
    
    signed long          stepsToGo;
    unsigned long        stepsForAcceleration, stepsForDeceleration, stepsForAccAndConstSpeed;
    unsigned long        timeForAcceleration, timeForAccAndConstSpeed;
    unsigned long        microStepsToGo, microStepsForAcceleration, microStepsForAccAndConstSpeed;
    signed long          t0, t0_old;
    unsigned long        stepExpiration, elapsedTime, lastStepTime;
    float                c0_acc, c0_dec, c1;
    unsigned char        microSteppingMode;       // 0: none, 1: halfStep, 2: quarterStep, 3: eigthStep, 4: sixteenthStep
    unsigned long        currentMicroStep;
    bool                 lastStep;
    
    void calculateSteppingValues();
    void kickDown();
    void kickUp();
    
public:
    
    unsigned char        dir_pin, step_pin, enable_pin, microStep_0_pin, microStep_1_pin, microStep_2_pin;
    unsigned char        highestSteppingMode;
    unsigned char        *stepModeCode;
    float                anglePerStep;
    
    
    CCStepperDevice(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, unsigned char *stepModeCodes, unsigned char microStep_0_pin, unsigned char microStep_1_pin, unsigned char microStep_2_pin, float anglePerStep);
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

