#ifndef MINECRAFT_TEXTRENDERER_H
#define MINECRAFT_TEXTRENDERER_H

#include <string>

class TextRenderer
{
public:
    TextRenderer(bool direction, int x, int y);
    void Reset(bool direction, int x, int y);
    void Print(std::string text, ...);
    void PrintRight(std::string text, ...);
    void NewLine();

private:
    bool direction;
    int x, y;
    char buffer[65535];
};

#endif //MINECRAFT_TEXTRENDERER_H
