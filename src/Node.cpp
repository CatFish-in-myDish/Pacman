/**
 * Represents a single node (tile) in the game graph.
 *
 * Each node holds its location and a list of adjacent neighbors (edges).
 *
 * Time Complexity: All methods are O(1).
 */
#include "../include/Node.h"

Node::Node(const Location &loc) : location(loc) {}

void Node::addNeighbor(Node *neighbor) { neighbors.push_back(neighbor); }

const std::vector<Node *> &Node::getNeighbors() const { return neighbors; }

Location Node::getLocation() const { return location; }
