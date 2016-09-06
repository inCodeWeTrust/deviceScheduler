//
//  CCDeviceScheduler.h
//  deviceScheduler
//
//  Created by Little Abakus on 09.06.14.
//  Copyright (c) 2014 Little Abakus. All rights reserved.
//

#ifndef __deviceScheduler__CCDeviceScheduler__
#define __deviceScheduler__CCDeviceScheduler__



#include "deviceScheduler.h"

#include "CCWorkflow.h"

#include "CCDevice.h"
#include "CCServoDevice.h"
#include "CCServoDevice_cross.h"
#include "CCStepperDevice.h"
#include "CCStepperDevice_A4988.h"
#include "CCStepperDevice_TMC260.h"
#include "CCDcControllerDevice.h"
#include "CCDcControllerDevice_fullBridge.h"

#include "CCControlButton.h"

#define DEVICESCHEDULER_VERBOSE                         0x00


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCDeviceScheduler
///
/// @brief Scheduler engine to manage all devices and their tasks.
///
/// The scheduler holds all devices and controls them by calling their startTask(), operateTask(), stopTask() etc. routines.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


class CCDeviceScheduler {

    /// The Scheduler's name
    /// Value, set by the user to be recognized by him.
    String   schedulerName;
    
    /// Scheduler parameter:
    /// Value holds the number of devices managed by him.
    unsigned char countOfDevices, countOfControlButtons;
    

    CCDeviceFlow* currentDeviceFlow;
    CCDeviceFlow* triggerDeviceFlow_start;
    CCDeviceFlow* triggerDeviceFlow_stop;
    CCTask* currentTask;
    CCDevice* currentDevice;
    CCDevice* triggerDevice_start;
    CCDevice* triggerDevice_stop;
    
    CCControlButton* currentButton;
    CCAction* currentAction;
    CCFlowControl* currentFlowControl;
    
    int notificationCode;
    String  notificationText;
    
    
    void handleStartEvent(unsigned long taskTime, CCDeviceFlow* currentDeviceFlow);
    void handleStopEvent(unsigned long taskTime, CCDeviceFlow* currentDeviceFlow);
    
    String getNameOfDeviceType(deviceType t);
    String getNameOfTaskEvent(event e);
    String getNameOfState(deviceState s);
    String getNameOfStoppingMode(stoppingMode s);
    String getNameOfDeviceAction(deviceAction d);
    String getLiteralOfWorkflowInfo(infoCode i);
    

public:
    
    
    CCDeviceScheduler(String schedulerName);
    ~CCDeviceScheduler();
    

    /// The Scheduler's device array.
    /// Array of all devices.
    CCDevice *device[14];
    
    /// Function adds a servo device to the device array and returns the index of the device.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param servo_pin the pin number of the servo's pwm-pin.
    /// @param minPosition the minimum PCM pulse width in microseconds corresponding to one end position.
    /// @param maxPosition the maximum PCM pulse width in microseconds corresponding to other end position.
    /// @param parkPosition the PCM pulse width in microseconds corresponding to the default position.
    /// @return the device index.
    CCDevice* addServo(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition);
    
    /// Function adds a double-servo device to the device array and returns the index of the device.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param servo_00_pin the pin number of the first servo's pwm-pin.
    /// @param minPosition_00 the minimum PCM pulse width in microseconds corresponding to one end position.
    /// @param midPosition_00 the PCM pulse width in microseconds corresponding to the middle position.
    /// @param maxPosition_00 the maximum PCM pulse width in microseconds corresponding to other end position.
    /// @param servo_01_pin the pin number of the first servo's pwm-pin.
    /// @param minPosition_01 the minimum PCM pulse width in microseconds corresponding to one end position.
    /// @param midPosition_01 the minimum PCM pulse width in microseconds corresponding to one end position.
    /// @param maxPosition_01 the maximum PCM pulse width in microseconds corresponding to other end position.
    /// @param parkPosition the PCM pulse width in microseconds corresponding to the default position of the first servo. The position of the second servo is to be calculated.
    /// @return the device index.
    CCDevice* addServoWithCounterServo(String deviceName, unsigned char servo_00_pin, int minPosition_00, int midPosition_00, int maxPosition_00, unsigned char servo_01_pin, int minPosition_01, int midPosition_01, int maxPosition_01, int parkPosition);
    

    /// Function adds a stepper device to the device array, that is driven by da driver like the A4988, and returns the index of the device.
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
    CCDevice* addStepper_A4988(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned int stepsPerRotation, unsigned char microStep_00_pin, unsigned char microStep_01_pin, unsigned char microStep_02_pin, signed char steppingCode_00, signed char steppingCode_01, signed char steppingCode_02, signed char steppingCode_03 = -1, signed char steppingCode_04 = -1, signed char steppingCode_05 = -1, signed char steppingCode_06 = -1, signed char steppingCode_07 = -1);
    
    /// Function adds a stepper device to the device array, that is driven by da driver like the TMC260, and returns the index of the device.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param dir_pin the pin number of the stepper driver's direction pin.
    /// @param step_pin the pin number of the stepper driver's step pin.
    /// @param enable_pin the pin number of the stepper driver's enable pin.
    /// @param chipSelect_pin the number of the drivers chip select pin (CS) for the SPI communication.
    /// @param currentMax the maximum current, that is applied to the motor coils in mA RMS.
    /// @param stepsPerRotation the number of steps needed to make a full rotation.
    /// @return the device index.
    CCDevice* addStepper_TMC260(String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned int stepsPerRotation, unsigned char chipSelect_pin, unsigned int currentMax);
    
    
    /// Function adds a switching device to the device array and returns the index of the device.
    /// A switching device is a device, that is simply switched on or off. Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param switching_pin the pin number of the device's controll pin.
    /// @param switchingPin_activ the state of the switching pin, where the device is activ.
    /// @return the device index.
    CCDevice* addDcController(String deviceName, unsigned char switching_pin, boolean switchingPin_activ);

    /// Function adds a full-bridge switching device (two channel) to the device array and returns the index of the device.
    /// A full-bridge switching device has two switching pins and a direction. Switching_A_pin is used to go froward direction, switching_b_pin for going reverse.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param switching_A_pin the pin number of the device's forward controll pin.
    /// @param switching_B_pin the pin number of the device's reverse controll pin.
    /// @param switchingPin_A_activ the state of the switching pin, where the device is activ forward.
    /// @param switchingPin_B_activ the state of the switching pin, where the device is activ reverse.
    /// @return the device index.
    CCDevice* addDcController_fullBridge(String deviceName, unsigned char switching_A_pin, boolean switchingPin_A_activ, unsigned char switching_B_pin, boolean switchingPin_B_activ);
    

    /// Function lists all registered devices.
    /// A list with all devices and bare informations are presented.
    void listDevices();
    

    
    /// Array of all control-inputs.
    CCControlButton *controlButton[8];
    
    /// Function adds a control button to the control button array and returns the index of the button.
    /// A control button is a input device, that can provide either a HIGH or a LOW level at a input pin or simply connect the pin and GND using the internal inputPullup-function. Specific parameters are passed.
    /// @param buttonName the human-readable name of the device (used for verbose output).
    /// @param button_pin the pin number of the button's pin.
    /// @param buttonActiv the state of the pin, where the button should trigger actions.
    /// @param pullup if a nonzero value is passed, the input pullup is activated.
    /// @return the button index.
    CCControlButton* addControlButton(String buttonName, unsigned char button_pin, boolean buttonActiv, boolean pullup);
    
    /// Function lists all registered control buttons.
    /// A list with all buttons and bare informations are presented.
    void listControlButtons();
    

    void listAllTasks(CCWorkflow* workflow);
    void listAllActions(CCWorkflow* workflow);
    
    
    /// Function calls the <reviewValues(CCTask* nextTask)> function of all devices.
    /// This causes the devices to approve, if all tasks are possible and do some basic calculations.
    /// This function needs to be called before the run loop is started.
    void reviewTasks(CCWorkflow* currentWorkflow);
    
    
    /// Function starts the scheduler, all registered tasks are driven as specified.
    int run(CCWorkflow* currentWorkflow);
    
    
};




#endif // defined(__deviceScheduler__CCDeviceScheduler__)


