//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "ApplicationLayer.h"
#include <vector>
#include <string>

namespace tridot2d {

	class Application {
	public:
		std::vector<ApplicationLayer*> layers;
		class Window* window;
		class DebugUI* debugUI;

		virtual void run();
		virtual void update();
		virtual void init(int width, int height, const std::string &title, int swapInterval = 1, bool maximized = false, bool fullscreen = false);
		virtual void shutdown();
		
		template<typename Layer>
		Layer* addLayer(Layer* layer) {
			layers.push_back(layer);
			return layer;
		}
	};

}
