//
//  CCTask.h
//  deviceScheduler
//
//  Created by little Ikarus on 07.01.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCTask__
#define __deviceScheduler__CCTask__


#include "deviceScheduler.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCTask
///
/// @brief Class for tasks to be executed
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCTask {
    
public:

    /// Getter method for getting the target of the device
    /// @sa target;
    float getTarget();
    
    /// Setter method for setting the target of the device
    /// @sa target;
    void setTarget(float target);
    
    /// Getter method for getting the velocity of the device
    /// @sa velocity;
    float getVelocity();
    
    /// Setter method for setting the velocity of the device
    /// @sa velocity;
    void setVelocity(float velocity);
    
    /// Getter method for getting the acceleration of the device
    /// @sa acceleration;
    float getAcceleration();
    
    /// Setter method for setting the acceleration of the device
    /// @sa acceleration;
    void setAcceleration(float acceleration);
    
    /// Getter method for getting the deceleration of the device
    /// @sa deceleration;
    float getDeceleration();
    
    /// Setter method for setting the deceleration of the device
    /// @sa deceleration;
    void setDeceleration(float deceleration);
    
    /// Getter method for getting the moveRelativ bit of the device
    /// @sa moveRelativ;
    boolean getMoveRelativ();

    /// Getter method for getting the withPositionReset bit of the device
    /// @sa withPositionReset;
    boolean getWithPositionReset();
    
    /// Getter method for getting the startDelay of the device
    /// @sa startDelay;
    unsigned long getStartDelay();
    
    /// Getter method for getting the startEvent of the device
    /// @sa startEvent, event;
    event getStartEvent();
    
    /// Getter method for getting the stopEvent of the device
    /// @sa stopEvent, event;
    event getStopEvent();
    
    /// Getter method for getting the startTime of the device
    /// @sa startTime;
    unsigned long getStartTime();
    
    /// Getter method for getting the timeOut of the device
    /// @sa timeOut;
    unsigned long getTimeout();
    
    /// Getter method for getting the startButton of the device
    /// @sa startButton;
    schedulerControlButton getStartButton();
    
    /// Getter method for getting the stopButton of the device
    /// @sa stopButton;
    schedulerControlButton getStopButton();
  
    /// Getter method for getting the startTriggerDevice of the device
    /// @sa startTriggerDevice;
    schedulerDevice getStartTriggerDevice();
    
    /// Getter method for getting the stopTriggerDevice of the device
    /// @sa stopTriggerDevice;
    schedulerDevice getStopTriggerDevice();
    
    /// Getter method for getting the startTriggerTask of the device
    /// @sa startTriggerTask;
    scheduledTask getStartTriggerTask();
    
    /// Getter method for getting the stopTriggerTask of the device
    /// @sa stopTriggerTask;
    scheduledTask getStopTriggerTask();
    
    /// Getter method for getting the startTriggerPosition of the device
    /// @sa startTriggerPosition;
    signed long getStartTriggerPosition();
    
    /// Getter method for getting the stopTriggerPosition of the device
    /// @sa stopTriggerPosition;
    signed long getStopTriggerPosition();
    
    /// Getter method for getting the stopping of the device
    /// @sa stopping, stoppingMode;
    stoppingMode getStopping();
    
    /// Getter method for getting the switchTaskPromptly bit of the device
    /// @sa switchTaskPromptly;
    boolean getSwitchTaskPromptly();
    
    /// Getter method for getting the sensor pin of the device
    /// @sa sensor;
    unsigned char getSensor();

    /// Getter method for getting the sensor value to initiate the dynamical stop of the device
    /// @sa initiatePerformanceValue;
    signed int getInitiatePerformanceValue();
    
    /// Getter method for getting the target sensor value of the device
    /// @sa targetValue;
    signed int getTargetValue();
    
    /// Getter method for getting the stopping performance of the device
    /// @sa stopPerformance;
    float getStopPerformance();
    
    /// Getter method for getting the approximation value of the device
    /// @sa approximation;
    approximationMode getApproximation();
    
    
    CCTask(float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, boolean withPositionReset);


    void setStartDelay(unsigned long startDelay);

    void startByDate(unsigned long startTime);
    void startByButton(schedulerControlButton startButton);
    void startAfterCompletionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask);
    void startByTriggerpositionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask, signed long startTriggerPosition);
    
    void switchToNextTaskByDate(unsigned long switchingTimeout);
    void switchToNextTaskByButton(schedulerControlButton switchingButton);
    void switchToNextTaskAfterCompletionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask);
    void switchToNextTaskByTriggerpositionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask, signed long switchingTriggerPosition);
    
    void stopByTimeout(unsigned long timeout, stoppingMode stopping);
    void stopByButton(schedulerControlButton stopButton, stoppingMode stopping = STOP_NORMAL);
    void stopAfterCompletionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, stoppingMode stopping);
    void stopByTriggerpositionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping);
    
    void stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, approximationMode approximation);
    
    
private:
    
    event                   startEvent;
    event                   stopEvent;
    boolean                 switchTaskPromptly;
    float                   target;
    float                   velocity;
    float                   acceleration;
    float                   deceleration;
    boolean                 moveRelativ;
    boolean                 withPositionReset;
    unsigned long           startDelay;
    unsigned long           startTime;
    unsigned long           timeout;
    schedulerControlButton  startButton;
    schedulerControlButton  stopButton;
    schedulerDevice         startTriggerDevice;
    scheduledTask           startTriggerTask;
    signed long             startTriggerPosition;
    schedulerDevice         stopTriggerDevice;
    scheduledTask           stopTriggerTask;
    signed long             stopTriggerPosition;
    unsigned char           sensor;
    signed int              initiatePerformanceValue;
    signed int              targetValue;
    float                   stopPerformance;
    stoppingMode            stopping;
    approximationMode       approximation;
    
};







#endif // defined(__deviceScheduler__CCTask__)
