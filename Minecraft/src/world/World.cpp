#include "World.h"

#include <iostream>

World::World(unsigned int seed)
        : seed(seed), noiseGenerator(0.25f, 1.0f, 2.0f, 0.75f)
{
    // Generate spawn chunk
    chunks.clear();
    GenerateChunk(glm::ivec2(0, 0));
    GLTick();

    // Create the chunk generating thread
    running = true;
    worldThread = new sf::Thread([this] {
        std::cout << "Started world generator" << std::endl;
        while (running) {
            Tick();
            sf::sleep(sf::milliseconds(2000));
        }

        std::cout << "Stopped world generator" << std::endl;
    });
    worldThread->launch();
}

World::~World()
{
    chunks.clear();
    running = false;
    worldThread->wait();
}

void World::Tick()
{
    // If no camera is present, we can't continue with this part
    if (!Minecraft::GetInstance().GetMasterRenderer().IsCameraAvailable())
        return;

    // Generate chunks around the player
    for (int x = -MINECRAFT_RENDER_DISTANCE / 2; x < MINECRAFT_RENDER_DISTANCE / 2; x++) {
        for (int z = -MINECRAFT_RENDER_DISTANCE / 2; z < MINECRAFT_RENDER_DISTANCE / 2; z++) {
            glm::ivec2 pos(x + GetPlayerChunkPosition().x,
                           z + GetPlayerChunkPosition().y);

            if (chunks.find(pos) == chunks.end() &&
                std::find_if(chunkQueue.begin(), chunkQueue.end(),
                             [&](FullChunk *c) { return c->GetPosition() == pos; }) == chunkQueue.end()) {
                GenerateChunk(pos);
                break;
            }
        }
    }
}

void World::GLTick()
{
    // Load all chunks in the queue
    if (!chunkQueue.empty()) {
        FullChunk *chunk = chunkQueue.front();
        chunkQueue.pop_front();
        chunk->Load();
        chunks[chunk->GetPosition()] = chunk;
    }

    // If no camera is present, we can't continue with this part
    if (!Minecraft::GetInstance().GetMasterRenderer().IsCameraAvailable())
        return;

    // Update all the chunks
    for (auto it = chunks.begin(); it != chunks.end(); it++) {
        // Check if it is too far away from the player
        glm::ivec2 pos = it->second->GetPosition();

        if (pos.x < GetPlayerChunkPosition().x - MINECRAFT_RENDER_DISTANCE / 2 ||
            pos.y < GetPlayerChunkPosition().y - MINECRAFT_RENDER_DISTANCE / 2 ||
            pos.x > GetPlayerChunkPosition().x + MINECRAFT_RENDER_DISTANCE / 2 ||
            pos.y > GetPlayerChunkPosition().y + MINECRAFT_RENDER_DISTANCE / 2) {
            // If that's the case, delete it
            DeleteChunk(pos);
            break;
        } else {
            it->second->Update();
        }
    }
}

FullChunk *World::GenerateChunk(glm::ivec2 position)
{
    // Create the actual chunk
    FullChunk *chunk = new FullChunk(*this, position);

    // Generate terrain
    for (int x = 0; x < MINECRAFT_CHUNK_SIZE; x++) {
        for (int z = 0; z < MINECRAFT_CHUNK_SIZE; z++) {
            int rx = x + chunk->GetPosition().x * 16;
            int rz = z + chunk->GetPosition().y * 16; // y = z

            float height = noiseGenerator.fractal(6, (float) rx / 64.0f, (float) rz / 64.0f) * 15.0f + 70.0f;

            for (int i = 0; i < (int) height; i++) {
                Block::State state(1);
                if (i >= height - 2) {
                    state.id = 2;
                } else if (i >= height - 5) {
                    state.id = 3;
                }

                chunk->SetChunkState(Block::Position(x, i, z), state);
            }
        }
    }

    chunkQueue.push_front(chunk);
    return chunk;
}

void World::DeleteChunk(glm::ivec2 position)
{
    if (chunks.find(position) == chunks.end())
        return;

    delete chunks[position];
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
