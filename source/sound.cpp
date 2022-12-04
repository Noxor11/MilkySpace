#include "sound.h"


Mix_Chunk *heavy_bullet;
Mix_Chunk *laser;
Mix_Chunk *hit;
Mix_Chunk *player_hit;
Mix_Chunk *low_health;
Mix_Chunk *game_over;


Mix_Chunk *move_up_menu;
Mix_Chunk *move_down_menu;
Mix_Chunk *confirm_select;
Mix_Chunk *click;




Mix_Music* songs[NUMBER_OF_SONGS];

int song_playing;



void apply_dsp_firm(){
    std::filesystem::create_directory("/3ds");

    if(!std::filesystem::exists("/3ds/dspfirm.cdc")){
        FILE* file = fopen("/3ds/dspfirm.cdc", "w");
        fclose(file);
    }
}

void playNextSong(){
	song_playing = (song_playing + 1) % NUMBER_OF_SONGS;
	Mix_FadeInMusic(songs[song_playing], 0, 300);
}