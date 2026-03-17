#include "astar.h"
#include "grid.h"
#include "console_ui.h"
#include "path_utils.h"
#include <queue>
#include <chrono>
#include <cstdio>
#include <cstdlib>

static int heuristicToClosestExit(Pos p, const Pos exits[MAX_EXITS], int exitCount) {
    int best = 1000000000;
    for (int i = 0; i < exitCount; ++i) {
        int h = std::abs(p.x - exits[i].x) + std::abs(p.y - exits[i].y);
        if (h < best) best = h;
    }
    return best;
}

struct ANode {
    int f;
    int g;
    Pos pos;
    bool operator<(const ANode& other) const {
        if (f != other.f) return f > other.f;
        return g > other.g;
    }
};

SearchStats runAutoModeAStar(
    int N, int M,
    const int field[MAX_N][MAX_M],
    Pos start,
    const Pos exits[MAX_EXITS],
    int exitCount,
    const AnimationSettings& anim
) {
    SearchStats stats;
    bool closed[MAX_N][MAX_M];
    bool pathMask[MAX_N][MAX_M];
    int gScore[MAX_N][MAX_M];
    Pos parent[MAX_N][MAX_M];
    clearBoolMask(closed);
    clearBoolMask(pathMask);

    for (int y = 0; y < N; ++y)
        for (int x = 0; x < M; ++x)
            gScore[y][x] = 1000000000;

    std::priority_queue<ANode> pq;
    gScore[start.y][start.x] = 0;
    parent[start.y][start.x] = start;
    pq.push({ heuristicToClosestExit(start, exits, exitCount), 0, start });

    Pos foundExit{};
    auto t1 = std::chrono::high_resolution_clock::now();
    const int dx[4] = { 1, -1, 0, 0 };
    const int dy[4] = { 0, 0, 1, -1 };

    while (!pq.empty()) {
        ANode curNode = pq.top(); pq.pop();
        Pos cur = curNode.pos;
        if (closed[cur.y][cur.x]) continue;
        closed[cur.y][cur.x] = true;
        ++stats.visitedCount;

        if (isExit(exits, exitCount, cur)) {
            stats.found = true;
            foundExit = cur;
            stats.reachedExit = cur;
            stats.reachedExitIndex = findExitIndex(exits, exitCount, cur);
            break;
        }

        if (anim.visualizeSearch) {
            renderGrid(N, M, field, start, exits, exitCount, cur, DIR_RIGHT, closed, pathMask,
                "A*: выполняется поиск...");
            sleepMs(anim.searchDelayMs);
        }

        for (int k = 0; k < 4; ++k) {
            int nx = cur.x + dx[k];
            int ny = cur.y + dy[k];
            if (!canMoveTo(N, M, field, nx, ny)) continue;
            int tentativeG = gScore[cur.y][cur.x] + cellCost(field[ny][nx]);
            if (tentativeG < gScore[ny][nx]) {
                gScore[ny][nx] = tentativeG;
                parent[ny][nx] = cur;
                int f = tentativeG + heuristicToClosestExit({ nx, ny }, exits, exitCount);
                pq.push({ f, tentativeG, { nx, ny } });
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    stats.elapsedMs = std::chrono::duration<double, std::milli>(t2 - t1).count();

    if (!stats.found) return stats;

    Pos path[MAX_N * MAX_M];
    int pathSize = buildPath(start, foundExit, parent, path);
    makePathMask(path, pathSize, pathMask);

    stats.pathLength = pathSize - 1;
    stats.pathCost = gScore[foundExit.y][foundExit.x];

    if (anim.animateRobot) {
        animateRobotPath(N, M, field, start, exits, exitCount, path, pathSize,
            closed, pathMask, anim.moveDelayMs, "A*");
    }
    else {
        char status[128];
        std::snprintf(status, sizeof(status), "A* завершён. Шаги: %d, стоимость: %d",
            stats.pathLength, stats.pathCost);
        renderGrid(N, M, field, start, exits, exitCount, foundExit, DIR_RIGHT, closed, pathMask, status);
    }
    return stats;
}
