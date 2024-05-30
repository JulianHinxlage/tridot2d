//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "render/Texture.h"
#include "audio/Audio.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace tridot2d {

	class TextureManager {
	public:
		std::string directory;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

		std::shared_ptr<Texture> get(const std::string& filename);
	};

	class AudioManager {
	public:
		std::string directory;
		std::unordered_map<std::string, std::shared_ptr<Audio>> audios;

		std::shared_ptr<Audio> get(const std::string& filename);
	};

}
