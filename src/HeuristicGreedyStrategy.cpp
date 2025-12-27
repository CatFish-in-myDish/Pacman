#include "../include/HeuristicGreedyStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>

Location HeuristicGreedyStrategy::findNextMove(Graph *graph, Entity *monster,
                                               Entity *target) {
  Location currentLoc = monster->getLocation();
  Location targetLoc = target->getLocation();

  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbors = currentNode->getNeighbors();

  Location bestMove = currentLoc;
  int minDistance = std::numeric_limits<int>::max();

  for (Node *neighbor : neighbors) {
    Location neighborLoc = neighbor->getLocation();

    // Calculate Manhattan distance
    int dx = abs(neighborLoc.x - targetLoc.x);
    int dy = abs(neighborLoc.y - targetLoc.y);

    // Consider toroidal wrapping
    if (dx > Graph::WIDTH / 2) {
      dx = Graph::WIDTH - dx;
    }
    if (dy > Graph::HEIGHT / 2) {
      dy = Graph::HEIGHT - dy;
    }

    int distance = dx + dy;

    if (distance < minDistance) {
      minDistance = distance;
      bestMove = neighborLoc;
    }
  }

  return bestMove;
}
