#ifndef BUILDING_H_
#define BUILDING_H_

#include "component.h"
#include "graphics.h"
#include "entity.h"

declare_body(building, {
	sprite *appearance;
	int is_entered;
	
	int last_entered;
	
	int timer;
	
	float height;
})

building *get_building(sprite *s);
entity *create_building(float x, float y, float height, sprite *roof, sprite *floor);

#endif