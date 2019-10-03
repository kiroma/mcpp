#ifndef MINECRAFT_OVERWORLDGENERATOR_H
#define MINECRAFT_OVERWORLDGENERATOR_H

#include "IChunkGenerator.h"

#include <memory>

class IBiome;

class OverworldGenerator : public IChunkGenerator
{
public:
    OverworldGenerator();

    FullChunk *GenerateChunk(glm::ivec2 position) override;
    const char *GetDisplayName(void) const override;

    std::shared_ptr<IBiome> GetBiomeAtPos(glm::ivec2 pos);

private:
    std::vector<std::shared_ptr<IBiome>> biomes;
};

#endif //MINECRAFT_OVERWORLDGENERATOR_H
