/*
Implements the Scatter Strategy for ghost movement.

This module defines the behaviour where a ghost moves toward
a predefined corner or target tile in the maze instead of
directly chasing Pacman.

The scatter mode is used to:
  - Reduce continuous pressure on Pacman.
  - Prevent ghost clustering.
  - Create strategic movement variation.

Each ghost is assigned a fixed scatter target
(e.g., top-left, top-right, bottom-left, bottom-right corner).
During scatter mode, the ghost computes the shortest path
toward its assigned corner using standard movement logic
or pathfinding algorithms.

This behaviour typically alternates with chase mode
as part of the ghost AI state system.

Time Complexity:
  - Target selection: O(1)
  - Movement update per frame: depends on pathfinding
    (e.g., O(E log V) if using A*).

Since scatter targets are fixed and simple,
the strategy remains lightweight and efficient.
*/

#include "../include/ScatterStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>

Location ScatterStrategy::findNextMove(Graph *graph, Entity *monster,
                                       Entity *target) {
  // For now, we target the top-left corner (0,0) as a placeholder scatter
  // point.
  Location targetLoc(0, 0);

  Location currentLoc = monster->getLocation();
  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbours = currentNode->getNeighbours();

  Location bestMove = currentLoc;
  double minDistance = std::numeric_limits<double>::max();

  for (Node *neighbour : neighbours) {
    Location neighbourLoc = neighbour->getLocation();

    // Calculate distance to scatter target (0,0)
    int dx = neighbourLoc.x - targetLoc.x;
    int dy = neighbourLoc.y - targetLoc.y;

    // Consider toroidal wrapping (standard shortest path on torus)
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
