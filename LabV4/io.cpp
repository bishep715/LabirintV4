#include "io.h"
#include "grid.h"
#include <fstream>
#include <sstream>
#include <string>

bool loadScenario(
    const char* filename,
    int& N, int& M,
    int field[MAX_N][MAX_M],
    Pos& start,
    Pos exits[MAX_EXITS],
    int& exitCount
) {
    std::ifstream fin(filename);
    if (!fin) return false;

    if (!(fin >> N >> M)) return false;
    if (N <= 0 || N > MAX_N || M <= 0 || M > MAX_M) return false;

    if (!(fin >> start.x >> start.y)) return false;
    if (!isInside(N, M, start.x, start.y)) return false;

    if (!(fin >> exitCount)) return false;
    if (exitCount <= 0 || exitCount > MAX_EXITS) return false;

    for (int i = 0; i < exitCount; ++i) {
        if (!(fin >> exits[i].x >> exits[i].y)) return false;
        if (!isInside(N, M, exits[i].x, exits[i].y)) return false;
    }

    std::string marker;
    fin >> marker;
    if (marker != "GRID") return false;

    clearGrid(field, N, M, CELL_FREE);

    // Формат Stage 4: каждая строка сетки содержит M цифр без пробелов.
    // 0 - свободно, 1 - стена, 2 - песок, 3 - вода.
    for (int y = 0; y < N; ++y) {
        std::string row;
        fin >> row;
        if ((int)row.size() != M) return false;
        for (int x = 0; x < M; ++x) {
            char c = row[x];
            if (c < '0' || c > '3') return false;
            field[y][x] = c - '0';
        }
    }

    // Старт и выходы не должны стоять в стенах.
    if (!isPassableCell(field[start.y][start.x])) return false;
    for (int i = 0; i < exitCount; ++i) {
        if (!isPassableCell(field[exits[i].y][exits[i].x])) return false;
    }

    return true;
}
