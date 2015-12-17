#include <Arduino.h>

#include "deviceScheduler.h"

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
    
    unsigned char        startEvent, stopEvent;
    unsigned long        startTime, startDelay, timeout;
    unsigned char        startButton, stopButton;
    bool                 startButtonState, stopButtonState, stopSharply, stopDynamically;
    unsigned char        startTriggerDevice, startTriggerMove, stopTriggerDevice, stopTriggerMove;
    signed long	         startTriggerPosition, stopTriggerPosition;
    unsigned char        sensor;
    signed int           initiatePerformanceValue, stopValue;
    float                stopPerformance;
    unsigned char        stopMode;
    boolean              runningNormally;
   
    
    
    unsigned int         deviceIndex;
    String               deviceName;
    unsigned char        type;
    unsigned char        countOfMoves;
    unsigned char        movePointer;
    
    float                currentPosition;
    bool                 directionDown;
    unsigned char        state;
    
    //        startTime, startDelay & startEvent could be changed by scheduler, so they need to exist aswell outside of the onEventMove
    
    struct onEventMove {
        unsigned char      startEvent;
        unsigned char      stopEvent;
        float              target;
        float              velocity;
        float              acceleration;
        float              deceleration;
        unsigned long      startDelay;
        unsigned long      startTime;
        unsigned long      timeout;
        unsigned char      startButton;
        unsigned char      stopButton;
        boolean            startButtonState;
        boolean            stopButtonState;
        unsigned char      startTriggerDevice;
        unsigned char      startTriggerMove;
        signed long        startTriggerPosition;
        unsigned char      stopTriggerDevice;
        unsigned char      stopTriggerMove;
        signed long        stopTriggerPosition;
        boolean            stopSharply;
        boolean            stopDynamically;
        unsigned char      sensor;
        signed int         initiatePerformanceValue, stopValue;
        float              stopPerformance;
        unsigned char      stopMode;
        
        
        
        
        
        onEventMove(float target, float velocity, float acceleration, unsigned long startDelay) : target(target), velocity(velocity), acceleration(acceleration), deceleration(0), startDelay(startDelay), startEvent(0), stopEvent(0), startTime(0), timeout(0), startButton(0), stopButton(0), startButtonState(0), stopButtonState(0), startTriggerDevice(0), startTriggerMove(0), startTriggerPosition(0), stopTriggerDevice(0), stopTriggerMove(0), stopTriggerPosition(0), stopSharply(0), stopDynamically(0), sensor(0), initiatePerformanceValue(0), stopValue(0), stopPerformance(0) {}
        onEventMove(float target, float velocity, float acceleration, float deceleration, unsigned long startDelay) : target(target), velocity(velocity), acceleration(acceleration), deceleration(deceleration), startDelay(startDelay), startEvent(0), stopEvent(0), startTime(0), timeout(0), startButton(0), stopButton(0), startButtonState(0), stopButtonState(0), startTriggerDevice(0), startTriggerMove(0), startTriggerPosition(0), stopTriggerDevice(0), stopTriggerMove(0), stopTriggerPosition(0), stopSharply(0), stopDynamically(0), sensor(0), initiatePerformanceValue(0), stopValue(0), stopPerformance(0) {}
    };
    
    onEventMove         *theMove[8];
    
    
    virtual ~CCDevice();

    void defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration);
    
    unsigned char addMove(float target, float velocity, float acceleration, float deceleration);
    unsigned char addMove(float target, float velocity, float acceleration);
    unsigned char addMove(float target, float velocity);
    unsigned char addMove(float target);
    unsigned char addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration);
    unsigned char addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration);
    unsigned char addMoveWithStartDelay(float target, unsigned long startDelay, float velocity);
    unsigned char addMoveWithStartDelay(float target, unsigned long startDelay);
    
    void startMoveByDate(unsigned char moveIndex, unsigned long startTime);
    void startMoveByButton(unsigned char moveIndex, unsigned char startButton, boolean startButtonState);
    void startMoveAfterCompletion(unsigned char moveIndex);
    void startMoveAfterCompletionOf(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove);
    void startMoveByTriggerposition(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
    
    void switchMoveByDate(unsigned char moveIndex, unsigned long startTime);
    void switchMoveByButton(unsigned char moveIndex, unsigned char startButton, boolean startButtonState);
    void switchMoveByTriggerposition(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);

    void stopMoveByTimeout(unsigned char moveIndex, unsigned long _timeout, boolean stopSharply);
    void stopMoveByButton(unsigned char moveIndex, unsigned char _stopButton, boolean _stopButtonState, boolean stopSharply);
    void stopMoveAfterCompletionOf(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove);
    void stopMoveByTriggerposition(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply);
    
    void stopMoveDynamically(unsigned char moveIndex, unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int stopValue, float stopPerformance, unsigned char stopMode);
    
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
