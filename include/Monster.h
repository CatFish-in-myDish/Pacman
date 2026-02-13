#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include "Graph.h"
#include "GreedyStrategy.h"
#include <string>

class Monster : public Entity {
public:
  enum Mode {
    CHASE,
    AMBUSH,
    SCATTER
  };

  Monster(const Location &loc, GreedyStrategy *strat,
          const std::string &monsterName);
  ~Monster();

  void move(Graph *graph, Entity *target);
  std::string getName() const;

  void setMode(Mode mode);
  Mode getMode() const;

  void setTerritoryMultiplier(double m);
  double getTerritoryMultiplier() const;

private:
  GreedyStrategy *strategy;
  std::string name;
  Mode currentMode;
  double speed;
  double moveAccumulator;
  double territoryMultiplier;

};
#endif
