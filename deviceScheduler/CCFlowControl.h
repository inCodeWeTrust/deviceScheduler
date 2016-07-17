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
#include "CCAction.h"
#include "deviceScheduler.h"




class CCDeviceFlow;

class CCFlowControl {
public:
    
    String              controlName;

    CCControlButton*    button;
    unsigned char       countOfActions;
    CCAction*           action[8];
    
    
    unsigned char       getCountOfActions();
    
    
    CCFlowControl(String controlName, CCControlButton* button);
    ~CCFlowControl();
    
    CCAction*           addAction(String actionName, infoCode workflowInfoCode);
    
    void deleteActions();
    
    
private:
    
    
    
};


#endif /* defined(__deviceScheduler__CCFlowControl__) */
