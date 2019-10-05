#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
    id = glCreateProgram();
    shaders.clear();
}

ShaderProgram::~ShaderProgram()
{ glDeleteProgram(id); }

void ShaderProgram::Build()
{
    // Attach all shaders to our program
    for (auto it = shaders.begin(); it != shaders.end(); it++)
        glAttachShader(id, it->get()->GetID());

    // Link the program
    glLinkProgram(id);
    glValidateProgram(id);

    // Free memory
    shaders.clear();
    shaders.shrink_to_fit();
}
