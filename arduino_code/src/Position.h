//
// Created by thevizwiz on 2/16/2024.
//

#ifndef GAMTRY_CODE_POSITION_H
#define GAMTRY_CODE_POSITION_H


#include <WString.h>

struct Position {
public:
    double x = 0;
    double y = 0;
    double z = 0;
    double theta = 0;
    double head_1 = 0;
    double head_2 = 0;
    double head_3 = 0;
    int head = 0;
    double g = 0;

    String toString() const;
};


#endif //GAMTRY_CODE_POSITION_H
