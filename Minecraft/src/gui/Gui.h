#ifndef MINECRAFT_GUI_H
#define MINECRAFT_GUI_H

#include <ext.hpp>

class Gui
{
public:
    glm::ivec2 GetPosition() const
    { return pos; }
    glm::ivec2 GetSize() const
    { return size; }
    int Area() const
    { return size.x * size.y; }

private:
    glm::ivec2 pos, size;
};

#endif //MINECRAFT_GUI_H
