#ifndef MINECRAFT_CHUNKSTATISTICS_H
#define MINECRAFT_CHUNKSTATISTICS_H

namespace ChunkStats
{
    enum UpdateType
    {
        CREATE,
        DESTROY,
        LOAD,
        REMESH,
        ALL
    };

    class Statistics {
    public:
        void Update();
        void RegisterChunkUpdate(UpdateType type);
        int GetChunkUpdates(UpdateType type);

    private:
        int tupdateC, tupdateD, tupdateL, tupdateR;
        int updateC, updateD, updateL, updateR;
        float dtime = 0;
    };
}

#endif //MINECRAFT_CHUNKSTATISTICS_H
