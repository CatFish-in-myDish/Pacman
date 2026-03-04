#ifndef ALLPAIRSHORTESTPATH_H
#define ALLPAIRSHORTESTPATH_H

#include "GreedyStrategy.h"

// Monster 3 Alternative: All-Pairs Shortest Path Ambush Strategy
// Uses precomputed DP table to intercept Pacman.
class AllPairShortestPath : public GreedyStrategy {
public:
  Location findNextMove(Graph *graph, Entity *monster, Entity *target) override;
};

#endif
