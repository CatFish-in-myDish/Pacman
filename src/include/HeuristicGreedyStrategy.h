#ifndef HEURISTICGREEDYSTRATEGY_H
#define HEURISTICGREEDYSTRATEGY_H

#include "GreedyStrategy.h"

// Monster 2: Heuristic Greedy - uses Manhattan distance
class HeuristicGreedyStrategy : public GreedyStrategy {
public:
    Location findNextMove(Graph* graph, Entity* monster, Entity* target) override;
};

#endif 