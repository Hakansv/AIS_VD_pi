# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


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

#
#
# -------  Plugin setup --------
#
set(PKG_VERSION  1.5.0)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME ais-vd)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME ais-vd) # As of GetCommonName() in plugin API
set(CPACK_PACKAGE_CONTACT "Hakan Svensson")
set(PKG_SUMMARY "Update AIS class A voyage data")
set(PKG_DESCRIPTION [=[
Update AIS class A voyage static data as:
Navigational status
Destination, Time/Date of arrival,
effective draught and persons on board
using NMEA0183 ECVSD sentence.
]=])

set(PKG_AUTHOR "Dirk R and Hakan S")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/Hakansv/ais-vd_pi)
set(PKG_INFO_URL https://opencpn-manuals.github.io/main/ais-vd/0.1/index.html)

set(SRC
  src/ais-vd_pi.h
  src/ais-vd_pi.cpp
  #src/SpesTextCtrl.h
  #src/SpesTextCtrl.cpp
)

set(PKG_API_LIB api-16)  #  A directory in libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("libs/plugingl")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

  add_subdirectory("libs/wxsvg")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxsvg)
endmacro ()
