//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Application.h"
#include "systems/Window.h"
#include "render/RenderContext.h"
#include "common/Singleton.h"
#include "util/strutil.h"

namespace tridot2d {

	void Application::run() {
		while (window->isOpen()) {
			window->beginFrame();
			update();
			window->endFrame();
		}
	}

	void Application::update() {
		for (auto& layer : layers) {
			if (layer) {
				if (layer->avtive) {
					Singleton::set(layer->entitySystem, false);
					layer->preUpdate();
					layer->entitySystem->update();
					layer->update();
					layer->postUpdate();
				}
			}
		}
	}

	void Application::init(int width, int height, const std::string& title) {
		window = Singleton::get<Window>();
		window->enableGUI = false;
		window->alwaysRefresh = true;
		window->init(width, height, title);
		RenderContext::set(window->getContext());

		for (auto& layer : layers) {
			if (layer) {
				Singleton::set(layer->entitySystem, false);
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
		window->shutdown();
	}

}
