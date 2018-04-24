#ifndef ITEM_H_
#define ITEM_H_

#include "graphics.h"
#include "component.h"
#include "entity.h"
#include "player.h"
#include "inventory.h"
#include "ground.h"

typedef struct {
	sprite *on_ground;
	sprite *in_inventory;
	int can_stack;
	void (*associated_action)(move_player*,entity*);
	int uses_tiles;
} item;

#define ITEM_NONE 0
#define ITEM_GOLD 1
#define ITEM_SLIME 2
#define ITEM_SWORD_RUSTY 3
#define ITEM_HOE_RUSTY 4
#define ITEM_WATER_RUSTY 5
#define ITEM_CLIP_RUSTY 6
#define ITEM_SEED_SLIME 7
#define ITEM_MAGIC_MISSILE_WAND 8
#define ITEM_SWORD_WOOD 9
#define ITEM_SWORD_IRON 10
#define ITEM_DAGGER_RUSTY 11
#define ITEM_DAGGER_WOOD 12
#define ITEM_DAGGER_IRON 13
#define ITEM_SEED_ZOMBIE 14
#define ITEM_SEED_SPIDER 15
#define ITEM_SEED_BAT 16
#define ITEM_SPIDER_LEG 17
#define ITEM_ZOMBIE_BONE 18
#define ITEM_BAT_WING 19
#define ITEM_POTION 20
#define ITEM_WOODEN_CLUB 21

declare_body(drop, {
	int id;
	int quantity;
	special_info info;
	float x;
	float y;
	sprite_renderer *renderer;
})

entity *create_drop(float x, float y, int id, int quantity, special_info info);
entity *qdrop(entity *e, int id, int quantity);
entity *qdropi(entity *e, int id, int quantity, special_info info);
entity *invdrop(int id, int quantity, special_info info);

sprite *item_sprite(int id);
int item_stackable(int id);

void (*item_action(int id))(move_player*,entity*);
int item_uses_tiles(int id);

const char *item_text(int id);

void init_items();

typedef entity* (*plant_creator)(float x, float y, ground_logic *gl);

plant_creator item_plant(int id);

#endif