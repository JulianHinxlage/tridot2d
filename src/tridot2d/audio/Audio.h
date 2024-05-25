//
// Copyright (c) 2022 Julian Hinxlage. All rights reserved.
//

#pragma once

#include <string>
#include <cstdint>

namespace tridot2d {

	class Audio {
	public:
		Audio();
		~Audio();
		virtual bool load(const std::string& file);
		uint32_t getId();
	
	private:
		uint32_t id;
	};

}
