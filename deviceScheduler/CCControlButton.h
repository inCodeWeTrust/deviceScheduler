//
//  CCControlButton.h
//  deviceScheduler
//
//  Created by Little Abakus on 01.04.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCControlButton__
#define __deviceScheduler__CCControlButton__



#include "CCControl.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCControlButton
///
/// @brief Class for buttons to control the workflow
///////////////////////////////////////////////////////////////////////////////////////////////////


#define CONTROLLBUTTON_PRESSED_NOTIFICATION     "endpoint reached"


class CCDeviceFlow;

class CCControlButton : public CCControl {
public:
    
 
    
    void        read();
    
    
    CCControlButton(unsigned int buttonIndex, String buttonName, unsigned int button_pin, int mode);
    ~CCControlButton();
    

    
};

#endif // defined(__deviceScheduler__CCControlButton__)

