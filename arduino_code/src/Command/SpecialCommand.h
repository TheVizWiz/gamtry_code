//
// Created by thevizwiz on 2/28/2024.
//

#ifndef ARDUINO_CODE_SPECIALCOMMAND_H
#define ARDUINO_CODE_SPECIALCOMMAND_H

#include "Command.h"

struct SpecialCommand : public Command {
private:
    int special = 0;

public:
    SpecialCommand(int special);

    void execute(GantryConfiguration gantry) override;

};


#endif //ARDUINO_CODE_SPECIALCOMMAND_H
