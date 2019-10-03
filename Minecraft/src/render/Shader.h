#ifndef MINECRAFT_SHADER_H
#define MINECRAFT_SHADER_H

#include <GL/glew.h>
#include <glm.hpp>

class Shader
{
public:
    Shader(const char *vertex_path, const char *fragment_path);
    ~Shader();

    void Enable() const;
    void Disable() const;

    GLuint GetProgramID() const;
    GLint UniformLocation(const char *uniform) const;
private:
    GLuint program;
    const char *vertexPath, *fragmentPath;

    GLuint Load();
};

#endif //MINECRAFT_SHADER_H
