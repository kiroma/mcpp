#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

#include "FullChunk.h"

class Chunk;

class World
{
public:
    World();
    ~World();

    FullChunk *chunk;
};

#endif //MINECRAFT_WORLD_H
