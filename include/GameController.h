#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GhostTerritory.h"
#include "Graph.h"
#include "Monster.h"
#include "Pacman.h"
#include <QString>
#include <chrono>
#include <unordered_set>
#include <vector>

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
  GhostTerritory *ghostTerritory;
  bool pacmanWasInsideTerritory; // for debug logging state changes

  // Chain Lightning State
  bool lightningActive;
  int lightningTimer;
  Location lightningStart;
  Location lightningEnd;

  std::pair<Monster *, Monster *>
  findClosestPair(const std::vector<Monster *> &monsters);

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
  const std::unordered_set<Location> &
  getPellets() const; // Locations of remaining pellets
  int getScore() const;
  bool isGameWon() const;
  bool isGameOver() const;
  double getSurvivedTime() const;

  bool isLightningActive() const { return lightningActive; }
  std::pair<Location, Location> getLightningArc() const {
    return {lightningStart, lightningEnd};
  }
};

#endif
