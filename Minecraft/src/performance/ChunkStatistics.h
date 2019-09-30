#ifndef MINECRAFT_CHUNKSTATISTICS_H
#define MINECRAFT_CHUNKSTATISTICS_H

namespace ChunkStatistics
{
    enum UpdateType
    {
        CREATE,
        DESTROY,
        LOAD,
        REMESH,
        ALL
    };

    void Update();
    void RegisterChunkUpdate(UpdateType type);
    int GetChunkUpdates(UpdateType type);
}

#endif //MINECRAFT_CHUNKSTATISTICS_H
