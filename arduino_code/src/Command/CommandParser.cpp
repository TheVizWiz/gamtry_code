//
// Created by thevizwiz on 2/28/2024.
//

#include "CommandParser.h"
#include "Vector.h"
#include "Logger/Logger.h"

static Logger logger = Logger("CommandParser");
static Logger inputLogger = Logger("Input");

Command CommandParser::parse(String input) {

    // send log messages through the serial interface.
    // This can help to separate messages into segments.
    if (input.length() >= 2) {
        String message = input.substring(2);
        message.trim();
        switch (input[0]) {
            case COMMAND_LOG:
                inputLogger.log(message);
                return Command::NO_COMMAND;
            case COMMAND_WARN:
                inputLogger.warn(message);
                return Command::NO_COMMAND;
            case COMMAND_ERR:
                inputLogger.err(message);
                return Command::NO_COMMAND;
            case COMMAND_READ:
                Command command = Command(CommandType::READ_COMMAND);
                command.fileName = message;
                return command;
        }
    }


//    Serial._println("inside of parse");

    Command command = Command(CommandType::BASE);
    String tokenArray[MAX_TOKENS_PER_COMMAND];
    Vector<String> tokens = Vector<String>(tokenArray);
    tokens.clear();
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



//    Serial._println("tokens:");
//    Serial._println(tokens.size());
//    for (const auto &item: tokens)
//        Serial._println(item);

    if (tokens.size() == 0) return Command::NO_COMMAND;


    for (const String token: tokens) {
        char commandChar = token[0];
        String amount = token.substring(1);

//        Serial._println(String("command char: ") + commandChar + " amount: " + amount);


        switch (commandChar) {

            //check for write command
            case COMMAND_WRITE:
                command.type = CommandType::WRITE_COMMAND;
                command.letters = String(amount);
                break;

                // check for special command
            case COMMAND_SPECIAL:
                command.type = CommandType::SPECIAL_COMMAND;
                command.code = amount.toInt();
                return command;

                // check for head change command
            case COMMAND_HEAD_CHANGE:
                command.type = CommandType::HEAD_CHANGE;
                command.head = amount.toInt();
                return command;

                // check for macro command
            case COMMAND_MACRO:
                command.type = CommandType::MACRO;
                command.code = amount.toInt();
                return command;

                //check for gripper command
            case COMMAND_GRIPPER:
                command.type = CommandType::GRIPPER_COMMAND;
                command.g = amount.toFloat();
                return command;

            case COMMAND_GLUE:
                command.type = CommandType::GLUE_COMMAND;
                command.glue_speed = amount.toFloat();
                break;

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
                break;

            case COMMAND_WRITE_BASE:
                command.base_size = amount.toFloat();
                break;

            case COMMAND_WRITE_SPACING:
                command.char_x_multiplier = amount.toFloat();
                break;


//            default:
//                Serial._println(String("Command char ") + commandChar + " does not match.");

        }
    }

    return command;
}

