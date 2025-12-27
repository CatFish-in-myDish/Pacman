#include "../include/GameController.h"
#include "../include/AggressiveGreedyStrategy.h"
#include "../include/DirectionalGreedyStrategy.h"
#include "../include/DistanceGreedyStrategy.h"
#include "../include/HeuristicGreedyStrategy.h"

GameController::GameController() {
  graph = nullptr;
  pacman = nullptr;
  running = false;
  gameOver = false;
  survivedTime = 0.0;
  initGame();
}

GameController::~GameController() {
  delete graph;
  delete pacman;
  for (Monster *monster : monsters) {
    delete monster;
  }
}

void GameController::initGame() {
  // Clean up existing game state
  delete graph;
  delete pacman;
  for (Monster *monster : monsters) {
    delete monster;
  }
  monsters.clear();

  // Create new game state
  graph = new Graph();

  // Start Pacman in center (row 14, col 14)
  pacman = new Pacman(Location(14, 14));

  // Start Monsters in corners/house areas
  monsters.push_back(
      new Monster(Location(1, 1), new DistanceGreedyStrategy(), "M1 (Dist)"));
  monsters.push_back(new Monster(Location(26, 1),
                                 new HeuristicGreedyStrategy(), "M2 (Heur)"));
  monsters.push_back(new Monster(Location(1, 29),
                                 new DirectionalGreedyStrategy(), "M3 (Dir)"));
  monsters.push_back(new Monster(Location(26, 29),
                                 new AggressiveGreedyStrategy(), "M4 (Aggr)"));

  running = false;
  gameOver = false;
  survivedTime = 0.0;
}

void GameController::startGame() {
  running = true;
  startTime = std::chrono::steady_clock::now();
}

void GameController::update() {
  if (!running || gameOver) {
    return;
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
      return; // Stop.
  }

  pacman->setLocation(Location(nextX, nextY));
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
    initGame();
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

bool GameController::isGameOver() const { return gameOver; }

double GameController::getSurvivedTime() const { return survivedTime; }
