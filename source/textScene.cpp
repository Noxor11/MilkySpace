#include "textScene.h"

C2D_TextBuf g_staticBuf;
std::array<C2D_Text, 21> g_staticText;
std::array<C2D_Text, 10> scoreNumberText;
C2D_Font font;





namespace textScene{

	void initTextScene(void)
	{
		g_staticBuf = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
		font = C2D_FontLoad("romfs:/gfx/OmegleRegular-owopB.bcfnt");


		//	------------------------------Set strings---------------------------------
		C2D_TextFontParse(&g_staticText[PLAY], 			font, g_staticBuf, "Play");
		C2D_TextFontParse(&g_staticText[SETTINGS_TXT], 	font, g_staticBuf, "Settings");
		C2D_TextFontParse(&g_staticText[NEXT_SONG_TXT], font, g_staticBuf, "Next Song");
		C2D_TextFontParse(&g_staticText[SCORE], 		font, g_staticBuf, "Score:");
		C2D_TextFontParse(&g_staticText[TITLE], 		font, g_staticBuf, "The Milky Way\n   Is Far Away");
        C2D_TextFontParse(&g_staticText[COLON], 		font, g_staticBuf, ":");
        C2D_TextFontParse(&g_staticText[TIME], 			font, g_staticBuf, "Time survived:");
        C2D_TextFontParse(&g_staticText[TRY_AGAIN], 	font, g_staticBuf, "Try again");
        C2D_TextFontParse(&g_staticText[GO_BACK], 		font, g_staticBuf, "Go back to menu");
        C2D_TextFontParse(&g_staticText[YOU_WON], 		font, g_staticBuf, "You won!");
		C2D_TextFontParse(&g_staticText[YOU_LOST], 		font, g_staticBuf, "You lost.");
        C2D_TextFontParse(&g_staticText[ROUND], 		font, g_staticBuf, "Round:");
        C2D_TextFontParse(&g_staticText[NEW_HIGHSCORE], font, g_staticBuf, "New Highscore!");
        C2D_TextFontParse(&g_staticText[NEW_BEST_TIME], font, g_staticBuf, "New Best Time!");

		// Pause

		C2D_TextFontParse(&g_staticText[PAUSED], 		font, g_staticBuf, "Paused");
		C2D_TextFontParse(&g_staticText[RESUME], 		font, g_staticBuf, "Resume");
		C2D_TextFontParse(&g_staticText[EXIT], 			font, g_staticBuf, "Exit");

        
		//	Settings

		C2D_TextFontParse(&g_staticText[YES], 			font, g_staticBuf, "< Yes >");
        C2D_TextFontParse(&g_staticText[NO], 			font, g_staticBuf, "< No >");

        C2D_TextFontParse(&g_staticText[INVERT_X],		font, g_staticBuf, "Invert X axis:");
        C2D_TextFontParse(&g_staticText[INVERT_Y], 		font, g_staticBuf, "Invert Y axis:");








		//	Numbers for score display
		for (int i = 0; i < (int) scoreNumberText.size(); i++){
			C2D_TextFontParse(&scoreNumberText[i], font, g_staticBuf, std::to_string(i).c_str());
		}
		//	---------------------------------------------------------------------------


		// --------------Optimize text strings-------------
		for(C2D_Text txt: g_staticText){
			C2D_TextOptimize(&txt);
		}


		for(C2D_Text txt: scoreNumberText){
			C2D_TextOptimize(&txt);
		}
		// ----------------------------------------------------
	}


    void drawTopMenuText(){
		const float size = 1.8;
		// Draw white title below
        C2D_DrawText(&g_staticText[TITLE], C2D_WithColor, 62, TOP_HEIGHT_CENTER - 60, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
	
		// Draw black title above
        C2D_DrawText(&g_staticText[TITLE], 0, 60, TOP_HEIGHT_CENTER - 60, 0.5f, size, size);

	}
	void drawBottomMenuText(){
		const int size = 1;
		C2D_DrawText(&g_staticText[PLAY], C2D_WithColor, 100, BOTTOM_HEIGHT_CENTER - 30, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		C2D_DrawText(&g_staticText[SETTINGS_TXT], C2D_WithColor, 100, BOTTOM_HEIGHT_CENTER, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		C2D_DrawText(&g_staticText[NEXT_SONG_TXT], C2D_WithColor, 100, BOTTOM_HEIGHT_CENTER + 30, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));

	}

	void drawPauseMenuTop(){
		const float size = 1.6;

		C2D_DrawText(&g_staticText[PAUSED], C2D_AtBaseline | C2D_WithColor, 130, TOP_HEIGHT_CENTER + 15, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));

	}

	void drawPauseMenuBottom(){
		const float size = 1;

		C2D_DrawText(&g_staticText[RESUME], 	  C2D_AtBaseline | C2D_WithColor, 100, BOTTOM_HEIGHT_CENTER, 		0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		C2D_DrawText(&g_staticText[SETTINGS_TXT], C2D_AtBaseline | C2D_WithColor, 100, BOTTOM_HEIGHT_CENTER + 30, 	0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		C2D_DrawText(&g_staticText[EXIT], C2D_AtBaseline | C2D_WithColor, 100, BOTTOM_HEIGHT_CENTER + 60, 	0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));

	}

	void drawSettingsMenu(Settings* settings){
        const int size 		= 1;
		const int base_X 	= 50;
		const int base_Y 	= BOTTOM_HEIGHT_CENTER - 60;

		
		//	Static Text
		for(int i = INVERT_X, posY = base_Y; i < INVERT_X + 2; i++, posY += 30){
			if(settings->selected == i){
				C2D_DrawText(&g_staticText[i], 					C2D_WithColor, base_X, posY, 0.5f, size, size, C2D_Color32f(255.0f, 0.0f, 0.0f, 255.0f));
				C2D_DrawText(&g_staticText[settings->options[i]], 	C2D_WithColor, 200, posY, 0.5f, size, size, C2D_Color32f(255.0f, 0.0f, 0.0f, 255.0f));
			}
			else{
				C2D_DrawText(&g_staticText[i], C2D_WithColor, base_X, posY, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
				C2D_DrawText(&g_staticText[settings->options[i]], 	C2D_WithColor, 200, posY, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));

			}

		}

	}


    void drawWonTop(std::string score, int seconds, int round){
        const int size = 2;
        C2D_DrawText(&g_staticText[YOU_WON], C2D_WithColor, 100, 0, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));

		drawRoundSmall(round);
        drawScore(score);
        drawTime(seconds);
    }

    void drawWonBottom(){
		drawDeathMenuBottom();
    }

	void drawNewHighscore(){
	    const float size = 0.7;
		C2D_DrawText(&g_staticText[NEW_HIGHSCORE], C2D_WithColor | C2D_AtBaseline, 120, TOP_HEIGHT_CENTER + 90, 0.5f, size, size, C2D_Color32f(0.0f, 255.0f, 0.0f, 255.0f));
	}

	void drawNewBestTime(){
	    const float size = 0.7;
		C2D_DrawText(&g_staticText[NEW_BEST_TIME], C2D_WithColor | C2D_AtBaseline, 120, TOP_HEIGHT_CENTER + 110, 0.5f, size, size, C2D_Color32f(0.0f, 255.0f, 0.0f, 255.0f));
	}

    void drawDeathMenuTop(std::string score, int seconds, int round){
		const int size = 2;
        C2D_DrawText(&g_staticText[YOU_LOST], C2D_WithColor, 100, 0, 0.5f, size, size, C2D_Color32f(255.0f, 0.0f, 0.0f, 255.0f));

		drawRoundSmall(round);
        drawScore(score);
        drawTime(seconds);
    }

    void drawDeathMenuBottom(){
		const int size = 1;

		C2D_DrawText(&g_staticText[TRY_AGAIN], C2D_WithColor, 60, BOTTOM_HEIGHT_CENTER - 30, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		C2D_DrawText(&g_staticText[GO_BACK], C2D_WithColor, 60, BOTTOM_HEIGHT_CENTER, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));

    }



	void drawScore(std::string score){
		const int size = 1;
        C2D_DrawText(&g_staticText[SCORE], C2D_AtBaseline | C2D_WithColor, 50, TOP_HEIGHT_CENTER, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));


		//	For every char in the score, convert to int and display it
		for(int i = 0, xPos = 250; i < (int) score.length(); i++, xPos += 20){
			int p1 = score[i] - '0';
				C2D_DrawText(&scoreNumberText[p1], C2D_AtBaseline | C2D_WithColor, xPos, TOP_HEIGHT_CENTER, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		}


	}

	void drawTime(int seconds){
		const int size = 1;
        int time = seconds;
        int hour = 0;
        int min = 0;
        int sec = 0;
    

        hour = time/3600;
        time = time%3600;
        min = time/60;
        time = time%60;
        sec = time;


        C2D_DrawText(&g_staticText[TIME], C2D_AtBaseline | C2D_WithColor, 50, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));


		std::string minutesStr = std::to_string(min);
        if(minutesStr.length() == 1)
            minutesStr = "0" + minutesStr;

		std::string hoursStr = std::to_string(hour);
        if(hoursStr.length() == 1)
            hoursStr = "0" + hoursStr;

		std::string secondsStr = std::to_string(sec);
        if(secondsStr.length() == 1)
            secondsStr = "0" + secondsStr;

        int realX = TOP_WIDTH_CENTER + 30;

        //	For every char in minutes, convert to int and display it
		for(int i = 0, xPos = realX; i < (int) hoursStr.length(); i++, xPos += 15){
			int p1 = hoursStr[i] - '0';
				C2D_DrawText(&scoreNumberText[p1], C2D_AtBaseline | C2D_WithColor, xPos, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		}
        
        C2D_DrawText(&g_staticText[COLON], C2D_AtBaseline | C2D_WithColor, realX + 33, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));


        //	For every char in minutes, convert to int and display it
		for(int i = 0, xPos = realX + 40; i < (int) minutesStr.length(); i++, xPos += 15){
			int p1 = minutesStr[i] - '0';
				C2D_DrawText(&scoreNumberText[p1], C2D_AtBaseline | C2D_WithColor, xPos, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		}

        C2D_DrawText(&g_staticText[COLON], C2D_AtBaseline | C2D_WithColor, realX + 37 + 35, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));


		//	For every char in minutes, convert to int and display it
		for(int i = 0, xPos = realX + 80; i < (int) secondsStr.length(); i++, xPos += 15){
			int p1 = secondsStr[i] - '0';
				C2D_DrawText(&scoreNumberText[p1], C2D_AtBaseline | C2D_WithColor, xPos, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		}


	}

	void drawRoundCenter(int round){
		const float size = 1.4;
		C2D_DrawText(&g_staticText[ROUND], C2D_AtBaseline | C2D_WithColor, 120, TOP_HEIGHT_CENTER, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));


		std::string round_str = std::to_string(round);

        //	For every char in round, convert to int and display it
		for(int i = 0, xPos = 240; i < (int) round_str.length(); i++, xPos += 15){
			int p1 = round_str[i] - '0';
				C2D_DrawText(&scoreNumberText[p1], C2D_AtBaseline | C2D_WithColor, xPos, TOP_HEIGHT_CENTER, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		}
	}

	void drawRoundSmall(int round){
		const int size = 1;
		C2D_DrawText(&g_staticText[ROUND], C2D_AtBaseline | C2D_WithColor, 50, TOP_HEIGHT_CENTER + 50, 0.5f, size, size, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));


		std::string round_str = std::to_string(round);

        //	For every char in round, convert to int and display it
		for(int i = 0, xPos = 230; i < (int) round_str.length(); i++, xPos += 15){
			int p1 = round_str[i] - '0';
				C2D_DrawText(&scoreNumberText[p1], C2D_AtBaseline | C2D_WithColor, xPos, TOP_HEIGHT_CENTER + 50, 0.5f, size + 0.2, size + 0.2, C2D_Color32f(255.0f,255.0f,255.0f,255.0f));
		}
	}


	void exitTextScene(void)
	{
		// Delete the text buffers
		C2D_TextBufDelete(g_staticBuf);
		C2D_FontFree(font);

	}
}