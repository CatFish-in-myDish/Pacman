#include "../include/DirectionalGreedyStrategy.h"
#include "../include/Graph.h"
#include "../include/Entity.h"
#include <cmath>

Location DirectionalGreedyStrategy::findNextMove(Graph* graph, Entity* monster, Entity* target) {
    Location currentLoc = monster->getLocation();
    Location targetLoc = target->getLocation();

    // Calculate differences with wrapping
    int dx = targetLoc.x - currentLoc.x;
    int dy = targetLoc.y - currentLoc.y;

    // Handle toroidal wrapping
    if (abs(dx) > Graph::WIDTH / 2) {
        dx = dx > 0 ? dx - Graph::WIDTH : dx + Graph::WIDTH;
    }
    if (abs(dy) > Graph::HEIGHT / 2) {
        dy = dy > 0 ? dy - Graph::HEIGHT : dy + Graph::HEIGHT;
    }

    // Determine primary axis (larger difference)
    Location nextLoc = currentLoc;

    if (abs(dx) >= abs(dy)) {
        // Move horizontally
        if (dx > 0) {
            nextLoc.x = (currentLoc.x + 1) % Graph::WIDTH;
        } else if (dx < 0) {
            nextLoc.x = (currentLoc.x - 1 + Graph::WIDTH) % Graph::WIDTH;
        } else {
            // If equal on x, move on y
            if (dy > 0) {
                nextLoc.y = (currentLoc.y + 1) % Graph::HEIGHT;
            } else if (dy < 0) {
                nextLoc.y = (currentLoc.y - 1 + Graph::HEIGHT) % Graph::HEIGHT;
            }
        }
    } else {
        // Move vertically
        if (dy > 0) {
            nextLoc.y = (currentLoc.y + 1) % Graph::HEIGHT;
        } else if (dy < 0) {
            nextLoc.y = (currentLoc.y - 1 + Graph::HEIGHT) % Graph::HEIGHT;
        }
    }

    return nextLoc;
}
