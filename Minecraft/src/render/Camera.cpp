#include "Camera.h"
#include "../Minecraft.h"

#include <gtc/matrix_transform.hpp>

#define MOVEMENT_SPEED ((float)0.1)
#define MOUSE_SENSITIVITY ((float)0.1)

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 5.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
    if (Minecraft::GetInstance().HasFocus()) {
        if (Minecraft::GetInstance().GetInput(sf::Keyboard::Key::W)) {
            position += glm::vec3(0.0f, 0.0f, -MOVEMENT_SPEED);
        }

        if (Minecraft::GetInstance().GetInput(sf::Keyboard::Key::S)) {
            position += glm::vec3(0.0f, 0.0f, MOVEMENT_SPEED);
        }

        if (Minecraft::GetInstance().GetInput(sf::Keyboard::Key::A)) {
            position += glm::vec3(-MOVEMENT_SPEED, 0.0f, 0.0f);
        }

        if (Minecraft::GetInstance().GetInput(sf::Keyboard::Key::D)) {
            position += glm::vec3(MOVEMENT_SPEED, 0.0f, 0.0f);
        }

        if (Minecraft::GetInstance().GetInput(sf::Keyboard::Key::LShift)) {
            position += glm::vec3(0.0f, -MOVEMENT_SPEED, 0.0f);
        }

        if (Minecraft::GetInstance().GetInput(sf::Keyboard::Key::Space)) {
            position += glm::vec3(0.0f, MOVEMENT_SPEED, 0.0f);
        }

        glm::vec2 rot = Minecraft::GetInstance().GetMousePosition() - lastMousePos;
        rotation += glm::vec3(rot.y, rot.x, 0.0f) * MOUSE_SENSITIVITY;
        lastMousePos = Minecraft::GetInstance().GetMousePosition();
    }
}

glm::mat4 Camera::GenerateViewMatrix() const
{
    glm::mat4 ret(1.0f);
    ret = glm::rotate(ret, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    ret = glm::rotate(ret, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    ret = glm::rotate(ret, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    ret = glm::translate(ret, -position);

    return ret;
}
