#include "../include/AStarStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Manhattan distance heuristic — admissible and consistent for a 4-connected
// grid, guaranteeing A* finds the optimal (shortest) path.
int AStarStrategy::heuristic(const Location &a, const Location &b) {
  int dx = abs(a.x - b.x);
  int dy = abs(a.y - b.y);

  // Account for toroidal wrapping (tunnel on row 14)
  if (dx > Graph::WIDTH / 2) {
    dx = Graph::WIDTH - dx;
  }
  if (dy > Graph::HEIGHT / 2) {
    dy = Graph::HEIGHT - dy;
  }

  return dx + dy;
}

Location AStarStrategy::findNextMove(Graph *graph, Entity *monster,
                                     Entity *target) {
  Location startLoc = monster->getLocation();
  Location goalLoc = target->getLocation();

  // Trivial case: already at goal
  if (startLoc == goalLoc) {
    return startLoc;
  }

  // --- A* Search ---
  // Each entry in the priority queue: (fCost, Location)
  // fCost = gCost + heuristic
  using PQEntry = std::pair<int, Location>;
  std::priority_queue<PQEntry, std::vector<PQEntry>, std::greater<PQEntry>>
      openSet;

  // gCost: cheapest known cost from start to each node
  std::unordered_map<Location, int> gCost;

  // parent: for path reconstruction
  std::unordered_map<Location, Location> parent;

  // closedSet: nodes already fully evaluated
  std::unordered_set<Location> closedSet;

  // Initialise start node
  gCost[startLoc] = 0;
  int startH = heuristic(startLoc, goalLoc);
  openSet.push({startH, startLoc});

  while (!openSet.empty()) {
    auto [currentF, currentLoc] = openSet.top();
    openSet.pop();

    // Goal reached — reconstruct path and return the first step
    if (currentLoc == goalLoc) {
      // Walk back from goal to start to find the first move
      Location step = goalLoc;
      while (parent.count(step) && parent[step] != startLoc) {
        step = parent[step];
      }
      return step;
    }

    // Skip if already evaluated (duplicate in the PQ)
    if (closedSet.count(currentLoc)) {
      continue;
    }
    closedSet.insert(currentLoc);

    // Expand neighbors
    Node *currentNode = graph->getNode(currentLoc);
    if (!currentNode) {
      continue;
    }

    int currentG = gCost[currentLoc];

    for (Node *neighbor : currentNode->getNeighbors()) {
      Location neighborLoc = neighbor->getLocation();

      if (closedSet.count(neighborLoc)) {
        continue;
      }

      // Each edge has uniform cost = 1
      int tentativeG = currentG + 1;

      // Only update if this path is better than any previously found
      auto it = gCost.find(neighborLoc);
      if (it == gCost.end() || tentativeG < it->second) {
        gCost[neighborLoc] = tentativeG;
        parent[neighborLoc] = currentLoc;
        int f = tentativeG + heuristic(neighborLoc, goalLoc);
        openSet.push({f, neighborLoc});
      }
    }
  }

  // No path found (should not happen in a connected maze) — stay in place
  return startLoc;
}
