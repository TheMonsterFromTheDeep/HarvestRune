#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"
#include "graphics.h"

typedef enum {
	DIR_DOWN,
	DIR_UP,
	DIR_LEFT,
	DIR_RIGHT
} direction;


entity *create_player();

declare_body(move_player, {
	sprite_renderer *renderer;
	int is_walking;
	direction dir;
	
	float health;
	float maxhealth;
	float mana;
	float maxmana;
	
	float manarate;
	
	int can_sleep;
	
	rot_sprite_renderer *attack;
})

#endif