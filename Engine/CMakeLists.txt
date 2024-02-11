add_executable(
  ${PROJECT_NAME} 
  main.cc
  common/camera.cc
  common/engine.cc
  common/entity.cc
  common/sprite.cc
  common/chess.cc
  render/gl_render.cc
  render/gl_texture.cc
  render/render_program.cc
  states/state_machine.cpp
  states/state.cpp
  ui/ui.cc
  ui/scene_graph_viewport.cc
  ui/scene_viewport.cc
  ui/properties_viewport.cc
  ui/spritemap_window.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLAD_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/external/imgui
  ${CMAKE_SOURCE_DIR}/external/imgui/backends
  ${CMAKE_SOURCE_DIR}/external/stb
)

target_link_libraries(${PROJECT_NAME} 
 PRIVATE
#  argparse::argparse
 glad
 glfw
 glm
 Imgui
 spdlog::spdlog
#  nlohmann_json::nlohmann_json
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets
)