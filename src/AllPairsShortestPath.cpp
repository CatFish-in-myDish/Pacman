/*
Cooperative Ambush AI strategy for ghosts/monsters using precomputed All-Pairs
Shortest Paths (APSP)

This module implements an ambush-style ghost AI for Pacman. Instead of greedily
chasing Pacman, each ghost predicts a future position ("ambush point") several
tiles ahead in Pacman's current moving direction and moves optimally toward that
point using a precomputed distance table.

Strategy highlights:
  - Precomputes shortest path distances from **every non-wall tile to every
other non-wall tile** using BFS from each starting position (multi-source
style APSP).
  - At runtime, ghosts instantly look up the best next move that reduces
distance to the predicted ambush location.
  - Falls back to chasing Pacman's current position if the predicted point
is invalid (wall, out of bounds, or unreachable).

Objective:
  - Increase catch probability by **intercepting** rather than tail-chasing
  - Exploit Pacman's predictable movement when moving in straight lines
  - Achieve near-optimal pathing in open areas with very low per-frame cost

Time Complexity:
  Precomputation (done once, lazily on first call):
    • O(V × (V + E)) where V = number of walkable tiles, E = number of edges
    • In a typical maze: V ≈ 20–30% of total tiles (e.g., ~200–400 walkable
tiles)
    • Total precomputation cost: roughly O(V²) in practice for grid graphs

  Runtime (per ghost, per frame):
    • O(degree) = O(1) to O(4) — just checking 2–4 neighbours
    • Distance lookups are O(1) average case (unordered_map)

Space Complexity:
  • O(V²) — distance table: unordered_map<Location, unordered_map<Location,
int>>
  • For ~300 walkable tiles → ~90,000–100,000 entries (~400–800 KB)
*/

#include "../include/AllPairsShortestPath.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include "../include/Node.h"
#include <queue>
#include <unordered_map>

Location AllPairsShortestPath::findNextMove(Graph *graph, Entity *monster,
                                            Entity *target) {
  // --- PART 1: DP PRECOMPUTATION (Run once) ---
  static std::unordered_map<Location, std::unordered_map<Location, int>>
      distTable;

  if (distTable.empty()) {
    for (int x = 0; x < Graph::WIDTH; ++x) {
      for (int y = 0; y < Graph::HEIGHT; ++y) {
        if (!Graph::isWall(x, y)) {
          Location start(x, y);
          std::queue<std::pair<Location, int>> q;
          q.push({start, 0});
          distTable[start][start] = 0;

          while (!q.empty()) {
            auto [curr, d] = q.front();
            q.pop();
            Node *node = graph->getNode(curr);
            if (!node)
              continue;

            for (Node *neighbour : node->getNeighbours()) {
              Location next = neighbour->getLocation();
              if (distTable[start].find(next) == distTable[start].end()) {
                distTable[start][next] = d + 1;
                q.push({next, d + 1});
              }
            }
          }
        }
      }
    }
  }

  // --- PART 2: AMBUSH TARGET SELECTION ---
  Location targetLoc = target->getLocation();
  Location targetDir = target->getLastDirection();

  // Predict Pacman's future position (The "Ambush" point)
  Location ambushPoint(targetLoc.x + targetDir.x * 4,
                       targetLoc.y + targetDir.y * 4);

  // Boundary check and snapping
  if (ambushPoint.x < 0)
    ambushPoint.x = 0;
  if (ambushPoint.x >= Graph::WIDTH)
    ambushPoint.x = Graph::WIDTH - 1;
  if (ambushPoint.y < 0)
    ambushPoint.y = 0;
  if (ambushPoint.y >= Graph::HEIGHT)
    ambushPoint.y = Graph::HEIGHT - 1;

  // Fallback: If the ambush point is a wall or unreachable, target Pacman
  // directly.
  if (Graph::isWall(ambushPoint.x, ambushPoint.y) ||
      distTable.find(ambushPoint) == distTable.end()) {
    ambushPoint = targetLoc;
  }

  // --- PART 3: OPTIMAL MOVE SELECTION ---
  Location currentLoc = monster->getLocation();
  Node *currentNode = graph->getNode(currentLoc);

  if (currentLoc == ambushPoint)
    return currentLoc;

  const auto &neighbours = currentNode->getNeighbours();
  Location bestMove = currentLoc;
  int minDistance = 1000000;

  for (Node *neighbour : neighbours) {
    Location nLoc = neighbour->getLocation();
    if (distTable[nLoc].count(ambushPoint)) {
      int d = distTable[nLoc][ambushPoint];
      if (d < minDistance) {
        minDistance = d;
        bestMove = nLoc;
      }
    }
  }

  return bestMove;
}
