#ifndef MINECRAFT_ENTITY_H
#define MINECRAFT_ENTITY_H

#include <vec3.hpp>
#include <mat4x4.hpp>
#include <glm.hpp>

class Entity
{
public:
    Entity(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
    virtual ~Entity();

    virtual void Update() = 0;
    glm::mat4 GenerateModelMatrix() const;

    void Translate(const glm::vec3 val)
    { position += val; }
    void Rotate(const glm::vec3 val)
    { rotation += val; }
    void Scale(const glm::vec3 val)
    { scale += val; }

    void Translate(bool condition, const glm::vec3 val);
    void Rotate(bool condition, const glm::vec3 val);
    void Scale(bool condition, const glm::vec3 val);

    void SetPosition(const glm::vec3 val)
    { position = val; }
    void SetRotation(const glm::vec3 val)
    { rotation = val; }
    void SetScale(const glm::vec3 val)
    { scale = val; }

    glm::vec3 GetPosition() const
    { return position; }
    glm::vec3 GetRotation() const
    { return rotation; }
    glm::vec3 GetScale() const
    { return scale; }

    glm::vec3 Right() const
    { return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotation))); }
    glm::vec3 Up() const
    { return glm::normalize(glm::cross(glm::radians(rotation), Right())); }

protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif //MINECRAFT_ENTITY_H
