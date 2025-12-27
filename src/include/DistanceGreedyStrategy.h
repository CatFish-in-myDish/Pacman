#ifndef DISTANCEGREEDYSTRATEGY_H
#define DISTANCEGREEDYSTRATEGY_H

#include "GreedyStrategy.h"

// Monster 1: Pure Distance Greedy - minimizes Euclidean distance
class DistanceGreedyStrategy : public GreedyStrategy {
public:
    Location findNextMove(Graph* graph, Entity* monster, Entity* target) override;
};

#endif