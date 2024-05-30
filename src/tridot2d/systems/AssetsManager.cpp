//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "AssetsManager.h"

namespace tridot2d {

    std::shared_ptr<Texture> TextureManager::get(const std::string& filename) {
        auto i = textures.find(filename);
        if (i != textures.end()) {
            return i->second;
        }
        if (filename == "") {
            textures[filename] = nullptr;
            return nullptr;
        }
        auto texture = std::make_shared<Texture>();
        texture->load(directory + filename);
        textures[filename] = texture;
        return texture;
    };

    std::shared_ptr<Audio> AudioManager::get(const std::string& filename) {
        auto i = audios.find(filename);
        if (i != audios.end()) {
            return i->second;
        }
        if (filename == "") {
            audios[filename] = nullptr;
            return nullptr;
        }
        auto audio = std::make_shared<Audio>();
        audio->load(directory + filename);
        audios[filename] = audio;
        return audio;
    }

}
