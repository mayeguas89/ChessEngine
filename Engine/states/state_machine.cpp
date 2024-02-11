#include "state_machine.h"

void StateMachine::ChangeState(std::unique_ptr<State> state)
{
  if (state_)
    state_->End();
  state_ = std::move(state);
  state_->Init();
}

void StateMachine::Run()
{
  if (state_)
    state_->Run();
}
