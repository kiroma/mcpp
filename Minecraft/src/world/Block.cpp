#include "Block.h"
#include "BlockFace.h"
#include "../utils/TextureCoords.h"

#include <string.h>

/*
 * TODO (at some point):
 *  Free all blocks allocated upon exiting.
 */

Block::Block *blocks[2048];

// Whoa, how deep does it get? Block::Block::Block::Block::Block::Block::Block::Block::...
Block::Block::Block(unsigned short id, const std::vector<float> faceVertices[],
                    const std::vector<float> textureCoordinates[],
                    const char *displayName)
        : id(id), displayName(displayName)
{
    for (int i = 0; i < 6; i++) {
        this->faceVertices[i] = faceVertices[i];
        if (i < 3)
            this->textureCoordinates[i] = textureCoordinates[i];
    }
}

Block::Block::~Block()
{
}

const std::vector<float> Block::Block::GetFaceVertices(int face) const
{
    return faceVertices[face];
}

const std::vector<float> Block::Block::GetTextureCoordinates(int direction) const
{
    return textureCoordinates[direction];
}

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

void Block::Database::RegisterBlock(int id, std::vector<float> *faceVertices, std::vector<float> *textureCoordinates,
                                    const char *displayName)
{
    std::vector<float> defaultFaceVertices[6] = {
            BlockFace::rightFace,
            BlockFace::leftFace,
            BlockFace::topFace,
            BlockFace::bottomFace,
            BlockFace::frontFace,
            BlockFace::backFace
    };

    std::vector<float> defaultTextureCoordinates[3] = {
            TextureCoords::GenerateTextureCoordinates(id - 1), // skip air
            TextureCoords::GenerateTextureCoordinates(id - 1),
            TextureCoords::GenerateTextureCoordinates(id - 1)
    };

    const std::vector<float> *fv = faceVertices == nullptr ? defaultFaceVertices : faceVertices;
    const std::vector<float> *tc = textureCoordinates == nullptr ? defaultTextureCoordinates : textureCoordinates;

    blocks[id] = new Block(id, fv, tc, displayName);
}

Block::Block &Block::Database::GetBlock(int id)
{
    return *blocks[id];
}
