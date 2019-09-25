#ifndef MINECRAFT_TEXTUREATLAS_H
#define MINECRAFT_TEXTUREATLAS_H

#include <string>
#include <vector>
#include <ext.hpp>
#include <vec2.hpp>
#include "Texture.h"

class TextureAtlas
{
public:
    TextureAtlas(const std::string &filename, int block_texture_size);
    const std::vector<float> GetTexture(const glm::i32vec2 coords) const;
    const glm::i32vec2 CalculatePositionFromIndex(int n) const;
    const Texture &GetInternalTexture() const
    { return texture; };
private:
    Texture texture;
    int atlasSize, blockTextureSize;
};

#endif //MINECRAFT_TEXTUREATLAS_H
