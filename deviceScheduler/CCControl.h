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



class CCDeviceFlow;

class CCControl {

protected:
    int verbosity;

    const String              controlName;
    const unsigned int        controlIndex;
    const controlType         type;
    const unsigned int        pin;
    const int                 mode;

    int                 sensorValue, sensorValue_prev;
    
    unsigned int        countOfActions;

    
public:
    
    CCControl(const String controlName, const unsigned int controlIndex, const controlType type, const unsigned int pin, const unsigned int mode);
    virtual             ~CCControl() = 0;
    virtual void        read() = 0;
    
    int                 value();
    
    bool                isGreaterThen(int minimum) const;
    bool                isSmallerThen(int maximum) const;
    bool                is(int fix) const;
    bool                isNot(int fix) const;
    bool                isAbout(int fix, int tolerance = 10) const;
    
    bool                getDigitalValue();
    int                 getAnalogValue();
    
    
    
    const String              getName() const;
    controlType         getType();
    
    
    
    void setVerbosity(int verbosity);

    
private:
    
    
    
};

#endif /* defined(__ArduinoSimulator__CCControl__) */
