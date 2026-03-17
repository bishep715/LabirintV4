#include "path_utils.h"
#include "console_ui.h"
#include "grid.h"
#include <cstring>
#include <cstdio>

int buildPath(
    Pos start,
    Pos finish,
    const Pos parent[MAX_N][MAX_M],
    Pos path[MAX_N * MAX_M]
) {
    int pathSize = 0;
    Pos cur = finish;
    while (true) {
        path[pathSize++] = cur;
        if (cur == start) break;
        cur = parent[cur.y][cur.x];
    }

    for (int i = 0; i < pathSize / 2; ++i) {
        Pos tmp = path[i];
        path[i] = path[pathSize - 1 - i];
        path[pathSize - 1 - i] = tmp;
    }
    return pathSize;
}

void makePathMask(const Pos path[MAX_N * MAX_M], int pathSize, bool pathMask[MAX_N][MAX_M]) {
    clearBoolMask(pathMask);
    for (int i = 0; i < pathSize; ++i) {
        pathMask[path[i].y][path[i].x] = true;
    }
}

Direction directionBetween(Pos a, Pos b) {
    if (b.x > a.x) return DIR_RIGHT;
    if (b.x < a.x) return DIR_LEFT;
    if (b.y > a.y) return DIR_DOWN;
    return DIR_UP;
}

int computePathCost(const Pos path[MAX_N * MAX_M], int pathSize, const int field[MAX_N][MAX_M]) {
    int cost = 0;
    for (int i = 1; i < pathSize; ++i) {
        cost += cellCost(field[path[i].y][path[i].x]);
    }
    return cost;
}

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
) {
    if (pathSize <= 0) return;

    char status[128];
    for (int i = 0; i < pathSize; ++i) {
        Direction dir = DIR_RIGHT;
        if (i + 1 < pathSize) dir = directionBetween(path[i], path[i + 1]);
        std::snprintf(status, sizeof(status), "%s: шаг %d из %d", titlePrefix, i, pathSize - 1);
        renderGrid(N, M, field, start, exits, exitCount, path[i], dir, visited, pathMask, status);
        sleepMs(moveDelayMs);
    }
}
