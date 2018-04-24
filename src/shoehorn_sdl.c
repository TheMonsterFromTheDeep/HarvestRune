#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdlib.h>
#include <stdio.h>
#include "shoehorn_sdl.h"

struct sound {
    Mix_Chunk *sdl_source;
};

static int should_play = 0;

static Mix_Music *music;

void set_music(const char* path) {
	if(!should_play) return;
	
	if(music) { Mix_FreeMusic(music); music = NULL; }
	music = Mix_LoadMUS(path);
	if(music) {
		Mix_PlayMusic(music, -1);
	}
}

void sound_init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        puts("Note: SDL was not initialized successfully.");
        return;
    }
    
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        puts("Note: could not open audio.");
        return;
    }
    
    should_play = 1;
}

sound *get_sound(const char* path) {
    sound *s = malloc(sizeof(sound));
    s->sdl_source = Mix_LoadWAV(path);
    return s;
}

void play_sound(sound *s) {
	if(should_play) {
		Mix_PlayChannel(-1, s->sdl_source, 0);
	}
}