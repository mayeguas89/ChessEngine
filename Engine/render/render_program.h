#pragma once

#include "glm/glm.hpp"

#include <string>
#include <unordered_map>
#include <vector>

enum class ShaderType
{
  None = -1,
  Vertex,
  Fragment
};

class RenderProgram
{
public:
  RenderProgram();
  ~RenderProgram() = default;

  bool AddShader(const std::string& filename, const ShaderType shaderType);
  bool Link();
  void Use();

  void SetVariables();
  void SetBool(const std::string& name, const bool value);
  void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
  void SetUniformVec3(const std::string& name, const glm::vec3& vector);
  void SetVertextAttribArray(const std::string& name,
                             const int size,
                             const unsigned int type,
                             const int stride,
                             const void* pointer);

  std::string errorLog;

private:
  unsigned int programId_;
  std::vector<unsigned int> shadersId_;
  std::unordered_map<std::string, int> variableMap_;
};