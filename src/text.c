#include "graphics.h"
#include "images_diffuse.h"
#include "inventory.h"

#include <math.h>

static void drawchar(float x, float y, char c) {
	if(c < ' ' || c > '~') return;
	
	draw_sprite_f(&sprite_letters, x, y, c - ' ');
}

int no_break(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '.') || (c == ',');
}

#define PRINTF(...) if(deb) printf(__VA_ARGS__)

void screen_print(const char *str, float x, float y, float width) {
	float sx = x;
	float sw = width;
	
	uncolorify();
	
	while(*str) {
		if(*str == '\n') {
			x = sx;
			width = sw;
			y -= 8;
			++str;
			while(*str == ' ') ++str; /* skip spaces after newlines */
			continue;
		}
		
		if(*str == '$') {
			++str;
			switch(*str) {
				case 'P': colorify(0.8f, 0.0f, 1.f); break;
				case 'N': uncolorify(); break;
				case 'g': colorify(0.7f, 0.7f, 0.5f); break;
				case 'R': colorify(0.95f, 0.05f, 0.05f); break;
				case 'B': colorify(0.3f, 0.3f, 0.95f); break;
				case 'M': colorify(0.5f, 1.f, 0.5f); break;
				case '1': colorify(0.7f, 0.8f, 0.85f); break;
				case '2': colorify(0.5f, 0.5f, 0.9f); break;
				default: uncolorify(); break;
			}
			++str;
			continue;
		}
		
		if(*str == '%') {
			++str;
			float value;
			switch(*str) {
				case 'm': value = highlighted_item_info.damage_min; break;
				case 'M': value = highlighted_item_info.damage_max; break;
				case 'C': value = highlighted_item_info.mana_cost; break;
				default: value = 0.f;
			}
			
			int total = (int)(round(value * 10));
			
			int digits = 0;
			int total_ = total;
			while(total_ > 0) {
				++digits;
				total_ /= 10;
			}
			if(digits < 2) digits = 2; /* Always do 0.0 */
			
			float needed = (digits) * 6;
			float advance = needed;
			
			drawchar(x + needed, y, ('0' + (total % 10)));
			drawchar(x + needed - 6, y, '.');
			--digits;
			needed -= 12;
			total /= 10;
			while(digits > 0) {
				drawchar(x + needed, y, ('0' + (total % 10)));
				total /= 10;
				--digits;
				needed -= 6;
			}
			
			++str;
			x += advance + 6;
			continue;
		}
		
		if(no_break(*str)) {
			float needed = 5;
			
			const char *check = str + 1;
			
			while(no_break(*check)) {
				needed += 6;
				++check;
			}
			
			
			if(needed > width) {
				x = sx;
				width = sw;
				y -= 8;
			}
			
			while(no_break(*str)) {
				drawchar(x, y, *str);
				x += 6;
				width -= 6;
				++str;
			}
			//++str;
		}
		else if(*str == ' ') {
			if(5 < width) {
				drawchar(x, y, ' ');
				x += 6;
				width -= 6;
			}
			else {
				x = sx;
				width = sw;
				y -= 8;
			}
			++str;
		}
		else {
			if(5 < width) {
				drawchar(x, y, *str);
				x += 6;
				width -= 6;
			}
			else {
				x = sx;
				width = sw;
				y -= 8;
				drawchar(x, y, *str);
				x += 6;
				width -= 6;
			}
			++str;
		}
		
		
		
		
		/*if(width <= 0) {
			x = sx;
			width = sw;
			y -= 8;
			if((*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z')) {
				drawchar(x, y, '-');
				x += 6;
				width -= 6;
			}
		}*/
	}
	
	uncolorify();
}