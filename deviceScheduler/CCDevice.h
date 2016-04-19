//
//  CCDevice.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCDevice__
#define __deviceScheduler__CCDevice__



#include <Arduino.h>

#include "deviceScheduler.h"
#include "CCTask.h"


//  verbosity:
#define CCDEVICE_BASICOUTPUT            0x01

#define CCDEVICE_VERBOSE                0


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCDevice
///
/// @brief base class for all devices
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
/// # This baseclass
/// holds all parameters, that are common for all devices,
/// including the array of tasks to be executed by this device
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////





class CCDevice {
    
public:

    /// Device parameter:
    /// Value, set by the owner of the device (CCDeviceScheduler) to be recognized by him.
    unsigned int         deviceIndex;
    
    /// Parameters of the device
    /// Value, set by the user to be recognized by him.
    String               deviceName;
    
    /// Parameters of the device
    /// Parameter that holds the type of the device.
    ///
    /// type                | description
    /// --------------------|--------------------------------------------------------------------------
    /// SERVODEVICE         | modell servo
    /// STEPPERDEVICE       | stepper motor with stepper driver
    /// DCCONTROLLERDEVICE  | any device to be pwm driven
    deviceType           type;
    
    /// Parameters of the device as a peer of a run-loop.
    /// Value, that holds the number of tasks to be executed within this run-loop.
    unsigned char        countOfTasks;
    
    /// Parameters of the device as a peer of a run-loop.
    /// Value, that holds the index of the current task.
    unsigned char        taskPointer;
    
    
    /// Default parameters for the device.
    /// A value for the device's default velocity is provided here.
    float                defaultVelocity;
    
    /// Default parameters for the device.
    /// A value for the device's default acceleration is provided here.
    float                defaultAcceleration;
    
    /// Default parameters for the device.
    /// A value for the device's default deceleration is provided here.
    float                defaultDeceleration;
    
    /// Device parameter:
    /// The device's current position.
    float                currentPosition;

    /// Device parameter:
    /// This value indicates, if device's position values increase (´false´) or decrease (`true`) while the task is executed.
    bool                 directionDown;
    
    /// Device parameter:
    /// This value indicates the device's current state.
    ///
    /// deviceState         | description
    /// --------------------|--------------------------------------------------------------------------
    /// SLEEPING            | device is idle, no pending moves
    /// MOVING              | device is performing a task
    /// MOVE_DONE           | device just finished a task
    /// PENDING_MOVES       | device is idle, tasks have to be executed
    deviceState          state;
    
    
    /// Parameter, related to the current task operation.
    /// The value means different things, depending on the device type.
    float               target;

    /// Parameter, related to the current task operation.
    /// The value means different things, depending on the device type.
    float               velocity;
    
    /// Parameter, related to the current task operation.
    /// The value means different things, depending on the device type.
    float               acceleration;
    
    /// Parameter, related to the current task operation.
    /// The value means different things, depending on the device type.
    float               deceleration;

    /// Parameter, related to the task controll of the current task.
    /// This parameter holds a delay to wait after occurance of the startEvent to start this task.
    unsigned long       startDelay;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter describes, how the tast is started:
    ///
    /// startEvent  | behavior
    /// ------------|-------------------------------------------------------------------------------
    /// NONE        | no specific starting given
    /// DATE        | starting at a specific time
    /// BUTTON      | starting on a hardware event
    /// FOLLOW      | starting when an other task of an other device is just finished
    /// POSITION    | starting when an other task of an other device has reached a specific position
    event                startEvent;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter describes, how the tast is started:
    ///
    /// stopEvent   | behavior
    /// ------------|-------------------------------------------------------------------------------
    /// NONE        | no specific stopping given
    /// DATE        | stopping after a timeout
    /// BUTTON      | stopping on a hardware event
    /// FOLLOW      | stopping when an other task of an other device is just finished
    /// POSITION    | stopping when an other task of an other device has reached a specific position
    event                stopEvent;

    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the time to start this task (ms after the scheduler was started).
    unsigned long        startTime;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the timeout to stop this task (ms after the task was started).
    unsigned long        timeout;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the pin number of a required hardware event to start this task.
    unsigned char        startButton;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the pin number of a required hardware event to stop this task.
    unsigned char        stopButton;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the required state of the pin to trigger the start of this task.
    bool                 startButtonState;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the required state of the pin to trigger the stop of this task.
    bool                 stopButtonState;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the index of a device, that shall trigger the start of this task.
    unsigned char        startTriggerDevice;

    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the index of a device, that shall trigger the stop of this task.
    unsigned char        stopTriggerDevice;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the index of a task of a device, on which the start of this task shall be triggered.
    unsigned char        startTriggerTask;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the index of a task of a device, on which the stop of this task shall be triggered.
    unsigned char        stopTriggerTask;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the position of a device on a task, on whitch the start of this task shall be triggered.
    signed long	         startTriggerPosition;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the position of a device on a task, on whitch the stop of this task shall be triggered.
    signed long	         stopTriggerPosition;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the the way stopping this task, when a stopping event occures.
    /// stoppingMode         | behavior
    /// ---------------------|--------------------------------------------------------------------
    /// STOP_IMMEDIATELY     | stop immediately
    /// STOP_NORMAL          | decelerate til halt with the given deceleration
    /// STOP_DYNAMIC         | approximate til a sensor shows a given value (reserved)
    stoppingMode         stopping;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter indicates if any ongoing task should be completed first, before this task is started.
    bool                 switchTaskPromptly;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the pin number of a required hardware event to stop this task.
    unsigned char        sensor;

    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the numerical value of the sensor pin, required to initiate the stopping of this task.
    signed int           initiatePerformanceValue;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the numerical value of the sensor pin, on whitch this task comletes.
    signed int           targetValue;

    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the feedback amplification while approximating the target position.
    float                stopPerformance;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the approximaton behavior, attaining the target position.
    /// approximationMode                         | behavior
    /// ------------------------------------------|--------------------------------------------------------------------
    /// SKIP_APPROXIMATION_IMMEDIATELY = 0x00     | stop immediately when the first sensor value raised the target
    /// SKIP_APPROXIMATION_VERY_FAST = 0x01       | stop when one value is within a luxuriantly tolerance
    /// SKIP_APPROXIMATION_FAST = 0x40            | ...
    /// SKIP_APPROXIMATION_PRECISE = 0x80         | ...
    /// SKIP_APPROXIMATION_VERY_PRECISE = 0xC0    | stop not until a huge amount of values are within a small tolerance
    /// SKIP_APPROXIMATION_NEVER = 0xFF           | keep approximating
    /// (skip when ´appriximationMode´ times the sensor value is within the tolerance of ´ +/- 256 / approximationMode´)
    approximationMode   approximation;
        
    /// Array of tasks to be performed.
    /// @see CCTask
    CCTask               *task[10];
    

    
    
    //        startTime, startDelay & startEvent could be changed by scheduler, so they need to exist aswell outside of the onEventTask
    
       
    
    virtual ~CCDevice();



    /// Function sets up device-specific default values for velocity, acceleration and deceleration.
    /// Call this function to be prepared for overloading [addTask(...)](@ref addTask) and [addTaskWithStartDelay(...)](@ref addTaskWithStartDelay) functions.
    /// @param defaultVelocity the device's default velocity.
    /// @param defaultAcceleration the device's default acceleration.
    /// @param defaultDeceleration the device's default deceleration.
    /// @see velocity
    /// @see acceleration
    /// @see deceleration
    void defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskWithStartDelay(...)](@ref addTaskWithStartDelay) with startDelay = 0.
    /// @param target the current task's target.
    /// @param velocity the current task's velocity.
    /// @param acceleration the current task's acceleration.
    /// @param deceleration the current task's deceleration.
    /// @return the task index.
    unsigned char addTask(float target, float velocity, float acceleration, float deceleration);
    
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target the current task's target.
    /// @param startDelay the current task's startDelay.
    /// @param velocity the current task's velocity.
    /// @param acceleration the current task's acceleration.
    /// @param deceleration the current task's deceleration.
    /// @return the task index.
    unsigned char addTaskWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration);
    
    
    /// Function deletes all tasks of the device.
    ///
    void deleteTasks();
    

    
    virtual void attachDevice() = 0;
    virtual void detachDevice() = 0;
    virtual void enableDevice() = 0;
    virtual void disableDevice() = 0;
    
    virtual void reviewValues() = 0;
    virtual void prepareNextTask() = 0;
    virtual void startTask() = 0;
    virtual void operateTask() = 0;
    virtual void initiateStop() = 0;
    virtual void stopTask() = 0;
    virtual void finishTask() = 0;
    
    virtual void setDriverControlRegister(boolean stepInterpolation, boolean doubleEdgeStepPulses, byte microSteppingMode);
    virtual void setChopperControlRegister_spreadCycle(byte blankingTimeValue, boolean chopperMode, boolean randomTOffTime, byte hysteresisDecrementPeriodValue, int hysteresisEnd, byte hysteresisStart, byte offTime);
    virtual void setChopperControlRegister_fastDecay(byte blankingTimeValue, boolean chopperMode, boolean randomTOffTime, boolean onlyTimerTerminatesDecayPhase, int sinwaveOffset, byte fastDecayTime, byte offTime);
    virtual void setCoolStepRegister(byte minCoolStepCurrentValue, byte currentDecrementSpeedValue, byte upperCoolStepThreshold, byte currentIncrementStepsValue, byte lowerCoolStepThreshold);
    virtual void setStallGuard2Register(boolean stallGuard2FilterEnable, int stallGuard2Threshold);
    virtual void setDriverConfigurationRegister(byte slopeControlHighSide, byte slopeControlLowSide, boolean shortToGndProtectionDisable, byte shortToGndDetectionTimerValue, boolean stepDirInterfaceDisable, byte selectReadOut);
    virtual void getReadOut(byte theReadOut);
    virtual void setCurrent(unsigned int current);
    
private:
    


    
};

#endif /* defined(__deviceScheduler__CCDevice__) */
