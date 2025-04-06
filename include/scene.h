#pragma once

#include "gameObject.h"
#include "gameThreads.h"
#include "textScene.h"
#include "sound.h"

#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>

#include <fstream>
#include <unistd.h>
#include <unordered_map>

extern C2D_SpriteSheet spriteSheet;


struct Settings {
    int selected;

    std::unordered_map<int, int> options;
};

Settings initSettings();

namespace gameScene {
    void initScene();
    int start();
    void stopScene();
	void stopAndClean(int error);
    void renderScene(C3D_RenderTarget* screen);
    void draw(Object* object);
}
