#pragma once

#include "utils/singleton.h"
#include "viewport.h"

#include <memory>
#include <vector>

class UI: public Singleton<UI>
{
public:
  void Init();
  void Update();
  void End();
  size_t RegisterMouseClickOnSceneViewport(Viewport::MouseClickOnSceneViewportCallbacks function);
  void UnregisterMouseClickOnSceneViewport(const size_t index);

private:
  std::vector<std::unique_ptr<Viewport>> viewports_;
};