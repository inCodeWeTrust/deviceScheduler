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
}

CCDeviceFlow::~CCDeviceFlow() {
    for (int i = countOfTasks - 1; i >= 0; i--) {
        delete task[i];
        task[i] = NULL;
    }

}

CCTask* CCDeviceFlow::addTask(float target, float velocity, float acceleration, float deceleration) {
    return registerTask(target, velocity, acceleration, deceleration, false, false);
}
CCTask* CCDeviceFlow::addTask(float target, float velocity, float acceleration) {
    return registerTask(target, velocity, acceleration, acceleration, false, false);
}
CCTask* CCDeviceFlow::addTask(float target, float velocity) {
    return registerTask(target, velocity, defaultAcceleration, defaultDeceleration, false, false);
}
CCTask* CCDeviceFlow::addTask(float target) {
    return registerTask(target, defaultVelocity, defaultAcceleration, defaultDeceleration, false, false);
}


CCTask* CCDeviceFlow::addTaskMoveRelativ(float relativTarget, float velocity, float acceleration, float deceleration) {
    return registerTask(relativTarget, velocity, acceleration, deceleration, true, false);
}
CCTask* CCDeviceFlow::addTaskMoveRelativ(float relativTarget, float velocity, float acceleration) {
    return registerTask(relativTarget, velocity, acceleration, acceleration, true, false);
}
CCTask* CCDeviceFlow::addTaskMoveRelativ(float relativTarget, float velocity) {
    return registerTask(relativTarget, velocity, defaultAcceleration, defaultDeceleration, true, false);
}
CCTask* CCDeviceFlow::addTaskMoveRelativ(float relativTarget) {
    return registerTask(relativTarget, defaultVelocity, defaultAcceleration, defaultDeceleration, true, false);
}


CCTask* CCDeviceFlow::addTaskWithPositionReset(float target, float velocity, float acceleration, float deceleration) {
    return registerTask(target, velocity, acceleration, deceleration, false, true);
}
CCTask* CCDeviceFlow::addTaskWithPositionReset(float target, float velocity, float acceleration) {
    return registerTask(target, velocity, acceleration, acceleration, false, true);
}
CCTask* CCDeviceFlow::addTaskWithPositionReset(float target, float velocity) {
    return registerTask(target, velocity, defaultAcceleration, defaultDeceleration, false, true);
}
CCTask* CCDeviceFlow::addTaskWithPositionReset(float target) {
    return registerTask(target, defaultVelocity, defaultAcceleration, defaultDeceleration, false, true);
}

CCTask* CCDeviceFlow::registerTask(float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, boolean withPositionReset) {
    task[countOfTasks] = new CCTask(target, velocity, acceleration, deceleration, moveRelativ, withPositionReset, countOfTasks);

//    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
//    Serial.print(F("[CCDeviceFlow]: "));
//        Serial.print(deviceFlowName);
//        Serial.print(F(" add task with target: "));
//        Serial.print(task[countOfTasks]->getTarget());
//        Serial.print(F(", velocity: "));
//        Serial.print(task[countOfTasks]->getVelocity());
//        Serial.print(F(", acceleration: "));
//        Serial.print(task[countOfTasks]->getAcceleration());
//        Serial.print(F(", deceleration: "));
//        Serial.print(task[countOfTasks]->getDeceleration());
//        Serial.print(F(", startDelay: "));
//        Serial.print(task[countOfTasks]->getStartDelay());
//        Serial.print(F("; at $"));
//        Serial.println((long)task[countOfTasks], HEX);
//    }
    
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
        Serial.print(F("task "));
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
        Serial.println(task[t]->getStartDelay());

    }
}
/*
void CCDeviceFlow::deleteTasks() {
    //    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
    Serial.print(F("[CCDeviceFlow]: delete tasks of "));
    Serial.print(deviceFlowName);
    Serial.print(F(": task "));
    //    }
    for (int j = countOfTasks - 1; j >= 0; j--) {
//        if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
            Serial.print(F(" #"));
            Serial.print(j);
//        }
        delete task[j];
        task[j] = NULL;
    }
//    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.println();
        Serial.println();
//    }
    
//    device[device]->setState() = SLEEPING;
    taskPointer = 0;
    countOfTasks = 0;
}
*/

unsigned char CCDeviceFlow::getCountOfTasks(){return countOfTasks;}
void CCDeviceFlow::setCountOfTasks(unsigned char count){countOfTasks = count;}
unsigned char CCDeviceFlow::getTaskPointer(){return taskPointer;}
void CCDeviceFlow::setTaskPointer(unsigned char pointer){taskPointer = pointer;}
void CCDeviceFlow::increaseTaskPointer(){taskPointer++;}
