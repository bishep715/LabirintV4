#include "bfs.h"
#include "grid.h"
#include "console_ui.h"
#include "path_utils.h"
#include <queue>
#include <chrono>
#include <cstdio>

SearchStats runAutoModeBFS(
    int N, int M,
    const int field[MAX_N][MAX_M],
    Pos start,
    const Pos exits[MAX_EXITS],
    int exitCount,
    const AnimationSettings& anim
) {
    SearchStats stats;
    bool visited[MAX_N][MAX_M];
    bool pathMask[MAX_N][MAX_M];
    Pos parent[MAX_N][MAX_M];
    clearBoolMask(visited);
    clearBoolMask(pathMask);

    std::queue<Pos> q;
    q.push(start);
    visited[start.y][start.x] = true;
    parent[start.y][start.x] = start;

    Pos foundExit{};

    auto t1 = std::chrono::high_resolution_clock::now();

    const int dx[4] = { 1, -1, 0, 0 };
    const int dy[4] = { 0, 0, 1, -1 };

    while (!q.empty()) {
        Pos cur = q.front();
        q.pop();
        ++stats.visitedCount;

        if (isExit(exits, exitCount, cur)) {
            stats.found = true;
            foundExit = cur;
            stats.reachedExit = cur;
            stats.reachedExitIndex = findExitIndex(exits, exitCount, cur);
            break;
        }

        if (anim.visualizeSearch) {
            renderGrid(N, M, field, start, exits, exitCount, cur, DIR_RIGHT, visited, pathMask,
                "BFS: выполняется поиск...");
            sleepMs(anim.searchDelayMs);
        }

        for (int k = 0; k < 4; ++k) {
            int nx = cur.x + dx[k];
            int ny = cur.y + dy[k];
            if (!canMoveTo(N, M, field, nx, ny)) continue;
            if (visited[ny][nx]) continue;
            visited[ny][nx] = true;
            parent[ny][nx] = cur;
            q.push({ nx, ny });
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    stats.elapsedMs = std::chrono::duration<double, std::milli>(t2 - t1).count();

    if (!stats.found) return stats;

    Pos path[MAX_N * MAX_M];
    int pathSize = buildPath(start, foundExit, parent, path);
    makePathMask(path, pathSize, pathMask);

    stats.pathLength = pathSize - 1;
    stats.pathCost = computePathCost(path, pathSize, field);

    if (anim.animateRobot) {
        animateRobotPath(N, M, field, start, exits, exitCount, path, pathSize,
            visited, pathMask, anim.moveDelayMs, "BFS");
    }
    else {
        char status[128];
        std::snprintf(status, sizeof(status), "BFS завершён. Длина пути: %d, стоимость: %d",
            stats.pathLength, stats.pathCost);
        renderGrid(N, M, field, start, exits, exitCount, foundExit, DIR_RIGHT, visited, pathMask, status);
    }
    return stats;
}
