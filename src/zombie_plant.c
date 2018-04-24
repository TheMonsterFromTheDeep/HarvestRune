#include "zombie.h"
#include "images_diffuse.h"
#include "tag.h"
#include "plant.h"

/*declare_body(zombie_plant, {
	sprite_renderer *renderer;
	ground_logic *ground;
})

declare_draw(zombie_plant, { })
declare_tick(zombie_plant, {
	if(self->renderer->frame <= 22) {
		self->renderer->animate = self->ground->state == GROUND_WET;
	}
	else if(self->renderer->animate) {
		self->renderer->animate = 0;
		
		create_zombie(parent->position.x, parent->position.y, rand() % 2 == 1);
	}
})

declare_head(zombie_plant)*/

entity *create_zombie_plant(float x, float y, ground_logic *ground) {
	entity *ent = ent_new();
	ent->position = vxy(x, y);
	ent->tag = TAG_PLANT;
	
	sprite_renderer *sr = get_animator_c(&sprite_zombie_plant, 21, vxy(8, 0));
	
	plant *sp = get_plant(sr, ground, 22, 8, create_zombie);
	
	add_component(ent, sr);
	add_component(ent, sp);
	
	return ent;
}

/*entity *create_zombie_plant(float x, float y, ground_logic *ground) {
	entity *plant = ent_new();
	plant->position = vxy(x, y);
	plant->tag = TAG_PLANT;
	
	sprite_renderer *sr = get_animator_c(&sprite_zombie_plant, 21, vxy(8, 0));
	
	zombie_plant *sp = c_new(zombie_plant);
	
	sp->renderer = sr;
	sp->ground = ground;
	
	add_component(plant, sr);
	add_component(plant, sp);
	
	return plant;
}*/