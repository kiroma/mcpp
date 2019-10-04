#ifndef MINECRAFT_TEXTUREATLAS_H
#define MINECRAFT_TEXTUREATLAS_H

#include "Texture.h"

#include <string>
#include <vector>
#include <glm.hpp>

class TextureAtlas
{
public:
    TextureAtlas(const std::string &filename, int block_texture_size);
    ~TextureAtlas()
    {}

    const std::vector<float> GetTexture(const glm::ivec2 coords) const;
    const glm::ivec2 CalculatePositionFromIndex(int n) const;
    const Texture &GetInternalTexture() const
    { return texture; };
private:
    Texture texture;
    int atlasSize, blockTextureSize;
};

#endif //MINECRAFT_TEXTUREATLAS_H
