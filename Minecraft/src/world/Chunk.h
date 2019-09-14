#ifndef MINECRAFT_CHUNK_H
#define MINECRAFT_CHUNK_H

class World;

#include "block.h"

#include <vector>
#include <vec3.hpp>

#define MINECRAFT_CHUNK_SIZE 16
#define MINECRAFT_CHUNK_VBO_COMPONENT_COUNT 3

class Chunk {
public:
    // Constructor & destructor
    Chunk(const World &world);
    ~Chunk();

    // Block management
    void SetChunkState(const Block::Position& position, Block::State state);
    Block::State GetChunkState(const Block::Position& position) const;

    // OpenGL
    void Update();
    unsigned int GetVBOID() const { return vboID; }
    unsigned int GetIBOID() const { return iboID; }
    unsigned int GetVAOID() const { return vaoID; }
    unsigned int GetCount() const { return indices.size(); }
    void AddFace(const std::vector<float>& faceVertices, const Block::Position& localPosition);

private:
    void Remesh();
    void TryAddFace(const std::vector<float>& faceVertices, const Block::Position& localPosition, const Block::Position& nextPosition);
    bool ShouldMakeBlockFaceAdjacentTo(const Block::Position &position);

private:
    // Block states
    Block::State blocks[MINECRAFT_CHUNK_SIZE][MINECRAFT_CHUNK_SIZE][MINECRAFT_CHUNK_SIZE];

    // Chunk updating
    bool needsRebuild;

    // OpenGL
    unsigned int vaoID, vboID, iboID, count;
    unsigned int indicesIndex;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif //MINECRAFT_CHUNK_H
