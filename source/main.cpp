#include "game.h"

int main() {

    srand(time(NULL));

	gameScene::initScene();
	Game game = Game();
	game.start();
	gameScene::stopScene();

    return 0;
}
