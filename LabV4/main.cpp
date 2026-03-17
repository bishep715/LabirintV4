#include <iostream>
#include <limits>
#include <locale>
#include <string>
#include "config.h"
#include "types.h"
#include "io.h"
#include "manual.h"
#include "bfs.h"
#include "dijkstra.h"
#include "astar.h"
#include "stats.h"

static AnimationSettings askAnimationSettings() {
    AnimationSettings a;
    int mode = 2;
    std::cout << "Выберите скорость анимации:\n";
    std::cout << "1 - Медленно\n";
    std::cout << "2 - Нормально\n";
    std::cout << "3 - Быстро\n";
    std::cout << "4 - Только итоговый путь\n";
    std::cout << "5 - Без анимации\n";
    std::cout << "Ваш выбор: ";
    std::cin >> mode;

    switch (mode) {
    case 1: a.visualizeSearch = true;  a.animateRobot = true;  a.searchDelayMs = 60; a.moveDelayMs = 180; break;
    case 2: a.visualizeSearch = true;  a.animateRobot = true;  a.searchDelayMs = 20; a.moveDelayMs = 90;  break;
    case 3: a.visualizeSearch = true;  a.animateRobot = true;  a.searchDelayMs = 3;  a.moveDelayMs = 25;  break;
    case 4: a.visualizeSearch = false; a.animateRobot = true;  a.searchDelayMs = 0;  a.moveDelayMs = 70;  break;
    case 5: a.visualizeSearch = false; a.animateRobot = false; a.searchDelayMs = 0;  a.moveDelayMs = 0;   break;
    default: break;
    }
    return a;
}

static void showLoadedScenarioInfo(int N, int M, Pos start, const Pos exits[MAX_EXITS], int exitCount) {
    std::cout << "\nСценарий загружен успешно.\n";
    std::cout << "Размер: " << N << " x " << M << "\n";
    std::cout << "Старт: (" << start.x << ", " << start.y << ")\n";
    std::cout << "Выходов: " << exitCount << "\n";
    for (int i = 0; i < exitCount; ++i) {
        std::cout << "  Выход " << (i + 1) << ": (" << exits[i].x << ", " << exits[i].y << ")\n";
    }
    std::cout << '\n';
}
// Основная часть
// проверка git

// Тестирую репозиторий

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");
    setlocale(LC_ALL, "RU");

    int N = 0, M = 0;
    int field[MAX_N][MAX_M]{};
    Pos start{};
    Pos exits[MAX_EXITS]{};
    int exitCount = 0;
    std::string scenarioPath;

    while (true) {
        std::cout << "=== Проект: Робот в лабиринте ===\n";
        std::cout << "1 - Загрузить сценарий из файла\n";
        std::cout << "0 - Выход\n";
        std::cout << "Ваш выбор: ";

        int mainChoice = -1;
        std::cin >> mainChoice;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }

        if (mainChoice == 0) break;
        if (mainChoice != 1) continue;

        std::cout << "Введите путь к файлу сценария: ";
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        std::getline(std::cin, scenarioPath);

        if (!loadScenario(scenarioPath.c_str(), N, M, field, start, exits, exitCount)) {
            std::cout << "Ошибка: не удалось загрузить сценарий.\n\n";
            continue;
        }

        showLoadedScenarioInfo(N, M, start, exits, exitCount);

        bool backToMain = false;
        while (!backToMain) {
            std::cout << "Выберите режим:\n";
            std::cout << "1 - Ручной режим\n";
            std::cout << "2 - BFS\n";
            std::cout << "3 - Дейкстра\n";
            std::cout << "4 - A*\n";
            std::cout << "5 - Сравнение BFS / Дейкстра / A*\n";
            std::cout << "0 - Назад\n";
            std::cout << "Ваш выбор: ";

            int mode = -1;
            std::cin >> mode;
            if (!std::cin) {
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                continue;
            }

            if (mode == 0) {
                backToMain = true;
                std::cout << '\n';
                continue;
            }

            if (mode == 1) {
                runManualMode(N, M, field, start, exits, exitCount);
                continue;
            }

            AnimationSettings anim = askAnimationSettings();

            if (mode == 2) {
                SearchStats bfsStats = runAutoModeBFS(N, M, field, start, exits, exitCount, anim);
                showStatsScreen("BFS", bfsStats);
            }
            else if (mode == 3) {
                SearchStats dStats = runAutoModeDijkstra(N, M, field, start, exits, exitCount, anim);
                showStatsScreen("Дейкстра", dStats);
            }
            else if (mode == 4) {
                SearchStats aStats = runAutoModeAStar(N, M, field, start, exits, exitCount, anim);
                showStatsScreen("A*", aStats);
            }
            else if (mode == 5) {
                SearchStats bfsStats = runAutoModeBFS(N, M, field, start, exits, exitCount, anim);
                SearchStats dStats = runAutoModeDijkstra(N, M, field, start, exits, exitCount, anim);
                SearchStats aStats = runAutoModeAStar(N, M, field, start, exits, exitCount, anim);
                showCompareScreen(bfsStats, dStats, aStats);

                std::cout << "Сохранить отчёт в файл? (1 - да, 0 - нет): ";
                int save = 0;
                std::cin >> save;
                if (save == 1) {
                    std::cout << "Введите имя файла отчёта: ";
                    std::string reportPath;
                    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    std::getline(std::cin, reportPath);
                    if (saveCompareReport(reportPath.c_str(), scenarioPath.c_str(), N, M, bfsStats, dStats, aStats)) {
                        std::cout << "Отчёт сохранён.\n";
                    }
                    else {
                        std::cout << "Ошибка сохранения отчёта.\n";
                    }
                }
                system("pause");
            }
        }
    }

    return 0;
}
