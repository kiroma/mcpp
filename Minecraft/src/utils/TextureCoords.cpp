#include "TextureCoords.h"

#include <iostream>

const glm::vec2 TextureCoords::GetTextureCoordinates(int n)
{
    float tx = (float)(n % MINECRAFT_ATLAS_UNITS);
    float ty = (float)(n / MINECRAFT_ATLAS_UNITS);

    float x = tx != 0 ? (tx * MINECRAFT_ATLAS_UNIT_SIZE) / MINECRAFT_ATLAS_SIZE : 0;
    float y = ty != 0 ? (ty * MINECRAFT_ATLAS_UNIT_SIZE) / MINECRAFT_ATLAS_SIZE : 0;

    return glm::vec2(x, y);
}

/*
 * TODO:
 *  FIND OUT WHY THESE TEXTURE COORDINATES WON'T WORK
 *  FOR ANYTHING ELSE THEN ID 0!
 */
const std::vector<float> TextureCoords::GenerateTextureCoordinates(int n)
{
    glm::vec2 pos = GetTextureCoordinates(n);
    std::vector<float> ret = {
            pos.x, pos.y + MINECRAFT_ATLAS_UNIT_FINAL_SIZE,
            pos.x + MINECRAFT_ATLAS_UNIT_FINAL_SIZE, pos.y + MINECRAFT_ATLAS_UNIT_FINAL_SIZE,
            pos.x + MINECRAFT_ATLAS_UNIT_FINAL_SIZE, 0,
            pos.x + MINECRAFT_ATLAS_UNIT_FINAL_SIZE, 0,
            pos.x, pos.y,
            pos.x, pos.y + MINECRAFT_ATLAS_UNIT_FINAL_SIZE
    };

    /*
    for (int i = 0; i < 6 * 2; i++) {
        std::cout << ret[i] << ", ";
        if (i % 2 != 0)
            std::cout << std::endl;
    }
     std::cout << "=========" << std::endl;
     */

    return ret;
}
