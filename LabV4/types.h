#pragma once

#include "config.h"

struct Pos {
    int x = 0;
    int y = 0;
};

inline bool operator==(const Pos& a, const Pos& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Pos& a, const Pos& b) {
    return !(a == b);
}

enum Direction {
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
};

struct AnimationSettings {
    bool visualizeSearch = true; // показывать процесс обхода
    bool animateRobot = true;    // анимировать движение робота по готовому пути
    int searchDelayMs = 15;      // задержка в мс при показе поиска
    int moveDelayMs = 80;        // задержка в мс при движении по пути
};

struct SearchStats {
    bool found = false;
    int visitedCount = 0;        // сколько клеток реально извлечено из очереди / PQ
    int pathLength = 0;          // длина пути в шагах
    int pathCost = 0;            // суммарная стоимость пути
    double elapsedMs = 0.0;      // время работы алгоритма

    Pos reachedExit{};           // к какому выходу пришли
    int reachedExitIndex = -1;   // индекс выхода в массиве exits[]
};

inline bool isInside(int N, int M, int x, int y) {
    return x >= 0 && x < M && y >= 0 && y < N;
}

inline bool isExit(const Pos exits[MAX_EXITS], int exitCount, Pos p) {
    for (int i = 0; i < exitCount; ++i) {
        if (exits[i] == p) return true;
    }
    return false;
}

inline int findExitIndex(const Pos exits[MAX_EXITS], int exitCount, Pos p) {
    for (int i = 0; i < exitCount; ++i) {
        if (exits[i] == p) return i;
    }
    return -1;
}
