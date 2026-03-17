#pragma once
#include "types.h"

void clearBoolMask(bool mask[MAX_N][MAX_M]);
void clearGrid(int field[MAX_N][MAX_M], int N, int M, int value = CELL_FREE);
bool canMoveTo(int N, int M, const int field[MAX_N][MAX_M], int x, int y);
