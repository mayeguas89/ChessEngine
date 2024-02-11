#pragma once
#include "glm/glm.hpp"

#include <functional>
#include <string>

class Viewport
{
public:
  typedef std::function<void(const glm::vec2&)> MouseClickOnSceneViewportCallbacks;
  Viewport(const std::string& name): name_{name} {}
  virtual ~Viewport() = default;

  virtual void Draw() = 0;

  std::string GetName() const
  {
    return name_;
  }
  virtual size_t RegisterMouseClickCallback(MouseClickOnSceneViewportCallbacks function)
  {
    return 0;
  }
  virtual void UnregisterMouseClickCallback(const size_t index) {}

protected:
  std::string name_;
};