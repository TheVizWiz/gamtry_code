//
// Created by thevizwiz on 2/16/2024.
//

#include "Command.h"
#include "CommandParser.h"




const Command Command::NO_COMMAND = Command(true);

Command::Command(boolean isNoCommand) : Command() {
    this->isNoCommand = isNoCommand;
}


