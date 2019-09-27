#include "World.h"

#include <iostream>

World::World()
{
    // Generate spawn chunk
    chunks.clear();
    GenerateChunk(glm::ivec2(0, 0));
}

World::~World()
{
    chunks.clear();
}

void World::Tick()
{
    // Generate chunks around the player
    for (int x = -MINECRAFT_RENDER_DISTANCE / 2; x < MINECRAFT_RENDER_DISTANCE / 2; x++) {
        for (int z = -MINECRAFT_RENDER_DISTANCE / 2; z < MINECRAFT_RENDER_DISTANCE / 2; z++) {
            glm::ivec2 pos(x + GetPlayerChunkPosition().x,
                           z + GetPlayerChunkPosition().y);

            if (chunks.find(pos) == chunks.end()) {
                GenerateChunk(pos);
                break;
            }
        }
    }

    // Update all chunks
    for (auto it = chunks.begin(); it != chunks.end(); it++) {
        // Check if it is too far away from the player
        glm::ivec2 pos = it->second->GetPosition();
        //std::cout << "pos.y: " << (float)pos.y << " condition: " << (float)(GetPlayerChunkPosition().y + MINECRAFT_RENDER_DISTANCE) << std::endl;
        if (pos.x < GetPlayerChunkPosition().x - MINECRAFT_RENDER_DISTANCE / 2 ||
            pos.y < GetPlayerChunkPosition().y - MINECRAFT_RENDER_DISTANCE / 2 ||
            pos.x > GetPlayerChunkPosition().x + MINECRAFT_RENDER_DISTANCE / 2 ||
            pos.y > GetPlayerChunkPosition().y + MINECRAFT_RENDER_DISTANCE / 2) {
            // If that's the case, delete it
            DeleteChunk(pos);
            break;
        } else {
            // Else, we can update it to make sure the mesh is up-to-date with the blocks
            it->second->Update();
        }
    }
}

FullChunk *World::CreateChunk(glm::ivec2 position)
{
    if (chunks.find(position) != chunks.end())
        return nullptr;

    FullChunk *chunk = new FullChunk(*this, position);
    chunks[position] = chunk;
    return chunk;
}

FullChunk *World::GenerateChunk(glm::ivec2 position)
{
    // Create the actual chunk
    FullChunk *chunk = CreateChunk(position);
    if (chunk == nullptr)
        return nullptr;

    // Generate terrain
    for (int x = 0; x < MINECRAFT_CHUNK_SIZE; x++) {
        for (int z = 0; z < MINECRAFT_CHUNK_SIZE; z++) {
            int height = rand() % 10 + 60;
            for (int i = 0; i < height; i++)
                chunk->SetChunkState(Block::Position(x, i, z), Block::State(1));
        }
    }

    return chunk;
}

void World::DeleteChunk(glm::ivec2 position)
{
    if (chunks.find(position) == chunks.end())
        return;

    chunks.erase(position);
}

Block::State World::GetWorldState(Block::Position position) const
{
    glm::ivec2 chunkpos(position.x / MINECRAFT_CHUNK_SIZE, position.z / MINECRAFT_CHUNK_SIZE);
    return chunks.at(chunkpos)->GetChunkState(
            Block::Position(position.x / MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SIZE, position.y,
                            position.z / MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SIZE));
}

void World::SetWorldState(Block::Position position, Block::State state)
{
    glm::ivec2 chunkpos(position.x / MINECRAFT_CHUNK_SIZE, position.z / MINECRAFT_CHUNK_SIZE);
    chunks.at(chunkpos)->SetChunkState(
            Block::Position(position.x / MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SIZE, position.y,
                            position.z / MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SIZE), state);
}

int World::GetHighestPoint(glm::ivec2 position) const
{
    for (int y = MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SECTIONS - 1; y > 0; y--) {
        if (GetWorldState(Block::Position(position.x, y, position.y)).id != 0)
            return y;
    }
    return 0;
}
