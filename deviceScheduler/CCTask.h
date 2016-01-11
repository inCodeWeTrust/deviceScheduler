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


class CCTask {
    
public:
    
    event       startEvent;
    event       stopEvent;
    boolean     switchMovePromptly;
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
    unsigned char       startTriggerMove;
    signed long         startTriggerPosition;
    unsigned char       stopTriggerDevice;
    unsigned char       stopTriggerMove;
    signed long         stopTriggerPosition;
    boolean             stopSharply;
    boolean             stopDynamically;
    unsigned char       sensor;
    signed int          initiatePerformanceValue;
    signed int          targetValue;
    float               stopPerformance;
    unsigned char       stopMode;
    
    
    CCTask(float target, float velocity, float acceleration, float deceleration, unsigned long startDelay);

    void startByDate(unsigned long startTime);
    void startByButton(unsigned char startButton, boolean startButtonState);
    void startAfterMy(unsigned char startTriggerMove);
    void startAfterCompletionOf(unsigned char startTriggerDevice, unsigned char startTriggerMove);
    void startByTriggerpositionOf(unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
    
    void switchToNextTaskByDate(unsigned long switchingTimeout);
    void switchToNextTaskByButton(unsigned char switchingButton, boolean switchingButtonState);
    void switchToNextTaskAfterCompletionOf(unsigned char switchingTriggerDevice, unsigned char switchingTriggerMove);
    void switchToNextTaskByTriggerpositionOf(unsigned char switchingTriggerDevice, unsigned char switchingTriggerMove, signed long switchingTriggerPosition);
    
    void stopByTimeout(unsigned long timeout, boolean stopSharply);
    void stopByButton(unsigned char stopButton, boolean stopButtonState, boolean stopSharply);
    void stopAfterCompletionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerMove);
    void stopByTriggerpositionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply);
    
    void stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, unsigned char stopMode);
    
};







#endif /* defined(__deviceScheduler__CCTask__) */
