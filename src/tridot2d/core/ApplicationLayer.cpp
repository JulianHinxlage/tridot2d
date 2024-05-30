//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "ApplicationLayer.h"
#include "common/Singleton.h"

#include "systems/Time.h"
#include "systems/Input.h"
#include "systems/Random.h"
#include "systems/Camera.h"
#include "physics/PhysicsSystem.h"
#include "audio/AudioSystem.h"
#include "particles/ParticleSystem.h"

#include "systems/AssetsManager.h"
#include "render/Renderer2D.h"
#include "render/TextRenderer.h"
#include "util/strutil.h"

namespace tridot2d {

	ApplicationLayer::ApplicationLayer() {
		entitySystem = new EntitySystem();
	}

	ApplicationLayer::~ApplicationLayer() {
		if (entitySystem) {
			delete entitySystem;
			entitySystem = nullptr;
		}
	}

	void MainLayer::init() {
		Singleton::get<Time>()->init();
		Singleton::get<Input>()->init();
		Singleton::get<Random>()->init();
		Singleton::get<Camera>();
		Singleton::get<PhysicsSystem>()->init();
		Singleton::get<AudioSystem>()->init();
		Singleton::get<ParticleSystem>()->init();

		Singleton::get<TextureManager>()->directory = searchPath("assets/textures") + "/";
		Singleton::get<AudioManager>()->directory = searchPath("assets/audio") + "/";
		auto *renderer = Singleton::get<Renderer2D>();
		auto *textRenderer = Singleton::get<TextRenderer>();

		renderer->init(false);
		textRenderer->renderer = renderer;
		textRenderer->init();
		textRenderer->setFont(searchPath("assets/font") + "/font.ttf", 160);
	}

	void MainLayer::preUpdate(){
		auto *time = Singleton::get<Time>();
		time->update();
		Singleton::get<Input>()->update();
		auto* camera = Singleton::get<Camera>();
		camera->update(time->deltaTime);
		Singleton::get<PhysicsSystem>()->update(time->deltaTime, 4);
		Singleton::get<AudioSystem>()->update();
		Singleton::get<ParticleSystem>()->update(time->deltaTime);


		auto* renderer = Singleton::get<Renderer2D>();
		auto* textRenderer = Singleton::get<TextRenderer>();
		renderer->begin(camera->getMatrix());
	}

	void MainLayer::postUpdate() {
		auto* renderer = Singleton::get<Renderer2D>();
		renderer->end();
	}

	void UiLayer::preUpdate() {
		auto* renderer = Singleton::get<Renderer2D>();
		auto* camera = Singleton::get<Camera>();
		renderer->begin(camera->getScreenMatrix());
	}

	void UiLayer::postUpdate() {
		auto* renderer = Singleton::get<Renderer2D>();
		renderer->end();
	}

}
