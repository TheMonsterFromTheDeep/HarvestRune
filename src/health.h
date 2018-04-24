#ifndef HEALTH_H_
#define HEALTH_H_

#include "component.h"
#include "entity.h"
#include "sounds.h"

declare_body(health, {
	float max;
	float value;
	float display;
	
	float position;
	float size;
	
	sound *hit;
})

health *get_health(float max, float position, float size, sound *hit);
void damage(entity *e, float amount);

#endif