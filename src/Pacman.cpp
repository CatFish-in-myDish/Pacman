#include "../include/Pacman.h"

Pacman::Pacman(const Location &loc)
    : Entity(loc), speed(1.0), moveAccumulator(0.0) {}

void Pacman::setSpeed(double s) { speed = s; }

double Pacman::getSpeed() const { return speed; }

double Pacman::getMoveAccumulator() const { return moveAccumulator; }

void Pacman::addMoveAccumulator(double v) { moveAccumulator += v; }

void Pacman::resetMoveStep() { moveAccumulator -= 1.0; }
