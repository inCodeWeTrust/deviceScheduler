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








CCStepperDevice_TMC260::CCStepperDevice_TMC260(unsigned int deviceIndex, String deviceName, unsigned char step_pin, unsigned char dir_pin, unsigned char enable_pin, unsigned char chipSelect_pin, unsigned int currentMax, unsigned int stepsPerRotation) : CCStepperDevice() {
    
    
    this->deviceIndex = deviceIndex;
    this->deviceName = deviceName;
    this->dir_pin = dir_pin;
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
	this->chipSelect_pin = chipSelect_pin;
    
    this->currentMax = currentMax;
    this->stepsPerRotation = stepsPerRotation;
    
    this->highestSteppingMode = 8;

    this->steppingUnit = new unsigned char[highestSteppingMode + 1];                        // create array for increment of microSteps according to microSteppingMode
    
    for (unsigned char codeIndex = 0; codeIndex <= highestSteppingMode; codeIndex++) {
        this->steppingUnit[codeIndex] = (1 << (highestSteppingMode - codeIndex));
    }
    
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

    // currentMax: 1600mA | SGT: 10
    
    
    // #################### DriverControlRegister: ####################
    
    // Enable STEP interpolation (INTPOL):
    //  0: Disable STEP pulse interpolation.
    //  1: Enable STEP pulse multiplication by 16.
    boolean stepInterpolation = 0;

    // Enable double edge STEP pulses (DEDGE):
    //  0: Rising STEP pulse edge is active, falling edge is inactive.
    //  1: Both rising and falling STEP pulse edges are active.
    boolean doubleEdgeStepPulses = 0;

    // Microstep resolution for STEP/DIR mode (MRES3):
    //  Microsteps per 90°:
    //  %0000 (0): 256
    //  %0001 (1): 128
    //  %0010 (2): 64
    //  %0011 (3): 32
    //  %0100 (4): 16
    //  %0101 (5): 8
    //  %0110 (6): 4
    //  %0111 (7): 2 (halfstep)
    //  %1000 (8): 1 (fullstep)
    byte microSteppingMode = 8;
    
    setDriverControlRegister(stepInterpolation, doubleEdgeStepPulses, microSteppingMode);

    
    // #################### ChopperControlRegister: ####################
    // blanking time (TBL):
    //  Blanking time interval, in system clock periods:
    //  %00: 16; %01: 24; %10: 36; %11: 54
    //  internal clock: 15 MHz ==> 1 systemClockPeriod is 67 ns
    //  16: 1.06 us [= 0]; 24: 1.6 us [= 1]; 36: 2.4 us [= 2]; 54: 3.6 us [= 3]
    byte blankingTimeValue = 3;
    
    // chopper mode (CHM):
    //  This mode bit affects the interpretation of the HDEC, HEND, and HSTRT parameters shown below.
    //  0: Standard mode (spreadCycle)
    //  1: Constant tOFF with fast decay time. Fast decay time is also terminated when the negative nominal current is reached. Fast decay is after on time.
    boolean chopperMode = 0;
    
    // random Toff time (RNDTF):
    //  Enable randomizing the slow decay phase duration:
    //  0: Chopper off time is fixed as set by bits tOFF
    //  1: Random mode, tOFF is random modulated by dNCLK= -12 ... +3 clocks.
    boolean randomTOffTime = 0;
    
    // Off time/MOSFET disable (TOFF):
    //  Duration of slow decay phase. If TOFF is 0, the MOSFETs are shut off. If TOFF is nonzero, slow decay time is a multiple of system clock periods: NCLK= 12 + (32 x TOFF) (Minimum time is 64clocks.)
    //  %0000: Driver disable, all bridges off
    //  %0001: 1 (use with TBL of minimum 24 clocks) %0010 ... %1111: 2 ... 15
    //  internal clock: 15 MHz ==> 1 systemClockPeriod is 67 ns, clocks = 12 + 32 * offTime
    //  offTime 0: MOSFET shut off; 2: (76 cycles): 5.1 us; 3: (108 cycles): 7.2 us; 15 (492 cycles): 32.8 us
    byte offTime = 3;
    
    if (chopperMode == 0) {
        // spreadCycleMode

        // hysteresis decrement interval (HDEC):
        //  Hysteresis decrement period setting, in system clock periods:
        //  %00: 16; %01: 32; %10: 48; %11: 64
        byte hysteresisDecrementPeriodValue = 0;
        
        // hysteresis end (low) value (HEND):
        //  %0000 ... %1111: Hysteresis is -3, -2, -1, 0, 1, ..., 12
        //  (1/512 of this setting adds to current setting) This is the hysteresis value which becomes used for the hysteresis chopper.
        int hysteresisEnd = 4;
        
        // hysteresis start value (HSTRT)
        //  hysteresis start offset from HEND
        //  %000: 1; %001: 2; %010: 3; %011: 4; ... %111: 8
        //  Effective: HEND + HSTRT must be ≤ 15
        byte hysteresisStart = 1;
    
        setChopperControlRegister_spreadCycle(blankingTimeValue, chopperMode, randomTOffTime, hysteresisDecrementPeriodValue, hysteresisEnd, hysteresisStart, offTime);

    } else {
        // fastDecayMode
        
        // Fast decay mode (with constant offTime) (HDEC):
        //  0: current comparator can terminate the fast decay phase before timer expires.
        //  1: only the timer terminates the fast decay phase.
        boolean onlyTimerTerminatesDecayPhase = 0;
        
        // sinwave offset (HEND):
        //  %0000 ... %1111; Offset is -3, -2, -1, 0, 1, ..., 12
        //  This is the sine wave offset and 1/512 of the value becomes added to the absolute value of each sine wave entry.
        int sinwaveOffset = 3;
        
        // fast decay time (HSTRT):
        //  duration of the fast decay phase. The MSB is HDEC0.
        //  Fast decay time is a multiple of system clock periods: NCLK= 32 x (HDEC0+HSTRT)
        byte fastDecayTime = 3;
        
        // Off time/MOSFET disable (TOFF):
        //  Duration of slow decay phase. If TOFF is 0, the MOSFETs are shut off. If TOFF is nonzero, slow decay time is a multiple of system clock periods: NCLK= 12 + (32 x TOFF) (Minimum time is 64clocks.)
        //  %0000: Driver disable, all bridges off
        //  %0001: 1 (use with TBL of minimum 24 clocks) %0010 ... %1111: 2 ... 15
        //  internal clock: 15 MHz ==> 1 systemClockPeriod is 67 ns, clocks = 12 + 32 * offTime
        //  offTime 0: MOSFET shut off; 2: (76 cycles): 5.1 us; 3: (108 cycles): 7.2 us; 15 (492 cycles): 32.8 us
        byte offTime = 3;
        
        setChopperControlRegister_fastDecay(blankingTimeValue, chopperMode, randomTOffTime, onlyTimerTerminatesDecayPhase , sinwaveOffset, fastDecayTime, offTime);
    }
 
    
    
    // #################### CoolStepControlRegister: ####################
    
    // Minimum coolStep current (SEIMIN):
    // 0: 1⁄2 CS current setting; 1: 1⁄4 CS current setting
    byte minCoolStepCurrentValue = 1;
    
    // current decrerment speed (SEDN):
    //  Number of times that the stallGuard2 value must be sampled equal to or above the upper threshold for each decrement of the coil current:
    //  %00: 32; %01: 8; %10: 2; %11: 1
    byte currentDecrementSpeedValue = 0x11;
    
    // upper cool step treshold as an offset from the lower threshold (SEMAX):
    //  If the stallGuard2 measurement value SG is sampled equal to or above (SEMIN+SEMAX+1) x 32 enough times, then the coil current scaling factor is decremented.
    byte upperCoolStepThreshold = 2;
    
    // current increment size (SEUP):
    // Number of current increment steps for each time that the stallGuard2 value SG is sampled below the lower threshold:
    //  %00: 1; %01: 2; %10: 4; %11: 8
    byte currentIncrementStepsValue = 3;
    
    // lower coolStep threshold / coolStep disable (SEMIN)
    // If SEMIN is 0, coolStep is disabled. If SEMIN is nonzero and the stallGuard2 value SG falls below SEMIN x 32, the coolStep current scaling factor is increased.
    byte lowerCoolStepThreshold = 0;
    
    setCoolStepRegister(minCoolStepCurrentValue, currentDecrementSpeedValue, upperCoolStepThreshold, currentIncrementStepsValue, lowerCoolStepThreshold);
    

    
    
    // #################### StallGuard2ControlRegister: ####################

    // stallGuard2 filter enable (SFILT):
    //  0: Standard mode, fastest response time.
    //  1: Filtered mode, updated once for each four fullsteps to compensate for variation in motor construction, highest accuracy.
    boolean stallGuard2FilterEnable = true;

    // stallGuard2 threshold value (SGT):
    // The stallGuard2 threshold value controls the optimum measurement range for readout. A lower value results in a higher sensitivity and requires less torque to indicate a stall. The value is a two’s complement signed integer. Values below -10 are not recommended. Range: -64 to +63
    int stallGuard2Threshold = 10;

    // Current scale (scales digital currents A and B) (CS)
    //  Current scaling for SPI and step/direction operation. %00000 ... %11111: 1/32, 2/32, 3/32, ... 32/32
    //  This value is biased by 1 and divided by 32, so the range is 1/32 to 32/32. Example: CS=0 is 1/32 current
    // calculate it:
    calculateCurrentSetup(currentMax);
    
    setStallGuard2Register(stallGuard2FilterEnable, stallGuard2Threshold);

    
    
    // #################### DriverConfigurationRegister: ####################
    
    
    // Slope control, high side (SLPH):
    //  %00: Minimum;
    //  %01: Minimum temperature compensation mode.
    //  %10: Medium temperature compensation mode.
    //  %11: Maximum
    //  In temperature compensated mode (tc), the MOSFET gate driver strength is increased if the overtemperature warning temperature is reached. This compensates for temperature dependency of high-side slope control.
    byte slopeControlHighSide = 0x10;
    
    // Slope control, low side (SLPL):
    //  %00: Minimum. %01: Minimum. %10: Medium. %11: Maximum.￼
    byte slopeControlLowSide = 0x10;
    
    // Short to GND protection disable (DISS2G):
    //  0: Short to GND protection is enabled. 1: Short to GND protection is disabled.
    boolean shortToGndProtectionDisable = 0;
    
    // Short to GND detection timer (TS2G1):
    //  %00: 3.2μs. %01: 1.6μs. %10: 1.2μs. %11: 0.8μs.
    byte shortToGndDetectionTimerValue = 0x01;
    
    // STEP/DIR interface disable (SDOFF):
    //  0: Enable STEP and DIR interface; 1: Disable STEP and DIR interface. SPI interface is used to move motor.
    boolean stepDirInterfaceDisable = false;
    
    // Sense resistor voltage-based current scaling (VSENSE):
    //  0: Full-scale sense resistor voltage is 305mV.
    //  1: Full-scale sense resistor voltage is 165mV. (Full-scale refers to a current setting of 31 and a DAC value of 255.)
    // senseResistorVoltage165mV is calculated in calculateCurrentSetup() called in stallGuard2Register setup
    
    // Select value for read out (RD bits) (RDSEL):
    //  %00: Microstep position read back
    //  %01: stallGuard2 level read back
    //  %10: stallGuard2 and coolStep current level read back
    //  %11: Reserved, do not use
    byte selectReadOut = 0x10;

    setDriverConfigurationRegister(slopeControlHighSide, slopeControlLowSide, shortToGndProtectionDisable, shortToGndDetectionTimerValue, stepDirInterfaceDisable, selectReadOut);

    
    
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
        Serial.print(F(", currentMax: "));
        Serial.print(currentMax);
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
    CCStepperDevice_TMC260::setupMicroSteppingMode();
}
void CCStepperDevice_TMC260::setupMicroSteppingMode() {
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
        Serial.print(F("##### set microSteppingValue: "));
        Serial.print(8 - microSteppingMode);
        Serial.print(F(" = mode "));
        Serial.println(microSteppingMode);
    }
    //delete the old value
    this->driverControl &= 0xFFFF0ul;
    //set the new value
    this->driverControl |= (8 - microSteppingMode);
    
    doTransaction(driverControl);
}





void CCStepperDevice_TMC260::calculateCurrentSetup(unsigned int current) {
    // the mesurement full scale on the chip can eiter be set to 165mV or 310mV
    // the shunt on the shield (R_SENSE) is 150mOhm
    // VsenseMax = R_SENSE * coilCurrentMax [/ 1000]
    //  ==> coilCurrentMax = VsenseMax / R_SENSE:
    // VsenseMax = 165mV ==> coilCurrentMax = 165mV / 150mOhm = 1100mA
    // VsenseMax = 310mV ==> coilCurrentMax = 310mV / 150mOhm = 2066mA
    
    unsigned int VSenseFullScale;                        // voltage in mV - it can either be 165mV or 310MV
    if ((long) R_SENSE * current < 165000) {
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
    //   300 mA: 32 * 0.15 * 0.3 / 0.165 =  8.73 -->  9
    //   600 mA: 32 * 0.15 * 0.6 / 0.165 = 17.45 --> 17
    //  1000 mA: 32 * 0.15 * 1.0 / 0.165 = 29.09 --> 29
    //  1090 mA: 32 * 0.15 * 1.09/ 0.165 = 31.71 --> 32
    //  1100 mA: 32 * 0.15 * 1.1 / 0.310 = 17.03 --> 17
    //  2060 mA: 32 * 0.15 * 1.0 / 0.310 = 31.89 --> 32

    currentScaleOf32 = min(currentScaleOf32, 32);

    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
        Serial.print(F("##### calculateCurrentSetup: currentScaleOf32: "));
        Serial.println(currentScaleOf32);
    }

}




void CCStepperDevice_TMC260::setCurrent(unsigned int current) {
    calculateCurrentSetup(current);
    
    //delete the old value
	stallGuard2Control &= ~0x1F;
	//set the new current scaling
	stallGuard2Control |= (currentScaleOf32 - 1) & 0x1F;
	doTransaction(stallGuard2Control);

    bitWrite(driverConfiguration, 6, senseResistorVoltage165mV);
    doTransaction(driverConfiguration);
	
}


void CCStepperDevice_TMC260::getReadOut(byte theReadOut) {
    
    driverConfiguration &= ~0x30;
    driverConfiguration |= (theReadOut & 0x3) << 4;
    
    doTransaction(driverConfiguration);
    
    
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SPIDEBUG) {
        Serial.print("Receive from chip: ");
        printDatagram(this->resultDatagram);
    }
    
    switch (theReadOut) {
        case READOUT_MICROSTEP_POSITION:
            microstepPosition = (this->resultDatagram >> 10) & 0x1FF;
            if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
                Serial.print("  microstepPosition: ");
                Serial.print(microstepPosition);
            }
            break;
        case READOUT_STALLGUARD_LEVEL:
            stallGuard2Value = (this->resultDatagram >> 10) & 0x3FF;
            if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
                Serial.print("  stallGuard2Value: ");
                Serial.print(stallGuard2Value);
            }
            break;
        default:
            coolStepScalingValue = (this->resultDatagram >> 10) & 0x1F;
            stallGuard2Value_upper = (this->resultDatagram >> 15) & 0x1F;
            
            if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
                Serial.print("  coolStepScalingValue: ");
                Serial.print(coolStepScalingValue);
                Serial.print("  stallGuard2Value_upper: ");
                Serial.print(stallGuard2Value_upper);
            }
    }
    
    standStil = bitRead(this->resultDatagram, 7);
    
    openLoad = (this->resultDatagram & 0x60) >> 5;
    
    shortToGnd = (this->resultDatagram & 0x18) >> 3;
    
    overTemperatureWarning = this->resultDatagram & 0x4;
    
    overTemperatureShutdown = this->resultDatagram & 0x2;
    
    stallGuard2Status = this->resultDatagram & 0x1;
    
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
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

void CCStepperDevice_TMC260::doTransaction(unsigned long datagram) {
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SPIDEBUG) {
        Serial.print("### Sending: ");
        printDatagram(datagram);
    }
    
    //select the TMC260
    digitalWrite(chipSelect_pin, LOW);
    
    //write/read the values
    resultDatagram = (SPI.transfer((datagram >> 16) & 0xff)) & 0xff;
    resultDatagram <<= 8;
    resultDatagram |= SPI.transfer((datagram >>  8) & 0xff);
    resultDatagram <<= 8;
    resultDatagram |= SPI.transfer((datagram) & 0xff);
//    Serial.println(resultDatagram, HEX);
    resultDatagram &= 0xfffff0;
    resultDatagram >>= 4;
    
    //deselect the TMC chip
    digitalWrite(chipSelect_pin, HIGH);

    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SPIDEBUG) {
        Serial.print("   ---   Receiving: ");
        printDatagram(resultDatagram);
        Serial.println();
    }
    
    
}
void CCStepperDevice_TMC260::setDriverControlRegister(boolean stepInterpolation, boolean doubleEdgeStepPulses, byte microSteppingMode) {
    
    driverControl = 0x00000;
    
    bitWrite(driverControl, 9, stepInterpolation);
    bitWrite(driverControl, 8, doubleEdgeStepPulses);
    driverControl |= microSteppingMode & 0x0f;
    
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
        Serial.print(F("#####  TMC260 setup: Driver: "));
        Serial.print(F("  stepInterpolation: ")) , Serial.print(stepInterpolation);
        Serial.print(F(", doubleEdgeStepPulses: ")) , Serial.print(doubleEdgeStepPulses);
        Serial.print(F(", microSteppingValue: ")) , Serial.print(8 - microSteppingMode);
        Serial.print(F(" = mode ")) , Serial.println(microSteppingMode);
    }
    
    
    doTransaction(driverControl);
    
}
void CCStepperDevice_TMC260::setChopperControlRegister_spreadCycle(byte blankingTimeValue, boolean chopperMode, boolean randomTOffTime, byte hysteresisDecrementPeriodValue, int hysteresisEnd, byte hysteresisStart, byte offTime) {
    
    chopperControl = 0x80000;
    
    chopperControl |= (blankingTimeValue & 0x03) << 15;
    bitWrite(chopperControl, 14, chopperMode);
    bitWrite(chopperControl, 13, randomTOffTime);
    chopperControl |= (hysteresisDecrementPeriodValue & 0x3) << 11;
    chopperControl |= ((hysteresisEnd + 3) & 0xF) << 7;
    chopperControl |= ((hysteresisStart - 1) & 0x7) << 4;
    chopperControl |= offTime & 0xF;
    
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
        Serial.print(F("#####  TMC260 setup: Chopper Spread Cycle: "));
        Serial.print(F("  blankingTimeValue: ")) , Serial.print(blankingTimeValue);
        Serial.print(F(", randomTOffTime: ")) , Serial.print(randomTOffTime);
        Serial.print(F(", hysteresisDecrementPeriodValue: ")) , Serial.print(hysteresisDecrementPeriodValue);
        Serial.print(F(", hysteresisEnd: ")) , Serial.print(hysteresisEnd);
        Serial.print(F(", hysteresisStart: ")) , Serial.print(hysteresisStart);
        Serial.print(F(", offTime: ")) , Serial.println(offTime);
    }

    doTransaction(chopperControl);
    
}
void CCStepperDevice_TMC260::setChopperControlRegister_fastDecay(byte blankingTimeValue, boolean chopperMode, boolean randomTOffTime, boolean onlyTimerTerminatesDecayPhase, int sinwaveOffset, byte fastDecayTime, byte offTime) {
    
    chopperControl = 0x80000;
    
    chopperControl |= (blankingTimeValue & 0x03) << 15;
    bitWrite(chopperControl, 14, chopperMode);
    bitWrite(chopperControl, 13, randomTOffTime);
    bitWrite(chopperControl, 12, onlyTimerTerminatesDecayPhase);
    chopperControl |= ((fastDecayTime & 0x8) >> 3) << 11;
    chopperControl |= ((sinwaveOffset + 3) & 0xF) << 7;
    chopperControl |= (fastDecayTime & 0x7) << 4;
    chopperControl |= offTime & 0xF;
    
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
        Serial.print(F("#####  TMC260 setup: Chopper Spread Cycle: "));
        Serial.print(F("  blankingTimeValue: ")) , Serial.print(blankingTimeValue);
        Serial.print(F(", randomTOffTime: ")) , Serial.print(randomTOffTime);
        Serial.print(F(", onlyTimerTerminatesDecayPhase: ")) , Serial.print(onlyTimerTerminatesDecayPhase);
        Serial.print(F(", sinwaveOffset: ")) , Serial.print(sinwaveOffset);
        Serial.print(F(", fastDecayTime: ")) , Serial.print(fastDecayTime);
        Serial.print(F(", offTime: ")) , Serial.println(offTime);
    }

    doTransaction(chopperControl);

}


void CCStepperDevice_TMC260::setCoolStepRegister(byte minCoolStepCurrentValue, byte currentDecrementSpeedValue, byte upperCoolStepThreshold, byte currentIncrementStepsValue, byte lowerCoolStepThreshold) {
    
    coolStepControl = 0xA0000;
    
    bitWrite(coolStepControl, 15, minCoolStepCurrentValue);
    coolStepControl |= (currentDecrementSpeedValue & 0x3) << 13;
    coolStepControl |= (upperCoolStepThreshold & 0xF) << 8;
    coolStepControl |= (currentIncrementStepsValue & 0x3) << 5;
    coolStepControl |= lowerCoolStepThreshold & 0xF;
    
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
        Serial.print(F("#####  TMC260 setup: Cool Step: "));
        Serial.print(F("  minCoolStepCurrentValue: ")) , Serial.print(minCoolStepCurrentValue);
        Serial.print(F(", currentDecrementSpeedValue: ")) , Serial.print(currentDecrementSpeedValue);
        Serial.print(F(", upperCoolStepThreshold: ")) , Serial.print(upperCoolStepThreshold);
        Serial.print(F(", currentIncrementStepsValue: ")) , Serial.print(currentIncrementStepsValue);
        Serial.print(F(", lowerCoolStepThreshold: ")) , Serial.println(lowerCoolStepThreshold);
    }

    doTransaction(coolStepControl);

}
void CCStepperDevice_TMC260::setStallGuard2Register(boolean stallGuard2FilterEnable, int stallGuard2Threshold) {

    stallGuard2Control = 0xC0000;
 
    bitWrite(stallGuard2Control, 16, stallGuard2FilterEnable);
    stallGuard2Control |= (stallGuard2Threshold & 0x7F) << 8;
    stallGuard2Control |= (currentScaleOf32 - 1) & 0x1F;
    
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
        Serial.print(F("#####  TMC260 setup: Stall Guard 2: "));
        Serial.print(F("  stallGuard2FilterEnable: ")) , Serial.print(stallGuard2FilterEnable);
        Serial.print(F(", stallGuard2Threshold: ")) , Serial.print(stallGuard2Threshold);
        Serial.print(F(", currentScaleOf32: ")) , Serial.println(currentScaleOf32);
    }

    doTransaction(stallGuard2Control);

}
void CCStepperDevice_TMC260::setDriverConfigurationRegister(byte slopeControlHighSide, byte slopeControlLowSide, boolean shortToGndProtectionDisable, byte shortToGndDetectionTimerValue, boolean stepDirInterfaceDisable, byte selectReadOut) {
    driverConfiguration = 0xE0000;
    
    driverConfiguration |= (slopeControlHighSide & 0x3) << 14;
    driverConfiguration |= (slopeControlLowSide & 0x3) << 12;
    bitWrite(driverConfiguration, 10, shortToGndProtectionDisable);
    driverConfiguration |= (shortToGndDetectionTimerValue & 0x3) << 8;
    bitWrite(driverConfiguration, 7, stepDirInterfaceDisable);
    bitWrite(driverConfiguration, 6, senseResistorVoltage165mV);
    driverConfiguration |= (selectReadOut & 0x3) << 4;
    
    if (CCSTEPPERDEVICE_TMC260_VERBOSE & CCSTEPPERDEVICE_TMC260_SETUPDEBUG) {
        Serial.print(F("#####  TMC260 setup: Driver Configuration: "));
        Serial.print(F("  slopeControlHighSide: ")) , Serial.print(slopeControlHighSide);
        Serial.print(F(", slopeControlLowSide: ")) , Serial.print(slopeControlLowSide);
        Serial.print(F(", shortToGndProtectionDisable: ")) , Serial.print(shortToGndProtectionDisable);
        Serial.print(F(", shortToGndDetectionTimerValue: ")) , Serial.print(shortToGndDetectionTimerValue);
        Serial.print(F(", stepDirInterfaceDisable: ")) , Serial.print(stepDirInterfaceDisable);
        Serial.print(F(", senseResistorVoltage165mV: ")) , Serial.print(senseResistorVoltage165mV);
        Serial.print(F(", selectReadOut: ")) , Serial.println(selectReadOut);
    }

    
    doTransaction(driverConfiguration);

}


void CCStepperDevice_TMC260::printDatagram(unsigned long datagram) {
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











