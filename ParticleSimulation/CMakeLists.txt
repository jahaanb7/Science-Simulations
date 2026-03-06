cmake_minimum_required(VERSION 3.16)
project(ParticleSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(PARTICLE_DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies")
option(PARTICLE_ALLOW_FETCH_DEPS "Allow fetching GLFW/GLM when not locally installed" ON)

# ── OpenGL ────────────────────────────────────────────────────────────────────
find_package(OpenGL REQUIRED)
target_link_libraries(ParticleSimulation PRIVATE OpenGL::GL)

# ── GLFW ─────────────────────────────────────────────────────────────────────
set(_glfw_target "")

find_package(glfw3 QUIET)
if(TARGET glfw)
    set(_glfw_target glfw)
elseif(TARGET glfw3)
    set(_glfw_target glfw3)
endif()

if(_glfw_target STREQUAL "")
    set(_glfw_local_dylib "${PARTICLE_DEPS_DIR}/library/libglfw.dylib")
    if(APPLE AND EXISTS "${_glfw_local_dylib}")
        # Local fallback for this repository's dependency folder on macOS.
        add_library(glfw_local SHARED IMPORTED)
        set_target_properties(glfw_local PROPERTIES
            IMPORTED_LOCATION "${_glfw_local_dylib}"
        )
        set(_glfw_target glfw_local)
    endif()
endif()

if(_glfw_target STREQUAL "" AND PARTICLE_ALLOW_FETCH_DEPS)
    include(FetchContent)
    # Fetch static GLFW so CI/demo builds do not rely on system package managers.
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw)
    set(_glfw_target glfw)
endif()

if(_glfw_target STREQUAL "")
    message(FATAL_ERROR "GLFW not found. Install GLFW or configure with -DPARTICLE_ALLOW_FETCH_DEPS=ON.")
else()
    target_link_libraries(ParticleSimulation PRIVATE ${_glfw_target})
endif()

# ── GLM ───────────────────────────────────────────────────────────────────────
# Try system install first, then local headers, then FetchContent.
find_package(glm QUIET)
if(glm_FOUND)
    target_link_libraries(ParticleSimulation PRIVATE glm::glm)
elseif(EXISTS "${PARTICLE_DEPS_DIR}/include/glm/glm.hpp")
    target_include_directories(ParticleSimulation PRIVATE "${PARTICLE_DEPS_DIR}/include")
elseif(PARTICLE_ALLOW_FETCH_DEPS)
    include(FetchContent)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glm)
    target_link_libraries(ParticleSimulation PRIVATE glm::glm)
else()
    message(FATAL_ERROR "GLM not found. Install GLM or configure with -DPARTICLE_ALLOW_FETCH_DEPS=ON.")
endif()

if(APPLE)
    # Keep local fallback dylib discoverable when used.
    set(_glfw_local_dylib "${PARTICLE_DEPS_DIR}/library/libglfw.dylib")
    if(TARGET glfw_local)
        get_filename_component(_glfw_local_dir "${_glfw_local_dylib}" DIRECTORY)
        target_link_options(ParticleSimulation PRIVATE "-Wl,-rpath,${_glfw_local_dir}")
    endif()
endif()

# ── macOS frameworks ──────────────────────────────────────────────────────────
if(APPLE)
    target_link_libraries(ParticleSimulation PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
