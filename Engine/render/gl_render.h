#pragma once

#include "utils/singleton.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <memory>
#include <string>

class GLRender: public Singleton<GLRender>
{
public:
  void Init();
  void Draw();
  void End();
  void Clear();
  void SwapBuffers();
  bool CheckWindowShouldClose();

  glm::vec<2, int> GetWindowSize() const
  {
    return windowSize_;
  }

  void SetWindowSize(const glm::vec<2, int>& windowSize)
  {
    windowSize_ = windowSize;
  }

  void SetWindowTitle(const std::string& windowTitle)
  {
    windowTitle_ = windowTitle;
  }

  std::string GetWindowTitle() const
  {
    return windowTitle_;
  }

  GLFWwindow* GetWindow() const
  {
    return window_.get();
  }

  float GetAspectRatio() const
  {
    return static_cast<float>(windowSize_.x / static_cast<float>(windowSize_.y));
  }

private:
  struct GLFWwindowDeleter
  {
    void operator()(GLFWwindow* window)
    {
      glfwDestroyWindow(window);
    }
  };

  void CreateWindow();
  void WindowResizeCallback(int width, int height);

  std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window_;
  glm::vec4 clearColor_{0.2, 0.2, 0.2, 1.0f};
  glm::vec<2, int> windowSize_;
  std::string windowTitle_;
};