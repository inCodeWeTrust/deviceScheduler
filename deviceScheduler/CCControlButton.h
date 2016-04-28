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



#define CCCONTROLBUTTON_BASICOUTPUT           0x01
#define CCCONTROLBUTTON_MEMORYDEBUG           0x02
#define CCCONTROLBUTTON_MOVEMENTDEBUG         0x04

#define CCCONTROLBUTTON_VERBOSE               0


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCControlButton
///
/// @brief Class for buttons to control the workflow
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCControlButton {
    
    
    unsigned int        buttonIndex;
    unsigned char       button_pin;
    boolean             button_activ;
    
public:
    String              buttonName;
    boolean             state;
    unsigned char       countOfActions;
    
    struct buttonAction {
    public:
        schedulerDevice     targetDevice;
        scheduledTask       validTask;
        deviceAction        targetAction;
        scheduledTask       followingTask;
        boolean             actionDone;
    };
    buttonAction        action[8];
    
    
    CCControlButton(unsigned int buttonIndex, String buttonName, unsigned char button_pin, boolean button_activ);
    ~CCControlButton();
    
    void evokeTaskJumpToTask(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction, scheduledTask followingTask);
    void evokeTaskJump(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction);
    
    void deleteActions();
    
    boolean getButtonState();
    boolean isActiv();
    
};

#endif /* defined(__deviceScheduler__CCControlButton__) */

