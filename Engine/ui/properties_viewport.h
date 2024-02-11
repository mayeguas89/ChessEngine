#pragma once

#include "ui/viewport.h"

class PropertiesViewport: public Viewport
{
public:
  PropertiesViewport(const std::string& name): Viewport{name} {}

  virtual void Draw() override;

private:
};