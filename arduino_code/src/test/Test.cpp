//
// Created by thevizwiz on 3/13/2024.
//

#include "Test.h"


void Test::testLimitSwitches(GantryConfiguration &gantry) {

    String s = "";

    s += "x1: ";
    s += gantry.x1LimitSwitchTriggered() ? 1 : 0;

    s += " x2: ";
    s += gantry.x2LimitSwitchTriggered() ? 1 : 0;

    s += " y: ";
    s += gantry.yLimitSwitchTriggered() ? 1 : 0;

    s += " z: ";
    s += gantry.zLimitSwitchTriggered() ? 1 : 0;

    s += " t: ";
    s += gantry.thetaLimitSwitchTriggered() ? 1 : 0;


    Serial.println(s);
}


/**
 * DO NOT CALL WITHOUT ZEROING GANTRY.
 * Tests the X axis by moving from 0 all the way to 400 in steps of 50 mm and waits for 5 seconds between each movement.
 * @param gantry
 */
void Test::testXAxis(GantryConfiguration &gantry) {

}


void Test::testYAxis(GantryConfiguration &gantry) {

}

void Test::testZAxis(GantryConfiguration &gantry) {

}

void Test::testThetaAxis(GantryConfiguration &gantry) {

}

void Test::testZeroingXAxis(GantryConfiguration &gantry) {

}

void Test::testZeroingYAxis(GantryConfiguration &gantry) {

}

void Test::testZeroingZAxis(GantryConfiguration &gantry) {

}

void Test::testZeroingThetaAxis(GantryConfiguration &gantry) {

}

