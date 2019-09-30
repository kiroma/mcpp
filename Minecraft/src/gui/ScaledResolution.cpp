#include "ScaledResolution.h"

#include <ext/matrix_clip_space.hpp>

ScaledResolution::ScaledResolution(int width, int height)
{ RealResize(width, height); }

void ScaledResolution::RealResize(int width, int height)
{
    this->width = width;
    this->height = height;
}

void ScaledResolution::SetScaleFactor(int factor)
{ this->factor = factor; }

int ScaledResolution::GetScaleFactor()
{ return factor; }

int ScaledResolution::GetScaledWidth()
{ return width / factor; }

int ScaledResolution::GetScaledHeight()
{ return height / factor; }

glm::mat4 ScaledResolution::GenerateMatrix()
{ return glm::ortho(0.0f, (float) GetScaledWidth(), (float) GetScaledHeight(), 0.0f); }
