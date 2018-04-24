#include "slime.h"
#include "entity.h"
#include "component.h"
#include "images_diffuse.h"
#include "graphics.h"
#include "tag.h"
#include "player.h"
#include "health.h"
#include "tutorial.h"
#include "item.h"
#include "plant.h"
#include "barrier.h"
#include "strength.h"
#include "sounds.h"

#include <stdlib.h>
#include <math.h>

declare_body(slime, {
	sprite_renderer *renderer;
	int mode;
	int timer;
	int goal;
	
	int attack;
	
	vec from;
	vec delta;
})

declare_draw(slime, { })
declare_tick(slime, {
	int can_attack = 1;
	
	if(self->mode) {
		float t = self->timer / 25.f;
		float jump = 2 * (t - 0.5f);
		jump = 1 - jump * jump;
		
		vec delta = vadd(vmul(self->delta, t), vxy(0, jump * len(self->delta) * 0.7f));
		
		parent->position = bar_clamp(self->from, delta, vxy(11.f, 6.f), vxy(0, 1.f), 0, NULL);
		
		if(t < 0.8f) can_attack = 0;
		
		if(t > 1.f) {
			self->timer = 0;
			self->mode = 0;
			self->goal = 30 + rand() % 140;
		}
	}
	else {
		if(self->timer > self->goal) {
			self->renderer->frame = 1;
			if(self->timer > self->goal + 20) {
				self->renderer->frame = 0;
				self->mode = 1;
				self->timer = 0;
				
				vec delta;
				
				entity *player = ent_first(within_radius(parent->position, 30.f), tagged(0xBEEFBEEF));
				
				if(player) {
					delta = vsub(player->position, parent->position);
				}
				else {
					float angle = rand() * (6.28 / RAND_MAX);
					float magnitude = 11.f + (rand() % 18000) * 0.001f;
					delta = vxy(magnitude * cos(angle), magnitude * sin(angle));
				}
				
				self->from = parent->position;
				self->delta = delta;
			}
		}
		
		
	}
	
	if(self->attack < 1) {
		if(can_attack) {
			entity *player = ent_first(within_radius(parent->position, 7.f), tagged(0xBEEFBEEF));
			if(player) {
				get_component(player, move_player)->health -= 1;
			}
			self->attack = 40;
		}
	}
	else --self->attack;
	
	++self->timer;
})

declare_head(slime)

declare_body(slime_kill, { })
declare_draw(slime_kill, { })

declare_tick(slime_kill, {
	static int is_first_slime = 1;
	if(is_first_slime) {
		special_info dmg;
		dmg.damage_min = 1.f;
		dmg.damage_max = 1.2f;
		dmg.mana_cost = 1.f;
		qdropi(parent, ITEM_MAGIC_MISSILE_WAND, 1, dmg);
	}
	
	qdrop(parent, ITEM_GOLD, strength_count(7));
	qdrop(parent, ITEM_SLIME, 1 + strength_count(4));
	
	
	if(strength_chance(1, 4)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.4f, 1.1f);
		dmg.damage_max = dmg.damage_min + strength_prop(0.3f, 0.9f);
		qdropi(parent, rand() % 2 ? ITEM_DAGGER_WOOD : ITEM_SWORD_WOOD, 1, dmg);
	}
	else if(strength_chance(1, 3)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.3f, 1.f);
		dmg.damage_max = dmg.damage_min + strength_prop(0.2f, 0.6f);
		qdropi(parent, rand() % 2 ? ITEM_DAGGER_RUSTY : ITEM_SWORD_RUSTY, 1, dmg);
	}
	
	strength_dec();
	
	is_first_slime = 0;
})

declare_head(slime_kill)

entity *create_slime(float x, float y) {
	entity *ent = ent_new();
	ent->position = vxy(x, y);
	ent->tag = TAG_ENEMY;
	ent->kill_handler = c_new(slime_kill);
	
	sprite_renderer *sr = get_animator_c(&sprite_slime, 2, vxy(11.f / 2, 0));
	sr->animate = 0;
	
	slime *sl = c_new(slime);
	sl->renderer = sr;
	sl->mode = 0;
	sl->timer = 0;
	sl->goal = 40;
	sl->attack = 0;
	
	add_component(ent, sr);
	add_component(ent, sl);
	add_component(ent, get_health(uniform(9, 14), 8, 9, s_slime_hit));
	
	save_tutorial_step(9);
	
	strength_inc();
	
	return ent;
}

/*declare_body(slime_plant, {
	sprite_renderer *renderer;
	ground_logic *ground;
})

declare_draw(slime_plant, { })
declare_tick(slime_plant, {
	if(self->ground->state == GROUND_WET) {
		save_tutorial_step(8);
	}
	
	if(self->renderer->frame <= 13) {
		self->renderer->animate = self->ground->state == GROUND_WET;
	}
	else if(self->renderer->animate) {
		self->renderer->animate = 0;
		
		create_slime(parent->position.x, parent->position.y);
	}
})

declare_head(slime_plant)*/

entity *create_slime_plant(float x, float y, ground_logic *ground) {
	entity *ent = ent_new();
	ent->position = vxy(x, y);
	ent->tag = TAG_PLANT;
	
	sprite_renderer *sr = get_animator_c(&sprite_slime_plant, 25, vxy(8, 0));
	
	plant *sp = get_plant(sr, ground, 13, 8, create_slime); //c_new(slime_plant);
	
	//sp->renderer = sr;
	//sp->ground = ground;
	
	add_component(ent, sr);
	add_component(ent, sp);
	
	save_tutorial_step(7);
	
	return ent;
}