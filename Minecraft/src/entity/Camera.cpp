#include "Camera.h"
#include "../Minecraft.h"
#include "../utils/math/DirectionVectors.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define MOVEMENT_SPEED ((float)5.0)
#define RUN_MULTIPLIER ((float)5.0)
#define MOUSE_SENSITIVITY ((float)0.1)

Camera::Camera(glm::vec3 position)
        : Entity(position)
{
    std::cout << "Camera instantiated" << std::endl;
}

void Camera::Update()
{
    Minecraft &minecraft = Minecraft::GetInstance();

    if (minecraft.HasFocus()) {
        // Translations
        DirectionVectors vectors;
        const float speed = MOVEMENT_SPEED * (minecraft.GetInput(sf::Keyboard::Key::LControl) ? RUN_MULTIPLIER : 1) *
                            minecraft.GetDeltaTime();
        const glm::vec3 forward = glm::vec3(glm::sin(glm::radians(rotation.y)), 0,
                                            -glm::cos(glm::radians(rotation.y))) * speed;
        const glm::vec3 right = glm::cross(glm::normalize(forward), glm::normalize((glm::vec3) vectors.Top())) * speed;

        // Forward & backward
        Translate(minecraft.GetInput(sf::Keyboard::Key::W), forward);
        Translate(minecraft.GetInput(sf::Keyboard::Key::S), -forward);

        // Left & right
        Translate(minecraft.GetInput(sf::Keyboard::Key::A), -right);
        Translate(minecraft.GetInput(sf::Keyboard::Key::D), right);

        // Up & down
        Translate(minecraft.GetInput(sf::Keyboard::Key::Space), glm::vec3(0, speed, 0));
        Translate(minecraft.GetInput(sf::Keyboard::Key::LShift), glm::vec3(0, -speed, 0));

        // Mouse look
        glm::vec2 rot = minecraft.GetMousePosition() - lastMousePos;
        rotation += glm::vec3(rot.y, rot.x, 0) * MOUSE_SENSITIVITY;
        lastMousePos = minecraft.GetMousePosition();
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
