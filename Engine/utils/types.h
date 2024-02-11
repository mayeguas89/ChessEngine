#pragma once

#include "glm/glm.hpp"

enum class Color
{
  White = 0,
  Black
};

struct Vertex
{
  Vertex() = default;
  Vertex(const glm::vec4& p_position, const glm::vec4 p_color, const glm::vec2& p_texturePosition):
    position{p_position},
    texturePosition{p_texturePosition},
    color{p_color}
  {}
  glm::vec4 position;
  glm::vec4 color;
  glm::vec2 texturePosition;
};