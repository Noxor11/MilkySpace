#pragma once

#include "gameSpecification.h"
#include "gameObject.h"
#include "textScene.h"

#include <unistd.h>
#include <unordered_map>

extern SpriteSheet spriteSheet;


struct Settings {
    int selected;

    std::unordered_map<int, int> options;
};

Settings initSettings();

namespace gameScene {
    void initScene();
    Result start();
    void stopScene();
	void stopAndClean(int error);
    void renderScene(RenderTarget* screen);
    void draw(Object* object);
}
