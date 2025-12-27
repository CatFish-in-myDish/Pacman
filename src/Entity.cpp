#include "../include/Entity.h"

Entity::Entity(const Location &loc) : location(loc), lastDirection(0, 0) {}

Location Entity::getLocation() const { return location; }

void Entity::setLocation(const Location &loc) { location = loc; }

Location Entity::getLastDirection() const { return lastDirection; }

void Entity::setLastDirection(const Location &dir) { lastDirection = dir; }
