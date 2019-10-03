#include "Shader.h"
#include "../utils/FileUtils.h"

#include <string>
#include <vector>
#include <iostream>

#include <gtc/type_ptr.hpp>

Shader::Shader(const char *vertex_path, const char *fragment_path)
        : vertexPath(vertex_path), fragmentPath(fragment_path)
{ program = Load(); }

Shader::~Shader()
{ glDeleteProgram(program); }

GLuint Shader::Load()
{
    GLuint program = glCreateProgram();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vertexsrc = FileUtils::ReadFile(vertexPath);
    const char *fragmentsrc = FileUtils::ReadFile(fragmentPath);

    GLint result = GL_NO_ERROR;

    // Vertex shader
    glShaderSource(vertex, 1, &vertexsrc, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> error(length);

        glGetShaderInfoLog(vertex, length, &length, &error[0]);
        std::cerr << "In \"" << vertexPath << "\":" << std::endl << &error[0] << std::endl;

        glDeleteProgram(program);
        goto out;
    }

    // Fragment shader
    glShaderSource(fragment, 1, &fragmentsrc, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> error(length);

        glGetShaderInfoLog(fragment, length, &length, &error[0]);
        std::cerr << "In \"" << fragmentPath << "\":" << std::endl << &error[0] << std::endl;

        glDeleteProgram(program);
        goto out;
    }

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glValidateProgram(program);

    out:
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

void Shader::Enable() const
{ glUseProgram(program); }

void Shader::Disable() const
{ glUseProgram(0); }

GLuint Shader::GetProgramID() const
{ return program; }

GLint Shader::UniformLocation(const char *uniform) const
{ return glGetUniformLocation(program, (const GLchar *) uniform); }
