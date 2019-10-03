#include "World.h"

#include "gen/IChunkGenerator.h"
#include "FullChunk.h"

#include <iostream>

World::World(unsigned int seed)
        : seed(seed), running(false)
{
    noiseGenerator = std::make_unique<SimplexNoise>(0.25f, 1.0f, 2.0f, 0.75f);
}

World::~World()
{
    running = false;
    chunks.clear();
    chunkQueue.clear();
    worldThread->wait();
}

void World::LoadGenerator(std::unique_ptr<IChunkGenerator> gen)
{
    // First of all, terminate the current generator, if one is present
    if (worldThread != nullptr && running) {
        running = false;
        worldThread->wait();
    }

    // With that out of the way, we clear all the current chunks
    for (auto it = chunks.begin(); it != chunks.end(); it++)
        DeleteChunk(it->first);

    for (auto it = chunkQueue.begin(); it != chunkQueue.end(); it++)
        delete *it;

    // Just in case
    chunks.clear();
    chunkQueue.clear();

    // Now load the actual generator
    worldGenerator.swap(gen);

    // We create a spawnpoint chunk so that the player doesn't appear floating in the air
    // and the player spawner has a way of determining the camera height
    chunkQueue.push_front(worldGenerator->GenerateChunk(glm::ivec2(0, 0)));

    // Create the chunk generating thread
    running = true;
    worldThread = new sf::Thread([this] {
        sf::sleep(sf::milliseconds(
                100)); // sleep for some time, to let other stuff run before begining, almost used as a yield
        std::cout << "Started world generator thread" << std::endl;
        while (running) {
            Tick();
            sf::sleep(sf::milliseconds(500));
        }

        std::cout << "Stopped world generator thread" << std::endl;
    });
    worldThread->launch();

    // First main thread tick
    GLTick();
    std::cout << "Loaded world generator \"" << worldGenerator->GetDisplayName() << "\"" << std::endl;
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
                /**
                 * TODO:
                 *  Find out why this crashes here.
                 */

                FullChunk *chunk = worldGenerator->GenerateChunk(pos);
                chunkQueue.push_front(chunk);
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
