#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

class Chunk;

class World
{
public:
    World();
    ~World();

    Chunk *chunk;
};

#endif //MINECRAFT_WORLD_H
