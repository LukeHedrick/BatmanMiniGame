#include "Engine.h"

int main() {
	Engine eng;

	if (eng.init() == false) {
		return -1;
	}

	eng.bufferModel();

	eng.loadTextures("textures/dirt.jpg");
	eng.loadTextures("textures/TestTexture.png");
	eng.loadTextures("textures/batman.png");

	if (eng.useShaders() == false) {
		return -1;
	}

	eng.gameLoop();

	std::cin.ignore();
	std::cin.get();
	return 0;
}