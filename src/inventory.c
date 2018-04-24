#include "inventory.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "player.h"
#include "button.h"
#include "item.h"
#include "entity.h"
#include "ground.h"
#include "slime.h"
#include "text.h"
#include "bat.h"
#include "sounds.h"
#include "tutorial.h"
#include "inventory.h"
#include "item.h"

extern entity *player;

//inv_item items[10];
#include <math.h>

extern void (*player_action)(move_player*,entity*);
extern int should_draw_cursor;

typedef struct {
	int id;
	int quantity;
	special_info info;
} inventory_slot;

typedef struct {
	int id;
	int price;
} shop_slot;

inventory_slot inventory[INV_SLOTS];

shop_slot shop[10];

inventory_slot unplaced;

special_info active_item_info;
special_info highlighted_item_info;
int active_item_id;

void init_inventory() {
	for(int i = 0; i < INV_SLOTS; ++i) {
		inventory[i] = (inventory_slot){ 0, 0 };
	}
	
	for(int i = 0; i < 10; ++i) {
		shop[i] = (shop_slot){ 0, 0 };
	}
	
	inventory[0].id = ITEM_SWORD_RUSTY;
	inventory[0].quantity = 1;
	inventory[0].info.damage_min = 0.9f;
	inventory[0].info.damage_max = 1.2f;
	
	inventory[1].id = ITEM_HOE_RUSTY;
	inventory[1].quantity = 1;
	
	inventory[2].id = ITEM_WATER_RUSTY;
	inventory[2].quantity = 1;
	
	inventory[3].id = ITEM_CLIP_RUSTY;
	inventory[3].quantity = 1;
	
	inventory[4].id = ITEM_SEED_SLIME;
	inventory[4].quantity = 10;
	
	shop[0].id = ITEM_SEED_SLIME;
	shop[0].price = 5;
	
	shop[1].id = ITEM_SEED_SPIDER;
	shop[1].price = 10;
	
	shop[2].id = ITEM_SEED_BAT;
	shop[2].price = 10;
	
	shop[3].id = ITEM_SEED_ZOMBIE;
	shop[3].price = 15;
	
	shop[4].id = ITEM_POTION;
	shop[4].price = 7;
}

int measure_gold() {
	int count = 0;
	for(int i = 0; i < INV_SLOTS; ++i) { if(inventory[i].id == ITEM_GOLD) count += inventory[i].quantity; }
	return count;
}

void take_gold(int amount) {
	for(int i = 0; i < INV_SLOTS; ++i) {
		if(inventory[i].id == ITEM_GOLD) {
			inventory[i].quantity -= amount;
			if(inventory[i].quantity <= 0) inventory[i].id = 0;
			if(inventory[i].quantity >= 0) return;
			
			amount += inventory[i].quantity;
			inventory[i].quantity = 0;
		}
	}
}

void lose_gold() {
	take_gold(measure_gold() / 2);
}

int inventory_count(int slot) { return inventory[slot].quantity; }
int inventory_id(int slot) { return inventory[slot].id; }
void inventory_put(int slot, int id, int quantity) {
	inventory[slot].id = id;
	inventory[slot].quantity = quantity;
}

void inventory_put_i(int slot, int id, int quantity, special_info info) {
	inventory[slot].id = id;
	inventory[slot].quantity = quantity;
	inventory[slot].info = info;
}

special_info *inventory_info(int slot) {
	return &inventory[slot].info;
}

static int current_item;

extern void act_null(move_player* mp, entity *e);

void inventory_use() {
	--inventory[current_item].quantity;
	if(inventory[current_item].quantity == 0) {
		inventory_put(current_item, 0, 0);
		player_action = act_null;
		should_draw_cursor = 0;
	}
}

declare_body(draw_inventory, {
	int slot;
	int has_slot;
})

extern float player_display_health;
extern float player_display_mana;

void draw_number(int number, float x, float y) {
	if(number == 0) {
		draw_sprite_f(&sprite_text, x - 3, y, 0);
		return;
	}
	
	while(number) {
		draw_sprite_f(&sprite_text, x - 3, y, (number % 10));
		number /= 10;
		x -= 4;
	}
}

int inventory_open = 0;
int inventory_is_writing = 0;

int inventory_buy = 0;

extern void recolorify();

declare_draw(draw_inventory, {
	uncolorify();
	
	draw_sprite(&sprite_inventory_back, 0, 0);
	
	if(inventory_buy) {
		draw_sprite_f(&sprite_inventory, 0, 0, 3);
		
		colorify(1.f, 1.f, 0.f);
		draw_number(measure_gold(), 17, 2);
		uncolorify();
		
		float x = 21;
		for(int i = 0; i < 10; ++i) {
			if(!shop[i].id) continue;
			draw_sprite(item_sprite(shop[i].id), x, 2);
			
			colorify(1.f, 1.f, 0.f);
			draw_number(shop[i].price, x + 16, 2);
			uncolorify();
			
			if(i == self->slot && self->has_slot) draw_sprite(&sprite_inventory_cursor, x, 2);
			x += 19;
		}
		
		vec cur = screen_cursor();
		
		if(cur.y <= 20 && cur.x > 20) {
			int id = shop[self->slot].id;
			if(id != 0) {
				draw_sprite(&sprite_inventory_text, 0, 42);
				screen_print(item_text(id), 10, 134, 189);
			}
		}
		
	}
	else {
		
		draw_rect((color){ 0.9f, 0.f, 0.f }, 1, 2, 9, 1 + (16 * player_display_health));
		draw_rect((color){ 0.5f, 1.f, 0.5f }, 10, 2, 9, 1 + (16 * player_display_mana));
		
		draw_sprite_f(&sprite_inventory, 0, 0, inventory_open);
		
		float x = 21;
		for(int i = 0; i < 10; ++i) {
			draw_sprite(item_sprite(inventory[i].id), x, 2);
			if(inventory[i].quantity > 1) {
				draw_number(inventory[i].quantity, x + 16, 2);
			}
			if(i == current_item) draw_sprite(&sprite_selector, x - 2, 0);
			if(i == self->slot && self->has_slot) draw_sprite(&sprite_inventory_cursor, x, 2);
			x += 19;
		}
		
		if(inventory_open) {
			
			
			x = 21;
			for(int i = 10; i < 20; ++i) {
				draw_sprite(item_sprite(inventory[i].id), x, 22);
				if(inventory[i].quantity > 1) {
					draw_number(inventory[i].quantity, x + 16, 22);
				}
				if(i == self->slot && self->has_slot) draw_sprite(&sprite_inventory_cursor, x, 22);
				x += 19;
			}
			
			vec cur = screen_cursor();
			draw_sprite(item_sprite(unplaced.id), cur.x - 8, cur.y - 8);
			if(unplaced.quantity > 1) draw_number(unplaced.quantity, cur.x + 8, cur.y - 8);
			
			if(cur.y <= 40 && unplaced.id == 0) {
				int id = inventory[self->slot].id;
				if(id != 0) {
					draw_sprite(&sprite_inventory_text, 0, 42);
					screen_print(item_text(id), 10, 134, 189);
				}
			}
			
		}
	}
	
	recolorify();

})

extern int should_draw_cursor;

int try_buy(draw_inventory *self) {
	int id = shop[self->slot].id;
	int price = shop[self->slot].price;
	
	if(measure_gold() < price) return 0;
	
	if(item_stackable(id)) {
		for(int i = 0; i < INV_SLOTS; ++i) {
			if(inventory[i].id == id) {
				inventory[i].quantity += 1;
				take_gold(price);
				return 1;
			}
		}
	}

	for(int i = 0; i < INV_SLOTS; ++i) {
		if(inventory_id(i) == 0) {
			special_info si;
			inventory_put_i(i, id, 1, si);
			take_gold(price);
			return 1;
		}
	}
	
	return 0;
}

declare_tick(draw_inventory, {
		
	
	if(inventory_buy) {
		self->has_slot = 0;
			
		vec cursor = screen_cursor();
		if(cursor.x > 20 && cursor.y <= 20) {
			int i = floor((cursor.x - 19) / 19);
			self->slot = i;
			self->has_slot = 1;
			
			if(on_down(get_key(LEFT_MOUSE))) {
				
				if(try_buy(self)) {
					
					play_sound(s_inventory_pick);
				}
				else {
					play_sound(s_cant_buy);
				}
			}
		}
		
		highlighted_item_info = inventory[self->slot].info;
	}
	else {
		for(int i = 0; i < 10; ++i) {
			if(on_down(get_key('0' + i))) {
				if(i == 0) current_item = 9;
				else current_item = i - 1;
			}
		}
		
		float y = get_scroll().y;
		if(y < 0) {
			current_item = (current_item + 1) % 10;
		}
		if(y > 0) {
			current_item = (10 + (current_item - 1)) % 10;
		}
		
		player_action = item_action(inventory[current_item].id);
		should_draw_cursor = item_uses_tiles(inventory[current_item].id);
		active_item_info = inventory[current_item].info;
		active_item_id = inventory[current_item].id;
		
		if((!get_component(player, move_player)->can_sleep && !inventory_buy) && on_down(get_key('E'))) {
			inventory_open = !inventory_open;
			if(!inventory_open && unplaced.id != 0) {
				invdrop(unplaced.id, unplaced.quantity, unplaced.info);
				unplaced.id = 0;
				unplaced.quantity = 0;
			}
		}
		
		if(inventory_open) {
			save_tutorial_step(13);
			
			self->has_slot = 0;
			
			vec cursor = screen_cursor();
			if(cursor.x > 20 && cursor.y <= 40) {
				int i = floor((cursor.x - 19) / 19);
				int j = floor(cursor.y / 20);
				self->slot = i + j * 10;
				self->has_slot = 1;
				
				if(on_down(get_key(LEFT_MOUSE))) {
					if(inventory[self->slot].id == unplaced.id && item_stackable(unplaced.id)) {
						inventory[self->slot].quantity += unplaced.quantity;
						unplaced.id = 0;
						unplaced.quantity = 0;
						play_sound(s_inventory_pick);
					}
					else {
						inventory_slot tmp = unplaced;
						unplaced = inventory[self->slot];
						inventory[self->slot] = tmp;
						play_sound(s_inventory_pick);
					}
				}
				if(on_down(get_key(RIGHT_MOUSE))) {
					if(unplaced.id == 0) {
						if(inventory[self->slot].quantity > 1) {
							int half = inventory[self->slot].quantity / 2;
							unplaced = inventory[self->slot];
							unplaced.quantity = half;
							inventory[self->slot].quantity -= half;
							play_sound(s_inventory_pick);
						}
						else if(inventory[self->slot].quantity > 0) {
							inventory_slot tmp = unplaced;
							unplaced = inventory[self->slot];
							inventory[self->slot] = tmp;
							play_sound(s_inventory_pick);
						}
					}
					else if(inventory[self->slot].id == unplaced.id && item_stackable(unplaced.id)) {
						++inventory[self->slot].quantity;
						--unplaced.quantity;
						if(unplaced.quantity == 0) {
							unplaced.id = 0;
							unplaced.quantity = 0;
						}
						play_sound(s_inventory_pick);
					}
					else if(inventory[self->slot].id == 0) {
						inventory[self->slot] = unplaced;
						inventory[self->slot].quantity = 1;
						--unplaced.quantity;
						if(unplaced.quantity == 0) {
							unplaced.id = 0;
							unplaced.quantity = 0;
						}
						play_sound(s_inventory_pick);
					}
				}
			}
			if(cursor.y > 40) {
				if(on_down(get_key(LEFT_MOUSE)) && (unplaced.id != 0)) {
					invdrop(unplaced.id, unplaced.quantity, unplaced.info);
					unplaced.id = 0;
					unplaced.quantity = 0;
				}
			}
			
			highlighted_item_info = inventory[self->slot].info;
		}
		
		inventory_is_writing = (inventory_open && self->has_slot);
	}
})
declare_head(draw_inventory)



entity *create_inventory() {
	entity *inv = ent_new();
	inv->position = vxy(0, 0);
	inv->fixed = 1;
	inv->layer = 1;
	
	draw_inventory *di = c_new(draw_inventory);
	di->slot = 0;
	di->has_slot = 0;
	current_item = 0;
	add_component(inv, di);
	
	unplaced.id = 0;
	unplaced.quantity = 0;
	
	//for(int i = 0; i < 10; ++i) {
	//	set_item(i, null_item());
	//}
	
	//set_item(0, item_m(&sprite_sword_rusty, &act_sword, 0));
	//set_item(1, item_m(&sprite_hoe_rusty, &act_hoe, 1));
	//set_item(2, item_m(&sprite_water_rusty, &act_water, 1));
	//set_item(3, item_m(&sprite_seed_slime, &act_plant, 1));
	//set_item(4, item_m(&sprite_clip_rusty, &act_snip, 1));
	
	return inv;
}