#include <iostream>
#include "World.h"

World::World()
{
    chunks.clear();
    for (int x = 0; x < 8; x++)
        for (int z = 0; z < 8; z++)
            CreateChunk(glm::ivec2(x, z));

    std::cout << "Created " << chunks.size() << " chunks." << std::endl;
}

World::~World()
{
    for (auto it = chunks.begin(); it != chunks.end(); it++)
        DeleteChunk(it->first);
}

FullChunk &World::CreateChunk(glm::ivec2 position)
{
    if (chunks.find(position) != chunks.end())
        return *((FullChunk *) (nullptr));

    FullChunk *chunk = new FullChunk(*this, position);
    chunks[position] = chunk;
    return *chunk;
}

void World::DeleteChunk(glm::ivec2 position)
{
    if (chunks.find(position) != chunks.end())
        return;

    chunks.erase(position);
}
