#include "gl_render.h"

#include "common/engine.h"
#include "glad/gl.h"
#include "spdlog/spdlog.h"
#include "ui/ui.h"

namespace
{
static void GlfwErrorCallback(int error, const char* description)
{
  spdlog::error("GLFW Error {}: {}\n", error, description);
}

static void WindowResizeCallback(GLFWwindow* window, int width, int height) {}
}

void GLRender::Init()
{
  glfwSetErrorCallback(GlfwErrorCallback);
  if (!glfwInit())
    throw std::runtime_error("Error initializing glfw");

  spdlog::info("glfw initialized");

  CreateWindow();

  glfwSetWindowUserPointer(window_.get(), this);

  glfwMakeContextCurrent(window_.get());
  glfwSwapInterval(1);
  gladLoadGL(glfwGetProcAddress);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  auto resizefunction = [](GLFWwindow* window, int width, int height)
  {
    auto render = static_cast<GLRender*>(glfwGetWindowUserPointer(window));
    if (!render)
      throw std::runtime_error("Error getting render pointer from glfw window");
    render->WindowResizeCallback(width, height);
  };

  glfwSetWindowSizeCallback(window_.get(), resizefunction);

  UI::Instance().Init();
}

void GLRender::Draw()
{
  // Render Agains texture
  glBindFramebuffer(GL_FRAMEBUFFER, Engine::Instance().frameBufferId_);
  glViewport(0, 0, windowSize_.x, windowSize_.y);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  unsigned int draw_buf = GL_COLOR_ATTACHMENT0;
  glDrawBuffers(1, &draw_buf);

  // Draw();
  const auto& entities = Engine::Instance().GetEntities();
  std::for_each(entities.begin(), entities.end(), [](auto& entity) { entity->Draw(); });
  const auto& playerOne = Engine::Instance().GetPlayerOne();
  const auto& playerTwo = Engine::Instance().GetPlayerTwo();
  std::for_each(playerOne.pieces_.begin(), playerOne.pieces_.end(), [](auto& entity) { entity->Draw(); });
  std::for_each(playerTwo.pieces_.begin(), playerTwo.pieces_.end(), [](auto& entity) { entity->Draw(); });

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, windowSize_.x, windowSize_.y);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw_buf = GL_COLOR_ATTACHMENT0;
  glDrawBuffers(1, &draw_buf);

  UI::Instance().Update();
}

void GLRender::End()
{
  window_.reset();
  glfwTerminate();
}

void GLRender::Clear()
{
  glClearColor(clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a);
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
}

void GLRender::SwapBuffers()
{
  glfwSwapBuffers(window_.get());
}

bool GLRender::CheckWindowShouldClose()
{
  return glfwWindowShouldClose(window_.get());
}

void GLRender::CreateWindow()
{
  auto window = glfwCreateWindow(windowSize_.x, windowSize_.y, windowTitle_.c_str(), nullptr, nullptr);
  if (!window)
    throw std::runtime_error("Error creating window");

  spdlog::info("Window {}, {} created", windowSize_.x, windowSize_.y);

  window_.reset(window);
}

void GLRender::WindowResizeCallback(int width, int height)
{
  windowSize_ = {width, height};
  glViewport(0, 0, width, height);
  glfwSetWindowSize(window_.get(), width, height);
  spdlog::info("Window resized to {}, {}", width, height);
  Engine::Instance().ResizeFrameBuffer(windowSize_);
}
