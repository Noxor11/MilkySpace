#include "game.h"

int main(){

    srand(time(NULL));

	if(int code = gameScene::initScene() != 0){
		printf("Scene not initialised.\nError code: %d", code);
		sleep(3);
		gameScene::stopScene();
		return -1;
	}

	Game game = Game();
	game.start();

	gameScene::stopScene();

    return 0;
}
