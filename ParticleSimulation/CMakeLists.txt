cmake_minimum_required(VERSION 3.16)
project(ParticleSimulation)

set(CMAKE_CXX_STANDARD 17)

# Define M_PI on Windows (MSVC doesn't include it by default)
if(MSVC)
    add_compile_definitions(_USE_MATH_DEFINES)
endif()

add_executable(ParticleSimulation
    main.cpp
    glad.c
)

target_include_directories(ParticleSimulation PRIVATE
    library
    ../dependencies/include
)

# ── OpenGL ────────────────────────────────────────────────────────────────────
find_package(OpenGL REQUIRED)
target_link_libraries(ParticleSimulation OpenGL::GL)

# ── GLFW ─────────────────────────────────────────────────────────────────────
find_package(glfw3 QUIET)
if(glfw3_FOUND)
    # Installed system-wide (CI / Homebrew / apt)
    target_link_libraries(ParticleSimulation glfw)
else()
    # Fall back to local dylib (your Mac dev machine)
    add_library(glfw_local SHARED IMPORTED)
    set_target_properties(glfw_local PROPERTIES
        IMPORTED_LOCATION /Users/juhi/Desktop/Simulation/dependencies/library/libglfw.dylib
    )
    target_link_libraries(ParticleSimulation glfw_local)
endif()

# ── GLM ───────────────────────────────────────────────────────────────────────
# Try system install first, fall back to local headers
find_package(glm QUIET)
if(glm_FOUND)
    target_link_libraries(ParticleSimulation glm::glm)
else()
    # Use the glm headers bundled in your dependencies folder
    target_include_directories(ParticleSimulation PRIVATE
        /Users/juhi/Desktop/Simulation/dependencies/include
    )
endif()

# ── macOS frameworks ──────────────────────────────────────────────────────────
if(APPLE)
    target_link_libraries(ParticleSimulation
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()