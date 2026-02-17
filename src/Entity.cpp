/**
 * Base class for all moving entities in the game (Pacman and Monsters).
 *
 * This class manages the position (Location) and direction of entities.
 *
 * Time Complexity: All methods (getters/setters) are O(1).
 */
#include "../include/Entity.h"

Entity::Entity(const Location &loc) : location(loc), lastDirection(0, 0) {}

Location Entity::getLocation() const { return location; }

void Entity::setLocation(const Location &loc) { location = loc; }

Location Entity::getLastDirection() const { return lastDirection; }

void Entity::setLastDirection(const Location &dir) { lastDirection = dir; }
