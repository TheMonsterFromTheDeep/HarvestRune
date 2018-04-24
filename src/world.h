#ifndef WORLD_H_
#define WORLD_H_

#include "entity.h"

entity *create_floor(float x, float y, int type);

void init_world();

#endif