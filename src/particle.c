#include "particle.h"
#include "entity.h"
#include <stdlib.h>

declare_draw(particle_system, {
    vec v = parent->position;
    for(int i = self->used; i >= 0; --i) {
        if(self->particles[i].life) {
            vec p = vadd(v, self->particles[i].position);
            draw_sprite_a(self->appearance, p.x, p.y, self->particles[i].alpha);
        }
    }
})

declare_tick(particle_system, {
    particle *particles = self->particles;
    
    if(self->emit) {
        for(int i = 0; i < self->emit_rate; ++i) {
            particles[self->used].life = self->lifetime;
            particles[self->used].position = v0();
            particles[self->used].velocity = vadd(vmul(normalize(vxy(rand() % 100 - 50, rand() % 100 - 50)), self->circle), self->velocity);
            ++self->used;
            if(self->used >= 100) break;
        }
        --self->emit;
    }
    
    for(int i = 0; i < self->used; ++i) {
        if(particles[i].life) {
            particles[i].position = vadd(particles[i].position, particles[i].velocity);
            particles[i].velocity = vadd(particles[i].velocity, self->acceleration);
            particles[i].alpha = particles[i].life / (float)self->lifetime;
            --particles[i].life;
        }
    }
})

declare_head(particle_system);

particle_system *create_particle_system(int emit, int emit_rate, int lifetime, vec vel, vec acceleration, float circle, sprite *app) {
    particle_system *ps = c_new(particle_system);
    ps->emit = emit;
    ps->emit_rate = emit_rate;
    ps->lifetime = lifetime;
    ps->velocity = vel;
    ps->acceleration = acceleration;
    ps->circle = circle;
    ps->appearance = app;
    
    ps->used = 0;
    
    return ps;
}