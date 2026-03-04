#ifndef BACKTRACKSTRATEGY_H
#define BACKTRACKSTRATEGY_H

#include "GreedyStrategy.h"

// Retraces the monster's recorded path history (LIFO).
// Falls back to fleeing from the target when history is exhausted.
class BacktrackStrategy : public GreedyStrategy {
public:
  Location findNextMove(Graph *graph, Entity *monster, Entity *target) override;
};

#endif
