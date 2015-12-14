


#include "CCDevice.h"


CCDevice::~CCDevice() {}

void CCDevice::defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration) {
    this->defaultVelocity = defaultVelocity;
    this->defaultAcceleration = defaultAcceleration;
    this->defaultDeceleration = defaultDeceleration;
}

/*
 unsigned char CCDevice::addMove(float target) {
 return addMoveWithStartDelay(target, 0UL, defaultVelocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addMove(float target, float velocity) {
 return addMoveWithStartDelay(target, 0UL, velocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addMove(float target, float velocity, float acceleration) {
 return addMoveWithStartDelay(target, 0UL, velocity, acceleration, defaultDeceleration);
 }
 */
unsigned char CCDevice::addMove(float target, float velocity, float acceleration, float deceleration) {
    return addMoveWithStartDelay(target, 0UL, velocity, acceleration, deceleration);
}

/*
 unsigned char CCDevice::addMoveWithStartDelay(float target, unsigned long startDelay) {
 return addMoveWithStartDelay(target, startDelay, defaultVelocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addMoveWithStartDelay(float target, unsigned long startDelay, float velocity) {
 return addMoveWithStartDelay(target, startDelay, velocity, defaultAcceleration, defaultDeceleration);
 }
 unsigned char CCDevice::addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration) {
 return addMoveWithStartDelay(target, startDelay, velocity, acceleration, defaultDeceleration);
 }
 */
unsigned char CCDevice::addMoveWithStartDelay(float target, unsigned long startDelay, float velocity, float acceleration, float deceleration) {
    theMove[countOfMoves] = new onEventMove(target, velocity, acceleration, deceleration, startDelay);
    
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCDevice]: add Move for "));
        Serial.print(deviceName);
        Serial.print(F(", target: "));
        Serial.print(theMove[countOfMoves]->target);
        Serial.print(F(", velocity: "));
        Serial.print(theMove[countOfMoves]->velocity);
        Serial.print(F(", acceleration: "));
        Serial.print(theMove[countOfMoves]->acceleration);
        Serial.print(F(", deceleration: "));
        Serial.print(theMove[countOfMoves]->deceleration);
        Serial.print(F(", startDelay: "));
        Serial.print(theMove[countOfMoves]->startDelay);
        Serial.print(F("; at $"));
        Serial.println((long)theMove[countOfMoves], HEX);
    }
    
    countOfMoves++;
    
    return countOfMoves - 1;
}

void CCDevice::setStartDateForMove(unsigned char moveIndex, unsigned long startTime) {
    theMove[moveIndex]->startEvent = DATE;
    theMove[moveIndex]->startTime = startTime;
}
void CCDevice::setStartButtonForMove(unsigned char moveIndex, unsigned char startButton, boolean startButtonState) {
    theMove[moveIndex]->startEvent = BUTTON;
    theMove[moveIndex]->startButton = startButton;
    theMove[moveIndex]->startButtonState = startButtonState;
}
void CCDevice::setStartEventForMove(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove, signed long startTriggerPosition) {
    theMove[moveIndex]->startEvent = POSITION;
    theMove[moveIndex]->startTriggerDevice = startTriggerDevice;
    theMove[moveIndex]->startTriggerMove = startTriggerMove;
    theMove[moveIndex]->startTriggerPosition = startTriggerPosition;
}
void CCDevice::setStartAfterCompletion(unsigned char moveIndex, unsigned char startTriggerDevice, unsigned char startTriggerMove) {
    theMove[moveIndex]->startEvent |= FOLLOW;
    theMove[moveIndex]->startTriggerDevice = startTriggerDevice;
    theMove[moveIndex]->startTriggerMove = startTriggerMove;
}

void CCDevice::setSwitchDateForMove(unsigned char moveIndex, unsigned long timeout) {
    theMove[moveIndex]->stopEvent = DATE | SWITCH;
    theMove[moveIndex]->timeout = timeout;
}
void CCDevice::setSwitchButtonForMove(unsigned char moveIndex, unsigned char stopButton, boolean stopButtonState) {
    theMove[moveIndex]->stopEvent = BUTTON | SWITCH;
    theMove[moveIndex]->stopButton = stopButton;
    theMove[moveIndex]->stopButtonState = stopButtonState;
}
void CCDevice::setSwitchEventForMove(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition) {
    theMove[moveIndex]->stopEvent = POSITION | SWITCH;
    theMove[moveIndex]->stopTriggerDevice = stopTriggerDevice;
    theMove[moveIndex]->stopTriggerMove = stopTriggerMove;
    theMove[moveIndex]->stopTriggerPosition = stopTriggerPosition;
}

void CCDevice::setTimeoutForMove(unsigned char moveIndex, unsigned long timeout, boolean stopSharply) {
    theMove[moveIndex]->stopEvent = DATE;
    theMove[moveIndex]->timeout = timeout;
    theMove[moveIndex]->stopSharply = stopSharply;
}
void CCDevice::setStopButtonForMove(unsigned char moveIndex, unsigned char stopButton, boolean stopButtonState, boolean stopSharply) {
    theMove[moveIndex]->stopEvent = BUTTON;
    theMove[moveIndex]->stopButton = stopButton;
    theMove[moveIndex]->stopButtonState = stopButtonState;
    theMove[moveIndex]->stopSharply = stopSharply;
}
void CCDevice::setStopEventForMove(unsigned char moveIndex, unsigned char stopTriggerDevice, unsigned char stopTriggerMove, signed long stopTriggerPosition, boolean stopSharply) {
    theMove[moveIndex]->stopEvent = POSITION;
    theMove[moveIndex]->stopTriggerDevice = stopTriggerDevice;
    theMove[moveIndex]->stopTriggerMove = stopTriggerMove;
    theMove[moveIndex]->stopTriggerPosition = stopTriggerPosition;
    theMove[moveIndex]->stopSharply = stopSharply;
}
void CCDevice::setStopDynamicalForMove(unsigned char moveIndex, unsigned char sensor, unsigned int initiatePerformanceValue, unsigned int stopValue, unsigned char stopPerformance) {
    theMove[moveIndex]->stopDynamically = true;
    theMove[moveIndex]->sensor = sensor;
    theMove[moveIndex]->initiatePerformanceValue = initiatePerformanceValue;
    theMove[moveIndex]->stopValue = stopValue;
    theMove[moveIndex]->stopPerformance = stopPerformance;
}

void CCDevice::deleteMoves() {
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCDevice]: delete moves of "));
        Serial.print(deviceName);
        Serial.print(F(": move "));
    }
    for (int j = countOfMoves - 1; j >= 0; j--) {
        if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
            Serial.print(F(" #"));
            Serial.print(j);
        }
        delete theMove[j];
        theMove[j] = NULL;
    }
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.println();
        Serial.println();
    }
    
    state = 0;
    movePointer = 0;
    countOfMoves = 0;
    
//    currentPosition = 0;
//    directionDown = 0;
    
}
