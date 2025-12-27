#ifndef NODE_H
#define NODE_H

#include "Location.h"
#include <vector>

class Node {
private:
  Location location;
  std::vector<Node *> neighbors;

public:
  Node(const Location &loc);

  void addNeighbor(Node *neighbor);
  const std::vector<Node *> &getNeighbors() const;
  Location getLocation() const;
};

#endif
