#ifndef MINECRAFT_CHUNK_H
#define MINECRAFT_CHUNK_H

class World;

#include "Block.h"

#include <vector>
#include <vec3.hpp>

#define MINECRAFT_CHUNK_SIZE 16
#define MINECRAFT_CHUNK_VBO_COUNT 2

class Chunk
{
public:
    // Constructor & destructor
    Chunk(const World &world);
    ~Chunk();

    // Block management
    void SetChunkState(const Block::Position &position, Block::State state);
    Block::State GetChunkState(const Block::Position &position) const;

    // OpenGL
    void Update();
    unsigned int GetVBOID(int vbo) const
    { return vbos[vbo]; };
    unsigned int GetIBOID() const
    { return iboID; }
    unsigned int GetVAOID() const
    { return vaoID; }
    unsigned int GetCount() const
    { return indices.size(); }
    void AddFace(const std::vector<float> &faceVertices, const std::vector<float> &texCoords,
                 const Block::Position &localPosition);

    void FreeMemory();

private:
    void Remesh();

    void TryAddFace(const std::vector<float> &faceVertices, const std::vector<float> &texCoords,
                    const Block::Position &localPosition, const Block::Position &nextPosition);
    bool ShouldMakeBlockFaceAdjacentTo(const Block::Position &position);
    void ShrinkVectors();

private:
    // Block states
    Block::State blocks[MINECRAFT_CHUNK_SIZE][MINECRAFT_CHUNK_SIZE][MINECRAFT_CHUNK_SIZE];

    // Chunk updating
    bool needsRebuild;

    // OpenGL
    unsigned int vaoID, iboID, count;
    unsigned int vbos[16];
    unsigned int vbosComponentCount[16]{3, 2}; // Add here the component count for future VBOs
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> textureCoords;
    unsigned int indicesIndex;
};

#endif //MINECRAFT_CHUNK_H
