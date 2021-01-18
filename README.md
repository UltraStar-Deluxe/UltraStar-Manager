# UltraStar-Manager Development README

[![AppVeyor build status](https://ci.appveyor.com/api/projects/status/gypi0njftnnsmfij/branch/master?svg=true)](https://ci.appveyor.com/project/bohning/ultrastar-manager/branch/master)
[![License](https://img.shields.io/badge/license-GPLv2-blue.svg)](LICENSE)

![UltraStar-Manager Logo](https://github.com/UltraStar-Deluxe/UltraStar-Manager/blob/master/src/resources/UltraStar-Manager128.png)


### 1. About
UltraStar-Manager (uman) is a free and open source song management tool using Qt. It should be compilable and runnable on Windows, Linux and Mac OS.

### 2. Release Notes
(under construction)

### 3. Command-Line Parameters
Command-line parameters are passed to the game adding it to the path of a shortcut or starting the game within the console.

- `-SongPath <PATH>`
  Example: `-SongPath "C:\Ultrastar Songs"`
    
This allows passing a certain folder as song path to UltraStar-Manager, conveniently via the folder's context menu, as explained below (expert tip: you can use the same steps to use this neat feature for UltraStar Deluxe itself!).
#### Windows

1. Open the Explorer and browse to the program folder of UltraStar-Manager.
2. Right-click on UltraStar-Manager.exe and select 'Create Shortcut'. Rename the shortcut if you want.
3. Right-click on the newly created shortcut file and select 'Properties'.
4. In the open tab 'Shortcut', under 'Target', add ' -SongPath' after 'UltraStar-Manager.exe' and confirm with 'OK'.
5. Right-click on the shortcut file again and select 'Cut' (STRG+X).
6. Enter 'shell:sendto' in the address bar and hit Enter.
7. Right-click inside the folder and select 'Paste' (STRG+V).

#### Mac OS X

1. Start Automator from your Applications folder or Launchpad.
2. Choose 'Service' as document type.
3. Select `folders` in the 'Service receives' dropdown menu, and `Finder` in the next dropdown menu.
4. On the left, under 'Actions', select 'Files & Folders', find 'Get Selected Finder Items' and drag this action to the right pane.
5. Again under 'Actions', select 'Utilities', find 'Run Shell Script' and drag this action to the right pane below the first item.
6. Leave `/bin/bash` in the 'Shell' dropdown menu, but select `as arguments` in the 'Pass inputs' dropdown menu.
7. Paste `exec /Applications/UltraStar-Manager.app/Contents/MacOS/UltraStar-Manager -songpath "$1"` into the script field.
8. Save, e.g. as 'Start UltraStar-Manager with this folder'.

### 4. Controls
(under construction)

### 5. Build and Run
#### Compiling on Windows
(under construction)

1. Download the Qt Online Installer for Windows from [here](http://download.qt.io/official_releases/online_installers/qt-unified-windows-x86-online.exe).
2. Install the Qt framework. 
   Select (at least)
   * Qt -> Qt 5.15.2 -> MinGW 8.1.0 64-bit
   * Qt -> Developer and Designer Tools -> MinGW 8.1.0 64-bit
3. Open and compile all task plugins (except albumartexchange, amazon and freecovers)
   * open respective *.pro files in subdirectory src\plugins with Qt Creator
   * disable shadow build in Project tab
   * build
4. Open and compile UltraStar-Manager
   * open UltraStar-Manager.pro in subdirectory src with Qt Creator
   * disable shadow build in Project tab
   * build
5. Manage your entire song collection with ease!

#### Compiling on Linux
(under construction)

1. Install the Qt framework: `sudo apt-get install qt5-default qttools5-dev-tools qtbase5-dev-tools qt5-qmake qtmultimedia5-dev libqt5multimediawidgets5 libqt5multimedia5-plugins libqt5multimedia5`
2. Install dependencies: `sudo apt-get install libtag1-dev libmediainfo-dev libzen-dev`
3. Clone repository: `git clone https://github.com/UltraStar-Deluxe/UltraStar-Manager && cd UltraStar-Manager`
4. Compile UltraStar-Manager plugins
   * from command line
     * audiotag plugin: `cd src/plugins/audiotag && qmake audiotag.pro && make && cd ../../../`
     * cleanup plugin: `cd src/plugins/cleanup && qmake cleanup.pro && make && cd ../../../`
     * lyrics plugin: `cd src/plugins/lyric && qmake lyric.pro && make && cd ../../../`
     * preparatory plugin: `cd src/plugins/preparatory && qmake preparatory.pro && make && cd ../../../`
     * rename plugin: `cd src/plugins/rename && qmake rename.pro && make && cd ../../../`
   * using Qt Creator: open the respective *.pro files in Qt Creator, disable shadow build in Project tab, then build   
5. Compile UltraStar-Manager
   * from command line: `cd src && qmake UltraStar-Manager.pro && make && cd ../` (Note: if "make" fails with "stdlib.h was not found", open makefile and remove "-isystem /usr/include " from it. Afterwards, run "make" again.)
   * using Qt Creator: open UltraStar-Manager.pro in Qt Creator, disable shadow build in Project tab, then build and run
6. Run UltraStar Manager: `bin/release/UltraStar-Manager` and manage your entire song collection with ease!

#### Compiling on Mac OS X
(under construction)

1. Install homebrew via `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
2. Install the Qt framework via `brew install qt5'.
3. Install external dependencies via `brew install taglib mediainfo`
4. Compile UltraStar-Manager plugins
   * from command line
     * audiotag plugin: `cd src/plugins/audiotag && qmake audiotag.pro && make`
     * cleanup plugin: `cd src/plugins/cleanup && qmake cleanup.pro && make`
     * lyrics plugin: `cd src/plugins/lyric && qmake lyric.pro && make`
     * preparatory plugin: `cd src/plugins/preparatory && qmake preparatory.pro && make`
     * rename plugin: `cd src/plugins/rename && qmake rename.pro && make`
   * using Qt Creator: open the respective *.pro files in Qt Creator, disable shadow build in Project tab, then build
4. Compile UltraStar-Manager
   * from command line: `cd src && qmake UltraStar-Manager.pro && make` 
   * using Qt Creator: open UltraStar-Manager.pro in Qt Creator, disable shadow build in Project tab, then build and run
5. Manage your entire song collection with ease!

### 6. Contribute
Feel free to fork this project, modify it to your hearts content and maybe also do pull requests to this repository for additional features, improvements or clean-ups.
