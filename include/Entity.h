#ifndef ENTITY_H
#define ENTITY_H

#include "Location.h"

// Abstract base class for movable game objects
class Entity {
protected:
  Location location;
  Location lastDirection;

public:
  Entity(const Location &loc);
  virtual ~Entity() = default;

  Location getLocation() const;
  void setLocation(const Location &loc);

  Location getLastDirection() const;
  void setLastDirection(const Location &dir);
};

#endif
