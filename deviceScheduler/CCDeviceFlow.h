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
private:
    
    String deviceFlowName;

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
    

    
public:
    
    /// Array of tasks to be performed.
    /// @see CCTask
    CCTask*             task[10];
    
    CCDevice*           device;
    
    
    
    CCDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity, float defaultAcceleration, float defaultDeceleration);
    ~CCDeviceFlow();
  
    
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration value.
    /// @param deceleration this task's deceleration value.
    /// @return the task index.
    CCTask* addTask(float target, float velocity = 0.0, float acceleration = 0.0, float deceleration = 0.0);
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param relativTarget this task's relativ target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    CCTask* addTaskMoveRelativ(float relativTarget, float velocity = 0.0, float acceleration = 0.0, float deceleration = 0.0);
    
    /// Function declares a task to be executed and returns its index.
    /// Before the task is executed, a position reset is performed.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    CCTask* addTaskWithPositionReset(float target, float velocity = 0.0, float acceleration = 0.0, float deceleration = 0.0);
    
    /// Function declares a task to be executed and returns its index.
    /// After the task is executed, a position reset is performed.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target this task's target.
    /// @param velocity this task's velocity.
    /// @param acceleration this task's acceleration.
    /// @param deceleration this task's deceleration.
    /// @return the task index.
    CCTask* addTaskWithPositionResetOnCompletion(float target, float velocity = 0.0, float acceleration = 0.0, float deceleration = 0.0);

    /// Function to register the tasks
    CCTask* registerTask(float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, positionResetMode positionReset);

    
    /// Function lists all tasks of all registered devices.
    /// A list with all tasks and bare informations are presented.
    void getTask(unsigned char t);
    
    /// Function lists all tasks of all registered devices.
    /// A list with all tasks and bare informations are presented.
    void getAllTasks();
    
   
    /// Function sets up device-specific default values for velocity, acceleration and deceleration.
    /// Call this function to be prepared for overloading [addTask(...)](@ref addTask) function.
    /// @param defaultVelocity the device's default velocity.
    /// @param defaultAcceleration the device's default acceleration.
    /// @param defaultDeceleration the device's default deceleration.
    void defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration = 0.0);

    
    
    
    /// Getter method for getting the name of this deviceFlow
    /// @sa deviceFlowName;
    String          getName();
    
    /// Getter method for getting the device's default velocity within this deviceFlow.
    /// @sa defaultVelocity;
    float           getDefaultVelocity();
    
    /// Getter method for getting the device's default acceleration within this deviceFlow.
    /// @sa defaultAcceleration;
    float           getDefaultAcceleration();
    
    /// Getter method for getting the device's default deceleration within this deviceFlow.
    /// @sa defaultDeceleration;
    float           getDefaultDeceleration();
    
    /// Getter method for getting the number of tasks of the device
    /// @sa countOfTasks;
    unsigned char   getCountOfTasks();
    
    /// Setter method for manipulating the number of tasks of the device
    /// @sa countOfTasks;
    void            setCountOfTasks(unsigned char count);
    
    /// Getter method for getting the taskPointer of the device
    /// @sa taskPointer;
    unsigned char   getTaskPointer();
    
    /// Setter method for setting the taskPointer of the device
    /// @sa taskPointer;
    void            setTaskPointer(unsigned char pointer);
    
    /// Setter method for increasing the taskPointer of the device
    /// @sa taskPointer;
    void            increaseTaskPointer();
    
    
};

#endif /* defined(__deviceScheduler__CCDeviceFlow__) */
