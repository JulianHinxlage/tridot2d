//
// Copyright (c) 2022 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Audio.h"
#include <glm/glm/glm.hpp>
#include <memory>

namespace tridot2d {

	class AudioSystem {
	public:
		void init();
		void update();
		void shutdown();

		uint32_t play(const std::shared_ptr<Audio>& audio, const glm::vec3& pos = {0, 0, 0});
		void stop(uint32_t id);
	private:
		std::vector<uint32_t> sources;
		std::vector<std::shared_ptr<Audio>> loadingQueue;
	};

}
