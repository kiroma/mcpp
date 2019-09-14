#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <glm.hpp>

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
}

#endif //MINECRAFT_BLOCK_H
