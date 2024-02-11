#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "render/gl_render.h"

void Camera::Update() {}

glm::mat4 Camera::GetViewMatrix() const
{
  return glm::lookAt(glm::vec3(position_), lookAt_, up_);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
  // return glm::perspective(glm::radians(fov_), GLRender::Instance().GetAspectRatio(), nearPlane_, farPlane_);
  return glm::ortho(-ortoghraphicScale_,
                    ortoghraphicScale_,
                    -ortoghraphicScale_,
                    ortoghraphicScale_,
                    nearPlane_,
                    farPlane_);
}
