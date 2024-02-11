#include "gl_texture.h"

#include "glad/gl.h"

#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "spdlog/spdlog.h"
#include "stb_image.h"

GLTexture::~GLTexture()
{
  if (id_ != 0)
    glDeleteTextures(1, &id_);
}

bool GLTexture::LoadFromFile(const std::string& filename)
{
  int width, height, components;
  if (!std::filesystem::exists(filename))
  {
    spdlog::error("Unable to find file {} to create a texture", filename);
    return false;
  }

  unsigned char* pixels = stbi_load(filename.c_str(), &width, &height, &components, 4);

  if (pixels == nullptr)
  {
    spdlog::error("The file {} does not contain any pixel", filename);
    return false;
  }

  spdlog::info("File read {}: generating texture of {}x{}", filename, width, height);
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glGenerateMipmap(GL_TEXTURE_2D);

  size_ = {width, height};

  stbi_image_free(pixels);

  return true;
}

void GLTexture::Bind(int index)
{
  if (id_ == 0)
  {
    throw std::runtime_error("Unable to bind texture. Is not initialized");
  }
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, id_);
}
