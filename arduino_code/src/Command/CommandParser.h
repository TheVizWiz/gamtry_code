//
// Created by thevizwiz on 2/28/2024.
//

#ifndef ARDUINO_CODE_COMMANDPARSER_H
#define ARDUINO_CODE_COMMANDPARSER_H

#include "Command.h"

class CommandParser {
public:
    static Command parse(String input);


public:
    static const char COMMAND_SPECIAL_HOME = 0;
    static const char COMMAND_SPECIAL_HOME_X = 1;
    static const char COMMAND_SPECIAL_HOME_Y = 2;
    static const char COMMAND_SPECIAL_HOME_Z = 3;
    static const char COMMAND_SPECIAL_HOME_THETA = 4;

private: //static variables
    static const char COMMAND_X = 'X';
    static const char COMMAND_Y = 'Y';
    static const char COMMAND_Z = 'Z';
    static const char COMMAND_THETA = 'T';
    static const char COMMAND_TIME = 't';
    static const char COMMAND_HEAD_CHANGE = 'H';
    static const char COMMAND_HEAD_1 = 'h';
    static const char COMMAND_HEAD_2 = 'i';
    static const char COMMAND_HEAD_3 = 'j';

    static const char COMMAND_SPECIAL = 'S';
    static const char COMMAND_MACRO = 'M';

    static const char COMMAND_LOG = 'L';
    static const char COMMAND_WARN = 'W';
    static const char COMMAND_ERR = 'E';

    static const char COMMAND_G = 'G'; 



    static const int MAX_TOKENS_PER_COMMAND = 10;


};


#endif //ARDUINO_CODE_COMMANDPARSER_H
