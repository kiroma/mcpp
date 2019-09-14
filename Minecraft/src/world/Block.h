#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <glm.hpp>
#include <vector>
#include <string>

namespace Block {
    struct Position : glm::i64vec3 {
        Position(int x, int y, int z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }
    };

    struct State {
        State() : State(0) {
        }

        State(short id) {
            this->id = id;
        }

        short id;
    };

    struct Block {
        short id;
        const std::vector<float> *faceVertices;
        const std::vector<float> *textureCoordinates;
        const char *displayName;
    };

    namespace Database {
        void Initialize();
        void RegisterBlock(int id, const std::vector<float> *faceVertices,
                           const std::vector<float> *textureCoordinates, const char *displayName);
        Block& GetBlock(int id);
    }
}

#endif //MINECRAFT_BLOCK_H
