#include "Block.h"
#include "BlockFace.h"
#include "../render/TextureAtlas.h"
#include "../render/RenderMaster.h"
#include "../Minecraft.h"

#include <iostream>
#include <cstring>

/*
 * TODO (at some point):
 *  Free all blocks allocated upon exiting.
 */

// The actual allocated blocks
Block::Block *blocks[2048];

// Whoa, how deep does it get? Block::Block::Block::Block::Block::Block::Block::Block::...
Block::Block::Block(unsigned short id, const std::vector<float> faceVertices[],
                    const std::vector<float> textureCoordinates[],
                    const char *displayName)
        : id(id), displayName(displayName)
{
    for (int i = 0; i < 6; i++) {
        this->faceVertices[i] = faceVertices[i];
        if (i < 3 && textureCoordinates != nullptr)
            this->textureCoordinates[i] = textureCoordinates[i];
    }
}

Block::Block::~Block()
{}

const std::vector<float> Block::Block::GetFaceVertices(int face) const
{ return faceVertices[face]; }

const std::vector<float> Block::Block::GetTextureCoordinates(int direction) const
{ return textureCoordinates[direction]; }

void Block::Database::Initialize()
{
    // Clear some space for blocks
    memset(blocks, 0, sizeof(blocks));

    // Register blocks
    RegisterBlock(0, nullptr, -1, "Air");
    RegisterBlock(1, nullptr, 0, "Stone");
    RegisterBlock(2, nullptr, 1, "Grass");
    RegisterBlock(3, nullptr, 2, "Dirt");
    RegisterBlock(4, nullptr, 3, "Cobblestone");
    RegisterBlock(5, nullptr, 4, "Oak Planks");
    RegisterBlock(6, nullptr, 5, "Oak Sapling");
    RegisterBlock(7, nullptr, 6, "Bedrock");
    RegisterBlock(8, nullptr, 7, "Flowing Water");
    RegisterBlock(9, nullptr, 8, "Water");
    RegisterBlock(11, nullptr, 9, "Flowing Lava");
    RegisterBlock(12, nullptr, 10, "Lava");
    RegisterBlock(13, nullptr, 11, "Sand");
    RegisterBlock(14, nullptr, 12, "Gravel");
    RegisterBlock(15, nullptr, 13, "Gold Ore");
    RegisterBlock(16, nullptr, 14, "Iron Ore");
    RegisterBlock(17, nullptr, 15, "Coal Ore");
}

void Block::Database::RegisterBlock(int id, const std::vector<float> *faceVertices,
                                    const std::vector<float> *textureCoordinates,
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

    const std::vector<float> *fv = faceVertices == nullptr ? defaultFaceVertices : faceVertices;
    const std::vector<float> *tc = textureCoordinates;

    blocks[id] = new Block(id, fv, tc, displayName);
}

void Block::Database::RegisterBlock(int id, const std::vector<float> *faceVertices,
                                    int atlas_id, const char *displayName)
{
    const TextureAtlas &atlas = Minecraft::GetInstance().GetMasterRenderer().GetTextureAtlas();
    const std::vector<float> texturecoords[3] = {
            atlas.GetTexture(atlas.CalculatePositionFromIndex(id - 1)), // skip air
            atlas.GetTexture(atlas.CalculatePositionFromIndex(id - 1)),
            atlas.GetTexture(atlas.CalculatePositionFromIndex(id - 1))
    };

    RegisterBlock(id, faceVertices, atlas_id != -1 ? texturecoords : nullptr, displayName);
}

const Block::Block &Block::Database::GetBlock(int id)
{ return *blocks[id]; }
