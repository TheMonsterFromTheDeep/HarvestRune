#include "scientist.h"
#include "player.h"
#include "images_diffuse.h"
#include "inventory.h"
#include "item.h"

#include <math.h>

extern entity *player;

static vec sci_sprite_center = { 7.f / 2, 0.f };

declare_body(scientist, {
	direction dir;
	int is_walking;
	
	sprite_renderer *renderer;
	
	int timer;
})

#define P 2.f
#define P2 (P / 2.f)
#define PIP (3.14 / P)
#define PA (3.f + P)

float wave(float t) {
	t = fmod(t, 6);
	if(t < P) return sin(PIP * (t - P2));
	if(t < 3) return 1.f;
	if(t < PA) return sin(PIP * (t - 3 + P2));
	return -1.f;
}

extern int inventory_buy;
extern int game_progress;

declare_draw(scientist, { })
declare_tick(scientist, {
	vec velocity = vxy(wave(self->timer * 0.05f), 0.f);
	
	direction new_dir = self->dir;
	int is_walking = 1;
	
	if(velocity.y != 0) {
		if(velocity.y < 0)
			new_dir = DIR_DOWN;
		else
			new_dir = DIR_UP;
	}
	else if(velocity.x != 0) {
		if(velocity.x < 0) new_dir = DIR_LEFT;
		else new_dir = DIR_RIGHT;
	}
	else {
		new_dir = self->dir;
		is_walking = 0;
	}
	
	inventory_buy = 0;
	
	if(fabs(player->position.y - parent->position.y) < 20 && fabs(player->position.x - parent->position.x) < 25) {
		velocity = v0();
		new_dir = DIR_DOWN;
		is_walking = 0;
		inventory_buy = 1;
		
		for(int i = 0; i < INV_SLOTS; ++i ) {
			int k = inventory_id(i);
			if(k == ITEM_SLIME || k == ITEM_SPIDER_LEG || k == ITEM_BAT_WING || k == ITEM_ZOMBIE_BONE) {
				game_progress += inventory_count(i);
				inventory_put(i, 0, 0);
			}
		}
	}
	else {
		++self->timer;
	}
	
	if(new_dir != self->dir || is_walking != self->is_walking) {
		switch(new_dir) {
			case DIR_DOWN:
				set_animator_c(self->renderer, is_walking ? &sprite_sci_front_walk : &sprite_sci_front, 3, sci_sprite_center);
				break;
			case DIR_UP:
				set_animator_c(self->renderer, is_walking ? &sprite_sci_back_walk : &sprite_sci_back, 3, sci_sprite_center);
				break;
			case DIR_LEFT:
				set_animator_c(self->renderer, is_walking ? &sprite_sci_left_walk : &sprite_sci_left, 3, sci_sprite_center);
				break;
			case DIR_RIGHT:
				set_animator_c(self->renderer, is_walking ? &sprite_sci_right_walk : &sprite_sci_right, 3, sci_sprite_center);
				break;
		}
	}
	
	self->dir = new_dir;
	self->is_walking = is_walking;
	
	parent->position = vadd(parent->position, velocity);
	
	
})

declare_head(scientist)

entity *create_scientist() {
	entity *ent = ent_new();
	
	ent->position = vxy(147, -133);
	
	sprite_renderer *sr = get_animator_c(&sprite_sci_front_walk, 3, sci_sprite_center);
	
	scientist *s = c_new(scientist);
	
	s->dir = DIR_DOWN;
	s->is_walking = 1;
	s->timer = 0;
	s->renderer = sr;
	
	add_component(ent, sr);
	add_component(ent, s);
	
	return ent;
}