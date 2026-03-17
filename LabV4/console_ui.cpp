#include "console_ui.h"
#include <iostream>

namespace {
    const char* RESET = "\x1b[0m";
    const char* FG_WHITE = "\x1b[37m";
    const char* FG_BLACK = "\x1b[30m";
    const char* FG_GREEN = "\x1b[32m";
    const char* FG_RED = "\x1b[31m";
    const char* FG_YELLOW = "\x1b[33m";
    const char* FG_BLUE = "\x1b[34m";
    const char* FG_CYAN = "\x1b[36m";
    const char* BG_WHITE = "\x1b[47m";
    const char* BG_GREEN = "\x1b[42m";
    const char* BG_RED = "\x1b[41m";
    const char* BG_YELLOW = "\x1b[43m";
    const char* BG_BLUE = "\x1b[44m";
    const char* BG_CYAN = "\x1b[46m";
}

void clearScreen() {
    system("cls");
}

void sleepMs(int ms) {
    Sleep(ms);
}

void printTitle(const char* title) {
    std::cout << "=== " << title << " ===\n\n";
}

static char robotChar(Direction dir) {
    switch (dir) {
    case DIR_UP: return '^';
    case DIR_RIGHT: return '>';
    case DIR_DOWN: return 'v';
    case DIR_LEFT: return '<';
    default: return 'R';
    }
}

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
) {
    clearScreen();
    printTitle("Робот в лабиринте");

    std::cout << "Условные обозначения: "
        << BG_GREEN << FG_BLACK << " S " << RESET << " старт, "
        << BG_RED << FG_WHITE << " E " << RESET << " выход, "
        << BG_WHITE << FG_BLACK << " # " << RESET << " стена, "
        << BG_YELLOW << FG_BLACK << " . " << RESET << " песок, "
        << BG_BLUE << FG_WHITE << " ~ " << RESET << " вода, "
        << BG_CYAN << FG_BLACK << " * " << RESET << " путь, "
        << FG_CYAN << "+" << RESET << " посещённые\n\n";

    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            Pos p{ x, y };

            if (robot == p) {
                std::cout << BG_CYAN << FG_BLACK << ' ' << robotChar(dir) << ' ' << RESET;
                continue;
            }
            if (start == p) {
                std::cout << BG_GREEN << FG_BLACK << " S " << RESET;
                continue;
            }
            if (isExit(exits, exitCount, p)) {
                std::cout << BG_RED << FG_WHITE << " E " << RESET;
                continue;
            }
            if (field[y][x] == CELL_WALL) {
                std::cout << BG_WHITE << FG_BLACK << " # " << RESET;
                continue;
            }
            if (pathMask[y][x]) {
                std::cout << BG_CYAN << FG_BLACK << " * " << RESET;
                continue;
            }
            if (field[y][x] == CELL_SAND) {
                std::cout << BG_YELLOW << FG_BLACK << " . " << RESET;
                continue;
            }
            if (field[y][x] == CELL_WATER) {
                std::cout << BG_BLUE << FG_WHITE << " ~ " << RESET;
                continue;
            }
            if (visited[y][x]) {
                std::cout << FG_CYAN << " + " << RESET;
                continue;
            }
            std::cout << "   ";
        }
        std::cout << '\n';
    }

    std::cout << "\n" << statusLine << "\n";
}
