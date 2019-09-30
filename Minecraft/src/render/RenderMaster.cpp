#include "RenderMaster.h"

#include "../world/World.h"
#include "../world/Chunk.h"
#include "../performance/ChunkStatistics.h"
#include "FontRenderer.h"

#include <iostream>
#include <unordered_map>
#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>

RenderMaster::RenderMaster()
{
    // Load textures and shaders
    textureSolidBlocks = new TextureAtlas("../res/textures/atlas.png", 16);
    shaderSolidBlock = new Shader("../res/shaders/solid/vertex.glsl", "../res/shaders/solid/fragment.glsl");
    shaderSolidBlock->Uniform1f("main_texture", glm::vec1(0));

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

    // Render chunks
    RenderChunks(world);

    // Render GUI
    const Minecraft &minecraft = Minecraft::GetInstance();
    const Camera &camera = minecraft.GetMasterRenderer().GetViewCamera();

    char buffer[65536];
    sprintf(buffer, "Minecraft C++ %d FPS (DT %.4f)", (int) minecraft.GetDebugFPS(),
            Minecraft::GetInstance().GetDeltaTime());
    FontRenderer::DrawString(buffer, glm::ivec2(1, 1));
    sprintf(buffer, "%d chunk updates (C %d D %d L %d R %d)", ChunkStatistics::GetChunkUpdates(ChunkStatistics::ALL),
            ChunkStatistics::GetChunkUpdates(ChunkStatistics::CREATE),
            ChunkStatistics::GetChunkUpdates(ChunkStatistics::DESTROY),
            ChunkStatistics::GetChunkUpdates(ChunkStatistics::LOAD),
            ChunkStatistics::GetChunkUpdates(ChunkStatistics::REMESH));
    FontRenderer::DrawString(buffer, glm::ivec2(1, 8 + 1));
    sprintf(buffer, "X %.2f Y %.2f Z %.2f", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    FontRenderer::DrawString(buffer, glm::ivec2(1, (2 * 8) + 1));

    // Leave it as it was
    glDepthMask(false);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void RenderMaster::SubmitProjection(const glm::mat4 projection) const
{
    shaderSolidBlock->Enable();
    shaderSolidBlock->UniformMat4("projection_matrix", projection);
    shaderSolidBlock->Disable();
}

void RenderMaster::RenderChunks(const World &world)
{
    // Load shader
    camera->Update();
    shaderSolidBlock->Enable();
    shaderSolidBlock->UniformMat4("view_matrix", camera->GenerateViewMatrix());

    // For every full chunk in the world
    for (auto it = world.RenderBegin(); it != world.RenderEnd(); it++) {
        //std::cout << "Rendering chunk: " << it->second << "." << std::endl;

        // For every chunk section of the full chunk
        for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++) {
            // Bind and enable stuff
            glBindVertexArray(it->second->GetSections()[i]->GetVAOID());
            for (int j = 0; j < MINECRAFT_CHUNK_SECTIONS; j++) {
                if (it->second->GetSections()[j]->GetVBOID(j) != -1)
                    glEnableVertexAttribArray(j);
            }

            // Position chunk
            glm::ivec2 chunkpos = it->second->GetPosition();
            shaderSolidBlock->UniformMat4("model_matrix", glm::translate(glm::mat4(1.0f), glm::vec3(
                    chunkpos.x * MINECRAFT_CHUNK_SIZE, i * MINECRAFT_CHUNK_SIZE,
                    chunkpos.y * MINECRAFT_CHUNK_SIZE)));

            // Bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureSolidBlocks->GetInternalTexture().GetID());

            // The call of truth
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->second->GetSections()[i]->GetIBOID());
            glDrawElements(GL_TRIANGLES, it->second->GetSections()[i]->GetCount(), GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // Disable models
            for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++) {
                if (it->second->GetSections()[i]->GetVBOID(i) != -1)
                    glDisableVertexAttribArray(i);
            }

            // Unbind stuff
            glBindVertexArray(0);
        }
    }

    shaderSolidBlock->Disable();
}

void RenderMaster::LoadCamera(Camera *camera)
{
    if (camera == nullptr) cameraLoaded = false; else cameraLoaded = true;
    this->camera = camera;
}
