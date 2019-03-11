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
#include "voiceOGraph.h"

#include "CCWorkflow.h"

#include "CCDevice.h"
#include "CCServoDevice.h"
#include "CCServoDevice_cross.h"
#include "CCStepperDevice.h"
#include "CCStepperDevice_A4988.h"
#include "CCStepperDevice_TMC260.h"
//#include "CCStepperDevice_TMC2130.h"
#include "CCDcControllerDevice.h"
#include "CCDcControllerDevice_fullBridge.h"

#include "CCControl.h"
#include "CCControlButton.h"
#include "CCControlSensor.h"
#include "CCControlEvent.h"



///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class CCDeviceScheduler
///
/// @brief Scheduler engine to manage all devices and their tasks.
///
/// The scheduler holds all devices and controls them by calling their startTask(), operateTask(), stopTask() etc. routines.
///
///////////////////////////////////////////////////////////////////////////////////////////////////



// min time for deviceLoop: 15, max time for deviceLoop: 18462
// min time for controls: 1, max time for controls: 12
// min time for flowControls: 1, max time for flowControls: 11

// min time for deviceLoop: 14, max time for deviceLoop: 18459
// min time for controls: 1, max time for controls: 14
// min time for flowControls: 1, max time for flowControls: 12



class CCDeviceScheduler {
    int verbosity;

    
    /// The Scheduler's name
    /// Value, set by the user to be recognized by him.
    const String   schedulerName;
    
    /// Scheduler parameter:
    /// Value holds the number of devices managed by him.
    unsigned int countOfDevices, countOfControls;
    
    unsigned long taskTime;
    
    unsigned int nextTaskID[MAX_DEVICES_PER_SCHEDULER];

    CCDeviceFlow* currentDeviceFlow;
    CCDeviceFlow* triggerDeviceFlow_start;
    CCDeviceFlow* triggerDeviceFlow_stop;
    CCTask* currentTask;
    CCDevice* currentDevice;
    CCDevice* triggerDevice_start;
    CCDevice* triggerDevice_stop;
    
    CCControl* currentButton;
    CCAction* currentAction;
    CCFlowControl* currentFlowControl;
    
    int notificationCode;
    String  notificationText;
    
    
    String getLiteralOfDeviceType(deviceType t);
    String getLiteralOfControllerType(controlType t);
    String getLiteralOfTaskEvent(event e);
    String getLiteralOfState(deviceState s);
    String getLiteralOfControlValueComparing(comparingMode c);
    String getLiteralOfBooleanState(bool s);
    String getLiteralOfStoppingMode(stoppingMode s);
    String getLiteralOfSwitchingMode(switchingMode s);
    String getLiteralOfDeviceAction(deviceAction d);
    String getLiteralOfWorkflowInfo(workflowInfoCode i);
    String getLiteralOfDeviceInfo(deviceInfoCode i);
    

public:
    
    
    CCDeviceScheduler(const String schedulerName);
    ~CCDeviceScheduler();
    
    const String getName();
    

    /// The Scheduler's device array.
    /// Array of all devices.
    CCDevice *device[MAX_DEVICES_PER_SCHEDULER];
    
    /// Function adds a servo device to the device array and returns the index of the device.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param servo_pin the pin number of the servo's pwm-pin.
    /// @param minPosition the minimum PCM pulse width in microseconds corresponding to one end position.
    /// @param maxPosition the maximum PCM pulse width in microseconds corresponding to other end position.
    /// @param parkPosition the PCM pulse width in microseconds corresponding to the default position.
    /// @return the device index.
    CCDevice* addServo(String deviceName, unsigned int servo_pin, int minPosition, int maxPosition, int parkPosition);
    
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
    CCDevice* addServoWithCounterServo(String deviceName, unsigned int servo_00_pin, int minPosition_00, int midPosition_00, int maxPosition_00, unsigned int servo_01_pin, int minPosition_01, int midPosition_01, int maxPosition_01, int parkPosition);
    

    /// Function adds a stepper device to the device array, that is driven by da driver like the A4988, and returns the index of the device.
    /// Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param dir_pin the pin number of the stepper driver's direction pin.
    /// @param step_pin the pin number of the stepper driver's step pin.
    /// @param enable_pin the pin number of the stepper driver's enable pin.
    /// @param stepsPerRotation the number of steps needed to make a full rotation.
    /// @param microStep_00_pin, microStep_01_pin, microStep_02_pin the pin numbers of the micro stepping pins.
    /// @param steppingCode_00 ... steppingCode_07 the specific pin combinations for the different stepping modes.
    /// @return the device index.
    CCDevice* addStepper_A4988(String deviceName, unsigned int step_pin, unsigned int dir_pin, unsigned int enable_pin, unsigned int stepsPerRotation, unsigned int microStep_00_pin, unsigned int microStep_01_pin, unsigned int microStep_02_pin, signed char steppingCode_00, signed char steppingCode_01, signed char steppingCode_02, signed char steppingCode_03 = -1, signed char steppingCode_04 = -1, signed char steppingCode_05 = -1, signed char steppingCode_06 = -1, signed char steppingCode_07 = -1);
    
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
    CCDevice* addStepper_TMC260(String deviceName, unsigned int step_pin, unsigned int dir_pin, unsigned int enable_pin, unsigned int stepsPerRotation, unsigned int chipSelect_pin, unsigned int currentMax);
    
//    /// Function adds a stepper device to the device array, that is driven by da driver like the TMC2130, and returns the index of the device.
//    /// Device-specific parameters are passed.
//    /// @param deviceName the human-readable name of the device (used for verbose output).
//    /// @param dir_pin the pin number of the stepper driver's direction pin.
//    /// @param step_pin the pin number of the stepper driver's step pin.
//    /// @param enable_pin the pin number of the stepper driver's enable pin.
//    /// @param chipSelect_pin the number of the drivers chip select pin (CS) for the SPI communication.
//    /// @param currentMax the maximum current, that is applied to the motor coils in mA RMS.
//    /// @param stepsPerRotation the number of steps needed to make a full rotation.
//    /// @return the device index.
//    CCDevice* addStepper_TMC2130(String deviceName, unsigned int step_pin, unsigned int dir_pin, unsigned int enable_pin, unsigned int stepsPerRotation, unsigned int chipSelect_pin, unsigned int currentMax);

    
    /// Function adds a switching device to the device array and returns the index of the device.
    /// A switching device is a device, that is simply switched on or off. Device-specific parameters are passed.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param switching_pin the pin number of the device's controll pin.
    /// @param switchingPin_active the state of the switching pin, where the device is active.
    /// @return the device index.
    CCDevice* addDcController(String deviceName, unsigned int switching_pin, bool switchingPin_active);

    /// Function adds a full-bridge switching device (two channel) to the device array and returns the index of the device.
    /// A full-bridge switching device has two switching pins and a direction. Switching_A_pin is used to go froward direction, switching_b_pin for going reverse.
    /// @param deviceName the human-readable name of the device (used for verbose output).
    /// @param switching_A_pin the pin number of the device's forward controll pin.
    /// @param switching_B_pin the pin number of the device's reverse controll pin.
    /// @param switchingPin_A_active the state of the switching pin, where the device is active forward.
    /// @param switchingPin_B_active the state of the switching pin, where the device is active reverse.
    /// @return the device index.
    CCDevice* addDcController_fullBridge(String deviceName, unsigned int switching_A_pin, bool switchingPin_A_active, unsigned int switching_B_pin, bool switchingPin_B_active);
    
    
    /// Function lists all registered devices.
    /// A list with all devices and bare informations are presented.
    void listDevices();
    void printMessage(unsigned long taskTime, CCDeviceFlow* df, schedulersJob job);

    
    /// Array of all control-inputs.
    CCControl *control[MAX_CONTROLS_PER_SCHEDULER];
    
    /// Function adds a control button to the control button array and returns the index of the button.
    /// A control button is a input device, that can provide either a HIGH or a LOW level at a input pin or simply connect the pin and GND using the internal inputPullup-function. Specific parameters are passed.
    /// @param buttonName the human-readable name of the device (used for verbose output).
    /// @param button_pin the pin number of the button's pin.
    /// @param buttonActive the state of the pin, where the button should trigger actions.
    /// @param the pinMode macro is passed (INPUT or INPUT_PULLUP).
    /// @return the button index.
    CCControl* addControlButton(String controlName, unsigned int pin, unsigned int mode = INPUT);
 
    
    CCControl* addControlSensor(String sensorName, unsigned int pin);
    
    CCControl* addControlEvent(String eventName);
    
    /// Function lists all registered control buttons.
    /// A list with all buttons and bare informations are presented.
    void listControls();
    
    void evaluateButtons();


    void listAllTasks(CCWorkflow* workflow);
    void listAllActions(CCWorkflow* workflow);
    
    
    /// Function calls the <reviewValues(CCTask* nextTask)> function of all devices.
    /// This causes the devices to approve, if all tasks are possible and do some basic calculations.
    /// This function needs to be called before the run loop is started.
    void reviewTasks(CCWorkflow* currentWorkflow);
    
    
    /// Function starts the scheduler, all registered tasks are driven as specified.
    /// @image html scheduler__run.png
    int run(CCWorkflow* currentWorkflow);
    
    void setVerbosity(int verbosity);
    
    bool getValueOfTriStateSwitch(unsigned int triStateSwitchPin, bool probedState);


};




#endif // defined(__deviceScheduler__CCDeviceScheduler__)


