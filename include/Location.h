#ifndef LOCATION_H
#define LOCATION_H

#include <functional>

// Immutable struct representing (x, y) coordinates
struct Location {
  int x;
  int y;

  Location() : x(0), y(0) {}
  Location(int x, int y) : x(x), y(y) {}

  bool operator==(const Location &other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Location &other) const { return !(*this == other); }
};

// Hash function for Location to use in unordered_map
namespace std {
template <> struct hash<Location> {
  size_t operator()(const Location &loc) const {
    return hash<int>()(loc.x) ^ (hash<int>()(loc.y) << 1);
  }
};
} // namespace std

#endif
