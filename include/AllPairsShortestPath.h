#ifndef ALLPAIRSSHORTESTPATH_H
#define ALLPAIRSSHORTESTPATH_H

#include "GreedyStrategy.h"

// Monster 3 Alternative: All-Pairs Shortest Path Ambush Strategy
// Uses precomputed DP table to intercept Pacman.
class AllPairsShortestPath : public GreedyStrategy {
public:
  Location findNextMove(Graph *graph, Entity *monster, Entity *target) override;
};

#endif
