/**
 * Hybrid randomized ghost AI strategy that mixes multiple behaviours
 *
 * This class implements a **non-deterministic / personality-varied** ghost
 * movement strategy by randomly selecting one of four different behaviours each
 * time a move is requested. It serves as a simple way to create ghosts that
 * feel less predictable and more "alive" compared to purely deterministic
 * chasers.
 *
 * Behaviours (chosen uniformly at random with equal probability ~25% each):
 *   0. **Aggressive greedy**     → direct greedy chase toward Pacman's current
 * position
 *   1. **Distance greedy**       → moves to neighbour that minimises Euclidean
 * distance to Pacman
 *   2. **All-pairs shortest path** → optimal path-based ambush / intercept
 * movement (uses precomputed APSP table)
 *   3. **Avoid other ghosts**    → tries to maximise total Euclidean distance
 * from **all other ghosts** (flee / spread-out behaviour when surrounded)
 *
 * Objective:
 *   - Break pattern repetition that pure deterministic strategies can produce
 *   - Create emergent, unpredictable group behaviour when multiple ghosts use
 * this strategy
 *   - Simulate "personality" or "mood swings" without complex state machines
 *   - Combine strengths of cheap greedy methods with occasional optimal-path
 * decisions
 *
 * Time Complexity:
 *   Per call to findNextMove():
 *     • Random choice:                O(1)
 *     • Aggressive / Distance greedy: O(1)  (constant neighbours)
 *     • All-pairs shortest path:      O(1)  (lookup-based, assuming precomputed
 * table) • Avoid-others mode:            O(D × G) ≈ O(1)   where D ≤ 4
 * (neighbours), G = number of other ghosts (usually 2–3) Overall worst-case:
 * O(G)  — still extremely fast
 *
 * Space Complexity:
 *   • O(1) per instance (only owns pointers to sub-strategies)
 *   • Sub-strategies (AggressiveGreedyStrategy, DistanceGreedyStrategy,
 * AllPairsShortestPath) have their own memory footprint (especially APSP's
 * O(V²) distance table)
 *
 */

#include "../include/RandomStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include "../include/Monster.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

RandomStrategy::RandomStrategy(const std::vector<Monster *> &monstersList)
    : monsters(monstersList) {
  aggressiveStrategy = new AggressiveGreedyStrategy();
  distanceStrategy = new DistanceGreedyStrategy();
  allPairsShortestPath = new AllPairsShortestPath();
  std::srand(std::time(nullptr)); // Seed random number generator
}

RandomStrategy::~RandomStrategy() {
  delete aggressiveStrategy;
  delete distanceStrategy;
  delete allPairsShortestPath;
}

Location RandomStrategy::findNextMove(Graph *graph, Entity *monster,
                                      Entity *target) {
  int choice = std::rand() % 4;

  if (choice == 0) {
    return aggressiveStrategy->findNextMove(graph, monster, target);
  } else if (choice == 1) {
    return distanceStrategy->findNextMove(graph, monster, target);
  } else if (choice == 2) {
    return allPairsShortestPath->findNextMove(graph, monster, target);
  } else {
    // Run away from other ghosts
    Location currentLoc = monster->getLocation();
    Node *currentNode = graph->getNode(currentLoc);
    const auto &neighbours = currentNode->getNeighbours();

    Location bestMove = currentLoc;
    double maxDistanceSum = -1.0;

    for (Node *neighbour : neighbours) {
      Location neighbourLoc = neighbour->getLocation();
      double currentDistanceSum = 0.0;

      for (Monster *other : monsters) {
        if (other == monster)
          continue;

        Location otherLoc = other->getLocation();
        int dx = neighbourLoc.x - otherLoc.x;
        int dy = neighbourLoc.y - otherLoc.y;

        // Toroidal wrap
        if (std::abs(dx) > Graph::WIDTH / 2)
          dx = Graph::WIDTH - std::abs(dx);
        if (std::abs(dy) > Graph::HEIGHT / 2)
          dy = Graph::HEIGHT - std::abs(dy);

        currentDistanceSum += std::sqrt(dx * dx + dy * dy);
      }

      if (currentDistanceSum > maxDistanceSum) {
        maxDistanceSum = currentDistanceSum;
        bestMove = neighbourLoc;
      }
    }

    return bestMove;
  }
}
