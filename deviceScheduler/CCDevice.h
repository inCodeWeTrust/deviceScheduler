#include <Arduino.h>


#define CCDEVICE_BASICOUTPUT         0x01
#define CCDEVICE_MEMORYDEBUG         0x02
#define CCDEVICE_CALCULATIONDEBUG    0x04
#define CCDEVICE_MOVEMENTDEBUG       0x08

#define CCDevice_VERBOSE CCDEVICE_BASICOUTPUT | CCDEVICE_MEMORYDEBUG | CCDEVICE_CALCULATIONDEBUG


//  device type
#define SERVODEVICE          1
#define STEPPERDEVICE        2
#define SOLENOIDDEVICE       3

// start/stopEvent:
#define DATE            0x01
#define BUTTON          0x02
#define POSITION        0x04

//  state:
#define MOVING		0x01
#define MOVE_DONE	0x02
#define PENDING_MOVES	0x04


#ifndef CCDEVICE
#define CCDEVICE 1

class CCDevice
{
public:
    float                target, velocity, acceleration;
    
    unsigned char        startEvent, stopEvent;
    unsigned long        startTime, startDelay, timeout;
    unsigned char        startButton, stopButton;
    boolean              startButtonState, stopButtonState, stopSharply;
    unsigned char        startTriggerDevice, startTriggerMove, stopTriggerDevice, stopTriggerMove;
    signed long	         startTriggerPosition, stopTriggerPosition;
    
    
    String               deviceName;
    unsigned char        type;
    unsigned char        countOfMoves;
    unsigned char        movePointer;
    
    signed long	         currentPosition;
    boolean              countingDown;
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
        
        onEventMove(float target, float velocity, float acceleration, unsigned long startDelay) : target(target), velocity(velocity), acceleration(acceleration), startDelay(startDelay), startEvent(0), stopEvent(0), startTime(0), timeout(0), startButton(0), stopButton(0), startButtonState(0), stopButtonState(0), startTriggerDevice(0), startTriggerMove(0), startTriggerPosition(0), stopTriggerDevice(0), stopTriggerMove(0), stopTriggerPosition(0), stopSharply(0) {}
    };
    
    onEventMove         *theMove[10];
    
    
    virtual ~CCDevice();
    
    unsigned char addMove(float target, float velocity, float acceleration, unsigned long startDelay);
    
    void setStartDateForMove(unsigned char moveIndex, unsigned long startTime);
    void setStartButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState);
    void setStartEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
    
    void setTimeoutForMove(unsigned char moveIndex, unsigned long _timeout, boolean stopSharply);
    void setStopButtonForMove(unsigned char moveIndex, unsigned char _stopButton, boolean _stopButtonState, boolean stopSharply);
    void setStopEventForMove(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply);
    
    void deleteMoves();
    
    virtual void attachDevice() = 0;
    virtual void detachDevice() = 0;
    virtual void enableDevice() = 0;
    virtual void disableDevice() = 0;
    
    virtual void prepareNextMove() = 0;
    virtual void startMove() = 0;
    virtual void driveDynamic() = 0;
    virtual void switchToNextMove() = 0;
    virtual void initiateStop() = 0;
    virtual void stopMoving() = 0;
    virtual void finishMove() = 0;
};

#endif
