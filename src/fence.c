#include "fence.h"
#include "tag.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "world.h"

entity *create_fence(float x, float y, int type) {
	entity *fence = ent_new();
	
	fence->position = vxy(x, y);
	fence->tag = TAG_FENCE;
	
	sprite_renderer *sr = get_renderer(&sprite_fence);
	sr->frame = type;
	
	add_component(fence, sr);
	
	return fence;
}

#define BOX_HEIGHT 20
#define BOX_WIDTH 15

void create_fences() {
	/*create_floor(0, 0, 0);
	create_fence(0, 0, 0);
	
	for(int y = 1; y < BOX_HEIGHT; ++y) {
		create_floor(0, y * 16, 0);
		create_fence(0, y * 16, 1);
		
	}
	create_floor(0, BOX_HEIGHT * 16, 0);
	create_fence(0, BOX_HEIGHT * 16, 2);
	
	for(int x = 1; x < BOX_WIDTH; ++x) {
		create_floor(x * 16, BOX_HEIGHT * 16, 0);
		create_fence(x * 16, BOX_HEIGHT * 16, 3);
		
	}
	create_floor(BOX_WIDTH * 16, BOX_HEIGHT * 16, 0);
	create_fence(BOX_WIDTH * 16, BOX_HEIGHT * 16, 4);

	for(int y = 1; y < BOX_HEIGHT; ++y) {
		create_floor(BOX_WIDTH * 16, y * 16, 0);
		create_fence(BOX_WIDTH * 16, y * 16, 5);
		
	}
	create_floor(BOX_WIDTH * 16, 0, 0);
	create_fence(BOX_WIDTH * 16, 0, 6);*/
	
}