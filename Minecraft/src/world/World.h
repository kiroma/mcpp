#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

#include "FullChunk.h"
#include "../Minecraft.h"
#include "../render/RenderMaster.h"

#include <unordered_map>

#define MINECRAFT_RENDER_DISTANCE 6

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
    World();
    ~World();

    void Tick();

    FullChunk *CreateChunk(glm::ivec2 position);
    FullChunk *GenerateChunk(glm::ivec2 position);
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

private:
    std::unordered_map<glm::ivec2, FullChunk *, KeyFuncs, KeyFuncs> chunks;
};

#endif //MINECRAFT_WORLD_H
