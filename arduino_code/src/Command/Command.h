//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_COMMAND_H
#define GAMTRY_CODE_COMMAND_H


#include "Position.h"
#include "Arduino.h"
#include "Vector.h"
#include "Gantry.h"


enum CommandType {
    NONE, BASE, HEAD_CHANGE, SPECIAL
};


struct Command : public Position {


public:

    static const Command NO_COMMAND;

    CommandType type;

    Command();

    Command(CommandType type);

    void execute(GantryConfiguration gantry);

    boolean isNoCommand();


public:
    float time;
    unsigned char special;
    boolean
            time_changed,
            x_changed,
            y_changed,
            z_changed,
            theta_changed,
            head_1_changed,
            head_2_changed,
            head_3_changed;

private:
    void executeBase(GantryConfiguration gantry);
    void executeHeadChange(GantryConfiguration gantry);
    void executeSpecial(GantryConfiguration gantry);
};

const Command Command::NO_COMMAND = Command(CommandType::NONE);

#endif //GAMTRY_CODE_COMMAND_H
