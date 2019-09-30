#include "FontRenderer.h"

#include "Texture.h"
#include "Shader.h"
#include "../Minecraft.h"
#include "../utils/FileUtils.h"

#include <GL/glew.h>
#include <iostream>

#define MINECRAFT_CHARACTER_SIZE 8

Texture *texture;
Shader *shader;
unsigned int vaoID, verticesID, textureCoordsID;

void FontRenderer::Inititalize(const char *texture_path)
{
    // Initialize texture
    texture = new Texture(texture_path);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    // Initialize shader
    shader = new Shader("../res/shaders/font/vertex.glsl", "../res/shaders/font/fragment.glsl");

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

void FontRenderer::DrawString(const char *text, glm::ivec2 pos)
{ DrawString(text, pos, sf::Color::White); }

void FontRenderer::DrawString(const char *text, glm::ivec2 pos, sf::Color color)
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
    shader->UniformMat4("matrix", Minecraft::GetInstance().GetScaledResolution().GenerateMatrix());
    shader->Uniform1f("main_texture", glm::vec1(0));

    int x = pos.x;
    for (const char *c = text; *c != '\0'; c++) {
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
