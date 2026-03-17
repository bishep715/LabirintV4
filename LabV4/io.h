#pragma once
#include "types.h"

bool loadScenario(
    const char* filename,
    int& N, int& M,
    int field[MAX_N][MAX_M],
    Pos& start,
    Pos exits[MAX_EXITS],
    int& exitCount
);
