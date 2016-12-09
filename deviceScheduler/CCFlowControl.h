//
//  CCFlowControl.h
//  deviceScheduler
//
//  Created by little Ikarus on 12.07.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#ifndef __deviceScheduler__CCFlowControl__
#define __deviceScheduler__CCFlowControl__

#include "CCControlButton.h"
#include "CCControlSensor.h"
#include "CCAction.h"
#include "deviceScheduler.h"




class CCDeviceFlow;

class CCFlowControl {
    
private:
    int verbosity;

    
    String              controlName;

    comparingMode       comparing;
    int                 target;
    unsigned int        countOfActions;

    
public:

    CCControl*          control;
    CCAction*           action[8];
    
    
    
    CCFlowControl(String controlName, CCControl* control, comparingMode comparing, int target);
    ~CCFlowControl();
    
    CCAction*           addAction(String actionName, workflowInfoCode workflowInfo);

    bool                needsToFire();
    
    comparingMode       getComparing();
    int                 getTarget();
    String              getName();
    unsigned int       getCountOfActions();
    
    void setVerbosity(int verbosity);

};


#endif /* defined(__deviceScheduler__CCFlowControl__) */
