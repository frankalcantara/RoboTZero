# Line Follower Simulator - C++ WebAssembly Build Configuration
cmake_minimum_required(VERSION 3.15)
project(LineFollowerSimulator VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../public)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
    ${CMAKE_SOURCE_DIR}/external/box2d/include
)

# Source files
set(SOURCES
    src/simulator.cpp
    src/optimizer.cpp
    src/physics.cpp
    src/pattern_recognizer.cpp
    src/bindings.cpp
)

# Artifact sources (Phase 2)
set(ARTIFACT_SOURCES
    # src/artifacts/straight.cpp
    # src/artifacts/circular_curve.cpp
    # src/artifacts/s_curve.cpp
    # src/artifacts/chicane.cpp
    # src/artifacts/hairpin.cpp
    # src/artifacts/spiral.cpp
    # src/artifacts/complex.cpp
)

# Optimizer sources (Phase 2)
set(OPTIMIZER_SOURCES
    # src/optimizers/gradient_descent.cpp
    # src/optimizers/lbfgs.cpp
    # src/optimizers/mpc.cpp
    # src/optimizers/direct_collocation.cpp
)

# Create executable
if(EMSCRIPTEN)
    # WebAssembly build
    add_executable(simulator ${SOURCES} ${ARTIFACT_SOURCES} ${OPTIMIZER_SOURCES})

    # Emscripten-specific settings
    set(EMSCRIPTEN_FLAGS
        -O3
        -s WASM=1
        -s MODULARIZE=1
        -s EXPORT_NAME="createSimulatorModule"
        -s ALLOW_MEMORY_GROWTH=1
        -s INITIAL_MEMORY=16MB
        -s MAXIMUM_MEMORY=512MB
        -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']
        --bind
        -s NO_DISABLE_EXCEPTION_CATCHING
        -s ASSERTIONS=0
    )

    # Debug flags (optional, comment out for production)
    # set(EMSCRIPTEN_FLAGS ${EMSCRIPTEN_FLAGS}
    #     -g
    #     -s ASSERTIONS=1
    #     -s SAFE_HEAP=1
    # )

    target_compile_options(simulator PRIVATE ${EMSCRIPTEN_FLAGS})
    target_link_options(simulator PRIVATE ${EMSCRIPTEN_FLAGS})

    # Output files
    set_target_properties(simulator PROPERTIES
        OUTPUT_NAME "simulator"
        SUFFIX ".js"
    )

    # Copy WASM file to public directory
    add_custom_command(TARGET simulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/simulator.wasm
            ${CMAKE_SOURCE_DIR}/../public/simulator.wasm
        COMMENT "Copying WASM file to public directory"
    )

else()
    # Native build (for testing)
    add_executable(simulator_native ${SOURCES} ${ARTIFACT_SOURCES} ${OPTIMIZER_SOURCES})
    target_compile_options(simulator_native PRIVATE -Wall -Wextra -O2)
endif()

# Box2D library
# Note: Will need to be compiled for WebAssembly separately
# See external/box2d/README.md for instructions

# Link libraries (when available)
if(EXISTS ${CMAKE_SOURCE_DIR}/external/box2d/build/src/libbox2d.a)
    if(EMSCRIPTEN)
        target_link_libraries(simulator
            ${CMAKE_SOURCE_DIR}/external/box2d/build/src/libbox2d.a
        )
    else()
        target_link_libraries(simulator_native
            ${CMAKE_SOURCE_DIR}/external/box2d/build/src/libbox2d.a
        )
    endif()
else()
    message(WARNING "Box2D library not found. Please compile Box2D first.")
    message(WARNING "See cpp/external/README.md for instructions")
endif()

# Installation
if(EMSCRIPTEN)
    install(FILES
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/simulator.js
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/simulator.wasm
        DESTINATION ${CMAKE_SOURCE_DIR}/../public
    )
endif()

# Print configuration
message(STATUS "")
message(STATUS "Line Follower Simulator Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
if(EMSCRIPTEN)
    message(STATUS "  Target: WebAssembly")
    message(STATUS "  Emscripten: ${EMSCRIPTEN_VERSION}")
else()
    message(STATUS "  Target: Native")
endif()
message(STATUS "")
