#include "DistanceGreedyStrategy.h"
#include "Graph.h"
#include "Entity.h"
#include <cmath>
#include <limits>

Location DistanceGreedyStrategy::findNextMove(Graph* graph, Entity* monster, Entity* target) {
    Location currentLoc = monster->getLocation();
    Location targetLoc = target->getLocation();

    Node* currentNode = graph->getNode(currentLoc);
    const auto& neighbors = currentNode->getNeighbors();

    Location bestMove = currentLoc;
    double minDistance = std::numeric_limits<double>::max();

    for (Node* neighbor : neighbors) {
        Location neighborLoc = neighbor->getLocation();

        // Calculate Euclidean distance
        int dx = neighborLoc.x - targetLoc.x;
        int dy = neighborLoc.y - targetLoc.y;

        // Consider toroidal wrapping for distance calculation
        if (abs(dx) > Graph::WIDTH / 2) {
            dx = Graph::WIDTH - abs(dx);
        }
        if (abs(dy) > Graph::HEIGHT / 2) {
            dy = Graph::HEIGHT - abs(dy);
        }

        double distance = sqrt(dx * dx + dy * dy);

        if (distance < minDistance) {
            minDistance = distance;
            bestMove = neighborLoc;
        }
    }

    return bestMove;
}
