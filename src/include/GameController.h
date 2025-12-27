#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Graph.h"
#include "Pacman.h"
#include "Monster.h"
#include <vector>
#include <QString>
#include <chrono>

class GameController {
private:
    Graph* graph;
    Pacman* pacman;
    std::vector<Monster*> monsters;
    bool running;
    std::chrono::steady_clock::time_point startTime;
    double survivedTime;
    bool gameOver;

    void movePacman();
    void moveMonsters();
    void checkCollisions();

public:
    GameController();
    ~GameController();

    void initGame();
    void startGame();
    void update();
    void handleInput(const QString& key);

    Pacman* getPacman() const;
    const std::vector<Monster*>& getMonsters() const;
    bool isGameOver() const;
    double getSurvivedTime() const;
};

#endif 