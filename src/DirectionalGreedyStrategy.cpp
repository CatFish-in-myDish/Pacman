/**
 * Implements a greedy strategy that prioritizes movement along the major axis of distance.
 *
 * This strategy calculates the difference in X and Y coordinates to the target and prioritizes
 * moving along the axis with the larger difference. It assigns scores to neighbors based on this
 * priority to choose the best move.
 *
 * Time Complexity: O(1) per move, as it evaluates a constant number of neighbors.
 */
#include "../include/DirectionalGreedyStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>

Location DirectionalGreedyStrategy::findNextMove(Graph *graph, Entity *monster,
                                                 Entity *target) {
  Location currentLoc = monster->getLocation();
  Location targetLoc = target->getLocation();

  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbors = currentNode->getNeighbors();

  // If no neighbors available, stay in place
  if (neighbors.empty()) {
    return currentLoc;
  }

  // Calculate direction vector from current position to target
  int dx = targetLoc.x - currentLoc.x;
  int dy = targetLoc.y - currentLoc.y;

  // Apply toroidal wrapping to find shortest path
  if (std::abs(dx) > Graph::WIDTH / 2) {
    dx = (dx > 0) ? dx - Graph::WIDTH : dx + Graph::WIDTH;
  }
  if (std::abs(dy) > Graph::HEIGHT / 2) {
    dy = (dy > 0) ? dy - Graph::HEIGHT : dy + Graph::HEIGHT;
  }

  // Determine which axis has larger distance (the "major axis")
  // Prioritise movement along the major axis to close distance faster
  bool preferHorizontal = std::abs(dx) >= std::abs(dy);

  // Evaluate each neighbor and choose the best move
  Location bestMove = currentLoc;
  double minScore = std::numeric_limits<double>::max();

  for (Node *neighbor : neighbors) {
    Location nextLoc = neighbor->getLocation();

    // Calculate distance from this neighbor to target
    int nDx = targetLoc.x - nextLoc.x;
    int nDy = targetLoc.y - nextLoc.y;

    // Apply toroidal wrapping
    if (std::abs(nDx) > Graph::WIDTH / 2) {
      nDx = (nDx > 0) ? nDx - Graph::WIDTH : nDx + Graph::WIDTH;
    }
    if (std::abs(nDy) > Graph::HEIGHT / 2) {
      nDy = (nDy > 0) ? nDy - Graph::HEIGHT : nDy + Graph::HEIGHT;
    }

    // Score calculation: heavily weight the major axis distance
    // This makes the monster prioritise closing the gap on the larger
    // difference
    double score;
    if (preferHorizontal) {
      score = std::abs(nDx) * 1000 + std::abs(nDy); // Prioritise X axis
    } else {
      score = std::abs(nDy) * 1000 + std::abs(nDx); // Prioritise Y axis
    }

    if (score < minScore) {
      minScore = score;
      bestMove = nextLoc;
    }
  }

  return bestMove;
}
