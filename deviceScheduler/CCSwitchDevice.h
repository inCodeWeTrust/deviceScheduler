#include <Arduino.h>

//#include "motorController.h"

#include "CCDevice.h"


#define CCSwitchDevice_BASICOUTPUT           0x01

#define CCSwitchDevice_VERBOSE               CCSwitchDevice_BASICOUTPUT





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
    void driveDynamic();
    void initiateStop();
    void stopMoving();
    void finishMove();
};

