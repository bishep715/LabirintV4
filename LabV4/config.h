#pragma once

// Общие настройки проекта.
// NOMINMAX нужен, чтобы windows.h не ломал std::min/std::max и numeric_limits::max().
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

// Максимально допустимые размеры поля.
const int MAX_N = 64;
const int MAX_M = 64;

// Максимальное количество выходов.
const int MAX_EXITS = 16;

// Значения клеток в поле.
enum CellType {
    CELL_FREE = 0,
    CELL_WALL = 1,
    CELL_SAND = 2,
    CELL_WATER = 3
};

inline int cellCost(int cell) {
    switch (cell) {
    case CELL_FREE:  return 1;
    case CELL_SAND:  return 3;
    case CELL_WATER: return 5;
    default:         return 1000000000; // для стены путь невозможен
    }
}

inline bool isPassableCell(int cell) {
    return cell != CELL_WALL;
}
