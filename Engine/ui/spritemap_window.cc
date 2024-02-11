#include "spritemap_window.h"

#include "common/engine.h"
#include "imgui.h"
#include "spdlog/spdlog.h"

#include <chrono>
#include <vector>

void SpriteMapWindow::Draw()
{
  static auto texture = Engine::Instance().GetTexture("Assets/Characters/red/char_red_1.png");
  static int textureMapSize[2] = {0, 0};
  static int tileSize[2] = {56, 56};
  static int offset[2] = {0, 0};
  static std::vector<bool> itemClicked;

  glm::vec2 textureSize = texture->GetTextureSize();
  ImGui::Begin(name_.c_str());
  {
    ImVec2 viewportPanelSize = ImGui::GetWindowSize();
    bool changed = ImGui::InputInt2("Rows x Columns", textureMapSize);
    ImGui::InputInt2("Tile Size", tileSize);
    ImGui::InputInt2("Offset", offset);
    // Store cursor
    auto cursorPos = ImGui::GetCursorScreenPos();
    ImGui::Image((void*)(intptr_t)texture->GetTextureId(), {textureSize.x, textureSize.y});

    // Do Checks
    {
      textureMapSize[0] = std::max(textureMapSize[0], 0);
      textureMapSize[1] = std::max(textureMapSize[1], 0);
      tileSize[0] = std::max(tileSize[0], 0);
      tileSize[1] = std::max(tileSize[1], 0);
      tileSize[0] = std::min(tileSize[0], (int)textureSize.x);
      tileSize[1] = std::min(tileSize[1], (int)textureSize.y);
    }

    {
      if (changed && textureMapSize[0] > 0 && textureMapSize[1] > 0)
      {
        itemClicked.resize(textureMapSize[0] * textureMapSize[1], false);
        spdlog::info("Vector resized to {}", textureMapSize[0] * textureMapSize[1]);
      }
    }

    auto drawList = ImGui::GetWindowDrawList();
    // Draw
    {
      for (int row = 0; row < textureMapSize[0]; row++)
      {
        for (int col = 0; col < textureMapSize[1]; col++)
        {
          int index = row * textureMapSize[1] + col;

          ImVec2 pMin = {cursorPos.x + col * tileSize[1] + offset[0], cursorPos.y + row * tileSize[0] + offset[1]};
          ImVec2 pMax = {cursorPos.x + (col + 1) * tileSize[1] + offset[0],
                         cursorPos.y + (row + 1) * tileSize[0] + offset[1]};
          const bool modCtrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
          if ((modCtrl && ImGui::IsMouseHoveringRect(pMin, pMax) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
          {
            if (itemClicked.at(index))
            {
              spdlog::info("Item at {} was unselected", index);
              itemClicked.at(index) = false;
            }
            else
            {
              spdlog::info("Item at {} was selected", index);
              itemClicked.at(index) = true;
            }
          }

          if (itemClicked.at(index))
            drawList->AddRectFilled(pMin, pMax, IM_COL32(0, 255, 0, 100));
          else
            drawList->AddRect(pMin,
                              pMax,
                              (ImGui::IsMouseHoveringRect(pMin, pMax)) ? IM_COL32(255, 0, 0, 255) :
                                                                         IM_COL32(0, 255, 0, 255));
        }
      }
    }

    {
      static bool animationRun = false;
      if (auto it = std::find(itemClicked.begin(), itemClicked.end(), true); it != itemClicked.end())
      {
        static float timeBetweenFrames = 100; // ms
        static bool loop = true;
        auto frameCount = std::count(itemClicked.begin(), itemClicked.end(), true);
        spdlog::info("Create animation from {} frames", frameCount);
        ImGui::InputFloat("time Between Frames", &timeBetweenFrames);
        ImGui::Checkbox("Loop", &loop);
        auto firstIndex = std::distance(itemClicked.begin(), it);
        auto lastIndex = firstIndex + frameCount;
        // Divide index / numCols to get the animation row
        int activeRow = firstIndex / textureMapSize[1];
        int firstCol = firstIndex - activeRow * textureMapSize[1];
        spdlog::info("Animation starts at frame {} in row {}", firstCol, activeRow);

        if (ImGui::Button("Play Animation"))
          animationRun = true;
        if (ImGui::Button("Stop Animation"))
          animationRun = false;
        static std::vector<std::vector<ImVec2>> uvs;
        auto numCols = textureMapSize[1];
        auto colSize = tileSize[0];
        auto rowSize = tileSize[1];

        static bool firstTime{true};
        static float elapsedTime = 0.f;
        static int frameIndex = 0;

        if (animationRun)
        {
          if (uvs.empty())
          {
            uvs.resize(frameCount);
            auto xScale = (1.f / (float)textureSize.x) * colSize;
            auto yScale = (1.f / (float)textureSize.y) * rowSize;

            for (int i = firstCol; i < frameCount + firstCol; i++)
            {
              auto col = i;
              auto row = activeRow;
              if (col >= numCols)
              {
                col = i - numCols;
                row = row + 1;
              }
              float xIni = col * xScale;
              float yIni = row * yScale;
              float xEnd = (col + 1) * xScale;
              float yEnd = (row + 1) * yScale;
              uvs.at(i - firstCol).push_back({xIni, yIni});
              uvs.at(i - firstCol).push_back({xEnd, yEnd});
            }
          }
          static std::chrono::time_point<std::chrono::high_resolution_clock> newTime;
          static std::chrono::time_point<std::chrono::high_resolution_clock> oldTime;
          static float deltaTime;

          if (firstTime)
          {
            oldTime = std::chrono::high_resolution_clock::now();
            firstTime = false;
          }

          auto timeNow = std::chrono::high_resolution_clock::now();
          auto timeElapsed =
            std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(timeNow - oldTime);
          deltaTime = timeElapsed.count();
          oldTime = timeNow;

          elapsedTime += deltaTime;
          if (elapsedTime > timeBetweenFrames)
          {
            elapsedTime = std::min(elapsedTime - timeBetweenFrames, 0.f);
            frameIndex++;
            if (frameIndex >= frameCount)
              frameIndex = 0;
          }

          ImGui::Image((void*)(intptr_t)texture->GetTextureId(),
                       {(float)colSize, (float)rowSize},
                       uvs.at(frameIndex).at(0),
                       uvs.at(frameIndex).at(1));
        }
        else
        {
          frameIndex = 0;
          firstTime = false;
          elapsedTime = 0.f;
          uvs.clear();
        }
      }
      else
        animationRun = false;
    }

    ImGui::End();
  }
}