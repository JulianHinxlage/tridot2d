//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Application.h"
#include "EntitySystem.h"
#include "systems/Window.h"
#include "systems/Camera.h"
#include "systems/DebugUI.h"
#include "render/RenderContext.h"
#include "common/Singleton.h"
#include "util/strutil.h"

namespace tridot2d {

	void Application::run() {
		while (window->isOpen()) {
			window->update();
			debugUI->beginFrame();
			window->beginFrame();
			update();
			debugUI->endFrame();
			window->endFrame();
		}
	}

	void Application::update() {
		for (auto& layer : layers) {
			if (layer) {
				if (layer->active) {
					layer->prepare();
					layer->preUpdate();
					layer->update();
					layer->postUpdate();
				}
			}
		}
	}

	void Application::init(int width, int height, const std::string& title, int swapInterval, bool maximized, bool fullscreen) {
		window = Singleton::get<Window>();
		window->alwaysRefresh = true;
		window->init(width, height, title, swapInterval, maximized, fullscreen);
		RenderContext::set(window->getContext());

		debugUI = Singleton::get<DebugUI>();
		debugUI->init();

		for (auto& layer : layers) {
			if (layer) {
				layer->prepare();
				layer->init();
			}
		}
	}

	void Application::shutdown() {
		for (auto& layer : layers) {
			if (layer) {
				delete layer;
				layer = nullptr;
			}
		}
		layers.clear();
		RenderContext::set(nullptr);
		debugUI->shutdown();
		window->shutdown();
	}

}
