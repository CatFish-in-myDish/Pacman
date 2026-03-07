/*
Cooperative "pincer / pinch" movement strategy for multiple ghosts

Implements a team-based ghost AI tactic that attempts to **trap / surround
Pacman** by coordinating with other ghosts in a simplified way.

Core idea — "Pinch" or "Pincer" movement:
  1. Identify the **closest other ghost** to Pacman (among all living
monsters)
  2. Compute the point that is **exactly opposite** to that ghost with
respect to Pacman → i.e. Pacman is the midpoint between the closest ghost
and the computed target point
  3. Greedily move toward that opposite point using **Euclidean distance**
(with toroidal wrapping)
  4. If no other ghost exists → simply chase Pacman directly

This creates a basic flanking / squeezing behaviour:
  - One ghost is already close → the current ghost tries to approach from the
other side
  - When multiple ghosts use this strategy → they naturally tend to spread
out around Pacman

Objective:
  - Increase catch probability through **coordinated encirclement** rather
than all chasing the same tail
  - Simulate simple team cooperation without heavy communication or path
coordination
  - Remain computationally cheap while still producing
better-than-independent chasing in many situations

Time Complexity:
  Per frame (per ghost using this strategy):
    • Finding closest ghost:      O(N)     where N = number of ghosts
(usually 3–4)
    • Choosing best neighbour:    O(1)     (≤4 neighbours)
    • Distance calculations:      constant-time
    Overall: O(N) — very fast for typical ghost counts

Space Complexity:
  • O(1) extra per instance (only stores reference to the monster list)
  • Relies on external std::vector<Monster*> passed at construction
*/

#include "../include/PinchGreedyStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include "../include/Monster.h"
#include <cmath>
#include <limits>

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
