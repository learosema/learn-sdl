if (WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

if (MINGW)
  # Avoid depending on libgcc/libstdc++/libwinpthread DLLs from the MinGW
  # toolchain, which copy_runtime_dlls() can't see since they aren't CMake targets.
  add_link_options(-static-libgcc -static-libstdc++ -static)
endif()
