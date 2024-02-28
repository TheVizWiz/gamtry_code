//
// Created by thevizwiz on 2/28/2024.
//

#include "CommandQueue.h"
#include "CommandParser.h"



Command CommandQueue::popNextCommand() {

    // no command exists, so tell it not to do anything
    if (commands.empty()) {
        return Command::NO_COMMAND;
    }


    Command command = commands.front();

    commands.remove(0);
    return command;
}

Command CommandQueue::queueCommand() {

    return queueCommand(SERIAL_PING_TRY_TIME);

}

Command CommandQueue::queueCommand(long tryMillis) {
    uint32_t startMillis = millis();


    Serial.println(SERIAL_PING_MESSAGE);
    while (millis() - startMillis <= tryMillis) {
        Command command = askSerialForNextCommand();
        if (command.isNoCommand())
            continue;
        commands.push_back(command);
        return command;
    }

    commands.push_back(Command::NO_COMMAND);
    return Command::NO_COMMAND;
}

Command CommandQueue::askSerialForNextCommand() {

    if (!Serial.available())
        return Command::NO_COMMAND;


    String inputString = Serial.readString();

    Command nextCommand = CommandParser::parse(inputString);
    return nextCommand;
}

unsigned int CommandQueue::numAvailableCommands() const {
    return commands.size();
}