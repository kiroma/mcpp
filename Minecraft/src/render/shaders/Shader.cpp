#include "Shader.h"
#include "../../utils/InputOutput.h"
#include "../../utils/exceptions/ReportedException.h"

#include <iostream>

Shader::Shader(ShaderType type, std::string path)
{
    this->type = type;
    this->id = glCreateShader((GLenum) type);
    int result = GL_NO_ERROR;

    std::vector<char> tsrc = InputOutput::ReadFile(path);
    const char *src = tsrc.data();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> error(length);

        glGetShaderInfoLog(id, length, &length, &error[0]);
        glDeleteShader(id);

        throw ReportedException(path,
                                "A GLSL error occurred. Please, check the source code of the shader to fix this issue.",
                                error.data());
    }
}

Shader::~Shader()
{ glDeleteShader(id); }
