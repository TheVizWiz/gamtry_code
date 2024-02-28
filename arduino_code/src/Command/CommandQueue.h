//
// Created by thevizwiz on 2/28/2024.
//

#ifndef ARDUINO_CODE_COMMANDQUEUE_H
#define ARDUINO_CODE_COMMANDQUEUE_H

#include "Command.h"


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


    static const int MAX_BUFFERED_COMMANDS = 10;
    static const int SERIAL_PING_TRY_TIME = 100;
    static const String SERIAL_PING_MESSAGE;
    Command commandArray[MAX_BUFFERED_COMMANDS];

private:

    Command askSerialForNextCommand();


};

const String CommandQueue::SERIAL_PING_MESSAGE = "pls send";


#endif //ARDUINO_CODE_COMMANDQUEUE_H
