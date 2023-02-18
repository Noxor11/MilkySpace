#pragma once
#include "scene.h"

#include <charconv>
#include <unordered_map>
#include <array>
#include <string>
#include <jansson.h>

struct Settings;

namespace textScene{

    #define NO 			    0
    #define YES 		    1
    #define PLAY 		    2
    #define SETTINGS_TXT 	3
    #define NEXT_SONG_TXT 	4
    #define SCORE 		    5
    #define TITLE 		    6
    #define COLON 		    7
    #define TIME 		    8
    #define TRY_AGAIN 	    9
    #define GO_BACK 	    10
    #define YOU_WON 	    11
    #define YOU_LOST 	    12
    #define ROUND    	    13
    #define NEW_HIGHSCORE   14
    #define NEW_BEST_TIME   15

    //  Pause
    #define PAUSED         	16
    #define RESUME         	17
    #define EXIT         	18


    //  Settings
    #define INVERT_X 	    19
    #define INVERT_Y 	    20


        
    
    void initTextScene(u8 language_code);   
    void initLanguageFonts(u8 language_code); 
    void drawTopMenuText();
    void drawBottomMenuText();
    
    void drawSettingsMenu(Settings* settings);

	void drawPauseMenuTop();
    void drawPauseMenuBottom();

    void drawWonTop(std::string score, int seconds, int round);
    void drawWonBottom();

    void drawNewHighscore();
	void drawNewBestTime();

    void drawDeathMenuTop(std::string score, int seconds, int round);
    void drawDeathMenuBottom();


    void drawTime(int seconds);
    void drawScore(std::string score);
    void drawRoundCenter(int round);
    void drawRoundSmall(int round);

   

    void exitTextScene(void);

    
}