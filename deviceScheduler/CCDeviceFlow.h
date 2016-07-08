//
//  CCDeviceFlow.h
//  deviceScheduler
//
//  Created by little Ikarus on 16.06.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCDeviceFlow__
#define __deviceScheduler__CCDeviceFlow__

#include "deviceScheduler.h"
#include "CCTask.h"

class CCDevice;
class CCDeviceFlow {
public:
    
    String deviceFlowName;
    
    /// Parameters of the device as a peer of a run-loop.
    /// Value, that holds the number of tasks to be executed within this run-loop.
    unsigned char        countOfTasks;
    
    /// Parameters of the device as a peer of a run-loop.
    /// Value, that holds the index of the current task.
    unsigned char        taskPointer;
    
    CCDevice*            device;
    
    CCDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity, float defaultAcceleration, float defaultDeceleration);
    ~CCDeviceFlow();

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
    /// Default parameters for the device.
    /// A value for the device's default velocity is provided here.
    float                defaultVelocity;
    
    /// Default parameters for the device.
    /// A value for the device's default acceleration is provided here.
    float                defaultAcceleration;
    
    /// Default parameters for the device.
    /// A value for the device's default deceleration is provided here.
    float                defaultDeceleration;
    

    /// Array of tasks to be performed.
    /// @see CCTask
    CCTask               *task[10];
    
    
    /// Getter method for getting the number of tasks of the device
    /// @sa countOfTasks;
    unsigned char getCountOfTasks();
    
    /// Setter method for manipulating the number of tasks of the device
    /// @sa countOfTasks;
    void setCountOfTasks(unsigned char count);
    
    /// Getter method for getting the taskPointer of the device
    /// @sa taskPointer;
    unsigned char getTaskPointer();
    
    /// Setter method for setting the taskPointer of the device
    /// @sa taskPointer;
    void setTaskPointer(unsigned char pointer);
    
    /// Setter method for increasing the taskPointer of the device
    /// @sa taskPointer;
    void increaseTaskPointer();
    
    
    
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration value.
    /// @param deceleration this task's deceleration value.
    /// @return the task index.
    CCTask* addTask(float target, float velocity, float acceleration, float deceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration and deceleration value.
    /// @return the task index.
    CCTask* addTask(float target, float velocity, float acceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    CCTask* addTask(float target, float velocity);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @return the task index.
    CCTask* addTask(float target);
    
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    CCTask* addTaskMoveRelativ(float relativTarget, float velocity, float acceleration, float deceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @return the task index.
    CCTask* addTaskMoveRelativ(float relativTarget, float velocity, float acceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    CCTask* addTaskMoveRelativ(float relativTarget, float velocity);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param relativTarget this task's relativ target.
    /// @return the task index.
    CCTask* addTaskMoveRelativ(float relativTarget);
    
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    CCTask* addTaskWithPositionReset(float target, float velocity, float acceleration, float deceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @return the task index.
    CCTask* addTaskWithPositionReset(float target, float velocity, float acceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    CCTask* addTaskWithPositionReset(float target, float velocity);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @return the task index.
    CCTask* addTaskWithPositionReset(float target);
    
    /// Function to register the tasks
    CCTask* registerTask(float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, boolean withPositionReset);

    /*
    /// Function deletes all tasks of the device.
    void deleteTasks(schedulerDevice device);
    */
    
    
    /// Function lists all tasks of all registered devices.
    /// A list with all tasks and bare informations are presented.
    void getTask(unsigned char t);
    
    /// Function lists all tasks of all registered devices.
    /// A list with all tasks and bare informations are presented.
    void getAllTasks();
    
    /// Function calls the <reviewValues()> function of all devices.
    /// This causes the devices to approve, if all tasks are possible and do some basic calculations.
    /// This function needs to be called before the run loop is started.
    void reviewTasks();
    
    /// Function deletes all tasks of all registered devices.
    void deleteAllTasks();
    
   
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
    

    
    
    /// Function lists all actions of all registered buttons.
    /// A list with all actions and bare informations are presented.
    void getAllActions();
    
    
    /// Function lists all actions of the specified button.
    /// A list with all actions and bare informations are presented.
    /// @param theButton the index of the button in question.
    void getActionsForControlButton(unsigned char theButton);
    
    /// Function deletes all actions of all registered buttons.
    void deleteAllActions();
    
    
    
    /// The Scheduler's workflow array.


    
};

#endif /* defined(__deviceScheduler__CCDeviceFlow__) */
