#include "common.h"

declare_draw(shoot_gun, { })
declare_tick(shoot_gun, {
    if(self->last_shot) --self->last_shot;
    else {
        self->shoot(parent->position);
        self->last_shot = self->cooldown;
    }
})

declare_head(shoot_gun);

declare_draw(particle_killer, { })
declare_tick(particle_killer, {
    --self->life;
    return !self->life;
})
declare_head(particle_killer);

particle_killer *get_particle_killer(int life) {
    particle_killer *pk = c_new(particle_killer);
    pk->life = life;
    return pk;
}

entity *create_explosion(vec pos) {    
	play_sound(sound_explosion);
    entity *e = ent_new();
    e->position = pos;
    e->layer = 2;
    
    add_component(e, create_particle_system(
        5,
        20,
        30,
        vy(5.f),
        vy(-0.1f),
        1.5f,
        &sprite_smoke_particle
    ));
    
    add_component(e, create_particle_system(
        10,
        10,
        40,
        vy(2.f),
        vy(-0.2f),
        3.f,
        &sprite_flame_particle
    ));
    
    add_component(e, get_particle_killer(50));
    
    return e;
}

declare_draw(number_renderer, {
	float x = parent->position.x + self->xoff;
	float y = parent->position.y + self->yoff;
	int v = *self->value;
	do {
		int digit = v % 10;
		v /= 10;
		draw_sprite_f(&sprite_number, x, y, digit);
		x -= 10;
	} while(v);
})

declare_tick(number_renderer, { })

declare_head(number_renderer)

float get_random_y() {
	float y = rand() % 121;
	y = round(y / 30) * 30;
	return y;
}

entity *get_wall(entity *source, int is_dumb) {
	float x = source->position.x;
	float y = source->position.y;
	x -= 1;
	x = floor(x / 30) * 30;
	y = floor(y / 30) * 30;
	
	if(is_dumb) return ent_first(within_radius(vxy(x, y), 1), tagged(TOWER), is_not(with_component(c_type(smasher_flag))));
	return ent_first(within_radius(vxy(x, y), 1), tagged(TOWER));
	
}

entity *get_floor(entity *source) {
	float x = source->position.x;
	float y = source->position.y;
	x += 20;
	x = floor(x / 30) * 30;
	y -= 1;
	y = floor(y / 30) * 30;
	return ent_first(within_radius(vxy(x, y), 1), tagged(TOWER));
}