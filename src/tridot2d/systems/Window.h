//
// Copyright (c) 2023 Julian Hinxlage. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <glm/glm/glm.hpp>

namespace tridot2d {

	class Window {
	public:
		bool alwaysRefresh = false;
		glm::vec3 clearColor;

		bool init(int width, int height, const std::string &title, int vsync = 1, bool maximized = false, bool fullscreen = false);
		void update();
		void beginFrame();
		void endFrame();
		void shutdown();
		void close();
		bool isOpen();
		void* getContext();
		void* getNativContext();
		void refresh();

		void setVSync(int interval);
		int getVSync();
		void setPosition(int x, int y);
		void setSize(int x, int y);
		void setTitle(const std::string& title);
		glm::vec2 getMonitorResolution();
		float getWidth();
		float getHight();

	private:
		void* context = nullptr;
		int vsyncInterval = 1;
	};

}
