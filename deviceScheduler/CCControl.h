//
//  CCControl.h
//  ArduinoSimulator
//
//  Created by Little Abakus on 12.10.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//

#ifndef __ArduinoSimulator__CCControl__
#define __ArduinoSimulator__CCControl__


#include "deviceScheduler.h"
#include "CCAction.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCControl
///
/// @brief Class for physical interaction to control the workflow
///////////////////////////////////////////////////////////////////////////////////////////////////


#define CONTROLLBUTTON_PRESSED_NOTIFICATION     "endpoint reached"


class CCDeviceFlow;

class CCControl {

protected:
    
    int                 sensorValue, sensorValue_prev;
    
    String              controlName;
    controlType         type;
    unsigned int        controlIndex;
    unsigned int        pin;
    int                 mode;

    unsigned int        countOfActions;

public:
    
    CCAction*           action[8];
    
    virtual             ~CCControl() = 0;
    virtual void        read() = 0;
    
    int                 value();
    
    bool                isGreaterThen(int minimum);
    bool                isSmallerThen(int maximum);
    bool                is(int fix);
    bool                isNot(int fix);
    bool                isAbout(int fix, int tolerance = 10);
    
    bool                getDigitalValue();
    int                 getAnalogValue();
    
    
    
    String              getName();
    unsigned int        getCountOfActions();
    
//    CCAction*           addAction(String actionName);    
//    void                deleteActions();
    
    
private:
    
    
    
};

#endif /* defined(__ArduinoSimulator__CCControl__) */
