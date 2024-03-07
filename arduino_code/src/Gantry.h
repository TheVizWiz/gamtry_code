//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_GANTRY_H
#define GAMTRY_CODE_GANTRY_H

#include "Position.h"
#include "AccelStepper.h"


#define X1_MOTOR_PINS 5, 6
#define X2_MOTOR_PINS 9, 10
#define Y_MOTOR_PINS 30, 32
#define Z_MOTOR_PINS 31, 33
#define THETA_MOTOR_PINS 7, 8

#define X_STEPS_PER_MM 5.0
#define Y_STEPS_PER_MM 4.5
#define Z_STEPS_PER_MM 4.0
#define THETA_STEPS_PER_DEG (40.0 / 9.0)

#define X_MM_PER_STEP (1 / X_STEPS_PER_MM)
#define Y_MM_PER_STEP (1 / Y_STEPS_PER_MM)
#define Z_MM_PER_STEP (1 / Z_STEPS_PER_MM)
#define THETA_DEG_PER_STEP (1 / THETA_STEPS_PER_DEG)

#define X_MAX_STEPS_PER_SECOND 600.0
#define Y_MAX_STEPS_PER_SECOND 1000.0
#define Z_MAX_STEPS_PER_SECOND 1000.0
#define THETA_MAX_STEPS_PER_SECOND 400.0

#define X_MAX_MM_PER_SECOND (X_MAX_STEPS_PER_SECOND * X_MM_PER_STEP)
#define Y_MAX_MM_PER_SECOND (Y_MAX_STEPS_PER_SECOND * Y_MM_PER_STEP)
#define Z_MAX_MM_PER_SECOND (Z_MAX_STEPS_PER_SECOND * Z_MM_PER_STEP)
#define THETA_MAX_DEG_PER_SECOND (THETA_MAX_STEPS_PER_SECOND * THETA_DEG_PER_STEP)



#define GRIPPER_PIN 9


class Head {

};

struct GantryConfiguration {
    Head head;
    Position position;

    AccelStepper
            x1_motor,
            x2_motor,
            y_motor,
            z_motor,
            theta_motor;

public:

    GantryConfiguration();

    void updatePosition();


};


#endif //GAMTRY_CODE_GANTRY_H
