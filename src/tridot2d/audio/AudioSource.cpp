//
// Copyright (c) 2022 Julian Hinxlage. All rights reserved.
//

#include "AudioSource.h"
#include "AudioSystem.h"
#include "common/Singleton.h"
#include "AL/al.h"
#include "AL/alc.h"

namespace tridot2d {

	void AudioSource::play() {
		if (Singleton::get<AudioSystem>()) {
			if (looping && id != 0) {
				Singleton::get<AudioSystem>()->stop(id);
				id = 0;
			}
			id = Singleton::get<AudioSystem>()->play(audio);
		}
	}

	void AudioSource::stop() {
		if (Singleton::get<AudioSystem>()) {
			Singleton::get<AudioSystem>()->stop(id);
			id = 0;
		}
	}

	void AudioSource::update(Entity& ent) {
		if (id != 0) {
			if (positional) {
				alSourcei(id, AL_SOURCE_RELATIVE, AL_FALSE);
				alSource3f(id, AL_POSITION, ent.position.x, ent.position.y, 0);
			}
			else {
				alSource3f(id, AL_POSITION, 0, 0, 0);
				alSourcei(id, AL_SOURCE_RELATIVE, AL_TRUE);
			}
			alSourcef(id, AL_GAIN, volume);
			alSourcef(id, AL_PITCH, pitch);
			alSourcei(id, AL_LOOPING, looping);
		}
	}

	void AudioListener::update(Entity& ent) {
		alListener3f(AL_POSITION, ent.position.x, ent.position.y, 0);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		alListenerf(AL_GAIN, volume);
	}

}
