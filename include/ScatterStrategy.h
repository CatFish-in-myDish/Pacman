#ifndef SCATTERSTRATEGY_H
#define SCATTERSTRATEGY_H

#include "GreedyStrategy.h"

// Monster Scatter Strategy: Flees to corners or patrols
class ScatterStrategy : public GreedyStrategy {
public:
  Location findNextMove(Graph *graph, Entity *monster, Entity *target) override;
};

#endif
