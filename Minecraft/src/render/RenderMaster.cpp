#include "RenderMaster.h"

#include "../entity/Camera.h"
#include "../gui/GuiScreen.h"
#include "../performance/ChunkStatistics.h"
#include "../world/FullChunk.h"
#include "../world/World.h"
#include "../world/Chunk.h"
#include "FontRenderer.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureAtlas.h"

#include <iostream>
#include <unordered_map>
#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>

RenderMaster::RenderMaster()
{
    // Load textures and shaders
    textureSolidBlocks = new TextureAtlas("../res/textures/atlas.png", 16);
    shaderSolidBlock = new Shader("../res/shaders/solid/vertex.glsl", "../res/shaders/solid/fragment.glsl");
    glUniform1i(shaderSolidBlock->UniformLocation("main_texture"), 0);

    // Initialize the font renderer
    FontRenderer::Inititalize("../res/textures/font/ascii.png");
}

RenderMaster::~RenderMaster()
{
    delete textureSolidBlocks;
    delete shaderSolidBlock;
    delete camera;
}

// --------------------------------------------------------------
//  Render the world
// --------------------------------------------------------------
void RenderMaster::RenderWorld(World &world)
{
    // Prepare
    glPushMatrix();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(true);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_TEXTURE_2D);

    glClearColor(0.2f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render stuff
    RenderChunks(world);
    RenderGUI();

    // Leave the OpenGL state as it was
    glDepthMask(false);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void RenderMaster::SubmitProjection(const glm::mat4 projection) const
{
    shaderSolidBlock->Enable();
    glUniformMatrix4fv(shaderSolidBlock->UniformLocation("projection_matrix"), 1, false, glm::value_ptr(projection));
    shaderSolidBlock->Disable();
}

void RenderMaster::RenderChunks(const World &world)
{
    // Load shader
    camera->Update();
    shaderSolidBlock->Enable();
    glUniformMatrix4fv(shaderSolidBlock->UniformLocation("view_matrix"), 1, false,
                       glm::value_ptr(camera->GenerateViewMatrix()));

    // For every full chunk in the world
    for (auto it = world.RenderBegin(); it != world.RenderEnd(); it++) {
        //std::cout << "Rendering chunk: " << it->second << "." << std::endl;

        // For every chunk section of the full chunk
        for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++) {
            // Bind and enable stuff
            glBindVertexArray(it->second->GetSections()[i]->GetVAOID());
            for (int j = 0; j < MINECRAFT_CHUNK_SECTIONS; j++) {
                if (it->second->GetSections()[j]->GetVBOID(j) != (unsigned int) -1)
                    glEnableVertexAttribArray(j);
            }

            // Position chunk
            glm::ivec2 chunkpos = it->second->GetPosition();
            glUniformMatrix4fv(shaderSolidBlock->UniformLocation("model_matrix"), 1, false,
                               glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(
                                       chunkpos.x * MINECRAFT_CHUNK_SIZE, i * MINECRAFT_CHUNK_SIZE,
                                       chunkpos.y * MINECRAFT_CHUNK_SIZE))));

            // Bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureSolidBlocks->GetInternalTexture().GetID());

            // The call of truth
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->second->GetSections()[i]->GetIBOID());
            glDrawElements(GL_TRIANGLES, it->second->GetSections()[i]->GetCount(), GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // Disable models
            for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++) {
                if (it->second->GetSections()[i]->GetVBOID(i) != (unsigned int) -1)
                    glDisableVertexAttribArray(i);
            }

            // Unbind stuff
            glBindVertexArray(0);
        }
    }

    shaderSolidBlock->Disable();
}

void RenderMaster::RenderGUI()
{
    GuiScreen *currentscreen = Minecraft::GetInstance().GetCurrentGUIScreen();
    if (currentscreen != nullptr) {
        currentscreen->DrawScreen();
    }
}

void RenderMaster::LoadCamera(Camera *camera)
{
    if (camera == nullptr) cameraLoaded = false; else cameraLoaded = true;
    this->camera = camera;
}
