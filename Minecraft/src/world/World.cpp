#include "World.h"
#include "Chunk.h"

World::World()
{
    chunk = new Chunk(*this);
}

World::~World()
{
    delete chunk;
}
