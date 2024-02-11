#include "sprite.h"

#include "common/engine.h"
#include "glad/gl.h"
#include "spdlog/spdlog.h"

#include <array>

Sprite2D::Sprite2D(const std::string& textureFile,
                   const glm::vec<2, int>& spriteSize,
                   const glm::vec<2, int>& spriteView,
                   const std::string& fragmentShader,
                   const std::string& vertexShader):
  Entity()
{
  texture_ = Engine::Instance().GetTexture(textureFile);
  auto compiledOne = program_.AddShader(vertexShader, ShaderType::Vertex);
  auto compiledTwo = program_.AddShader(fragmentShader, ShaderType::Fragment);
  if (not compiledOne || not compiledTwo)
  {
    spdlog::error("Error in compile shaders!");
    throw std::runtime_error(program_.errorLog);
  }
  auto linked = program_.Link();
  if (not linked)
  {
    spdlog::error("Error in linking shaders!");
    throw std::runtime_error(program_.errorLog);
  }

  // To show part of the texture we have to show only the appropiate texCoordinates
  // Example: character made of 56x56 px
  // Full texture
  auto texSize = texture_->GetTextureSize();
  if (spriteSize.x != 0 && spriteSize.y != 0)
  {
    auto x = (1.f / (float)texSize.x) * spriteSize.x;
    auto y = (1.f / (float)texSize.y) * spriteSize.y;
    texCoord_ = {glm::vec2{spriteView.x * x, spriteView.y * y},
                 glm::vec2{spriteView.x * x, spriteView.y * y + y},
                 glm::vec2{spriteView.x * x + x, spriteView.y * y + y},
                 glm::vec2{spriteView.x * x + x, spriteView.y * y}};
  }
  else
  {
    texCoord_ = {glm::vec2{0.f, 0.f}, glm::vec2{0.f, 1.0f}, glm::vec2{1.f, 1.f}, glm::vec2{1.f, 0.f}};
  }
  std::vector<Vertex> vertices(vertexPosition_.size());
  std::generate(vertices.begin(),
                vertices.end(),
                [this, n = 0]() mutable
                {
                  auto vertex = Vertex{vertexPosition_.at(n), glm::vec4{}, texCoord_.at(n)};
                  n++;
                  return vertex;
                });

  glGenVertexArrays(1, &bufferObject_.id);
  glGenBuffers(1, &bufferObject_.VBO);
  glBindBuffer(GL_ARRAY_BUFFER, bufferObject_.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &bufferObject_.VIO);
  glBindBuffer(GL_ARRAY_BUFFER, bufferObject_.VIO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * indices_.size(), indices_.data(), GL_STATIC_DRAW);
}

Sprite2D::~Sprite2D() {}

void Sprite2D::Draw()
{
  glBindVertexArray(bufferObject_.id);
  // Se hace el bind buffer y se suben los datos pertenecientes a este buffer
  glBindBuffer(GL_ARRAY_BUFFER, bufferObject_.VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject_.VIO);

  program_.Use();
  texture_->Bind();
  program_.SetVertextAttribArray("vertexPosition", 4, GL_FLOAT, sizeof(Vertex), (void*)0x00);
  program_.SetVertextAttribArray("texCoordinate", 2, GL_FLOAT, sizeof(Vertex), (void*)(2 * sizeof(glm::vec4)));
  program_.SetBool("active", active_);
  program_.SetUniformMat4("Model", GetModelMatrix());
  // Set Common stuff like camera matrix
  // program_.SetVariables();
  program_.SetUniformMat4("Projection", Engine::Instance().GetMainCamera().GetProjectionMatrix());
  program_.SetUniformMat4("View", Engine::Instance().GetMainCamera().GetViewMatrix());

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, nullptr);
}
