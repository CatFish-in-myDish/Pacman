#ifndef NODE_H
#define NODE_H

#include "Location.h"
#include <vector>

class Node {
private:
  Location location;
  std::vector<Node *> neighbours;

public:
  Node(const Location &loc);

  void addNeighbour(Node *neighbour);
  const std::vector<Node *> &getNeighbours() const;
  Location getLocation() const;
};

#endif
