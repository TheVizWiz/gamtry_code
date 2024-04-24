//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_GANTRY_H
#define GAMTRY_CODE_GANTRY_H

#include "Position.h"
#include "AccelStepper.h"
#include "Servo.h"
#include "Command/Command.h"

#define X1_MOTOR_PINS 22, 24
#define X2_MOTOR_PINS 26, 28
#define Y_MOTOR_PINS 30, 32
#define Z_MOTOR_PINS 34, 36
#define THETA_MOTOR_PINS 38, 40

#define GRIPPER_PIN 8

#define X_MICROSTEPS 8
#define Y_MICROSTEPS 8
#define Z_MICROSTEPS 8
#define THETA_MICROSTEPS 8


#define X_STEPS_PER_MM (5.0 * X_MICROSTEPS)  // CORRECT
#define Y_STEPS_PER_MM (5.0 * Y_MICROSTEPS)  // UNKNOWN (should be 5.0)
#define Z_STEPS_PER_MM (200.0 * Z_MICROSTEPS) // CALCULATED, NEEDS TESTING
#define THETA_STEPS_PER_DEG (40.0 * THETA_MICROSTEPS / 9.0) //

#define X_MM_PER_STEP (1 / X_STEPS_PER_MM)
#define Y_MM_PER_STEP (1 / Y_STEPS_PER_MM)
#define Z_MM_PER_STEP (1 / Z_STEPS_PER_MM)
#define THETA_DEG_PER_STEP (1 / THETA_STEPS_PER_DEG)


#define X_MAX_STEPS_PER_SECOND (1000.0 * X_MICROSTEPS)
#define Y_MAX_STEPS_PER_SECOND (500.0 * Y_MICROSTEPS)
#define Z_MAX_STEPS_PER_SECOND (1500.0 * Z_MICROSTEPS)
#define THETA_MAX_STEPS_PER_SECOND (100.0 * THETA_MICROSTEPS)


#define X_ACCELERATION (X_MAX_STEPS_PER_SECOND * X_MAX_STEPS_PER_SECOND / 100)
#define Y_ACCELERATION (Y_MAX_STEPS_PER_SECOND * Y_MAX_STEPS_PER_SECOND / 100)
#define Z_ACCELERATION (Z_MAX_STEPS_PER_SECOND * Z_MAX_STEPS_PER_SECOND / 100)
#define THETA_ACCELERATION      (THETA_MAX_STEPS_PER_SECOND * THETA_MAX_STEPS_PER_SECOND / 100)

#define X_MAX_MM_PER_SECOND (X_MAX_STEPS_PER_SECOND * X_MM_PER_STEP)
#define Y_MAX_MM_PER_SECOND (Y_MAX_STEPS_PER_SECOND * Y_MM_PER_STEP)
#define Z_MAX_MM_PER_SECOND (Z_MAX_STEPS_PER_SECOND * Z_MM_PER_STEP)
#define THETA_MAX_DEG_PER_SECOND (THETA_MAX_STEPS_PER_SECOND * THETA_DEG_PER_STEP)

#define X_ZEROING_STEPS_PER_SECOND (X_MAX_STEPS_PER_SECOND / 4)
#define Y_ZEROING_STEPS_PER_SECOND (Y_MAX_STEPS_PER_SECOND / 4)
#define Z_ZEROING_STEPS_PER_SECOND (Z_MAX_STEPS_PER_SECOND / 2)
#define THETA_ZEROING_STEPS_PER_SECOND (THETA_MAX_STEPS_PER_SECOND / 2)

#define MAX_X_MM 450.0
#define MAX_Y_MM 490.0
#define MAX_Z_MM 145.0
#define MAX_THETA_DEG 180.0

#define MAX_G 180.0

#define HEAD_PIN_1
#define HEAD_PIN_2
#define HEAD_PIN_3
#define HEAD_PIN_4


//#define LIMIT_SWITCH_INTERRUPTS
//
#define MASTER_LIMIT_SWITCH 8



#ifdef LIMIT_SWITCH_INTERRUPTS
#define X1_LIMIT_SWITCH_PIN 2
#define X2_LIMIT_SWITCH_PIN 3
#define Y_LIMIT_SWITCH_PIN 18
#define Z_LIMIT_SWITCH_PIN 19
#define THETA_LIMIT_SWITCH_PIN 20
#else
#define X1_LIMIT_SWITCH_PIN 45
#define X2_LIMIT_SWITCH_PIN 47
#define Y_LIMIT_SWITCH_PIN 49
#define Z_LIMIT_SWITCH_PIN 51
#define THETA_LIMIT_SWITCH_PIN 53
#endif

class Command;


struct GantryConfiguration {
    int head;
    Position position;

    AccelStepper
            x1_motor,
            x2_motor,
            y_motor,
            z_motor,
            theta_motor;

    Servo gripperServo; 

public:

    GantryConfiguration() = default;

    boolean initialize();

    void execute(Command &command);
    void execute(String &command);
    void execute(char *command);
    void execute(Command *commands, int numCommands);
    void execute(String *commands, int numCommands);
    void execute(char **commands, int numCommands);

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
    boolean masterLimitSwitchTriggered();


    boolean xMaxLimitReached();
    boolean yMaxLimitReached();
    boolean zMaxLimitReached();
    boolean thetaMaxLimitReached();

};


#endif //GAMTRY_CODE_GANTRY_H
