set(NAME "05-framebuffer")
add_executable(${NAME})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_sources(${NAME} PRIVATE framebuffer-app.cpp main.cpp)


embed_resource(assets/scifi-font.png "scifi-font.hpp" "SCIFI_FONT")

target_link_libraries(${NAME} PUBLIC SDL3::SDL3 fmt 00-common)
copy_runtime_dlls(${NAME})
