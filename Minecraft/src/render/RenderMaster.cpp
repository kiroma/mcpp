#include "RenderMaster.h"

#include "../world/World.h"
#include "../world/Chunk.h"

#include <GL/glew.h>
#include <iostream>


RenderMaster::RenderMaster(glm::mat4 projectionMatrix) {
    shaderSolidBlock = new Shader("../res/solid/vertex.glsl", "../res/solid/fragment.glsl");
    camera = new Camera;
    SubmitProjection(projectionMatrix);
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
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world.chunk->GetIBOID());
            glBindBuffer(GL_ARRAY_BUFFER, world.chunk->GetVBOID());
            glEnableVertexAttribArray(0);

            glDrawElements(GL_TRIANGLES, world.chunk->GetCount(), GL_UNSIGNED_INT, nullptr);

            glDisableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            shaderSolidBlock->Disable();
            break;
        }
        default:
            break;
    }
}
