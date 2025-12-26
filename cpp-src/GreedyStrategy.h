#ifndef GREEDYSTRATEGY_H
#define GREEDYSTRATEGY_H

#include "Location.h"

// Forward declarations
class Graph;
class Entity;

// Abstract interface for greedy strategies
class GreedyStrategy {
public:
    virtual ~GreedyStrategy() = default;

    // Find the next move for a monster pursuing a target
    virtual Location findNextMove(Graph* graph, Entity* monster, Entity* target) = 0;
};

#endif // GREEDYSTRATEGY_H
