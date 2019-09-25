#ifndef MINECRAFT_TEXTURE_H
#define MINECRAFT_TEXTURE_H

class Texture
{
public:
    Texture(const char *filename);
    ~Texture();

    unsigned int GetID() const
    { return id; }
    int GetWidth() const
    { return width; }
    int GetHeight() const
    { return height; }
    int GetSize() const
    { return width > height ? width : height; }
    int GetChannelCount() const
    { return channels; }
private:
    int width, height, channels;
    unsigned int id;
    unsigned char *data;
};

#endif //MINECRAFT_TEXTURE_H
