//
//  CCControlButton.cpp
//  deviceScheduler
//
//  Created by Little Abakus on 01.04.16.
//  Copyright (c) 2016 Little Abakus. All rights reserved.
//


#include "CCControlButton.h"



CCControlButton::CCControlButton(unsigned int buttonIndex, String buttonName, unsigned char button_pin, boolean buttonActiv, boolean pullup) {
    
    this->buttonIndex = buttonIndex;
    this->buttonName = buttonName;
    this->button_pin = button_pin;
    this->buttonActiv = buttonActiv;
    this->pullup = pullup;
    
    if (pullup) {
        pinMode(button_pin, INPUT_PULLUP);
    } else {
        pinMode(button_pin, INPUT);
    }
    
    //    this->notificationCode = CONTROLLBUTTON_PRESSED_CODE;
    //    this->notificationText = CONTROLLBUTTON_PRESSED_NOTIFICATION;
    
    countOfActions = 0;
    
    
    if (CONTROLBUTTON_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: setup "));
        Serial.print(buttonName);
        Serial.print(F(", button_pin: "));
        Serial.print(button_pin);
        Serial.print(F(", buttonActiv: "));
        Serial.print(buttonActiv);
        Serial.print(F(", pullup: "));
        Serial.print(pullup);
        if (CONTROLBUTTON_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.println((long)this, HEX);
        }
    }    
}
CCControlButton::~CCControlButton() {
    if (CONTROLBUTTON_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: button "));
        Serial.print(buttonName);
        Serial.println(F(" destructed"));
    }
}




void CCControlButton::readButtonState() {
    state = digitalRead(button_pin);
    activ = (state == buttonActiv);
    
    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_MONITORING) {
        Serial.print(F("[CCControlButton]: "));
        Serial.print(buttonName);
        Serial.print(F(", state: "));
        Serial.print(state);
        Serial.print(F(", activ: "));
        Serial.println(activ);
    }
    

}

boolean CCControlButton::readIfActiv() {
    state = digitalRead(button_pin);
    activ = (state == buttonActiv);
    
    //    if (CONTROLBUTTON_VERBOSE & BASICOUTPUT) {
    //        Serial.print(F("[CCControlButton]: "));
    //        Serial.print(buttonName);
    //        Serial.print(F(", state: "));
    //        Serial.print(state);
    //        Serial.print(F(", activ: "));
    //        Serial.println(activ);
    //    }
    
    
    return activ;
}

boolean CCControlButton::isActiv(){return activ;}
boolean CCControlButton::getState(){return state;}

CCControlButton::buttonAction CCControlButton::getAction(unsigned char a){return action[a];}
void CCControlButton::setActionDone(unsigned char a){action[a].actionDone = true;}
void CCControlButton::resetActionDone(unsigned char a){action[a].actionDone = false;}
String CCControlButton::getButtonName(){return buttonName;}
String CCControlButton::getButtonActiv(){if (buttonActiv) return "HIGH"; return "LOW";}



