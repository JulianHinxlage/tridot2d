//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "systems/Window.h"
#include "systems/Input.h"
#include "systems/EntitySystem.h"
#include "systems/Random.h"
#include "render/Renderer2D.h"
#include "systems/Time.h"
#include "audio/AudioSystem.h"
#include "physics/PhysicsSystem.h"
#include "Camera.h"
#include "particles/ParticleSystem.h"
#include "components/commonComponents.h"

namespace tridot2d {

	class Application {
	public:
		Input *input = nullptr;
		Window* window = nullptr;
		EntitySystem* entitySystem = nullptr;
		TextureManager* textureManager = nullptr;
		Renderer2D* renderer = nullptr;
		Random* random = nullptr;
		Camera* camera = nullptr;
		Time* time = nullptr;
		AudioSystem* audioSystem = nullptr;
		AudioManager* audioManager = nullptr;
		PhysicsSystem* physicsSystem = nullptr;
		ParticleSystem* particleSystem = nullptr;

		void startup();
		void run();
		void shutdown();

		virtual void preUpdate() {};
		virtual void update() {};
		virtual void postUpdate() {};

		virtual void init() {};
	};

}
