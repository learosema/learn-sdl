set(NAME "00-common")
add_library(${NAME} STATIC)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(${NAME} PRIVATE cxx_std_20)
target_sources(${NAME} PRIVATE sdl-deleters.cpp)
target_link_libraries(${NAME} PRIVATE SDL3::SDL3)
