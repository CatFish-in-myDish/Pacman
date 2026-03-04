/**
 * Implements the Backtrack Strategy for frightened ghost movement.
 *
 * When a monster is in FRIGHTENED mode, this strategy retraces the
 * monster's recorded path history in reverse (LIFO order). Once the
 * history is exhausted, the monster flees by choosing the neighbour
 * that maximises Euclidean distance from Pacman.
 *
 * Time Complexity:
 *   - With history: O(1) per move (pop from deque)
 *   - Without history (flee): O(1) per move (constant neighbours)
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
