#include "../include/Monster.h"

Monster::Monster(const Location &loc, GreedyStrategy *strat,
                 const std::string &monsterName)
    : Entity(loc), strategy(strat), name(monsterName), slowTicks(0) {}

Monster::~Monster() { delete strategy; }

void Monster::move(Graph *graph, Entity *target) {
  if (slowTicks > 0) {
    slowTicks--;
    if (slowTicks % 2 != 0) {
      return; // Skip move every other tick when slowed
    }
  }

  Location nextLoc = strategy->findNextMove(graph, this, target);
  setLocation(nextLoc);
}

std::string Monster::getName() const { return name; }

void Monster::applySlow(int ticks) { slowTicks = ticks; }

bool Monster::isSlowed() const { return slowTicks > 0; }
