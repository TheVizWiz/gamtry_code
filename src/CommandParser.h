//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_COMMANDPARSER_H
#define GAMTRY_CODE_COMMANDPARSER_H


#include "Position.h"
#include "Arduino.h"


class CommandParser {


public:
    CommandParser();
    Position parse(String input);



};


#endif //GAMTRY_CODE_COMMANDPARSER_H
