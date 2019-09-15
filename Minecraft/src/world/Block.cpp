#include "Block.h"
#include "BlockFace.h"

#include <string.h>

Block::Block *blocks[2048];

const std::vector<float> defaultFaceVertices[6] = {
        BlockFace::rightFace,
        BlockFace::leftFace,
        BlockFace::topFace,
        BlockFace::bottomFace,
        BlockFace::frontFace,
        BlockFace::backFace
};

const std::vector<float> defaultFaceTextureCoords[3] = {
        {
                0, 0,
                0.0625f, 0,
                0.0625f, 0.0625f,
                0.0625f, 0.0625f,
                0, 0.0625f,
                0, 0
        },
        {
                0, 0,
                0.0625f, 0,
                0.0625f, 0.0625f,
                0.0625f, 0.0625f,
                0, 0.0625f,
                0, 0
        },
        {
                0, 0,
                0.0625f, 0,
                0.0625f, 0.0625f,
                0.0625f, 0.0625f,
                0, 0.0625f,
                0, 0
        }
};

void Block::Database::Initialize()
{
    // Clear some space for blocks
    memset(blocks, 0, sizeof(blocks));

    // Register blocks
    RegisterBlock(0, nullptr, nullptr, "Air");
    RegisterBlock(1, nullptr, nullptr, "Stone");
    RegisterBlock(2, nullptr, nullptr, "Dirt");
    RegisterBlock(3, nullptr, nullptr, "Gravel");
    RegisterBlock(4, nullptr, nullptr, "Oak Planks");
    RegisterBlock(5, nullptr, nullptr, "Stone Bricks");
    RegisterBlock(6, nullptr, nullptr, "TNT");
    RegisterBlock(7, nullptr, nullptr, "Yellow Hardened Clay");
    RegisterBlock(8, nullptr, nullptr, "Diamond Block");
    RegisterBlock(9, nullptr, nullptr, "Coal Ore");
    RegisterBlock(10, nullptr, nullptr, "Diamond Ore");
    RegisterBlock(11, nullptr, nullptr, "Emerald Ore");
    RegisterBlock(12, nullptr, nullptr, "Gold Ore");
    RegisterBlock(13, nullptr, nullptr, "Iron Ore");
    RegisterBlock(14, nullptr, nullptr, "Lapis Ore");
    RegisterBlock(15, nullptr, nullptr, "Redstone Ore");
}

void Block::Database::RegisterBlock(int id, const std::vector<float> *faceVertices,
                                    const std::vector<float> *textureCoordinates, const char *displayName)
{
    Block *block = (Block *) malloc(sizeof(Block));
    block->id = id;
    block->displayName = displayName;
    if (id == 0) goto out;

    if (faceVertices != nullptr) block->faceVertices = faceVertices; else block->faceVertices = defaultFaceVertices;
    if (textureCoordinates != nullptr)
        block->textureCoordinates = textureCoordinates;
    else block->textureCoordinates = defaultFaceTextureCoords;

    out:
    blocks[id] = block;
}

Block::Block &Block::Database::GetBlock(int id)
{
    return *blocks[id];
}
