set(NAME "01-window")
add_executable(${NAME})


target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_sources(${NAME} PRIVATE main.cpp)
target_link_libraries(${NAME} PUBLIC SDL3::SDL3 fmt)
