#include "scene.h"


C2D_SpriteSheet spriteSheet;


Settings initSettings(){
	Settings set;
	set.selected = INVERT_X;
	
	set.options.emplace(std::make_pair(INVERT_X, NO));
	set.options.emplace(std::make_pair(INVERT_Y, NO));

	return set;
}

namespace gameScene{


	int initScene() {

		if(SDL_InitSubSystem(SDL_INIT_AUDIO) == 0){
			printf("There was an error. %s", SDL_GetError());
			sleep(3);
			SDL_Quit();
			gfxExit();
		}


		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
		
		romfsInit();
		cfguInit(); // Allow C2D_FontLoadSystem to work
		gfxInitDefault();
		textScene::initTextScene();
		C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
		C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
		C2D_Prepare();
		

		// Load graphics
		spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
		if (!spriteSheet) svcBreak(USERBREAK_PANIC);
		

		songs[0] = Mix_LoadMUS("romfs:/sound/music/background_loop.mp3");
		songs[1] = Mix_LoadMUS("romfs:/sound/music/background_2.mp3");
		songs[2] = Mix_LoadMUS("romfs:/sound/music/background_3.mp3");
		songs[3] = Mix_LoadMUS("romfs:/sound/music/background_4.mp3");
		songs[4] = Mix_LoadMUS("romfs:/sound/music/background_5.mp3");
		songs[5] = Mix_LoadMUS("romfs:/sound/music/background_6.mp3");
		songs[6] = Mix_LoadMUS("romfs:/sound/music/background_7.mp3");

		
		if(!songs[0]){
			puts("Music could not be loaded.");
			sleep(3);
			SDL_Quit();
			gfxExit();
			return -1;
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

	void stopScene() {


		// tell threads to exit & wait for them to exit
		runThreads = false;
		for (int i = 0; i < NUMTHREADS; i ++)
		{
			threadJoin(threads[i], 1e8);
			threadFree(threads[i]);
		}

		SDL_Quit();
		C2D_Fini();
		C3D_Fini();
		gfxExit();
		textScene::exitTextScene();
		romfsExit();
		



		// Delete graphics
		C2D_SpriteSheetFree(spriteSheet);

		
	}


	void renderScene(C3D_RenderTarget* screen) {


		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(screen, C2D_Color32(0, 0, 0, 0xFF));
		C2D_SceneBegin(screen);
	}

	void draw(Object* object) {
		C2D_DrawSprite(object->getSpritePtr());

	}

}

