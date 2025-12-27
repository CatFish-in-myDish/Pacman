#include "../include/Graph.h"

Graph::Graph() { initializeGrid(); }

Graph::~Graph() {
  // Clean up all dynamically allocated nodes
  for (auto &pair : nodes) {
    delete pair.second;
  }
  nodes.clear();
}

void Graph::initializeGrid() {
  // 1. Create all nodes
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      Location loc(x, y);
      nodes[loc] = new Node(loc);
    }
  }

  // 2. Connect neighbors (Toroidal)
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      Node *current = nodes[Location(x, y)];

      // Neighbors: Up, Down, Left, Right
      addNeighbor(current, x, y - 1); // Up
      addNeighbor(current, x, y + 1); // Down
      addNeighbor(current, x - 1, y); // Left
      addNeighbor(current, x + 1, y); // Right
    }
  }
}

void Graph::addNeighbor(Node *current, int nx, int ny) {
  // Toroidal wrap-around logic
  if (nx < 0)
    nx = WIDTH - 1;
  if (nx >= WIDTH)
    nx = 0;
  if (ny < 0)
    ny = HEIGHT - 1;
  if (ny >= HEIGHT)
    ny = 0;

  Node *neighbor = nodes[Location(nx, ny)];
  current->addNeighbor(neighbor);
}

Node *Graph::getNode(const Location &loc) {
  auto it = nodes.find(loc);
  if (it != nodes.end()) {
    return it->second;
  }
  return nullptr;
}

Node *Graph::getNode(int x, int y) { return getNode(Location(x, y)); }
