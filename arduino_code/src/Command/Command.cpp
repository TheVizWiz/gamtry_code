//
// Created by thevizwiz on 2/16/2024.
//

#include "Command.h"
#include "CommandParser.h"
#include "Logger/Logger.h"


const Command Command::NO_COMMAND = Command(CommandType::NONE);

static Logger logger = Logger("Command");

Command::Command() {

}

Command::Command(CommandType type) : Command() {
    this->type = type;
}

void Command::execute(GantryConfiguration &gantry) {

    logger.log(String("Running new command. Command: ") + toString());

    switch (this->type) {
        case NONE:
            break;
        case BASE:
            executeBase(gantry);
            break;
        case HEAD_CHANGE:
            executeHeadChange(gantry);
            break;
        case SPECIAL_COMMAND:
            executeSpecial(gantry);
            break;
        case MACRO:
            break;
    }
}

void Command::executeBase(GantryConfiguration &gantry) {




//    Serial.print("Gantry x position: ");
//    Serial._println(gantry.position.x);
//    Serial.print("Wanted x position: ");
//    Serial._println(x);
    logger.log("Executing base command.");

    x = max(min(x, MAX_X_MM), 0);
    y = max(min(y, MAX_Y_MM), 0);
    z = max(min(z, MAX_Z_MM), 0);
    theta = max(min(theta, MAX_THETA_DEG), 0);


    double dx = x_changed ? (x - gantry.position.x) : 0; // mm
    double dy = y_changed ? (y - gantry.position.y) : 0; // mm
    double dz = z_changed ? (z - gantry.position.z) : 0; // mm
    double dtheta = theta_changed ? (theta - gantry.position.theta) : 0; // mm



    double min_x_time = abs(dx) / X_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
    double min_y_time = abs(dy) / Y_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
    double min_z_time = abs(dz) / Z_MAX_MM_PER_SECOND; // mm / (mm / s) = mm * s / mm = s = seconds
    double min_theta_time = abs(dtheta) / THETA_MAX_DEG_PER_SECOND; // deg / (deg / s) = deg * s / deg = s = seconds
//
//    Serial._println(min_x_time);
//    Serial._println(min_y_time);
//    Serial._println(min_z_time);
//    Serial._println(min_theta_time);

    time = max(min_x_time, time);
    time = max(min_y_time, time);
    time = max(min_z_time, time);
    time = max(min_theta_time, time);

    logger.log(String("time for total movement:") + time);


//    Serial.print("calculated time: ");
//    Serial._println(time);


    double x_speed_mm = dx / time; // mm per second
    double y_speed_mm = dy / time; // mm per second
    double z_speed_mm = dz / time; // mm per second
    double theta_speed_mm = dtheta / time; // deg per second


    double x_speed_steps = x_speed_mm * X_STEPS_PER_MM; // steps per second
    double y_speed_steps = y_speed_mm * Y_STEPS_PER_MM; // steps per second
    double z_speed_steps = z_speed_mm * Z_STEPS_PER_MM; // steps per second
    double theta_speed_steps = theta_speed_mm * THETA_STEPS_PER_DEG; // steps per second

//    Serial.print("Calculated speed: ");
//    Serial._println(x_speed_steps);

    if (x_changed) {
        gantry.x1_motor.moveTo(x * X_STEPS_PER_MM);
        gantry.x2_motor.moveTo(x * X_STEPS_PER_MM);
        gantry.x1_motor.setSpeed(x_speed_steps);
        gantry.x2_motor.setSpeed(x_speed_steps);
    }


    if (y_changed) {
        gantry.y_motor.moveTo(y * Y_STEPS_PER_MM);
        gantry.y_motor.setSpeed(y_speed_steps);
    }

    if (z_changed) {
        gantry.z_motor.moveTo(z * Z_STEPS_PER_MM);
        gantry.z_motor.setSpeed(z_speed_steps);
    }

    if (theta_changed) {
        gantry.theta_motor.moveTo(theta * THETA_STEPS_PER_DEG);
        gantry.theta_motor.setSpeed(theta_speed_steps);
    }


    long x_wanted = x * X_STEPS_PER_MM;
    long y_wanted = y * Y_STEPS_PER_MM;
    long z_wanted = z * Z_STEPS_PER_MM;
    long theta_wanted = theta * THETA_STEPS_PER_DEG;





//    Serial._println(String("Time: ") + time);
//    Serial._println(String("Speed: ") + x_speed_steps);



    unsigned long currentTime = millis();


    while (true) {


        boolean x_finished = !x_changed ||
                             (gantry.x1_motor.currentPosition() == x_wanted &&
                              gantry.x2_motor.currentPosition() == x_wanted);
        boolean y_finished = !y_changed ||
                             gantry.y_motor.currentPosition() == y_wanted;
        boolean z_finished = !z_changed ||
                             gantry.z_motor.currentPosition() == z_wanted;
        boolean theta_finished = !theta_changed ||
                                 gantry.theta_motor.currentPosition() == theta_wanted;


        if (x_finished && y_finished && z_finished && theta_finished)
            break;


        boolean x_outside_limits =
                (x_speed_steps < 0 && x < 0 && gantry.x1LimitSwitchTriggered()) ||
                (x_speed_steps < 0 && x < 0 && gantry.x2LimitSwitchTriggered()) ||
                (x_speed_steps > 0 && gantry.xMaxLimitReached());

        boolean y_outside_limits =
                (y_speed_steps < 0 && y < 0 && gantry.yLimitSwitchTriggered()) ||
                (y_speed_steps > 0 && gantry.yMaxLimitReached());

        boolean z_outside_limits =
                (z_speed_steps < 0 && z < 0 && gantry.zLimitSwitchTriggered()) ||
                (z_speed_steps > 0 && gantry.zMaxLimitReached());

        boolean theta_outside_limits =
                (theta_speed_steps < 0 && theta < 0 && gantry.thetaLimitSwitchTriggered()) ||
                (theta_speed_steps > 0 && gantry.thetaMaxLimitReached());

        if (x_outside_limits || y_outside_limits || z_outside_limits || theta_outside_limits) {
            logger.err("Limits exceeded. Exiting base movement loop early.");
            gantry.updatePosition();
            return;
        }


        if (x_changed)
            gantry.x1_motor.runSpeedToPosition();
        if (x_changed)
            gantry.x2_motor.runSpeedToPosition();
        if (y_changed)
            gantry.y_motor.run();
        if (z_changed)
            gantry.z_motor.runSpeedToPosition();
        if (theta_changed)
            gantry.theta_motor.runSpeedToPosition();


        gantry.updatePosition();
    }

    gantry.x1_motor.setSpeed(0);
    gantry.x2_motor.setSpeed(0);
    gantry.y_motor.setSpeed(0);
    gantry.z_motor.setSpeed(0);
    gantry.theta_motor.setSpeed(0);

    logger.log("Successfully executed base command.");

}

void Command::executeHeadChange(GantryConfiguration &gantry) {


}

void Command::executeSpecial(GantryConfiguration &gantry) {

    switch (this->code) {
        case CommandParser::COMMAND_SPECIAL_HOME:
            executeHoming(gantry);
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_X:
            gantry.homeXAxis();
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_Y:
            gantry.homeYAxis();
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_Z:
            gantry.homeZAxis();
            break;
        case CommandParser::COMMAND_SPECIAL_HOME_THETA:
            gantry.homeThetaAxis();
            break;
    }

}

void Command::executeHoming(GantryConfiguration &gantry) {
    gantry.homeXAxis();
    gantry.homeYAxis();
    gantry.homeZAxis();
//    gantry.homeThetaAxis();
}

boolean Command::isNoCommand() const {
    return this->type == CommandType::NONE;
}

String Command::toString() {

    String s;

    switch (type) {

        case NONE:
            return "{NO_COMMAND}";
            break;
        case BASE:
            s = "{";

            if (x_changed) s = s + "X: " + x + " ";
            if (y_changed) s = s + "Y: " + y + " ";
            if (z_changed) s = s + "Z: " + z + " ";
            if (time_changed) s = s + "time: " + time + " ";
            if (theta_changed) s = s + "theta: " + theta + " ";
            if (head_1_changed) s = s + "head_1: " + head_1 + " ";
            s = s.substring(0, s.length() - 1) + "}";
            return s;
            break;
        case HEAD_CHANGE:
            return "{HEAD_CHANGE}";
            break;
        case SPECIAL_COMMAND:
            return String("{SPECIAL: ") + code + "}";
            break;
        case MACRO:
            break;
    }

    return "Command";

}


