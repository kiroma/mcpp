#include <iostream>
#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(const std::string &filename, int block_texture_size)
        : texture(filename.c_str()), blockTextureSize(block_texture_size)
{}

const std::vector<float> TextureAtlas::GetTexture(const glm::i32vec2 coords) const
{
    // Thanks to https://github.com/Hopson97/HopsonCraft/blob/master/Source/Texture/TextureAtlas.cpp
    static const int textures_per_axis = texture.GetSize() / blockTextureSize;
    static const float unit_size = 1.0f / (float) textures_per_axis;
    static const float pixel_size = 1.0f / (float) texture.GetSize();

    float base_x = (coords.x * unit_size) + 0.5f * pixel_size;
    float base_y = (coords.y * unit_size) + 0.5f * pixel_size;

    return {
            (base_x + unit_size) - 0.5f * pixel_size, (base_y + unit_size) - 0.5f * pixel_size,
            base_x, (base_y + unit_size) - 0.5f * pixel_size,
            base_x, base_y,
            (base_x + unit_size) - 0.5f * pixel_size, base_y
    };
}

const glm::i32vec2 TextureAtlas::CalculatePositionFromIndex(int n) const
{
    static const int textures_per_axis = texture.GetSize() / blockTextureSize;
    return glm::i32vec2(n % textures_per_axis, (int) glm::floor((double) n / (double) textures_per_axis));
}
