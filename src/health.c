#include "health.h"
#include "graphics.h"
#include "tutorial.h"

health *get_health(float max, float position, float size, sound *hit) {
	health *h = c_new(health);
	h->max = max;
	h->value = max;
	h->display = max;
	h->position = position;
	h->size = size;
	h->hit = hit;
	
	return h;
}

declare_draw(health, {
	float x = parent->position.x - self->size * 0.5f;
	float y = parent->position.y + self->position;
	draw_rect((color) { 1.f, 0.f, 0.f}, x, y, self->size * (self->display / self->max), 2);
})

declare_tick(health, {
	self->display += (self->value - self->display) * 0.5f;
	if(self->value <= 0) {
		save_tutorial_step(10);
		return 1;
	}
})

declare_head(health)

void damage(entity *e, float amount) {
	health *h = get_component(e, health);
	if(h) {
		h->value -= amount;
		if(h->hit) play_sound(h->hit);
	}
}