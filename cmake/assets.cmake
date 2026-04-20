# Plagiated from 
# https://github.com/MartinHelmut/cpp-gui-template-sdl3

# Assets for all platforms
set(SHARED_STATIC_ASSETS ${PROJECT_SOURCE_DIR}/assets/fonts/Manrope.ttf)

# Platform specific static assets
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_sources(${NAME} PUBLIC
    ${SHARED_STATIC_ASSETS}
    ${PROJECT_SOURCE_DIR}/res/icon.ico
    ${PROJECT_SOURCE_DIR}/res/app.rc
    ${PROJECT_SOURCE_DIR}/res/Manifests/App.manifest)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(MACOSX_STATIC_ASSETS
    ${SHARED_STATIC_ASSETS}
    ${PROJECT_SOURCE_DIR}/src/assets/icons/icon.icns)
  set_source_files_properties(${MACOSX_STATIC_ASSETS}
    PROPERTIES MACOSX_PACKAGE_LOCATION ${CMAKE_INSTALL_DATADIR})
  target_sources(${NAME} PUBLIC ${MACOSX_STATIC_ASSETS})
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_sources(${NAME} PRIVATE ${SHARED_STATIC_ASSETS})
endif ()

# General target installation settings
install(TARGETS ${NAME}
  BUNDLE DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Settings for packaging per platform
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  include(${PROJECT_SOURCE_DIR}/cmake/packaging/Windows.cmake)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  include(${PROJECT_SOURCE_DIR}/cmake/packaging/Linux.cmake)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  include(${PROJECT_SOURCE_DIR}/cmake/packaging/Darwin.cmake)
endif ()

