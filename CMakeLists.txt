# ~~~
# Author:      Rasbats aka. Mike Rossiter
# Copyright:
# License:     GPLv2+
# ~~~


# -------- Cmake setup ---------
#
cmake_minimum_required(VERSION 3.12.0)

cmake_policy(SET CMP0042 NEW)

if (POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif ()

if (POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif ()

# Locations where cmake looks for cmake modules.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build ${CMAKE_SOURCE_DIR}/cmake)

if (WIN32)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/buildwin)
endif ()
message(STATUS "Cmake version: ${CMAKE_VERSION}.")


# -------- Build setup (mostly in PluginCompiler) --------
#
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
message(STATUS "Building: ${BUILD_TYPE}")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(
    -Wno-inconsistent-missing-override
    -Wno-potentially-evaluated-expression
  )
endif ()

# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/ais-vd-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/ais-vd-beta"
    CACHE STRING 
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/ais-vd-prod"
    CACHE STRING 
    "Default repository for tagged builds not matching 'beta'"
)
option(ais-vd_USE_SVG "Use SVG graphics" OFF)

set(OCPN_TARGET_TUPLE "" CACHE STRING
  "Target spec: \"platform;version;arch\""
)

string(TOLOWER "${OCPN_TARGET_TUPLE}" _lc_target)
if ("${_lc_target}" MATCHES "android*")
  set(QT_ANDROID ON)
  set(OCPN_USE_BUNDLED_WXCURL OFF)
  # Until we have a proper toolchain file:
  set(CMAKE_CROSSCOMPILING ON)
else ()
  set(QT_ANDROID OFF)
  add_definitions(-D__OCPN_USE_CURL__)
endif ()
#
#
# -------  Plugin setup --------
#
project(ais-vd_pi VERSION 1.1.15)
set(PKG_PRERELEASE "Beta")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME ais-vd)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME ais-vd) # As of GetCommonName() in plugin API
set(CPACK_PACKAGE_CONTACT "Dirk R Hakan S")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Update AIS voyage data")
set(CPACK_PACKAGE_DESCRIPTION [=[
Update AIS class A voyage data as 
Destination 
Time/Date of arrival 
and Navigational status
by NMEA0183 ECVSD.
]=])

set(PKG_AUTHOR "Dirk R & Hakan S")
set(PKG_IS_OPEN_SOURCE "yes")
set(CPACK_PACKAGE_HOMEPAGE_URL https://github.com/Hakansv/ais-vd_pi)
set(PKG_INFO_URL https://github.com/Hakansv/ais-vd_pi)

set(SRC
  src/ais-vd_pi.h
 	src/ais-vd_pi.cpp
 	#src/SpesTextCtrl.h
 	#src/SpesTextCtrl.cpp
)

# -------- Setup completed, build the plugin --------
#
add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC})
include_directories(BEFORE ${CMAKE_BINARY_DIR}/include)

add_subdirectory("libs/api-16")
target_link_libraries(${CMAKE_PROJECT_NAME} ocpn::api)

set(VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(PACKAGE_NAME ${CMAKE_PROJECT_NAME})

# Create the minimim ocpn version id string like ov50
string(REGEX REPLACE "\([0-9]\)\.\([0-9]\).*" "ov\\1\\2"
       OCPN_MIN_VERSION ${API_OCPN_MIN_VERSION})

# if (ais-vd_USE_SVG)
#   add_definitions(-DSHIPDRIVER_USE_SVG)
# endif ()

# Set up targets. Targets sets up a recursive call with BUILD_TYPE set to
# 'flatpak', 'pkg' or 'tarball'. The initial call without BUILD_TYPE ends
# here.
#
include(Targets)
create_targets(
  ${PROJECT_SOURCE_DIR}/flatpak/org.opencpn.OpenCPN.Plugin.ais-vd.yaml
)
if ("${BUILD_TYPE}" STREQUAL "")
  return ()
endif ()

if (NOT ${BUILD_TYPE} STREQUAL "flatpak")
  # Build package as required (flatpak already dealt with).
  #
  include(PluginCompiler)
  include(PluginLocalization)
  include(PluginInstall)
  include(PluginPackage)
  if (QT_ANDROID)
    include(libs/AndroidLibs.cmake)
  else ()
    include(PluginLibs)
  endif ()

  include_directories(BEFORE ${CMAKE_BINARY_DIR}/include)
  include_directories(${CMAKE_SOURCE_DIR}/src)
  include_directories(${CMAKE_SOURCE_DIR}/include)
  #include_directories(${CMAKE_SOURCE_DIR}/libs/gdal/src)

  add_subdirectory("libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)
  
  #add_subdirectory("libs/jsoncpp")
  #target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)
  
  add_subdirectory("libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)
  
  if (ARCH STREQUAL "armhf")
   add_compile_definitions(OCPN_ARMHF)
  endif ()  
  
endif ()

configure_file(
  ${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/include//config.h
  @ONLY
)

configure_file(
  # The cloudsmith upload script
  ${CMAKE_SOURCE_DIR}/ci/upload.sh.in ${CMAKE_BINARY_DIR}/upload.sh
  @ONLY
)
configure_file(
  # The cloudsmith upload script, windows bat file.
  ${CMAKE_SOURCE_DIR}/ci/upload.bat.in ${CMAKE_BINARY_DIR}/upload.bat
  @ONLY
)


set(checksum "@checksum@")
configure_file(
  # The XML metadata file
  ${CMAKE_SOURCE_DIR}/plugin.xml.in
  ${CMAKE_BINARY_DIR}/${pkg_displayname}.xml.in
  @ONLY
)
