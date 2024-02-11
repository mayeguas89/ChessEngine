#pragma once

#include "glm/glm.hpp"

#include <string>

class Entity
{
public:
  Entity(const std::string& name = ""): name_{name}
  {
    id_++;
  }

  virtual void Update();

  glm::mat4 GetModelMatrix() const
  {
    return modelMatrix_;
  }

  glm::vec4 GetPosition() const
  {
    return position_;
  }

  glm::vec4 GetRotation() const
  {
    return rotation_;
  }

  glm::vec4 GetScale() const
  {
    return scale_;
  }

  void SetPosition(const glm::vec4& position)
  {
    position_ = position;
  }

  void SetScale(const glm::vec4& scale)
  {
    scale_ = scale;
  }

  void SetRotation(const glm::vec4& rotation)
  {
    rotation_ = rotation;
  }

  virtual void Draw() = 0;

  std::string GetName() const
  {
    return name_;
  }

  bool active_{false};

protected:
  void ComputeModelMatrix();
  inline static unsigned int id_;
  std::string name_;

  glm::vec4 position_{0.f, 0.f, 0.f, 0.f};
  glm::vec4 rotation_{0.f, 0.f, 0.f, 0.f};
  glm::vec4 scale_{1.f, 1.f, 1.f, 0.f};

  glm::mat4 modelMatrix_;
};
