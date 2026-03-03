#ifndef CENTERTERRITORIALGREEDYSTRATEGY_H
#define CENTERTERRITORIALGREEDYSTRATEGY_H

#include "GreedyStrategy.h"

// Monster 5: Center Territorial Guard – patrols the maze center and chases
// Pacman only when he enters a small territory around that center.
class CenterTerritorialGreedyStrategy : public GreedyStrategy {
public:
  Location findNextMove(Graph *graph, Entity *monster, Entity *target) override;

private:
  static const int TERRITORY_RADIUS = 6;
  static constexpr int CENTER_X = 27;
  static constexpr int CENTER_Y = 28;
};

#endif
