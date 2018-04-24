#ifndef STRENGTH_H_
#define STRENGTH_H_

float strength_prop(float min, float max);
int strength_chance(int num, int denom);
int strength_count(int count);

void strength_inc();
void strength_dec();

float uniform(float a, float b);

#endif