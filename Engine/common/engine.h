#pragma once

#include "chess.h"
#include "common/camera.h"
#include "common/entity.h"
#include "render/gl_texture.h"
#include "states/state_machine.h"
#include "utils/singleton.h"

#include <memory>
#include <tuple>
#include <unordered_map>

class Engine: public Singleton<Engine>
{
public:
  void Init();
  void Run();
  void End();

  std::shared_ptr<GLTexture> GetTexture(const std::string& filename);

  std::vector<std::shared_ptr<Entity>> GetEntities() const
  {
    return entities_;
  }

  void SetCalibrationCoordinates(const glm::vec2& calibrationCoordinates)
  {
    calibrationCoordinates_ = calibrationCoordinates;
  }

  chess::Player& GetPlayerOne()
  {
    return playerWhite_;
  }

  chess::Player& GetPlayerTwo()
  {
    return playerBlack_;
  }

  size_t AddEntity(std::shared_ptr<Entity> entity)
  {
    entities_.push_back(std::move(entity));
    return entities_.size() - 1;
  }

  bool RemoveEntity(const uint32_t index)
  {
    if (entities_.size() <= index)
      return false;

    entities_.erase(entities_.begin() + index);
    return true;
  }

  const Camera& GetMainCamera() const
  {
    return mainCamera_;
  }

  Camera& GetMainCamera()
  {
    return mainCamera_;
  }

  void ResizeFrameBuffer(const glm::vec<2, int>& windowSize);

  void RayTest(const glm::vec2& worldCoordinates);

  unsigned int textureId_ = 0;
  unsigned int frameBufferId_ = 0;

private:
  std::shared_ptr<chess::Board> board_;
  chess::Player playerWhite_{Color::White};
  chess::Player playerBlack_{Color::Black};
  glm::vec2 calibrationCoordinates_;
  bool isRunning_{false};
  std::vector<std::shared_ptr<Entity>> entities_;
  std::unordered_map<std::string, std::shared_ptr<GLTexture>> textureMap_;
  Camera mainCamera_;
  Color currentPlayer;
  StateMachine stateMachine_;
};