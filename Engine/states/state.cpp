#include "state.h"

#include "common/engine.h"
#include "spdlog/spdlog.h"
#include "state_machine.h"
#include "ui/ui.h"

void CalibrationState::Init()
{
  spdlog::info("Starting calibration");
  auto icon = std::make_shared<Sprite2D>(ICON_PATH);
  icon->SetPosition({-0.5f, 7.5f, -0.1f, 0.f});
  icon->SetScale({0.5f, 0.5f, 1.f, 1.f});
  icon->active_ = true;
  iconId_ = Engine::Instance().AddEntity(std::move(icon));
  uiIndexCallback_ = UI::Instance().RegisterMouseClickOnSceneViewport(
    [&](const glm::vec2& worldCoordinates)
    {
      if (worldCoordinates.x > -0.7f && worldCoordinates.x < -0.3f && worldCoordinates.y > 7.7f
          && worldCoordinates.y < 8.3f)
      {
        calibrationCoordinates_ = worldCoordinates;
        spdlog::info("Hit on target");
        spdlog::info("Calibration {{0,0}} is now ({},{})", worldCoordinates.x, worldCoordinates.y);
        stateMachine.ChangeState(std::move(std::make_unique<GameInitState>(stateMachine)));
      }
    });
}

void CalibrationState::Run() {}

void CalibrationState::End()
{
  Engine::Instance().SetCalibrationCoordinates(calibrationCoordinates_);
  if (not Engine::Instance().RemoveEntity(iconId_))
    spdlog::info("Icon could not be removed");
  UI::Instance().UnregisterMouseClickOnSceneViewport(uiIndexCallback_);
}

void GameInitState::Init()
{
  spdlog::info("Starting GameInitState");
  Engine::Instance().GetPlayerOne().Init();
  Engine::Instance().GetPlayerTwo().Init();
  stateMachine.ChangeState(std::move(std::make_unique<PlayerOneState>(stateMachine)));
}

void GameInitState::Run() {}

void GameInitState::End() {}

void PlayerOneState::Init()
{
  spdlog::info("Starting PlayerOneState");
}
void PlayerOneState::Run() {}

void PlayerOneState::End() {}

void PlayerTwoState::Run() {}

void PlayerTwoState::End() {}

void PlayerTwoState::Init()
{
  spdlog::info("Starting PlayerTwoState");
}