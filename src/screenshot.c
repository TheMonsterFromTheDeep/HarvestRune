#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "window.h"

#include <stdio.h>

void take_screenshot(int frame) {
	char buf[256];
	snprintf(buf, 256, "screenshots/screenshot-%04d.png", frame);
	
	unsigned char *rgba = malloc(get_screen_width() * get_screen_height() * sizeof(unsigned char) * 4);
	
	glReadPixels(0, 0, get_screen_width(), get_screen_height(), GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	
	stbi_flip_vertically_on_write(1);
	stbi_write_png(buf, get_screen_width(), get_screen_height(), 4, rgba, 0);
	
	free(rgba);
}