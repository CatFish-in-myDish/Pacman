/*
Implements the A* pathfinding algorithm to find the optimal path to the target.
This file implements the A* (A-Star) pathfinding algorithm for the Smart Ghost
in the Pacman game. The goal of this module is to enable intelligent and optimal
movement of the ghost toward Pacman by computing the shortest path in the maze.

Unlike a greedy approach that simply moves in the direction of Pacman, A*
evaluates both: g(n) → actual cost from the ghost's position to the current node
h(n) → estimated cost from the current node to Pacman (using Manhattan distance)
The total cost is calculated as: f(n) = g(n) + h(n)

The maze is treated as a grid where:
Each cell is a node.
Valid neighbouring cells represent possible moves.
Walls are treated as blocked nodes.

The algorithm uses:
A priority queue (open list) to select the node with the lowest f-cost.
A closed list to avoid revisiting explored nodes.
Parent tracking to reconstruct the shortest path once Pacman's position is
ched.

Time Complexity: O(E log V), where V is the number of vertices (nodes) and E is
the number of edges in the graph. In the worst case for a grid, this can be
proportional to the number of cells (N * M).
*/

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

    // Expand neighbours
    Node *currentNode = graph->getNode(currentLoc);
    if (!currentNode) {
      continue;
    }

    int currentG = gCost[currentLoc];

    for (Node *neighbour : currentNode->getNeighbours()) {
      Location neighbourLoc = neighbour->getLocation();

      if (closedSet.count(neighbourLoc)) {
        continue;
      }

      // Each edge has uniform cost = 1
      int tentativeG = currentG + 1;

      // Only update if this path is better than any previously found
      auto it = gCost.find(neighbourLoc);
      if (it == gCost.end() || tentativeG < it->second) {
        gCost[neighbourLoc] = tentativeG;
        parent[neighbourLoc] = currentLoc;
        int f = tentativeG + heuristic(neighbourLoc, goalLoc);
        openSet.push({f, neighbourLoc});
      }
    }
  }

  // No path found (should not happen in a connected maze) — stay in place
  return startLoc;
}
