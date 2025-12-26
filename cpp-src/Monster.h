#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include "GreedyStrategy.h"
#include "Graph.h"
#include <string>

class Monster : public Entity {
private:
    GreedyStrategy* strategy;
    std::string name;

public:
    Monster(const Location& loc, GreedyStrategy* strat, const std::string& monsterName);
    ~Monster();

    void move(Graph* graph, Entity* target);
    std::string getName() const;
};

#endif // MONSTER_H
