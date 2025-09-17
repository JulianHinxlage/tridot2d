//
// Copyright (c) 2023 Julian Hinxlage. All rights reserved.
//

#include "Window.h"
#include "util/strutil.h"
#include "common/Log.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace tridot2d {

	bool Window::init(int width, int height, const std::string& title, int vsync, bool maximized, bool fullscreen) {
		//init glfw
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) {
			printf("failed to initialize GLFW\n");
			return false;
		}

		//create window
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		if (maximized) {
			glfwWindowHint(GLFW_MAXIMIZED, 1);
		}

		if (fullscreen) {
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			int x = 0;
			int y = 0;
			glfwGetMonitorWorkarea(monitor, &x, &y, &width, &height);
			context = (void*)glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
		}
		else {
			context = (void*)glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		}
		if (!context) {
			printf("failed to create Window\n");
			return false;
		}
		glfwMakeContextCurrent((GLFWwindow*)context);
#if !defined(__EMSCRIPTEN__)
		setVSync(vsync);
#endif

		Log::info("OpenGL version: %s", glGetString(GL_VERSION));
		Log::info("GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		Log::info("GPU vendor: %s", glGetString(GL_VENDOR));
		Log::info("GPU: %s", glGetString(GL_RENDERER));

#if !defined(__EMSCRIPTEN__)
		//init glew
		if (glewInit() != GLEW_OK) {
			Log::error("failed to initialize OpenGL");
			return false;
		}
#endif

		clearColor = { 1, 1, 1 };
		return true;
	}

	void Window::update() {
		if (!alwaysRefresh) {
			glfwWaitEvents();
		}
		glfwPollEvents();
	}

	void Window::beginFrame() {
		int x = 0;
		int y = 0;
		glfwGetFramebufferSize((GLFWwindow*)context, &x, &y);
		glViewport(0, 0, x, y);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::endFrame() {
		glfwSwapBuffers((GLFWwindow*)context);
	}

	void Window::shutdown() {
		if (context) {
			glfwDestroyWindow((GLFWwindow*)context);
			glfwTerminate();
		}
	}

	void Window::close() {
		glfwSetWindowShouldClose((GLFWwindow*)context, 1);
	}

	bool Window::isOpen() {
		return glfwWindowShouldClose((GLFWwindow*)context) == 0;
	}

	void* Window::getContext() {
		return context;
	}

	void* Window::getNativContext() {
#if WIN32
		return glfwGetWin32Window((GLFWwindow*)context);
#else
		return nullptr;
#endif
	}

	void Window::refresh() {
		glfwPostEmptyEvent();
	}

	void Window::setVSync(int interval) {
		glfwSwapInterval(interval);
		vsyncInterval = interval;
	}

	int Window::getVSync() {
		return vsyncInterval;
	}

	void Window::setPosition(int x, int y) {
		glfwSetWindowPos((GLFWwindow*)context, x, y);
	}

	void Window::setSize(int x, int y) {
		glfwSetWindowSize((GLFWwindow*)context, x, y);
	}

	void Window::setTitle(const std::string& title) {
		glfwSetWindowTitle((GLFWwindow*)context, title.c_str());
	}

	glm::vec2 Window::getMonitorResolution() {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		return { mode->width, mode->height };
	}

	float Window::getWidth() {
		int x = 0;
		int y = 0;
		glfwGetWindowSize((GLFWwindow*)context, &x, &y);
		return x;
	}

	float Window::getHight() {
		int x = 0;
		int y = 0;
		glfwGetWindowSize((GLFWwindow*)context, &x, &y);
		return y;
	}

}
