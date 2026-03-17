#pragma once
#include "types.h"

int buildPath(
    Pos start,
    Pos finish,
    const Pos parent[MAX_N][MAX_M],
    Pos path[MAX_N * MAX_M]
);

void makePathMask(const Pos path[MAX_N * MAX_M], int pathSize, bool pathMask[MAX_N][MAX_M]);
Direction directionBetween(Pos a, Pos b);
int computePathCost(const Pos path[MAX_N * MAX_M], int pathSize, const int field[MAX_N][MAX_M]);

void animateRobotPath(
    int N, int M,
    const int field[MAX_N][MAX_M],
    Pos start,
    const Pos exits[MAX_EXITS],
    int exitCount,
    const Pos path[MAX_N * MAX_M],
    int pathSize,
    const bool visited[MAX_N][MAX_M],
    const bool pathMask[MAX_N][MAX_M],
    int moveDelayMs,
    const char* titlePrefix
);
