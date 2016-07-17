//
//  CCWorkflow.h
//  deviceScheduler
//
//  Created by little Ikarus on 16.06.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCWorkflow__
#define __deviceScheduler__CCWorkflow__


#include "deviceScheduler.h"
#include "CCDeviceFlow.h"
#include "CCFlowControl.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCStepperDevice
///
/// @brief Device class for stepper motor devices
///
///////////////////////////////////////////////////////////////////////////////////////////////////




class CCWorkflow {
    
public:
    String workflowName;
    
    /// Parameters of the device as a peer of a run-loop.
    /// Value, that holds the number of tasks to be executed within this run-loop.
    unsigned char       countOfDeviceFlows;
    CCDeviceFlow        *deviceFlow[10];
    
    
    unsigned char       countOfFlowControls;
    CCFlowControl       *flowControl[6];
    
    
    infoCode            workflowInfo;
    String              infoHistory;

    
    CCWorkflow(String name);
    ~CCWorkflow();
    
    /// Function declares a task-flow for a device and returns its index.
    /// It creates an instance of [CCDeviceFlow](@ref deviceFlow) and puts it into the deviceFlow array of the workflow.
    /// @param device the index of the device.
    /// @param defaultVelocity the device's default velocity within this workflow.
    /// @param defaultAcceleration the device's default acceleration within this workflow.
    /// @param defaultDeceleration the device's default deceleration within this workflow.
    /// @return the deviceFlow index.
    CCDeviceFlow* addDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity, float defaultAcceleration, float defaultDeceleration);
    
    /// Function declares a task-flow for a device and returns its index.
    /// It creates an instance of [CCDeviceFlow](@ref deviceFlow) and puts it into the deviceFlow array of the workflow.
    /// @param device the index of the device.
    /// @param defaultVelocity the device's default velocity within this workflow.
    /// @param defaultAcceleration the device's default acceleration within this workflow.
    /// deceleration is set to same value as to acceleration.
    /// @return the deviceFlow index.
    CCDeviceFlow* addDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity, float defaultAcceleration);
    
    /// Function declares a task-flow for a device and returns its index.
    /// It creates an instance of [CCDeviceFlow](@ref deviceFlow) and puts it into the deviceFlow array of the workflow.
    /// @param device the index of the device.
    /// @param defaultVelocity the device's default velocity within this workflow.
    /// @return the deviceFlow index.
    CCDeviceFlow* addDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity);
    
    /// Function declares a task-flow for a device and returns its index.
    /// It creates an instance of [CCDeviceFlow](@ref deviceFlow) and puts it into the deviceFlow array of the workflow.
    /// @param device the index of the device.
    /// @return the deviceFlow index.
    CCDeviceFlow* addDeviceFlow(String deviceFlowName, CCDevice* device);
    
    
    
    
    
    CCFlowControl* addFlowControl(String controlName, CCControlButton* controlButton);

    
//    
//    String getNameOfDeviceType(deviceType t);
//    String getNameOfTaskEvent(event e);
//    String getNameOfState(deviceState s);
//    String getNameOfStoppingMode(stoppingMode s);
//    String getNameOfDeviceAction(deviceAction d);

};











#endif /* defined(__deviceScheduler__CCWorkflow__) */
