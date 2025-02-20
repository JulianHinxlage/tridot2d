//
// Copyright (c) 2025 Julian Hinxlage. All rights reserved.
//

#pragma once

namespace tridot2d {

	class DebugUI {
	public:
		bool active = false;

		void init();
		void beginFrame();
		void endFrame();
		void shutdown();
		bool isInFrame();

	private:
		void* imguiContext = nullptr;
		bool inFrame = false;
	};

}
