/**
 * Implements the "Ghost Territory" mechanic using Computational Geometry.
 *
 * This module dynamically forms a territory around all ghosts by computing
 * the Convex Hull of their current positions using the QuickHull algorithm.
 * The convex hull represents the smallest convex polygon that encloses
 * all ghost positions.
 *
 * After constructing the hull, the system checks whether Pacman lies
 * inside this convex polygon using orientation and cross-product tests.
 * If Pacman is inside the hull, it indicates that he is surrounded
 * or trapped within the ghost territory.
 *
 * Core Concepts Used:
 *   - QuickHull Algorithm for Convex Hull construction.
 *   - Cross Product to determine point orientation.
 *   - Point-in-Convex-Polygon test for containment check.
 *
 * Time Complexity:
 *   - quickHull(): Average O(N log N), Worst Case O(N²),
 *                  where N is the number of ghosts.
 *   - isInsideConvexPolygon(): O(K),
 *                  where K is the number of hull vertices.
 *
 * Since the number of ghosts is small in gameplay,
 * this computation remains efficient for real-time execution.
 */

#include "../include/GhostTerritory.h"
#include <algorithm>
#include <cmath>
#include <set>

GhostTerritory::GhostTerritory() : pacmanInside(false) {}

// ─── Cross product of vectors (O→A) × (O→B) ────────────────────────────────
long long GhostTerritory::cross(const Location &O, const Location &A,
                                const Location &B) {
  return (long long)(A.x - O.x) * (B.y - O.y) -
         (long long)(A.y - O.y) * (B.x - O.x);
}

// ─── QuickHull — Divide & Conquer ──────────────────────────────────────────
std::vector<Location>
GhostTerritory::quickHull(const std::vector<Location> &points) {
  // Need at least 3 distinct, non-collinear points for a hull polygon.
  if (points.size() < 3) {
    return {};
  }

  // Deduplicate
  std::set<Location> uniqueSet(points.begin(), points.end());
  std::vector<Location> unique(uniqueSet.begin(), uniqueSet.end());

  if (unique.size() < 3) {
    return {};
  }

  // Find leftmost and rightmost points
  auto minIt = std::min_element(
      unique.begin(), unique.end(), [](const Location &a, const Location &b) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
      });
  auto maxIt = std::max_element(
      unique.begin(), unique.end(), [](const Location &a, const Location &b) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
      });

  Location A = *minIt;
  Location B = *maxIt;

  // Partition into upper and lower sets (above and below line A→B)
  std::vector<Location> upper, lower;
  for (const Location &p : unique) {
    if (p == A || p == B)
      continue;
    long long cp = cross(A, B, p);
    if (cp > 0) {
      upper.push_back(p);
    } else if (cp < 0) {
      lower.push_back(p);
    }
    // cp == 0 → collinear, skip (not part of hull boundary)
  }

  // Build hull: upper side (A→B) then lower side (B→A)
  std::vector<Location> hullResult;
  hullResult.push_back(A);

  quickHullRecurse(upper, A, B, hullResult);

  hullResult.push_back(B);

  quickHullRecurse(lower, B, A, hullResult);

  // If all points were collinear, the hull degenerates
  if (hullResult.size() < 3) {
    return {};
  }

  return hullResult;
}

// ─── Recursive QuickHull helper ─────────────────────────────────────────────
void GhostTerritory::quickHullRecurse(const std::vector<Location> &points,
                                      const Location &A, const Location &B,
                                      std::vector<Location> &hullOut) {
  if (points.empty()) {
    return;
  }

  // Find the point farthest from line A→B (on the positive/left side)
  long long maxDist = 0;
  const Location *farthest = nullptr;
  for (const Location &p : points) {
    long long d = cross(A, B, p);
    if (d > maxDist) {
      maxDist = d;
      farthest = &p;
    }
  }

  if (!farthest) {
    return; // All remaining points are collinear with A→B
  }

  Location C = *farthest;

  // Partition: points left of A→C go to set1, points left of C→B go to set2
  std::vector<Location> set1, set2;
  for (const Location &p : points) {
    if (p == C)
      continue;
    if (cross(A, C, p) > 0) {
      set1.push_back(p);
    } else if (cross(C, B, p) > 0) {
      set2.push_back(p);
    }
    // Points inside triangle ACB are discarded
  }

  // Recurse on A→C side, then add C, then recurse on C→B side
  quickHullRecurse(set1, A, C, hullOut);
  hullOut.push_back(C);
  quickHullRecurse(set2, C, B, hullOut);
}

// ─── Point-in-convex-polygon (cross-product method) ─────────────────────────
bool GhostTerritory::isInsideConvexPolygon(const std::vector<Location> &polygon,
                                           const Location &point) {
  int n = static_cast<int>(polygon.size());
  if (n < 3) {
    return false;
  }

  // Check that the point is strictly on the same side of every edge.
  // Works regardless of winding order (CW or CCW).
  // First edge determines expected sign; all must match and be non-zero.
  long long firstCross = cross(polygon[0], polygon[1], point);
  if (firstCross == 0) {
    return false; // On edge/vertex
  }
  bool expectPositive = (firstCross > 0);

  for (int i = 1; i < n; ++i) {
    const Location &a = polygon[i];
    const Location &b = polygon[(i + 1) % n];
    long long cp = cross(a, b, point);
    if (cp == 0) {
      return false; // On boundary
    }
    if ((cp > 0) != expectPositive) {
      return false; // Different side → outside
    }
  }
  return true;
}

// ─── Public API ─────────────────────────────────────────────────────────────
void GhostTerritory::update(const std::vector<Location> &ghostPositions,
                            const Location &pacmanPos) {
  hull = quickHull(ghostPositions);

  if (hull.size() < 3) {
    pacmanInside = false;
    return;
  }

  pacmanInside = isInsideConvexPolygon(hull, pacmanPos);
}

bool GhostTerritory::isPacmanInside() const { return pacmanInside; }

const std::vector<Location> &GhostTerritory::getHull() const { return hull; }
