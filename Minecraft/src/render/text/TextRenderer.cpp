#include "TextRenderer.h"
#include "../../Minecraft.h"
#include "FontRenderer.h"

#include <cstdarg>

TextRenderer::TextRenderer(bool direction, int x, int y)
{ Reset(direction, x, y); }

void TextRenderer::Reset(bool direction, int x, int y)
{
    this->direction = direction;
    this->x = x;
    this->y = y;
}

void TextRenderer::Print(std::string text, ...)
{
    va_list l;
    va_start(l, text);
    vsprintf(buffer, text.data(), l);
    va_end(l);

    Minecraft::GetInstance().GetFontRenderer().DrawString(buffer, glm::ivec2(x, y));
    y += direction ? MINECRAFT_CHARACTER_SIZE : -MINECRAFT_CHARACTER_SIZE;
}

void TextRenderer::PrintRight(std::string text, ...)
{
    va_list l;
    va_start(l, text);
    vsprintf(buffer, text.data(), l);
    va_end(l);

    Minecraft::GetInstance().GetFontRenderer().DrawString(buffer, glm::ivec2(
            x - Minecraft::GetInstance().GetFontRenderer().GetStringWidth(buffer), y));
    y += direction ? MINECRAFT_CHARACTER_SIZE : -MINECRAFT_CHARACTER_SIZE;
}

void TextRenderer::NewLine()
{
    y += direction ? MINECRAFT_CHARACTER_SIZE : -MINECRAFT_CHARACTER_SIZE;
}
