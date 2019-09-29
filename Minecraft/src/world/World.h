#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

#include "FullChunk.h"
#include "ChunkUpdate.h"
#include "../Minecraft.h"
#include "../render/RenderMaster.h"
#include "../utils/math/SimplexNoise.h"

#include <unordered_map>
#include <queue>
#include <SFML/System.hpp>

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
    void Second();

    FullChunk *GenerateChunk(glm::ivec2 position);
    void DeleteChunk(glm::ivec2 position);

    Block::State GetWorldState(Block::Position position) const;
    void SetWorldState(Block::Position position, Block::State state);
    int GetHighestPoint(glm::ivec2 position) const;

    void RegisterChunkUpdate(UpdateType type);

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

    int GetChunkUpdateState(UpdateType type) {
        switch (type) {
            case CREATE:
                return updateC;
            case DESTROY:
                return updateD;
            case LOAD:
                return updateL;
            case REMESH:
                return updateR;
            case ALL:
                return updateC + updateD + updateL + updateR;
            default:
                break;
        }

        return 0;
    }

private:
    std::unordered_map<glm::ivec2, FullChunk *, KeyFuncs, KeyFuncs> chunks;
    std::deque<FullChunk*> chunkQueue;
    sf::Thread *worldThread;
    bool running;
    SimplexNoise noiseGenerator;
    unsigned int seed;
    int tupdateC, tupdateD, tupdateL, tupdateR;
    int updateC, updateD, updateL, updateR;
    float time = 0;
};

#endif //MINECRAFT_WORLD_H
