//
//  CCSwitchDevice.h
//  deviceScheduler
//
//  Created by Little Abakus on 30.05.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//


#ifndef __deviceScheduler__CCSWITCHDEVICE__
#define __deviceScheduler__CCSWITCHDEVICE__


#include <Arduino.h>
#include "CCDevice.h"


#define CCSWITCHDEVICE_BASICOUTPUT           0x01
#define CCSWITCHDEVICE_VERBOSE               CCSWITCHDEVICE_BASICOUTPUT





class CCSwitchDevice : public CCDevice {

  public:

    unsigned char       switching_pin;
    bool                defaultState;


    CCSwitchDevice(unsigned int deviceIndex, String deviceName, unsigned char switching_pin, boolean defaultState);
    ~CCSwitchDevice();


    void attachDevice();
    void detachDevice();
    void enableDevice();
    void disableDevice();
    
    void reviewValues();
    void prepareNextMove();
    void startMove();
    void drive();
    void initiateStop();
    void stopMoving();
    void finishMove();
};


#endif /* defined(__deviceScheduler__CCSwitchDevice__) */
