cmake_minimum_required(VERSION 3.28)

project(ESP32Sim
  VERSION 0.1.0
  DESCRIPTION "Deterministic ESP32 simulator core"
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ESP32SIM_BUILD_TESTS "Build native tests" ON)
option(ESP32SIM_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

include(cmake/CompilerWarnings.cmake)

# Fora do if(): o alvo WASM também registra teste (smoke), mesmo com
# ESP32SIM_BUILD_TESTS=OFF.
enable_testing()

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/include/esp32sim")
configure_file(
  "${PROJECT_SOURCE_DIR}/include/esp32sim/version.hpp.in"
  "${PROJECT_BINARY_DIR}/include/esp32sim/version.hpp"
  @ONLY
)

add_subdirectory(core)

if(EMSCRIPTEN)
  add_subdirectory(wasm)
endif()

if(ESP32SIM_BUILD_TESTS)
  add_subdirectory(tests)
endif()
