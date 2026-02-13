#ifndef ASTARSTRATEGY_H
#define ASTARSTRATEGY_H

#include "GreedyStrategy.h"

// Monster 1: A* Pathfinding - uses Manhattan distance heuristic with
// min-heap priority queue to find the guaranteed shortest path to Pacman.
// Replaces the basic Euclidean distance greedy approach.
class AStarStrategy : public GreedyStrategy {
public:
  Location findNextMove(Graph *graph, Entity *monster, Entity *target) override;

private:
  // Manhattan distance heuristic (admissible for 4-connected grid)
  static int heuristic(const Location &a, const Location &b);
};

#endif
