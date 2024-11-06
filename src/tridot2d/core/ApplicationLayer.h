//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

namespace tridot2d {

	class ApplicationLayer {
	public:
		class EntitySystem* entitySystem;
		bool active = true;

		ApplicationLayer();
		virtual ~ApplicationLayer();
		virtual void prepare();
		virtual void init();
		virtual void preUpdate();
		virtual void update();
		virtual void postUpdate();
	};

	class MainLayer : public ApplicationLayer {
	public:
		void init() override;
		virtual void preUpdate() override;
	};

	class SceneLayer : public ApplicationLayer {
	public:
		class Camera* camera;

		SceneLayer();
		virtual ~SceneLayer();
		virtual void prepare();
		virtual void init();
		virtual void preUpdate() override;
		virtual void postUpdate() override;
	};

	class UiLayer : public SceneLayer {
	public:
		virtual void preUpdate() override;
		virtual void postUpdate() override;
	};

}
