//
//  CCDeviceScheduler.h
//  deviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCDeviceScheduler__
#define __deviceScheduler__CCDeviceScheduler__


#include <Arduino.h>
#include <avr/pgmspace.h>

#include "deviceScheduler.h"

#include "CCDevice.h"
#include "CCServoDevice.h"
#include "CCStepperDevice.h"
#include "CCStepperDevice_A4988.h"
#include "CCStepperDevice_TMC260.h"
#include "CCDcControllerDevice.h"

#include "CCControlButton.h"



//  log visuals:
#define DEVICESCHEDULER_BASICOUTPUT                     0x01

#define DEVICESCHEDULER_SHOW_TASK_VIEW                  0x10
#define DEVICESCHEDULER_SHOW_TAB_VIEW                   0x20

#define DEVICESCHEDULER_VERBOSE                         (DEVICESCHEDULER_BASICOUTPUT | DEVICESCHEDULER_SHOW_TASK_VIEW)





///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCDeviceScheduler
///
/// @brief Scheduler engine to manage all devices and their tasks.
///
/// The scheduler holds all devices and controls them by calling their startTask(), operateTask(), stopTask() etc. routines.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

class CCDeviceScheduler {
    
public:


    /// Scheduler parameter:
    /// Value holds the number of devices managed by him.
    unsigned char countOfDevices, countOfControlButtons;
    
    /// The Scheduler's device array.
    /// Array of all devices.
    CCDevice *device[8];
    CCControlButton *controlButton[8];
    
    CCDeviceScheduler();
    ~CCDeviceScheduler();
    


    /// Function adds a servo device to the device array and returns the index of the device.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param servo_pin the pin number of the servo's pwm-pin.
    /// @param minPosition the minimum PCM pulse width in microseconds corresponding to one end position.
    /// @param maxPosition the maximum PCM pulse width in microseconds corresponding to other end position.
    /// @param parkPosition the PCM pulse width in microseconds corresponding to the default position.
    /// @return the device index.
    unsigned char addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition);
    
    /// Function adds a stepper device to the device array and returns the index of the device.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param dir_pin the pin number of the stepper driver's direction pin.
    /// @param step_pin the pin number of the stepper driver's step pin.
    /// @param enable_pin the pin number of the stepper driver's enable pin.
    /// @param highestSteppingMode the number of available stepping modes (full step, half step, quarter step ...).
    /// @param stepModeCodesString a string containing the specific pin combinations for the different stepping modes.
    /// @param microStepPinsString a string containing the pin numbers of the micro stepping pins.
    /// i.e. "28, 30, 32": micro stepping pins are pin 28, pin 30 and pin 32
    /// @param stepsPerRotation the number of steps needed to make a full rotation.
    /// @return the device index.
    unsigned char addStepper_A4988(String deviceName, unsigned char dir_pin, unsigned char step_pin, unsigned char enable_pin, unsigned char highestSteppingMode, String stepModeCodesString, String microStepPinsString, unsigned int stepsPerRotation);
   
    
    unsigned char addStepper_TMC260(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char chipSelect_pin, unsigned int currentMax, unsigned int stepsPerRotation);
    
    
    /// Function adds a switching device to the device array and returns the index of the device.
    /// A switching device is a device, that is simply switched on or off. Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param switching_pin the pin number of the device's controll pin.
    /// @param switchingPin_activ the state of the switching pin, where the device is activ.
    /// @return the device index.
    unsigned char addDcController(String deviceName, unsigned char switching_pin, boolean switchingPin_activ);

  
    


    /// Function lists all registered devices.
    /// A list with all devices and bare informations are presented.
    void getAllDevices();
    
    /// Function lists all tasks of all registered devices.
    /// A list with all tasks and bare informations are presented.
    void getAllTasks();
    
    /// Function lists all tasks of the specified device.
    /// A list with all tasks and bare informations are presented.
    void getTasksForDevice(unsigned char theDevice);
    
    /// Function calls the <reviewValues()> function of all devices.
    /// This causes the devices to approve, if all tasks are possible and do some basic calculations.
    /// This function needs to be called before the run loop is started.
    void reviewTasks();

    /// Function deletes all tasks of all registered devices.
    void deleteAllTasks();
    
    
    
    
    
    
    unsigned char addControlButton(String buttonName, unsigned char button_pin, boolean button_activ);
    void getAllControlButtons();
    void getAllActions();
    void getActionsForControlButton(unsigned char theButton);

    void deleteAllActions();
    
    
    
    void run();
    void handleStartEvent(unsigned long taskTime, unsigned char device, event startEvent);
    void handleStopEvent(unsigned long taskTime, unsigned char device, event stopEvent);
    

private:
    deviceAction action;
    unsigned char actionDevice;
    char increaseTaskPointerBy;
    
    String getNameOfDeviceType(deviceType t);
    String getNameOfTaskEvent(event e);
    String getNameOfState(deviceState s);
    String getNameOfStoppingMode(stoppingMode s);
    String getNameOfDeviceAction(deviceAction d);
    String formatNumber(long theData, unsigned char len);

    
};



#endif /* defined(__deviceScheduler__CCDeviceScheduler__) */


