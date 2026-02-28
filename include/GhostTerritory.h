#ifndef GHOSTTERRITORY_H
#define GHOSTTERRITORY_H

#include "Location.h"
#include <vector>

// Computes the Convex Hull of ghost positions using QuickHull (Divide &
// Conquer) and checks whether Pacman lies inside the "Ghost Territory."
class GhostTerritory {
public:
  GhostTerritory();

  // Recompute the convex hull from ghost positions and test Pacman inclusion.
  // Should be called each frame after ghosts move.
  void update(const std::vector<Location> &ghostPositions,
              const Location &pacmanPos);

  // Whether Pacman is currently inside the ghost territory hull.
  bool isPacmanInside() const;

  // Get the last computed hull (for debugging/testing).
  const std::vector<Location> &getHull() const;

private:
  std::vector<Location> hull;
  bool pacmanInside;

  // QuickHull algorithm — Divide & Conquer, O(N log N) average.
  static std::vector<Location> quickHull(const std::vector<Location> &points);

  // Recursive helper: finds points above line (A→B) and builds that side of
  // hull.
  static void quickHullRecurse(const std::vector<Location> &points,
                               const Location &A, const Location &B,
                               std::vector<Location> &hullOut);

  // Cross product of vectors (O→A) and (O→B).
  // Positive = counter-clockwise, Negative = clockwise, 0 = collinear.
  static long long cross(const Location &O, const Location &A,
                         const Location &B);

  // Point-in-convex-polygon test using cross-product winding.
  static bool isInsideConvexPolygon(const std::vector<Location> &polygon,
                                    const Location &point);
};

#endif
