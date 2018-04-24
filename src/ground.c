#include "ground.h"
#include "images_diffuse.h"
#include "tag.h"
#include "tutorial.h"

declare_draw(ground_logic, { })
declare_tick(ground_logic, {
	if(self->state >= GROUND_PREPARED) {
		save_tutorial_step(6);
	}
	
	if(self->can_change_state) {
		
		
		entity *player = ent_first(tagged(0xBEEFBEEF),
			within_radius(
				vadd(parent->position, vxy(8, 8)), 10
			));
			
		if(player && self->state < GROUND_PREPARED) {
			if(self->state <= GROUND_DIRT) {
				self->state = GROUND_DIRT;
				self->transition = 0;
				self->timer = -50;
			}
		}
		else {
			int min_state = 0;
			if(self->child_plant) {
				min_state = GROUND_PREPARED;
			}
			
			if(self->state == min_state) self->transition = 0;
			
			if(self->timer < 10) ++self->timer;
			else {
				self->timer = 0;
				if(self->state > min_state) {
					++self->transition;
					if(self->transition > 7) {
						--self->state;
						self->transition = 0;
						
						self->timer = -300; /* Take a while to transition again */
					}
				}
			}
		}
		
		self->renderer->frame = (self->state * 8) - self->transition;
	}
})

declare_head(ground_logic)

entity *create_ground(float x, float y) {
	entity *ground = ent_new();
	ground->position = vxy(x, y);
	ground->tag = TAG_GROUND;
	
	sprite_renderer *sr = get_renderer(&sprite_ground);
	
	ground_logic *gl = c_new(ground_logic);
	gl->state = GROUND_GRASS;
	gl->can_change_state = 1;
	gl->transition = 0;
	gl->renderer = sr;
	gl->timer = 0;
	gl->child_plant = NULL;
	
	add_component(ground, sr);
	add_component(ground, gl);
	
	return ground;
}