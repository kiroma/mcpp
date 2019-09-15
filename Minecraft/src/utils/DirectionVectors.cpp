#include "DirectionVectors.h"

DirectionVectors::DirectionVectors()
        : DirectionVectors(Block::Position(0, 0, 0))
{ }

DirectionVectors::DirectionVectors(Block::Position pos)
        : position(pos)
{ }

void DirectionVectors::Update(Block::Position pos)
{ this->position = pos; }

Block::Position DirectionVectors::Right()
{ return Block::Position(position.x + 1, position.y, position.z); }

Block::Position DirectionVectors::Left()
{ return Block::Position(position.x - 1, position.y, position.z); }

Block::Position DirectionVectors::Top()
{ return Block::Position(position.x, position.y + 1, position.z); }

Block::Position DirectionVectors::Bottom()
{ return Block::Position(position.x, position.y - 1, position.z); }

Block::Position DirectionVectors::Front()
{ return Block::Position(position.x, position.y, position.z + 1); }

Block::Position DirectionVectors::Back()
{ return Block::Position(position.x, position.y, position.z - 1); }
