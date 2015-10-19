#include <Arduino.h>

//#include "motorController.h"

#include "CCDevice.h"
#include <Servo.h>


#define CCSwitchDevice_BASICOUTPUT           0x01

#define CCSwitchDevice_VERBOSE               CCSwitchDevice_BASICOUTPUT





class CCSwitchDevice : public CCDevice {

  public:

    unsigned char       switching_pin;
    bool                defaultState;


    CCSwitchDevice(String deviceName, unsigned char switching_pin, bool defaultState);
    ~CCSwitchDevice();


    void attachDevice();
    void detachDevice();
    void enableDevice();
    void disableDevice();
    
    void reviewValues();
    void prepareNextMove();
    void startMove();
    void driveDynamic();
    void initiateStop();
    void stopMoving();
    void finishMove();
};

