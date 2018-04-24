#include "building.h"

#define FRAMES 20

declare_draw(building, {
	draw_sprite_f(self->appearance, parent->position.x, parent->position.y, 1);
	
	if(self->is_entered) {
		draw_sprite_fa(self->appearance, parent->position.x, parent->position.y, 0, self->timer / (float)FRAMES);
	}
	else {
		draw_sprite_fa(self->appearance, parent->position.x, parent->position.y, 0, (FRAMES - self->timer) / (float)FRAMES);
	}
})

declare_tick(building, {
	if(is_entered != last_entered) {
		timer = FRAMES;
	}
	last_entered = is_entered;
	
	if(timer > 0) --timer;
})

declare_head(building);

building *get_building(sprite *s) {
	building *b = c_new(building);
	b->appearance = s;
	
	b->timer = 0;
	b->is_entered = 0;
	b->last_entered = 0;
	
	return b;
}