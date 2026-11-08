add_library(esp32sim_core STATIC
  src/abi.cpp
)

target_include_directories(esp32sim_core
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)

esp32sim_set_warnings(esp32sim_core)

if(EMSCRIPTEN)
  # -matomics/-mbulk-memory são exigidos por --shared-memory (E0-05) e valem
  # para tudo que entra no módulo, por isso são PUBLIC.
  target_compile_options(esp32sim_core PUBLIC -fno-exceptions -fno-rtti
    -matomics -mbulk-memory)
endif()
