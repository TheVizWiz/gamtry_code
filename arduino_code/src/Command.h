//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_COMMAND_H
#define GAMTRY_CODE_COMMAND_H


#include "Position.h"
#include "Arduino.h"
#include "Vector.h"
#include "Gantry.h"


#define MAX_BUFFERED_COMMANDS 10

struct Command {

public:

    static const Command NO_COMMAND;
    Command();
    Command(boolean isNoCommand);
    boolean isNoCommand;
    virtual void execute(GantryConfiguration gantry);
};



struct BaseCommand : Command, Position {
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


class CommandParser {


public:
    static Command parse(String input);


private:
    static const char COMMAND_X = 'X';
    static const char COMMAND_Y = 'Y';
    static const char COMMAND_Z = 'Z';
    static const char COMMAND_THETA = 'T';
    static const char COMMAND_TIME = 't';
    static const char COMMAND_HEAD_CHANGE = 'H';
    static const char COMMAND_HEAD_1 = 'h';
    static const char COMMAND_HEAD_2 = 'i';
    static const char COMMAND_HEAD_3 = 'j';

    static const int MAX_COMMANDS_PER_MESSAGE = 10;


};


class CommandQueue {
public:
    Vector<Command> commands = Vector<Command>(commandArray);


public:

    /**
     * Gets the next command in the queue of commands to do, and removes it from the queue.
     * @return The next command to execute.
     */
    Command popNextCommand();

    /**
     * Tries to queue a single command and polls the
     * @return
     */
    Command queueCommand();

    Command queueCommand(long tryMillis);

    unsigned int numAvailableCommands() const;


private:
    Command commandArray[MAX_BUFFERED_COMMANDS];

    Command askSerialForNextCommand();


};

#endif //GAMTRY_CODE_COMMAND_H
