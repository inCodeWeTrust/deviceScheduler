//
//  CCDeviceOperations.h
//  deviceScheduler
//
//  Created by little Ikarus on 16.06.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCDeviceOperations__
#define __deviceScheduler__CCDeviceOperations__

#include "deviceScheduler.h"
#include "CCTask.h"


class CCDeviceOperation {
    
    String name;
    
    /// Parameters of the device as a peer of a run-loop.
    /// Value, that holds the number of tasks to be executed within this run-loop.
    unsigned char        countOfTasks;
    
    /// Parameters of the device as a peer of a run-loop.
    /// Value, that holds the index of the current task.
    unsigned char        taskPointer;
    
    
    
    
public:
    /// Array of tasks to be performed.
    /// @see CCTask
    CCTask               *task[10];
    
    
    /// Getter method for getting the number of tasks of the device
    /// @sa countOfTasks;
    unsigned char getCountOfTasks();
    
    /// Setter method for manipulating the number of tasks of the device
    /// @sa countOfTasks;
    void setCountOfTasks();
    
    /// Getter method for getting the taskPointer of the device
    /// @sa taskPointer;
    unsigned char getTaskPointer();
    
    /// Setter method for setting the taskPointer of the device
    /// @sa taskPointer;
    void setTaskPointer();
    
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
    scheduledTask addTask(schedulerDevice device, float target, float velocity, float acceleration, float deceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration and deceleration value.
    /// @return the task index.
    scheduledTask addTask(schedulerDevice device, float target, float velocity, float acceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    scheduledTask addTask(schedulerDevice device, float target, float velocity);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTask(...)](@ref addTask) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @return the task index.
    scheduledTask addTask(schedulerDevice device, float target);
    
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    scheduledTask addTaskMoveRelativ(schedulerDevice device, float relativTarget, float velocity, float acceleration, float deceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @return the task index.
    scheduledTask addTaskMoveRelativ(schedulerDevice device, float relativTarget, float velocity, float acceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    scheduledTask addTaskMoveRelativ(schedulerDevice device, float relativTarget, float velocity);
    
    /// Function declares a task to be executed and returns its index.
    /// It calls [addTaskMoveRelativ(...)](@ref addTaskMoveRelativ) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param relativTarget this task's relativ target.
    /// @return the task index.
    scheduledTask addTaskMoveRelativ(schedulerDevice device, float relativTarget);
    
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    scheduledTask addTaskWithPositionReset(schedulerDevice device, float target, float velocity, float acceleration, float deceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [deceleration](@ref deceleration) = -[acceleration](@ref acceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @return the task index.
    scheduledTask addTaskWithPositionReset(schedulerDevice device, float target, float velocity, float acceleration);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @return the task index.
    scheduledTask addTaskWithPositionReset(schedulerDevice device, float target, float velocity);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It calls [addTaskWithPositionReset(...)](@ref addTaskWithPositionReset) with [defaultVelocity](@ref defaultVelocity), [defaultAcceleration](@ref defaultAcceleration) and [defaultDeceleration](@ref defaultDeceleration).
    /// @param target this task's target.
    /// @return the task index.
    scheduledTask addTaskWithPositionReset(schedulerDevice device, float target);
    
    
    /// Function deletes all tasks of the device.
    void deleteTasks(schedulerDevice device);
    
};

#endif /* defined(__deviceScheduler__CCDeviceOperations__) */
