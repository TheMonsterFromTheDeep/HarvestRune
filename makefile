# Ccompiler
CC= gcc

# Executable name
BIN= ld41

# Source and build paths
SRC_DIR= ./src
BUILD_DIR= ./build

# Source files
SRCS=main.c window.c event.c images_diffuse.c loops.c particle.c renderer.c vector.c entity.c component.c button.c game.c icon.c player.c screenshot.c ground.c slime.c inventory.c health.c zombie.c zombie_plant.c spider.c fence.c world.c tutorial.c item.c projectile.c memtrack.c plant.c building.c text.c scientist.c barrier.c genworld.c tile.c bat.c strength.c stb_imp.c


# Sprites


# Object files to build
OBJS= $(SRCS:%.c=%.o)

# Dependencies for each source
DEPS= $(OBJS:%.o=$(BUILD_DIR)/%.d)

# Flags for the compiler
CFLAGS= -O2 -Wall -Wno-unused-variable -lglfw3 -lopengl32 -lgdi32 -lpthread -lzgcl -lsdl2 -lSDL2_mixer

# Default path for make install
INSTALL_PATH?=/usr/local

$(BIN) : $(OBJS:%=$(BUILD_DIR)/%)
#build/shoehorn_sdl.o
	$(CC) $^ build/shoehorn_sdl.o -o $@ $(CFLAGS)

-include $(DEPS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -MMD -c $< -o $@ $(CFLAGS)
	
shoehorn:
	gcc -c src/shoehorn_sdl.c -o build/shoehorn_sdl.o -lmingw32 -lsdl2 -lsdl2_mixer

.PHONY : clean install windows sprite
clean :
	rm -rf $(BUILD_DIR)
	rm $(BIN) || rm $(BIN).exe
	
sprite:
	make -C sprite

install : $(BIN)
	cp $(BIN) $(INSTALL_PATH)/bin
    
windows : $(OBJS:%=$(BUILD_DIR)/%)
	$(CC) $^ build/shoehorn_sdl.o -o HarvestRune.exe $(CFLAGS) -mwindows