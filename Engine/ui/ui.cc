#include "ui/ui.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "render/gl_render.h"
#include "ui.h"
#include "ui/properties_viewport.h"
#include "ui/scene_graph_viewport.h"
#include "ui/scene_viewport.h"
#include "ui/spritemap_window.h"
#include "ui/viewport.h"

#include <algorithm>
#include <string>

static const std::string SCENE_WINDOW_NAME = "Scene";
static const std::string SCENE_GRAPH_WINDOW_NAME = "Scene Graph";
static const std::string PROPERTIES_WINDOW_NAME = "Properties";
static const std::string SPRITEMAP_WINDOW_NAME = "SpriteMap";

namespace
{
static void StartFrame()
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

static void EndFrame()
{
  // Render dear imgui into screen
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGui::EndFrame();
}

static void CreateDockSpace()
{
  static bool opt_fullscreen = true;
  static bool opt_padding = false;
  static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode; //ImGuiDockNodeFlags_None
  static auto initialized = false;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
  // because it would be confusing to have two docking targets within each others.
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoMove;
  windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  ImGui::Begin("Docking", nullptr, windowFlags);
  ImGui::PopStyleVar(3);

  // Submit the DockSpace
  ImGuiID dockspaceId = ImGui::GetID("DockSpace");

  if (!initialized)
  {
    initialized = true;
    // Clear out existing layout
    ImGui::DockBuilderRemoveNode(dockspaceId);
    // Add empty node
    ImGui::DockBuilderAddNode(dockspaceId, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
    // Main node should cover entire window
    ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());
    // get id of main dock space area
    ImGuiID dockspaceMainId = dockspaceId;
    // Create a dock node for the right docked window
    auto properties = ImGui::DockBuilderSplitNode(dockspaceMainId, ImGuiDir_Right, 0.25f, NULL, &dockspaceMainId);
    auto sceneGraph = ImGui::DockBuilderSplitNode(dockspaceMainId, ImGuiDir_Left, 0.25f, NULL, &dockspaceMainId);
    ImGui::DockBuilderFinish(dockspaceId);

    ImGui::DockBuilderDockWindow(SCENE_WINDOW_NAME.c_str(), dockspaceMainId);

    ImGui::DockBuilderDockWindow(PROPERTIES_WINDOW_NAME.c_str(), properties);

    ImGui::DockBuilderDockWindow(SCENE_GRAPH_WINDOW_NAME.c_str(), sceneGraph);

    // Aditional Windows Will be showed only when requested
    // viewports.push_back(std::move(std::make_unique<SpriteMapWindow>(SPRITEMAP_WINDOW_NAME)));
  }
  ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
  ImGui::End();
}
}

void UI::Init()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  const char* glsl_version = "#version 130";

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(GLRender::Instance().GetWindow(), true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  viewports_.push_back(std::move(std::make_unique<SceneViewport>(SCENE_WINDOW_NAME)));
  viewports_.push_back(std::move(std::make_unique<PropertiesViewport>(PROPERTIES_WINDOW_NAME)));
  viewports_.push_back(std::move(std::make_unique<SceneGraphViewport>(SCENE_GRAPH_WINDOW_NAME)));
}

void UI::Update()
{
  StartFrame();
  CreateDockSpace();
  std::for_each(viewports_.begin(), viewports_.end(), [](auto& viewport) { viewport->Draw(); });
  EndFrame();
}

void UI::End()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

size_t UI::RegisterMouseClickOnSceneViewport(Viewport::MouseClickOnSceneViewportCallbacks function)
{
  return viewports_.at(0)->RegisterMouseClickCallback(function);
}

void UI::UnregisterMouseClickOnSceneViewport(const size_t index)
{
  viewports_.at(0)->UnregisterMouseClickCallback(index);
}
