#pragma once

#include "types.h"

void clearScreen();
void sleepMs(int ms);
void printTitle(const char* title);

// Отрисовка поля со служебными слоями.
void renderGrid(
    int N, int M,
    const int field[MAX_N][MAX_M],
    Pos start,
    const Pos exits[MAX_EXITS],
    int exitCount,
    Pos robot,
    Direction dir,
    const bool visited[MAX_N][MAX_M],
    const bool pathMask[MAX_N][MAX_M],
    const char* statusLine
);
