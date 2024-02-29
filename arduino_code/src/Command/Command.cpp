//
// Created by thevizwiz on 2/16/2024.
//

#include "Command.h"
#include "CommandParser.h"


Command::Command(CommandType type) : Command() {
    this->type = type;
}

boolean Command::isNoCommand() {
    return type == CommandType::NONE;
}


void Command::execute(GantryConfiguration gantry) {

    switch (this->type) {

        case NONE:
            break;
        case BASE:
            executeBase(gantry);
            break;
        case HEAD_CHANGE:
            executeHeadChange(gantry);
            break;
        case SPECIAL:
            executeSpecial(gantry);
            break;
    }
}

void Command::executeBase(GantryConfiguration gantry) {


    float dx = x_changed ? (gantry.position.x - x) : 0; // mm
    float dy = y_changed ? (gantry.position.y - y) : 0; // mm
    float dz = z_changed ? (gantry.position.z - z) : 0; // mm
    float dtheta = theta_changed ? (gantry.position.theta - theta) : 0; // mm





    if (time_changed) {
        float min_x_time = abs(dx) / X_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
        float min_y_time = abs(dx) / Y_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
        float min_z_time = abs(dx) / Z_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
        float min_theta_time = abs(dx) / THETA_MAX_DEG_PER_SECOND; // deg / (deg / s) = deg * s / deg = s = seconds

        time = min(min_x_time, time);
        time = min(min_y_time, time);
        time = min(min_z_time, time);
        time = min(min_theta_time, time);
    }


    float x_speed_mm = dx / time; // mm per second
    float y_speed_mm = dy / time; // mm per second
    float z_speed_mm = dz / time; // mm per second
    float theta_speed_mm = dtheta / time; // deg per second


    float x_speed_steps = x_speed_mm * X_STEPS_PER_MM; // steps per second
    float y_speed_steps = y_speed_mm * Y_STEPS_PER_MM; // steps per second
    float z_speed_steps = z_speed_mm * Z_STEPS_PER_MM; // steps per second
    float theta_speed_steps = theta_speed_mm * THETA_STEPS_PER_DEG; // steps per second

    gantry.x1_motor.setSpeed(x_speed_steps);
    gantry.x2_motor.setSpeed(x_speed_steps);
    gantry.y_motor.setSpeed(y_speed_steps);
    gantry.z_motor.setSpeed(z_speed_steps);
    gantry.theta_motor.setSpeed(theta_speed_steps);



    unsigned long currentTime = millis();

    // old approach - probably not needed? unsure about this.

//    while (gantry.x1_motor.runSpeedToPosition() &&
//    gantry.x2_motor.runSpeedToPosition() &&
//    gantry.y_motor.runSpeedToPosition() &&
//    gantry.z_motor.runSpeedToPosition() &&
//    gantry.theta_motor.runSpeedToPosition())


    while ((millis() - currentTime) / 1000.0f <= time) {
        gantry.x1_motor.runSpeedToPosition();
        gantry.x2_motor.runSpeedToPosition();
        gantry.y_motor.runSpeedToPosition();
        gantry.z_motor.runSpeedToPosition();
        gantry.theta_motor.runSpeedToPosition();
    }

    gantry.updatePosition();










}

void Command::executeHeadChange(GantryConfiguration gantry) {

}

void Command::executeSpecial(GantryConfiguration gantry) {

}


