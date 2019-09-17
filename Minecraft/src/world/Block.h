#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <glm.hpp>
#include <vector>
#include <string>

namespace Block
{
    struct Position : glm::i64vec3
    {
        Position(int x, int y, int z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
    };

    struct State
    {
        State() : State(0)
        {
        }

        State(unsigned short id)
        {
            this->id = id;
        }

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
        std::vector<float> faceVertices[6];
        std::vector<float> textureCoordinates[3];
        const char *displayName;
    };

    namespace Database
    {
        void Initialize();
        void RegisterBlock(int id, std::vector<float> *faceVertices,
                           std::vector<float> *textureCoordinates, const char *displayName);
        Block &GetBlock(int id);
    }
}

#endif //MINECRAFT_BLOCK_H
