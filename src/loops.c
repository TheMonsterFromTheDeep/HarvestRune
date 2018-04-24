#include "event.h"
#include "entity.h"
#include "component.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "button.h"
#include "player.h"
#include "ground.h"
#include "slime.h"
#include "inventory.h"
#include "spider.h"
#include "fence.h"
#include "world.h"
#include "tutorial.h"
#include "item.h"
#include "building.h"
#include "scientist.h"
#include "tag.h"
#include "tiles.h"
#include "bat.h"
#include "shoehorn_sdl.h"

#include <math.h>

//joystick camera;

entity *player;
vec camera_goal;

extern int inventory_open;

static entity *inv_ent;

float time_of_day = 1440 / 3.f;

static float test_y(entity *e) {
	float y = e->position.y;
	if(e->tag == TAG_FENCE) y += 8;
	return y;
}

int predicate(entity *a, entity *b) {
	int atag = a->tag == TAG_GROUND || a->tag == TAG_FLOOR;
	int btag = b->tag == TAG_GROUND || b->tag == TAG_FLOOR;
	if(atag && !btag) return 1;
	if(btag) return 0;
	
	float ay = test_y(a);
	float by = test_y(b);
	
	return ay > by;
}

entity *menu;

void (*main_callback)(void);

void do_game();

void do_menu() {
	int state = 0;
	
	vec cursor = screen_cursor();
	if(cursor.y < 98) state = 1;
	if(cursor.y < 59) state = 2;
	
	sprite_renderer *sr = get_component(menu, sprite_renderer);
	sr->frame = state;
	
	if(on_down(get_key(LEFT_MOUSE))) {
		if(state != 0) {
			if(state == 2) {
				create_tutorial();
			}
			ent_destroy(menu);
			main_callback = &do_game;
		}
	}
}

int fade_timer = 50;

void set_fade_timer() {
	fade_timer = 0;
}

void set_morning() {
	time_of_day = 1440 / 10.f;
}

sound *s_pickup_drop;
sound *s_inventory_pick;
sound *s_inventory_place;
sound *s_hoe;
sound *s_snip;
sound *s_water;
sound *s_swipe;
sound *s_slime_hit;
sound *s_zombie_hit = NULL;
sound *s_spider_hit = NULL ;
sound *s_bat_hit = NULL;
sound *s_magic_missile;
sound *s_plant;
sound *s_cant_buy;
sound *s_potion;

void init() {

	sound_init();
	set_music("snd/music.wav");
	
	s_pickup_drop = get_sound("snd/pickup_drop.wav");
	s_inventory_pick = get_sound("snd/inventory_pick.wav");
	s_inventory_place = get_sound("snd/inventory_place.wav");
	s_hoe = get_sound("snd/hoe.wav");
	s_snip = get_sound("snd/snip.wav");
	s_water = get_sound("snd/water.wav");
	s_swipe = get_sound("snd/swipe.wav");
	s_slime_hit = get_sound("snd/slime_hit.wav");
	s_zombie_hit = s_spider_hit = s_bat_hit = s_slime_hit;
	s_magic_missile = get_sound("snd/magic_missile.wav");
	s_plant = get_sound("snd/plant.wav");
	s_cant_buy = get_sound("snd/cant_buy.wav");
	s_potion = get_sound("snd/potion.wav");
	
	//camera = joystick_from(key_axis('A', 'D'), key_axis('S', 'W'));
	
	ent_set_predicate(predicate);
	
	init_items();
	
	create_tiles();
	
	for(float x = 0; x < 14; ++x) {
		for(float y = 0; y < 19; ++y) {
			create_ground(16 + x * 16, y * 16);
		}
	}
	
	create_building(16 * 16, 46, 46, &sprite_house_roof, &sprite_house);
	create_building(16 * 8, 16 * -8, 48, &sprite_lab_roof, &sprite_lab);
	
	init_world();
	//create_fences();
	
	player = create_player(); 
	
	create_scientist();
	
	init_inventory();
	
	inv_ent = create_inventory();
	
	menu = ent_new();
	menu->fixed = 1;
	menu->layer = 1;
	add_component(menu, get_renderer(&sprite_menu));
	//
	
	//create_slime_plant(50, 50);
	main_callback = &do_menu;
}

float night[3] = { 0.4f, 0.2f, 0.6f };
float sunrise[3] = { 0.9f, 0.6f, 0.4f };
float noon[3] = { 0.93f, 1.f, 1.f };
float sunset[3] = { 0.9f, 0.5f, 0.3f };

static void interp(float *a, float *b, float *out, float t) {
	out[0] = a[0] + (b[0] - a[0]) * t;
	out[1] = a[1] + (b[1] - a[1]) * t;
	out[2] = a[2] + (b[2] - a[2]) * t;
}

static void do_color(float t) {
	float color[3];
	if(t < 1.f) interp(night, sunrise, color, t);
	else if(t < 2.f) interp(sunrise, noon, color, t - 1);
	else if(t < 3.f) interp(noon, sunset, color, t - 2);
	else interp(sunset, night, color, t - 3);
	
	colorify(color[0], color[1], color[2]);
}

#define UP_FACTOR 10.f
#define U_1 (1.f/UP_FACTOR)

static float up(float t) {
	float c = fmod(t, 1.f);
	if(c < U_1) {
		return t + (UP_FACTOR - 1) * c;
	}
	return t - c + 1;
}

void recolorify() {
	float t = (4 * time_of_day) / 1440;
	
	do_color(up(t));
}

void do_game() {
	time_of_day += 0.1f;//0.16f;
	if(time_of_day >= 1440.f) time_of_day = 0.f;

	float t = (4 * time_of_day) / 1440;
	
	do_color(up(t));
	
	
	//if(inventory_open) {
	//	c_tick(inv_ent);
	//}
	//else {
		ent_process();
	//}

	vec c = current_camera();
	vec offset = vxy(get_cam_width() * 0.5f, get_cam_height() * 0.5f);
	
	camera_goal = vadd(camera_goal, vmul(vsub(player->position, camera_goal), 0.1f));
	c = vsub(camera_goal, offset);
	camera_position(c);
	
	float world_width = 50 * 16;
	float world_height = 30 * 16;
	
	
	//clamp_camera(0, world_width - 2 * offset.x, -20, world_height - 2 * offset.y);
}

void loop() {
	if(fade_timer < 40) {
		float t = fade_timer / 40.f;
		global_colorify(t, t, t);
		++fade_timer;
	} else { global_uncolorify(); }
	
	main_callback();
}