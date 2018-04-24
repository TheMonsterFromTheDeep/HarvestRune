#include "item.h"
#include "images_diffuse.h"
#include "button.h"
#include "inventory.h"
#include "tutorial.h"
#include "slime.h"
#include "zombie.h"
#include "bat.h"
#include "spider.h"
#include "sounds.h"

#include <stdlib.h>

static item itemdata[22];

#define I(id, ...) itemdata[id] = (item){__VA_ARGS__}

extern void act_sword(move_player*,entity*);
extern void act_hoe(move_player*,entity*);
extern void act_water(move_player*,entity*);
extern void act_plant(move_player*,entity*);
extern void act_snip(move_player*,entity*);
extern void act_shoot(move_player*,entity*);
extern void act_potion(move_player*,entity*);

void act_null(move_player* mp, entity *e) { }

const char const *descriptions[] = {
	"Empty\n\nYou should never see this description.",
	"$1Gold Coin\n$BValue in real-world money: 0.0\n\n$NSomehow, gold coins are incredibly common. Every single enemy seems to be carrying some, despite coming from a plant.",
	"$PSlime Protoplasm\n$BCollect & talk to Scientist to advance the game\n\n$NNot jello.",
	"$gRusty Sword\n$RDamage: %m-%M\n\n$NForged poorly and left in the rain, this sword has not seen any glory, and won't.",
	"$gRusty Hoe\n\n$NOriginally intended to be a toy, this hoe can only barely scratch the ground.",
	"$gRusty Watering Can\n\n$NOne would imagine a watering can isn't the most likely item to rust, but this one has.",
	"$gRusty Clippers\n\n$NBe careful! If you cut your finger you might get tetanus.",
	"$gSlime Seeds\n$BGrows slime plants\n\n$NThe first ever plant seed genetically engineered to grow an animal! Or, at least, moving creature. Not exactly an animal.",
	"$1Magic Missile Wand\n$RDamage: %m-%M\n$MMana cost: %C\n\n$NA straightforward magical projectile. Hardly more dangerous than a $gRusty Sword$N, which is rather disappointing.",
	"$1Wooden Sword\n$RDamage: %m-%M\n\n$NA wooden club in the shape of a sword. In fact, a $1Wooden Club$N is better.",
	"$2Iron Sword\n$RDamage: %m-%M\n\n$NA giant knife. Should not be used for food preparation.",
	"$gRusty Dagger\n$RDamage: %m-%M\n\n$NCould well be a sword whose front end crumbled.",
	"$1Wooden Dagger\n$RDamage: %m-%M\n\n$NTales are told of the warrior whose wooden dagger was the key piece of kindling to burn the enemy stronghold.",
	"$2Iron Dagger\n$RDamage: %m-%M\n\n$NA short, pointy weapon; should not be used by anyone with a short, pointy temper.",
	"$1Zombie Seeds\n$BGrows zombie plants\n\n$NNow you too can have the ability to cause the apocalypse.",
	"$1Spider Seeds\n$BGrows spider plants\n\n$NWee little bugs; big teeth.",
	"$1Bat Seeds\n$BGrows bat plants\n\n$NThe perfect companion for any budding vampire.",
	"$PSpider Leg\n$BCollect & talk to Scientist to advance the game\n\n$NNot quite as tasty as a chicken leg.",
	"$PZombie Bone\n$BCollect & talk to Scientist to advance the game\n\n$NOne would think that a bone should be dropped by a skeleton. As it turns out, skeletons are merely a class of zombie.",
	"$PBat Wing\n$BCollect & talk to Scientist to advance the game\n\n$NNo, not the superhero. Just a plain bat wing. Strangely, a bat can drop more than two.",
	"$2Health Potion\n$BHeals one-third of your health\n\n$NSure, you can just sleep in your bed, but the intense battlefield of the farm leaves no time to rest.",
	"$2Wooden Club\n$RDamage: %m-%M\n\n$NNot to be confused with a nightclub, even when used at night."
};

void init_items() {
	I(ITEM_NONE, &sprite_drop_gold, &sprite_item_none, 0, act_null, 0);
	I(ITEM_GOLD, &sprite_drop_gold, &sprite_item_gold, 1, act_null, 0);
	I(ITEM_SLIME, &sprite_drop_slime, &sprite_item_slime, 1, act_null, 0);
	I(ITEM_SWORD_RUSTY, &sprite_drop_sword, &sprite_sword_rusty, 0, act_sword, 0);
	I(ITEM_HOE_RUSTY, &sprite_drop_hoe, &sprite_hoe_rusty, 0, act_hoe, 1);
	I(ITEM_WATER_RUSTY, &sprite_drop_water, &sprite_water_rusty, 0, act_water, 1);
	I(ITEM_CLIP_RUSTY, &sprite_drop_clip, &sprite_clip_rusty, 0, act_snip, 1);
	I(ITEM_SEED_SLIME, &sprite_drop_seed_slime, &sprite_seed_slime, 1, act_plant, 1);
	I(ITEM_MAGIC_MISSILE_WAND, &sprite_drop_magic_missile_wand, &sprite_magic_missile_wand, 0, act_shoot, 0);
	I(ITEM_SWORD_WOOD, &sprite_drop_sword, &sprite_sword_wood, 0, act_sword, 0);
	I(ITEM_SWORD_IRON, &sprite_drop_sword, &sprite_sword_iron, 0, act_sword, 0);
	I(ITEM_DAGGER_RUSTY, &sprite_drop_dagger, &sprite_dagger_rusty, 0, act_sword, 0);
	I(ITEM_DAGGER_WOOD, &sprite_drop_dagger, &sprite_dagger_wood, 0, act_sword, 0);
	I(ITEM_DAGGER_IRON, &sprite_drop_dagger, &sprite_dagger_iron, 0, act_sword, 0);
	I(ITEM_SEED_ZOMBIE, &sprite_drop_seed_zombie, &sprite_seed_zombie, 1, act_plant, 1);
	I(ITEM_SEED_SPIDER, &sprite_drop_seed_spider, &sprite_seed_spider, 1, act_plant, 1);
	I(ITEM_SEED_BAT, &sprite_drop_seed_bat, &sprite_seed_bat, 1, act_plant, 1);
	I(ITEM_SPIDER_LEG, &sprite_drop_spider_leg, &sprite_spider_leg, 1, act_null, 0);
	I(ITEM_ZOMBIE_BONE, &sprite_drop_zombie_bone, &sprite_zombie_bone, 1, act_null, 0);
	I(ITEM_BAT_WING, &sprite_drop_bat_wing, &sprite_bat_wing, 1, act_null, 0);
	I(ITEM_POTION, &sprite_drop_potion, &sprite_potion, 1, act_potion, 0);
	I(ITEM_WOODEN_CLUB, &sprite_drop_club_wood, &sprite_club_wood, 0, act_sword, 0);
}

sprite *item_sprite(int id) {
	return itemdata[id].in_inventory;
}

int item_stackable(int id) {
	return itemdata[id].can_stack;
}

void (*item_action(int id))(move_player*,entity*) {
	return itemdata[id].associated_action;
}

int item_uses_tiles(int id) {
	return itemdata[id].uses_tiles;
}

const char *item_text(int id) {
	return descriptions[id];
}

entity *create_spider_plant(float x, float y, ground_logic *gl);

plant_creator item_plant(int id) {
	if(id == ITEM_SEED_SLIME) return create_slime_plant;
	if(id == ITEM_SEED_BAT) return create_bat_plant;
	if(id == ITEM_SEED_SPIDER) return create_spider_plant;
	if(id == ITEM_SEED_ZOMBIE) return create_zombie_plant;
	return NULL;
}

declare_draw(drop, { })
declare_tick(drop, {
	vec check = vadd(parent->position, vxy(self->x, self->y));
	vec cur = world_cursor();
	
	int select = (len2(vsub(check, cur)) < 5 * 5);
	self->renderer->frame = select;
	
	item i = itemdata[self->id];
	
	if(select) {
		if(on_down(get_key(RIGHT_MOUSE))) {			
			if(itemdata[self->id].can_stack) {
				for(int i = 0; i < INV_SLOTS; ++i) {
					if(inventory_id(i) == self->id) {
						int quantity = inventory_count(i);
						quantity += self->quantity;
						inventory_put_i(i, self->id, quantity, self->info);
						save_tutorial_step(11);
						play_sound(s_pickup_drop);
						return 1;
					}
				}
			}
			
			for(int i = 0; i < INV_SLOTS; ++i) {
				if(inventory_id(i) == 0) {
					inventory_put_i(i, self->id, self->quantity, self->info);
					save_tutorial_step(11);
					play_sound(s_pickup_drop);
					return 1;
				}
			}
		}
	}
})
declare_head(drop)

entity *create_drop(float x, float y, int id, int quantity, special_info info) {
	entity *ent = ent_new();
	
	ent->position = vxy(x, y);
	
	drop *d = c_new(drop);
	d->id = id;
	d->quantity = quantity;
	d->info = info;
	
	sprite *ap = itemdata[id].on_ground;
	d->x = ap->width * 0.5f;
	d->y = ap->height * 0.5f;
	sprite_renderer *sr = get_renderer(ap);
	
	d->renderer = sr;
	
	add_component(ent, d);
	add_component(ent, sr);
	
	return ent;
}

entity *qdrop(entity *e, int id, int quantity) {
	if(quantity < 1) return NULL;
	float dx = ((rand() % 8000) - 4000) * 0.001f;
	float dy = ((rand() % 8000) - 4000) * 0.001f;
	
	special_info fo;
	
	return create_drop(e->position.x + dx, e->position.y + dy, id, quantity, fo);
}

entity *qdropi(entity *e, int id, int quantity, special_info info) {
	float dx = ((rand() % 8000) - 4000) * 0.001f;
	float dy = ((rand() % 8000) - 4000) * 0.001f;
	
	return create_drop(e->position.x + dx, e->position.y + dy, id, quantity, info);
}

extern entity *player;

entity *invdrop(int id, int quantity, special_info info) {
	float dx = ((rand() % 16000) - 8000) * 0.001f;
	float dy = ((rand() % 16000) - 8000) * 0.001f;
	
	return create_drop(player->position.x + dx, player->position.y + dy, id, quantity, info);
}