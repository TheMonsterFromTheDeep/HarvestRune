#ifndef RD_PARTICLE_H
#define RD_PARTICLE_H

#include "component.h"
#include "vector.h"
#include "graphics.h"

typedef struct {
    vec position;
    vec velocity;
    int life;
    float alpha;
} particle;

declare_body(particle_system, {
    int emit;
    int emit_rate;
    float circle;
    vec acceleration;
    vec velocity;
    int used;
    int lifetime;
    particle particles[100];
    sprite *appearance;
})

particle_system *create_particle_system(int,int,int,vec,vec,float,sprite*);

#endif