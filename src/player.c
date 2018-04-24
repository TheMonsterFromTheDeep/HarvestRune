#include "player.h"
#include "component.h"
#include "button.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "ground.h"
#include "tag.h"
#include "entity.h"
#include "slime.h"
#include "health.h"
#include "zombie.h"
#include "spider.h"
#include "tutorial.h"
#include "inventory.h"
#include "projectile.h"
#include "barrier.h"
#include "item.h"
#include "sounds.h"
#include "strength.h"
#include "plant.h"

#include <math.h>

static joystick controls;

vec player_tile;

static vec player_sprite_center = { 7.f / 2, 0.f };

int should_draw_cursor = 0;
int cursor_valid = 0;
float cursor_radius = 32;

declare_body(tile_cursor, { sprite_renderer *renderer; })
declare_draw(tile_cursor, { })
declare_tick(tile_cursor, {
	vec cur = world_cursor();
	player_tile = vsnap(vsub(cur, vxy(8, 8)), 16);
	parent->position = player_tile;
	
	parent->visible = should_draw_cursor;
	
	self->renderer->frame = cursor_valid;
})
declare_head(tile_cursor)



declare_draw(move_player, {
	if(self->can_sleep) {
		draw_sprite(&sprite_sleep, parent->position.x - 31, parent->position.y + 16);
	}
})

void act_hoe(move_player *self, entity *parent) {
	if(len2(vsub(player_tile, parent->position)) < 32 * 32) {
		entity *ent = ent_first(with_component(c_type(ground_logic)), within_radius(player_tile, 1.f));
		if(!ent) return;
		
		ground_logic *ground = get_component(ent, ground_logic);
		if(ground) {
			if(ground->state < GROUND_PREPARED) { ++ground->state; }
			if(ground->state < GROUND_WET) {
				play_sound(s_hoe);
				ground->timer = -500;
				ground->transition = 0;
			}
		}
	}
}

void act_water(move_player *self, entity *parent) {
	if(len2(vsub(player_tile, parent->position)) < 32 * 32) {
		entity *ent = ent_first(with_component(c_type(ground_logic)), within_radius(player_tile, 1.f));
		if(!ent) return;
		
		ground_logic *ground = get_component(ent, ground_logic);
		if(ground) {
			if(ground->state >= GROUND_PREPARED) {
				play_sound(s_water);
				ground->state = GROUND_WET;
				ground->timer = -500;
				ground->transition = 0;
			}
		}
	}
}

void act_plant(move_player *self, entity *parent) {
	if(len2(vsub(player_tile, parent->position)) < 32 * 32) {
		entity *ent = ent_first(with_component(c_type(ground_logic)), within_radius(player_tile, 1.f));
		if(!ent) return;
		
		ground_logic *ground = get_component(ent, ground_logic);
		if(ground) {
			if(ground->state >= GROUND_PREPARED && !ground->child_plant) {
				play_sound(s_plant);
				ground->child_plant = item_plant(active_item_id)(player_tile.x + 8, player_tile.y + 2, ground);
				inventory_use();
			}
		}
	}
}

void act_potion(move_player *self, entity *parent) {
	self->health += 6;
	if(self->health > self->maxhealth) self->health = self->maxhealth;
	
	play_sound(s_potion);
	inventory_use();
}

void act_snip(move_player *self, entity *parent) {
	if(len2(vsub(player_tile, parent->position)) < 32 * 32) {
		entity *ent = ent_first(with_component(c_type(ground_logic)), within_radius(player_tile, 1.f));
		if(!ent) return;
		
		ground_logic *ground = get_component(ent, ground_logic);
		if(ground) {
			if(ground->child_plant) {
				save_tutorial_step(12);
				play_sound(s_snip);
				if(plant_clip(ground->child_plant))
					ground->child_plant = NULL;
			}
		}
	}
}

float sword_arc = 90;

void act_sword(move_player *self, entity *parent) {
	vec cur = world_cursor();
	vec delta = vsub(cur, parent->position);
	
	float threshold = cos(sword_arc * .01745329251);
	
	for_ent(enemy, ent_all(
		within_radius(parent->position, 25),
		tagged(TAG_ENEMY)
		), {
			vec edelta = vsub(enemy->position, parent->position);
			if(vcos(delta, edelta) > threshold || (len2(edelta) < 5 * 5)) {
				damage(enemy, uniform(active_item_info.damage_min, active_item_info.damage_max));
			}
	})
	
	self->attack->offset = vmul(normalize(delta), -5);
	self->attack->rotation = atan2(delta.y, delta.x) * 57.2957795131;
	self->attack->renderer->animate = 1;
	self->attack->renderer->frame = 0;
	
	play_sound(s_swipe);
}

void act_shoot(move_player *self, entity *parent) {
	if(self->mana >= active_item_info.mana_cost) {
		play_sound(s_magic_missile);
		vec v = vsub(world_cursor(), parent->position);
		v = vmul(normalize(v), 3.f);
		create_projectile(parent->position, v, uniform(active_item_info.damage_min, active_item_info.damage_max), get_animator_c(&sprite_magic_missile, 2, vxy(5, 2.5f)));
		
		self->mana -= active_item_info.mana_cost;
		self->manarate = 0.f;
	}
}

void (*player_action)(move_player*,entity*) = act_sword;

float player_display_health;
float player_display_mana;
extern int inventory_open;
extern int inventory_buy;

extern void lose_gold();

extern void set_fade_timer();
extern void set_morning();

declare_tick(move_player, {
	
	
	vec velocity = joystick_value(controls);
	
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
		switch(new_dir) {
			case DIR_DOWN:
				set_animator_c(self->renderer, is_walking ? &sprite_player_front_walk : &sprite_player_front, 3, player_sprite_center);
				break;
			case DIR_UP:
				set_animator_c(self->renderer, is_walking ? &sprite_player_back_walk : &sprite_player_back, 3, player_sprite_center);
				break;
			case DIR_LEFT:
				set_animator_c(self->renderer, is_walking ? &sprite_player_left_walk : &sprite_player_left, 3, player_sprite_center);
				break;
			case DIR_RIGHT:
				set_animator_c(self->renderer, is_walking ? &sprite_player_right_walk : &sprite_player_right, 3, player_sprite_center);
				break;
		}
	}
	
	self->dir = new_dir;
	self->is_walking = is_walking;
	
	parent->position = bar_clamp(parent->position, velocity, vxy(7.f, 2.f), vxy(0.f, 1.f), 0, NULL);
	
	if(!inventory_open && !inventory_buy) {
		if(on_down(get_key(LEFT_MOUSE))) {
			player_action(self, parent);
		}
	}
	
	if(on_down(get_key('M'))) {
		vec cur = world_cursor();
		printf("Measured point (%f %f) -> [%f %f]\n", cur.x, cur.y, round(cur.x), round(cur.y));
	}
	
	self->manarate += (1.f - self->manarate) * 0.002f;
	
	self->mana += self->manarate;
	if(self->mana > self->maxmana) self->mana = self->maxmana;
	
	float cur_display = self->health / self->maxhealth;
	float cur_dispmana = self->mana / self->maxmana;
	player_display_health += (cur_display - player_display_health) * 0.7f;
	player_display_mana += (cur_dispmana - player_display_mana) * 0.7f;
	
	self->can_sleep = (parent->position.x > 258) && (len2(vsub(parent->position, vxy(262, 36))) < 16 * 16);
	
	if(self->can_sleep && on_down(get_key('E'))) {
		parent->position = vxy(17 * 16, 20);
		self->health = self->maxhealth;
		self->mana = self->maxmana;
		
		set_fade_timer();
		set_morning();
		
		for_ent(g, ent_all(with_component(c_type(ground_logic))), {
			ground_logic *gl = get_component(g, ground_logic);
			if(gl->state > GROUND_PREPARED) gl->state = GROUND_PREPARED;
		})
		
		for_ent(p, ent_all(with_component(c_type(plant))), {
			plant *pl = get_component(p, plant);
			if(pl->renderer->frame >= pl->start_frame)
				pl->renderer->frame = pl->start_frame;
		})
	}
	
	if(self->health <= 0) {
		for(int i = 0; i < INV_SLOTS; ++i ) {
			int k = inventory_id(i);
			if(k == ITEM_SLIME || k == ITEM_SPIDER_LEG || k == ITEM_BAT_WING || k == ITEM_ZOMBIE_BONE) {
				inventory_put(i, 0, 0);
			}
		}
		lose_gold();
		
		parent->position = vxy(17 * 16, 20);
		self->health = self->maxhealth;
		self->mana = self->maxmana;
		
		set_fade_timer();
	}
	
	//self->can_sleep = within
	/*if(on_down(get_key(' '))) {
		which_action = (which_action + 1) % 5;
		switch(which_action) {
			case 0: player_action = act_hoe; break;
			case 1: player_action = act_sword; break;
			case 2: player_action = act_water; break;
			case 3: player_action = act_plant; break;
			case 4: player_action = act_snip; break;
		}
	}*/
	
	if(self->attack->renderer->frame == 7) {
		self->attack->renderer->animate = 0;
	}
	
	cursor_valid = 0;
	if(len2(vsub(player_tile, parent->position)) < cursor_radius * cursor_radius) {
		entity *ent = ent_first(with_component(c_type(ground_logic)), within_radius(player_tile, 1.f));
		if(ent) cursor_valid = 1;
	}
})

declare_head(move_player)

entity *create_player() {
	entity *player = ent_new();
	
	player->tag = 0xBEEFBEEF; /* Lazy magic number */
	
	move_player *player_mover = c_new(move_player);
	sprite_renderer *renderer = get_animator_c(&sprite_player_front, 3, player_sprite_center);
	
	rot_sprite_renderer *rs = rot_renderer(get_animator_c(&sprite_combat, 1, vxy(25, 25)));
	rs->rotation = 45;
	rs->renderer->animate = 0;
	rs->renderer->frame = 7;
	
	player_mover->renderer = renderer;
	player_mover->dir = DIR_DOWN;
	player_mover->is_walking = 0;
	player_mover->attack = rs;
	player_mover->maxhealth = 20;
	player_mover->health = 20;
	player_mover->maxmana = 10;
	player_mover->mana = 10;
	player_mover->manarate = 1.f;
	player_mover->can_sleep = 0;
	
	add_component(player, renderer);
	add_component(player, player_mover);
	add_component(player, rs);
	add_component(player, rs->renderer);
	
	controls = joystick_from(key_axis('A', 'D'), key_axis('S', 'W'));
	
	entity *cursor = ent_new();
	sprite_renderer *csr = get_renderer(&sprite_cursor);
	tile_cursor *ctc = c_new(tile_cursor);
	ctc->renderer = csr;
	add_component(cursor, ctc);
	add_component(cursor, csr);
	
	return player;
}