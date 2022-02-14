## INSTALL: Building Plugins generic README.

Install build dependencies as described in the 
[manual](https://opencpn-manuals.github.io/main/AlternativeWorkflow/Local-Build.html)
Then clone this repository, enter it and make
`rm -rf build; mkdir build; cd build`.

A "normal" (not flatpak) tar.gz tarball which can be used by the new plugin
installer available from OpenCPN 5.2.0 is built using:

    $ cmake ..
    $ make tarball

To build the flatpak tarball:

    $ cmake ..
    $ make flatpak

Historically, it has been possible to build legacy packages like
an NSIS installer on Windows and .deb packages on Linux. This ability
has been removed in the 5.6.0 cycle.

#### Building for Android

This plugins is not prepared for Androids. 
It's unlikely a AIS Class A transponder would connect through a Andorid.


#### Building on windows (MSVC)
On windows, a different workflow is used:

    > ..\buildwin\win_deps.bat
    > cmake -T v141_xp -G "Visual Studio 15 2017" ^
           -DCMAKE_BUILD_TYPE=RelWithDebInfo  ..
    > cmake --build . --target tarball --config RelWithDebInfo
