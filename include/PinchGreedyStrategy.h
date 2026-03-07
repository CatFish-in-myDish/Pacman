#ifndef PINCHGREEDYSTRATEGY_H
#define PINCHGREEDYSTRATEGY_H

#include "GreedyStrategy.h"
#include <vector>

class Monster;

class PinchGreedyStrategy : public GreedyStrategy {
private:
  const std::vector<Monster *> &allMonsters;

public:
  PinchGreedyStrategy(const std::vector<Monster *> &monsters);
  Location findNextMove(Graph *graph, Entity *monster, Entity *target) override;
};

#endif
