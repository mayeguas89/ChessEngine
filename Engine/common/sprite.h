#pragma once

#include "common/entity.h"
#include "glm/glm.hpp"
#include "render/gl_texture.h"
#include "render/render_program.h"

#include <array>
#include <memory>

class Sprite2D: public Entity
{
public:
  Sprite2D(const std::string& textureFile,
           const glm::vec<2, int>& spriteSize = {0, 0},
           const glm::vec<2, int>& spriteView = {0, 0},
           const std::string& fragmentShader = "Assets/shaders/sprite.fragment",
           const std::string& vertexShader = "Assets/shaders/sprite.vertex");
  ~Sprite2D();
  virtual void Draw() override;
  virtual void Update() override
  {
    Entity::Update();
  }

  bool active_{false};

  const glm::vec<2, int>& GetTextureSize() const
  {
    return texture_->GetTextureSize();
  }

protected:
  struct BufferObject
  {
    unsigned int id, VBO, VIO;
  };
  BufferObject bufferObject_;
  RenderProgram program_;
  std::shared_ptr<GLTexture> texture_;
  std::array<glm::vec4, 4> vertexPosition_{glm::vec4{-0.5f, -0.5f, 0.f, 1.f},
                                           glm::vec4{-0.5f, 0.5f, 0.f, 1.f},
                                           glm::vec4{0.5f, 0.5f, 0.f, 1.f},
                                           glm::vec4{0.5f, -0.5f, 0.f, 1.f}};
  std::array<glm::vec2, 4> texCoord_;
  std::array<uint32_t, 6> indices_{0, 1, 2, 2, 3, 0};
};