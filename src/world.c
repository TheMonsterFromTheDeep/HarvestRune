#include "world.h"
#include "tag.h"
#include "graphics.h"
#include "images_diffuse.h"

entity *create_floor(float x, float y, int type) {
	entity *already_exists = ent_first(within_radius(vxy(x,y), 2), tagged(TAG_FLOOR));
	if(already_exists) return NULL;
	
	entity *floor = ent_new();
	
	floor->position = vxy(x, y);
	floor->tag = TAG_FLOOR;
	
	sprite_renderer *sr = get_renderer(&sprite_world_ground);
	sr->frame = type;
	
	add_component(floor, sr);
	
	return floor;
}

/*void init_world() {
	create_floor(10 * 16, -1 * 16, 1);
	create_floor(11 * 16, -1 * 16, 1);
	
	for(int x = 0; x < 30; ++x) {
		for(int y = -1; y > -5; --y) {
			create_floor(x * 16, y * 16, 0);
		}
	}
}*/