#ifndef MINECRAFT_SHADERPROGRAM_H
#define MINECRAFT_SHADERPROGRAM_H

#include "Shader.h"

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void Build();
    void AttachShader(std::unique_ptr<Shader> shader)
    { shaders.push_back(std::move(shader)); }

    unsigned int GetProgramID() const
    { return id; }
    int UniformLocation(const char *uniform) const
    { return glGetUniformLocation(id, uniform); }
    void Enable() const
    { glUseProgram(id); }
    void Disable() const
    { glUseProgram(0); }

private:
    unsigned int id;
    std::vector<std::unique_ptr<Shader>> shaders;
};

#endif //MINECRAFT_SHADERPROGRAM_H
