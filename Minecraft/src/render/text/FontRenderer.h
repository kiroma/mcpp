#ifndef MINECRAFT_FONTRENDERER_H
#define MINECRAFT_FONTRENDERER_H

class Texture;

class ShaderProgram;

#include <SFML/Graphics/Color.hpp>
#include <glm/ext.hpp>
#include <memory>

#define MINECRAFT_CHARACTER_SIZE 8

class FontRenderer
{
public:
    FontRenderer(const char *texture_path);
    ~FontRenderer();

    void DrawString(std::string text, glm::ivec2 pos);
    void DrawString(std::string text, glm::ivec2 pos, sf::Color color);
    int GetStringWidth(std::string text);

private:
    std::unique_ptr<Texture> texture;
    std::unique_ptr<ShaderProgram> shader;

    unsigned int vaoID, verticesID, textureCoordsID;
};

#endif //MINECRAFT_FONTRENDERER_H
