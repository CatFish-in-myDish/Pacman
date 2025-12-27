#include "../include/AggressiveGreedyStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>

Location AggressiveGreedyStrategy::findNextMove(Graph *graph, Entity *monster,
                                                Entity *target) {
  Location currentLoc = monster->getLocation();
  Location targetLoc = target->getLocation();
  Location targetDir = target->getLastDirection();

  // Predict where Pacman will be 2 steps in the future
  Location predictedLoc = targetLoc;

  if (targetDir.x != 0 || targetDir.y != 0) {
    // Apply direction twice with wrapping
    int predX = (targetLoc.x + targetDir.x * 2) % Graph::WIDTH;
    int predY = (targetLoc.y + targetDir.y * 2) % Graph::HEIGHT;

    // Handle negative modulo
    if (predX < 0)
      predX += Graph::WIDTH;
    if (predY < 0)
      predY += Graph::HEIGHT;

    predictedLoc = Location(predX, predY);
  }

  // Find neighbor closest to predicted location
  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbors = currentNode->getNeighbors();

  Location bestMove = currentLoc;
  double minDistance = std::numeric_limits<double>::max();

  for (Node *neighbor : neighbors) {
    Location neighborLoc = neighbor->getLocation();

    // Calculate distance to predicted location
    int dx = neighborLoc.x - predictedLoc.x;
    int dy = neighborLoc.y - predictedLoc.y;

    // Consider toroidal wrapping
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
