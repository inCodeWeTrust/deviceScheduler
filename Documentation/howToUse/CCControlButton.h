//
//  CCControlButton.h
//  deviceScheduler
//
//  Created by Little Abakus on 01.04.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCControlButton__
#define __deviceScheduler__CCControlButton__

#include <Arduino.h>
#include "deviceScheduler.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCControlButton
///
/// @brief Class for buttons to control the workflow
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCControlButton {
    
   
public:
    /// Function sets up a mechanism to maipulate the normal task order of the scheduler. If an appropriate signal appears, pending tasks, that are waiting for their start event can be started or the task order is rewinded or forwarded. So it's possible to start, stop, skip, repeat or jump over tasks inside the scheduler's task order.
    /// \param targetDevice specifies the device whose task order shall become manipulated.
    /// \param validTask specifies the task at whitch the manipulation shall take place.
    /// \param targetAction specifies the manipulation mode. Possible values are: "START", "STOP_AND_SWITCH", "STOP", "STOP_SHARP_AND_SWITCH" and "STOP_SHARP"
    /// \param followingTask specifies the task to switch to.
    void evokeTaskJumpToTask(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction, scheduledTask followingTask);

    /// Function sets up a mechanism to start a pending task, stop a running task or skip a running task and jumping over to the next task.
    /// \param targetDevice specifies the device whose task order shall become forwarded.
    /// \param validTask specifies the task at whitch the manipulation shall take place.
    /// \param targetAction specifies the manipulation mode. Possible values are: "START", "STOP_AND_SWITCH", "STOP", "STOP_SHARP_AND_SWITCH" and "STOP_SHARP"
    void evokeTaskJump(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction);
};

#endif /* defined(__deviceScheduler__CCControlButton__) */

