//
// Created by thevizwiz on 2/28/2024.
//

#include "CommandQueue.h"
#include "CommandParser.h"
#include "Logger/Logger.h"


const String CommandQueue::SERIAL_PING_MESSAGE = "pls send";

static Logger logger = Logger("CommandQueue");


Command CommandQueue::popNextCommand() {

    logger.log("Getting next command in list...");

    // no command exists, so tell it not to do anything
    if (commands.empty()) {
        logger.log("No command in list. Returning No Command.");
        return Command::NO_COMMAND;
    }


    Command command = commands.front();

//    Serial.println(#commands);

    logger.log(String("Command found. Command: ") + commands[0].toString());
    commands.remove(0);
    return command;
}

Command CommandQueue::queueCommand() {
    return queueCommand(SERIAL_PING_TRY_TIME);

}

Command CommandQueue::queueCommand(long tryMillis) {

    if (commands.size() == MAX_BUFFERED_COMMANDS)
        return Command::NO_COMMAND;
    uint32_t startMillis = millis();


    Serial1.println(SERIAL_PING_MESSAGE);
//    Serial.println(SERIAL_PING_MESSAGE);
    while (millis() - startMillis <= tryMillis) {
        Command command = askSerialForNextCommand();
        if (command.isNoCommand())
            continue;
        commands.push_back(command);
        return command;
    }

    return Command::NO_COMMAND;
}

Command CommandQueue::askSerialForNextCommand() {

    if (!Serial1.available() && !Serial.available())
        return Command::NO_COMMAND;

    logger.log("Message received through serial.");
    Command nextCommand;
    String inputString;

    String computerString = Serial.available() ? Serial.readString() : "";
    String espString = Serial1.available() ? Serial1.readString() : "";

    if (espString[0] == COMMAND_STARTING_CHAR_ESP) {
        inputString = espString.substring(2);
        logger.log("ESP Command received. Message: " + inputString);
    } else {
        inputString = computerString;
        logger.log("Computer/Serial Command received. Message: " + inputString);
    }

    if (inputString == "") {
        logger.warn("Empty input string, assuming faulty input and returning no command.");
        return Command::NO_COMMAND;
    }


    nextCommand = CommandParser::parse(inputString);
    return nextCommand;
}

unsigned int CommandQueue::numAvailableCommands() const {
    return commands.size();
}