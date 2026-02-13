#include "../include/GameController.h"
#include "../include/AStarStrategy.h"
#include "../include/AggressiveGreedyStrategy.h"
#include "../include/DirectionalGreedyStrategy.h"
#include "../include/DistanceGreedyStrategy.h"
#include "../include/HeuristicGreedyStrategy.h"
#include <algorithm>
#include <cmath>
#include <QDebug>
GameController::GameController() {
  graph = nullptr;
  pacman = nullptr;
  ghostTerritory = nullptr;
  running = false;
  gameOver = false;
  survivedTime = 0.0;
  score = 0;
  gameWon = false;
  round = 1;
  pacmanWasInsideTerritory = false;
  winTime = std::chrono::steady_clock::now();
  initGame(true);
}

GameController::~GameController() {
  delete graph;
  delete pacman;
  delete ghostTerritory;
  for (Monster *monster : monsters) {
    delete monster;
  }
}

void GameController::initGame(bool resetScore) {
  // Clean up existing game state
  delete graph;
  delete pacman;
  delete ghostTerritory;
  for (Monster *monster : monsters) {
    delete monster;
  }
  monsters.clear();

  // Create new game state
  graph = new Graph();
  ghostTerritory = new GhostTerritory();
  pacmanWasInsideTerritory = false;

  // Initialize pellets on all path tiles
  pellets.clear();
  for (int x = 0; x < Graph::WIDTH; ++x) {
    for (int y = 0; y < Graph::HEIGHT; ++y) {
      if (!Graph::isWall(x, y)) {
        pellets.insert(Location(x, y));
      }
    }
  }

  // Start Pacman in center (row 14, col 14)
  pacman = new Pacman(Location(14, 14));

  // Start Monsters in corners/house areas
  monsters.push_back(
      new Monster(Location(1, 1), new AStarStrategy(), "M1 (A*)"));
  monsters.push_back(
      new Monster(Location(26, 1), new HeuristicGreedyStrategy(), "M2 (Heur)"));
  monsters.push_back(new Monster(Location(1, 29),
                                 new DirectionalGreedyStrategy(), "M3 (Dir)"));
  monsters.push_back(new Monster(Location(26, 29),
                                 new AggressiveGreedyStrategy(), "M4 (Aggr)"));

  // Remove pellets that are under initial entities (Pacman and Monsters)
  pellets.erase(pacman->getLocation());
  for (Monster *m : monsters) {
    pellets.erase(m->getLocation());
  }

  // Reset or advance score/round/win state
  if (resetScore) {
    score = 0;
    round = 1;
  } else {
    ++round;
  }
  gameWon = false;

  running = false;
  gameOver = false;
  survivedTime = 0.0;
}

void GameController::startGame() {
  running = true;
  startTime = std::chrono::steady_clock::now();
}

void GameController::update() {
  // If game is paused because of win, allow auto-advance after 2 seconds
  if (gameWon) {
    auto now = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration<double>(now - winTime).count();
    if (dt > 2.0) {
      // Start next round automatically
      initGame(false);
      startGame();
    }
    return;
  }

  // Dynamic Ghost Aggression: Assign roles based on distance
  if (pacman) {
    std::vector<Monster *> sortedMonsters = monsters;
    Location pacLoc = pacman->getLocation();

    std::sort(sortedMonsters.begin(), sortedMonsters.end(),
              [pacLoc](Monster *a, Monster *b) {
                auto distSq = [pacLoc](Location loc) {
                  int dx = loc.x - pacLoc.x;
                  int dy = loc.y - pacLoc.y;
                  // Toroidal wrapping distance
                  if (std::abs(dx) > Graph::WIDTH / 2) dx = Graph::WIDTH - std::abs(dx);
                  if (std::abs(dy) > Graph::HEIGHT / 2) dy = Graph::HEIGHT - std::abs(dy);
                  return dx * dx + dy * dy;
                };
                return distSq(a->getLocation()) < distSq(b->getLocation());
              });

    if (!sortedMonsters.empty()) {
      // Closest -> Chase
      sortedMonsters[0]->setMode(Monster::CHASE);

      // Farthest -> Ambush (if more than 1 ghost)
      if (sortedMonsters.size() > 1) {
        sortedMonsters.back()->setMode(Monster::AMBUSH);
      }

      // Others -> Scatter
      for (size_t i = 1; i < sortedMonsters.size() - 1; ++i) {
        sortedMonsters[i]->setMode(Monster::SCATTER);
      }
    }
  }

  // ── Ghost Territory: recompute convex hull and apply speed modifiers ──
  if (pacman && monsters.size() >= 3) {
    std::vector<Location> ghostPositions;
    ghostPositions.reserve(monsters.size());
    for (Monster *m : monsters) {
      ghostPositions.push_back(m->getLocation());
    }
    ghostTerritory->update(ghostPositions, pacman->getLocation());

    bool inside = ghostTerritory->isPacmanInside();

    // Log state change
    if (inside != pacmanWasInsideTerritory) {
      if (inside) {
        qDebug() << "[GhostTerritory] Pacman ENTERED ghost territory! "
                    "Ghosts→1.2x, Pacman→1.25x";
      } else {
        qDebug() << "[GhostTerritory] Pacman LEFT ghost territory. "
                    "All speeds→1.0x";
      }
      pacmanWasInsideTerritory = inside;
    }

    // Apply speed multipliers
    double ghostMul = inside ? 1.2 : 1.0;
    double pacMul   = inside ? 1.25 : 1.0;
    for (Monster *m : monsters) {
      m->setTerritoryMultiplier(ghostMul);
    }
    pacman->setSpeed(pacMul);
  } else {
    // Fewer than 3 ghosts → no territory effect
    for (Monster *m : monsters) {
      m->setTerritoryMultiplier(1.0);
    }
    if (pacman) {
      pacman->setSpeed(1.0);
    }
  }


  // Move Pacman
  movePacman();

  // Move Monsters
  moveMonsters();

  // Check Collisions
  checkCollisions();

  // Update Time
  auto currentTime = std::chrono::steady_clock::now();
  survivedTime = std::chrono::duration<double>(currentTime - startTime).count();
}

void GameController::movePacman() {
  Location dir = pacman->getLastDirection();
  if (dir.x == 0 && dir.y == 0) {
    return;
  }

  pacman->addMoveAccumulator(pacman->getSpeed());

  while (pacman->getMoveAccumulator() >= 1.0) {
    Location current = pacman->getLocation();
    int nextX = current.x + dir.x;
    int nextY = current.y + dir.y;

    // Handle toroidal wrapping
    if (nextX < 0)
      nextX = Graph::WIDTH - 1;
    if (nextX >= Graph::WIDTH)
      nextX = 0;
    if (nextY < 0)
      nextY = Graph::HEIGHT - 1;
    if (nextY >= Graph::HEIGHT)
      nextY = 0;

    // Check if Wall
    if (Graph::isWall(nextX, nextY)) {
      pacman->resetMoveStep();
      break;
    }

    pacman->setLocation(Location(nextX, nextY));
    pacman->resetMoveStep();

    // Eat pellet if present at new location
    Location newLoc = pacman->getLocation();
    if (pellets.erase(newLoc) > 0) {
      // Pellet consumed - increment score
      score += 10;
      // Win if no pellets remain
      if (pellets.empty()) {
        gameWon = true;
        running = false;
        winTime = std::chrono::steady_clock::now();
      }
    }
  }
}

void GameController::moveMonsters() {
  for (Monster *monster : monsters) {
    monster->move(graph, pacman);
  }
}

void GameController::checkCollisions() {
  for (Monster *monster : monsters) {
    if (monster->getLocation() == pacman->getLocation()) {
      gameOver = true;
      running = false;
      break;
    }
  }
}

void GameController::handleInput(const QString &key) {
  if (gameOver && key == "R") {
    initGame(true);
    startGame();
    return;
  }

  if (gameWon && key == "R") {
    initGame(false);
    startGame();
    return;
  }

  // If game not started, start on first move
  if (!running && !gameOver) {
    startGame();
  }

  if (key == "UP") {
    pacman->setLastDirection(Location(0, -1));
  } else if (key == "DOWN") {
    pacman->setLastDirection(Location(0, 1));
  } else if (key == "LEFT") {
    pacman->setLastDirection(Location(-1, 0));
  } else if (key == "RIGHT") {
    pacman->setLastDirection(Location(1, 0));
  }
}

Pacman *GameController::getPacman() const { return pacman; }

const std::vector<Monster *> &GameController::getMonsters() const {
  return monsters;
}

const std::unordered_set<Location> &GameController::getPellets() const {
  return pellets;
}

int GameController::getScore() const { return score; }

bool GameController::isGameWon() const { return gameWon; }

int GameController::getRound() const { return round; }

bool GameController::isGameOver() const { return gameOver; }

double GameController::getSurvivedTime() const { return survivedTime; }
