#pragma once
#include "glm/glm.hpp"

#include <string>

class GLTexture
{
public:
  GLTexture() = default;
  ~GLTexture();

  bool LoadFromFile(const std::string& filename);

  unsigned int GetTextureId() const
  {
    return id_;
  }

  glm::vec<2, int> GetTextureSize() const
  {
    return size_;
  }

  void Bind(int index = 0);

private:
  unsigned int id_{0};
  glm::vec<2, int> size_;
};