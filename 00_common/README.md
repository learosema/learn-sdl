# Shared resources

This folder contains shared resources used by the sub projects.

- [sdl-deleters.cpp](./sdl-deleters.cpp); custom deleters for SDL_Window, SDL_Texture etc for use with smart pointers
  (`std::unique_ptr`, ...). 
  They call the appropriate Destroy functions.

