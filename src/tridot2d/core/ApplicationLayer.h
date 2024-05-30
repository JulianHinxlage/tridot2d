//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "EntitySystem.h"

namespace tridot2d {

	class ApplicationLayer {
	public:
		EntitySystem* entitySystem;
		bool avtive = true;

		ApplicationLayer();
		virtual ~ApplicationLayer();
		virtual void init() {};
		virtual void preUpdate() {};
		virtual void update() {};
		virtual void postUpdate() {};
	};

	class MainLayer : public ApplicationLayer {
	public:
		void init() override;
		virtual void preUpdate() override;
		virtual void postUpdate() override;
	};

	class UiLayer : public ApplicationLayer {
	public:
		virtual void preUpdate() override;
		virtual void postUpdate() override;
	};

}
