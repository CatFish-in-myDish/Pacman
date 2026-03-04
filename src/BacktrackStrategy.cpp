/**
 * Frightened-mode movement strategy for ghosts using path backtracking + Euclidean flee
 *
 * This module implements the classic "frightened" ghost behaviour seen in many Pac-Man variants.
 * When a ghost enters FRIGHTENED mode (usually after Pac-Man eats a power pellet), it should
 * appear panicked and behave less aggressively.
 *
 * Core behaviour:
 *   1. While there is recorded movement history → retrace steps in **reverse order** (LIFO)
 *      → effectively "running back the way it came"
 *   2. Once history is exhausted → switch to pure flee mode:
 *      - Choose the neighbour that maximises **Euclidean distance** from Pac-Man's current position
 *      - Includes basic toroidal (wrap-around) distance handling for mazes with tunnel warping
 *
 * This creates the characteristic "scared wobbling back-and-forth" motion when frightened,
 * followed by purposeful fleeing once the ghost has retraced its recent path.
 *
 * Objective:
 *   - Simulate frightened / avoidant behaviour without complex pathfinding
 *   - Reuse existing path history (assumed to be maintained by the Monster class)
 *   - Provide visually convincing panic movement with very low computational cost
 *   - Maintain reasonable escape behaviour in open spaces or after history is depleted
 *
 * Time Complexity:
 *   - When retracing history:         O(1)     — single pop operation from deque/stack
 *   - When fleeing (history empty):   O(1)     — fixed number of neighbours (≤4)
 *                                              - distance calculations are constant-time
 *   Overall per-frame cost:           O(1)     — extremely cheap, ideal for real-time
 *
 * Space Complexity:
 *   - Dominated by the path history stored in Monster class (not in this strategy)
 *   - Typical history size: 8–30 positions (depending on implementation)
 *   - This class itself uses negligible extra memory

 *
 */
#include "../include/BacktrackStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include "../include/Monster.h"
#include <cmath>

Location BacktrackStrategy::findNextMove(Graph *graph, Entity *monster,
                                         Entity *target) {
  // Cast to Monster to access path history
  Monster *m = dynamic_cast<Monster *>(monster);

  // If we have recorded history, retrace it (LIFO)
  if (m && m->hasHistory()) {
    return m->popLastPosition();
  }

  // History exhausted — flee from Pacman (maximise distance)
  Location currentLoc = monster->getLocation();
  Location targetLoc = target->getLocation();

  Node *currentNode = graph->getNode(currentLoc);
  if (!currentNode) {
    return currentLoc;
  }
  const auto &neighbours = currentNode->getNeighbours();

  Location bestMove = currentLoc;
  double maxDistance = -1.0;

  for (Node *neighbour : neighbours) {
    Location neighbourLoc = neighbour->getLocation();

    int dx = neighbourLoc.x - targetLoc.x;
    int dy = neighbourLoc.y - targetLoc.y;

    // Consider toroidal wrapping
    if (std::abs(dx) > Graph::WIDTH / 2) {
      dx = Graph::WIDTH - std::abs(dx);
    }
    if (std::abs(dy) > Graph::HEIGHT / 2) {
      dy = Graph::HEIGHT - std::abs(dy);
    }

    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance > maxDistance) {
      maxDistance = distance;
      bestMove = neighbourLoc;
    }
  }

  return bestMove;
}
