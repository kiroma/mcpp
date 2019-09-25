#include "World.h"

World::World()
{
    chunk = new FullChunk(*this);
}

World::~World()
{
    delete chunk;
}
