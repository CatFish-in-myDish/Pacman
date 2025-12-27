#include "../include/DirectionalGreedyStrategy.h"
#include "../include/Entity.h"
#include "../include/Graph.h"
#include <cmath>
#include <limits>

Location DirectionalGreedyStrategy::findNextMove(Graph *graph, Entity *monster,
                                                 Entity *target) {
  Location currentLoc = monster->getLocation();
  Location targetLoc = target->getLocation();

  Node *currentNode = graph->getNode(currentLoc);
  const auto &neighbors = currentNode->getNeighbors();

  // If no neighbors (trap?), stay put
  if (neighbors.empty()) {
      return currentLoc;
  }

  Location bestMove = currentLoc;
  double minScore = std::numeric_limits<double>::max();

  for (Node *neighbor : neighbors) {
    Location nextLoc = neighbor->getLocation();
    
    // Calculate differences
    int dx = targetLoc.x - nextLoc.x;
    int dy = targetLoc.y - nextLoc.y;
    
    // Ideal direction analysis
    // We want to minimize the distance on the "largest" axis first
    // But since we are constrained to neighbors, we just pick the neighbor
    // that minimizes the distance to target, prioritizing axes?
    // The original strategy was: Look at dx, dy. If |dx| > |dy|, move X. 
    // We can simulate this by 'scoring' neighbors based on how well they align with the major axis.
    
    // Simpler approach that keeps the spirit: 
    // Minimize Chebyshev distance? Or just prioritize the axis with larger delta.
    
    // Let's implement the Priority Logic:
    // 1. Determine major axis from Current to Target
    int majorDx = targetLoc.x - currentLoc.x;
    int majorDy = targetLoc.y - currentLoc.y;
    
    // Wrapping logic for target vector
    if (std::abs(majorDx) > Graph::WIDTH / 2) majorDx = (majorDx > 0) ? majorDx - Graph::WIDTH : majorDx + Graph::WIDTH;
    if (std::abs(majorDy) > Graph::HEIGHT / 2) majorDy = (majorDy > 0) ? majorDy - Graph::HEIGHT : majorDy + Graph::HEIGHT;

    bool preferHorizontal = std::abs(majorDx) >= std::abs(majorDy);

    // Score: Lower is better.
    // If neighbor reduces major axis diff, good.
    int nDx = targetLoc.x - nextLoc.x;
    int nDy = targetLoc.y - nextLoc.y;
     if (std::abs(nDx) > Graph::WIDTH / 2) nDx = (nDx > 0) ? nDx - Graph::WIDTH : nDx + Graph::WIDTH;
     if (std::abs(nDy) > Graph::HEIGHT / 2) nDy = (nDy > 0) ? nDy - Graph::HEIGHT : nDy + Graph::HEIGHT;
     
    // Weighted distance: Penalize the minor axis slightly to prefer major axis moves?
    // Actually, simple Euclidean works for wall-respecting too, but to be 'Directional':
    // We want the neighbor that gets closest on the preferred axis.
    
    double dist;
    if (preferHorizontal) {
        // Primary cost is X distance, secondary is Y (tie breaker)
        dist = std::abs(nDx) * 1000 + std::abs(nDy);
    } else {
        dist = std::abs(nDy) * 1000 + std::abs(nDx);
    }

    if (dist < minScore) {
        minScore = dist;
        bestMove = nextLoc;
    }
  }

  return bestMove;
}
