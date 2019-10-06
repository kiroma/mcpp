#ifndef MINECRAFT_IBIOME_H
#define MINECRAFT_IBIOME_H

#include "../../Block.h"
#include "../../../Minecraft.h"

#include <glm/ext.hpp>

class IBiome
{
public:
    virtual int GetHeight(glm::ivec2 pos) = 0;
    virtual Block::State GetGroundState(glm::ivec2 pos, int depth) = 0;
    virtual const char *GetDisplayName() = 0;

protected:
    // Ugly hack to get a non-const world (because I'm too lazy to change it's constness in Minecraft.h)
    World &GetWorld()
    { return (World &) Minecraft::GetInstance().GetCurrentWorld(); }
};

#endif //MINECRAFT_IBIOME_H
