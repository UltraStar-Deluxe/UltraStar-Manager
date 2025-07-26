# UltraStar-Manager Development README

[![Build](https://github.com/UltraStar-Deluxe/UltraStar-Manager/actions/workflows/main.yml/badge.svg)](https://github.com/UltraStar-Deluxe/UltraStar-Manager/actions/workflows/main.yml)
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
   Select (recommended)
   * Qt -> Qt 6.5 -> MSVC 2019
   * Qt -> Developer and Designer Tools -> MSVC 2019
3. Open and compile UltraStar-Manager
   * open `all.pro` with Qt Creator
   * disable shadow build in Project tab
   * build
4. Manage your entire song collection with ease!

#### Compiling on Linux
(under construction)

1. Install the Qt framework: `sudo apt-get install qt6-base-dev qt6-multimedia-dev`
2. Install dependencies: `sudo apt-get install libtag1-dev libcld2-dev libmediainfo-dev libebur128-dev libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-render-util0 libxcb-xinerama0 libxcb-xkb-dev libxkbcommon-x11-0 libxcb-cursor0 libgtk2.0-dev`
3. Clone repository: `git clone https://github.com/UltraStar-Deluxe/UltraStar-Manager && cd UltraStar-Manager`
4. Compile UltraStar-Manager
   * from command line: `qmake6 && make` (Note: if "make" fails with "stdlib.h was not found", open makefile and remove "-isystem /usr/include " from it. Afterwards, run "make" again.)
   * using Qt Creator: open `all.pro` in Qt Creator, disable shadow build in Project tab, then build and run
5. Run UltraStar Manager: `bin/release/UltraStar-Manager` and manage your entire song collection with ease!

#### Compiling on Mac OS X
(under construction)

1. Install homebrew via `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
2. Install the Qt framework via `brew install qt`.
3. Install external dependencies via `brew install taglib mediainfo libebur128`
4. Compile UltraStar-Manager
   * from command line: `qmake6 && make`
   * using Qt Creator: open `all.pro` in Qt Creator, disable shadow build in Project tab, then build and run
5. Manage your entire song collection with ease!

### 6. Contribute
Feel free to fork this project, modify it to your hearts content and maybe also do pull requests to this repository for additional features, improvements or clean-ups.
