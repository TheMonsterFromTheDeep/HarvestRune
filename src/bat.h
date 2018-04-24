#ifndef SPIDER_H_
#define SPIDER_H_

#include "ground.h"
#include "entity.h"

entity *create_bat(float x, float y);
entity *create_bat_plant(float x, float y, ground_logic *gl);

#endif