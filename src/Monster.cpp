/**
 * Represents a ghost/monster entity in the game.
 *
 * This class handles monster movement, strategy switching (Chase, Ambush, Scatter),
 * speed management, and special effects like being slowed or territory buffs.
 *
 * Time Complexity:
 * - move(): O(1) assuming the strategy's findNextMove is efficient.
 * - setMode(): O(1).
 */
#include "../include/Monster.h"
#include <algorithm>

#include "../include/AStarStrategy.h"
#include "../include/AggressiveGreedyStrategy.h"
#include "../include/ScatterStrategy.h"

Monster::Monster(const Location &loc, GreedyStrategy *strat,
                 const std::string &monsterName)
    : Entity(loc), strategy(strat), originalStrategy(strat), name(monsterName),
      currentMode(CHASE), speed(1.0), moveAccumulator(0.0), slowTicks(0),
      territoryMultiplier(1.0) {}

Monster::~Monster() {
  if (strategy != originalStrategy) {
    delete strategy;
  }
  delete originalStrategy;
}

void Monster::move(Graph *graph, Entity *target) {
  // Cap effective speed at 1.5
  double effectiveSpeed = std::min(speed * territoryMultiplier, 1.5);
  moveAccumulator += effectiveSpeed;

  if (slowTicks > 0) {
    slowTicks--;
    if (slowTicks % 2 != 0) {
      return; // Skip move every other tick when slowed
    }
  }

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

  // Clean up temporary strategy if it's not the original
  if (strategy != originalStrategy) {
    delete strategy;
    strategy = originalStrategy;
  }

  switch (mode) {
    case CHASE:
      // Nearest monsters: use AstarStrategy  (direct pursuit)
      strategy = new AStarStrategy();
      speed = 1.5;
      break;
    case AMBUSH:
      // Farthest monsters: use AggressiveGreedyStrategy (predictive intercept)
      strategy = new AggressiveGreedyStrategy();
      speed = 1.25;
      break;
    case SCATTER:
      // Used during lightning
      strategy = new ScatterStrategy();
      speed = 1.0;
      break;
  }

  currentMode = mode;
}

Monster::Mode Monster::getMode() const { return currentMode; }

void Monster::setTerritoryMultiplier(double m) { territoryMultiplier = m; }

double Monster::getTerritoryMultiplier() const { return territoryMultiplier; }
