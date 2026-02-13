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

  void applySlow(int ticks);
  bool isSlowed() const;

private:
  int slowTicks;
};

  void setMode(Mode mode);
  Mode getMode() const;

private:
  GreedyStrategy *strategy;
  std::string name;
  Mode currentMode;
  double speed;
  double moveAccumulator;

};
#endif
