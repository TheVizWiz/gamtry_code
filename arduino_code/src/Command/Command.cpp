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


