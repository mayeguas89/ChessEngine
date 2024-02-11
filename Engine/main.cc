#include "common/engine.h"

#include <spdlog/spdlog.h>

int main(int argc, char const* argv[])
{
  spdlog::info("Starting Engine");

  auto& engine = Engine::Instance();
  try
  {
    engine.Init();
    engine.Run();
  }
  catch (const std::exception& e)
  {
    spdlog::error("{}\n", e.what());
  }
  engine.End();
  return 0;
}