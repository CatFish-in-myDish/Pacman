#ifndef PACMAN_H
#define PACMAN_H

#include "Entity.h"

class Pacman : public Entity {
public:
    Pacman(const Location& loc);
};

#endif // PACMAN_H
