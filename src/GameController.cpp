/**
 * Controls and manages the overall flow of the Pacman game.
 * This file implements the Game Controller module, which acts as the
 * central coordination unit of the system. It ensures smooth interaction
 * between Pacman, ghosts, the maze, scoring system, and game states.
 *
 * The main responsibilities of this module include:
 *   Initialising the maze and all game entities (Pacman and ghosts).
 *   Running the main game loop and handling frame-by-frame updates.
 *   Processing player input and updating Pacman’s movement.
 *   Triggering ghost movement strategies (e.g., A* pathfinding).
 *   Detecting collisions (Pacman vs ghosts, Pacman vs pellets).
 *   Updating score and checking win/lose conditions.
 *
 * The game follows a structured update cycle:
 *   1. Read input.
 *   2. Update entity positions.
 *   3. Apply AI strategies.
 *   4. Check collisions and update score.
 *   5. Render updated state.
 *
 * Game states such as Start, Running, Pause, and Game Over
 * are managed centrally to maintain consistent gameplay behaviour.
 *
 * Time Complexity:
 *   The controller runs once per frame and updates all entities.
 *   If there are G ghosts and V grid cells, the per-frame complexity
 *   is approximately O(G + V) (excluding individual AI complexity).
 *   Overall performance is optimised for real-time execution.
 */

#include "../include/GameController.h"
#include "../include/AStarStrategy.h"
#include "../include/AggressiveGreedyStrategy.h"
#include "../include/CenterTerritorialGreedyStrategy.h"
#include "../include/DirectionalGreedyStrategy.h"
#include "../include/DistanceGreedyStrategy.h"
#include "../include/HeuristicGreedyStrategy.h"
#include "../include/PinchGreedyStrategy.h"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <limits>

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
  lightningActive = false;
  lightningTimer = 0;
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

  // Initialise pellets on all path tiles
  pellets.clear();
  for (int x = 0; x < Graph::WIDTH; ++x) {
    for (int y = 0; y < Graph::HEIGHT; ++y) {
      if (!Graph::isWall(x, y)) {
        pellets.insert(Location(x, y));
      }
    }
  }

  // Start Pacman in centre (row 28, col 28) — scaled 2× from (14,14)
  pacman = new Pacman(Location(28, 28));

  // Start Monsters in corners/house areas — scaled 2× from original positions
  monsters.push_back(
      new Monster(Location(2, 2), new AStarStrategy(), "M1 (A*)"));
  monsters.push_back(
      new Monster(Location(52, 2), new HeuristicGreedyStrategy(), "M2 (Heur)"));
  monsters.push_back(new Monster(Location(2, 58),
                                 new DirectionalGreedyStrategy(), "M3 (Dir)"));
  monsters.push_back(new Monster(Location(52, 58),
                                 new AggressiveGreedyStrategy(), "M4 (Aggr)"));
  monsters.push_back(new Monster(Location(28, 58),
                                 new PinchGreedyStrategy(monsters), "M5 (Pinch)"));

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
  lightningActive = false;
  lightningTimer = 0;
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
  // If Lightning is active, force SCATTER on all ghosts
  if (pacman) {
    if (lightningActive) {
      for (Monster *m : monsters) {
        m->setMode(Monster::SCATTER);
      }
    } else {
      // Normal behaviour: Dynamic Role Assignment
      std::vector<Monster *> sortedMonsters = monsters;
      Location pacLoc = pacman->getLocation();

      std::sort(sortedMonsters.begin(), sortedMonsters.end(),
                [pacLoc](Monster *a, Monster *b) {
                  auto distSq = [pacLoc](Location loc) {
                    int dx = loc.x - pacLoc.x;
                    int dy = loc.y - pacLoc.y;
                    // Toroidal wrapping distance
                    if (std::abs(dx) > Graph::WIDTH / 2)
                      dx = Graph::WIDTH - std::abs(dx);
                    if (std::abs(dy) > Graph::HEIGHT / 2)
                      dy = Graph::HEIGHT - std::abs(dy);
                    return dx * dx + dy * dy;
                  };
                  return distSq(a->getLocation()) < distSq(b->getLocation());
                });

      if (!sortedMonsters.empty()) {
        // Closest → Chase (fast, direct pursuit)
        // Farthest → Ambush (medium speed, predictive intercept)
        // Middle → Chase (default standard behaviour)
        for (size_t i = 0; i < sortedMonsters.size(); ++i) {
          if (i == 0) {
            sortedMonsters[i]->setMode(Monster::CHASE);
          } else if (i == sortedMonsters.size() - 1) {
            sortedMonsters[i]->setMode(Monster::AMBUSH);
          } else {
            sortedMonsters[i]->setMode(Monster::NORMAL);
          }
        }
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
                    "Ghosts→0.9x, Pacman→1.1x";
      } else {
        qDebug() << "[GhostTerritory] Pacman LEFT ghost territory. "
                    "All speeds→1.0x";
      }
      pacmanWasInsideTerritory = inside;
    }

    // Apply speed multipliers
    double ghostMul = inside ? 0.9 : 1.0;
    double pacMul = inside ? 1.1 : 1.0;
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

  if (lightningActive) {
    lightningTimer--;
    if (lightningTimer <= 0) {
      lightningActive = false;
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
  } else if (key == "Z") {
    if (score >= 100 && monsters.size() >= 2) {
      std::pair<Monster *, Monster *> closest = findClosestPair(monsters);
      if (closest.first && closest.second) {
        score -= 100;                 // Cost
        closest.first->applySlow(20); // Slow for 20 ticks
        closest.second->applySlow(20);

        lightningActive = true;
        lightningTimer = 20; // Scatter mode and visual bolt matches slow duration
        lightningStart = closest.first->getLocation();
        lightningEnd = closest.second->getLocation();
      }
    }
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
double getDistance(const Location &l1, const Location &l2) {
  long long dx = l1.x - l2.x;
  long long dy = l1.y - l2.y;
  return std::sqrt(dx * dx + dy * dy);
}

// Compare functions for sorting
bool compareX(Monster *a, Monster *b) {
  return a->getLocation().x < b->getLocation().x;
}

bool compareY(Monster *a, Monster *b) {
  return a->getLocation().y < b->getLocation().y;
}

std::pair<Monster *, Monster *> bruteForce(std::vector<Monster *> &monsters,
                                           int n) {
  double min_dist = std::numeric_limits<double>::max();
  std::pair<Monster *, Monster *> min_pair = {nullptr, nullptr};

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      double d =
          getDistance(monsters[i]->getLocation(), monsters[j]->getLocation());
      if (d < min_dist) {
        min_dist = d;
        min_pair = {monsters[i], monsters[j]};
      }
    }
  }
  return min_pair;
}

std::pair<Monster *, Monster *>
stripClosest(std::vector<Monster *> &strip, double d,
             std::pair<Monster *, Monster *> best_pair) {
  double min_dist = d;
  std::pair<Monster *, Monster *> min_pair = best_pair;

  std::sort(strip.begin(), strip.end(), compareY);

  for (size_t i = 0; i < strip.size(); ++i) {
    for (size_t j = i + 1;
         j < strip.size() &&
         (strip[j]->getLocation().y - strip[i]->getLocation().y) < min_dist;
         ++j) {
      double dist =
          getDistance(strip[i]->getLocation(), strip[j]->getLocation());
      if (dist < min_dist) {
        min_dist = dist;
        min_pair = {strip[i], strip[j]};
      }
    }
  }
  return min_pair;
}

std::pair<Monster *, Monster *> closestUtil(std::vector<Monster *> &monsters,
                                            int n) {
  if (n <= 3) {
    return bruteForce(monsters, n);
  }

  int mid = n / 2;
  Monster *midPoint = monsters[mid];

  std::vector<Monster *> leftPart(monsters.begin(), monsters.begin() + mid);
  std::vector<Monster *> rightPart(monsters.begin() + mid, monsters.end());

  std::pair<Monster *, Monster *> dl = closestUtil(leftPart, mid);
  std::pair<Monster *, Monster *> dr = closestUtil(rightPart, n - mid);

  double d1 = std::numeric_limits<double>::max();
  double d2 = std::numeric_limits<double>::max();

  if (dl.first && dl.second)
    d1 = getDistance(dl.first->getLocation(), dl.second->getLocation());
  if (dr.first && dr.second)
    d2 = getDistance(dr.first->getLocation(), dr.second->getLocation());

  double d = std::min(d1, d2);
  std::pair<Monster *, Monster *> min_pair = (d1 < d2) ? dl : dr;
  if (!dl.first)
    min_pair = dr;
  else if (!dr.first)
    min_pair = dl;

  std::vector<Monster *> strip;
  for (int i = 0; i < n; i++) {
    if (std::abs(monsters[i]->getLocation().x - midPoint->getLocation().x) <
        d) {
      strip.push_back(monsters[i]);
    }
  }

  return stripClosest(strip, d, min_pair);
}

std::pair<Monster *, Monster *>
GameController::findClosestPair(const std::vector<Monster *> &monsters) {
  if (monsters.size() < 2)
    return {nullptr, nullptr};

  std::vector<Monster *> sortedMonsters = monsters;
  std::sort(sortedMonsters.begin(), sortedMonsters.end(), compareX);

  return closestUtil(sortedMonsters, sortedMonsters.size());
}
