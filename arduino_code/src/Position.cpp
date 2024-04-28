//
// Created by thevizwiz on 2/17/2024.
//

#include "Position.h"


char * Position::toString() const {
    String s = "{";

    s = s + "X: " + x + " ";
    s = s + "Y: " + y + " ";
    s = s + "Z: " + z + " ";
    s = s + "theta: " + theta + "}";
    return const_cast<char *>(s.c_str());
}
