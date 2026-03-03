/**
 * Implements the Center Territorial Greedy strategy for the 5th ghost.
 *
 * The ghost guards the center of the maze. It greedily navigates to the
 * exact center (26, 31) if Pacman is outside a 6-tile territory radius.
 * When Pacman enters the territory, the ghost switches to greedy pursuit.
 *
 * Decision logic (per move):
 *   1. Compute toroidal Euclidean distance from Pacman to the center.
 *   2. If distance <= TERRITORY_RADIUS → target = Pacman (chase).
 *   3. Otherwise                      → target = Center  (retreat).
 *   4. Pick the neighbour node that minimises toroidal Euclidean distance
 *      to the active target.
 *
 * Time Complexity: O(1) per move (constant number of neighbours evaluated).
 */
#include "../include/CenterTerritorialGreedyStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>

Location CenterTerritorialGreedyStrategy::findNextMove(Graph *graph,
                                                       Entity *monster,
                                                       Entity *target) {
  Location currentLoc = monster->getLocation();
  Location pacmanLoc = target->getLocation();
  Location center(CENTER_X, CENTER_Y);

  // ── Determine active target ──────────────────────────────────────────
  // Compute toroidal distance from Pacman to the center
  int dpx = pacmanLoc.x - center.x;
  int dpy = pacmanLoc.y - center.y;

  if (std::abs(dpx) > Graph::WIDTH / 2)
    dpx = Graph::WIDTH - std::abs(dpx);
  if (std::abs(dpy) > Graph::HEIGHT / 2)
    dpy = Graph::HEIGHT - std::abs(dpy);

  double pacmanDistToCenter = std::sqrt(dpx * dpx + dpy * dpy);

  // Chase Pacman if he is inside the territory; otherwise retreat to center
  Location activeTarget =
      (pacmanDistToCenter <= TERRITORY_RADIUS) ? pacmanLoc : center;

  // ── Greedy move towards active target ────────────────────────────────
  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbours = currentNode->getNeighbours();

  Location bestMove = currentLoc;
  double minDistance = std::numeric_limits<double>::max();

  for (Node *neighbour : neighbours) {
    Location neighbourLoc = neighbour->getLocation();

    int dx = neighbourLoc.x - activeTarget.x;
    int dy = neighbourLoc.y - activeTarget.y;

    // Toroidal wrapping
    if (std::abs(dx) > Graph::WIDTH / 2)
      dx = Graph::WIDTH - std::abs(dx);
    if (std::abs(dy) > Graph::HEIGHT / 2)
      dy = Graph::HEIGHT - std::abs(dy);

    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance < minDistance) {
      minDistance = distance;
      bestMove = neighbourLoc;
    }
  }

  return bestMove;
}
