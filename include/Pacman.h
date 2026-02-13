#ifndef PACMAN_H
#define PACMAN_H

#include "Entity.h"

class Pacman : public Entity {
public:
  Pacman(const Location &loc);

  void setSpeed(double s);
  double getSpeed() const;
  double getMoveAccumulator() const;
  void addMoveAccumulator(double v);
  void resetMoveStep();

private:
  double speed;
  double moveAccumulator;
};

#endif
