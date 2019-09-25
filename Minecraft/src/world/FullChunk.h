#ifndef MINECRAFT_FULLCHUNK_H
#define MINECRAFT_FULLCHUNK_H

#include "Chunk.h"
#include "World.h"

// Change for custom height chunks
#define MINECRAFT_CHUNK_SECTIONS 16

class FullChunk
{
public:
    FullChunk(World &world);
    ~FullChunk();

    const std::vector<Chunk *> &GetSections() const
    { return sections; }

private:
    std::vector<Chunk *> sections;
    const World &world;
};

#endif //MINECRAFT_FULLCHUNK_H
