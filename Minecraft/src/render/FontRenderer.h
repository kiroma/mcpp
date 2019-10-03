#ifndef MINECRAFT_FONTRENDERER_H
#define MINECRAFT_FONTRENDERER_H

#include <SFML/Graphics/Color.hpp>
#include <ext.hpp>

#define MINECRAFT_CHARACTER_SIZE 8

namespace FontRenderer
{
    void Inititalize(const char *texture_path);
    void DrawString(const char *text, glm::ivec2 pos);
    void DrawString(const char *text, glm::ivec2 pos, sf::Color color);
}

#endif //MINECRAFT_FONTRENDERER_H
