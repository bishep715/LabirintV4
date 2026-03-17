#include "grid.h"

void clearBoolMask(bool mask[MAX_N][MAX_M]) {
    for (int y = 0; y < MAX_N; ++y)
        for (int x = 0; x < MAX_M; ++x)
            mask[y][x] = false;
}

void clearGrid(int field[MAX_N][MAX_M], int N, int M, int value) {
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < M; ++x)
            field[y][x] = value;
}

bool canMoveTo(int N, int M, const int field[MAX_N][MAX_M], int x, int y) {
    return isInside(N, M, x, y) && isPassableCell(field[y][x]);
}
