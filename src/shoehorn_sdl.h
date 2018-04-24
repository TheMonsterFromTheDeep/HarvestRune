#ifndef SHOEHORN_SDL
#define SHOEHORN_SDL

struct sound;
typedef struct sound sound;

sound* get_sound(const char*);
void play_sound(sound*);

void set_music(const char*);

void sound_init();

#endif