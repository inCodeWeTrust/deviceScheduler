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

class CCDevice;
class CCControlButton;


class CCTask {

private:
    scheduledTask           taskID;
    event                   startEvent;
    event                   stopEvent;
    boolean                 switchTaskPromptly;
    float                   target;
    float                   velocity;
    float                   acceleration;
    float                   deceleration;
    boolean                 moveRelativ;
    positionResetMode       positionReset;
    unsigned long           startDelay;
    unsigned long           startTime;
    unsigned long           timeout;
    CCControlButton*        startButton;
    CCControlButton*        stopButton;
    CCDevice*               startTriggerDevice;
    scheduledTask           startTriggerTaskID;
    signed long             startTriggerPosition;
    CCDevice*               stopTriggerDevice;
    scheduledTask           stopTriggerTaskID;
    signed long             stopTriggerPosition;
    unsigned char           sensor;
    signed int              initiatePerformanceValue;
    signed int              targetValue;
    float                   stopPerformance;
    stoppingMode            stopping;
    unsigned int            approximationCurve;
    unsigned int            gap;
    boolean                 reversedApproximation;
    approximationMode       approximation;
    
    

   
public:
    
    
   
    void setStartDelay(unsigned long startDelay);
    
    void startByDate(unsigned long startTime);
    void startByButton(CCControlButton* startButton);
    void startAfterCompletionOf(CCDevice* startTriggerDevice, CCTask* startTriggerTask);
    void startByTriggerpositionOf(CCDevice* startTriggerDevice, CCTask* startTriggerTask, signed long startTriggerPosition);
    
    void switchToNextTaskByDate(unsigned long switchingTimeout);
    void switchToNextTaskByButton(CCControlButton* switchingButton);
    void switchToNextTaskAfterCompletionOf(CCDevice* switchingTriggerDevice, CCTask* switchingTriggerTask);
    void switchToNextTaskByTriggerpositionOf(CCDevice* switchingTriggerDevice, CCTask* switchingTriggerTask, signed long switchingTriggerPosition);
    
    void stopByTimeout(unsigned long timeout, stoppingMode stopping);
    void stopByButton(CCControlButton* stopButton, stoppingMode stopping = STOP_NORMAL);
    void stopAfterCompletionOf(CCDevice* stopTriggerDevice, CCTask* stopTriggerTask, stoppingMode stopping);
    void stopByTriggerpositionOf(CCDevice* stopTriggerDevice, CCTask* stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping);
    
    void stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, approximationMode approximation);
    
    void stopDynamicallyBySensor_new(unsigned char sensor, unsigned int targetValue, float approximationCurve, float gap, approximationMode approximation);
    
    

    CCTask(float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, positionResetMode positionReset, scheduledTask taskID);
    
    
    
    
    
    

    /// Getter method for getting the target of the device
    /// @sa target;
    scheduledTask getTaskID();
    
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

    /// Getter method for getting the positionReset mode of the device
    /// @sa positionReset;
    positionResetMode getPositionReset();
    
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
    CCControlButton* getStartButton();
    
    /// Getter method for getting the stopButton of the device
    /// @sa stopButton;
    CCControlButton* getStopButton();
  
    /// Getter method for getting the startTriggerDevice of the device
    /// @sa startTriggerDevice;
    CCDevice* getStartTriggerDevice();
    
    /// Getter method for getting the stopTriggerDevice of the device
    /// @sa stopTriggerDevice;
    CCDevice* getStopTriggerDevice();
    
    /// Getter method for getting the startTriggerTask of the device
    /// @sa startTriggerTask;
    scheduledTask getStartTriggerTaskID();
    
    /// Getter method for getting the stopTriggerTask of the device
    /// @sa stopTriggerTask;
    scheduledTask getStopTriggerTaskID();
    
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
    
    /// Getter method for getting the approximationCurve value of this task
    /// @sa approximationCurve;
    unsigned int getApproximationCurve();
    
    /// Getter method for getting the approximation value of this task
    /// @sa gap;
    unsigned int getGap();
    
    /// Getter method for getting the direction of the approximation value of this task
    /// @sa approximationCurve;
    boolean getReversedApproximation();
    
    
    
};







#endif // defined(__deviceScheduler__CCTask__)
