//
// Created by thevizwiz on 2/28/2024.
//

#include "BaseCommand.h"



BaseCommand::BaseCommand() {
    this->isNoCommand = false;
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->theta = 0;
}

void BaseCommand::execute(GantryConfiguration gantry) {
    if (theta_changed) {

    }
}

String BaseCommand::toString() {

    if (isNoCommand)
        return "not a command";


    String string = String("{");

    if (time_changed)
        string = string + "time: " + time + ", ";
    if (x_changed)
        string = string + "x: " + x + ", ";
    if (y_changed)
        string = string + "y: " + y + ", ";
    if (z_changed)
        string = string + "z: " + z + ", ";
    if (theta_changed)
        string = string + "theta: " + theta + ", ";
    if (head_changed)
        string = string + "head: " + head + ", ";
    if (head_1_changed)
        string = string + "head_1: " + head_1 + ", ";
    if (head_2_changed)
        string = string + "head_2: " + head_2 + ", ";
    if (head_3_changed)
        string = string + "head_3: " + head_3 + ", ";

    if (string.length() > 2)
        string = string.substring(0, string.length() - 2);

    string = string + "}";


    return string;
}
