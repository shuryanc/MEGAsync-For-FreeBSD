MEGA Sync Client
================

Easy automated syncing between your computers and your MEGA cloud drive.

This repository contains all the development history of the official sync client of MEGA:
https://mega.nz/sync

#### Supported Platforms

* Microsoft Windows operating systems
* OS X
* Linux

#### Get the source
```
git clone --recursive https://github.com/meganz/MEGAsync.git
```
clone SDK from https://github.com/meganz/sdk.git to <local repository>/MEGASync/src/mega

Please refer to <local repository>/MEGASync/src/mega/README.md to download 3rdparty, if you haven't done so.
You can use the same 3rdparty folder for MEGASync if you have already built SDK. 
A typical layout may look like this:
.../3rdparty (one build to be used by all below, and more)
.../sdk (standalone sdk)
.../desktop (megasync)
.../desktop/megasync/mega (SDK specifically for megasync) 

# Windows compilation

##### Requirements:
* Visual Studio 2015 or above
* Qt 5.12.8
* Qt Creator

# Installing Qt 5.12.8 (open source version):
Download Qt online installer from https://www.qt.io/download-open-source?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5
    * Go to the bottom of the page and download the "Qt online installer"
    * Start the installer and go to the component selection page
    * If v5.12.8 is not available at first, select "Archive" and it will automatically go to the next page 
    * Go back (at top left). It will generate the available Qt versions
    * Select 5.12.8 and install

##### Preparation (Using QT Creator):
1.- Prepare QTCreator to use the Visual Studio 2015 or 2017 toolchain and Qt 5.12.8 - 32 bits

2.- Clone or download this repo

3.- Create a folder `Release_x32` inside the root of the code that you have just downloaded

4.- Download the required third party libraries from this link:
https://mega.nz/#!8tZglCTR!cZ_B39i9YxLgt3_8x9LsOpFhFDHuAh4jdz78IuQTQT4

5.- Uncompress that file into `src\MEGASync\mega\bindings\qt`
6.- Open the project `src/MEGA.pro` with QTCreator

7.- Select the folder `Release_x32` as the target for Release and Debug builds

8.- Build the project

9.- Copy or move the .dll files from the folder `src\MEGASync\mega\bindings\qt\3rdparty\bin` to the folder `Release_x32\MEGASync`

10.-  Enjoy!

It's recommended to go to `Project -> Run` in QTCreator and disable the option `Run in terminal`

##### Preparation (Using CMake):
1.- Download and install CMake from https://cmake.org/download/. Mininimum required version is 3.15
2.- Create cmake-build-x64 inside cmake (folder names matching pattern cmake-build-* in contrib/cmake are ignored by git)
3.- Run cmake-gui from the directory cmake-build-x64
4.- Provide the following information:
	3.a Source code folder: .../contrib/cmake/ 
	3.b Cmake-build-x64: .../contrib/cmake/cmake-build-64
5.- Configure with the desired Visual Studio settings
6.- Set the following in cmake-gui.exe or CMakeCache.txt:
    QT_BASE_PATH = C:/Qt/5.12.8/msvc2017_x64 (or the desired version of msvc)
	if using 3rdparty from vcpkg:
	  USE_THIRDPARTY_FROM_VCPKG = 1
	  USE_PREBUILT_3RDPARTY = 0	
    VCPKG_TRIPLET =x64-windows-mega (or the desired triplet from .../desktop/megasync/mega/contrib/cmake/vcpkg_extra_triplets)
7.- Generate 
8.- Go to cmake-build-x64 folder and open the sln file with the Visual Studio Version used in step 5
9.- Build the solution
10.-It will generate the artifacts in cmake-build-x64 folder
11.-Dependencies: You will need the following dependencies to execute MEGASync.exe which you can copy from 3rdparty folder and Qt installation:
Debug:
    1 avcodec-57.dll
    2 avformat-57.dll
    3 avutil-55.dll
    4 caresd.dll
    5 libcurld.dll
    6 libeay32.dll
    7 libsodium.dll
    8 pdfium.dll
    9 Qt5Cored.dll
    10 Qt5Guid.dll
    11 Qt5Networkd.dll
    12 Qt5Widgetsd.dll
    13 Qt5WinExtrasd.dll
    14 ssleay32.dll
    15 swresample-2.dll
    16 swscale-4.dll

Release:       
    1 avcodec-57.dll
    2 avformat-57.dll
    3 avutil-55.dll
    4 cares.dll
    5 libcurl.dll
    6 libeay32.dll
    7 libsodium.dll
    8 pdfium.dll
    9 Qt5Core.dll
    10 Qt5Gui.dll
    11 Qt5Network.dll
    12 Qt5Widgets.dll
    13 Qt5WinExtras.dll
    14 ssleay32.dll
    15 swresample-2.dll
    16 swscale-4.dll

12.- Enjoy!

# OS X compilation

##### Requirements:
* Xcode 8.3
* Qt 5.6.3

##### Preparation:
1.- Install Xcode in your system

2.- Clone or download this repo

3.- Download the required third party libraries and configuration file (`config.h`) from this link:
https://mega.nz/#!T14B0Y4T!EXGg9dRtacgz9_vCUtVq5tj6C3PBpkOHgXG289EBxBM

4.- Uncompress that file and move the folder `3rdparty` into `src/MEGASync/mega/bindings/qt`/ and the file `config.h` into `src/MEGASync/mega/include/mega/`

5.- Run the script `installer_mac.sh` to build the project and generate the application bundle for MEGAsync. If you want to generate an Apple disk image (DMG file), add the flag `--create-dmg`. Build directory is `Release_x64`

6.- Enjoy!

## Linux compilation

#### Requirements:

* Using `gcc` compiler below version 5 might result in compiler errors. 
* Third party requirements vary depending on your system.


#### Preparation:

First install dependencies. e.g., for a debian/ubuntu with QT >= 5.6 (e.g: Ubuntu 18.04):
```
sudo apt-get install libzen-dev libmediainfo-dev debhelper qtbase5-dev qt5-qmake libqt5x11extras5-dev libqt5dbus5 libqt5svg5-dev libcrypto++-dev libraw-dev libc-ares-dev libssl-dev libsqlite3-dev zlib1g-dev wget dh-autoreconf cdbs unzip libtool-bin pkg-config qt5-default qttools5-dev-tools libavcodec-dev libavutil-dev libavformat-dev libswscale-dev libmediainfo-dev
```

for older debian/ubuntu based systems (e.g: Ubuntu 16.04):
```
sudo apt-get install build-essential autoconf automake m4 libtool libtool-bin qt4-qmake make libqt4-dev libcrypto++-dev libsqlite3-dev libc-ares-dev libcurl4-openssl-dev libssl-dev libraw-dev libavcodec-dev libavutil-dev libavformat-dev libswscale-dev libmediainfo-dev
# Optional, if you wish to build nautilus extension:
sudo apt-get install libnautilus-extension-dev
```

Addition: for RPM based (e.g: Fedora): 
```
sudo dnf install libtool gcc-c++ c-ares-devel cryptopp-devel openssl-devel qt-devel sqlite-devel zlib-devel LibRaw-devel
```

Building:
```
cd src
./configure
qmake MEGA.pro
lrelease MEGASync/MEGASync.pro
make
```

Note: when compiling for Fedora/RHEL/CentOS and alike, `qmake` and `lrelease` might be missing for qt4. Use `qmake-qt4` and `lrelease-qt4` instead. Also, adding `-q` to `configure` is recommended in order to download and build cryptopp for RHEL and CentOS.

Known Issues
------------
For Solus, c-ares might not compile do to a CFLAG defined by gcc: -D_FORTIFY_SOURCE=2. This issue and its possible solution is described here https://github.com/c-ares/c-ares/issues/58 and http://ma.tc/ehuboqatec.md.
