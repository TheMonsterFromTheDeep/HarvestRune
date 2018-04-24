#include "bat.h"
#include "component.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "tag.h"
#include "health.h"
#include "player.h"
#include "plant.h"
#include "sounds.h"
#include "item.h"
#include "strength.h"

extern entity *player;

declare_body(bat, {
	sprite_renderer *renderer;
	vec velocity;
	
	int direction;
	int attack;
})

declare_draw(bat, { })
declare_tick(bat, {
	vec delta = vmul(normalize(vsub(player->position, parent->position)), 2.4f);
	
	self->velocity = vadd(self->velocity, vmul(vsub(delta, self->velocity), 0.01f));
	
	parent->position = vadd(parent->position, self->velocity);
	
	int direction = 0;
	if(self->velocity.y > self->velocity.x) {
		direction = self->velocity.y > 0 ? 3 : 0;
	}
	else if(self->velocity.x != 0) {
		direction = self->velocity.x > 0 ? 2 : 1;
	}
	else {
		direction = self->direction;
	}
	
	self->renderer->start = direction * 2;
	self->renderer->end = direction * 2 + 1;
	self->renderer->animate = 1;
	
	if(self->attack < 1) {
		if(len2(vsub(parent->position, player->position)) < 5 * 5) {
			get_component(player, move_player)->health -= 1;
			
			self->attack = 30;
		}
	}
	else --self->attack;
})

declare_head(bat)

declare_body(bat_kill, { })
declare_draw(bat_kill, { })

declare_tick(bat_kill, {
	qdrop(parent, ITEM_GOLD, 2 + strength_count(9));
	qdrop(parent, ITEM_BAT_WING, 1 + strength_count(3));
	
	if(strength_chance(1, 5)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.7f, 1.2f);
		dmg.damage_max = dmg.damage_min + strength_prop(0.5f, 0.9f);
		dmg.mana_cost = 1.2f - strength_prop(0.1f, 0.4f);
		qdropi(parent, ITEM_MAGIC_MISSILE_WAND, 1, dmg);
	}
	else if(strength_chance(1, 3)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.5f, 1.1f);
		dmg.damage_max = dmg.damage_min + strength_prop(0.3f, 0.7f);
		qdropi(parent, rand() % 2 ? ITEM_DAGGER_WOOD : ITEM_SWORD_WOOD, 1, dmg);
	}
	
	strength_dec();
})

declare_head(bat_kill)

entity *create_bat(float x, float y) {
	entity *ent = ent_new();
	
	ent->kill_handler = c_new(bat_kill);
	
	strength_inc();
	
	ent->position = vxy(x, y);
	ent->tag = TAG_ENEMY;
	
	sprite_renderer *sr = get_renderer(&sprite_bat);
	sr->center = vxy(9, 0);
	sr->delay = 2;
	
	bat *s = c_new(bat);
	s->renderer = sr;
	s->velocity = v0();
	s->direction = 0;
	s->attack = 0;
	
	health *h = get_health(7, 8, 8, s_bat_hit);
	
	add_component(ent, sr);
	add_component(ent, s);
	add_component(ent, h);
	
	return ent;
}

/*declare_body(spider_plant, {
	sprite_renderer *renderer;
	ground_logic *ground;
})

declare_draw(spider_plant, { })
declare_tick(spider_plant, {
	if(self->renderer->frame <= 15) {
		self->renderer->animate = self->ground->state == GROUND_WET;
	}
	else if(self->renderer->animate) {
		self->renderer->animate = 0;
		
		create_spider(parent->position.x, parent->position.y);
	}
})

declare_head(spider_plant)*/

entity *create_bat_plant(float x, float y, ground_logic *ground) {
	entity *ent = ent_new();
	ent->position = vxy(x, y);
	ent->tag = TAG_PLANT;
	
	sprite_renderer *sr = get_animator_c(&sprite_bat_plant, 19, vxy(8, 0));
	
	plant *sp = get_plant(sr, ground, 18, 6, create_bat);
	
	add_component(ent, sr);
	add_component(ent, sp);
	
	return ent;
}