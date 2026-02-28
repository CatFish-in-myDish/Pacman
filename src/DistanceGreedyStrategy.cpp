/**
 * Implements a standard greedy strategy that minimises Euclidean distance to
 * the target.
 *
 * This strategy simply calculates the Euclidean distance from each neighbour to
 * the target's current location and chooses the neighbour that offers the
 * minimum distance.
 *
 * Time Complexity: O(1) per move, as it evaluates a constant number of
 * neighbours.
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
  const auto &neighbours = currentNode->getNeighbours();

  Location bestMove = currentLoc;
  double minDistance = std::numeric_limits<double>::max();

  for (Node *neighbour : neighbours) {
    Location neighbourLoc = neighbour->getLocation();

    // Calculate Euclidean distance
    int dx = neighbourLoc.x - targetLoc.x;
    int dy = neighbourLoc.y - targetLoc.y;

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
      bestMove = neighbourLoc;
    }
  }

  return bestMove;
}
