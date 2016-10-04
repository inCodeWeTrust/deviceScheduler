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
#include "CCAction.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCControlButton
///
/// @brief Class for buttons to control the workflow
///////////////////////////////////////////////////////////////////////////////////////////////////


#define CONTROLLBUTTON_PRESSED_NOTIFICATION     "endpoint reached"


class CCDeviceFlow;

class CCControlButton {
public:
    
    CCAction*           action[8];

    
    void                readButtonState();
    bool             readIfActiv();
    bool             getState();
    bool             isActiv();
    
    String              getName();
    unsigned char       getCountOfActions();
    String              getButtonActiv();
    
    
    CCControlButton(unsigned int buttonIndex, String buttonName, unsigned char button_pin, bool buttonActiv, bool pullup);
    ~CCControlButton();
    
    CCAction*           addAction(String actionName);
   
    void deleteActions();
    
    
private:
    
    bool             state;
    bool             activ;

    String              buttonName;
    unsigned int        buttonIndex;
    unsigned char       button_pin;
    bool             pullup;
    bool             buttonActiv;
    unsigned char       countOfActions;

    
};

#endif // defined(__deviceScheduler__CCControlButton__)

