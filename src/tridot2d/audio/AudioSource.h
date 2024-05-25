//
// Copyright (c) 2022 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Audio.h"
#include "systems/EntitySystem.h"

namespace tridot2d {

	class AudioListener : public Component {
	public:
		float volume = 1.0f;

		void update(Entity& ent, float dt) override;
	};

	class AudioSource : public Component {
	public:
		std::shared_ptr<Audio> audio;
		float volume = 1.0f;
		float pitch = 1.0f;
		bool positional = true;
		bool looping = false;
		uint32_t id = 0;

		void play();
		void stop();

		void update(Entity& ent, float dt) override;
	};

}
