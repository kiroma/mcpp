#include "FullChunk.h"
#include <iostream>

FullChunk::FullChunk(World &world, glm::ivec2 position)
        : world(world), position(position)
{
    sections.clear();
    for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++)
        sections.push_back(new Chunk(&world, this, i));
}

FullChunk::~FullChunk()
{
    for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++)
        delete sections[i];

    sections.clear();
    sections.shrink_to_fit();
}

Block::State FullChunk::GetChunkState(const Block::Position &position) const
{
    return sections[position.y / MINECRAFT_CHUNK_SECTIONS]->GetChunkState(
            Block::Position(position.x, position.y - (position.y / MINECRAFT_CHUNK_SECTIONS * MINECRAFT_CHUNK_SIZE), position.z));
}

void FullChunk::SetChunkState(const Block::Position &position, Block::State state)
{
    sections[position.y / MINECRAFT_CHUNK_SECTIONS]->SetChunkState(
            Block::Position(position.x, position.y - (position.y / MINECRAFT_CHUNK_SECTIONS * MINECRAFT_CHUNK_SIZE), position.z), state);
}

void FullChunk::Update() const
{
    for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++)
        sections[i]->Update();
}

const Chunk &FullChunk::GetSection(int n) const
{ return *sections[n]; }

void FullChunk::FreeMemory() const
{
    for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++)
        sections[i]->FreeMemory();
}
