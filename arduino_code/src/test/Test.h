//
// Created by thevizwiz on 3/13/2024.
//

#ifndef ARDUINO_CODE_TEST_H
#define ARDUINO_CODE_TEST_H

#include "Gantry.h"
#include "Command/Command.h"
#include "Command/CommandParser.h"
#include "Command/CommandQueue.h"

class Test {

public:
    static void testLimitSwitches(GantryConfiguration &gantry);
    static void testXAxis(GantryConfiguration &gantry);
    static void testYAxis(GantryConfiguration &gantry);
    static void testZAxis(GantryConfiguration &gantry);
    static void testThetaAxis(GantryConfiguration &gantry);

    static void testZeroingXAxis(GantryConfiguration &gantry);
    static void testZeroingYAxis(GantryConfiguration &gantry);
    static void testZeroingZAxis(GantryConfiguration &gantry);
    static void testZeroingThetaAxis(GantryConfiguration &gantry);

};


#endif //ARDUINO_CODE_TEST_H
