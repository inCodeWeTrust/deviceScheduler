//
//  CCControlButton.h
//  deviceScheduler
//
//  Created by Little Abakus on 01.04.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCControlButton__
#define __deviceScheduler__CCControlButton__


#include "deviceScheduler.h"



#define CCCONTROLBUTTON_BASICOUTPUT           0x01
#define CCCONTROLBUTTON_MEMORYDEBUG           0x02

#define CCCONTROLBUTTON_VERBOSE               0


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCControlButton
///
/// @brief Class for buttons to control the workflow
///////////////////////////////////////////////////////////////////////////////////////////////////


#define CONTROLLBUTTON_PRESSED_NOTIFICATION     "endpoint reached"

class CCControlButton {
    
    struct buttonAction {
    public:
        schedulerDevice     targetDevice;
        scheduledTask       validTask;
        deviceAction        targetAction;
        scheduledTask       followingTask;
        int                 notificationCode;
        String              notificationText;
        boolean             actionDone;
    };
    buttonAction        *action;

public:
    
    void                readButtonState();
    boolean             readIfActiv();
    boolean             getState();
    boolean             isActiv();
    
    buttonAction        getAction(unsigned char a);
    void                setActionDone(unsigned char a);
    String              getButtonName();
    unsigned char       getCountOfActions();
    String              getButtonActiv();
    
    
    CCControlButton(unsigned int buttonIndex, String buttonName, unsigned char button_pin, boolean buttonActiv, boolean pullup);
    ~CCControlButton();
    
    void evokeTaskJumpToTask(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction, scheduledTask followingTask, int notificationCode = 0, String notificationText = "");
    void evokeTaskJump(schedulerDevice targetDevice, scheduledTask validTask, deviceAction targetAction, int notificationCode = 0, String notificationText = "");
    
    void evokeBreak(schedulerDevice targetDevice, scheduledTask validTask, int notificationCode = 0, String notificationText = "");
    void evokeBreak(int notificationCode = 0, String notificationText = "");

    void deleteActions();
    
    
private:
    
    boolean             state;
    boolean             activ;

    String              buttonName;
    unsigned int        buttonIndex;
    unsigned char       button_pin;
    boolean             pullup;
    boolean             buttonActiv;
    unsigned char       countOfActions;

    
};

#endif // defined(__deviceScheduler__CCControlButton__)

