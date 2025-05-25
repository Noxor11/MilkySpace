#include "textScene.h"
#include "gameSpecification.h"
#include "global.h"

#include <array>


std::array<Text, TEXT_LINES> g_staticText;
std::array<Text, 10> scoreNumberText;

namespace textScene {

	Result initTextScene() {		
		return InitTextScene();
	}


    void drawTopMenuText() {
		const float size = 1.8;
		// Draw white title below
        DrawText(&g_staticText[TITLE], text_flags::WithColor, 62, TOP_HEIGHT_CENTER - 60, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
	
		// Draw black title above
        DrawText(&g_staticText[TITLE], 0, 60, TOP_HEIGHT_CENTER - 60, 0.5f, size, size);

	}
	void drawBottomMenuText() {
		const int size = 1;
		DrawText(&g_staticText[PLAY], text_flags::WithColor, 100, BOTTOM_HEIGHT_CENTER - 30, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		DrawText(&g_staticText[SETTINGS_TXT], text_flags::WithColor, 100, BOTTOM_HEIGHT_CENTER, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		DrawText(&g_staticText[NEXT_SONG_TXT], text_flags::WithColor, 100, BOTTOM_HEIGHT_CENTER + 30, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));

	}

	void drawPauseMenuTop() {
		const float size = 1.6;

		DrawText(&g_staticText[PAUSED], text_flags::AtBaseline | text_flags::WithColor, 130, TOP_HEIGHT_CENTER + 15, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));

	}

	void drawPauseMenuBottom() {
		const float size = 1;

		DrawText(&g_staticText[RESUME], 	  text_flags::AtBaseline | text_flags::WithColor, 100, BOTTOM_HEIGHT_CENTER, 		0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		DrawText(&g_staticText[SETTINGS_TXT], text_flags::AtBaseline | text_flags::WithColor, 100, BOTTOM_HEIGHT_CENTER + 30, 	0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		DrawText(&g_staticText[EXIT], text_flags::AtBaseline | text_flags::WithColor, 100, BOTTOM_HEIGHT_CENTER + 60, 	0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));

	}

	void drawSettingsMenu(Settings* settings) {
        const int size 		= 1;
		const int base_X 	= 50;
		const int base_Y 	= BOTTOM_HEIGHT_CENTER - 60;

		
		//	Static Text
		for (int i = INVERT_X, posY = base_Y; i < INVERT_X + 2; i++, posY += 30) {
			if (settings->selected == i) {
				DrawText(&g_staticText[i], 					text_flags::WithColor, base_X, posY, 0.5f, size, size, Color32f(255.0f, 0.0f, 0.0f, 255.0f));
				DrawText(&g_staticText[settings->options[i]], 	text_flags::WithColor, 200, posY, 0.5f, size, size, Color32f(255.0f, 0.0f, 0.0f, 255.0f));
			} else {
				DrawText(&g_staticText[i], text_flags::WithColor, base_X, posY, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
				DrawText(&g_staticText[settings->options[i]], 	text_flags::WithColor, 200, posY, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));

			}

		}

	}


    void drawWonTop(std::string score, int seconds, int round) {
        const int size = 2;
        DrawText(&g_staticText[YOU_WON], text_flags::WithColor, 100, 0, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));

		drawRoundSmall(round);
        drawScore(score);
        drawTime(seconds);
    }

    void drawWonBottom() {
		drawDeathMenuBottom();
    }

	void drawNewHighscore() {
	    const float size = 0.7;
		DrawText(&g_staticText[NEW_HIGHSCORE], text_flags::WithColor | text_flags::AtBaseline, 120, TOP_HEIGHT_CENTER + 90, 0.5f, size, size, Color32f(0.0f, 255.0f, 0.0f, 255.0f));
	}

	void drawNewBestTime() {
	    const float size = 0.7;
		DrawText(&g_staticText[NEW_BEST_TIME], text_flags::WithColor | text_flags::AtBaseline, 120, TOP_HEIGHT_CENTER + 110, 0.5f, size, size, Color32f(0.0f, 255.0f, 0.0f, 255.0f));
	}

    void drawDeathMenuTop(std::string score, int seconds, int round) {
		const int size = 2;
		const int align_x_center = TOP_WIDTH_CENTER - g_staticText[YOU_LOST].width;
        DrawText(&g_staticText[YOU_LOST], text_flags::WithColor, align_x_center, 0, 0.5f, size, size, Color32f(255.0f, 0.0f, 0.0f, 255.0f));

		drawRoundSmall(round);
        drawScore(score);
        drawTime(seconds);
    }

    void drawDeathMenuBottom() {
		const int size = 1;

		DrawText(&g_staticText[TRY_AGAIN], text_flags::WithColor, 60, BOTTOM_HEIGHT_CENTER - 30, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		DrawText(&g_staticText[GO_BACK], text_flags::WithColor, 60, BOTTOM_HEIGHT_CENTER, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));

    }



	void drawScore(std::string score) {
		const int size = 1;
        DrawText(&g_staticText[SCORE], text_flags::AtBaseline | text_flags::WithColor, 50, TOP_HEIGHT_CENTER, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));


		//	For every char in the score, convert to int and display it
		for (int i = 0, xPos = 250; i < (int) score.length(); i++, xPos += 20) {
			int p1 = score[i] - '0';
				DrawText(&scoreNumberText[p1], text_flags::AtBaseline | text_flags::WithColor, xPos, TOP_HEIGHT_CENTER, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		}


	}

	void drawTime(int seconds) {
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


        DrawText(&g_staticText[TIME], text_flags::AtBaseline | text_flags::WithColor, 50, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));


		std::string minutesStr = std::to_string(min);
        if (minutesStr.length() == 1)
            minutesStr = "0" + minutesStr;

		std::string hoursStr = std::to_string(hour);
        if (hoursStr.length() == 1)
            hoursStr = "0" + hoursStr;

		std::string secondsStr = std::to_string(sec);
        if (secondsStr.length() == 1)
            secondsStr = "0" + secondsStr;

        int realX = TOP_WIDTH_CENTER + 30;

        //	For every char in hours, convert to int and display it
		for (int i = 0, xPos = realX; i < (int) hoursStr.length(); i++, xPos += 15) {
			int p1 = hoursStr[i] - '0';
				DrawText(&scoreNumberText[p1], text_flags::AtBaseline | text_flags::WithColor, xPos, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		}
        
        DrawText(&g_staticText[COLON], text_flags::AtBaseline | text_flags::WithColor, realX + 33, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));


        //	For every char in minutes, convert to int and display it
		for (int i = 0, xPos = realX + 40; i < (int) minutesStr.length(); i++, xPos += 15) {
			int p1 = minutesStr[i] - '0';
				DrawText(&scoreNumberText[p1], text_flags::AtBaseline | text_flags::WithColor, xPos, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		}

        DrawText(&g_staticText[COLON], text_flags::AtBaseline | text_flags::WithColor, realX + 37 + 35, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));


		//	For every char in seconds, convert to int and display it
		for (int i = 0, xPos = realX + 80; i < (int) secondsStr.length(); i++, xPos += 15) {
			int p1 = secondsStr[i] - '0';
				DrawText(&scoreNumberText[p1], text_flags::AtBaseline | text_flags::WithColor, xPos, TOP_HEIGHT_CENTER - 40, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		}
	}

	void drawRoundCenter(int round) {
		const float size = 1.4;
		DrawText(&g_staticText[ROUND], text_flags::AtBaseline | text_flags::WithColor, 120, TOP_HEIGHT_CENTER, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));


		std::string round_str = std::to_string(round);

        //	For every char in round, convert to int and display it
		for (int i = 0, xPos = 240; i < (int) round_str.length(); i++, xPos += 15) {
			int p1 = round_str[i] - '0';
				DrawText(&scoreNumberText[p1], text_flags::AtBaseline | text_flags::WithColor, xPos, TOP_HEIGHT_CENTER, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));
		}
	}

	void drawRoundSmall(int round) {
		const int size = 1;
		DrawText(&g_staticText[ROUND], text_flags::AtBaseline | text_flags::WithColor, 50, TOP_HEIGHT_CENTER + 50, 0.5f, size, size, Color32f(255.0f,255.0f,255.0f,255.0f));


		std::string round_str = std::to_string(round);

        //	For every char in round, convert to int and display it
		for (int i = 0, xPos = 230; i < (int) round_str.length(); i++, xPos += 15) {
			int p1 = round_str[i] - '0';
				DrawText(&scoreNumberText[p1], text_flags::AtBaseline | text_flags::WithColor, xPos, TOP_HEIGHT_CENTER + 50, 0.5f, size + 0.2, size + 0.2, Color32f(255.0f,255.0f,255.0f,255.0f));
		}
	}


	void exitTextScene(void) {
		ExitTextScene();
	}
}
