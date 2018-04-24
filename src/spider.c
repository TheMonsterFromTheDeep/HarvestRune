#include "spider.h"
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

declare_body(spider, {
	sprite_renderer *renderer;
	vec velocity;
	
	int direction;
	int attack;
})

declare_draw(spider, { })
declare_tick(spider, {
	vec delta = vmul(normalize(vsub(player->position, parent->position)), 2.f);
	
	self->velocity = vadd(self->velocity, vmul(vsub(delta, self->velocity), 0.05f));
	
	parent->position = vadd(parent->position, self->velocity);
	
	int direction = 0;
	int moving = 1;
	if(self->velocity.y > self->velocity.x) {
		direction = self->velocity.y > 0 ? 3 : 0;
	}
	else if(self->velocity.x != 0) {
		direction = self->velocity.x > 0 ? 1 : 2;
	}
	else {
		direction = self->direction;
		moving = 0;
	}
	
	if(!moving) {
		self->renderer->frame = direction * 3;
		self->renderer->animate = 0;
	}
	else {
		self->renderer->start = direction * 3 + 1;
		self->renderer->end = direction * 3 + 2;
		self->renderer->animate = 1;
	}
	
	if(self->attack < 1) {
		if(len2(vsub(parent->position, player->position)) < 5 * 5) {
			get_component(player, move_player)->health -= 1;
			
			self->attack = 30;
		}
	}
	else --self->attack;
})

declare_head(spider)

declare_body(spider_kill, { })
declare_draw(spider_kill, { })

declare_tick(spider_kill, {
	qdrop(parent, ITEM_GOLD, 2 + strength_count(7));
	qdrop(parent, ITEM_SPIDER_LEG, 1 + strength_count(6));
	
	if(strength_chance(1, 5)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.6f, 1.4f);
		dmg.damage_max = dmg.damage_min + strength_prop(0.5f, 1.5f);
		qdropi(parent, rand() % 2 ? ITEM_DAGGER_IRON : ITEM_SWORD_IRON, 1, dmg);
	}
	else if(strength_chance(1, 3)) {
		special_info dmg;
		dmg.damage_min = strength_prop(0.5f, 1.1f);
		dmg.damage_max = dmg.damage_min + strength_prop(0.3f, 0.7f);
		qdropi(parent, rand() % 2 ? ITEM_DAGGER_WOOD : ITEM_SWORD_WOOD, 1, dmg);
	}
	
	strength_dec();
})

declare_head(spider_kill)

entity *create_spider(float x, float y) {
	entity *ent = ent_new();
	
	strength_inc();
	
	ent->position = vxy(x, y);
	ent->tag = TAG_ENEMY;
	ent->kill_handler = c_new(spider_kill);
	
	sprite_renderer *sr = get_renderer(&sprite_spider);
	sr->center = vxy(4, 0);
	sr->delay = 2;
	
	spider *s = c_new(spider);
	s->renderer = sr;
	s->velocity = v0();
	s->direction = 0;
	s->attack = 0;
	
	health *h = get_health(5, 8, 8, s_spider_hit);
	
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

entity *create_spider_plant(float x, float y, ground_logic *ground) {
	entity *ent = ent_new();
	ent->position = vxy(x, y);
	ent->tag = TAG_PLANT;
	
	sprite_renderer *sr = get_animator_c(&sprite_spider_plant, 18, vxy(8, 0));
	
	plant *sp = get_plant(sr, ground, 15, 8, create_spider);
	
	add_component(ent, sr);
	add_component(ent, sp);
	
	return ent;
}