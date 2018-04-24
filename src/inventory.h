#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "entity.h"
#include "player.h"
#include "images_diffuse.h"
#include "ground.h"

entity *create_inventory();

typedef struct {
	sprite *appearance;
	void (*use)(move_player*,entity*);
	int uses_tiles;
} inv_item;

typedef struct {
	float damage_min;
	float damage_max;
	float radius;
	float mana_cost;
} special_info;

#define INV_SLOTS 20
int inventory_count(int slot);
int inventory_id(int slot);
void inventory_put(int slot, int id, int quantity);
void inventory_put_i(int slot, int id, int quantity, special_info info);
void inventory_use();

extern special_info active_item_info;
extern special_info highlighted_item_info;
extern int active_item_id;

special_info *inventory_info(int slot);

void init_inventory();

#endif