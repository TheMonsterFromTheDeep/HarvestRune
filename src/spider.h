#ifndef SPIDER_H_
#define SPIDER_H_

#include "ground.h"
#include "entity.h"

entity *create_spider(float x, float y);
entity *create_spider_plant(float x, float y, ground_logic *gl);

#endif