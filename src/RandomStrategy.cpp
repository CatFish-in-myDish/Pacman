#include "../include/RandomStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include "../include/Monster.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>

RandomStrategy::RandomStrategy(const std::vector<Monster *> &monstersList)
    : monsters(monstersList) {
  aggressiveStrategy = new AggressiveGreedyStrategy();
  distanceStrategy = new DistanceGreedyStrategy();
  allPairShortestPath = new AllPairShortestPath();
  std::srand(std::time(nullptr)); // Seed random number generator
}

RandomStrategy::~RandomStrategy() {
  delete aggressiveStrategy;
  delete distanceStrategy;
  delete allPairShortestPath;
}

Location RandomStrategy::findNextMove(Graph *graph, Entity *monster,
                                      Entity *target) {
  int choice = std::rand() % 4;

  if (choice == 0) {
    return aggressiveStrategy->findNextMove(graph, monster, target);
  } else if (choice == 1) {
    return distanceStrategy->findNextMove(graph, monster, target);
  } else if (choice == 2) {
    return allPairShortestPath->findNextMove(graph, monster, target);
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
        if (other == monster) continue;

        Location otherLoc = other->getLocation();
        int dx = neighbourLoc.x - otherLoc.x;
        int dy = neighbourLoc.y - otherLoc.y;

        // Toroidal wrap
        if (std::abs(dx) > Graph::WIDTH / 2) dx = Graph::WIDTH - std::abs(dx);
        if (std::abs(dy) > Graph::HEIGHT / 2) dy = Graph::HEIGHT - std::abs(dy);

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
