#include "barrier.h"

#include <stddef.h>

typedef struct {
	float value;
	float end0;
	float end1;
	int strength;
} barrier;

#define B(v, e0, e1) (barrier){v, e0, e1, 1}
#define B0(v, e0, e1) (barrier){v, e0, e1, 0}

barrier vertical[] = {
	/* Fence left */
	B0(6, 3, 325),
	B0(9, 3, 325),
	
	/* Fence right */
	B0(246, 3, 229),
	B0(249, 3, 229),
	
	
	B0(342, 3, 309),
	B0(345, 3, 309),
	
	/* Player house */
	B(256, 0, 47),
	B(304, 0, 47),
	
	/* Bed */
	B0(267, 26, 45),
	
	B0(486, -269, 339),
	B0(-23, -269, 339),
	
	B(128, -176, -128),
	B(208, -176, -128)
};

barrier horizontal[] = {
	/* Fence left */
	B0(3, 6, 9),
	
	/* Fence center */
	B0(309, 6, 345),
	B0(307, 6, 345),
	
	/* Fence right */
	B0(3, 246, 249),
	B0(229, 246, 249),
	
	B0(3, 342, 345),
	
	/* Player house */
	B(0, 256, 283),
	B(0, 294, 304),
	B(47, 256, 304),
	
	/* Bed */
	B0(26, 258, 267),
	
	B0(-262, -23, 486),
	B0(339, -23, 486),
	
	B(-176, 128, 158),
	B(-176, 176, 208),
	B(-128, 128, 208)
};

static float epsilon = 0.005f;

vec bar_clamp(vec pos, vec step, vec size, vec center, int strength, int *flag) {
	size_t vlen = sizeof(vertical) / sizeof(vertical[0]);
	size_t hlen = sizeof(horizontal) / sizeof(horizontal[0]);
	
	vec total = vadd(pos, step);
	
	float xhp = center.x + (size.x * 0.5f);
	float xhm = center.x - (size.x * 0.5f);
	float yhp = center.y + (size.y * 0.5f);
	float yhm = center.y - (size.y * 0.5f);
	
	int flag_value = 0;
	
	for(size_t i = 0; i < vlen; ++i) {
		barrier b = vertical[i];
		if(strength <= b.strength) {
			if(pos.y >= b.end0 && pos.y <= b.end1) {
				if(pos.x <= b.value && (total.x + xhp) > b.value) {
					total.x = b.value - epsilon - xhp;
					flag_value = 1;
				}
				if(pos.x >= b.value && (total.x + xhm) < b.value) {
					total.x = b.value + epsilon - xhm;
					flag_value = 1;
				}
			}
		}
	}
	
	for(size_t i = 0; i < hlen; ++i) {
		barrier b = horizontal[i];
		if(strength <= b.strength) {
			if(pos.x >= b.end0 && pos.x <= b.end1) {
				if(pos.y <= b.value && (total.y + yhp) > b.value) {
					total.y = b.value - epsilon - yhp;
					flag_value = 1;
				}
				if(pos.y >= b.value && (total.y + yhm) < b.value) {
					total.y = b.value + epsilon - yhm;
					flag_value = 1;
				}
			}
		}
	}
	
	if(flag) *flag = flag_value;
	
	return total;
}

