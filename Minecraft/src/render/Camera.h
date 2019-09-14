#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include <glm.hpp>

class Camera {
public:
    Camera();

    void Update();
    glm::mat4 GenerateViewMatrix() const;
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec2 lastMousePos;
};

#endif //MINECRAFT_CAMERA_H
