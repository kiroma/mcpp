#include "Chunk.h"

#include "World.h"
#include "BlockFace.h"
#include "FullChunk.h"
#include "../utils/Vectors.h"
#include "../utils/DirectionVectors.h"

#include <iostream>
#include <vector>
#include <string.h>
#include <vec3.hpp>
#include <GL/glew.h>

// --------------------------------------------------------------
//  Constructor: Allocate resources
// --------------------------------------------------------------
Chunk::Chunk(World *world, FullChunk *parent, int y_position)
        : count(0), parent(*parent), y_position(y_position)
{
    // Generate VBOs
    memset(vbos, -1, 16 * sizeof(unsigned int));
    for (int i = 0; i < MINECRAFT_CHUNK_VBO_COUNT; i++) {
        glGenBuffers(1, &vbos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    }

    // Generate IBO
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // Generate VAO and assign VBOs to slot 0 of it
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    for (int i = 0; i < MINECRAFT_CHUNK_VBO_COUNT; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, vbosComponentCount[i], GL_FLOAT, false, 0, nullptr);
        glDisableVertexAttribArray(i);
    }

    // Unbind stuff
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Update chunk itself
    srand((unsigned int) this);
    memset(blocks, 0, MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SIZE * sizeof(Block::State));
    for (int y = 0; y < MINECRAFT_CHUNK_SIZE; y++)
        for (int x = 0; x < MINECRAFT_CHUNK_SIZE; x++)
            for (int z = 0; z < MINECRAFT_CHUNK_SIZE; z++)
                SetChunkState(Block::Position(x, y, z), Block::State(1));

    needsRebuild = true;
    Update();
}

// --------------------------------------------------------------
//  Destructor: Clean up
// --------------------------------------------------------------
Chunk::~Chunk()
{
    // Release resources
    FreeMemory();

    // Delete VBOs and VAO
    for (int i = 0; i < MINECRAFT_CHUNK_VBO_COUNT; i++) {
        glDeleteBuffers(1, &vbos[i]);
    }

    glDeleteBuffers(1, &iboID);
    glDeleteVertexArrays(1, &vaoID);

    // Make sure no VAO or VBO is bound (heaven forbid using a deleted buffer)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// --------------------------------------------------------------
//  Set the state of a block in the chunk at the
//  specified position
// --------------------------------------------------------------
void Chunk::SetChunkState(const Block::Position &position, Block::State state)
{
    if ((position.x > MINECRAFT_CHUNK_SIZE || position.x < 0) ||
        (position.y > MINECRAFT_CHUNK_SIZE || position.y < 0) ||
        (position.z > MINECRAFT_CHUNK_SIZE || position.z < 0))
        return;

    blocks[position.x][position.y][position.z] = state;
    needsRebuild = true;
}

// --------------------------------------------------------------
//  Get the state of a block in the chunk at the
//  specified position
// --------------------------------------------------------------
Block::State Chunk::GetChunkState(const Block::Position &position) const
{
    if ((position.x > MINECRAFT_CHUNK_SIZE || position.x < 0) ||
        (position.y > MINECRAFT_CHUNK_SIZE || position.y < 0) ||
        (position.z > MINECRAFT_CHUNK_SIZE || position.z < 0)) {
        Block::State air;
        return air;
    }

    return blocks[position.x][position.y][position.z];
}

// --------------------------------------------------------------
//  Update the chunk:
//      - Rebuild mesh if needed
//      - Update blocks on it
// --------------------------------------------------------------
void Chunk::Update()
{
    if (needsRebuild) {
        Remesh();
        needsRebuild = false;
    }
}

// --------------------------------------------------------------
//  Reconstruct chunk mesh based on the blocks it contains
// --------------------------------------------------------------
void Chunk::Remesh()
{
    using namespace BlockFace;

#ifdef MINECRAFT_DEBUG
    std::cout << "Remeshing chunk " << std::hex << this << "..." << std::endl;
#endif

    FreeMemory();
    indicesIndex = 0;

    DirectionVectors vectors;
    for (int y = 0; y < MINECRAFT_CHUNK_SIZE; y++) {
        for (int x = 0; x < MINECRAFT_CHUNK_SIZE; x++) {
            for (int z = 0; z < MINECRAFT_CHUNK_SIZE; z++) {
                const Block::Position pos(x, y, z);
                const Block::State state = GetChunkState(pos);
                const Block::Block &block = Block::Database::GetBlock(state.id);

                if (state.id == 0) {
                    continue;
                }

                vectors.Update(pos);

                /*
                for (int i = 0; i < 2 * 6; i++) {
                    std::cout << block.textureCoordinates[1][i] << ", ";
                }
                std::cout << std::endl;
                 */

                TryAddFace(block.GetFaceVertices(0), block.GetTextureCoordinates(0), pos,
                           vectors.Right());
                TryAddFace(block.GetFaceVertices(1), block.GetTextureCoordinates(0), pos,
                           vectors.Left());
                TryAddFace(block.GetFaceVertices(2), block.GetTextureCoordinates(1), pos,
                           vectors.Top());
                TryAddFace(block.GetFaceVertices(3), block.GetTextureCoordinates(1), pos,
                           vectors.Bottom());
                TryAddFace(block.GetFaceVertices(4), block.GetTextureCoordinates(2), pos,
                           vectors.Front());
                TryAddFace(block.GetFaceVertices(5), block.GetTextureCoordinates(2), pos,
                           vectors.Back());
            }
        }
    }

    // Load indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Load vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Load texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), &textureCoords[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Chunk::TryAddFace(const std::vector<float> &faceVertices, const std::vector<float> &texCoords,
                       const Block::Position &localPosition, const Block::Position &nextPosition)
{
    if (ShouldMakeBlockFaceAdjacentTo(nextPosition)) {
        AddFace(faceVertices, texCoords, localPosition);
    }
}

bool Chunk::ShouldMakeBlockFaceAdjacentTo(const Block::Position &position)
{
    if ((position.x > MINECRAFT_CHUNK_SIZE - 1 || position.x < 0) ||
        (position.y > MINECRAFT_CHUNK_SIZE - 1 || position.y < 0) ||
        (position.z > MINECRAFT_CHUNK_SIZE - 1 || position.z < 0)) {
        return true;
    }

    return GetChunkState(position).id == 0;
}

void Chunk::AddFace(const std::vector<float> &faceVertices, const std::vector<float> &texCoords,
                    const Block::Position &localPosition)
{
    for (int i = 0, j = 0; i < 4; i++) {
        Vectors::Concat(vertices, {
                faceVertices[j++] + (float) localPosition.x,
                faceVertices[j++] + (float) localPosition.y,
                faceVertices[j++] + (float) localPosition.z
        });
    }

    Vectors::Concat<float>(textureCoords, texCoords);
    Vectors::Concat(indices, {
            indicesIndex,
            indicesIndex + 1,
            indicesIndex + 2,
            indicesIndex + 2,
            indicesIndex + 3,
            indicesIndex
    });

    indicesIndex += 4;
}

void Chunk::ShrinkVectors()
{
    vertices.shrink_to_fit();
    indices.shrink_to_fit();
    textureCoords.shrink_to_fit();
}

void Chunk::FreeMemory()
{
    vertices.clear();
    indices.clear();
    textureCoords.clear();
    ShrinkVectors();
}
