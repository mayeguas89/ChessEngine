cmake_minimum_required(VERSION 3.26)

project(
  CHESS_ENGINE 
  VERSION 1.0 
  DESCRIPTION "ChessEngine"
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_RUNTIME_LIBRARY_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)

if(CMAKE_CONFIGURATION_TYPES) # Multi-config generator (Visual Studio)
  foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CONFIG}")
  endforeach()
endif()

add_subdirectory(external)
add_subdirectory(Engine)