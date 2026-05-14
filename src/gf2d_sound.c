#include <gf2d_sound.h>
#include <SDL_mixer.h>
#include "simple_logger.h"
static struct Sound_Manager
{
    Mix_Chunk *hurt_sound;
    Mix_Chunk *die_sound;
} soundManager;

void gf2d_sound_init()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    soundManager.hurt_sound = Mix_LoadWAV("src/explosion.wav");
    soundManager.die_sound = Mix_LoadWAV("src/die.mp3");
    slog("SDL_mixer Error: %s\n", Mix_GetError());
}

void gf2d_sound_play_hurt()
{
    Mix_PlayChannel(-1,soundManager.hurt_sound, 0);;
}

void gf2d_sound_play_die()
{
    Mix_PlayChannel(-1,soundManager.die_sound, 0);
    slog("SDL_mixer Error: %s\n", Mix_GetError());
}