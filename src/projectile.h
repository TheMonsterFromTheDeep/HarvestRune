#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "graphics.h"
#include "entity.h"

entity *create_projectile(vec position, vec velocity, float damage, sprite_renderer *renderer);

#endif