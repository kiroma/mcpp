#ifndef MINECRAFT_RENDERMASTER_H
#define MINECRAFT_RENDERMASTER_H

#include "Shader.h"
#include "Camera.h"

class World;

#define MC_RENDER_PASSES 2

class RenderMaster {
public:
    RenderMaster();
    ~RenderMaster();

    void RenderWorld(const World &world);
    void SubmitProjection(glm::mat4 projection) const;
private:
    // Shaders
    Shader *shaderSolidBlock;

    // Camera
    Camera *camera;

    void RenderWorldPass(int pass, const World& world);
};

#endif //MINECRAFT_RENDERMASTER_H
