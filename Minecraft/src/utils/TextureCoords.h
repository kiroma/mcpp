#ifndef MINECRAFT_TEXTURECOORDS_H
#define MINECRAFT_TEXTURECOORDS_H

#define MINECRAFT_ATLAS_SIZE 256

#include <glm.hpp>

namespace TextureCoords
{
    glm::vec2 GetTextureCoordinates(int n);
}

#endif //MINECRAFT_TEXTURECOORDS_H
