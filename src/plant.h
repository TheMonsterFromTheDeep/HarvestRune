#ifndef PLANT_H_
#define PLANT_H_

#include "graphics.h"
#include "ground.h"
#include "component.h"
#include "entity.h"

declare_body(plant, {
	sprite_renderer *renderer;
	ground_logic *ground;
	
	int growth_frame;
	int start_frame;
	entity* (*spawn)(float x, float y);
})

plant *get_plant(sprite_renderer *renderer, ground_logic *ground, int growth_frame, int start_frame, entity* (*spawn)(float x, float y));

/* returns whether the plant was ent_destroyed() */
int plant_clip(entity *e);

#endif