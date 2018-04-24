#ifndef SLIME_H_
#define SLIME_H_

#include "ground.h"

entity *create_slime(float x, float y);

entity *create_slime_plant(float x, float y, ground_logic *parent);

#endif