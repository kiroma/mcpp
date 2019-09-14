#include "RenderMaster.h"

#include "../world/World.h"
#include "../world/Chunk.h"

#include <GL/glew.h>
#include <iostream>


RenderMaster::RenderMaster() {
    shaderSolidBlock = new Shader("../res/solid/vertex.glsl", "../res/solid/fragment.glsl");
    camera = new Camera;
}

RenderMaster::~RenderMaster() {
    delete shaderSolidBlock;
    delete camera;
}

// --------------------------------------------------------------
//  Render the world
// --------------------------------------------------------------
void RenderMaster::RenderWorld(const World &world) {
    for (int i = 0; i < MC_RENDER_PASSES; i++)
        RenderWorldPass(i, world);
}

void RenderMaster::SubmitProjection(glm::mat4 projection) const {
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
void RenderMaster::RenderWorldPass(int pass, const World &world) {
    switch (pass) {
        case 0: {
            camera->Update();
            shaderSolidBlock->Enable();
            shaderSolidBlock->UniformMat4("view_matrix", camera->GenerateViewMatrix());

            glBindVertexArray(world.chunk->GetVAOID());
            for (int i = 0; i < 16; i++) {
                if (world.chunk->GetVBOID(i) != -1)
                    glEnableVertexAttribArray(i);
            }

            // The call of truth
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world.chunk->GetIBOID());
            glDrawElements(GL_TRIANGLES, world.chunk->GetCount(), GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            for (int i = 0; i < 16; i++) {
                if (world.chunk->GetVBOID(i) != -1)
                    glDisableVertexAttribArray(i);
            }

            glBindVertexArray(0);
            shaderSolidBlock->Disable();

            break;
        }
        default:
            break;
    }
}
