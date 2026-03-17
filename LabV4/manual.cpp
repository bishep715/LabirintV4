#include "manual.h"
#include "console_ui.h"
#include "grid.h"
#include <conio.h>

void runManualMode(
    int N, int M,
    const int field[MAX_N][MAX_M],
    Pos start,
    const Pos exits[MAX_EXITS],
    int exitCount
) {
    bool visited[MAX_N][MAX_M];
    bool pathMask[MAX_N][MAX_M];
    clearBoolMask(visited);
    clearBoolMask(pathMask);

    Pos robot = start;
    Direction dir = DIR_RIGHT;
    const char* status = "Управление: W A S D, выход: Q";

    while (true) {
        visited[robot.y][robot.x] = true;
        renderGrid(N, M, field, start, exits, exitCount, robot, dir, visited, pathMask, status);

        if (isExit(exits, exitCount, robot)) {
            renderGrid(N, M, field, start, exits, exitCount, robot, dir, visited, pathMask, "Выход достигнут!");
            system("pause");
            return;
        }

        int ch = _getch();
        if (ch == 'q' || ch == 'Q') return;

        Pos next = robot;
        if (ch == 'w' || ch == 'W') { next.y--; dir = DIR_UP; }
        if (ch == 'd' || ch == 'D') { next.x++; dir = DIR_RIGHT; }
        if (ch == 's' || ch == 'S') { next.y++; dir = DIR_DOWN; }
        if (ch == 'a' || ch == 'A') { next.x--; dir = DIR_LEFT; }

        if (canMoveTo(N, M, field, next.x, next.y)) {
            robot = next;
            status = "Ход выполнен.";
        }
        else {
            status = "Нельзя пройти: стена или граница поля.";
        }
    }
}
