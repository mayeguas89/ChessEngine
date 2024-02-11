#pragma once

#include "ui/viewport.h"

class SpriteMapWindow: public Viewport
{
public:
  SpriteMapWindow(const std::string& name): Viewport{name} {}
  virtual void Draw() override;

private:
};