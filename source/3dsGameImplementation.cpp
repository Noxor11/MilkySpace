#include "gameSpecification.h"
#include "sound.h"
#include "textScene.h"

#ifdef __3DS__

    #include <3ds/gfx.h>
    #include <3ds/thread.h>
    #include <3ds/romfs.h>
    #include <3ds/services/hid.h>


    #include <c2d/base.h>
    #include <c3d/renderqueue.h>
    #include <citro2d.h>
    #include <sys/unistd.h>

    #include <sys/types.h>
    #include <filesystem>
    #include <jansson.h>

    struct json_language {
        u8 json_code;
        std::unordered_map<std::string, const char*> strings; 
    };

    std::unordered_map<u8, json_language> languages;
    json_language* current_language;


    TextBuf g_staticBuf;
    Font font;

    extern std::array<Text, TEXT_LINES> g_staticText;
    extern std::array<Text, 10> scoreNumberText;

    void Fade(unsigned int color) {
        C2D_Fade(color);
    }

    u32 Color32(u8 r, u8 g, u8 b, u8 a) {
        return C2D_Color32(r, g, b, a);
    }

    u32 Color32f(float r, float g, float b, float a) {
        return C2D_Color32f(C2D_FloatToU8(r),C2D_FloatToU8(g),C2D_FloatToU8(b),C2D_FloatToU8(a));
    }

    float SetFramerate(float fps) {
        return C3D_FrameRate(fps);
    }

    void EndFrame(u8 flags) {
        C3D_FrameEnd(flags);
    }

    void waitForVBlank() {
		gspWaitForVBlank();
    }

    RenderTarget* CreateScreenTarget(Screen screen, ScreenSide side) {
        return C2D_CreateScreenTarget((gfxScreen_t)screen, (gfx3dSide_t)side);
    }

    void TargetClear(RenderTarget* target, u32 color) {
        C2D_TargetClear(target, color);
    }

    bool FrameBegin(u8 flags) {
        return C3D_FrameBegin(flags);
    }

    void SceneBegin(RenderTarget* target) {
        C2D_SceneBegin(target);
    }

    void SpriteFromSheet(Sprite *sprite, SpriteSheet sheet, size_t index) {
        C2D_SpriteFromSheet(sprite, sheet, index);
    }

    void SpriteSetCenter(Sprite* sprite, float x, float y) {
        C2D_SpriteSetCenter(sprite, x, y);
    }

    void SpriteFromImage(Sprite* sprite, Image image) {
        C2D_SpriteFromImage(sprite, image);
    }

    bool DrawRectangle(
        float x, float y, float z, float w, float h,
        u32 clr0, u32 clr1, u32 clr2, u32 clr3) {
        
        return C2D_DrawRectangle(x, y, z, w, h, clr0, clr1, clr2, clr3);
    }

    bool DrawRectSolid(
        float x, float y, float z, float w, float h,
        u32 clr)
    {
        return DrawRectangle(x,y,z,w,h,clr,clr,clr,clr);
    }

    bool DrawSprite(const Sprite* sprite) {
        return C2D_DrawImage(sprite->image, &sprite->params, NULL);
    }   

    // void DrawText(const Text* text, uint32_t flags, float x, float y, float z, float scaleX, float scaleY, ...) {
    //     C2D_DrawText(text, flags, x, y, z, scaleX, scaleY);
    // }

    void DrawText(const Text* text, uint32_t flags, float x, float y, float z, float scaleX, float scaleY) {
        C2D_DrawText(text, flags, x, y, z, scaleX, scaleY);
    }



    const char* TextFontParse(Text* text, Font font, TextBuf buf, const char* str) {
        return C2D_TextFontParse(text, font, buf, str);
    }

    TextBuf TextBufNew(size_t maxGlyphs) {
        return C2D_TextBufNew(maxGlyphs);
    }

    void TextBufDelete(TextBuf buf) {
        C2D_TextBufDelete(buf);        
    }

    Font FontLoad(const char* filename) {
        return C2D_FontLoad(filename);
    }

    void TextOptimize(const Text* text) {
        C2D_TextOptimize(text);
    }

    void FontFree(Font font) {
        C2D_FontFree(font);
    }

    u64 GetSystemTick() {
        return svcGetSystemTick();
    }

    void PlayChunkOnChannel(uint8_t channel, Mix_Chunk* chunk, int loops) {
        Mix_PlayChannel(channel, chunk, loops);
    }

    void PlayMusic(Mix_Music* music, int loops) {
        Mix_PlayMusic(music, loops);
    }

    void HookMusicFinished(MusicFunction func) {
        Mix_HookMusicFinished(func);
    }

    void PauseMusic() {
        Mix_PauseMusic();
    }

    void ResumeMusic() {
        Mix_ResumeMusic();
    }

    void FadeOutMusic(int ms) {
        Mix_FadeOutMusic(ms);
    }

    void FadeInMusic(Mix_Music* music, int loops, int ms) {
        Mix_FadeInMusic(music, loops, ms);
    }

    Thread CreateThread(ThreadFunction entrypoint, void* arg, size_t stack_size, int prio, int core_id, bool detached) {
        return threadCreate(entrypoint, arg, stack_size, prio, core_id, detached);
    }

    void SleepThread(int64_t ns) {
        svcSleepThread(ns);
    }

    Result GetThreadPriority(int32_t *out, Handle handle) {
        return svcGetThreadPriority(out, handle);
    }

    unsigned int getCurrentThreadHandle() {
        return CUR_THREAD_HANDLE;
    }

    Result JoinThread(Thread thread, u64 timeout_ns) {
        return threadJoin(thread, timeout_ns);
    }

    void FreeThread(Thread thread) {
        return threadFree(thread);
    }

    Result InitROMFS() {
        return romfsInit();
    }

    static void apply_dsp_firm() {
        std::filesystem::create_directory("/3ds");

        if (!std::filesystem::exists("/3ds/dspfirm.cdc")) {
            FILE* file = fopen("/3ds/dspfirm.cdc", "w");
            fclose(file);
        }
    }

    Result InitAudio() {
        apply_dsp_firm();

		if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
			return -1;
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) == -1)
			return -2;

        
		songs[0] = Mix_LoadMUS("romfs:/sound/music/background_loop.mp3");
		songs[1] = Mix_LoadMUS("romfs:/sound/music/background_2.mp3");
		songs[2] = Mix_LoadMUS("romfs:/sound/music/background_3.mp3");
		songs[3] = Mix_LoadMUS("romfs:/sound/music/background_4.mp3");
		songs[4] = Mix_LoadMUS("romfs:/sound/music/background_5.mp3");
		songs[5] = Mix_LoadMUS("romfs:/sound/music/background_6.mp3");
		songs[6] = Mix_LoadMUS("romfs:/sound/music/background_7.mp3");

		
		if (!songs[0]) {
			puts("Music could not be loaded.");
			sleep(3);
			SDL_Quit();
			gfxExit();
			return -5;
		}


		laser 			= Mix_LoadWAV("romfs:/sound/laser_shot.wav");
		heavy_bullet 	= Mix_LoadWAV("romfs:/sound/heavy_bullet.wav");
		hit 			= Mix_LoadWAV("romfs:/sound/8-bit-hit-7_NWM.wav");
		player_hit		= Mix_LoadWAV("romfs:/sound/player_hit.wav");
		game_over 		= Mix_LoadWAV("romfs:/sound/game_over.wav");
		low_health 		= Mix_LoadWAV("romfs:/sound/low_health.wav");

		move_up_menu 	= Mix_LoadWAV("romfs:/sound/move_up.wav");
		move_down_menu 	= Mix_LoadWAV("romfs:/sound/move_down.wav");
		confirm_select 	= Mix_LoadWAV("romfs:/sound/confirm_selection.wav");
		click		 	= Mix_LoadWAV("romfs:/sound/click.wav");

        return 0;
    }

    Result InitGraphics() {
        cfguInit(); // Allow C2D_FontLoadSystem to work
		gfxInitDefault();

		if (!C3D_Init(C3D_DEFAULT_CMDBUF_SIZE) || !C2D_Init(C2D_DEFAULT_MAX_OBJECTS))
			return -3;

		C2D_Prepare();		

		// Load graphics
		spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
		if (!spriteSheet) 
			return -4;
		
        return 0;
    }


    static void initLanguageFonts(u8 language_code) {
		json_error_t error;

		std::string play;

		for (auto& language_file : std::filesystem::directory_iterator("romfs:/languages/")) {
			json_t* object = json_load_file(language_file.path().c_str(), JSON_DECODE_ANY, &error);
			
			json_language language;
			language.json_code = json_integer_value(json_object_get(object, "language_code"));
			
			json_t* strings_obj = json_object_get(object, "strings");

			const char* key;
			json_t* value;

			json_object_foreach(strings_obj, key, value) {
				language.strings[std::string(key)] = json_string_value(value);
			}

			languages[language.json_code] = language;
		}

		// if language not supported
		if (!languages.contains(language_code)) {
			current_language = &languages[CFG_LANGUAGE_EN];
		} else {
			current_language = &languages[language_code];
		}
	}

    Result InitTextScene() {
        u8 language;
        GetSystemLanguage(&language);

		initLanguageFonts(language);

		g_staticBuf = TextBufNew(4096); // support up to 4096 glyphs in the buffer
		font = FontLoad("romfs:/gfx/OmegleRegular-owopB.bcfnt");

		//	------------------------------Set strings---------------------------------
		TextFontParse(&g_staticText[PLAY], 			font, g_staticBuf, current_language->strings["play"]);
		TextFontParse(&g_staticText[SETTINGS_TXT], 	font, g_staticBuf, current_language->strings["settings"]);
		TextFontParse(&g_staticText[NEXT_SONG_TXT], font, g_staticBuf, current_language->strings["next_song"]);
		TextFontParse(&g_staticText[SCORE], 		font, g_staticBuf, current_language->strings["score"]);
		TextFontParse(&g_staticText[TITLE], 		font, g_staticBuf, current_language->strings["title"]);
        TextFontParse(&g_staticText[COLON], 		font, g_staticBuf, current_language->strings["colon"]);
        TextFontParse(&g_staticText[TIME], 			font, g_staticBuf, current_language->strings["time_survived"]);
        TextFontParse(&g_staticText[TRY_AGAIN], 	font, g_staticBuf, current_language->strings["try_again"]);
        TextFontParse(&g_staticText[GO_BACK], 		font, g_staticBuf, current_language->strings["go_back_menu"]);
        TextFontParse(&g_staticText[YOU_WON], 		font, g_staticBuf, current_language->strings["you_won"]);
		TextFontParse(&g_staticText[YOU_LOST], 		font, g_staticBuf, current_language->strings["you_lost"]);
        TextFontParse(&g_staticText[ROUND], 		font, g_staticBuf, current_language->strings["round"]);
        TextFontParse(&g_staticText[NEW_HIGHSCORE], font, g_staticBuf, current_language->strings["new_highscore"]);
        TextFontParse(&g_staticText[NEW_BEST_TIME], font, g_staticBuf, current_language->strings["new_best_time"]);

		// Pause

		TextFontParse(&g_staticText[PAUSED], 		font, g_staticBuf, current_language->strings["paused"]);
		TextFontParse(&g_staticText[RESUME], 		font, g_staticBuf, current_language->strings["resume"]);
		TextFontParse(&g_staticText[EXIT], 			font, g_staticBuf, current_language->strings["exit"]);

		//	Settings

		TextFontParse(&g_staticText[YES], 			font, g_staticBuf, current_language->strings["yes"]);
        TextFontParse(&g_staticText[NO], 			font, g_staticBuf, current_language->strings["no"]);

        TextFontParse(&g_staticText[INVERT_X],		font, g_staticBuf, current_language->strings["invert_x"]);
        TextFontParse(&g_staticText[INVERT_Y], 		font, g_staticBuf, current_language->strings["invert_y"]);

		//	Numbers for score display
		for (int i = 0; i < (int) scoreNumberText.size(); i++) {
			TextFontParse(&scoreNumberText[i], font, g_staticBuf, std::to_string(i).c_str());
		}
		//	---------------------------------------------------------------------------

		// --------------Optimize text strings-------------
		for (Text txt: g_staticText) {
			TextOptimize(&txt);
		}

		for (Text txt: scoreNumberText) {
			TextOptimize(&txt);
		}
		// ----------------------------------------------------

        return 0;
    }

    Result GetSystemLanguage(u8 *language) {
        return CFGU_GetSystemLanguage(language);
    }

    void HandleBadResult(Result result) {
        gfxInitDefault();
        consoleInit(GFX_TOP, NULL);
        printf("Scene not initialised.\nError code: %ld\n\n", result);
        const char* msg;
        switch (result) {
            case -1: msg = "SDL_Audio could not be initialised.";	break;
            case -2: msg = "Could not open audio mixer.";			break;
            case -3: msg = "C2D could not be initialised.";			break;
            case -4: msg = "Spritesheet could not be loaded";		break;
        }
        puts(msg);
        puts("\nPress START to exit.");

        while(aptMainLoop()) {
            hidScanInput();

            u32 kbDown = hidKeysDown();

            if (kbDown & KEY_START)
                break;
        }
		
		switch (result) {
			case -4:
				C3D_Fini();
				C2D_Fini();
			case -3:
				textScene::exitTextScene();
				gfxExit();
				cfguExit();
			case -2: 
				SDL_Quit();		
		}
		
		romfsExit();
		gfxExit();
        exit(1);
	}

    void ExitGraphics() {
        SDL_Quit();
		C2D_Fini();
		C3D_Fini();
		gfxExit();
		textScene::exitTextScene();
		romfsExit();
		
		// Delete graphics
		C2D_SpriteSheetFree(spriteSheet);
    }

    void ExitTextScene() {
        // Delete the text buffers
		TextBufDelete(g_staticBuf);
		FontFree(font);
    }

    bool ShouldNotCloseWindow() {
        return aptMainLoop();
    }

    void ScanInput() {
        hidScanInput();
    }

    int32_t GetKeysDown() {
        return hidKeysDown();
    }

    int32_t GetKeysHeld() {
        return hidKeysHeld();
    }



#endif