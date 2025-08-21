//
// Copyright (c) 2022 Julian Hinxlage. All rights reserved.
//

#include "Audio.h"
#include "common/Log.h"
#include "common/Log.h"
#include "AL/al.h"
#include "AL/alc.h"
#include <fstream>
#include <bit>

namespace tridot2d {

    std::int32_t convert_to_int(char* buffer, std::size_t len)
    {
        std::int32_t a = 0;
        if (std::endian::native == std::endian::little)
            std::memcpy(&a, buffer, len);
        else
            for (std::size_t i = 0; i < len; ++i)
                reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
        return a;
    }

    bool load_wav_file_header(std::ifstream& file,
        std::uint8_t& channels,
        std::int32_t& sampleRate,
        std::uint8_t& bitsPerSample,
        ALsizei& size)
    {
        char buffer[4];
        if (!file.is_open())
            return false;

        // the RIFF
        if (!file.read(buffer, 4))
        {
            Log::error("could not read RIFF");
            return false;
        }
        if (std::strncmp(buffer, "RIFF", 4) != 0)
        {
            Log::error("file is not a valid WAVE file (header doesn't begin with RIFF)");
            return false;
        }

        // the size of the file
        if (!file.read(buffer, 4))
        {
            Log::error("could not read size of file");
            return false;
        }

        // the WAVE
        if (!file.read(buffer, 4))
        {
            Log::error("could not read WAVE");
            return false;
        }
        if (std::strncmp(buffer, "WAVE", 4) != 0)
        {
            std:Log::error("file is not a valid WAVE file (header doesn't contain WAVE)");
            return false;
        }

        // "fmt/0"
        if (!file.read(buffer, 4))
        {
            Log::error("could not read fmt/0");
            return false;
        }

        // this is always 16, the size of the fmt data chunk
        if (!file.read(buffer, 4))
        {
            Log::error("could not read the 16");
            return false;
        }

        // PCM should be 1?
        if (!file.read(buffer, 2))
        {
            Log::error("could not read PCM");
            return false;
        }

        // the number of channels
        if (!file.read(buffer, 2))
        {
            Log::error("could not read number of channels");
            return false;
        }
        channels = convert_to_int(buffer, 2);

        // sample rate
        if (!file.read(buffer, 4))
        {
            Log::error("could not read sample rate");
            return false;
        }
        sampleRate = convert_to_int(buffer, 4);

        // (sampleRate * bitsPerSample * channels) / 8
        if (!file.read(buffer, 4))
        {
            Log::error("could not read (sampleRate * bitsPerSample * channels) / 8");
            return false;
        }

        // ?? dafaq
        if (!file.read(buffer, 2))
        {
            Log::error("could not read dafaq");
            return false;
        }

        // bitsPerSample
        if (!file.read(buffer, 2))
        {
            Log::error("could not read bits per sample");
            return false;
        }
        bitsPerSample = convert_to_int(buffer, 2);

        // data chunk header "data"
        if (!file.read(buffer, 4))
        {
            Log::error("could not read data chunk header");
            return false;
        }
        if (std::strncmp(buffer, "data", 4) != 0)
        {
            Log::error("file is not a valid WAVE file (doesn't have 'data' tag)");
            return false;
        }

        // size of data
        if (!file.read(buffer, 4))
        {
            Log::error("could not read data size");
            return false;
        }
        size = convert_to_int(buffer, 4);

        /* cannot be at the end of file */
        if (file.eof())
        {
            Log::error("reached EOF on the file");
            return false;
        }
        if (file.fail())
        {
            Log::error("fail state set on the file");
            return false;
        }

        return true;
    }

    char* load_wav(const std::string& filename,
        std::uint8_t& channels,
        std::int32_t& sampleRate,
        std::uint8_t& bitsPerSample,
        ALsizei& size)
    {
        std::ifstream in(filename, std::ios::binary);
        if (!in.is_open())
        {
            Log::error("Could not open \"%s\"", filename.c_str());
            return nullptr;
        }
        if (!load_wav_file_header(in, channels, sampleRate, bitsPerSample, size))
        {
            Log::error("Could not load wav header of \"%s\"", filename.c_str());
            return nullptr;
        }

        char* data = new char[size];

        in.read(data, size);

        return data;
    }

	Audio::Audio() {
		id = 0;
	}

	Audio::~Audio() {
		if (id != 0) {
			alDeleteBuffers(1, &id);
			id = 0;
		}
	}

	bool Audio::load(const std::string& file) {
        if (!alcGetCurrentContext()) {
            Log::warning("audio system not initialized");
            return false;
        }

        uint8_t channels = 0;
        int32_t sampleRate = 0;
        uint8_t bitsPerSample = 0;
        ALsizei size = 0;
        char *data = load_wav(file, channels, sampleRate, bitsPerSample, size);
        ALenum format = 0;
        if (channels == 1) {
            if (bitsPerSample == 8) {
                format = AL_FORMAT_MONO8;
            }else if (bitsPerSample == 16) {
                format = AL_FORMAT_MONO16;
            }
        }else if (channels == 2) {
            if (bitsPerSample == 8) {
                format = AL_FORMAT_STEREO8;
            }
            else if (bitsPerSample == 16) {
                format = AL_FORMAT_STEREO16;
            }
        }

        if (data == nullptr) {
            Log::warning("could not load audio %s", file.c_str());
            return false;
        }

        if (id == 0) {
            alGenBuffers(1, &id);
        }

        if (format == AL_FORMAT_STEREO8) {
            //convert stereo to mono
            char* monoData = new char[size / 2];
            for (int i = 0; i < size; i += 2) {
                char left = *(char*)&data[i];
                char right = *(char*)&data[i + 1];
                monoData[i / 2] = ((int)left + right) / 2;
            }
            alBufferData(id, AL_FORMAT_MONO8, monoData, size / 2, sampleRate);
            delete[] monoData;
        }
        else if (format == AL_FORMAT_STEREO16) {
            //convert stereo to mono
            short* monoData = new short[size / 4];
            for (int i = 0; i < size; i += 4) {
                short left = *(short*)&data[i];
                short right = *(short*)&data[i + 2];
                monoData[i / 4] = ((int)left + right) / 2;
            }
            alBufferData(id, AL_FORMAT_MONO16, monoData, size / 2, sampleRate);
            delete[] monoData;
        }
        else if (format == AL_FORMAT_MONO8) {
            alBufferData(id, format, data, size, sampleRate);
        }
        else if (format == AL_FORMAT_MONO16) {
            alBufferData(id, format, data, size, sampleRate);
        }
        free(data);
        return true;
	}

    uint32_t Audio::getId() {
        return id;
    }

}
