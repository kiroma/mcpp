#include "FontRenderer.h"

#include "../../gui/ScaledResolution.h"
#include "../../utils/InputOutput.h"
#include "../../Minecraft.h"
#include "../shaders/ShaderProgram.h"
#include "../Texture.h"

#include <GL/glew.h>
#include <iostream>

FontRenderer::FontRenderer(const char *texture_path)
{
    // Initialize texture
    texture = std::make_unique<Texture>(texture_path);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    // Initialize shader
    shader = std::make_unique<ShaderProgram>();
    shader->AttachShader(std::make_unique<Shader>(VERTEX, "../res/shaders/font/vertex.glsl"));
    shader->AttachShader(std::make_unique<Shader>(FRAGMENT, "../res/shaders/font/fragment.glsl"));
    shader->Build();

    // Initialize arrays (woah, look at that code wall)
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &verticesID);
    glGenBuffers(1, &textureCoordsID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordsID);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindVertexArray(vaoID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordsID);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

FontRenderer::~FontRenderer()
{
    glDeleteBuffers(1, &verticesID);
    glDeleteBuffers(1, &textureCoordsID);
    glDeleteVertexArrays(1, &vaoID);
}

void FontRenderer::DrawString(std::string text, glm::ivec2 pos)
{ DrawString(text, pos, sf::Color::White); }

void FontRenderer::DrawString(std::string text, glm::ivec2 pos, sf::Color color)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(vaoID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    shader->Enable();
    glUniformMatrix4fv(shader->UniformLocation("matrix"), 1, false,
                       glm::value_ptr(Minecraft::GetInstance().GetScaledResolution().GenerateMatrix()));
    glUniform1i(shader->UniformLocation("main_texture"), 0);
    glUniform4f(shader->UniformLocation("main_color"), color.r, color.g, color.b, color.a);

    int x = pos.x;
    for (auto c = text.begin(); c != text.end(); c++) {
        static const int CHARACTERS = texture->GetSize() / MINECRAFT_CHARACTER_SIZE;
        static const float CHARACTER_UNIT = (float) MINECRAFT_CHARACTER_SIZE / (float) texture->GetSize();

        int index = (int) *c;
        int tx = index % CHARACTERS * MINECRAFT_CHARACTER_SIZE;
        int ty = index / CHARACTERS * MINECRAFT_CHARACTER_SIZE;
        float cx = (float) tx / (float) texture->GetSize();
        float cy = (float) ty / (float) texture->GetSize();

        const float vertices[] = {
                (float) x, (float) pos.y,
                (float) x + MINECRAFT_CHARACTER_SIZE, (float) pos.y,
                (float) x + MINECRAFT_CHARACTER_SIZE, (float) pos.y + MINECRAFT_CHARACTER_SIZE,
                (float) x, (float) pos.y + MINECRAFT_CHARACTER_SIZE
        };

        const float texturecoordinates[] = {
                cx, cy,
                cx + CHARACTER_UNIT, cy,
                cx + CHARACTER_UNIT, cy + CHARACTER_UNIT,
                cx, cy + CHARACTER_UNIT
        };

        glBindBuffer(GL_ARRAY_BUFFER, verticesID);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, textureCoordsID);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), texturecoordinates, GL_DYNAMIC_DRAW);

        glDrawArrays(GL_QUADS, 0, 8);

        x += MINECRAFT_CHARACTER_SIZE - 2;
    }

    shader->Disable();

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

int FontRenderer::GetStringWidth(std::string text)
{
    int ret = 0;
    for (auto c = text.begin(); c != text.end(); c++)
        ret += MINECRAFT_CHARACTER_SIZE - 2;

    return ret;
}
