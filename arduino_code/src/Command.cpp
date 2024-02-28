//
// Created by thevizwiz on 2/16/2024.
//

#include "Command.h"


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

    Command command = askSerialForNextCommand();

    if (command.isNoCommand)
        return command;

    commands.push_back(command);

    return command;

}

Command CommandQueue::queueCommand(long tryMillis) {
    uint32_t startMillis = millis();

    while (millis() - startMillis <= tryMillis) {
        Command command = askSerialForNextCommand();
        if (command.isNoCommand)
            continue;
        commands.push_back(command);
        return command;
    }

    commands.push_back(Command::NO_COMMAND);
    return Command::NO_COMMAND;
}

Command CommandQueue::askSerialForNextCommand() {
    Serial.println("pls send");
    delay(10);

    if (!Serial.available())
        return Command::NO_COMMAND;


    String inputString = Serial.readString();

    Command nextCommand = CommandParser::parse(inputString);
    return nextCommand;
}

unsigned int CommandQueue::numAvailableCommands() const {
    return commands.size();
}


Command CommandParser::parse(String input) {


    BaseCommand command = BaseCommand();
    String tokenArray[MAX_COMMANDS_PER_MESSAGE];
    Vector<String> tokens = Vector<String>(tokenArray);
    input.trim();

    int currentLocation = 0;

    while (currentLocation < input.length()) {
        if (input[currentLocation] == ' ') {
            tokens.push_back(input.substring(0, currentLocation));
            input = input.substring(currentLocation + 1);
            currentLocation = 0;

        } else {
            currentLocation++;
        }
    }

    tokens.push_back(input);
    for (const auto &item: tokens)


        for (auto token: tokens) {
            char commandChar = token[0];
            String amount = token.substring(1);


            switch (commandChar) {
                case COMMAND_TIME:
                    command.time = amount.toInt();
                    command.time_changed = true;
                    break;
                case COMMAND_X:
                    command.x = amount.toFloat();
                    command.x_changed = true;
                    break;
                case COMMAND_Y:
                    command.y = amount.toFloat();
                    command.y_changed = true;
                    break;
                case COMMAND_Z:
                    command.z = amount.toFloat();
                    command.z_changed = true;
                    break;
                case COMMAND_THETA:
                    command.theta = amount.toFloat();
                    command.theta_changed = true;
                    break;
                case COMMAND_HEAD_CHANGE:
                    command.head = amount.toInt();
                    command.head_changed = true;
                    break;
                case COMMAND_HEAD_1:
                    command.head_1 = amount.toFloat();
                    command.head_1_changed = true;
                    break;
                case COMMAND_HEAD_2:
                    command.head_2 = amount.toFloat();
                    command.head_2_changed = true;
                    break;
                case COMMAND_HEAD_3:
                    command.head_3 = amount.toFloat();
                    command.head_3_changed = true;

            }
        }

    return command;
}


const Command Command::NO_COMMAND = Command(true);


BaseCommand::BaseCommand() {
    this->isNoCommand = false;
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->theta = 0;
}

Command::Command(boolean isNoCommand) : Command() {
    this->isNoCommand = isNoCommand;
}

String BaseCommand::toString() {

    if (isNoCommand)
        return "not a command";


    String string = String("{");

    if (time_changed)
        string = string + "time: " + time + ", ";
    if (x_changed)
        string = string + "x: " + x + ", ";
    if (y_changed)
        string = string + "y: " + y + ", ";
    if (z_changed)
        string = string + "z: " + z + ", ";
    if (theta_changed)
        string = string + "theta: " + theta + ", ";
    if (head_changed)
        string = string + "head: " + head + ", ";
    if (head_1_changed)
        string = string + "head_1: " + head_1 + ", ";
    if (head_2_changed)
        string = string + "head_2: " + head_2 + ", ";
    if (head_3_changed)
        string = string + "head_3: " + head_3 + ", ";

    if (string.length() > 2)
        string = string.substring(0, string.length() - 2);

    string = string + "}";


    return string;
}

void BaseCommand::execute(GantryConfiguration gantry) {
    if (theta_changed)
}


