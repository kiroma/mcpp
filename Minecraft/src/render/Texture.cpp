#include "Texture.h"

#include <GL/glew.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

Texture::Texture(const char *filename)
{
    // Load the image
    data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture from \"" << filename << "\"." << std::endl;
        return;
    }

    // Generate the texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Assign some parameters to it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load the data into the texture buffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    delete[] data;
    glDeleteTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, 0);
}
