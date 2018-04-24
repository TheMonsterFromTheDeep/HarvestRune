#include "plant.h"

declare_draw(plant, { })
declare_tick(plant, {
	if(self->ground->state == GROUND_WET) {
		save_tutorial_step(8);
	}
	
	if(self->renderer->frame <= self->growth_frame) {
		self->renderer->animate = self->ground->state == GROUND_WET;
	}
	else if(self->renderer->animate) {
		self->renderer->animate = 0;
		
		self->spawn(parent->position.x, parent->position.y);
	}
})

declare_head(plant)

plant *get_plant(sprite_renderer *renderer, ground_logic *ground, int growth_frame, int start_frame, entity* (*spawn)(float x, float y)) {
	plant *p = c_new(plant);
	
	p->renderer = renderer;
	p->ground = ground;
	p->growth_frame = growth_frame;
	p->start_frame = start_frame;
	p->spawn = spawn;
	
	return p;
}

int plant_clip(entity *e) {
	plant *p = get_component(e, plant);
	if(!p) return 0;
	
	if(p->renderer->frame > (p->start_frame + 2) || p->renderer->frame >= p->growth_frame) {
		p->renderer->frame = p->start_frame;
		return 0;
	}
	else {
		ent_destroy(e);
		return 1;
	}
}