#include "textScene.h"

#define TEXT_LINES 21

C2D_TextBuf g_staticBuf;
std::array<C2D_Text, TEXT_LINES> g_staticText;
std::array<C2D_Text, 10> scoreNumberText;

C2D_Font font;

struct json_language{
	u8 json_code;
	std::unordered_map<std::string, const char*> strings; 
};

std::unordered_map<u8, json_language> languages;
json_language* current_language;


namespace textScene{

	void initTextScene(u8 language_code){

		initLanguageFonts(language_code);

		g_staticBuf = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
		font = C2D_FontLoad("romfs:/gfx/OmegleRegular-owopB.bcfnt");


		//	------------------------------Set strings---------------------------------
		C2D_TextFontParse(&g_staticText[PLAY], 			font, g_staticBuf, current_language->strings["play"]);
		C2D_TextFontParse(&g_staticText[SETTINGS_TXT], 	font, g_staticBuf, current_language->strings["settings"]);
		C2D_TextFontParse(&g_staticText[NEXT_SONG_TXT], font, g_staticBuf, current_language->strings["next_song"]);
		C2D_TextFontParse(&g_staticText[SCORE], 		font, g_staticBuf, current_language->strings["score"]);
		C2D_TextFontParse(&g_staticText[TITLE], 		font, g_staticBuf, current_language->strings["title"]);
        C2D_TextFontParse(&g_staticText[COLON], 		font, g_staticBuf, current_language->strings["colon"]);
        C2D_TextFontParse(&g_staticText[TIME], 			font, g_staticBuf, current_language->strings["time_survived"]);
        C2D_TextFontParse(&g_staticText[TRY_AGAIN], 	font, g_staticBuf, current_language->strings["try_again"]);
        C2D_TextFontParse(&g_staticText[GO_BACK], 		font, g_staticBuf, current_language->strings["go_back_menu"]);
        C2D_TextFontParse(&g_staticText[YOU_WON], 		font, g_staticBuf, current_language->strings["you_won"]);
		C2D_TextFontParse(&g_staticText[YOU_LOST], 		font, g_staticBuf, current_language->strings["you_lost"]);
        C2D_TextFontParse(&g_staticText[ROUND], 		font, g_staticBuf, current_language->strings["round"]);
        C2D_TextFontParse(&g_staticText[NEW_HIGHSCORE], font, g_staticBuf, current_language->strings["new_highscore"]);
        C2D_TextFontParse(&g_staticText[NEW_BEST_TIME], font, g_staticBuf, current_language->strings["new_best_time"]);

		// Pause

		C2D_TextFontParse(&g_staticText[PAUSED], 		font, g_staticBuf, current_language->strings["paused"]);
		C2D_TextFontParse(&g_staticText[RESUME], 		font, g_staticBuf, current_language->strings["resume"]);
		C2D_TextFontParse(&g_staticText[EXIT], 			font, g_staticBuf, current_language->strings["exit"]);

        
		//	Settings

		C2D_TextFontParse(&g_staticText[YES], 			font, g_staticBuf, current_language->strings["yes"]);
        C2D_TextFontParse(&g_staticText[NO], 			font, g_staticBuf, current_language->strings["no"]);

        C2D_TextFontParse(&g_staticText[INVERT_X],		font, g_staticBuf, current_language->strings["invert_x"]);
        C2D_TextFontParse(&g_staticText[INVERT_Y], 		font, g_staticBuf, current_language->strings["invert_y"]);








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

	void initLanguageFonts(u8 language_code){
		json_error_t error;

		std::string play;


		for (auto& language_file : std::filesystem::directory_iterator("romfs:/languages/")){
			json_t* object = json_load_file(language_file.path().c_str(), JSON_DECODE_ANY, &error);
			
			json_language language;
			language.json_code = json_integer_value(json_object_get(object, "language_code"));
			
			json_t* strings_obj = json_object_get(object, "strings");

			const char* key;
			json_t* value;

			json_object_foreach(strings_obj, key, value){
				language.strings[std::string(key)] = json_string_value(value);
			}

			languages[language.json_code] = language;
		}


		// if language not supported
		if (!languages.contains(language_code)){
			current_language = &languages[CFG_LANGUAGE_EN];
		} else{
			current_language = &languages[language_code];
		}
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
		const int align_x_center = TOP_WIDTH_CENTER - g_staticText[YOU_LOST].width;
        C2D_DrawText(&g_staticText[YOU_LOST], C2D_WithColor, align_x_center, 0, 0.5f, size, size, C2D_Color32f(255.0f, 0.0f, 0.0f, 255.0f));

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