include(FetchContent)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY "https://github.com/onqtam/doctest.git"
  GIT_TAG v2.5.2
)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
  GIT_TAG 12.1.0
)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-3.4.14
  FIND_PACKAGE_ARGS NAMES SDL3 CONFIG
)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
  GIT_TAG v1.15.3
)

FetchContent_Declare(
  nuklear
  GIT_REPOSITORY "https://github.com/Immediate-Mode-UI/Nuklear.git"
  GIT_TAG v4.13.3
)

# FetchContent_Declare(
#   cmrc
#   GIT_REPOSITORY "https://github.com/vector-of-bool/cmrc.git"
#   GIT_TAG 2.0.1
# )

FetchContent_Declare(
  SDL_Image
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL_image.git"
  GIT_TAG release-3.4.4
  FIND_PACKAGE_ARGS NAMES SDL3_image CONFIG
)


# For SDL2 to be able to override options
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(DOCTEST_NO_INSTALL ON)
set(FMT_INSTALL OFF)
set(SPDLOG_FMT_EXTERNAL ON)

FetchContent_MakeAvailable(doctest fmt SDL3 SDL_Image spdlog nuklear)

add_library(nuklear INTERFACE)
target_include_directories(nuklear INTERFACE ${nuklear_SOURCE_DIR}/)