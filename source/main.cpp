#include "game.h"
#include "scene.h"
#include <cstdio>

#ifdef __wasm__
#include <emscripten/emscripten.h>


EMSCRIPTEN_KEEPALIVE
void start_game(void* game) {
	Game* g = (Game*)game;
	g->start();
	// gameScene::stopScene();

};

#endif

Game* game = nullptr;

int main() {

    srand(time(NULL));

	gameScene::initScene();
	game = new Game();
	#ifndef __wasm__
	while (game->start());
	#else
	emscripten_set_main_loop_arg(start_game, (void*)game, 60, 0);
	#endif
    return 0;
}
