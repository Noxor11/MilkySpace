#pragma once

#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>

#define NUMBER_OF_SONGS 7


extern Mix_Chunk *laser;
extern Mix_Chunk *heavy_bullet;

extern Mix_Chunk *hit;
extern Mix_Chunk *player_hit;
extern Mix_Chunk *low_health;
extern Mix_Chunk *game_over;


extern Mix_Chunk *move_up_menu;
extern Mix_Chunk *move_down_menu;
extern Mix_Chunk *confirm_select;
extern Mix_Chunk *click;



extern Mix_Music* songs[NUMBER_OF_SONGS];

extern int song_playing;


void playMusicOnLoop();

void playNextSong();