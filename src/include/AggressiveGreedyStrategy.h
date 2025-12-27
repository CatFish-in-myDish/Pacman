#ifndef AGGRESSIVEGREEDYSTRATEGY_H
#define AGGRESSIVEGREEDYSTRATEGY_H

#include "GreedyStrategy.h"

// Monster 4: Aggressive Greedy - predicts Pacman's future position
class AggressiveGreedyStrategy : public GreedyStrategy {
public:
    Location findNextMove(Graph* graph, Entity* monster, Entity* target) override;
};

#endif 