#include <Arduino.h>


//  device type
#define SERVODEVICE                     0x01
#define STEPPERDEVICE                   0x02
#define SWITCHINGDEVICE                 0x03
#define SOLENOIDDEVICE                  0x04

// start/stopEvent:
#define NONE                            0x00
#define DATE                            0x01
#define BUTTON                          0x02
#define POSITION                        0x04
#define FOLLOW                          0x08
#define SWITCH                          0x10

//  device state:
#define SLEEPING                        0x00
#define MOVING                          0x01
#define MOVE_DONE                       0x02
#define PENDING_MOVES	                0x04


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
    bool                 startButtonState, stopButtonState, stopSharply, stopDynamicaly;
    unsigned char        startTriggerDevice, startTriggerMove, stopTriggerDevice, stopTriggerMove;
    signed long	         startTriggerPosition, stopTriggerPosition;
    unsigned char        sensor;
    unsigned int         initiatePerformanceValue, stopValue;
    unsigned char        stopPerformance;
   
    
    
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
        boolean            stopDynamicaly;
        unsigned char      sensor;
        unsigned int       initiatePerformanceValue, stopValue;
        unsigned char      stopPerformance;
        
        onEventMove(float target, float velocity, float acceleration, unsigned long startDelay) : target(target), velocity(velocity), acceleration(acceleration), deceleration(0), startDelay(startDelay), startEvent(0), stopEvent(0), startTime(0), timeout(0), startButton(0), stopButton(0), startButtonState(0), stopButtonState(0), startTriggerDevice(0), startTriggerMove(0), startTriggerPosition(0), stopTriggerDevice(0), stopTriggerMove(0), stopTriggerPosition(0), stopSharply(0), stopDynamicaly(0), sensor(0), initiatePerformanceValue(0), stopValue(0), stopPerformance(0) {}
        onEventMove(float target, float velocity, float acceleration, float deceleration, unsigned long startDelay) : target(target), velocity(velocity), acceleration(acceleration), deceleration(deceleration), startDelay(startDelay), startEvent(0), stopEvent(0), startTime(0), timeout(0), startButton(0), stopButton(0), startButtonState(0), stopButtonState(0), startTriggerDevice(0), startTriggerMove(0), startTriggerPosition(0), stopTriggerDevice(0), stopTriggerMove(0), stopTriggerPosition(0), stopSharply(0), stopDynamicaly(0), sensor(0), initiatePerformanceValue(0), stopValue(0), stopPerformance(0) {}
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
    
    void setStartDateForMove(unsigned char moveIndex, unsigned long startTime);
    void setStartButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState);
    void setStartEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
    
    void setStartAfterCompletion(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove);
    
    void setSwitchDateForMove(unsigned char moveIndex, unsigned long startTime);
    void setSwitchButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState);
    void setSwitchEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);

    void setTimeoutForMove(unsigned char moveIndex, unsigned long _timeout, boolean stopSharply);
    void setStopButtonForMove(unsigned char moveIndex, unsigned char _stopButton, boolean _stopButtonState, boolean stopSharply);
    void setStopEventForMove(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply);
    
    void setStopDynamicalForMove(unsigned char moveIndex, unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int stopValue, unsigned char stopPerformance);
    
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
