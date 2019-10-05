#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

class IChunkGenerator;

#include "../entity/Camera.h"
#include "gen/noise/SimplexNoise.h"
#include "../render/RenderMaster.h"
#include "../world/Chunk.h"
#include "../Minecraft.h"
#include "Block.h"

#include <SFML/System.hpp>
#include <unordered_map>
#include <queue>

#define MINECRAFT_RENDER_DISTANCE 8

struct KeyFuncs
{
    size_t operator()(const glm::ivec2 &k) const
    { return std::hash<int>()(k.x) ^ std::hash<int>()(k.y); }

    bool operator()(const glm::ivec2 &a, const glm::ivec2 &b) const
    { return a.x == b.x && a.y == b.y; }
};

class World
{
public:
    World(unsigned int seed);
    ~World();

    void Tick();
    void GLTick();

    void LoadGenerator(std::unique_ptr<IChunkGenerator> gen);
    void DeleteChunk(glm::ivec2 position);

    Block::State GetWorldState(Block::Position position) const;
    void SetWorldState(Block::Position position, Block::State state);
    int GetHighestPoint(glm::ivec2 position) const;

    std::unordered_map<glm::ivec2, FullChunk *, KeyFuncs, KeyFuncs>::const_iterator RenderBegin() const
    { return chunks.begin(); }

    std::unordered_map<glm::ivec2, FullChunk *, KeyFuncs, KeyFuncs>::const_iterator RenderEnd() const
    { return chunks.end(); }

    glm::ivec2 GetPlayerChunkPosition() const
    {
        return glm::ivec2(Minecraft::GetInstance().GetMasterRenderer().GetViewCamera().GetPosition().x /
                          MINECRAFT_CHUNK_SIZE,
                          Minecraft::GetInstance().GetMasterRenderer().GetViewCamera().GetPosition().z /
                          MINECRAFT_CHUNK_SIZE);
    }

    const SimplexNoise &GetNoiseGenerator()
    { return *noiseGenerator; }

private:
    std::unordered_map<glm::ivec2, FullChunk *, KeyFuncs, KeyFuncs> chunks;
    std::deque<FullChunk *> chunkQueue;
    sf::Thread *worldThread = nullptr;
    std::unique_ptr<SimplexNoise> noiseGenerator;
    std::unique_ptr<IChunkGenerator> worldGenerator;
    unsigned int seed;
    bool running;
};

#endif //MINECRAFT_WORLD_H
