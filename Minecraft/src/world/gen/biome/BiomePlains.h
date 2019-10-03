#ifndef MINECRAFT_BIOMEPLAINS_H
#define MINECRAFT_BIOMEPLAINS_H

#include "IBiome.h"

class BiomePlains : public IBiome
{
public:
    int GetHeight(glm::ivec2 pos) override;
    Block::State GetGroundState(glm::ivec2 pos, int depth) override;
    const char *GetDisplayName() override;
};

#endif //MINECRAFT_BIOMEPLAINS_H
