cmake_minimum_required(VERSION 3.20)
project(Engine3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put all binaries in build/bin so the .exe and any DLLs sit together
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ---------------------------------------------------------------------------
# GLFW (built from source in ThirdParty/glfw)
# ---------------------------------------------------------------------------
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
add_subdirectory(ThirdParty/glfw)

# ---------------------------------------------------------------------------
# GLAD 2.0 - the loader is generated at configure time (requires Python).
# REPRODUCIBLE = use the bundled spec instead of fetching it from Khronos.
# API gl:core=3.3 = OpenGL 3.3 Core. Change the version here if needed.
# ---------------------------------------------------------------------------
add_subdirectory(ThirdParty/glad/cmake glad_cmake)
glad_add_library(glad STATIC REPRODUCIBLE API gl:core=3.3)

# ---------------------------------------------------------------------------
# GLM (header-only, target glm::glm)
# ---------------------------------------------------------------------------
add_subdirectory(ThirdParty/glm)

# ---------------------------------------------------------------------------
# Engine executable
# ---------------------------------------------------------------------------
add_executable(Engine3D Engine3D/Engine3D.cpp)
target_link_libraries(Engine3D PRIVATE glfw glad glm::glm)

# Make Engine3D the startup project in generated Visual Studio solutions
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine3D)
