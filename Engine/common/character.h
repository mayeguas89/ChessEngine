#pragma once

#include "common/entity.h"
#include "render/gl_texture.h"

class Character: public Entity
{
public:
  Character();
  virtual void Update() override;

private:
  GLTexture texture;
};