#include "zombie.h"
#include "component.h"
#include "button.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "ground.h"
#include "tag.h"
#include "entity.h"
#include "slime.h"
#include "health.h"
#include "sounds.h"
#include "item.h"
#include "strength.h"
#include "barrier.h"

#include <math.h>
#include <stdlib.h>

static vec zombie_sprite_center = { 7.f / 2, 0.f };

declare_draw(zombie, { })

extern entity *player;

#define SWITCH_DIR(name, center)\
switch(new_dir) {\
	case DIR_DOWN:\
		set_animator_c(self->renderer, is_walking ? &sprite_ ## name ## _front_walk : &sprite_ ## name ## _front, 3, center);\
		break;\
	case DIR_UP:\
		set_animator_c(self->renderer, is_walking ? &sprite_ ## name ##_back_walk : &sprite_ ## name ##_back, 3, center);\
		break;\
	case DIR_LEFT:\
		set_animator_c(self->renderer, is_walking ? &sprite_ ## name ##_left_walk : &sprite_ ## name ##_left, 3, center);\
		break;\
	case DIR_RIGHT:\
		set_animator_c(self->renderer, is_walking ? &sprite_ ## name ##_right_walk : &sprite_ ## name ##_right, 3, center);\
		break;\
}

declare_tick(zombie, {
	vec velocity = vmul(normalize(vsub(player->position, parent->position)), 0.4f);
	if(fabs(velocity.x) < 0.05) velocity.x = 0;
	if(fabs(velocity.y) < 0.05) velocity.y = 0;
	
	direction new_dir;
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
	
	if(new_dir != self->dir || is_walking != self->is_walking) {
		if(self->is_skeleton) {
			SWITCH_DIR(skeleton, zombie_sprite_center)
		}
		else {
			SWITCH_DIR(zombie, zombie_sprite_center)
		}
	}
	
	self->dir = new_dir;
	self->is_walking = is_walking;
	
	parent->position = bar_clamp(parent->position, velocity, vxy(7.f, 2.f), vxy(0.f, 1.f), 0, NULL);
	
	if(self->attack->renderer->frame == 6) {
		self->attack->renderer->animate = 0;
	}
	
	if(self->attack_timer < 1) {
		if(len2(vsub(parent->position, player->position)) < 25 * 25) {
			vec aim_perfect = vsub(player->position, parent->position);
			float angle = atan2(aim_perfect.y, aim_perfect.x);
			angle += ((rand() % 10000) - 5000) * 0.0001;
			
			vec aim = vxy(cos(angle), sin(angle));
			
			float threshold = cos(10 * .01745329251);
			
			if(vcos(aim, aim_perfect) > threshold) {
				get_component(player, move_player)->health -= 1.5f;
			}

			self->attack->offset = vmul(normalize(aim), -5);
			self->attack->rotation = atan2(aim.y, aim.x) * 57.2957795131;
			self->attack->renderer->animate = 1;
			self->attack->renderer->frame = 0;
			
			self->attack_timer = 80;
		}
	}
	else --self->attack_timer;
})

declare_head(zombie)

declare_body(zombie_kill, { })
declare_draw(zombie_kill, { })

declare_tick(zombie_kill, {
	qdrop(parent, ITEM_GOLD, 2 + strength_count(11));
	qdrop(parent, ITEM_ZOMBIE_BONE, 1 + strength_count(7));
	
	if(strength_chance(1, 6)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.2f, 0.5f);
		dmg.damage_max = dmg.damage_min + strength_prop(2.f, 4.5f);
		qdropi(parent, ITEM_WOODEN_CLUB, 1, dmg);
	}
	else if(strength_chance(1, 3)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.9f, 1.5f);
		dmg.damage_max = dmg.damage_min + strength_prop(0.7f, 1.3f);
		dmg.mana_cost = 2.f - strength_prop(0.3f, 1.2f);
		qdropi(parent, ITEM_MAGIC_MISSILE_WAND, 1, dmg);
	}
	else if(strength_chance(1, 3)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.7f, 1.5f);
		dmg.damage_max = dmg.damage_min + strength_prop(0.8f, 1.9f);
		qdropi(parent, rand() % 2 ? ITEM_DAGGER_IRON : ITEM_SWORD_IRON, 1, dmg);
	}
	
	strength_dec();
	strength_dec();
})

declare_head(zombie_kill)

entity *create_zombie(float x, float y) {
	int is_skeleton = rand() % 2;
	entity *ent = ent_new();
	
	ent->kill_handler = c_new(zombie_kill);
	
	strength_inc();
	strength_inc();
	
	ent->position = vxy(x, y);
	ent->tag = TAG_ENEMY;
	
	zombie *z = c_new(zombie);
	sprite_renderer *renderer = get_animator_c(is_skeleton ? &sprite_zombie_front : &sprite_skeleton_front, 3, zombie_sprite_center);
	
	rot_sprite_renderer *rs = rot_renderer(get_animator_c(&sprite_combat_zombie, 1, vxy(25, 25)));
	rs->renderer->animate = 0;
	rs->renderer->frame = 6;
	
	z->renderer = renderer;
	z->dir = DIR_DOWN;
	z->is_walking = 0;
	z->attack = rs;
	z->attack_timer = 0;
	z->is_skeleton = is_skeleton;
	
	add_component(ent, renderer);
	add_component(ent, z);
	add_component(ent, rs);
	add_component(ent, get_health(12, 14, 7, s_zombie_hit));
	
	return ent;
}