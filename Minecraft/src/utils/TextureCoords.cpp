#include "TextureCoords.h"

glm::vec2 GetTextureCoordinates(int n)
{
    int unitspersize = MINECRAFT_ATLAS_SIZE / 16;
    int col = n % unitspersize;
    int row = (int) glm::trunc((float) n / (float) unitspersize);
    return glm::vec2((float) col / unitspersize, (float) row / unitspersize);
}
