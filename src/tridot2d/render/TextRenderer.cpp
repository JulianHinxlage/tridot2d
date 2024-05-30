//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "TextRenderer.h"
#include "util/strutil.h"
#include "common/Log.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

namespace tridot2d {

	class Atlas {
	public:
		std::shared_ptr<Texture> texture;
		class Glypth {
		public:
			glm::vec2 corrds1 = { 0, 0 };
			glm::vec2 corrds2 = { 1, 1 };
			glm::vec2 size = { 1, 1 };
			glm::vec2 offset = { 0, 0 };
			float stride = 0;
		};
		std::map<char, Glypth> glypths;
		float lineStride = 0;
	};

	std::map<std::string, std::map<int, std::shared_ptr<Atlas>>> atlases;
	
	void TextRenderer::init() {
	
	}

	void TextRenderer::setFont(const std::string& file, int size) {
		atlas = atlases[file][size];
		if (atlas) {
			return;
		}
		
		atlas = std::make_shared<Atlas>();
		atlases[file][size] = atlas;

		std::string content = readFile(file);
		stbtt_fontinfo info;
		if (!stbtt_InitFont(&info, (unsigned char*)content.data(), 0)) {
			Log::error("failed to load file %s", file.c_str());
			return;
		}

		Image image;
		image.init(size * 8, size * 8, 4, 8);
		for (int x = 0; x < image.getWidth(); x++) {
			for (int y = 0; y < image.getHeight(); y++) {
				image.set(x,  y, color::transparent);
			}
		}

		int curserX = 0;
		int curserY = 0;
		int maxHeight = 0;

		float scale = stbtt_ScaleForPixelHeight(&info, size);
		int ascent = 0;
		int descent = 0;
		int lineGap = 0;
		stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
		atlas->lineStride = (float)(lineGap + ascent + descent) * scale / size;

		for (unsigned char c = 0; c < 128; c++) {

			int w = 0;
			int h = 0;
			int xOff = 0;
			int yOff = 0;
			unsigned char *data = stbtt_GetCodepointBitmap(&info, scale, scale, c, &w, &h, &xOff, &yOff);
			int bytesPerPixel = 1;

			int advance = 0;
			int lsb = 0;
			stbtt_GetCodepointHMetrics(&info, c, &advance, &lsb);


			if (curserX + w > image.getWidth()) {
				curserX = 0;
				curserY += maxHeight;
				maxHeight = 0;
			}
			

			for (int x = 0; x < w; x++) {
				for (int y = 0; y < h; y++) {
					unsigned char *pixel = &data[(x + y * w) * bytesPerPixel];
					if (*pixel == 255) {
						image.set(curserX + x, curserY + y, Color(0xffffff));
					}
				}
			}
			stbtt_FreeBitmap(data, nullptr);

			Atlas::Glypth g;
			g.corrds1 = glm::vec2(curserX, curserY) / glm::vec2(image.getWidth(), image.getHeight());
			g.corrds2 = glm::vec2(curserX + w, curserY + h) / glm::vec2(image.getWidth(), image.getHeight());
			g.size = { (float)w / size, (float)h / size };
			g.stride = (float)advance * scale / (float)size;
			g.offset.x = (float)lsb * scale / size;
			g.offset.y = -((float)h + (float)yOff) / size;
			atlas->glypths[c] = g;

			curserX += w + 2;
			maxHeight = std::max(maxHeight, h + 2);
		}



		atlas->texture = std::make_shared<Texture>();
		atlas->texture->load(image);
	}

	glm::vec2 TextRenderer::getTextSize(const std::string& text, glm::vec2 scale) {
		if (!atlas) {
			return { 0, 0 };
		}

		glm::vec2 size = { 0, 0 };
		for (char c : text) {
			auto i = atlas->glypths.find(c);
			if (i != atlas->glypths.end()) {
				Atlas::Glypth& g = i->second;
				size.x += g.stride * scale.x;
			}
		}
		size.y = atlas->lineStride * scale.y;
		return size;
	}

	void TextRenderer::submit(const std::string& text, glm::vec2 position, glm::vec2 scale, float rotation, float depth, Color color) {
		if (!atlas) {
			return;
		}

		for (char c : text) {
			auto i = atlas->glypths.find(c);
			if (i != atlas->glypths.end()) {
				Atlas::Glypth& g = i->second;

				glm::vec2 p = position;
				p += g.size * scale * 0.5f;
				p += g.offset * scale;
				renderer->submitQuad(p, scale * g.size, rotation, depth, atlas->texture.get(), color, g.corrds1, g.corrds2);
				position.x += g.stride * scale.x;
			}
		}
	}

}
