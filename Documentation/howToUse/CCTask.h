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

    /// Function sets up the task to be started at a specified starting time.
    /// \param startTime specifies the delay after the scheduler was started in milliseconds.
    void startByDate(unsigned long startTime);
    
    /// Function sets up the task to be started when a specified input pin has a specified value.
    /// \param startButton holds the pin number of the input pin, that should start this task.
    /// \param startButtonState specifies the state of the pin to start this task.
    void startByButton(unsigned char startButton, boolean startButtonState);
    
    /// Function sets up the task to be started when a specified device has finished a specified task.
    /// \param startTriggerDevice holds the device, that should start this task.
    /// \param startTriggerTask specifies a task of the device "startDevice". After it's completion the task is started.
    void startAfterCompletionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask);

    /// Function sets up the task to be started when a specified device has reached a specified position.
    /// \param startTriggerDevice holds the device, that should start this task.
    /// \param startTriggerTask specifies a task of the device "startDevice" whilst whitch the task is to be started.
    /// \param startTriggerPosition specifies a position of the device "startDevice" on it's task "startTriggerTask" at whitch the task is to be started.
    void startByTriggerpositionOf(schedulerDevice startTriggerDevice, scheduledTask startTriggerTask, signed long startTriggerPosition);
    
    /// Function sets up the task to be switched to the next task after a specified timeout.
    /// \param switchingTimeout specifies the delay after the task was started in milliseconds.
    void switchToNextTaskByDate(unsigned long switchingTimeout);

    /// Function sets up the task to be switched to the next task when a specified input pin has a specified value.
    /// \param switchingButton holds the pin number of the input pin, that should switch this task.
    /// \param switchingButtonState specifies the state of the pin "switchingButton" to switch this task.
    void switchToNextTaskByButton(unsigned char switchingButton, boolean switchingButtonState);
    
    /// Function sets up the task to be switched to the next task when a specified device has finished a specified task.
    /// \param switchingTriggerDevice holds the device, that should start this task.
    /// \param switchingTriggerTask specifies a task of the device "switchingTriggerDevice". After it's completion the task is switched.
    void switchToNextTaskAfterCompletionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask);
    
    /// Function sets up the task to be switched to the next task when a specified device has reached a specified position.
    /// \param switchingTriggerDevice holds the device, that should switch this task.
    /// \param switchingTriggerTask specifies a task of the device "switchingTriggerDevice" whilst whitch the task is to be switched.
    /// \param switchingTriggerPosition specifies a position of the device "switchingTriggerDevice" on it's task "switchingTriggerTask" at whitch the task is to be switched.
    void switchToNextTaskByTriggerpositionOf(schedulerDevice switchingTriggerDevice, scheduledTask switchingTriggerTask, signed long switchingTriggerPosition);
    
    /// Function sets up the task to be stopped after a specified timeout.
    /// \param timeout specifies the delay after the task was started in milliseconds.
    /// \param stopping specifies the mode to stop the task. Possible values are: "STOP_IMMEDIATELY" and "STOP_NORMAL". Passing "STOP_IMMEDIATELY" will stop the device at once, when the timeout expired. Passing "STOP_NORMAL" will ramp down the device with the task's deceleration.
    /// \sa addTask()
    void stopByTimeout(unsigned long timeout, stoppingMode stopping);

    /// Function sets up the task to be stopped when a specified input pin has a specified value.
    /// \param stopButton holds the pin number of the input pin, that should stop this task.
    /// \param stopButtonState specifies the state of the pin to stop this task.
    /// \param stopping specifies the mode to stop the task. Possible values are: "STOP_IMMEDIATELY" and "STOP_NORMAL". Passing "STOP_IMMEDIATELY" will stop the device at once, when the timeout expired. Passing "STOP_NORMAL" will ramp down the device with the task's deceleration.
    /// \sa addTask()
    void stopByButton(unsigned char stopButton, boolean stopButtonState, stoppingMode stopping);

    /// Function sets up the task to be stopped when a specified device has finished a specified task.
    /// \param stopTriggerDevice holds the device, that should stop this task.
    /// \param stopTriggerTask specifies a task of the device "stopTriggerDevice". After it's completion the task is stopped.
    /// \param stopping specifies the mode to stop the task. Possible values are: "STOP_IMMEDIATELY" and "STOP_NORMAL". Passing "STOP_IMMEDIATELY" will stop the device at once, when the timeout expired. Passing "STOP_NORMAL" will ramp down the device with the task's deceleration.
    /// \sa addTask()
    void stopAfterCompletionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, stoppingMode stopping);

    /// Function sets up the task to be stopped when a specified device has reached a specified position.
    /// \param stopTriggerDevice holds the device, that should stop this task.
    /// \param stopTriggerTask specifies a task of the device "stopTriggerDevice" whilst whitch the task is to be stopped.
    /// \param stopTriggerPosition specifies a position of the device "stopTriggerDevice" on it's task "stopTriggerTask" at whitch the task is to be stopped.
    /// \param stopping specifies the mode to stop the task. Possible values are: "STOP_IMMEDIATELY" and "STOP_NORMAL". Passing "STOP_IMMEDIATELY" will stop the device at once, when the timeout expired. Passing "STOP_NORMAL" will ramp down the device with the task's deceleration.
    /// \sa addTask()
    void stopByTriggerpositionOf(schedulerDevice stopTriggerDevice, scheduledTask stopTriggerTask, signed long stopTriggerPosition, stoppingMode stopping);
    
    /// Function sets up the task to be decelerated and stopped when a specified sensor reaches specified values. The device is decelerated when the sensor value "initiatePerformanceValue" is exceeded. The velocity while the approximation is controlled by the error amplifying value "stopPerformance" and the device is stopped when the sensor values have stabilized at "targetValue".
    /// \param sensor holds the pin number of the sensor, that should determine task process.
    /// \param initiatePerformanceValue specifies a value of the sensor from withch on the approximation process is started.
    /// \param targetValue specifies the desired target value of the sensor "sensor" where teh task is to be stopped.
    /// \param stopPerformance specifies an error amplifying factor that determines the velocity of the device when approximating the target sensor value "targetValue".
    /// \param approximation specifies the precision of the approximation. Possible values are: "SKIP_APPROXIMATION_IMMEDIATELY", "SKIP_APPROXIMATION_VERY_FAST", "SKIP_APPROXIMATION_FAST", "SKIP_APPROXIMATION_PRECISE", "SKIP_APPROXIMATION_VERY_PRECISE" and "SKIP_APPROXIMATION_NEVER"
    void stopDynamicallyBySensor(unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int targetValue, float stopPerformance, approximationMode approximation);
    
};







#endif /* defined(__deviceScheduler__CCTask__) */








