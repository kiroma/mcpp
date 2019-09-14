#ifndef MINECRAFT_BLOCKFACE_H
#define MINECRAFT_BLOCKFACE_H

#include <vector>
#include <vec3.hpp>

#define BLOCK_SIZE 1

namespace BlockFace {
    typedef enum {
        FRONT,
        BACK,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    } BlockFace;

    const std::vector<float> frontFace
            {
                    0, 0, BLOCK_SIZE,
                    BLOCK_SIZE, 0, BLOCK_SIZE,
                    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
                    0, BLOCK_SIZE, BLOCK_SIZE
            };

    const std::vector<float> backFace
            {
                    BLOCK_SIZE, 0, 0,
                    0, 0, 0,
                    0, BLOCK_SIZE, 0,
                    BLOCK_SIZE, BLOCK_SIZE, 0
            };

    const std::vector<float> leftFace
            {
                    0, 0, 0,
                    0, 0, BLOCK_SIZE,
                    0, BLOCK_SIZE, BLOCK_SIZE,
                    0, BLOCK_SIZE, 0
            };

    const std::vector<float> rightFace
            {
                    BLOCK_SIZE, 0, BLOCK_SIZE,
                    BLOCK_SIZE, 0, 0,
                    BLOCK_SIZE, BLOCK_SIZE, 0,
                    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE
            };

    const std::vector<float> topFace
            {
                    0, BLOCK_SIZE, BLOCK_SIZE,
                    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
                    BLOCK_SIZE, BLOCK_SIZE, 0,
                    0, BLOCK_SIZE, 0
            };

    const std::vector<float> bottomFace
            {
                    0, 0, 0,
                    BLOCK_SIZE, 0, 0,
                    BLOCK_SIZE, 0, BLOCK_SIZE,
                    0, 0, BLOCK_SIZE
            };
}

#endif //MINECRAFT_BLOCKFACE_H
