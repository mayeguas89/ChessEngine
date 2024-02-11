#include "entity.h"

#include "glm/gtc/matrix_transform.hpp"

void Entity::Update()
{
  ComputeModelMatrix();
}

void Entity::ComputeModelMatrix()
{
  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));
  model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.f, 0.f));
  model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.f, 0.f));
  model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.f, 1.f));
  modelMatrix_ = glm::scale(model, glm::vec3(scale_));
}
