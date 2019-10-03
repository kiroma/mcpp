#include "BiomeExtremeHills.h"

#include "../../World.h"

int BiomeExtremeHills::GetHeight(glm::ivec2 pos)
{
    return (GetWorld().GetNoiseGenerator().fractal(6, (float) pos.x / 128.0f, (float) pos.y / 128.0f) * 52.0f) +
           144.0f;
}

Block::State BiomeExtremeHills::GetGroundState(glm::ivec2 pos, int depth)
{
    (void) pos;
    (void) depth;

    // 75% grass 25% gravel
    return GetWorld().GetNoiseGenerator().noise(pos.x, pos.y) < 0.5f ? Block::State(2) : Block::State(
            13);
}

const char *BiomeExtremeHills::GetDisplayName()
{ return "extreme_hills"; }
