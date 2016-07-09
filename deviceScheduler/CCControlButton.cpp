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
    
    
    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
        Serial.print(F("[CCControlButton]: setup "));
        Serial.print(buttonName);
        Serial.print(F(", button_pin: "));
        Serial.print(button_pin);
        Serial.print(F(", buttonActiv: "));
        Serial.print(buttonActiv);
        Serial.print(F(", pullup: "));
        Serial.print(pullup);
        Serial.print(F(", at $"));
        Serial.println((long)this, HEX);
    }
    
//    action = new buttonAction[8];

}
CCControlButton::~CCControlButton() {
//    delete action[];
    countOfActions = 0;
}


CCAction* CCControlButton::addAction(String actionName) {
    Serial.print(F("[CCControlButton]: add Action for "));
    Serial.println(buttonName);

    action[countOfActions++] = new CCAction(actionName);
    return action[countOfActions - 1];
}

void CCControlButton::deleteActions() {
    countOfActions = 0; 
}


void CCControlButton::readButtonState() {
    state = digitalRead(button_pin);
    activ = (state == buttonActiv);
    
//    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
//        Serial.print(F("[CCControlButton]: "));
//        Serial.print(buttonName);
//        Serial.print(F(", state: "));
//        Serial.print(state);
//        Serial.print(F(", activ: "));
//        Serial.println(activ);
//    }

}

boolean CCControlButton::readIfActiv() {
    state = digitalRead(button_pin);
    activ = (state == buttonActiv);
    
    //    if (CCCONTROLBUTTON_VERBOSE & CCCONTROLBUTTON_BASICOUTPUT) {
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

String CCControlButton::getButtonName(){return buttonName;}
String CCControlButton::getButtonActiv(){if (buttonActiv) return "HIGH"; return "LOW";}
unsigned char CCControlButton::getCountOfActions(){return countOfActions;}



