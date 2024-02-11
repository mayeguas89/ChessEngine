#pragma once
#include "glm/glm.hpp"
#include "spdlog/spdlog.h"

class Camera
{
public:
  Camera() = default;
  ~Camera() = default;

  void Update();
  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetProjectionMatrix() const;

  void SetFov(const float fov)
  {
    fov_ = fov;
  }

  void SetLookAt(const glm::vec3& lookAt)
  {
    lookAt_ = lookAt;
  }

  void SetPosition(const glm::vec3& position)
  {
    position_ = position;
  }

  void ZoomOut()
  {
    ortoghraphicScale_ += MOVEMENT_RATE;
    spdlog::info("ZoomOut ortographicScale: {}", ortoghraphicScale_);
  }

  void ZoomIn()
  {
    ortoghraphicScale_ = std::max(ortoghraphicScale_ - MOVEMENT_RATE, MIN_ORTHO_SCALE);
    spdlog::info("ZoomIn ortographicScale: {}", ortoghraphicScale_);
  }

  const glm::vec3& GetPosition() const
  {
    return position_;
  }

  const glm::vec3& GetLookAt() const
  {
    return lookAt_;
  }

private:
  inline static const float MIN_ORTHO_SCALE = 1.5f;
  inline static const float MOVEMENT_RATE = .5f;
  inline static float ortoghraphicScale_{10.5};
  glm::vec3 position_;
  glm::vec3 lookAt_;
  glm::vec3 up_{0.f, 1.f, 0.f};
  float fov_{90.f};
  float nearPlane_{0.1f};
  float farPlane_{100.f};
};