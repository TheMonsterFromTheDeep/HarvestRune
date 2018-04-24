#include "building.h"
#include "tag.h"
#include "graphics.h"
#include "images_diffuse.h"

#define FRAMES 20

int game_progress = 0;

declare_body(progress_meter, { })
declare_draw(progress_meter, {
	float t = game_progress / 2500.f;
	
	if(t >= 1) {
		draw_sprite(&sprite_youwin, parent->position.x, parent->position.y);
	}
	
	draw_rect(rgb(0.9f, 0.f, 1.f), parent->position.x + 71,parent->position.y + 9,6,34 * t);
	
})
declare_tick(progress_meter, { })
declare_head(progress_meter)

declare_draw(building, {
	if(self->last_entered) {
		draw_sprite_a(self->appearance, parent->position.x, parent->position.y/* - self->height*/, self->timer / (float)FRAMES);
	}
	else {
		draw_sprite_a(self->appearance, parent->position.x, parent->position.y/* - self->height*/, (FRAMES - self->timer) / (float)FRAMES);
	}
})

extern entity *player;

declare_tick(building, {
	self->is_entered = 0;
	
	if(player->position.y > parent->position.y && player->position.y < parent->position.y + self->height) {
		if(player->position.x > parent->position.x && player->position.x < parent->position.x + self->appearance->width) {
			self->is_entered = 1;
		}
	}
	
	if(self->is_entered != self->last_entered) {
		self->timer = FRAMES;
	}
	self->last_entered = self->is_entered;
	
	if(self->timer > 0) --self->timer;
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

entity *create_building(float x, float y, float height, sprite *roof, sprite *floor) {
	entity *top = ent_new();
	top->position = vxy(x, y - height);
	top->tag = TAG_BUILDING;
	
	building *b = get_building(roof);
	b->height = height;
	
	add_component(top, b);
	
	entity *fl = ent_new();
	fl->position = vxy(x, y);
	fl->tag = TAG_FLOOR;
	
	sprite_renderer *sr = get_renderer(floor);
	sr->center.y = height;
	add_component(fl, sr);
	
	if(roof == &sprite_lab_roof) {
		add_component(fl, c_new(progress_meter));
	}
	//add_component(ent, bb);
	
	return top;
}