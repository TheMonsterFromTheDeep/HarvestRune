//14 x 10

#include "tiles.h"
#include "graphics.h"
#include "tag.h"
#include "images_diffuse.h"
#include <math.h>

extern int get_tile(int x, int y);

declare_body(tiles, { })

int offset_y = 26;//-30;
int offset_x = 11;//-10;

declare_draw(tiles, {
	vec c = current_camera();
	
	int ix = (int)floor(c.x / 16);
	int iy = (int)floor(c.y / 16);
	
	for(int x = 0; x < 15; ++x) {
		for(int y = 0; y < 11; ++y) {
			draw_sprite_f(&sprite_world_ground, 16 * (ix + x), 16 * (iy + y), get_tile(offset_x + ix + x, -(offset_y + iy + y)));
		}
	}
})

declare_tick(tiles, { })

declare_head(tiles)

entity *create_tiles() {
	entity *ent = ent_new();
	ent->tag = TAG_FLOOR;

	add_component(ent, c_new(tiles));
	
	return ent;
}