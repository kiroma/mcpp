#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include "Entity.h"

#include <glm/glm.hpp>

class Camera : public Entity
{
public:
    Camera(glm::vec3 position);
    ~Camera()
    {}

    void Update();
    glm::mat4 GenerateViewMatrix() const;
private:
    glm::vec2 lastMousePos;
};

#endif //MINECRAFT_CAMERA_H
