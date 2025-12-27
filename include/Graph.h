#ifndef GRAPH_H
#define GRAPH_H

#include "Location.h"
#include "Node.h"
#include <unordered_map>

class Graph {
public:
  static const int WIDTH = 28;
  static const int HEIGHT = 31;

private:
  std::unordered_map<Location, Node *> nodes;

  void initializeGrid();
  void addNeighbor(Node *current, int nx, int ny);

public:
  Graph();
  ~Graph();

  Node *getNode(const Location &loc);
  Node *getNode(int x, int y);
  static bool isWall(int x, int y);
};

#endif
