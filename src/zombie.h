#ifndef ZOMBIE_H_
#define ZOMBIE_H_

#include "player.h"
#include "ground.h"

declare_body(zombie, {
	sprite_renderer *renderer;
	int is_walking;
	direction dir;
	
	rot_sprite_renderer *attack;
	
	int attack_timer;
	int is_skeleton;
})

entity *create_zombie(float x, float y);
entity *create_zombie_plant(float x, float y, ground_logic *ground);

#endif