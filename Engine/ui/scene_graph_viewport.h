#pragma once

#include "ui/viewport.h"

class SceneGraphViewport: public Viewport
{
public:
  SceneGraphViewport(const std::string& name): Viewport{name} {}

  virtual void Draw() override;

private:
};