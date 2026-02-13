#include "../include/ScatterStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>

Location ScatterStrategy::findNextMove(Graph *graph, Entity *monster,
                                        Entity *target) {
  // TODO: Implement specific scatter condition/logic here.
  // For now, we target the top-left corner (0,0) as a placeholder scatter point.
  Location targetLoc(0, 0); 
  
  Location currentLoc = monster->getLocation();
  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbors = currentNode->getNeighbors();

  Location bestMove = currentLoc;
  double minDistance = std::numeric_limits<double>::max();

  for (Node *neighbor : neighbors) {
    Location neighborLoc = neighbor->getLocation();

    // Calculate distance to scatter target (0,0)
    int dx = neighborLoc.x - targetLoc.x;
    int dy = neighborLoc.y - targetLoc.y;

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
      bestMove = neighborLoc;
    }
  }

  return bestMove;
}
