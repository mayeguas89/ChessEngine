#pragma once

#include "state.h"

#include <memory>

class StateMachine
{
public:
  StateMachine() = default;
  ~StateMachine() = default;
  void ChangeState(std::unique_ptr<State> state);
  void Run();

private:
  std::unique_ptr<State> state_;
};