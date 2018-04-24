#define STB_IMAGE_IMPLEMENTATION
#define STB_PNG_ONLY
#include "stb_image.h"

#include <stdio.h>

const char *filepath = "map.png";
unsigned char *data;

int width;
int height;

FILE *out;

int tiles[500][500];

void put_floor(int x, int y, int type) {
	tiles[x][y] = type;
	//fprintf(out, "    create_floor(%d, %d, %d);\n", x, y, type);
}

int rgb(int x, int y) {
	int index = x + (y * width);
	index *= 4;
	return ((data[index] << 16) | (data[index + 1] << 8) | data[index + 2]);
}

int offset_x = 10;
int offset_y = 29;

void tile(int x, int y) {
	int c = rgb(x, y);
	
	switch(c) {
	case 0x003d9e30: {
		int type = 0;
		if(rgb(x - 1, y) == 0x003d9e30 && rgb(x + 1, y) == 0x003d9e30) {
			type = 3;
		}
		else if(rgb(x - 1, y) == 0x003d9e30 && rgb(x, y + 1) == 0x003d9e30) {
			type = 4;
		}
		else if(rgb(x + 1, y) == 0x003d9e30 && rgb(x, y + 1) == 0x003d9e30) {
			type = 2;
		}
		else if(rgb(x - 1, y) == 0x003d9e30 && rgb(x, y - 1) == 0x003d9e30) {
			type = 7;
		}
		else if(rgb(x + 1, y) == 0x003d9e30 && rgb(x, y - 1) == 0x003d9e30) {
			type = 8;
		}
		else if(rgb(x, y - 1) == 0x003d9e30 && rgb(x, y + 1) == 0x003d9e30 && x < 20) {
			type = 1;
		}
		else if(rgb(x, y - 1) == 0x003d9e30 && rgb(x, y + 1) == 0x003d9e30 && x >= 20) {
			type = 5;
		}
		else if(rgb(x, y - 1) == 0x003d9e30 && x < 20) {
			type = 0;
		}
		else if(rgb(x, y - 1) == 0x003d9e30 && x >= 20) {
			type = 6;
		}
		else if(rgb(x, y + 1) == 0x003d9e30 && x < 20) {
			type = 10;
		}
		else if(rgb(x, y + 1) == 0x003d9e30 && x >= 20) {
			type = 9;
		}
		
		fprintf(out, "    create_fence(%d, %d, %d);\n", (x - offset_x) * 16, (-y + offset_y) * 16, type);
	}
	case 0x00118e00:
		put_floor(x, y, 1);
		break;
		
	case 0x009b8c39:
		put_floor(x, y, 2);
		break;
		
	case 0x00838383:
		put_floor(x, y, 3);
		break;
	}
	
}

int main(int argc, char **argv) {
	//stbi_set_flip_vertically_on_load(1);
	
	int i;
	data = stbi_load(filepath, &width, &height, &i, 4);
	
	float offset = 0;
	
	out = fopen("genworld.c", "w");
	
	fprintf(out, "#include \"world.h\"\n#include \"fence.h\"\n\nvoid init_world() {\n");
	
	for(int x = 0; x < 500; ++x) {
		for(int y = 0; y < 500; ++y) {
			put_floor(x, y, 0);
		}
	}
	
	for(int x = 0; x < width; ++x) {
		for(int y = 0; y < height; ++y) {
			tile(x, y);
		}
	}
	
	fputs("}", out);
	
	fprintf(out, "\n\nint tiles[%d][%d] = {", width, height);
	
	for(int x = 0; x < width; ++x) {
		fputs("{ ", out);
		for(int y = 0; y < height; ++y) {
			fprintf(out, "%d, ", tiles[x][y]);
		}
		fputs(" },", out);
	}
	
	fputs("};\n\nint get_tile(int x, int y) { return tiles[x][y]; }", out);
	
	fclose(out);
}