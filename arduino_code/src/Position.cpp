//
// Created by thevizwiz on 2/17/2024.
//

#include "Position.h"


String Position::toString() const {
    String s = "{";

    s = s + "X: " + x + " ";
    s = s + "Y: " + y + " ";
    s = s + "Z: " + z + " ";
    s = s + "theta: " + theta + "}";
    return s;
}
