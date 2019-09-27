#ifndef MINECRAFT_RENDERMASTER_H
#define MINECRAFT_RENDERMASTER_H

#include "Shader.h"
#include "../entity/Camera.h"
#include "Texture.h"
#include "TextureAtlas.h"

class World;

#define MC_RENDER_PASSES 2

class RenderMaster
{
public:
    RenderMaster();
    ~RenderMaster();

    void RenderWorld(const World &world);
    void SubmitProjection(const glm::mat4 projection) const;
    void LoadCamera(Camera *camera);

    const Shader &GetCurrentShader() const
    { return *shaderSolidBlock; }
    const TextureAtlas &GetTextureAtlas() const
    { return *textureSolidBlocks; }
    const Camera &GetViewCamera() const
    { return *camera; }

private:
    Shader *shaderSolidBlock;
    TextureAtlas *textureSolidBlocks;
    Camera *camera;

    // World rendering routines
    void RenderChunks(const World &world);
};

#endif //MINECRAFT_RENDERMASTER_H
