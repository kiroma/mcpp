#include "ChunkStatistics.h"
#include "../Minecraft.h"

int tupdateC, tupdateD, tupdateL, tupdateR;
int updateC, updateD, updateL, updateR;
float dtime = 0;

void ChunkStatistics::Update()
{
    dtime += Minecraft::GetInstance().GetDeltaTime();

    if (dtime > 1) {
        dtime = 0;
        updateC = tupdateC;
        updateD = tupdateD;
        updateL = tupdateL;
        updateR = tupdateR;
        tupdateC = 0;
        tupdateD = 0;
        tupdateL = 0;
        tupdateR = 0;
    }
}

void ChunkStatistics::RegisterChunkUpdate(UpdateType type)
{
    switch (type) {
        case CREATE:
            tupdateC++;
            break;
        case DESTROY:
            tupdateD++;
            break;
        case LOAD:
            tupdateL++;
            break;
        case REMESH:
            tupdateR++;
            break;
        default:
            break;
    }
}

int ChunkStatistics::GetChunkUpdates(UpdateType type)
{
    switch (type) {
        case CREATE:
            return updateC;
        case DESTROY:
            return updateD;
        case LOAD:
            return updateL;
        case REMESH:
            return updateR;
        case ALL:
            return updateC + updateD + updateL + updateR;
        default:
            break;
    }

    return 0;
}
