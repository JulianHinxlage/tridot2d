//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "ApplicationLayer.h"
#include "EntitySystem.h"
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

	void ApplicationLayer::prepare() {
		Singleton::set(entitySystem, false);
	}

	void ApplicationLayer::init() {}
	
	void ApplicationLayer::preUpdate() {}
	
	void ApplicationLayer::update() {
		entitySystem->update();
	}

	void ApplicationLayer::postUpdate() {}



	void MainLayer::init() {
		ApplicationLayer::init();

		Singleton::get<Time>()->init();
		Singleton::get<Input>()->init();
		Singleton::get<Random>()->init();
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
		ApplicationLayer::prepare();

		auto *time = Singleton::get<Time>();
		time->update();
		Singleton::get<Input>()->update();
		Singleton::get<PhysicsSystem>()->update(time->deltaTime, 4);
		Singleton::get<AudioSystem>()->update();
		Singleton::get<ParticleSystem>()->update();
	}



	SceneLayer::SceneLayer() {
		camera = new Camera();
	}

	SceneLayer::~SceneLayer() {
		if (camera) {
			delete camera;
			camera = nullptr;
		}
	}

	void SceneLayer::prepare() {
		ApplicationLayer::prepare();
		Singleton::set(camera, false);
	}

	void SceneLayer::init() {
		ApplicationLayer::init();
	}

	void SceneLayer::preUpdate() {
		ApplicationLayer::preUpdate();
		camera->update();
		auto* renderer = Singleton::get<Renderer2D>();
		renderer->begin(camera->getMatrix());
	}

	void SceneLayer::postUpdate() {
		ApplicationLayer::postUpdate();
		auto* renderer = Singleton::get<Renderer2D>();
		renderer->end();
	}



	void UiLayer::preUpdate() {
		ApplicationLayer::preUpdate();
		camera->update();
		camera->position = camera->resolution * 0.5f;
		camera->scale = camera->resolution * 0.5f;
		camera->scale.x /= camera->aspectRatio;
		auto* renderer = Singleton::get<Renderer2D>();
		renderer->begin(camera->getMatrix());
	}

	void UiLayer::postUpdate() {
		ApplicationLayer::postUpdate();

		auto* renderer = Singleton::get<Renderer2D>();
		renderer->end();
	}

}
