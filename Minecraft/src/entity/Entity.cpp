#include "Entity.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

Entity::Entity(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        : position(position), rotation(rotation), scale(scale)
{}

Entity::~Entity()
{}

void Entity::Translate(bool condition, const glm::vec3 val)
{ if (condition) Translate(val); }

void Entity::Rotate(bool condition, const glm::vec3 val)
{ if (condition) Rotate(val); }

void Entity::Scale(bool condition, const glm::vec3 val)
{ if (condition) Scale(val); }

glm::mat4 Entity::GenerateModelMatrix() const
{
    glm::mat4 ret(1.0f);
    ret = glm::rotate(ret, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    ret = glm::rotate(ret, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    ret = glm::rotate(ret, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    ret = glm::scale(ret, scale);
    ret = glm::translate(ret, position);

    return ret;
}
