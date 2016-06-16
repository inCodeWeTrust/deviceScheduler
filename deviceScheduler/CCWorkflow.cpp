//
//  CCWorkflow.cpp
//  deviceScheduler
//
//  Created by little Ikarus on 16.06.16.
//  Copyright (c) 2016 little Ikarus. All rights reserved.
//

#include "CCWorkflow.h"




CCWorkflow::CCWorkflow(String name) {
    this->name = name;
};


scheduledTask CCWorkflow::addTask(schedulerDevice device, float target, float velocity, float acceleration, float deceleration) {
    return registerTask(device, target, velocity, acceleration, deceleration, false, false);
}
scheduledTask CCWorkflow::addTask(schedulerDevice device, float target, float velocity, float acceleration) {
    return registerTask(device, target, velocity, acceleration, acceleration, false, false);
}
scheduledTask CCWorkflow::addTask(schedulerDevice device, float target, float velocity) {
    return registerTask(device, target, velocity, defaultAcceleration, defaultDeceleration, false, false);
}
scheduledTask CCWorkflow::addTask(schedulerDevice device, float target) {
    return registerTask(device, target, defaultVelocity, defaultAcceleration, defaultDeceleration, false, false);
}


scheduledTask CCWorkflow::addTaskMoveRelativ(schedulerDevice device, float relativTarget, float velocity, float acceleration, float deceleration) {
    return registerTask(device, relativTarget, velocity, acceleration, deceleration, true, false);
}
scheduledTask CCWorkflow::addTaskMoveRelativ(schedulerDevice device, float relativTarget, float velocity, float acceleration) {
    return registerTask(device, relativTarget, velocity, acceleration, acceleration, true, false);
}
scheduledTask CCWorkflow::addTaskMoveRelativ(schedulerDevice device, float relativTarget, float velocity) {
    return registerTask(device, relativTarget, velocity, defaultAcceleration, defaultDeceleration, true, false);
}
scheduledTask CCWorkflow::addTaskMoveRelativ(schedulerDevice device, float relativTarget) {
    return registerTask(device, relativTarget, defaultVelocity, defaultAcceleration, defaultDeceleration, true, false);
}


scheduledTask CCWorkflow::addTaskWithPositionReset(schedulerDevice device, float target, float velocity, float acceleration, float deceleration) {
    return registerTask(device, target, velocity, acceleration, deceleration, false, true);
}
scheduledTask CCWorkflow::addTaskWithPositionReset(schedulerDevice device, float target, float velocity, float acceleration) {
    return registerTask(device, target, velocity, acceleration, acceleration, false, true);
}
scheduledTask CCWorkflow::addTaskWithPositionReset(schedulerDevice device, float target, float velocity) {
    return registerTask(device, target, velocity, defaultAcceleration, defaultDeceleration, false, true);
}
scheduledTask CCWorkflow::addTaskWithPositionReset(schedulerDevice device, float target) {
    return registerTask(device, target, defaultVelocity, defaultAcceleration, defaultDeceleration, false, true);
}

scheduledTask CCWorkflow::registerTask(schedulerDevice device, float target, float velocity, float acceleration, float deceleration, boolean moveRelativ, boolean withPositionReset) {
    task[countOfTasks][device] = new CCTask(target, velocity, acceleration, deceleration, moveRelativ, withPositionReset);
    
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCDevice]: add Task for "));
        Serial.print(deviceName);
        Serial.print(F(", target: "));
        Serial.print(task[countOfTasks]->getTarget());
        Serial.print(F(", velocity: "));
        Serial.print(task[countOfTasks]->getVelocity());
        Serial.print(F(", acceleration: "));
        Serial.print(task[countOfTasks]->getAcceleration());
        Serial.print(F(", deceleration: "));
        Serial.print(task[countOfTasks]->getDeceleration());
        Serial.print(F(", startDelay: "));
        Serial.print(task[countOfTasks]->getStartDelay());
        Serial.print(F("; at $"));
        Serial.println((long)task[countOfTasks], HEX);
    }
    
    countOfTasks++;
    
    return countOfTasks - 1;
}

void CCWorkflow::deleteTasks() {
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.print(F("[CCDevice]: delete tasks of "));
        Serial.print(deviceName);
        Serial.print(F(": move "));
    }
    for (int j = countOfTasks - 1; j >= 0; j--) {
        if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
            Serial.print(F(" #"));
            Serial.print(j);
        }
        delete task[j];
        task[j] = NULL;
    }
    if (CCDEVICE_VERBOSE & CCDEVICE_BASICOUTPUT) {
        Serial.println();
        Serial.println();
    }
    
    state = SLEEPING;
    taskPointer = 0;
    countOfTasks = 0;
    
}
