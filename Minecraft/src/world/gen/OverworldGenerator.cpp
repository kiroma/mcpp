#include "OverworldGenerator.h"

#include "../../utils/math/MathHelper.h"
#include "../FullChunk.h"
#include "../Chunk.h"
#include "../World.h"
#include "biome/IBiome.h"
#include "biome/BiomePlains.h"
#include "biome/BiomeExtremeHills.h"

#include <iostream>

#define MINECRAFT_SMOOTH_RANGE 3

OverworldGenerator::OverworldGenerator()
{
    // Initialize all biomes
    biomes.clear();
    biomes.insert(biomes.end(), std::make_shared<BiomePlains>());
    biomes.insert(biomes.end(), std::make_shared<BiomeExtremeHills>());
}

FullChunk *OverworldGenerator::GenerateChunk(glm::ivec2 position)
{
    // Create the actual chunk
    FullChunk *chunk = new FullChunk(GetWorld(), position);

    // Generate terrain
    for (int x = 0; x < MINECRAFT_CHUNK_SIZE; x++) {
        for (int z = 0; z < MINECRAFT_CHUNK_SIZE; z++) {
            // Absolute coorinates
            glm::ivec2 apos(x + chunk->GetPosition().x * 16, z + chunk->GetPosition().y * 16);

            // Get the biomes we'll be working with
            std::shared_ptr<IBiome> biome = GetBiomeAtPos(apos);

            int height = (int) biome->GetHeight(apos);
            for (int i = 0; i < height; i++) {
                Block::State state(1);
                if (i >= height - 1) {
                    state = biome->GetGroundState(apos, 0);
                } else if (i >= height - 5) {
                    state.id = 3;
                }

                chunk->SetChunkState(Block::Position(x, i, z), state);
            }
        }
    }

    return chunk;
}

const char *OverworldGenerator::GetDisplayName(void) const
{ return "overworld_gen"; }

std::shared_ptr<IBiome> OverworldGenerator::GetBiomeAtPos(glm::ivec2 pos)
{
    float h = (GetWorld().GetNoiseGenerator().fractal(6, (float) pos.x / 256.0f, (float) pos.y / 256.0f) + 1) * 0.5f;
    int index = (int) glm::floor(h * (float) biomes.size());
    return biomes[index];
}
