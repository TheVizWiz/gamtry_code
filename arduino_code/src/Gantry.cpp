//
// Created by thevizwiz on 2/29/2024.
//



#include "Gantry.h"

GantryConfiguration::GantryConfiguration() {

    head = Head();


    x1_motor = AccelStepper(AccelStepper::DRIVER, X1_MOTOR_PINS);
    x2_motor = AccelStepper(AccelStepper::DRIVER, X2_MOTOR_PINS);
    y_motor = AccelStepper(AccelStepper::DRIVER, Y_MOTOR_PINS);
    z_motor = AccelStepper(AccelStepper::DRIVER, Z_MOTOR_PINS);
    theta_motor = AccelStepper(AccelStepper::DRIVER, THETA_MOTOR_PINS);

    x1_motor.setMaxSpeed(X_MAX_STEPS_PER_SECOND);
    x2_motor.setMaxSpeed(X_MAX_STEPS_PER_SECOND);
    y_motor.setMaxSpeed(Y_MAX_STEPS_PER_SECOND);
    z_motor.setMaxSpeed(Z_MAX_STEPS_PER_SECOND);
    theta_motor.setMaxSpeed(THETA_MAX_STEPS_PER_SECOND);
}

void GantryConfiguration::updatePosition() {
    position.x = (x1_motor.currentPosition() + x2_motor.currentPosition()) / 2 * X_MM_PER_STEP;
    position.y = y_motor.currentPosition() * Y_MM_PER_STEP;
    position.z = z_motor.currentPosition() * Z_MM_PER_STEP;
    position.theta = theta_motor.currentPosition() * THETA_DEG_PER_STEP;

}
