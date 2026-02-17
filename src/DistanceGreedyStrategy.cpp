/**
 * Implements a standard greedy strategy that minimizes Euclidean distance to the target.
 *
 * This strategy simply calculates the Euclidean distance from each neighbor to the target's
 * current location and chooses the neighbor that offers the minimum distance.
 *
 * Time Complexity: O(1) per move, as it evaluates a constant number of neighbors.
 */
#include "../include/DistanceGreedyStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>

Location DistanceGreedyStrategy::findNextMove(Graph *graph, Entity *monster,
                                              Entity *target) {
  Location currentLoc = monster->getLocation();
  Location targetLoc = target->getLocation();

  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbors = currentNode->getNeighbors();

  Location bestMove = currentLoc;
  double minDistance = std::numeric_limits<double>::max();

  for (Node *neighbor : neighbors) {
    Location neighborLoc = neighbor->getLocation();

    // Calculate Euclidean distance
    int dx = neighborLoc.x - targetLoc.x;
    int dy = neighborLoc.y - targetLoc.y;

    // Consider toroidal wrapping for distance calculation
    if (abs(dx) > Graph::WIDTH / 2) {
      dx = Graph::WIDTH - abs(dx);
    }
    if (abs(dy) > Graph::HEIGHT / 2) {
      dy = Graph::HEIGHT - abs(dy);
    }

    double distance = sqrt(dx * dx + dy * dy);

    if (distance < minDistance) {
      minDistance = distance;
      bestMove = neighborLoc;
    }
  }

  return bestMove;
}
