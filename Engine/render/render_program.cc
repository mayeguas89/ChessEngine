#include "render_program.h"

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "spdlog/spdlog.h"

#include <fstream>
#include <iosfwd>
#include <iostream>

namespace
{
char* ReadShader(const std::string& filename)
{
  char* source = nullptr;
  std::streampos size;
  std::ifstream file{filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate};
  if (!file.is_open())
  {
    spdlog::error("Unable to open file {}", filename);
  }
  else
  {
    size = file.tellg();
    source = new char[size + std::streampos(1)];
    file.seekg(0, std::ios::beg);
    file.read(source, size);
    source[size] = '\0';
    file.close();
  }
  return source;
}

unsigned int CompileShader(const char* shaderSource, const ShaderType shaderType)
{
  // Tipo de shader
  unsigned int type{0};
  switch (shaderType)
  {
    case ShaderType::Vertex:
    {
      type = GL_VERTEX_SHADER;
      break;
    }
    case ShaderType::Fragment:
    {
      type = GL_FRAGMENT_SHADER;
      break;
    }
  }

  auto shaderId = glCreateShader(type);
  glShaderSource(shaderId, 1, &shaderSource, nullptr);
  glCompileShader(shaderId);
  return shaderId;
}

void CheckCompilationErrors(const unsigned int shaderId, std::string& errorLog)
{
  static const int BUFFER_SIZE = 1024;
  GLint succes;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &succes);
  if (succes)
    return;
  GLsizei logLength = 0;
  errorLog.resize(BUFFER_SIZE);
  glGetShaderInfoLog(shaderId, BUFFER_SIZE, &logLength, errorLog.data());
}

void CheckLinkErrors(const unsigned int programId, std::string& errorLog)
{
  static const int BUFFER_SIZE = 1024;
  GLint succes;
  glGetProgramiv(programId, GL_LINK_STATUS, &succes);
  if (succes)
    return;
  GLsizei logLength = 0;
  errorLog.resize(BUFFER_SIZE);
  glGetProgramInfoLog(programId, BUFFER_SIZE, &logLength, errorLog.data());
}

void SetupVariableList(const unsigned int programId, std::unordered_map<std::string, int>& variableMap)
{
  static const unsigned int MAX_VARIABLE_NAME_SIZE = 100;

  GLsizei length;
  GLint size;
  GLenum type;
  std::string name;
  int numberOfVariables = 0;

  glUseProgram(programId);
  glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numberOfVariables);
  for (int i = 0; i < numberOfVariables; i++)
  {
    name.clear();
    name.reserve(MAX_VARIABLE_NAME_SIZE);
    glGetActiveAttrib(programId, (GLuint)i, MAX_VARIABLE_NAME_SIZE, &length, &size, &type, name.data());
    auto location = glGetAttribLocation(programId, name.c_str());
    variableMap.insert({std::string{name.data(), name.data() + length}, location});
  }
  glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numberOfVariables);
  for (int i = 0; i < numberOfVariables; i++)
  {
    name.clear();
    name.reserve(MAX_VARIABLE_NAME_SIZE);
    glGetActiveUniform(programId, (GLuint)i, MAX_VARIABLE_NAME_SIZE, &length, &size, &type, name.data());
    auto location = glGetUniformLocation(programId, name.c_str());
    variableMap.insert({std::string{name.data(), name.data() + length}, location});
  }
}
}

RenderProgram::RenderProgram(): programId_{glCreateProgram()} {}

/**
 * @brief Add a shader to the program shader list. It creates its id, compiles it and add it if no error is show
 * 
 * @param filename 
 * @param shaderType 
 * @return true if the shader id is added to the list
 * @return false if the shader type is not supported, the filename could not be read or the shader source code has compilation errors.
 */
bool RenderProgram::AddShader(const std::string& filename, const ShaderType shaderType)
{
  // Check Type
  if (shaderType != ShaderType::Vertex && shaderType != ShaderType::Fragment)
    return false;

  // Read shader file
  auto shaderSource = ReadShader(filename);
  if (!shaderSource)
    return false;

  // Compile source code
  auto shaderId = CompileShader(shaderSource, shaderType);
  if (shaderId == 0)
    return false;

  CheckCompilationErrors(shaderId, errorLog);
  if (!errorLog.empty())
    return false;

  // Add the identifier to the list
  shadersId_.push_back(shaderId);
  return true;
}

/**
 * @brief Attach all the compiled shaders and link them all together in the program
 * 
 * @return true 
 * @return false if the linking process returns errors
 */
bool RenderProgram::Link()
{
  glUseProgram(programId_);
  std::for_each(shadersId_.begin(),
                shadersId_.end(),
                [this](const auto shaderId) { glAttachShader(programId_, shaderId); });
  glLinkProgram(programId_);
  CheckLinkErrors(programId_, errorLog);
  if (!errorLog.empty())
    return false;

  // Fulfill the variable map with its location
  SetupVariableList(programId_, variableMap_);
  return true;
}

void RenderProgram::Use()
{
  glUseProgram(programId_);
}

void RenderProgram::SetVariables() {}

void RenderProgram::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
  if (auto it = variableMap_.find(name); it != variableMap_.end())
  {
    glUniformMatrix4fv(it->second, 1, GL_FALSE, &matrix[0][0]);
  }
}
void RenderProgram::SetBool(const std::string& name, const bool value)
{
  if (auto it = variableMap_.find(name); it != variableMap_.end())
  {
    glUniform1i(it->second, value);
  }
}

void RenderProgram::SetUniformVec3(const std::string& name, const glm::vec3& vector)
{
  if (auto it = variableMap_.find(name); it != variableMap_.end())
  {
    glUniform3fv(it->second, 1, &vector[0]);
  }
}

void RenderProgram::SetVertextAttribArray(const std::string& name,
                                          const int size,
                                          const unsigned int type,
                                          const int stride,
                                          const void* pointer)
{
  if (auto it = variableMap_.find(name); it != variableMap_.end())
  {
    glEnableVertexAttribArray(it->second);
    glVertexAttribPointer(it->second, size, type, GL_FALSE, stride, pointer);
  }
}