#include "BiomePlains.h"

#include "../../World.h"

int BiomePlains::GetHeight(glm::ivec2 pos)
{ return (GetWorld().GetNoiseGenerator().fractal(6, (float) pos.x / 128.0f, (float) pos.y / 128.0f) * 16.0f) + 80.0f; }

Block::State BiomePlains::GetGroundState(glm::ivec2 pos, int depth)
{
    (void) pos;
    (void) depth;
    return Block::State(2);
}

const char *BiomePlains::GetDisplayName()
{ return "plains"; }
