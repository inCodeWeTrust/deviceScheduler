//
//  CCTask.h
//  deviceScheduler
//
//  Created by little Ikarus on 07.01.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCTask__
#define __deviceScheduler__CCTask__

#include <Arduino.h>
#include "deviceScheduler.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCTask
///
/// @brief Class for tasks to be executed
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCTask {
    
public:

    
    event       startEvent;
    event       stopEvent;
    boolean     switchTaskPromptly;
    float               target;
    float               velocity;
    float               acceleration;
    float               deceleration;
    unsigned long       startDelay;
    unsigned long       startTime;
    unsigned long       timeout;
    unsigned char       startButton;
    unsigned char       stopButton;
    boolean             startButtonState;
    boolean             stopButtonState;
    schedulerDevice     startTriggerDevice;
    scheduledTask       startTriggerTask;
    signed long         startTriggerPosition;
    schedulerDevice     stopTriggerDevice;
    scheduledTask       stopTriggerTask;
    signed long         stopTriggerPosition;
    unsigned char       sensor;
    signed int          initiatePerformanceValue;
    signed int          targetValue;
    float               stopPerformance;
    stoppingMode        stopping;
    approximationMode   approximation;
    
    
    CCTask(float target, float velocity, float acceleration, float deceleration, unsigned long startDelay);



    void startByDate(unsigned long startTime);
    void startByButton(unsigned char startButton, boolean startButtonState);
    void startAfterCompletionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask);
    void startByTriggerpositionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask, signed long startTriggerPosition);
    
    void switchToNextTaskByDate(unsigned long switchingTimeout);
    void switchToNextTaskByButton(unsigned char switchingButton, boolean switchingButtonState);
    void switchToNextTaskAfterCompletionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask);
    void switchToNextTaskByTriggerpositionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask, signed long switchingTriggerPosition);
    
    void stopByTimeout(unsigned long timeout, stoppingMode stopping);
    void stopByButton(unsigned char stopButton, boolean stopButtonState, stoppingMode stopping);
    void stopAfterCompletionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, stoppingMode stopping);
    void stopByTriggerpositionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping);
    
    void stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, approximationMode approximation);
    
};







#endif /* defined(__deviceScheduler__CCTask__) */
