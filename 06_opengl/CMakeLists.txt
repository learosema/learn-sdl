set(NAME "06-opengl")
add_executable(${NAME})
add_subdirectory(glad)
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed-glsl.cmake")

embed_glsl("quad.vert.glsl" quadVertexShader)
embed_glsl("quad.frag.glsl" quadFragmentShader)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_sources(${NAME} PRIVATE shader-utils.cpp opengl-app.cpp main.cpp)



target_link_libraries(${NAME} PUBLIC SDL3::SDL3 fmt glad 00-common)
copy_runtime_dlls(${NAME})
