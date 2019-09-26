#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

#include "FullChunk.h"

#include <unordered_map>

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

    FullChunk &CreateChunk(glm::ivec2 position);
    void DeleteChunk(glm::ivec2 position);

    std::unordered_map<glm::ivec2, FullChunk *, KeyFuncs, KeyFuncs>::const_iterator RenderBegin() const
    { return chunks.begin(); }

    std::unordered_map<glm::ivec2, FullChunk *, KeyFuncs, KeyFuncs>::const_iterator RenderEnd() const
    { return chunks.end(); }

private:
    std::unordered_map<glm::ivec2, FullChunk *, KeyFuncs, KeyFuncs> chunks;
};

#endif //MINECRAFT_WORLD_H
