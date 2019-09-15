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

    void Uniform1f(const char *uniform, glm::vec1 val) const;
    void Uniform2f(const char *uniform, glm::vec2 val) const;
    void Uniform3f(const char *uniform, glm::vec3 val) const;
    void Uniform4f(const char *uniform, glm::vec4 val) const;
    void UniformMat4(const char *uniform, glm::mat4 matrix) const;

    GLuint GetProgramID() const;
private:
    // Members
    GLuint program;
    const char *vertexPath, *fragmentPath;

    // Methods
    GLuint Load();
    GLint UniformLocation(const char *uniform) const;
};

#endif //MINECRAFT_SHADER_H
