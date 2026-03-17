#include "dijkstra.h"
#include "grid.h"
#include "console_ui.h"
#include "path_utils.h"
#include <queue>
#include <chrono>
#include <cstdio>

struct DNode {
    int dist;
    Pos pos;
    bool operator<(const DNode& other) const { return dist > other.dist; }
};

SearchStats runAutoModeDijkstra(
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
    int dist[MAX_N][MAX_M];
    Pos parent[MAX_N][MAX_M];
    clearBoolMask(closed);
    clearBoolMask(pathMask);

    for (int y = 0; y < N; ++y)
        for (int x = 0; x < M; ++x)
            dist[y][x] = 1000000000;

    std::priority_queue<DNode> pq;
    dist[start.y][start.x] = 0;
    parent[start.y][start.x] = start;
    pq.push({ 0, start });

    Pos foundExit{};
    auto t1 = std::chrono::high_resolution_clock::now();
    const int dx[4] = { 1, -1, 0, 0 };
    const int dy[4] = { 0, 0, 1, -1 };

    while (!pq.empty()) {
        DNode curNode = pq.top(); pq.pop();
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
                "Дейкстра: выполняется поиск...");
            sleepMs(anim.searchDelayMs);
        }

        for (int k = 0; k < 4; ++k) {
            int nx = cur.x + dx[k];
            int ny = cur.y + dy[k];
            if (!canMoveTo(N, M, field, nx, ny)) continue;
            int nd = dist[cur.y][cur.x] + cellCost(field[ny][nx]);
            if (nd < dist[ny][nx]) {
                dist[ny][nx] = nd;
                parent[ny][nx] = cur;
                pq.push({ nd, { nx, ny } });
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
    stats.pathCost = dist[foundExit.y][foundExit.x];

    if (anim.animateRobot) {
        animateRobotPath(N, M, field, start, exits, exitCount, path, pathSize,
            closed, pathMask, anim.moveDelayMs, "Дейкстра");
    }
    else {
        char status[128];
        std::snprintf(status, sizeof(status), "Дейкстра завершён. Шаги: %d, стоимость: %d",
            stats.pathLength, stats.pathCost);
        renderGrid(N, M, field, start, exits, exitCount, foundExit, DIR_RIGHT, closed, pathMask, status);
    }
    return stats;
}
