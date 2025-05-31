#include "scene.h"
#include "gameSpecification.h"
#include "gameThreads.h"


#include <sys/types.h>

SpriteSheet spriteSheet;


Settings initSettings() {
	Settings set;
	set.selected = INVERT_X;
	
	set.options.emplace(std::make_pair(INVERT_X, NO));
	set.options.emplace(std::make_pair(INVERT_Y, NO));

	return set;
}

namespace gameScene {

	void initScene() {
		Result code = 0;
		if ((code = start()) < 0) {
			HandleBadResult(code);
		}
	}

	Result start() {
		Result result = 0;

		result += InitROMFS();
		result += InitAudio();
		result += InitGraphics();
		result += textScene::initTextScene();

		if (result != 0) {
			HandleBadResult(result);
		}
		
		return 0;
	}

	void stopScene() {
		puts("stopping scene...");
		exitThreads();
		ExitGraphics();
	}

	void renderScene(RenderTarget* screen) {
		FrameBegin(frame_flags::FRAME_SYNCDRAW);
		TargetClear(screen, Color32(0, 0, 0, 0xFF));
		SceneBegin(screen);
	}

	void draw(Object* object) {
		DrawSprite(object->getSpritePtr());
	}

}
