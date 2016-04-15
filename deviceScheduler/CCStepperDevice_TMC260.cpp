//
// CCStepperDevice_TMC260.cpp
// C++ code
// ----------------------------------
// Developed with embedXcode+
// http://embedXcode.weebly.com
//
// Project 		deviceScheduler
//
// Created by 	Little Abakus, 15.04.16 11:56
// 				Little Abakus
//
// Copyright 	(c) Little Abakus, 2016
// License		<#license#>
//
// See 			CCStepperDevice_TMC260.h and ReadMe.txt for references
//



#include <SPI.h>

#include "Arduino.h"
#include "CCStepperDevice_TMC260.h"



//TMC26X register definitions
#define DRIVER_CONTROL_REGISTER 0x0ul
#define CHOPPER_CONFIG_REGISTER 0x80000ul
#define COOL_STEP_REGISTER  0xA0000ul
#define STALL_GUARD2_LOAD_MEASURE_REGISTER 0xC0000ul
#define DRIVER_CONFIG_REGISTER 0xE0000ul

#define REGISTER_BIT_PATTERN 0xFFFFFul

//definitions for the driver control register
#define MICROSTEPPING_PATTERN 0xFul
#define STEP_INTERPOLATION 0x200ul
#define DOUBLE_EDGE_STEP 0x100ul
#define VSENSE 0x40ul
#define READ_MICROSTEP_POSTION 0x0ul
#define READ_STALL_GUARD_READING 0x10ul
#define READ_STALL_GUARD_AND_COOL_STEP 0x20ul
#define READ_SELECTION_PATTERN 0x30ul

//definitions for the chopper config register
#define CHOPPER_MODE_STANDARD 0x0ul
#define CHOPPER_MODE_T_OFF_FAST_DECAY 0x4000ul
#define T_OFF_PATTERN 0xful
#define RANDOM_TOFF_TIME 0x2000ul
#define BLANK_TIMING_PATTERN 0x18000ul
#define BLANK_TIMING_SHIFT 15
#define HYSTERESIS_DECREMENT_PATTERN 0x1800ul
#define HYSTERESIS_DECREMENT_SHIFT 11
#define HYSTERESIS_LOW_VALUE_PATTERN 0x780ul
#define HYSTERESIS_LOW_SHIFT 7
#define HYSTERESIS_START_VALUE_PATTERN 0x78ul
#define HYSTERESIS_START_VALUE_SHIFT 4
#define T_OFF_TIMING_PATERN 0xFul

//definitions for cool step register
#define MINIMUM_CURRENT_FOURTH 0x8000ul
#define CURRENT_DOWN_STEP_SPEED_PATTERN 0x6000ul
#define SE_MAX_PATTERN 0xF00ul
#define SE_CURRENT_STEP_WIDTH_PATTERN 0x60ul
#define SE_MIN_PATTERN 0xful

//definitions for stall guard2 current register
#define STALL_GUARD_FILTER_ENABLED 0x10000ul
#define STALL_GUARD_TRESHHOLD_VALUE_PATTERN 0x17F00ul
#define CURRENT_SCALING_PATTERN 0x1Ful
#define STALL_GUARD_CONFIG_PATTERN 0x17F00ul
#define STALL_GUARD_VALUE_PATTERN 0x7F00ul

//definitions for the input from the TCM260
#define STATUS_STALL_GUARD_STATUS 0x1ul
#define STATUS_OVER_TEMPERATURE_SHUTDOWN 0x2ul
#define STATUS_OVER_TEMPERATURE_WARNING 0x4ul
#define STATUS_SHORT_TO_GROUND_A 0x8ul
#define STATUS_SHORT_TO_GROUND_B 0x10ul
#define STATUS_OPEN_LOAD_A 0x20ul
#define STATUS_OPEN_LOAD_B 0x40ul
#define STATUS_STAND_STILL 0x80ul
#define READOUT_VALUE_PATTERN 0xFFC00ul









CCStepperDevice_TMC260::CCStepperDevice_TMC260(unsigned int deviceIndex, String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char chipSelect_pin, unsigned int current, unsigned int resistor, unsigned int stepsPerRotation) : CCStepperDevice() {
    
    
    this->deviceIndex = deviceIndex;
    this->deviceName = deviceName;
    this->dir_pin = dir_pin;
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
	this->chipSelect_pin = chipSelect_pin;
    
    this->resistor = resistor;
    this->current = current;
    this->stepsPerRotation = stepsPerRotation;
    
    this->highestSteppingMode = 8;
    
    stepsPerDegree = stepsPerRotation / 360.0;                                              // save time executing prepareNextTask()
    degreesPerMicroStep = 360.0 / stepsPerRotation / (1 << highestSteppingMode);            // save time when calculatin currentPosition in operateTask()
    
    acceleration_max = 4000;
    
    type = STEPPERDEVICE;
    state = SLEEPING;
    taskPointer = 0;
    countOfTasks = 0;
    
    defaultVelocity = 0;
    defaultAcceleration = 0;
    defaultDeceleration = 0;

    currentMicroStep = 0;
    currentPosition = 0;
    
    prepareAndStartNextTaskWhenFinished = false;

    
    
    attachDevice();

    
    
    
    // ################################### Stepper  ###################################
    unsigned int coilCurrentMax = 1000;                  // current in mA
    unsigned int motorVoltage = 12000;                   // voltage in mV
    unsigned int motorResistance = 1.7;                  // resistance in Ohm
    //  unsigned int motorInductance = 46;                   // inductance in Mh
    unsigned int motorStepsPerRotation = 200;
    unsigned int motorStepsPerSecondMax [] = {
        600, 700, 1250, 2500, 5000, 10000, 20000, 40000, 80000};

    // ################################### Shield  ###################################
    
#define R_SENSE  150                                 // resistor value in mOhm

    
    // ################################### TMC 260  ###################################
    unsigned long datagram = 0x00000;
    
    
   
    
   //  StallGuard2ControlRegister:
    boolean stallGuard2FilterEnable = true;
    int stallGuard2Threshold = 30;
    byte currentScaleOf32;                  // approved
    
    //  DriverConfigurationRegister
    byte slopeControlHighSide;
    byte slopeControlLowSide;
    boolean shortToGndProtectionDisable;
    float shortToGndDetectionTimer;
    boolean stepDirInterfaceDisable;
    boolean senseResistorVoltage165mV;          // approved
    byte selectReadOut;
#define READOUT_MICROSTEP_POSITION    0
#define READOUT_STALLGUARD_LEVEL      1
#define READOUT_CURRENT_LEVEL         2
    
    //  TMC260 readback:
    unsigned int microstepPosition;
    unsigned int stallGuard2Value;
    byte coolStepScalingValue;
    byte stallGuard2Value_upper;
    boolean standStil;
    byte openLoad;
    byte shortToGnd;
    boolean overTemperatureWarning;
    boolean overTemperatureShutdown;
    boolean stallGuard2Status;
    

    // #################### DriverControlRegister: ####################
    boolean stepInterpolation = false;
    boolean doubleEdgeStepPulses = false;

    datagram = 0x00000;
    
    bitWrite(datagram, 9, stepInterpolation);
    bitWrite(datagram, 8, doubleEdgeStepPulses);
    
    datagram |= 8;
    
    doTransaction(datagram);

    
    
    unsigned int VSenseFullScale;                        // voltage in mV
    //  VsenseMax = R_SENSE * coilCurrentMax [/ 1000]
    //  here: VsenseMax = 0.15 Ohm * 1000 mA = 150 mV  ==> we choose: VSENSE = 1: Full-scale sense resistor voltage is 165mV;
    if ((long) R_SENSE * coilCurrentMax <= 165000) {
        VSenseFullScale = 165;
        senseResistorVoltage165mV = true;
    }
    else {
        VSenseFullScale = 305;
        senseResistorVoltage165mV = false;
    }
    //  from datasheet:  I = (CS + 1) / 32 * Vfs / Rs
    //  here:  CS + 1 = currentScaleOf32; Vfs = VSenseFullScale / 1000; Rs = R_SENSE / 1000
    //  so: I / 1000 = currentScaleOf32 / 32 * VSenseFullScale / R_SENSE
    //  ==>  currentScaleOf32 = (32 * R_SENSE * I) / VSenseFullScale;
    currentScaleOf32 = (float)(32.0 * R_SENSE * current) / VSenseFullScale / 1000.0 + 0.5;               // (+ 0.5 is for better rounding)
    //   100 mA: 32 * 0.15 * 0.1 / 0.165 =  2.91 -->  3
    //   300 mA: 32 * 0.15 * 0.3 / 0.165 =  8.73 -->  9
    //   600 mA: 32 * 0.15 * 0.6 / 0.165 = 17.45 --> 17
    //  1000 mA: 32 * 0.15 * 1.0 / 0.165 = 29.09 --> 29
    
    if (verbose) {
        Serial.print("set currentScaleOf32 to ");
        Serial.println(currentScaleOf32);
        Serial.print("VSenseFullScale: ");
        Serial.print(VSenseFullScale);
        Serial.print("  senseResistorVoltage165mV: ");
        Serial.println(senseResistorVoltage165mV);
    }
    
    
    // #################### ChopperControlRegister: ####################
    datagram = 0x80000;

    
    // blanking time:
    //  internal clock: 15 MHz ==> 1 systemClockPeriod is 67 ns
    //  blanking clocktimes 16: 1.06 us [= 0]; 24: 1.6 us [= 1]; 36: 2.4 us [= 2]; 54: 3.6 us [= 3]
    byte blankingTimeValue = 3;
    datagram |= blankingTimeValue << 15;
    
    // chopper mode:
    //  spreadCycle [= 0], constantOfftime [= 1]
    boolean chopperMode = 0;
    bitWrite(datagram, 14, chopperMode);

    // random off time:
    boolean randomTOffTime = 0;
    bitWrite(datagram, 13, randomTOffTime);
    

    byte hysteresisDecrementPeriodValue;
    int hysteresisEnd;
    byte hysteresisStart;
 
    boolean onlyTimerTerminatesDecayPhase;
    int sinwaveOffset = 3;
    byte fastDecayTime = 3;
    
    if (chopperMode == CHOPPER_SPREADCYCLE_MODE) {
        // spread cycle mode - standard mode:

        // hysteresis decrement period:
        //  16 (fast decrement) [= 0]; 32 [= 1]; 48 [= 2]; 64 (very slow decrement) [= 3]
        hysteresisDecrementPeriodValue = 0;
        datagram |= (hysteresisDecrementPeriodValue & 0x3) << 11;
        
        // hysteresis end:
        // [-3, -2, -1, 0, 1, ..., 12] (1/512 of this setting adds to current setting)
        // This is the hysteresis value which becomes used for the hysteresis chopper.
        hysteresisEnd = 4;
        datagram |= ((hysteresisEnd + 3) & 0xF) << 7;
        
        // hysteresis start offset from hysteresis end
        // 1 [= 0]; 2 [= 1]; 3 [= 2] ... 8 [= 7]
        // Effective: HEND+HSTRT must be ≤ 15
        hysteresisStart = 1;
        datagram |= ((hysteresisStart - 1) & 0x7) << 4;
    }
    
    else {
        // constant offTime mode:
        onlyTimerTerminatesDecayPhase = false;
        bitWrite(datagram, 12, onlyTimerTerminatesDecayPhase);
        
        // sinwave offset: [-3, -2, -1, 0, 1, ..., 12]
        //  (sine wave offset and 1/512 of the value becomes added to the absolute value of each sine wave entry)
        sinwaveOffset = 3;
        datagram |= ((sinwaveOffset + 3) & 0xF) << 7;
        
        // fast decay time: [0 ... 15]
        // a multiple of system clock periods: NCLK= 32 x (HDEC0+HSTRT)
        fastDecayTime = constrain(fastDecayTime, 0, 15);
        datagram |= ((fastDecayTime & 0x8) >> 3) << 11;
        datagram |= (fastDecayTime & 0x7) << 4;
        
        
    }
    
    //  internal clock: 15 MHz ==> 1 systemClockPeriod is 67 ns, clocks = 12 + 32 * offTime
    //  offTime 0: MOSFET shut off; 2: (76 cycles): 5.1 us; 3: (108 cycles): 7.2 us; 15 (492 cycles): 32.8 us
    byte offTime = 3;
    datagram |= offTime & 0xF;
    
    
    doTransaction(datagram);

    
    
    // #################### CoolStepControlRegister: ####################
    datagram = 0xA0000;
    
    //  CoolStepControlRegister:
    boolean fourthCoolStepCurrent = true;
    bitWrite(datagram, 15, fourthCoolStepCurrent);
 
    // current decrerment speed: [32, 8, 2, 1]
    // 32 [= 0], 8 [= 1], 2 [= 2], 1 [= 3]
    byte currentDecrementSpeedValue = 0;
    datagram |= (currentDecrementSpeedValue & 0x3) << 13;
    
    // upper cool step treshold:
    // If the stallGuard2 measurement value SG is sampled equal to or above (SEMIN+SEMAX+1) x 32 enough times, then the coil current scaling factor is decremented.
    byte upperCoolStepThreshold = 2;
    datagram |= (upperCoolStepThreshold & 0xF) << 8;

    byte currentIncrementSteps = 8;
    byte lowerCoolStepThreshold = 0;
    boolean coolStepEnable;
    
    
    
    
    
    
    upperCoolStepThreshold = constrain(upperCoolStepThreshold, 0, 15);
    
    
    currentIncrementSteps = constrain(currentIncrementSteps, 1, 8);
    unsigned int currentIncrementStepsValue[] = {
        1, 2, 4, 8  };
    for (unsigned long index = 0; index <= 3; index++) {
        if (currentIncrementStepsValue[index] >= currentIncrementSteps) {
            datagram |= (index & 0x3) << 5;
            break;
        }
    }
    
    
    //  if (coolStepEnable) {
    lowerCoolStepThreshold = constrain(lowerCoolStepThreshold, 0, 15);
    datagram |= lowerCoolStepThreshold & 0xF;
    //  }
    //  else {
    //    datagram &= 0xFFFF0;
    //  }
    
    
    doTransaction(datagram);

    
    
    setCurrent(2500);
    setStallGuard2ControlRegister();
    
    setDriverConfigurationRegister();

    
    
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice_TMC260]: setup "));
        Serial.print(deviceName);
        Serial.print(F(": currentPosition: "));
        Serial.print(currentPosition);
        Serial.print(F(", dir_pin: "));
        Serial.print(dir_pin);
        Serial.print(F(", step_pin: "));
        Serial.print(step_pin);
        Serial.print(F(", enable_pin: "));
        Serial.print(enable_pin);
        Serial.print(F(", chipSelect_pin: "));
        Serial.print(chipSelect_pin);
        Serial.print(F(", current: "));
        Serial.print(current);
        Serial.print(F(", resitor: "));
        Serial.print(resistor);
        Serial.print(F("steppingModes: "));
        Serial.print(highestSteppingMode + 1);
        Serial.print(F("stepsPerDegree: "));
        Serial.print(stepsPerDegree);
        Serial.print(F(", at $"));
        Serial.println((long) this, HEX);
    }
    

    
}




void CCStepperDevice_TMC260::attachDevice() {
    pinMode(dir_pin, OUTPUT);
    digitalWrite(dir_pin, LOW);

    pinMode(step_pin, OUTPUT);
    digitalWrite(step_pin, LOW);
    
    pinMode(chipSelect_pin, OUTPUT);
    digitalWrite(chipSelect_pin, HIGH);

    pinMode(enable_pin, OUTPUT);
    digitalWrite(enable_pin, HIGH);
    
   
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV8);
    SPI.setDataMode(SPI_MODE3);
    
    SPI.begin();
    
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice_TMC260]: "));
        Serial.print(deviceName);
        Serial.println(F(" attached"));
    }
}
void CCStepperDevice_TMC260::detachDevice() {
    digitalWrite(enable_pin, HIGH);
    
    pinMode(dir_pin, INPUT);
    pinMode(step_pin, INPUT);
    pinMode(enable_pin, INPUT_PULLUP);
    pinMode(chipSelect_pin, INPUT_PULLUP);
    
    if (CCSTEPPERDEVICE_VERBOSE & CCSTEPPERDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCStepperDevice_TMC260]: device "));
        Serial.print(deviceName);
        Serial.println(F(" detached"));
    }
}







void CCStepperDevice_TMC260::setupMicroSteppingMode(unsigned char data) {
    microSteppingMode = data;
    CCStepperDevice::setupMicroSteppingMode();
}
void CCStepperDevice_TMC260::setupMicroSteppingMode() {
#ifdef DEBUG
    Serial.print("set microSteppingValue: ");
    Serial.println(8 - microSteppingMode);
#endif
    //delete the old value
    this->driver_control_register_value &= 0xFFFF0ul;
    //set the new value
    this->driver_control_register_value |= (8 - microSteppingMode);
    
    send262(driver_control_register_value);
}









void setCurrent(unsigned int current) {
}


void getReadOut(byte theRegisterToRead) {
    selectReadOut = theRegisterToRead;
    setDriverConfigurationRegister();
}

void doTransaction(unsigned long datagram) {
    if (verboseSPI) {
        Serial.print("Sending: ");
        printDatagram(datagram);
    }
    
    //select the first TMC260
    unsigned long resultDatagram;
    digitalWrite(TMC260_01_CHIPSELECT_PIN, LOW);
    
    //write/read the values
    resultDatagram = SPI.transfer((datagram >> 16) & 0xff);
    resultDatagram <<= 8;
    resultDatagram |= SPI.transfer((datagram >>  8) & 0xff);
    resultDatagram <<= 8;
    resultDatagram |= SPI.transfer((datagram) & 0xff);
    resultDatagram >>= 4;
    
    //deselect the TMC chip
    digitalWrite(TMC260_01_CHIPSELECT_PIN, HIGH);
    
    
    if (verboseSPI) {
        Serial.print("Receive from first chip: ");
        printDatagram(resultDatagram);
    }
    
    if (selectReadOut == READOUT_MICROSTEP_POSITION) {
        microstepPosition = (resultDatagram >> 10) & 0x1FF;
        if (verbose) {
            Serial.print("  microstepPosition: ");
            Serial.print(microstepPosition);
        }
    }
    else if (selectReadOut == READOUT_STALLGUARD_LEVEL) {
        //    stallGuard2Value = (resultDatagram >> 10) & 0x3FF;
        stallGuard2Value = (resultDatagram >> 10);
        if (verbose) {
            Serial.print("  stallGuard2Value: ");
            Serial.print(stallGuard2Value);
        }
    }
    else {
        coolStepScalingValue = (resultDatagram >> 10) & 0x1F;
        stallGuard2Value_upper = (resultDatagram >> 15) & 0x1F;
        
        if (verbose) {
            Serial.print("  coolStepScalingValue: ");
            Serial.print(coolStepScalingValue);
            Serial.print("  stallGuard2Value_upper: ");
            Serial.print(stallGuard2Value_upper);
        }
    }
    
    standStil = bitRead(resultDatagram, 7);
    
    openLoad = (resultDatagram & 0x60) >> 5;
    
    shortToGnd = (resultDatagram & 0x18) >> 3;
    
    overTemperatureWarning = resultDatagram & 0x4;
    
    overTemperatureShutdown = resultDatagram & 0x2;
    
    stallGuard2Status = resultDatagram & 0x1;
    
    if (verbose) {
        //    Serial.print("  standStil: ");
        //    Serial.print(standStil);
        //    Serial.print("  openLoad: ");
        //    Serial.print(openLoad);
        //    Serial.print("  shortToGnd: ");
        //    Serial.print(shortToGnd);
        //    Serial.print("  overTemperatureWarning: ");
        //    Serial.print(overTemperatureWarning);
        //    Serial.print("  overTemperatureShutdown: ");
        //    Serial.print(overTemperatureShutdown);
        Serial.print("  stallGuard2Status: ");
        Serial.println(stallGuard2Status);
    }
    
    
    
    
}




unsigned long setCoolStepControlRegister() {
    unsigned long 
    
    
    return datagram;
}


unsigned long setStallGuard2ControlRegister() {
    unsigned long datagram = 0xC0000;
    
    bitWrite(datagram, 16, stallGuard2FilterEnable);
    
    
    stallGuard2Threshold = constrain(stallGuard2Threshold, -64, 63);
    datagram |= (stallGuard2Threshold & 0x7F) << 8;
    
    
    currentScaleOf32 = constrain(currentScaleOf32, 1, 32);
    datagram |= (currentScaleOf32 - 1) & 0x1F;
    
    
    doTransaction(datagram);
    
    return datagram;
}


unsigned long setDriverConfigurationRegister() {
    unsigned long datagram = 0xE0000;
    
    slopeControlHighSide = constrain(slopeControlHighSide, 0, 3);
    datagram |= (slopeControlHighSide & 0x3) << 14;
    
    
    slopeControlLowSide = constrain(slopeControlLowSide, 0, 3);
    datagram |= (slopeControlLowSide & 0x3) << 12;
    
    
    bitWrite(datagram, 10, shortToGndProtectionDisable);
    
    
    shortToGndDetectionTimer = constrain(shortToGndDetectionTimer, 0.8, 3.2);
    float shortToGndDetectionTimerValueList[] = {
        3.2, 1.6, 1.2, 0.8                                                                                                  };
    for (unsigned long index = 0; index <= 3; index++) {
        if (shortToGndDetectionTimerValueList[index] <= shortToGndDetectionTimer) {
            datagram |= (index & 0x3) << 8;
            break;
        }
    }
    
    
    bitWrite(datagram, 7, stepDirInterfaceDisable);
    
    
    bitWrite(datagram, 6, senseResistorVoltage165mV);
    
    selectReadOut = constrain(selectReadOut, 0, 2);
    datagram |= (selectReadOut & 0x3) << 4;
    
    
    doTransaction(datagram);
    
    return datagram;
}

void setMotorDirection() {
    digitalWrite(DIR_PIN, motorDirection);
}

void printDatagram(unsigned long datagram) {
    char out[20];
    
    for (int i = 0; i <= 19; i++) {
        out[i] = ((datagram >> i) & 1) + '0';    
    }
    
    Serial.print(out[19]);
    Serial.print(out[18]);
    Serial.print(out[17]);
    Serial.print(out[16]);
    Serial.print(" ");
    Serial.print(out[15]);
    Serial.print(out[14]);
    Serial.print(out[13]);
    Serial.print(out[12]);
    Serial.print(" ");
    Serial.print(out[11]);
    Serial.print(out[10]);
    Serial.print(out[9]);
    Serial.print(out[8]);
    Serial.print(" ");
    Serial.print(out[7]);
    Serial.print(out[6]);
    Serial.print(out[5]);
    Serial.print(out[4]);
    Serial.print(" ");
    Serial.print(out[3]);
    Serial.print(out[2]);
    Serial.print(out[1]);
    Serial.print(out[0]);
    
    //  Serial.println();
}











void CCStepperDevice_TMC260::setCurrent(unsigned int current) {
    unsigned char current_scaling = 0;
	//calculate the current scaling from the max current setting (in mA)
	double mASetting = (double)current;
    double resistor_value = (double) this->resistor;
	// remove vesense flag
	this->driver_configuration_register_value &= ~(VSENSE);
	//this is derrived from I=(cs+1)/32*(Vsense/Rsense)
    //leading to cs = CS = 32*R*I/V (with V = 0,31V oder 0,165V  and I = 1000*current)
	//with Rsense=0,15
	//for vsense = 0,310V (VSENSE not set)
	//or vsense = 0,165V (VSENSE set)
	current_scaling = (byte)((resistor_value*mASetting*32.0/(0.31*1000.0*1000.0))-0.5); //theoretically - 1.0 for better rounding it is 0.5
	
	//check if the current scalingis too low
	if (current_scaling<16) {
        //set the csense bit to get a use half the sense voltage (to support lower motor currents)
		this->driver_configuration_register_value |= VSENSE;
        //and recalculate the current setting
        current_scaling = (byte)((resistor_value*mASetting*32.0/(0.165*1000.0*1000.0))-0.5); //theoretically - 1.0 for better rounding it is 0.5
#ifdef DEBUG
		Serial.print("CS (Vsense=1): ");
		Serial.println(current_scaling);
	} else {
        Serial.print("CS: ");
        Serial.println(current_scaling);
#endif
    }
    
	//do some sanity checks
	if (current_scaling>31) {
		current_scaling=31;
	}
	//delete the old value
	stall_guard2_current_register_value &= ~(CURRENT_SCALING_PATTERN);
	//set the new current scaling
	stall_guard2_current_register_value |= current_scaling;
	//if started we directly send it to the motor
	if (started) {
        send262(driver_configuration_register_value);
		send262(stall_guard2_current_register_value);
	}
}

unsigned int CCStepperDevice_TMC260::getCurrent(void) {
    //we calculate the current according to the datasheet to be on the safe side
    //this is not the fastest but the most accurate and illustrative way
    double result = (double)(stall_guard2_current_register_value & CURRENT_SCALING_PATTERN);
    double resistor_value = (double)this->resistor;
    double voltage = (driver_configuration_register_value & VSENSE)? 0.165:0.31;
    result = (result+1.0)/32.0*voltage/resistor_value*1000.0*1000.0;
    return (unsigned int)result;
}

void CCStepperDevice_TMC260::setStallGuardThreshold(char stall_guard_threshold, char stall_guard_filter_enabled) {
	if (stall_guard_threshold<-64) {
		stall_guard_threshold = -64;
        //We just have 5 bits
	} else if (stall_guard_threshold > 63) {
		stall_guard_threshold = 63;
	}
	//add trim down to 7 bits
	stall_guard_threshold &=0x7f;
	//delete old stall guard settings
	stall_guard2_current_register_value &= ~(STALL_GUARD_CONFIG_PATTERN);
	if (stall_guard_filter_enabled) {
		stall_guard2_current_register_value |= STALL_GUARD_FILTER_ENABLED;
	}
	//Set the new stall guard threshold
	stall_guard2_current_register_value |= (((unsigned long)stall_guard_threshold << 8) & STALL_GUARD_CONFIG_PATTERN);
	//if started we directly send it to the motor
	if (started) {
		send262(stall_guard2_current_register_value);
	}
}

char CCStepperDevice_TMC260::getStallGuardThreshold(void) {
    unsigned long stall_guard_threshold = stall_guard2_current_register_value & STALL_GUARD_VALUE_PATTERN;
    //shift it down to bit 0
    stall_guard_threshold >>=8;
    //convert the value to an int to correctly handle the negative numbers
    char result = stall_guard_threshold;
    //check if it is negative and fill it up with leading 1 for proper negative number representation
//    if (result & _BV(6)) {
//        result |= 0xC0;
//    }
    return result;
}

char CCStepperDevice_TMC260::getStallGuardFilter(void) {
    if (stall_guard2_current_register_value & STALL_GUARD_FILTER_ENABLED) {
        return -1;
    } else {
        return 0;
    }
}

/*
 * constant_off_time: The off time setting controls the minimum chopper frequency.
 * For most applications an off time within	the range of 5μs to 20μs will fit.
 *		2...15: off time setting
 *
 * blank_time: Selects the comparator blank time. This time needs to safely cover the switching event and the
 * duration of the ringing on the sense resistor. For
 *		0: min. setting 3: max. setting
 *
 * fast_decay_time_setting: Fast decay time setting. With CHM=1, these bits control the portion of fast decay for each chopper cycle.
 *		0: slow decay only
 *		1...15: duration of fast decay phase
 *
 * sine_wave_offset: Sine wave offset. With CHM=1, these bits control the sine wave offset.
 * A positive offset corrects for zero crossing error.
 *		-3..-1: negative offset 0: no offset 1...12: positive offset
 *
 * use_current_comparator: Selects usage of the current comparator for termination of the fast decay cycle.
 * If current comparator is enabled, it terminates the fast decay cycle in case the current
 * reaches a higher negative value than the actual positive value.
 *		1: enable comparator termination of fast decay cycle
 *		0: end by time only
 */
void CCStepperDevice_TMC260::setConstantOffTimeChopper(char constant_off_time, char blank_time, char fast_decay_time_setting, char sine_wave_offset, unsigned char use_current_comparator) {
	//perform some sanity checks
	if (constant_off_time<2) {
		constant_off_time=2;
	} else if (constant_off_time>15) {
		constant_off_time=15;
	}
    //save the constant off time
    this->constant_off_time = constant_off_time;
	char blank_value;
	//calculate the value acc to the clock cycles
	if (blank_time>=54) {
		blank_value=3;
	} else if (blank_time>=36) {
		blank_value=2;
	} else if (blank_time>=24) {
		blank_value=1;
	} else {
		blank_value=0;
	}
	if (fast_decay_time_setting<0) {
		fast_decay_time_setting=0;
	} else if (fast_decay_time_setting>15) {
		fast_decay_time_setting=15;
	}
	if (sine_wave_offset < -3) {
		sine_wave_offset = -3;
	} else if (sine_wave_offset>12) {
		sine_wave_offset = 12;
	}
	//shift the sine_wave_offset
	sine_wave_offset +=3;
	
	//calculate the register setting
	//first of all delete all the values for this
	chopper_config_register &= ~((1<<12) | BLANK_TIMING_PATTERN | HYSTERESIS_DECREMENT_PATTERN | HYSTERESIS_LOW_VALUE_PATTERN | HYSTERESIS_START_VALUE_PATTERN | T_OFF_TIMING_PATERN);
	//set the constant off pattern
	chopper_config_register |= CHOPPER_MODE_T_OFF_FAST_DECAY;
	//set the blank timing value
	chopper_config_register |= ((unsigned long)blank_value) << BLANK_TIMING_SHIFT;
	//setting the constant off time
	chopper_config_register |= constant_off_time;
	//set the fast decay time
	//set msb
	chopper_config_register |= (((unsigned long)(fast_decay_time_setting & 0x8))<<HYSTERESIS_DECREMENT_SHIFT);
	//other bits
	chopper_config_register |= (((unsigned long)(fast_decay_time_setting & 0x7))<<HYSTERESIS_START_VALUE_SHIFT);
	//set the sine wave offset
	chopper_config_register |= (unsigned long)sine_wave_offset << HYSTERESIS_LOW_SHIFT;
	//using the current comparator?
	if (!use_current_comparator) {
		chopper_config_register |= (1<<12);
	}
	//if started we directly send it to the motor
	if (started) {
		send262(driver_control_register_value);
	}
}

/*
 * constant_off_time: The off time setting controls the minimum chopper frequency.
 * For most applications an off time within	the range of 5μs to 20μs will fit.
 *		2...15: off time setting
 *
 * blank_time: Selects the comparator blank time. This time needs to safely cover the switching event and the
 * duration of the ringing on the sense resistor. For
 *		0: min. setting 3: max. setting
 *
 * hysteresis_start: Hysteresis start setting. Please remark, that this value is an offset to the hysteresis end value HEND.
 *		1...8
 *
 * hysteresis_end: Hysteresis end setting. Sets the hysteresis end value after a number of decrements. Decrement interval time is controlled by HDEC.
 * The sum HSTRT+HEND must be <16. At a current setting CS of max. 30 (amplitude reduced to 240), the sum is not limited.
 *		-3..-1: negative HEND 0: zero HEND 1...12: positive HEND
 *
 * hysteresis_decrement: Hysteresis decrement setting. This setting determines the slope of the hysteresis during on time and during fast decay time.
 *		0: fast decrement 3: very slow decrement
 */

void CCStepperDevice_TMC260::setSpreadCycleChopper(char constant_off_time, char blank_time, char hysteresis_start, char hysteresis_end, char hysteresis_decrement) {
	//perform some sanity checks
	if (constant_off_time<2) {
		constant_off_time=2;
	} else if (constant_off_time>15) {
		constant_off_time=15;
	}
    //save the constant off time
    this->constant_off_time = constant_off_time;
	char blank_value;
	//calculate the value acc to the clock cycles
	if (blank_time>=54) {
		blank_value=3;
	} else if (blank_time>=36) {
		blank_value=2;
	} else if (blank_time>=24) {
		blank_value=1;
	} else {
		blank_value=0;
	}
	if (hysteresis_start<1) {
		hysteresis_start=1;
	} else if (hysteresis_start>8) {
		hysteresis_start=8;
	}
	hysteresis_start--;
    
	if (hysteresis_end < -3) {
		hysteresis_end = -3;
	} else if (hysteresis_end>12) {
		hysteresis_end = 12;
	}
	//shift the hysteresis_end
	hysteresis_end +=3;
    
	if (hysteresis_decrement<0) {
		hysteresis_decrement=0;
	} else if (hysteresis_decrement>3) {
		hysteresis_decrement=3;
	}
	
	//first of all delete all the values for this
	chopper_config_register &= ~(CHOPPER_MODE_T_OFF_FAST_DECAY | BLANK_TIMING_PATTERN | HYSTERESIS_DECREMENT_PATTERN | HYSTERESIS_LOW_VALUE_PATTERN | HYSTERESIS_START_VALUE_PATTERN | T_OFF_TIMING_PATERN);
    
	//set the blank timing value
	chopper_config_register |= ((unsigned long)blank_value) << BLANK_TIMING_SHIFT;
	//setting the constant off time
	chopper_config_register |= constant_off_time;
	//set the hysteresis_start
	chopper_config_register |= ((unsigned long)hysteresis_start) << HYSTERESIS_START_VALUE_SHIFT;
	//set the hysteresis end
	chopper_config_register |= ((unsigned long)hysteresis_end) << HYSTERESIS_LOW_SHIFT;
	//set the hystereis decrement
	chopper_config_register |= ((unsigned long)blank_value) << BLANK_TIMING_SHIFT;
	//if started we directly send it to the motor
	if (started) {
		send262(driver_control_register_value);
	}
}

/*
 * In a constant off time chopper scheme both coil choppers run freely, i.e. are not synchronized.
 * The frequency of each chopper mainly depends on the coil current and the position dependant motor coil inductivity, thus it depends on the microstep position.
 * With some motors a slightly audible beat can occur between the chopper frequencies, especially when they are near to each other. This typically occurs at a
 * few microstep positions within each quarter wave. This effect normally is not audible when compared to mechanical noise generated by ball bearings, etc.
 * Further factors which can cause a similar effect are a poor layout of sense resistor GND connection.
 * Hint: A common factor, which can cause motor noise, is a bad PCB layout causing coupling of both sense resistor voltages
 * (please refer to sense resistor layout hint in chapter 8.1).
 * In order to minimize the effect of a beat between both chopper frequencies, an internal random generator is provided.
 * It modulates the slow decay time setting when switched on by the RNDTF bit. The RNDTF feature further spreads the chopper spectrum,
 * reducing electromagnetic emission on single frequencies.
 */
void CCStepperDevice_TMC260::setRandomOffTime(char value) {
	if (value) {
		chopper_config_register |= RANDOM_TOFF_TIME;
	} else {
		chopper_config_register &= ~(RANDOM_TOFF_TIME);
	}
	//if started we directly send it to the motor
	if (started) {
		send262(driver_control_register_value);
	}
}

void CCStepperDevice_TMC260::setCoolStepConfiguration(unsigned int lower_SG_threshold, unsigned int SG_hysteresis, unsigned char current_decrement_step_size,
                                             unsigned char current_increment_step_size, unsigned char lower_current_limit) {
    //sanitize the input values
    if (lower_SG_threshold>480) {
        lower_SG_threshold = 480;
    }
    //divide by 32
    lower_SG_threshold >>=5;
    if (SG_hysteresis>480) {
        SG_hysteresis=480;
    }
    //divide by 32
    SG_hysteresis >>=5;
    if (current_decrement_step_size>3) {
        current_decrement_step_size=3;
    }
    if (current_increment_step_size>3) {
        current_increment_step_size=3;
    }
    if (lower_current_limit>1) {
        lower_current_limit=1;
    }
    //store the lower level in order to enable/disable the cool step
    this->cool_step_lower_threshold=lower_SG_threshold;
    //if cool step is not enabled we delete the lower value to keep it disabled
    if (!this->cool_step_enabled) {
        lower_SG_threshold=0;
    }
    //the good news is that we can start with a complete new cool step register value
    //and simply set the values in the register
    cool_step_register_value = ((unsigned long)lower_SG_threshold) | (((unsigned long)SG_hysteresis)<<8) | (((unsigned long)current_decrement_step_size)<<5)
    | (((unsigned long)current_increment_step_size)<<13) | (((unsigned long)lower_current_limit)<<15)
    //and of course we have to include the signature of the register
    | COOL_STEP_REGISTER;
    //Serial.println(cool_step_register_value,HEX);
    if (started) {
        send262(cool_step_register_value);
    }
}

void CCStepperDevice_TMC260::setCoolStepEnabled(boolean enabled) {
    //simply delete the lower limit to disable the cool step
    cool_step_register_value &= ~SE_MIN_PATTERN;
    //and set it to the proper value if cool step is to be enabled
    if (enabled) {
        cool_step_register_value |=this->cool_step_lower_threshold;
    }
    //and save the enabled status
    this->cool_step_enabled = enabled;
    //save the register value
    if (started) {
        send262(cool_step_register_value);
    }
}

boolean CCStepperDevice_TMC260::isCoolStepEnabled(void) {
    return this->cool_step_enabled;
}

unsigned int CCStepperDevice_TMC260::getCoolStepLowerSgThreshold() {
    //we return our internally stored value - in order to provide the correct setting even if cool step is not enabled
    return this->cool_step_lower_threshold<<5;
}

unsigned int CCStepperDevice_TMC260::getCoolStepUpperSgThreshold() {
    return (unsigned char)((cool_step_register_value & SE_MAX_PATTERN)>>8)<<5;
}

unsigned char CCStepperDevice_TMC260::getCoolStepCurrentIncrementSize() {
    return (unsigned char)((cool_step_register_value & CURRENT_DOWN_STEP_SPEED_PATTERN)>>13);
}

unsigned char CCStepperDevice_TMC260::getCoolStepNumberOfSGReadings() {
    return (unsigned char)((cool_step_register_value & SE_CURRENT_STEP_WIDTH_PATTERN)>>5);
}

unsigned char CCStepperDevice_TMC260::getCoolStepLowerCurrentLimit() {
    return (unsigned char)((cool_step_register_value & MINIMUM_CURRENT_FOURTH)>>15);
}

void CCStepperDevice_TMC260::setEnabled(boolean enabled) {
    //delete the t_off in the chopper config to get sure
    chopper_config_register &= ~(T_OFF_PATTERN);
    if (enabled) {
        //and set the t_off time
        chopper_config_register |= this->constant_off_time;
    }
    //if not enabled we don't have to do anything since we already delete t_off from the register
	if (started) {
		send262(chopper_config_register);
	}
}

boolean CCStepperDevice_TMC260::isEnabled() {
    if (chopper_config_register & T_OFF_PATTERN) {
        return true;
    } else {
        return false;
    }
}

/*
 * reads a value from the TMC26X status register. The value is not obtained directly but can then
 * be read by the various status routines.
 *
 */
void CCStepperDevice_TMC260::readStatus(char read_value) {
    unsigned long old_driver_configuration_register_value = driver_configuration_register_value;
	//reset the readout configuration
	driver_configuration_register_value &= ~(READ_SELECTION_PATTERN);
	//this now equals TMC26X_READOUT_POSITION - so we just have to check the other two options
	if (read_value == TMC26X_READOUT_STALLGUARD) {
		driver_configuration_register_value |= READ_STALL_GUARD_READING;
	} else if (read_value == TMC26X_READOUT_CURRENT) {
		driver_configuration_register_value |= READ_STALL_GUARD_AND_COOL_STEP;
	}
	//all other cases are ignored to prevent funny values
    //check if the readout is configured for the value we are interested in
    if (driver_configuration_register_value!=old_driver_configuration_register_value) {
        //because then we need to write the value twice - one time for configuring, second time to get the value, see below
        send262(driver_configuration_register_value);
    }
    //write the configuration to get the last status
	send262(driver_configuration_register_value);
}

int CCStepperDevice_TMC260::getMotorPosition(void) {
	//we read it out even if we are not started yet - perhaps it is useful information for somebody
	readStatus(TMC26X_READOUT_POSITION);
    return getReadoutValue();
}

//reads the stall guard setting from last status
//returns -1 if stallguard information is not present
int CCStepperDevice_TMC260::getCurrentStallGuardReading(void) {
	//if we don't yet started there cannot be a stall guard value
	if (!started) {
		return -1;
	}
	//not time optimal, but solution optiomal:
	//first read out the stall guard value
	readStatus(TMC26X_READOUT_STALLGUARD);
	return getReadoutValue();
}

unsigned char CCStepperDevice_TMC260::getCurrentCSReading(void) {
	//if we don't yet started there cannot be a stall guard value
	if (!started) {
		return 0;
	}
	//not time optimal, but solution optiomal:
	//first read out the stall guard value
	readStatus(TMC26X_READOUT_CURRENT);
	return (getReadoutValue() & 0x1f);
}

unsigned int CCStepperDevice_TMC260::getCurrentCurrent(void) {
    double result = (double)getCurrentCSReading();
    double resistor_value = (double)this->resistor;
    double voltage = (driver_configuration_register_value & VSENSE)? 0.165:0.31;
    result = (result+1.0)/32.0*voltage/resistor_value*1000.0*1000.0;
    return (unsigned int)result;
}

/*
 return true if the stallguard threshold has been reached
 */
boolean CCStepperDevice_TMC260::isStallGuardOverThreshold(void) {
	if (!this->started) {
		return false;
	}
	return (driver_status_result & STATUS_STALL_GUARD_STATUS);
}

/*
 returns if there is any over temperature condition:
 OVER_TEMPERATURE_PREWARING if pre warning level has been reached
 OVER_TEMPERATURE_SHUTDOWN if the temperature is so hot that the driver is shut down
 Any of those levels are not too good.
 */
char CCStepperDevice_TMC260::getOverTemperature(void) {
	if (!this->started) {
		return 0;
	}
	if (driver_status_result & STATUS_OVER_TEMPERATURE_SHUTDOWN) {
		return TMC26X_OVERTEMPERATURE_SHUTDOWN;
	}
	if (driver_status_result & STATUS_OVER_TEMPERATURE_WARNING) {
		return TMC26X_OVERTEMPERATURE_PREWARING;
	}
	return 0;
}

//is motor channel A shorted to ground
boolean CCStepperDevice_TMC260::isShortToGroundA(void) {
	if (!this->started) {
		return false;
	}
	return (driver_status_result & STATUS_SHORT_TO_GROUND_A);
}

//is motor channel B shorted to ground
boolean CCStepperDevice_TMC260::isShortToGroundB(void) {
	if (!this->started) {
		return false;
	}
	return (driver_status_result & STATUS_SHORT_TO_GROUND_B);
}

//is motor channel A connected
boolean CCStepperDevice_TMC260::isOpenLoadA(void) {
	if (!this->started) {
		return false;
	}
	return (driver_status_result & STATUS_OPEN_LOAD_A);
}

//is motor channel B connected
boolean CCStepperDevice_TMC260::isOpenLoadB(void) {
	if (!this->started) {
		return false;
	}
	return (driver_status_result & STATUS_OPEN_LOAD_B);
}

//is chopper inactive since 2^20 clock cycles - defaults to ~0,08s
boolean CCStepperDevice_TMC260::isStandStill(void) {
	if (!this->started) {
		return false;
	}
	return (driver_status_result & STATUS_STAND_STILL);
}

//is chopper inactive since 2^20 clock cycles - defaults to ~0,08s
boolean CCStepperDevice_TMC260::isStallGuardReached(void) {
	if (!this->started) {
		return false;
	}
	return (driver_status_result & STATUS_STALL_GUARD_STATUS);
}

//reads the stall guard setting from last status
//returns -1 if stallguard inforamtion is not present
int CCStepperDevice_TMC260::getReadoutValue(void) {
	return (int)(driver_status_result >> 10);
}

int CCStepperDevice_TMC260::getResistor() {
    return this->resistor;
}

boolean CCStepperDevice_TMC260::isCurrentScalingHalfed() {
    if (this->driver_configuration_register_value & VSENSE) {
        return true;
    } else {
        return false;
    }
}

void CCStepperDevice_TMC260::debugLastStatus() {
#ifdef DEBUG
    if (this->started) {
		if (this->getOverTemperature()&TMC26X_OVERTEMPERATURE_PREWARING) {
			Serial.println("WARNING: Overtemperature Prewarning!");
		} else if (this->getOverTemperature()&TMC26X_OVERTEMPERATURE_SHUTDOWN) {
			Serial.println("ERROR: Overtemperature Shutdown!");
		}
		if (this->isShortToGroundA()) {
			Serial.println("ERROR: SHORT to ground on channel A!");
		}
		if (this->isShortToGroundB()) {
			Serial.println("ERROR: SHORT to ground on channel A!");
		}
		if (this->isOpenLoadA()) {
			Serial.println("ERROR: Channel A seems to be unconnected!");
		}
		if (this->isOpenLoadB()) {
			Serial.println("ERROR: Channel B seems to be unconnected!");
		}
		if (this->isStallGuardReached()) {
			Serial.println("INFO: Stall Guard level reached!");
		}
		if (this->isStandStill()) {
			Serial.println("INFO: Motor is standing still.");
		}
		unsigned long readout_config = driver_configuration_register_value & READ_SELECTION_PATTERN;
		int value = getReadoutValue();
		if (readout_config == READ_MICROSTEP_POSTION) {
			Serial.print("Microstep postion phase A: ");
			Serial.println(value);
		} else if (readout_config == READ_STALL_GUARD_READING) {
			Serial.print("Stall Guard value:");
			Serial.println(value);
		} else if (readout_config == READ_STALL_GUARD_AND_COOL_STEP) {
			int stallGuard = value & 0xf;
			int current = value & 0x1F0;
			Serial.print("Approx Stall Guard: ");
			Serial.println(stallGuard);
			Serial.print("Current level");
			Serial.println(current);
		}
	}
#endif
}

/*
 * send register settings to the stepper driver via SPI
 * returns the current status
 */
inline void CCStepperDevice_TMC260::send262(unsigned long datagram) {
	unsigned long i_datagram;
    
    //preserver the previous spi mode
//    unsigned char oldMode =  SPCR & SPI_MODE_MASK;
	
    //if the mode is not correct set it to mode 3
//    if (oldMode != SPI_MODE3) {
        SPI.setDataMode(SPI_MODE3);
//    }
	
	//select the TMC driver
	digitalWrite(chipSelect_pin,LOW);
    
	//ensure that only valid bist are set (0-19)
	//datagram &=REGISTER_BIT_PATTERN;
	
#ifdef DEBUG
	Serial.print("Sending ");
	Serial.println(datagram,HEX);
#endif
    
	//write/read the values
	i_datagram = SPI.transfer((datagram >> 16) & 0xff);
	i_datagram <<= 8;
	i_datagram |= SPI.transfer((datagram >>  8) & 0xff);
	i_datagram <<= 8;
	i_datagram |= SPI.transfer((datagram) & 0xff);
	i_datagram >>= 4;
	
#ifdef DEBUG
	Serial.print("Received ");
	Serial.println(i_datagram,HEX);
	debugLastStatus();
#endif
	//deselect the TMC chip
	digitalWrite(chipSelect_pin,HIGH);
    
    //restore the previous SPI mode if neccessary
    //if the mode is not correct set it to mode 3
//    if (oldMode != SPI_MODE3) {
//        SPI.setDataMode(oldMode);
//    }
    
	
	//store the datagram as status result
	driver_status_result = i_datagram;
}