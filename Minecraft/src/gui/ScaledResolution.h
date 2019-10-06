#ifndef MINECRAFT_SCALEDRESOLUTION_H
#define MINECRAFT_SCALEDRESOLUTION_H

#include <glm/mat4x4.hpp>

class ScaledResolution
{
public:
    ScaledResolution()
    {};
    ScaledResolution(int width, int height);

    void RealResize(int width, int height);
    void SetScaleFactor(int factor);
    int GetScaleFactor();
    int GetScaledWidth();
    int GetScaledHeight();
    glm::mat4 GenerateMatrix();

private:
    int width, height;
    int factor;
};

#endif //MINECRAFT_SCALEDRESOLUTION_H
