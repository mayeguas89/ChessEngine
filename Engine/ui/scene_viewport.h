#pragma once

#include "ui.h"
#include "viewport.h"

#include <unordered_map>

class SceneViewport: public Viewport
{
public:
  SceneViewport(const std::string& name): Viewport{name} {}

  virtual void Draw() override;

  size_t RegisterMouseClickCallback(MouseClickOnSceneViewportCallbacks function) override
  {
    mouseClickOnSceneViewportcallbacks_.insert({mouseClickOnSceneViewportcallbacks_.size(), function});
    return mouseClickOnSceneViewportcallbacks_.size();
  }
  void UnregisterMouseClickCallback(const size_t index) override
  {
    if (auto it = mouseClickOnSceneViewportcallbacks_.find(index); it != mouseClickOnSceneViewportcallbacks_.end())
      mouseClickOnSceneViewportcallbacks_.erase(it);
  }

private:
  std::unordered_map<size_t, MouseClickOnSceneViewportCallbacks> mouseClickOnSceneViewportcallbacks_;
};