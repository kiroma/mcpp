#ifndef MINECRAFT_BIOMEEXTREMEHILLS_H
#define MINECRAFT_BIOMEEXTREMEHILLS_H

#include "IBiome.h"

class BiomeExtremeHills : public IBiome
{
    int GetHeight(glm::ivec2 pos) override;
    Block::State GetGroundState(glm::ivec2 pos, int depth) override;
    const char *GetDisplayName() override;
};

#endif //MINECRAFT_BIOMEEXTREMEHILLS_H
