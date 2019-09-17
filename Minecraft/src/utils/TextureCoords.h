#ifndef MINECRAFT_TEXTURECOORDS_H
#define MINECRAFT_TEXTURECOORDS_H

#define MINECRAFT_ATLAS_SIZE (256)
#define MINECRAFT_ATLAS_UNITS (16)
#define MINECRAFT_ATLAS_UNIT_SIZE (MINECRAFT_ATLAS_SIZE / MINECRAFT_ATLAS_UNITS)
#define MINECRAFT_ATLAS_UNIT_FINAL_SIZE ((float)MINECRAFT_ATLAS_UNIT_SIZE / (float)MINECRAFT_ATLAS_SIZE)

#include <glm.hpp>
#include <vector>

namespace TextureCoords
{
    const glm::vec2 GetTextureCoordinates(int n);
    const std::vector<float> GenerateTextureCoordinates(int n);
}

#endif //MINECRAFT_TEXTURECOORDS_H
