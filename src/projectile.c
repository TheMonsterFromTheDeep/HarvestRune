#include "projectile.h"
#include "component.h"
#include "tag.h"
#include "health.h"
#include "barrier.h"

#include <math.h>

declare_body(projectile, {
	vec velocity;
	float damage;
	int lifetime;
})

declare_draw(projectile, { })
declare_tick(projectile, {
	int collision = 0;
	parent->position = bar_clamp(parent->position, self->velocity, vxy(1, 1), vxy(0, 0), 1, &collision);
	
	if(collision) return 1;
	
	entity *target = ent_first(tagged(TAG_ENEMY), within_radius(parent->position, 6.f));
	if(target) {
		damage(target, self->damage);
		return 1;
	}
	
	
	--self->lifetime;
	if(self->lifetime <= 0) return 1;
})

declare_head(projectile)

entity *create_projectile(vec position, vec velocity, float damage, sprite_renderer *renderer) {
	entity *ent = ent_new();
	ent->position = position;
	
	projectile *p = c_new(projectile);
	p->velocity = velocity;
	p->damage = damage;
	p->lifetime = 250;
	
	rot_sprite_renderer *rs = rot_renderer(renderer);
	rs->rotation = 57.2957795131 * atan2(velocity.y, velocity.x);
	
	add_component(ent, renderer);
	add_component(ent, rs);
	add_component(ent, p);
	
	return ent;
}