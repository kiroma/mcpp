#include "Chunk.h"

#include "World.h"
#include "BlockFace.h"
#include "../utils/Vectors.h"

#include <iostream>
#include <vector>
#include <string.h>
#include <vec3.hpp>
#include <GL/glew.h>

// --------------------------------------------------------------
//  Constructor: Allocate resources
// --------------------------------------------------------------
Chunk::Chunk(const World &world)
        : count(0) {
    // Generate VBO
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // Generate IBO
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // Generate VAO and assign VBOs to slot 0 of it
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, MINECRAFT_CHUNK_VBO_COMPONENT_COUNT, GL_FLOAT, false, 0, nullptr);
    glDisableVertexAttribArray(0);

    // Unbind stuff
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Update chunk itself
    memset(blocks, 0, MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SIZE * MINECRAFT_CHUNK_SIZE * sizeof(Block::State));
    SetChunkState(Block::Position(0, 0, 0), Block::State(1));
    SetChunkState(Block::Position(3, 3, 4), Block::State(1));
    SetChunkState(Block::Position(3, 3, 5), Block::State(1));
    SetChunkState(Block::Position(3, 3, 6), Block::State(1));
    SetChunkState(Block::Position(3, 3, 7), Block::State(1));
    SetChunkState(Block::Position(3, 4, 7), Block::State(1));
    SetChunkState(Block::Position(3, 5, 7), Block::State(1));

    needsRebuild = true;
    Update();
}

// --------------------------------------------------------------
//  Destructor: Clean up
// --------------------------------------------------------------
Chunk::~Chunk() {
    // Delete VBO and VAO
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &iboID);
    glDeleteVertexArrays(1, &vaoID);

    // Make sure no VAO or VBO is bound(heaven forbid using a deleted buffer)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// --------------------------------------------------------------
//  Set the state of a block in the chunk at the
//  specified position
// --------------------------------------------------------------
void Chunk::SetChunkState(const Block::Position &position, Block::State state) {
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
Block::State Chunk::GetChunkState(const Block::Position &position) const {
    if ((position.x > 16 || position.x < 0) ||
        (position.y > 16 || position.y < 0) ||
        (position.z > 16 || position.z < 0)) {
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
void Chunk::Update() {
    if (needsRebuild) {
        Remesh();
        needsRebuild = false;
    }
}

// --------------------------------------------------------------
//  Reconstruct chunk mesh based on the blocks it contains
// --------------------------------------------------------------
void Chunk::Remesh() {
    using namespace BlockFace;

#ifdef MINECRAFT_DEBUG
    std::cout << "Remeshing chunk " << std::hex << this << "..." << std::endl;
#endif

    vertices.clear();
    indices.clear();
    indicesIndex = 0;

    for (int y = 0; y < MINECRAFT_CHUNK_SIZE; y++) {
        for (int x = 0; x < MINECRAFT_CHUNK_SIZE; x++) {
            for (int z = 0; z < MINECRAFT_CHUNK_SIZE; z++) {
                const Block::Position pos(x, y, z);
                const Block::State state = GetChunkState(pos);

                if (state.id == 0) {
                    continue;
                }

                TryAddFace(rightFace, pos, Block::Position(pos.x + 1, pos.y, pos.z));
                TryAddFace(leftFace, pos, Block::Position(pos.x - 1, pos.y, pos.z));
                TryAddFace(topFace, pos, Block::Position(pos.x, pos.y + 1, pos.z));
                TryAddFace(bottomFace, pos, Block::Position(pos.x, pos.y - 1, pos.z));
                TryAddFace(frontFace, pos, Block::Position(pos.x, pos.y, pos.z + 1));
                TryAddFace(backFace, pos, Block::Position(pos.x, pos.y, pos.z - 1));
            }
        }
    }

    // Load indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Load vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*std::cout << (float) vertices.size() << std::endl;
    std::cout << (float) indices.size() << std::endl;*/
}

void
Chunk::TryAddFace(const std::vector<float> &faceVertices, const Block::Position &localPosition, const Block::Position &nextPosition) {
    if (ShouldMakeBlockFaceAdjacentTo(nextPosition)) {
        AddFace(faceVertices, localPosition);
    }
}

bool Chunk::ShouldMakeBlockFaceAdjacentTo(const Block::Position &position) {
    return GetChunkState(position).id == 0;
}

void Chunk::AddFace(const std::vector<float> &faceVertices, const Block::Position &localPosition) {
    for (int i = 0, j = 0; i < 4; i++) {
        vertices.push_back(faceVertices[j++] + (float) localPosition.x);
        vertices.push_back(faceVertices[j++] + (float) localPosition.y);
        vertices.push_back(faceVertices[j++] + (float) localPosition.z);
    }

    indices.insert(indices.end(), {
            indicesIndex,
            indicesIndex + 1,
            indicesIndex + 2,
            indicesIndex + 2,
            indicesIndex + 3,
            indicesIndex
    });

    indicesIndex += 4;
}
