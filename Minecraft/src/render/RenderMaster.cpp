#include "RenderMaster.h"

#include "../world/World.h"
#include "../world/Chunk.h"

#include <GL/glew.h>
#include <iostream>

RenderMaster::RenderMaster()
{
    // Load textures and shaders
    textureSolidBlocks = new TextureAtlas("../res/textures/atlas.png", 16);
    shaderSolidBlock = new Shader("../res/shaders/solid/vertex.glsl", "../res/shaders/solid/fragment.glsl");
    shaderSolidBlock->Uniform1f("main_texture", glm::vec1(0));

    // Load camera
    camera = new Camera;
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
void RenderMaster::RenderWorld(const World &world)
{
    for (int i = 0; i < MC_RENDER_PASSES; i++)
        RenderWorldPass(i, world);
}

void RenderMaster::SubmitProjection(glm::mat4 projection) const
{
    shaderSolidBlock->Enable();
    shaderSolidBlock->UniformMat4("projection_matrix", projection);
    shaderSolidBlock->Disable();
}

// --------------------------------------------------------------
//  Render a pass of the world
//  Passes:
//      Pass 1 - Render chunks
//      (more passes to be added soon, like clouds and water...)
// --------------------------------------------------------------
void RenderMaster::RenderWorldPass(int pass, const World &world)
{
    switch (pass) {
        case 0: {
            // Load shader
            camera->Update();
            shaderSolidBlock->Enable();
            shaderSolidBlock->UniformMat4("view_matrix", camera->GenerateViewMatrix());

            // For every chunk section
            for (int i = 0; i < MINECRAFT_CHUNK_SECTIONS; i++) {
                // Bind and enable stuff
                glBindVertexArray(world.chunk->GetSections()[i]->GetVAOID());
                for (int i = 0; i < 16; i++) {
                    if (world.chunk->GetSections()[i]->GetVBOID(i) != -1)
                        glEnableVertexAttribArray(i);
                }

                // Position chunk
                shaderSolidBlock->UniformMat4("model_matrix", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, i * 16.0f, 0.0f)));

                // Bind textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureSolidBlocks->GetInternalTexture().GetID());

                // The call of truth
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world.chunk->GetSections()[i]->GetIBOID());
                glDrawElements(GL_TRIANGLES, world.chunk->GetSections()[i]->GetCount(), GL_UNSIGNED_INT, nullptr);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                // Disable models
                for (int i = 0; i < 16; i++) {
                    if (world.chunk->GetSections()[i]->GetVBOID(i) != -1)
                        glDisableVertexAttribArray(i);
                }

                // Unbind stuff
                glBindVertexArray(0);
            }

            shaderSolidBlock->Disable();
            break;
        }
        default:
            break;
    }
}
