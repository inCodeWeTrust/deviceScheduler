//
//  CCDeviceFlow.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 16.06.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCDeviceFlow.h"


CCDeviceFlow::CCDeviceFlow(String deviceFlowName, CCDevice* device, float defaultVelocity, float defaultAcceleration, float defaultDeceleration) {
    this->deviceFlowName = deviceFlowName;
    this->device = device;
    this->defaultVelocity = defaultVelocity;
    this->defaultAcceleration = defaultAcceleration;
    this->defaultDeceleration = defaultDeceleration;
    this->countOfTasks = 0;
    
    if (DEVICEFLOW_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceFlow]: setup deviceFlow "));
        Serial.print(this->deviceFlowName);
        Serial.print(F(", defaultVelocity: "));
        Serial.print(this->defaultVelocity);
        Serial.print(F(", defaultAcceleration: "));
        Serial.print(this->defaultAcceleration);
        Serial.print(F(", defaultDeceleration: "));
        Serial.print(this->defaultDeceleration);
        if (DEVICEFLOW_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.print((long)this, HEX);
        }
        Serial.println();
    }

}

CCDeviceFlow::~CCDeviceFlow() {
    if (DEVICEFLOW_VERBOSE & BASICOUTPUT) {
        Serial.print(F("[CCDeviceFlow]: "));
        Serial.print(deviceFlowName);
        Serial.print(F(": delete task "));
    }
    for (int j = countOfTasks - 1; j >= 0; j--) {
        if (DEVICEFLOW_VERBOSE & BASICOUTPUT) {
            Serial.print(F(" #"));
            Serial.print(j);
        }
        delete task[j];
        task[j] = NULL;
    }
    if (DEVICEFLOW_VERBOSE & BASICOUTPUT) {
        Serial.println();
    }
}

void CCDeviceFlow::defineDefaults(float defaultVelocity, float defaultAcceleration, float defaultDeceleration) {
    this->defaultVelocity = defaultVelocity;
    this->defaultAcceleration = defaultAcceleration;
    if (defaultDeceleration == 0.0) {
        this->defaultDeceleration = defaultAcceleration;
    } else {
        this->defaultDeceleration = defaultDeceleration;
    }
}

CCTask* CCDeviceFlow::addTask(float target, float velocity, float acceleration, float deceleration) {
    if (velocity == 0.0) {
        return registerTask(target, defaultVelocity, defaultAcceleration, defaultDeceleration, false, false);
    }
    if (acceleration == 0.0) {
        return registerTask(target, velocity, defaultAcceleration, defaultDeceleration, false, false);
    }
    if (deceleration == 0.0) {
        return registerTask(target, velocity, acceleration, acceleration, false, false);
    }
    return registerTask(target, velocity, acceleration, deceleration, false, false);
}

CCTask* CCDeviceFlow::addTaskMoveRelativ(float relativTarget, float velocity, float acceleration, float deceleration) {
    if (velocity == 0.0) {
        return registerTask(relativTarget, defaultVelocity, defaultAcceleration, defaultDeceleration, true, false);
    }
    if (acceleration == 0.0) {
        return registerTask(relativTarget, velocity, defaultAcceleration, defaultDeceleration, true, false);
    }
    if (deceleration == 0.0) {
        return registerTask(relativTarget, velocity, acceleration, acceleration, true, false);
    }
    return registerTask(relativTarget, velocity, acceleration, deceleration, true, false);
}

CCTask* CCDeviceFlow::addTaskWithPositionReset(float target, float velocity, float acceleration, float deceleration) {
    if (velocity == 0.0) {
        return registerTask(target, defaultVelocity, defaultAcceleration, defaultDeceleration, false, true);
    }
    if (acceleration == 0.0) {
        return registerTask(target, velocity, defaultAcceleration, defaultDeceleration, false, true);
    }
    if (deceleration == 0.0) {
        return registerTask(target, velocity, acceleration, acceleration, false, true);
    }
    return registerTask(target, velocity, acceleration, deceleration, false, true);
}

CCTask* CCDeviceFlow::registerTask(float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, boolean withPositionReset) {
    task[countOfTasks] = new CCTask(target, velocity, acceleration, deceleration, moveRelativ, withPositionReset, countOfTasks);

    if (DEVICEFLOW_VERBOSE & BASICOUTPUT) {
    Serial.print(F("[CCDeviceFlow]: "));
        Serial.print(deviceFlowName);
        Serial.print(F(" add task with target: "));
        Serial.print(task[countOfTasks]->getTarget());
        Serial.print(F(", velocity: "));
        Serial.print(task[countOfTasks]->getVelocity());
        Serial.print(F(", acceleration: "));
        Serial.print(task[countOfTasks]->getAcceleration());
        Serial.print(F(", deceleration: "));
        Serial.print(task[countOfTasks]->getDeceleration());
        Serial.print(F(", startDelay: "));
        Serial.print(task[countOfTasks]->getStartDelay());
        if (moveRelativ) {
            Serial.print(F(", move relativ"));
        }
        if (withPositionReset) {
            Serial.print(F(", with position reset"));
        }
        if (DEVICEFLOW_VERBOSE & MEMORYDEBUG) {
            Serial.print(F(", at $"));
            Serial.print((long)this, HEX);
        }
        Serial.println();
    }
    
    countOfTasks++;
    
    return task[countOfTasks - 1];
}

void CCDeviceFlow::getAllTasks() {
    for (unsigned char t = 0; t < taskPointer; t++) {
        getTask(t);
    }
}
void CCDeviceFlow::getTask(unsigned char t) {
    if (t < countOfTasks) {
        Serial.print(F("[CCDeviceFlow]: "));
        Serial.print(deviceFlowName);
        Serial.print(F(": task "));
        Serial.print(t);
        Serial.print(F(": target: "));
        Serial.print(task[t]->getTarget());
        Serial.print(F(", velocity: "));
        Serial.print(task[t]->getVelocity());
        Serial.print(F(", acceleration: "));
        Serial.print(task[t]->getAcceleration());
        Serial.print(F(", deceleration: "));
        Serial.print(task[t]->getDeceleration());
        Serial.print(F(", startDelay: "));
        Serial.print(task[t]->getStartDelay());
        if (task[t]->getMoveRelativ()) {
            Serial.print(F(", move relativ"));
        }
        if (task[t]->getWithPositionReset()) {
            Serial.println(F(", with position reset"));
        }
    }
}

String CCDeviceFlow::getName(){return deviceFlowName;}
unsigned char CCDeviceFlow::getCountOfTasks(){return countOfTasks;}
void CCDeviceFlow::setCountOfTasks(unsigned char count){countOfTasks = count;}
unsigned char CCDeviceFlow::getTaskPointer(){return taskPointer;}
void CCDeviceFlow::setTaskPointer(unsigned char pointer){taskPointer = pointer;}
void CCDeviceFlow::increaseTaskPointer(){taskPointer++;}
float CCDeviceFlow::getDefaultVelocity(){return defaultVelocity;}
float CCDeviceFlow::getDefaultAcceleration(){return defaultAcceleration;}
float CCDeviceFlow::getDefaultDeceleration(){return defaultDeceleration;}
