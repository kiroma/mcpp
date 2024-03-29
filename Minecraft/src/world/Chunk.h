#ifndef MINECRAFT_CHUNK_H
#define MINECRAFT_CHUNK_H

class World;

class FullChunk;

#include "Block.h"

#include <vector>
#include <glm/vec3.hpp>

#define MINECRAFT_CHUNK_SIZE 16
#define MINECRAFT_CHUNK_VBO_COUNT 2

class Chunk
{
public:
    // Constructor & destructor
    Chunk(World *world, FullChunk *parent, int section_number);
    ~Chunk();
    void Load();

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
    World &world;
    const FullChunk &parent;
    bool needsRebuild;
    int sectionNumber;

    // OpenGL
    unsigned int count, vaoID, iboID;
    unsigned int vbos[16];
    unsigned int vbosComponentCount[16]{3, 2}; // Add here the component count for future VBOs
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> textureCoords;
    unsigned int indicesIndex;
    bool loaded = false;
};

#endif //MINECRAFT_CHUNK_H
