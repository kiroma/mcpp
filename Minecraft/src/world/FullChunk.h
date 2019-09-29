#ifndef MINECRAFT_FULLCHUNK_H
#define MINECRAFT_FULLCHUNK_H

#include "Chunk.h"
#include "World.h"

// Change for custom height chunks
#define MINECRAFT_CHUNK_SECTIONS 16

class FullChunk
{
public:
    FullChunk(World &world, glm::ivec2 position);
    ~FullChunk();
    void Load() const;

    Block::State GetChunkState(const Block::Position &position) const;
    void SetChunkState(const Block::Position &position, Block::State state);
    void Update() const;
    const Chunk &GetSection(int n) const;
    void FreeMemory() const;

    const std::vector<Chunk *> &GetSections() const
    { return sections; }

    const glm::ivec2 GetPosition() const
    { return position; }

private:
    std::vector<Chunk *> sections;
    const World &world;
    const glm::ivec2 position;
};

#endif //MINECRAFT_FULLCHUNK_H
