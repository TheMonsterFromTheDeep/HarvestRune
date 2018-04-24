#include "tutorial.h"
#include "component.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "button.h"

int tutorial_step =-2;
int frame_wait = 0;

extern int inventory_is_writing;

void save_tutorial_step(int step) {
	if(step == tutorial_step) {
		++tutorial_step;
		frame_wait = 1;
	}
}

declare_body(tutorial, {
	sprite_renderer *renderer;
})

declare_draw(tutorial, { })
declare_tick(tutorial, {
	if(tutorial_step <= 5 || tutorial_step >= 14) {
		if(on_down(get_key(' '))) {
			++tutorial_step;
			frame_wait = 1;
		}
	}
	if(frame_wait) --frame_wait;
	else {
		self->renderer->frame = tutorial_step + 2;
	}
	parent->visible = !inventory_is_writing;
	
	if(tutorial_step >= 19) {
		return 1;
	}
})

declare_head(tutorial);

entity *create_tutorial() {
	entity *ent = ent_new();
	ent->fixed = 1;
	ent->layer = 2;
	
	sprite_renderer *sr = get_renderer(&sprite_tutorial);
	tutorial *t = c_new(tutorial);
	t->renderer = sr;
	
	add_component(ent, sr);
	add_component(ent, t);
	
	return ent;
}