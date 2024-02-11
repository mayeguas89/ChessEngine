#include "properties_viewport.h"

#include "common/engine.h"
#include "imgui.h"

void PropertiesViewport::Draw()
{
  ImGui::Begin(name_.c_str());
  auto& camera = Engine::Instance().GetMainCamera();
  {
    ImGui::SeparatorText("Camera");
    static float fPosition[3];
    static float flookAt[3];
    static float fScale[3];
    const auto& position = camera.GetPosition();
    fPosition[0] = position.x;
    fPosition[1] = position.y;
    fPosition[2] = position.z;
    const auto& lookAt = camera.GetLookAt();
    flookAt[0] = lookAt.x;
    flookAt[1] = lookAt.y;
    flookAt[2] = lookAt.z;
    if (ImGui::InputFloat3("Cam. Position", fPosition))
      camera.SetPosition({fPosition[0], fPosition[1], fPosition[2]});
    if (ImGui::InputFloat3("LookAt", flookAt))
      camera.SetLookAt({flookAt[0], flookAt[1], flookAt[2]});
    ImGui::Text("Zoom");
    ImGui::SameLine();
    if (ImGui::Button("+"))
      camera.ZoomIn();
    ImGui::SameLine();
    if (ImGui::Button("-"))
      camera.ZoomOut();
  }
  ImGui::End();
}