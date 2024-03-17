//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_GANTRY_H
#define GAMTRY_CODE_GANTRY_H

#include "Position.h"
#include "AccelStepper.h"


#define X1_MOTOR_PINS 24, 22
#define X2_MOTOR_PINS 28, 26
#define Y_MOTOR_PINS 25, 23
#define Z_MOTOR_PINS 29, 27
#define THETA_MOTOR_PINS 37, 35


#define X_MICROSTEPS 4
#define Y_MICROSTEPS 1
#define Z_MICROSTEPS 4
#define THETA_MICROSTEPS 1


#define X_STEPS_PER_MM (5.0 * X_MICROSTEPS)  // CORRECT
#define Y_STEPS_PER_MM (5.0 * Y_MICROSTEPS)  // UNKNOWN (should be 5.0)
#define Z_STEPS_PER_MM (200.0 * Z_MICROSTEPS) // CALCULATED, NEEDS TESTING
#define THETA_STEPS_PER_DEG (40.0 * THETA_MICROSTEPS / 9.0) //

#define X_MM_PER_STEP (1 / X_STEPS_PER_MM)
#define Y_MM_PER_STEP (1 / Y_STEPS_PER_MM)
#define Z_MM_PER_STEP (1 / Z_STEPS_PER_MM)
#define THETA_DEG_PER_STEP (1 / THETA_STEPS_PER_DEG)


#define X_MAX_STEPS_PER_SECOND (500.0 * X_MICROSTEPS)
#define Y_MAX_STEPS_PER_SECOND (500.0 * Y_MICROSTEPS)
#define Z_MAX_STEPS_PER_SECOND (500.0 * Z_MICROSTEPS)
#define THETA_MAX_STEPS_PER_SECOND (500.0 * THETA_MICROSTEPS)

#define X_MAX_MM_PER_SECOND (X_MAX_STEPS_PER_SECOND * X_MM_PER_STEP)
#define Y_MAX_MM_PER_SECOND (Y_MAX_STEPS_PER_SECOND * Y_MM_PER_STEP)
#define Z_MAX_MM_PER_SECOND (Z_MAX_STEPS_PER_SECOND * Z_MM_PER_STEP)
#define THETA_MAX_DEG_PER_SECOND (THETA_MAX_STEPS_PER_SECOND * THETA_DEG_PER_STEP)

#define X_ZEROING_STEPS_PER_SECOND (X_MAX_STEPS_PER_SECOND / 8)
#define Y_ZEROING_STEPS_PER_SECOND (X_MAX_STEPS_PER_SECOND / 8)
#define Z_ZEROING_STEPS_PER_SECOND (X_MAX_STEPS_PER_SECOND / 8)
#define THETA_ZEROING_STEPS_PER_SECOND (X_MAX_STEPS_PER_SECOND / 8)

#define MAX_X_MM 400.0
#define MAX_Y_MM 300.0
#define MAX_Z_MM 80.0
#define MAX_THETA_DEG 270.0


#define HEAD_PIN_1
#define HEAD_PIN_2
#define HEAD_PIN_3
#define HEAD_PIN_4

#define LIMIT_SWITCH_NORMALLY_CLOSED true

#define X1_LIMIT_SWITCH_PIN 8
#define X2_LIMIT_SWITCH_PIN 9
#define Y_LIMIT_SWITCH_PIN 10
#define Z_LIMIT_SWITCH_PIN 11
#define THETA_LIMIT_SWITCH_PIN 12


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

    void homeXAxis();
    void homeYAxis();
    void homeZAxis();
    void homeThetaAxis();


    boolean x1LimitSwitchTriggered();
    boolean x2LimitSwitchTriggered();
    boolean yLimitSwitchTriggered();
    boolean zLimitSwitchTriggered();
    boolean thetaLimitSwitchTriggered();

    boolean xMaxLimitReached();
    boolean yMaxLimitReached();
    boolean zMaxLimitReached();
    boolean thetaMaxLimitReached();

};


#endif //GAMTRY_CODE_GANTRY_H
