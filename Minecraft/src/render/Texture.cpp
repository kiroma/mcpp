#include "Texture.h"

#include <GL/glew.h>
#include <iostream>

#include <SFML/Graphics/Image.hpp>

Texture::Texture(const char *filename)
{
    // Load the image
    const sf::Image img = [&](){
        sf::Image temp;
        if (!temp.loadFromFile(filename))
            throw std::runtime_error("Failed to load texture from \"" + std::string(filename) + "\".");

        return temp;
    }();
    
    width = img.getSize().x;
    height = img.getSize().y;

    // Generate the texture
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Assign some parameters to it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load the data into the texture buffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    glDisable(GL_TEXTURE_2D);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, 0);
}
