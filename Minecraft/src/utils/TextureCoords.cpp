#include "TextureCoords.h"

#include <iostream>

const glm::vec2 TextureCoords::GetTextureCoordinates(int n)
{
    int tx = n % MINECRAFT_ATLAS_UNITS;
    int ty = n / MINECRAFT_ATLAS_UNITS;
    float u = (float)tx / (float)MINECRAFT_ATLAS_UNIT_SIZE;
    float v = (float)ty / (float)MINECRAFT_ATLAS_UNIT_SIZE;

    //std::cout << u << "," << v << std::endl;
    return glm::vec2(u, v);
}

const std::vector<float> TextureCoords::GenerateTextureCoordinates(int n)
{
    glm::vec2 pos = GetTextureCoordinates(n);
    std::cout << pos.x << ", " << pos.y << std::endl;
    std::vector<float> ret = {
            pos.x, pos.y + MINECRAFT_ATLAS_UNIT_FINAL_SIZE,
            pos.x + MINECRAFT_ATLAS_UNIT_FINAL_SIZE, pos.y + MINECRAFT_ATLAS_UNIT_FINAL_SIZE,
            pos.x + MINECRAFT_ATLAS_UNIT_FINAL_SIZE, pos.y,
            pos.x + MINECRAFT_ATLAS_UNIT_FINAL_SIZE, pos.y,
            pos.x, pos.y,
            pos.x, pos.y + MINECRAFT_ATLAS_UNIT_FINAL_SIZE
    };

    /*
    for (int i = 0; i < 6 * 2; i++) {
        std::cout << ret[i] << ", ";
        if (i % 2 != 0)
            std::cout << std::endl;
    }
     */
    std::cout << "=========" << std::endl;

    return ret;
}
