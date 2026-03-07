#ifndef RANDOMSTRATEGY_H
#define RANDOMSTRATEGY_H

#include "GreedyStrategy.h"
#include "AggressiveGreedyStrategy.h"
#include "DistanceGreedyStrategy.h"
#include "AllPairsShortestPath.h"
#include <vector>

class Entity;
class Monster;

class RandomStrategy : public GreedyStrategy {
private:
  std::vector<Monster *> monsters;
  AggressiveGreedyStrategy *aggressiveStrategy;
  DistanceGreedyStrategy *distanceStrategy;
  AllPairsShortestPath *allPairsShortestPath;

public:
  RandomStrategy(const std::vector<Monster *> &monstersList);
  ~RandomStrategy();

  Location findNextMove(Graph *graph, Entity *monster, Entity *target) override;
};

#endif
