# ~~~
# Summary:      Find and link general plugin libraries
# License:      GPLv3+
# Copyright (c) 2021 Alec Leamas
#
# Find and link general libraries to use: gettext, wxWidgets and OpenGL
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

find_package(Gettext REQUIRED)

set(wxWidgets_USE_DEBUG OFF)
set(wxWidgets_USE_UNICODE ON)
set(wxWidgets_USE_UNIVERSAL OFF)
set(wxWidgets_USE_STATIC OFF)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(OpenGL)
if (TARGET OpenGL::GL)
  target_link_libraries(${PACKAGE_NAME} OpenGL::GL)
else ()
  message(WARNING "Cannot locate usable OpenGL libs and headers.")
endif ()
if (NOT OPENGL_GLU_FOUND)
  message(WARNING "Cannot find OpenGL GLU extension.")
endif ()
if (APPLE)
  # As of 3.19.2, cmake's FindOpenGL does not link to the directory
  # containing gl.h. cmake bug? Intended due to missing subdir GL/gl.h?
  find_path(GL_H_DIR NAMES gl.h)
  if (GL_H_DIR)
    target_include_directories(${PACKAGE_NAME} PRIVATE "${GL_H_DIR}")
  else ()
    message(WARNING "Cannot locate OpenGL header file gl.h")
  endif ()
endif ()

set(wxWidgets_USE_LIBS base core net xml html adv stc)

find_package(wxWidgets REQUIRED base core net xml html adv)
if (MSYS)
  # This is just a hack. I think the bug is in FindwxWidgets.cmake
  string(
    REGEX REPLACE "/usr/local" "\\\\;C:/MinGW/msys/1.0/usr/local"
    wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS}
  )
endif ()

include(${wxWidgets_USE_FILE})
target_link_libraries(${PACKAGE_NAME} ${wxWidgets_LIBRARIES})

if (WIN32)
  add_subdirectory("${PROJECT_SOURCE_DIR}/libs/WindowsHeaders")
  target_link_libraries(${PACKAGE_NAME} windows::headers)
endif ()
