//
// Created by thevizwiz on 2/28/2024.
//

#include "CommandQueue.h"
#include "CommandParser.h"

const String CommandQueue::SERIAL_PING_MESSAGE = "pls send";


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

    if (commands.size() == MAX_BUFFERED_COMMANDS)
        return Command::NO_COMMAND;
    uint32_t startMillis = millis();


    Serial1.println(SERIAL_PING_MESSAGE);
    Serial.println(SERIAL_PING_MESSAGE);
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

    Serial.println("received message.");
    Command nextCommand;
    String inputString;

    String computerString = Serial.available() ? Serial.readString() : "";
    String espString = Serial1.available() ? Serial1.readString() : "";

    if (espString[0] == COMMAND_STARTING_CHAR_ESP) {
        Serial.println(String("received esp message: ") + espString);
        inputString = espString.substring(2);
    } else {
        inputString = computerString;
    }

    if (inputString == "")
        return Command::NO_COMMAND;


    Serial.print("message: ");
    Serial.println(inputString);
    nextCommand = CommandParser::parse(inputString);
    return nextCommand;
}

unsigned int CommandQueue::numAvailableCommands() const {
    return commands.size();
}