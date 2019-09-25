#include "FullChunk.h"
#include <iostream>

FullChunk::FullChunk(World &world)
        : world(world)
{
    sections.clear();
    for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++) {
        sections.push_back(new Chunk(&world, this, i));
    }
}

FullChunk::~FullChunk()
{
    sections.clear();
    sections.shrink_to_fit();
}
