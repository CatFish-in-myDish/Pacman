#include "Monster.h"

Monster::Monster(const Location& loc, GreedyStrategy* strat, const std::string& monsterName)
    : Entity(loc), strategy(strat), name(monsterName) {}

Monster::~Monster() {
    delete strategy;
}

void Monster::move(Graph* graph, Entity* target) {
    Location nextLoc = strategy->findNextMove(graph, this, target);
    setLocation(nextLoc);
}

std::string Monster::getName() const {
    return name;
}
