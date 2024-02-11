#include "scene_graph_viewport.h"

#include "common/engine.h"
#include "imgui.h"


void SceneGraphViewport::Draw()
{
  ImGui::Begin(name_.c_str());
  ImGui::Button("Button 1");
  ImGui::Button("Button 2");
  ImGui::Button("Button 3");
  ImGui::End();
}