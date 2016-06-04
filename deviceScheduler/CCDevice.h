//
//  CCDevice.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCDevice__
#define __deviceScheduler__CCDevice__


#include "deviceScheduler.h"

#include "CCTask.h"


//  verbosity:
#define CCDEVICE_BASICOUTPUT            0x01

#define CCDEVICE_VERBOSE                0


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCDevice
///
/// @brief base class for all devices
/// This baseclass holds all parameters, that are common for all devices, including the array of tasks to be executed by this device
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////
class CCDevice {
    
public:
    
    /// Getter method for getting the name of the device
    /// \sa deviceName;
    String getDeviceName();
    
    /// Getter method for getting the type of the device
    /// \sa deviceType;
    deviceType getType();
    
    /// Getter method for getting the number of tasks of the device
    /// \sa countOfTasks;
    unsigned char getCountOfTasks();

    /// Setter method for setting the number of tasks of the device
    /// \sa countOfTasks;
    void setCountOfTasks(unsigned char count);

    /// Getter method for getting the taskPointer of the device
    /// \sa taskPointer;
    unsigned char getTaskPointer();
    
    /// Setter method for setting the taskPointer of the device
    /// \sa taskPointer;
    void setTaskPointer(unsigned char pointer);
    
    /// Setter method for increasing the taskPointer of the device
    /// \sa taskPointer;
    void increaseTaskPointer();
    
    /// Getter method for getting the current position of the device
    /// \sa currentPosition;
    float getCurrentPosition();

    /// Setter method for setting the current position of the device
    /// \sa currentPosition;
    void setCurrentPosition(float position);
    
    /// Getter method for getting the target of the device
    /// \sa target;
    float getTarget();
    
    /// Getter method for getting the velocity of the device
    /// \sa velocity;
    float getVelocity();
    
    /// Getter method for getting the acceleration of the device
    /// \sa acceleration;
    float getAcceleration();
    
    /// Getter method for getting the deceleration of the device
    /// \sa deceleration;
    float getDeceleration();
    
    /// Getter method for getting the counting direction of the moving device
    /// \sa directionDown;
    bool getDirectionDown();
    
    /// Getter method for getting the state of the device
    /// \sa deviceState;
    deviceState getState();
    
    /// Setter method for Setting the state of the device
    /// \sa deviceState;
    void setState(deviceState state);
    
    /// Getter method for getting the startDelay of the device
    /// \sa startDelay;
    unsigned long getStartDelay();
    
    /// Setter method for setting the startDelay of the device
    /// \sa startDelay;
    void setStartDelay(unsigned long delay);
    
    /// Getter method for getting the startEvent of the device
    /// \sa startEvent, event;
    event getStartEvent();
    
    /// Setter method for setting the startEvent of the device
    /// \sa startEvent, event;
    void setStartEvent(event startEvent);
    
    /// Getter method for getting the stopEvent of the device
    /// \sa stopEvent, event;
    event getStopEvent();
    
    /// Setter method for setting the stopEvent of the device
    /// \sa stopEvent, event;
    void setStopEvent(event stopEvent);
    
    /// Getter method for getting the startTime of the device
    /// \sa startTime;
    unsigned long getStartTime();
    
    /// Setter method for setting the startTime of the device
    /// \sa startTime;
    void setStartTime(unsigned long startTime);
    
    /// Getter method for getting the timeOut of the device
    /// \sa timeOut;
    unsigned long getTimeout();
    
    /// Getter method for getting the startButton of the device
    /// \sa startButton;
    schedulerControlButton getStartButton();
    
    /// Getter method for getting the stopButton of the device
    /// \sa stopButton;
    schedulerControlButton getStopButton();
    
   /// Getter method for getting the startTriggerDevice of the device
    /// \sa startTriggerDevice;
    schedulerDevice getStartTriggerDevice();
   
    /// Getter method for getting the stopTriggerDevice of the device
    /// \sa stopTriggerDevice;
    schedulerDevice getStopTriggerDevice();
    
    /// Getter method for getting the startTriggerTask of the device
    /// \sa startTriggerTask;
    scheduledTask getStartTriggerTask();
    
    /// Getter method for getting the stopTriggerTask of the device
    /// \sa stopTriggerTask;
    scheduledTask getStopTriggerTask();
    
   /// Getter method for getting the startTriggerPosition of the device
    /// \sa startTriggerPosition;
    signed long getStartTriggerPosition();

    /// Getter method for getting the stopTriggerPosition of the device
    /// \sa stopTriggerPosition;
    signed long getStopTriggerPosition();

    /// Getter method for getting the stopping of the device
    /// \sa stopping, stoppingMode;
    stoppingMode getStopping();
    
    /// Setter method for setting the stopping of the device
    /// \sa stopping, stoppingMode;
    void setStopping(stoppingMode mode);
    
    /// Getter method for getting the switchTaskPromptly bit of the device
    /// \sa switchTaskPromptly;
    boolean getSwitchTaskPromptly();
    
    /// Setter method for setting the switchTaskPromptly bit of the device
    /// \sa switchTaskPromptly;
    void setSwitchTaskPromptly(boolean switchPromptly);
    
    /// Getter method for getting the sensor of the device
    /// \sa sensor;
    unsigned char getSensor();
    
    /// Getter method for getting the sensor value to initiate the dynamical stop of the device
    /// \sa initiatePerformanceValue;
    signed int getInitiatePerformanceValue();
    
    /// Getter method for getting the target sensor value of the device
    /// \sa targetValue;
    signed int getTargetValue();
    
    /// Getter method for getting the stopping performance of the device
    /// \sa stopPerformance;
    float getStopPerformance();
    
    /// Getter method for getting the approximation value of the device
    /// \sa approximation;
    approximationMode getApproximation();

    
    /// Array of tasks to be performed.
    /// @see CCTask
    CCTask               *task[10];
    
    
    //        startTime, startDelay & startEvent could be changed by scheduler, so they need to exist aswell outside of the onEventTask
    
    
     virtual ~CCDevice() = 0;


    /// Function sets up device-specific default values for velocity, acceleration and deceleration.
    /// Call this function to be prepared for overloading [addTask(...)](@ref addTask) function.
    /// @param defaultVelocity the device's default velocity.
    /// @param defaultAcceleration the device's default acceleration.
    /// @param defaultDeceleration the device's default deceleration.
    void defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration);

    /// Function sets up device-specific default values for velocity, acceleration and deceleration.
    /// It calls [addTask(...)](@ref addTask) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param defaultVelocity the device's default velocity.
    /// @param defaultAcceleration the device's default acceleration and deceleration value.
    void defineDefaults(float defaultVelocity, float defaultAcceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration value.
    /// @param deceleration this task's deceleration value.
    /// @return the task index.
    scheduledTask addTask(float target, float velocity, float acceleration, float deceleration);

    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration and deceleration value.
    /// @return the task index.
    scheduledTask addTask(float target, float velocity, float acceleration);

    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    scheduledTask addTask(float target, float velocity);

    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @return the task index.
    scheduledTask addTask(float target);
    
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    scheduledTask addTaskMoveRelativ(float relativTarget, float velocity, float acceleration, float deceleration);

    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @return the task index.
    scheduledTask addTaskMoveRelativ(float relativTarget, float velocity, float acceleration);

    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    scheduledTask addTaskMoveRelativ(float relativTarget, float velocity);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param relativTarget this task's relativ target.
    /// @return the task index.
    scheduledTask addTaskMoveRelativ(float relativTarget);
    
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    scheduledTask addTaskWithPositionReset(float target, float velocity, float acceleration, float deceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @return the task index.
    scheduledTask addTaskWithPositionReset(float target, float velocity, float acceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    scheduledTask addTaskWithPositionReset(float target, float velocity);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @return the task index.
    scheduledTask addTaskWithPositionReset(float target);
    
    
    /// Function deletes all tasks of the device.
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
    

    virtual void getReadOut(byte theReadOut) = 0;

    
protected:
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
    
    /// Parameter, related to the current task operation.
    /// The value determines, if the provided target value is meant absolute or relativ.
    boolean             moveRelativ;
    
    /// Parameter, related to the current task operation.
    /// The value determines, if current position is set to zero before job start.
    boolean             withPositionReset;
    
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
    schedulerControlButton        startButton;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the pin number of a required hardware event to stop this task.
    schedulerControlButton        stopButton;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the index of a device, that shall trigger the start of this task.
    schedulerDevice        startTriggerDevice;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the index of a device, that shall trigger the stop of this task.
    schedulerDevice        stopTriggerDevice;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the index of a task of a device, on which the start of this task shall be triggered.
    scheduledTask        startTriggerTask;
    
    /// Parameter, related to the task controll of the current task.
    /// This parameter holds the index of a task of a device, on which the stop of this task shall be triggered.
    scheduledTask        stopTriggerTask;
    
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
    
    /// Default parameters for the device.
    /// A value for the device's default velocity is provided here.
    float                defaultVelocity;
    
    /// Default parameters for the device.
    /// A value for the device's default acceleration is provided here.
    float                defaultAcceleration;
    
    /// Default parameters for the device.
    /// A value for the device's default deceleration is provided here.
    float                defaultDeceleration;

    
private:
    
    /// Device parameter:
    /// Value, set by the owner of the device (CCDeviceScheduler) to be recognized by him.
    unsigned int         deviceIndex;
    
    
    /// Function to register the tasks
    scheduledTask registerTask(float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, boolean withPositionReset);

    
};

#endif // defined(__deviceScheduler__CCDevice__)
