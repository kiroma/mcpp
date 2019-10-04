#ifndef MINECRAFT_RENDERMASTER_H
#define MINECRAFT_RENDERMASTER_H

#include "../entity/Camera.h"
#include "Shader.h"
#include "TextureAtlas.h"

class Texture;

class World;

#include <memory>
#include <mat4x4.hpp>

#define MC_RENDER_PASSES 2

class RenderMaster
{
    void RenderChunks(const World &world);
    void RenderGUI();

public:
    RenderMaster();
    ~RenderMaster()
    {}

    void RenderWorld(World &world);
    void SubmitProjection(const glm::mat4 projection) const;
    void LoadCamera(std::unique_ptr<Camera> camera);

    const Shader &GetCurrentShader() const
    { return *shaderSolidBlock; }
    const TextureAtlas &GetTextureAtlas() const
    { return *textureSolidBlocks; }
    const Camera &GetViewCamera() const
    { return *camera; }
    bool IsCameraAvailable() const
    { return cameraLoaded; }

private:
    std::unique_ptr<Shader> shaderSolidBlock;
    std::unique_ptr<TextureAtlas> textureSolidBlocks;
    std::unique_ptr<Camera> camera;
    bool cameraLoaded = false;
};

#endif //MINECRAFT_RENDERMASTER_H
