#include "strength.h"
#include <stdlib.h>

static int enemies;

static float t() {
	float t_ = enemies / 20.f;
	if(t_ > 1.f) t_ = 1.f;
	return t_;
}

float strength_prop(float min, float max) {
	float half = min + ((max - min) / 2.f);
	
	min = min + (half - min) * t();
	max = max + (max - half) * t();
	
	int value = rand() % 10000;
	
	return min + value * 0.0001 * (max - min);
}

float uniform(float a, float b) {
	int value = rand() % 10000;
	
	return a + value * 0.0001 * (b - a);
}

int strength_chance(int num, int denom) {
	denom *= (4 - 3 * t());
	
	return rand() % denom < num;
}

int strength_count(int count) {
	float possible = t() * 0.3f + 0.7f;
	
	int value = rand() % (int)(count * possible);
	return value;
}

void strength_inc() { ++enemies; }
void strength_dec() { --enemies; }