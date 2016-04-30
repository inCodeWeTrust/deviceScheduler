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
#include "CCDeviceScheduler.h"


#include "CCTask.h"





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
    /// Array of tasks to be performed.
    /// @see CCTask
    CCTask               *task[10];

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
    /// @return the task pointer to this task.
    scheduledTask addTask(float target, float velocity, float acceleration, float deceleration);
    
    
    /// Function declares a task to be executed and returns its index.
    /// It creates an instance of [CCTask](@ref task) and puts it into the task array of the device.
    /// @param target the current task's target.
    /// @param startDelay the current task's startDelay.
    /// @param velocity the current task's velocity.
    /// @param acceleration the current task's acceleration.
    /// @param deceleration the current task's deceleration.
    /// @return the task pointer to this task.
    scheduledTask addTaskWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration);
    
 
    
};

#endif /* defined(__deviceScheduler__CCDevice__) */


