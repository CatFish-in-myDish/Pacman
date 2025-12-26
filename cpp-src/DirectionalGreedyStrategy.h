#ifndef DIRECTIONALGREEDYSTRATEGY_H
#define DIRECTIONALGREEDYSTRATEGY_H

#include "GreedyStrategy.h"

// Monster 3: Directional Greedy - prioritizes reducing larger axis difference
class DirectionalGreedyStrategy : public GreedyStrategy {
public:
    Location findNextMove(Graph* graph, Entity* monster, Entity* target) override;
};

#endif // DIRECTIONALGREEDYSTRATEGY_H
