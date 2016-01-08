#include <Arduino.h>

#include "deviceScheduler.h"
#include "CCTask.h"

//  verbosity:
#define CCDEVICE_BASICOUTPUT            0x01

#define CCDEVICE_VERBOSE                CCDEVICE_BASICOUTPUT



#ifndef CCDEVICE
#define CCDEVICE 1

class CCDevice
{
public:
    float                target, velocity, acceleration, deceleration;
    float                defaultVelocity, defaultAcceleration, defaultDeceleration;
    
    event                startEvent, stopEvent;
    boolean              switchMovePromptly;
    unsigned long        startTime, startDelay, timeout;
    unsigned char        startButton, stopButton;
    bool                 startButtonState, stopButtonState, stopSharply, stopDynamically;
    unsigned char        startTriggerDevice, startTriggerMove, stopTriggerDevice, stopTriggerMove;
    signed long	         startTriggerPosition, stopTriggerPosition;
    unsigned char        sensor;
    signed int           initiatePerformanceValue, targetValue;
    float                stopPerformance;
    unsigned char        stopMode;
   
    CCTask               *task[10];
    
    unsigned int         deviceIndex;
    String               deviceName;
    deviceType           type;
    unsigned char        countOfTasks;
    unsigned char        taskPointer;
    
    float                currentPosition;
    bool                 directionDown;
    deviceState        state;
    
    //        startTime, startDelay & startEvent could be changed by scheduler, so they need to exist aswell outside of the onEventMove
    
       
    
    virtual ~CCDevice();

    void defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration);
    
    unsigned char addTask(float target, float velocity, float acceleration, float deceleration);
    unsigned char addTask(float target, float velocity, float acceleration);
    unsigned char addTask(float target, float velocity);
    unsigned char addTask(float target);
    unsigned char addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration);
    unsigned char addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration);
    unsigned char addMoveWithStartDelay(float target, unsigned long startDelay, float velocity);
    unsigned char addMoveWithStartDelay(float target, unsigned long startDelay);
    
    
    void deleteMoves();
    
    virtual void attachDevice() = 0;
    virtual void detachDevice() = 0;
    virtual void enableDevice() = 0;
    virtual void disableDevice() = 0;
    
    virtual void reviewValues() = 0;
    virtual void prepareNextMove() = 0;
    virtual void startMove() = 0;
    virtual void driveDynamic() = 0;
    virtual void initiateStop() = 0;
    virtual void stopMoving() = 0;
    virtual void finishMove() = 0;
};

#endif
