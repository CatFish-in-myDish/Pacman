#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include "Graph.h"
#include "GreedyStrategy.h"
#include <deque>
#include <string>

class Monster : public Entity {
public:
  enum Mode { NORMAL, CHASE, AMBUSH, SCATTER, FRIGHTENED };

  static const int MAX_HISTORY = 15;

  Monster(const Location &loc, GreedyStrategy *strat,
          const std::string &monsterName);
  ~Monster();

  void move(Graph *graph, Entity *target);
  std::string getName() const;

  void applySlow(int ticks);
  bool isSlowed() const;

  void setMode(Mode mode);
  Mode getMode() const;

  void setTerritoryMultiplier(double m);
  double getTerritoryMultiplier() const;

  // Backtracking / Frightened mode
  void recordPosition();
  Location popLastPosition();
  bool hasHistory() const;
  void clearHistory();
  void applyFrightened(int ticks);
  bool isFrightened() const;
  Location getSpawnLocation() const;

private:
  GreedyStrategy *strategy;
  GreedyStrategy *originalStrategy;
  std::string name;
  Mode currentMode;
  double speed;
  double moveAccumulator;
  double territoryMultiplier;
  int slowTicks;

  // Backtracking state
  std::deque<Location> pathHistory;
  int frightenedTicks;
  Location spawnLocation;
};
#endif
