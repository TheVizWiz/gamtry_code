//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_COMMAND_H
#define GAMTRY_CODE_COMMAND_H


#include "Position.h"
#include "Gantry.h"


enum CommandType {
    NONE, BASE, HEAD_CHANGE, SPECIAL_COMMAND, MACRO
};


struct Command : public Position {

private:
    constexpr static const float BUFFER_TIME = 0.3f;


public:

    static const Command NO_COMMAND;


    CommandType type;

    Command();

    Command(CommandType type);

    void execute(GantryConfiguration &gantry);

    boolean isNoCommand() const;

    String toString();

public:
    double time = 0;
    uint8_t code;
    boolean
            time_changed = false,
            x_changed = false,
            y_changed = false,
            z_changed = false,
            theta_changed = false,
            head_1_changed = false,
            head_2_changed = false,
            head_3_changed = false;

private:
    void executeBase(GantryConfiguration &gantry);
    void executeHeadChange(GantryConfiguration &gantry);
    void executeSpecial(GantryConfiguration &gantry);
    void executeHoming(GantryConfiguration &gantry);

};




#endif //GAMTRY_CODE_COMMAND_H
