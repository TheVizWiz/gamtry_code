//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_GANTRY_H
#define GAMTRY_CODE_GANTRY_H

#include "Position.h"


typedef enum {
    NO_HEAD, GRIPPER, GLUE, STAMP
} Head;

typedef struct {
    Head head;
    Position position;
} GantryConfiguration ;




#endif //GAMTRY_CODE_GANTRY_H
