#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Graph.h"
#include "Monster.h"
#include "Pacman.h"
#include <QString>
#include <chrono>
#include <vector>
#include <unordered_set>

class GameController {
private:
  Graph *graph;
  Pacman *pacman;
  std::vector<Monster *> monsters;
  std::unordered_set<Location> pellets; // Pellet locations on the map
  bool running;
  std::chrono::steady_clock::time_point startTime;
  double survivedTime;
  int score;
  int round;
  std::chrono::steady_clock::time_point winTime;
  bool gameWon;
  bool gameOver;

  void movePacman();
  void moveMonsters();
  void checkCollisions();

public:
  GameController();
  ~GameController();

  void initGame(bool resetScore = true);
  void startGame();
  void update();
  void handleInput(const QString &key);

  int getRound() const;

  Pacman *getPacman() const;
  const std::vector<Monster *> &getMonsters() const;
  const std::unordered_set<Location> &getPellets() const; // Locations of remaining pellets
  int getScore() const;
  bool isGameWon() const;
  bool isGameOver() const;
  double getSurvivedTime() const;
};

#endif
