/**
 * Implements a Cooperative Ambush strategy using Dynamic Programming.
 * 
 * Instead of chasing Pacman directly, this strategy targets a tile ahead
 * of Pacman's current direction. It uses a precomputed All-Pairs Shortest Path
 * (APSP) table to instantly find the optimal move to that ambush point.
 */
#include "../include/AllPairShortestPath.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include "../include/Node.h"
#include <cmath>
#include <queue>
#include <unordered_map>

Location AllPairShortestPath::findNextMove(Graph *graph, Entity *monster,
                                           Entity *target) {
  // --- PART 1: DP PRECOMPUTATION (Run once) ---
  static std::unordered_map<Location, std::unordered_map<Location, int>> distTable;
  
  if (distTable.empty()) {
    for (int x = 0; x < Graph::WIDTH; ++x) {
      for (int y = 0; y < Graph::HEIGHT; ++y) {
        if (!Graph::isWall(x, y)) {
          Location start(x, y);
          std::queue<std::pair<Location, int>> q;
          q.push({start, 0});
          distTable[start][start] = 0;

          while (!q.empty()) {
            auto [curr, d] = q.front(); q.pop();
            Node* node = graph->getNode(curr);
            if (!node) continue;

            for (Node* neighbor : node->getNeighbours()) {
              Location next = neighbor->getLocation();
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
  Location ambushPoint(targetLoc.x + targetDir.x * 4, targetLoc.y + targetDir.y * 4);
  
  // Boundary check and snapping
  if (ambushPoint.x < 0) ambushPoint.x = 0;
  if (ambushPoint.x >= Graph::WIDTH) ambushPoint.x = Graph::WIDTH - 1;
  if (ambushPoint.y < 0) ambushPoint.y = 0;
  if (ambushPoint.y >= Graph::HEIGHT) ambushPoint.y = Graph::HEIGHT - 1;

  // Fallback: If the ambush point is a wall or unreachable, target Pacman directly.
  if (Graph::isWall(ambushPoint.x, ambushPoint.y) || distTable.find(ambushPoint) == distTable.end()) {
    ambushPoint = targetLoc;
  }
  
  // --- PART 3: OPTIMAL MOVE SELECTION ---
  Location currentLoc = monster->getLocation();
  Node *currentNode = graph->getNode(currentLoc);
  
  if (currentLoc == ambushPoint) return currentLoc;

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
