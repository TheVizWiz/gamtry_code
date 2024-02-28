//
// Created by thevizwiz on 2/28/2024.
//

#include "CommandParser.h"


Command CommandParser::parse(String input) {


    Command command = Command(CommandType::BASE);
    String tokenArray[MAX_TOKENS_PER_COMMAND];
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

    if (tokens.size() == 0) return Command::NO_COMMAND;


    for (const String token: tokens) {
        char commandChar = token[0];
        String amount = token.substring(1);


        switch (commandChar) {


            // check for special command
            case COMMAND_SPECIAL:
                command.type = CommandType::SPECIAL;
                command.special = amount.toInt();
                return command;

                // check for head change command
            case COMMAND_HEAD_CHANGE:
                command.type = CommandType::HEAD_CHANGE;
                command.head = amount.toInt();
                return command;



                // all other commands considered base commands
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

