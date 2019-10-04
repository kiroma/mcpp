#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <vector>
#include <string>
#include <array>
#include <memory>
#include <glm.hpp>

namespace Block
{
    struct Position : glm::ivec3
    {
        Position(int x, int y, int z) : glm::ivec3(x, y, z)
        {}
    };

    struct State
    {
        State() : State(0)
        {}

        State(unsigned short id)
        { this->id = id; }

        unsigned short id;
    };

    class Block
    {
    public:
        Block(unsigned short id, const std::vector<float> faceVertices[], const std::vector<float> textureCoordinates[],
              const char *displayName);
        ~Block();

        const std::vector<float> GetFaceVertices(int face) const;
        const std::vector<float> GetTextureCoordinates(int direction) const;
    private:
        unsigned short id;
        std::array<std::vector<float>, 6> faceVertices;
        std::array<std::vector<float>, 3> textureCoordinates;
        const char *displayName;
    };

    class Database
    {
    public:
        Database();
        void RegisterBlock(int id, const std::vector<float> *faceVertices,
                           const std::vector<float> *textureCoordinates, const char *displayName);
        void RegisterBlock(int id, const std::vector<float> *faceVertices,
                           int atlas_id, const char *displayName);
        const Block &GetBlock(int id);
    private:
        // The actual allocated blocks
        std::array<std::unique_ptr<Block>, 2048> blocks;
    };
}

#endif //MINECRAFT_BLOCK_H
