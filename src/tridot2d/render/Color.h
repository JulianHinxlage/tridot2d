//
// Copyright (c) 2021 Julian Hinxlage. All rights reserved.
//

#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace tridot2d {

    class Color {
    public:
        union{
            uint32_t value;
            struct{
                uint8_t r;
                uint8_t g;
                uint8_t b;
                uint8_t a;
            };
        };

        Color(uint32_t value = 0xffffff, uint8_t alpha = 0xff);
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        Color(const glm::vec4 &color);
        Color(const glm::vec3 &color);
        glm::vec4 vec() const;
        bool operator==(const Color &color) const;
        bool operator!=(const Color &color) const;
        Color operator*(float rhs) const;
        Color operator/(float rhs) const;
        Color operator*(Color rhs) const;
        Color operator+(Color rhs) const;
        Color operator-(Color rhs) const;
    };

    namespace color {
        static const Color white = Color(0xffffff);
        static const Color black = Color(0x000000);
        static const Color red = Color(0xff0000);
        static const Color green = Color(0x00ff00);
        static const Color blue = Color(0x0000ff);
        static const Color transparent = Color(0xffffff, 0x00);
    }

}

