#ifndef MINECRAFT_TEXTURE_H
#define MINECRAFT_TEXTURE_H

class Texture {
public:
    Texture(const char *filename);
    ~Texture();

    unsigned int GetID() const { return id; }
private:
    int width, height, channels;
    unsigned int id;
    unsigned char *data;
};

#endif //MINECRAFT_TEXTURE_H
