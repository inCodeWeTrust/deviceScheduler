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
    unsigned char       startTriggerDevice;
    unsigned char       startTriggerTask;
    signed long         startTriggerPosition;
    unsigned char       stopTriggerDevice;
    unsigned char       stopTriggerTask;
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
    void startAfterMy(unsigned char startTriggerTask);
    void startAfterCompletionOf(unsigned char startTriggerDevice, unsigned char startTriggerTask);
    void startByTriggerpositionOf(unsigned char startTriggerDevice, unsigned char startTriggerTask, signed long startTriggerPosition);
    
    void switchToNextTaskByDate(unsigned long switchingTimeout);
    void switchToNextTaskByButton(unsigned char switchingButton, boolean switchingButtonState);
    void switchToNextTaskAfterCompletionOf(unsigned char switchingTriggerDevice, unsigned char switchingTriggerTask);
    void switchToNextTaskByTriggerpositionOf(unsigned char switchingTriggerDevice, unsigned char switchingTriggerTask, signed long switchingTriggerPosition);
    
    void stopByTimeout(unsigned long timeout, stoppingMode stopping);
    void stopByButton(unsigned char stopButton, boolean stopButtonState, stoppingMode stopping);
    void stopAfterCompletionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerTask, stoppingMode stopping);
    void stopByTriggerpositionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping);
    
    void stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, approximationMode approximation);
    
};







#endif /* defined(__deviceScheduler__CCTask__) */
