//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Application.h"
#include "render/RenderContext.h"
#include <filesystem>

namespace tridot2d {

	std::string searchPath(std::string path) {
		std::string orig = path;
		for (int i = 0; i < 3; i++) {
			if (std::filesystem::exists(path)) {
				return path;
			}
			path = "../" + path;
		}
		return orig;
	}

	void Application::startup() {
		window = Singleton::get<Window>();
		input = Singleton::get<Input>();
		textureManager = Singleton::get<TextureManager>();
		entitySystem = Singleton::get<EntitySystem>();
		renderer = Singleton::get<Renderer2D>();
		random = Singleton::get<Random>();
		camera = Singleton::get<Camera>();
		time = Singleton::get<Time>();
		audioSystem = Singleton::get<AudioSystem>();
		audioManager = Singleton::get<AudioManager>();
		physicsSystem = Singleton::get<PhysicsSystem>();
		particleSystem = Singleton::get<ParticleSystem>();
		
		window->enableGUI = false;
		window->alwaysRefresh = true;
		window->init(800, 600, "Tridot2D", 1);
		RenderContext::set(window->getContext());

		input->init();
		random->init();
		textureManager->directory = searchPath("assets/textures") + "/";
		audioManager->directory = searchPath("assets/audio") + "/";
		renderer->init(false);
		time->init();
		audioSystem->init();
		physicsSystem->init();
		particleSystem->init();

		init();
	}

	void Application::run() {
		while (window->isOpen()) {
			window->beginFrame();
			input->update();
			time->update();

			camera->aspectRatio = window->getWidth() / window->getHight();
			camera->update(time->deltaTime);
			renderer->begin(camera->getMatrix());

			preUpdate();
			entitySystem->update(time->deltaTime);
			physicsSystem->update(time->deltaTime, 4);
			particleSystem->updaet(time->deltaTime);
			update();

			renderer->end();
			window->endFrame();
			postUpdate();
		}
	}

	void Application::shutdown() {
		RenderContext::set(nullptr);
		window->shutdown();
		entitySystem->clear();
		textureManager->textures.clear();
	}

}
