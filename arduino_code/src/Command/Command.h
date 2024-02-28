//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_COMMAND_H
#define GAMTRY_CODE_COMMAND_H


#include "Position.h"
#include "Arduino.h"
#include "Vector.h"
#include "Gantry.h"
#include "BaseCommand.h"



struct Command {


public:
    static const Command NO_COMMAND;

    Command();

    Command(boolean isNoCommand);

    boolean isNoCommand;

    virtual void execute(GantryConfiguration gantry);
};





#endif //GAMTRY_CODE_COMMAND_H
