#ifndef GROUND_H_
#define GROUND_H_

#include "entity.h"
#include "component.h"
#include "graphics.h"

typedef enum {
	GROUND_GRASS = 0,
	GROUND_DIRT = 1,
	GROUND_PREPARED = 2,
	GROUND_WET = 3
} ground_state;

declare_body(ground_logic, {
	ground_state state;
	
	sprite_renderer *renderer;
	
	int can_change_state;
	int transition;
	int timer;
	entity *child_plant;
})

entity *create_ground(float x, float y);

#endif