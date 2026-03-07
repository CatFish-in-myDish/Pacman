/*
Represents a single node (tile) in the game graph.

Each node holds its location and a list of adjacent neighbours (edges).

Time Complexity: All methods are O(1).
*/

#include "../include/Node.h"

Node::Node(const Location &loc) : location(loc) {}

void Node::addNeighbour(Node *neighbour) { neighbours.push_back(neighbour); }

const std::vector<Node *> &Node::getNeighbours() const { return neighbours; }

Location Node::getLocation() const { return location; }
