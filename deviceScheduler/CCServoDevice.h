#include <Arduino.h>

//#include "motorController.h"

#include "CCDevice.h"
#include <Servo.h>


#define CCServoDevice_BASICOUTPUT         0x01
#define CCServoDevice_MEMORYDEBUG         0x02
#define CCServoDevice_CALCULATIONDEBUG    0x04
#define CCServoDevice_MOVEMENTDEBUG       0x08

//static unsigned char CCServoDevice_VERBOSE = 0;
#define CCServoDevice_VERBOSE            0x0F
//#define CCServoDevice_VERBOSE CCServoDevice_BASICOUTPUT | CCServoDevice_MEMORYDEBUG | CCServoDevice_CALCULATIONDEBUG





class CCServoDevice : public CCDevice {
    unsigned long        t0;
    signed long          elapsedTime;
    signed long          wayToGo, wayForAcceleration, wayForConstantSpeed;
    unsigned long        timeForAcceleration, timeForConstantSpeed;
    signed long          deltaS;



  public:

    Servo                theServo;
    unsigned char        servo_pin;
    unsigned int         startPosition, targetPosition;
    unsigned int         minPosition, maxPosition, parkPosition;



    CCServoDevice(String deviceName, unsigned char servo_pin, int minPosition, int maxPosition, int parkPosition);
    ~CCServoDevice();


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

