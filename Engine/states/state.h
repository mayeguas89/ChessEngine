#pragma once
#include "common/sprite.h"

#include <memory>
#include <string>
class StateMachine;
struct State
{
  State(StateMachine& p_stateMachine): stateMachine{p_stateMachine} {}
  virtual void Init() = 0;
  virtual void Run() = 0;
  virtual void End() = 0;

protected:
  StateMachine& stateMachine;
};

class CalibrationState: public State
{
public:
  CalibrationState(StateMachine& p_stateMachine): State(p_stateMachine) {}
  virtual void Init() override;
  virtual void Run() override;
  virtual void End() override;

private:
  inline static const std::string ICON_PATH{"Assets/target_icon.png"};
  uint32_t iconId_;
  size_t uiIndexCallback_;
  glm::vec2 calibrationCoordinates_;
};

class GameInitState: public State
{
public:
  GameInitState(StateMachine& p_stateMachine): State(p_stateMachine) {}
  virtual void Init() override;
  virtual void Run() override;
  virtual void End() override;
};

class PlayerOneState: public State
{
public:
  PlayerOneState(StateMachine& p_stateMachine): State(p_stateMachine) {}
  virtual void Init() override;
  virtual void Run() override;
  virtual void End() override;
};

class PlayerTwoState: public State
{
public:
  PlayerTwoState(StateMachine& p_stateMachine): State(p_stateMachine) {}
  virtual void Init() override;
  virtual void Run() override;
  virtual void End() override;
};