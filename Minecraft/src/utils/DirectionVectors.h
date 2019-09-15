#ifndef MINECRAFT_DIRECTIONVECTORS_H
#define MINECRAFT_DIRECTIONVECTORS_H

#include "../world/Block.h"

class DirectionVectors
{
public:
    DirectionVectors();
    DirectionVectors(Block::Position pos);

    void Update(Block::Position pos);

    Block::Position Right();
    Block::Position Left();
    Block::Position Top();
    Block::Position Bottom();
    Block::Position Front();
    Block::Position Back();
private:
    Block::Position position;
};

#endif //MINECRAFT_DIRECTIONVECTORS_H
