
#include "systems/Application.h"
#include "common/Log.h"

using namespace tridot2d;

class BoxEntity : public Entity {
public:
	BoxEntity(const glm::vec2 position = { 0, 0 }, int color = 0) {
		this->position = position;

		Color cs[] = {
			Color(0xC3FF93),
			Color(0xFFDB5C),
			Color(0xFFAF61),
			Color(0xFF70AB),
		};
		Color c = cs[color % 4];

		addComponent(Sprite("", c));
		addComponent(Velocity(Singleton::get<Random>()->getVec2() * 2.0f - 1.0f));
	}
};

class Test : public Application {
public:
	void init() override {
		camera->scale *= 4;
		camera->mouseZoomEnabled = true;
		camera->mouseMoveEnabled = true;

		float size = 40;
		for (int i = 0; i < 2000; i++) {
			entitySystem->addEntity(BoxEntity(random->getVec2() * size - size / 2, i));
		}
	}

	void update() override {
		if (time->frameTicks(1)) {
			Log::trace("FPS: %f", time->framesPerSecond);
		}
	}
};

int main(int argc, char* argv[]) {
	Test app;
	app.startup();
	app.run();
	app.shutdown();
}

#if WIN32
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nCmdShow) {
	return main(1, &lpCmdLine);
}
#endif
