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

    unsigned char       startEvent;
    unsigned char       stopEvent;
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
    signed int          stopValue;
    float               stopPerformance;
    unsigned char       stopMode;
    
    
    CCTask(float target, float velocity, float acceleration, float deceleration, unsigned long startDelay);
    
    void startByDate(unsigned long startTime);
    void startByButton(unsigned char startButton, boolean startButtonState);
    void startAfterCompletion(unsigned char moveIndex);
    void startAfterCompletionOf(unsigned char startTriggerDevice, unsigned char startTriggerMove);
    void startByTriggerposition(unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
    
    void switchByDate(unsigned long startTime);
    void switchByButton(unsigned char startButton, boolean startButtonState);
    void switchByTriggerposition(unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition);
    
    void stopByTimeout(unsigned long _timeout, boolean stopSharply);
    void stopByButton(unsigned char _stopButton, boolean _stopButtonState, boolean stopSharply);
    void stopAfterCompletionOf(unsigned char stopTriggerDevice, unsigned char stopTriggerMove);
    void stopByTriggerposition(unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply);
    
    void stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int stopValue, float stopPerformance, unsigned char stopMode);
    
    
};







#endif /* defined(__deviceScheduler__CCTask__) */
