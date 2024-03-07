//
// Created by thevizwiz on 2/28/2024.
//

#ifndef ARDUINO_CODE_COMMANDQUEUE_H
#define ARDUINO_CODE_COMMANDQUEUE_H

#include "Command.h"
#include "Vector.h"

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
     * @return The command if queued.
     */
    Command queueCommand();

    Command queueCommand(long tryMillis);

    unsigned int numAvailableCommands() const;


private:


    static const int MAX_BUFFERED_COMMANDS = 10;
    static const int SERIAL_PING_TRY_TIME = 1000;
    static const String SERIAL_PING_MESSAGE;
    Command commandArray[MAX_BUFFERED_COMMANDS];

private:

    Command askSerialForNextCommand();


};



#endif //ARDUINO_CODE_COMMANDQUEUE_H
