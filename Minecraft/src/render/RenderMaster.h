#ifndef MINECRAFT_RENDERMASTER_H
#define MINECRAFT_RENDERMASTER_H

class Shader;
class Camera;
class Texture;
class TextureAtlas;
class World;

#include <mat4x4.hpp>

#define MC_RENDER_PASSES 2

class RenderMaster
{
public:
    RenderMaster();
    ~RenderMaster();

    void RenderWorld(World &world);
    void SubmitProjection(const glm::mat4 projection) const;
    void LoadCamera(Camera *camera);

    const Shader &GetCurrentShader() const
    { return *shaderSolidBlock; }
    const TextureAtlas &GetTextureAtlas() const
    { return *textureSolidBlocks; }
    const Camera &GetViewCamera() const
    { return *camera; }
    bool IsCameraAvailable() const
    { return cameraLoaded; }

private:
    Shader *shaderSolidBlock;
    TextureAtlas *textureSolidBlocks;
    Camera *camera;
    bool cameraLoaded = false;

    // World rendering routines
    void RenderChunks(const World &world);
    void RenderGUI();
};

#endif //MINECRAFT_RENDERMASTER_H
