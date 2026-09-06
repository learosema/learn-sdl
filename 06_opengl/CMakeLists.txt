set(NAME "06-opengl")
add_executable(${NAME})
add_subdirectory(glad)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_sources(${NAME} PRIVATE opengl-app.cpp main.cpp)

target_link_libraries(${NAME} PUBLIC SDL3::SDL3 fmt glad 00-common)
copy_runtime_dlls(${NAME})
