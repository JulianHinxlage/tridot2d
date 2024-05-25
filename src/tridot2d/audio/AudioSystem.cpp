//
// Copyright (c) 2022 Julian Hinxlage. All rights reserved.
//

#include "AudioSystem.h"
#include "Audio.h"
#include "AudioSource.h"
#include "common/Log.h"
#include "AL/al.h"
#include "AL/alc.h"

namespace tridot2d {

	void AudioSystem::init() {
        ALCdevice* device = alcOpenDevice(nullptr);

        if (!device) {
            Log::error("could not open audio device");
            return;
        }

        ALCcontext* context = alcCreateContext(device, nullptr);
        if (!context) {
            Log::error("could not create audio context");
            return;
        }
        alcMakeContextCurrent((ALCcontext*)context);

        Log::info("OpenAl version: %s", alGetString(AL_VERSION));
        Log::info("OpenAl vendor: %s", alGetString(AL_VENDOR));

        alListener3f(AL_POSITION, 0, 0, 0);
        alListener3f(AL_VELOCITY, 0, 0, 0);
        alListenerf(AL_GAIN, 1);
	}

	void AudioSystem::update() {
        for (int i = 0; i < sources.size(); i++) {
            uint32_t id = sources[i];
            int state = 0;
            alGetSourcei(id, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED) {
                alDeleteSources(1, &id);
                sources.erase(sources.begin() + i);
                i--;
            }
        }

        for (int i = 0; i < loadingQueue.size(); i++) {
            auto& audio = loadingQueue[i];
            if (audio->getId() != 0) {
                play(audio);
                loadingQueue.erase(loadingQueue.begin() + i);
                i--;
            }
        }
	}

	void AudioSystem::shutdown() {
        for (int i = 0; i < sources.size(); i++) {
            uint32_t id = sources[i];
            alDeleteSources(1, &id);
        }
        loadingQueue.clear();
        sources.clear();

        ALCcontext* context = alcGetCurrentContext();
        ALCdevice* device = alcGetContextsDevice(context);
        alcMakeContextCurrent(nullptr);
        if (context) {
            alcDestroyContext((ALCcontext*)context);
        }
        if (device) {
            alcCloseDevice((ALCdevice*)device);
        }
	}

    uint32_t AudioSystem::play(const std::shared_ptr<Audio>& audio, const glm::vec3& pos) {
        if (!audio) {
            return 0;
        }
        if (audio->getId() == 0) {
            loadingQueue.push_back(audio);
            return 0;
        }
        else {
            uint32_t id = 0;
            alGenSources(1, &id);
            alSourcei(id, AL_BUFFER, audio->getId());
            alSourcei(id, AL_LOOPING, false);
            alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);
            alSourcePlay(id);
            sources.push_back(id);
            return id;
        }
    }

    void AudioSystem::stop(uint32_t id) {
        for (int i = 0; i < sources.size(); i++) {
            if (sources[i] == id) {
                sources.erase(sources.begin() + i);
                i--;
            }
        }
        alDeleteSources(1, &id);
    }

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
