


#include "CCDevice.h"


CCDevice::~CCDevice() {}

unsigned char CCDevice::addMove(float target, float velocity, float acceleration, unsigned long startDelay) {
  theMove[countOfMoves] = new onEventMove(target, velocity, acceleration, startDelay);

  if (CCDevice_VERBOSE & CCDEVICE_MEMORYDEBUG) {
    Serial.print(F("[CCDevice]: add Move for "));
    Serial.print(deviceName);
    Serial.print(F(", target: "));
    Serial.print(theMove[countOfMoves]->target);
    Serial.print(F(", velocity: "));
    Serial.print(theMove[countOfMoves]->velocity);
    Serial.print(F(", acceleration: "));
    Serial.print(theMove[countOfMoves]->acceleration);
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

void CCDevice::deleteMoves() {
  if (CCDevice_VERBOSE & CCDEVICE_MEMORYDEBUG) {
    Serial.print(F("[CCDevice]: delete moves of "));
    Serial.print(deviceName);
    Serial.print(F(": move "));
  }
  for (int j = countOfMoves - 1; j >= 0; j--) {
    if (CCDevice_VERBOSE & CCDEVICE_MEMORYDEBUG) {
      Serial.print(F(" #"));
      Serial.print(j);
    }
    delete theMove[j];
    theMove[j] = NULL;
  }
  if (CCDevice_VERBOSE & CCDEVICE_MEMORYDEBUG) {
    Serial.println();
  }
  state = 0;
  movePointer = 0;
  countOfMoves = 0;
}
