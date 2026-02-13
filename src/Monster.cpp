#include "../include/Monster.h"

#include "../include/AggressiveGreedyStrategy.h"
#include "../include/DistanceGreedyStrategy.h"
#include "../include/ScatterStrategy.h"

Monster::Monster(const Location &loc, GreedyStrategy *strat,
                 const std::string &monsterName)
    : Entity(loc), strategy(strat), name(monsterName), currentMode(CHASE), speed(1.0), moveAccumulator(0.0), slowTicks(0), territoryMultiplier(1.0) {}

Monster::~Monster() { delete strategy; }

void Monster::move(Graph *graph, Entity *target) {
  moveAccumulator += speed * territoryMultiplier;
  if (slowTicks > 0) {
    slowTicks--;
    if (slowTicks % 2 != 0) {
      return; // Skip move every other tick when slowed
    }
  }

  Location nextLoc = strategy->findNextMove(graph, this, target);
  setLocation(nextLoc);
  moveAccumulator += speed;
  while (moveAccumulator >= 1.0) {
    Location nextLoc = strategy->findNextMove(graph, this, target);
    setLocation(nextLoc);
    moveAccumulator -= 1.0;
  }
}

std::string Monster::getName() const { return name; }

void Monster::applySlow(int ticks) { slowTicks = ticks; }

bool Monster::isSlowed() const { return slowTicks > 0; }
void Monster::setMode(Mode mode) {
  if (currentMode == mode) {
    return;
  }

  GreedyStrategy* newStrategy = nullptr;

  switch (mode) {
    case CHASE:
      newStrategy = new DistanceGreedyStrategy();
      speed = 1.5;
      break;
    case AMBUSH:
      newStrategy = new AggressiveGreedyStrategy();
      speed = 1.25;
      break;
    case SCATTER:
      newStrategy = new ScatterStrategy();
      speed = 1.0;
      break;
  }

  if (newStrategy) {
    delete strategy;
    strategy = newStrategy;
    currentMode = mode;
  }
}

Monster::Mode Monster::getMode() const { return currentMode; }

void Monster::setTerritoryMultiplier(double m) { territoryMultiplier = m; }

double Monster::getTerritoryMultiplier() const { return territoryMultiplier; }
