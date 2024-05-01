//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_COMMAND_H
#define GAMTRY_CODE_COMMAND_H


#include "Position.h"
#include "Gantry.h"

struct GantryConfiguration;

enum CommandType {
    NONE, BASE, HEAD_CHANGE, SPECIAL_COMMAND, MACRO, GRIPPER_COMMAND, WRITE_COMMAND, GLUE_COMMAND, READ_COMMAND
};


struct Command : public Position {

private:
    constexpr static const float BUFFER_TIME = 0.3f;


public:

    static const Command NO_COMMAND;


    CommandType type;

    Command();

    Command(CommandType type);

    void execute(GantryConfiguration &gantry);

    boolean isNoCommand() const;

    String toString();

public:
    double time = 0;
    uint8_t code;
    boolean
            time_changed = false,
            x_changed = false,
            y_changed = false,
            z_changed = false,
            theta_changed = false,
            head_1_changed = false,
            head_2_changed = false,
            head_3_changed = false;
    String letters;
    String fileName;
    float base_size = 20.0;
    float char_x_multiplier = 1.2;//
    float glue_mm = 0.0f;


private:
    void executeBase(GantryConfiguration &gantry);
    void executeHeadChange(GantryConfiguration &gantry);
    void executeSpecial(GantryConfiguration &gantry);
    void executeHoming(GantryConfiguration &gantry);
    void executeGripper(GantryConfiguration &gantry);
    void executeWrite(GantryConfiguration &gantry);
    void drawLetter(GantryConfiguration &gantry, char letter, float x_start, float y_start, float width, float height,
                    float z_start, float z_jump);

    void executeGlue(GantryConfiguration &gantry);
    void executeRead(GantryConfiguration &gantry);
};




#endif //GAMTRY_CODE_COMMAND_H
