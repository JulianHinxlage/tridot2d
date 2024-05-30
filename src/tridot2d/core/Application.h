//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "ApplicationLayer.h"
/*
#include "systems/Window.h"
#include "systems/Input.h"
#include "core/EntitySystem.h"
#include "systems/Random.h"
#include "render/Renderer2D.h"
#include "systems/Time.h"
#include "audio/AudioSystem.h"
#include "physics/PhysicsSystem.h"
#include "systems/Camera.h"
#include "particles/ParticleSystem.h"
#include "components/commonComponents.h"
#include "render/TextRenderer.h"
*/

namespace tridot2d {

	class Application {
	public:
		std::vector<ApplicationLayer*> layers;
		class Window* window;

		virtual void run();
		virtual void update();
		virtual void init(int width, int height, const std::string &title);
		virtual void shutdown();
		
		template<typename Layer>
		Layer* addLayer(Layer* layer) {
			layers.push_back(layer);
			return layer;
		}
	};

}
