#ifndef MINECRAFT_WORLDGENERATOR_H
#define MINECRAFT_WORLDGENERATOR_H

#include "../FullChunk.h"
#include "../../Minecraft.h"

class IChunkGenerator
{
public:
    virtual FullChunk *GenerateChunk(glm::ivec2 position) = 0;
    virtual const char *GetDisplayName(void) const = 0;

protected:
    // Ugly hack to get a non-const world (because I'm too lazy to change it's constness in Minecraft.h)
    World &GetWorld()
    { return (World &) Minecraft::GetInstance().GetCurrentWorld(); }
};

#endif //MINECRAFT_WORLDGENERATOR_H
