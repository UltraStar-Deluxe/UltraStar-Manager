# UltraStar Manager Development README

[![Build Status](https://travis-ci.org/UltraStar-Deluxe/UltraStar-Manager.svg?branch=master)](https://travis-ci.org/UltraStar-Deluxe/UltraStar-Manager)
[![License](https://img.shields.io/badge/license-GPLv2-blue.svg)](LICENSE)

![UltraStar Manager Logo](https://github.com/UltraStar-Deluxe/UltraStar-Manager/blob/master/src/resources/uman128.png)


### 1. About
UltraStar Manager (uman) is a free and open source song management tool using Qt. It should be compilable and runnable on Windows, Linux and Mac OS.

### 2. Release Notes
(under construction)

### 3. Command-Line Parameters
Command-line parameters are passed to the game adding it to the path of a
shortcut or starting the game within the console.

- `-Songpath <PATH>`
    Example: `-SongPath "C:\Ultrastar Songs"`
    
This allows passing a certain folder as song path to UltraStar Manager, conveniently via the folder's context menu, as explained below.
#### Windows

1. Open the Explorer and browse to the program folder of UltraStar Manager.
2. Right-click on UltraStarManager.exe and select 'Create Shortcut'.
3. Right-click on the newly created shortcut file and select 'Properties'.
4. In the open tab 'Shortcut', under 'Target', add ' -SongPath' after 'UltraStarManager.exe' and confirm with 'OK'.
5. Right-click on the shortcut file again and select 'Cut' (STRG+X).
6. Enter 'shell:sendto' in the address bar and hit Enter.
7. Right-click inside the folder and select 'Paste' (STRG+V).

#### Mac OS X

1. Start Automator from your Applications folder or Launchpad.
2. (under construction)

### 4. Controls
(under construction)

### 5. Build and Run
#### Compiling on Windows
(under construction)

1. Download the Qt Online Installer for Windows from [here](http://download.qt.io/official_releases/online_installers/qt-unified-windows-x86-online.exe).
2. Install the Qt framework. The Qt Account login can be skipped.  
   Select (at least)
   * Qt -> Qt 5.8 -> MinGW 5.3.0 32 bit
   * Qt -> Tools -> MinGW 5.3.0.
3. Open and compile all task plugins (open respective *.pro files in subdirectory src\plugins with Qt Creator, disable shadow build).
4. Open and compile UltraStar Manager (open uman.pro in subdirectory src with Qt Creator, disable shadow build).
5. Add DLL dependencies to bin\wip directory
   * Qt libraries: Qt5Core.dll, Qt5Gui.dll, Qt5Multimedia.dll, Qt5Network.dll, Qt5PrintSupport.dll, Qt5Widgets.dll, Qt5Xml.dll
   * Qt imageformats: qgif.dll, qjpeg.dll to subdirectory wip\imageformats
   * Qt platforms: qwindows.dll to wip\platforms
   * MinGW runtime libraries: libgcc_s_dw2-1.dll, libstdc++-6.dll, libwinpthread-1.dll
   * Bass library: bass.dll
   * TagLib library: libtag.dll
6. Manage your entire song collection with ease!

#### Compiling on Linux
(under construction)

1. Install the Qt framework: `sudo apt get install qt5-default qttools5-dev-tools qtbase5-dev-tools qt5-qmake`
2. Install dependencies: `sudo apt get install libtag1-dev libmediainfo-dev libzen-dev`
3. Compile UltraStar Manager plugins
   * from command line
     * audiotag plugin: `cd src/plugins/audiotag && qmake audiotag.pro && make`
     * cleanup plugin: `cd src/plugins/cleanup && qmake cleanup.pro && make`
     * lyrics plugin: `cd src/plugins/lyric && qmake lyric.pro && make`
     * preparatory plugin: `cd src/plugins/preparatory && qmake preparatory.pro && make`
     * rename plugin: `cd src/plugins/rename && qmake rename.pro && make`
   * using Qt Creator: open the respective *.pro files in Qt Creator, disable shadow build in Project tab, then build
3. Compile UltraStar Manager
   * from command line: `cd src && qmake uman.pro && make` 
   * using Qt Creator: open uman.pro in Qt Creator, disable shadow build in Project tab, then build and run
4. Manage your entire song collection with ease!

#### Compiling on Mac OS X
(under construction)

1. Install homebrew via `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
2. Install the Qt framework via `brew install qt5'.
3. Install external dependencies via `brew install taglib mediainfo`
4. Compile UltraStar Manager plugins
   * from command line
     * audiotag plugin: `cd src/plugins/audiotag && qmake audiotag.pro && make`
     * cleanup plugin: `cd src/plugins/cleanup && qmake cleanup.pro && make`
     * lyrics plugin: `cd src/plugins/lyric && qmake lyric.pro && make`
     * preparatory plugin: `cd src/plugins/preparatory && qmake preparatory.pro && make`
     * rename plugin: `cd src/plugins/rename && qmake rename.pro && make`
   * using Qt Creator: open the respective *.pro files in Qt Creator, disable shadow build in Project tab, then build
4. Compile UltraStar Manager
   * from command line: `cd src && qmake uman.pro && make` 
   * using Qt Creator: open uman.pro in Qt Creator, disable shadow build in Project tab, then build and run
5. Manage your entire song collection with ease!

### 6. Contribute
Feel free to fork this project, modify it to your hearts content and maybe also do pull requests to this repository for additional features, improvements or clean-ups.
