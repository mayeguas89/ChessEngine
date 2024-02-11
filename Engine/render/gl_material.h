#pragma once

#include "gl_texture.h"
#include "render_program.h"

#include <vector>

class GLMaterial
{
public:
private:
  std::vector<RenderProgram> renderPrograms_;
};