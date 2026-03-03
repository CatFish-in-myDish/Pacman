/**
 * Implements PinchGreedyStrategy.
 * 
 * Creates a "pincer" movement by finding the closest other ghost to Pacman, 
 * computing the exact opposite point relative to Pacman, and greedily moving 
 * towards that opposite point.
 */
#include "../include/PinchGreedyStrategy.h"
#include "../include/Entity.h"
#include "../include/Monster.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>
#include <algorithm>

PinchGreedyStrategy::PinchGreedyStrategy(const std::vector<Monster *> &monsters)
    : allMonsters(monsters) {}

Location PinchGreedyStrategy::findNextMove(Graph *graph, Entity *monster,
                                           Entity *target) {
  Location currentLoc = monster->getLocation();
  Location pacmanLoc = target->getLocation();

  double minDistanceToPacman = std::numeric_limits<double>::max();
  Monster *closestOtherGhost = nullptr;

  for (Monster *otherGhost : allMonsters) {
    if (otherGhost == monster || !otherGhost)
      continue;

    Location otherLoc = otherGhost->getLocation();

    int dx = otherLoc.x - pacmanLoc.x;
    int dy = otherLoc.y - pacmanLoc.y;

    if (std::abs(dx) > Graph::WIDTH / 2) {
      dx = Graph::WIDTH - std::abs(dx);
    }
    if (std::abs(dy) > Graph::HEIGHT / 2) {
      dy = Graph::HEIGHT - std::abs(dy);
    }

    double dist = std::sqrt(dx * dx + dy * dy);
    if (dist < minDistanceToPacman) {
      minDistanceToPacman = dist;
      closestOtherGhost = otherGhost;
    }
  }

  Location targetLoc = pacmanLoc; // Fallback if no other ghost
  if (closestOtherGhost) {
    Location closestLoc = closestOtherGhost->getLocation();
    int ox = pacmanLoc.x - closestLoc.x;
    int oy = pacmanLoc.y - closestLoc.y;

    // Toroidal shortest offset
    if (std::abs(ox) > Graph::WIDTH / 2) {
      ox = (ox > 0) ? (ox - Graph::WIDTH) : (ox + Graph::WIDTH);
    }
    if (std::abs(oy) > Graph::HEIGHT / 2) {
      oy = (oy > 0) ? (oy - Graph::HEIGHT) : (oy + Graph::HEIGHT);
    }

    targetLoc.x = pacmanLoc.x + ox;
    targetLoc.y = pacmanLoc.y + oy;

    // Toroidal wrap the target location
    targetLoc.x = (targetLoc.x % Graph::WIDTH + Graph::WIDTH) % Graph::WIDTH;
    targetLoc.y = (targetLoc.y % Graph::HEIGHT + Graph::HEIGHT) % Graph::HEIGHT;
  }

  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbours = currentNode->getNeighbours();

  Location bestMove = currentLoc;
  double minDistanceToTarget = std::numeric_limits<double>::max();

  for (Node *neighbour : neighbours) {
    Location neighbourLoc = neighbour->getLocation();

    int dx = neighbourLoc.x - targetLoc.x;
    int dy = neighbourLoc.y - targetLoc.y;

    if (std::abs(dx) > Graph::WIDTH / 2) {
      dx = Graph::WIDTH - std::abs(dx);
    }
    if (std::abs(dy) > Graph::HEIGHT / 2) {
      dy = Graph::HEIGHT - std::abs(dy);
    }

    double dist = std::sqrt(dx * dx + dy * dy);

    if (dist < minDistanceToTarget) {
      minDistanceToTarget = dist;
      bestMove = neighbourLoc;
    }
  }

  return bestMove;
}
