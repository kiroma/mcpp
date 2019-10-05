#ifndef MINECRAFT_SHADER_H
#define MINECRAFT_SHADER_H

#include "ShaderType.h"

#include <string>

class Shader
{
public:
    Shader(ShaderType type, std::string path);
    ~Shader();

    ShaderType GetType()
    { return type; }
    unsigned int GetID()
    { return id; }

private:
    ShaderType type;
    unsigned int id;
};

#endif //MINECRAFT_SHADER_H
