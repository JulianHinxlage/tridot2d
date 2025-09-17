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


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

namespace tridot2d {

#ifdef __EMSCRIPTEN__
	static Application* g_currentApp = nullptr;
	static void MainLoopWrapper() {
	    if (g_currentApp && g_currentApp->window->isOpen()) {
	        g_currentApp->window->setVSync(2);
	        g_currentApp->window->update();
	        g_currentApp->debugUI->beginFrame();
	        g_currentApp->window->beginFrame();
	        g_currentApp->update();
	        g_currentApp->debugUI->endFrame();
	        g_currentApp->window->endFrame();
	    } else {
	        emscripten_cancel_main_loop();
	    }
	}
#endif

	void Application::run() {
#ifdef __EMSCRIPTEN__
    	g_currentApp = this;
    	emscripten_set_main_loop(MainLoopWrapper, 0, 1);
#else
		while (window->isOpen()) {
			window->update();
			debugUI->beginFrame();
			window->beginFrame();
			update();
			debugUI->endFrame();
			window->endFrame();
		}
#endif
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
