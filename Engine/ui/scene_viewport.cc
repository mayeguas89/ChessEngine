#include "scene_viewport.h"

#include "common/engine.h"
#include "glm/ext/matrix_projection.hpp"
#include "glm/glm.hpp"
#include "imgui.h"
#include "render/gl_render.h"
#include "spdlog/spdlog.h"

void SceneViewport::Draw()
{
  ImGui::Begin(name_.c_str());
  ImVec2 viewportPanelSize = ImGui::GetWindowSize();
  ImGui::Image((void*)(intptr_t)Engine::Instance().textureId_, viewportPanelSize);

  if (ImGui::IsWindowFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
  {
    auto winPos = ImGui::GetWindowPos();
    auto camera = Engine::Instance().GetMainCamera();
    auto mousePos = ImGui::GetMousePos();
    mousePos.x -= winPos.x;
    mousePos.y -= winPos.y;
    spdlog::info("{},{}", mousePos.x, mousePos.y);
    const auto windowSize = ImGui::GetWindowSize();
    const auto depth = 0;
    glm::vec4 viewport = glm::vec4(0, 0, windowSize.x, windowSize.y);
    glm::vec3 wincoord = glm::vec3(mousePos.x, mousePos.y, depth);
    glm::vec3 worldCoord =
      glm::unProject(wincoord, camera.GetViewMatrix(), camera.GetProjectionMatrix(), viewport);

    spdlog::info("clicked on position {},{},{}", worldCoord.x, worldCoord.y, worldCoord.z);
    Engine::Instance().RayTest(glm::vec2{worldCoord});

    std::for_each(mouseClickOnSceneViewportcallbacks_.begin(),
                  mouseClickOnSceneViewportcallbacks_.end(),
                  [&worldCoord](auto& pair)
                  {
                    auto f = pair.second;
                    f({worldCoord.x, worldCoord.y});
                  });
  }
  ImGui::End();
}