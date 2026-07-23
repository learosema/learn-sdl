set(NAME "03-nuklear")
add_executable(${NAME})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_sources(${NAME} PRIVATE tiles-app.cpp main.cpp)

cmrc_add_resource_library(assets 
    assets/awesome-font.png
)

target_link_libraries(${NAME} PUBLIC SDL3::SDL3 fmt nuklear assets)
