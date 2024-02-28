//
// Created by thevizwiz on 2/28/2024.
//

#ifndef ARDUINO_CODE_BASECOMMAND_H
#define ARDUINO_CODE_BASECOMMAND_H

#include "Arduino.h"
#include "Command.h"
#include "Position.h"

class BaseCommand : public Command, public Position {
public:
    unsigned long time = 0;
    boolean time_changed = false,
            x_changed = false,
            y_changed = false,
            z_changed = false,
            theta_changed = false,
            head_changed = false,
            head_1_changed = false,
            head_2_changed = false,
            head_3_changed = false;


public:

    BaseCommand();

    BaseCommand(boolean isNoCommand);

    String toString();

    void execute(GantryConfiguration gantry) override;


};

#endif //ARDUINO_CODE_BASECOMMAND_H
