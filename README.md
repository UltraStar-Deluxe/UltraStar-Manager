# UltraStar Manager Development README

![UltraStar Manager Logo](https://github.com/UltraStar-Deluxe/UltraStar-Manager/blob/master/src/resources/uman128.png)


###1. About
UltraStar Manager (uman) is a free and open source song management tool using Qt. It should be compilable and runnable on Windows, Linux and Mac OS.

###2. Release Notes
(under construction)

###3. Command-Line Parameters
Command-line parameters are passed to the game adding it to the path of a
shortcut or starting the game within the console.

The following parameters are possible. They can be joined in any possible way.
- `-Songpath <PATH>`   : Same as config Songpath.
                         Example: `-SongPath "C:\Ultrastar Songs"`

###4. Controls
(under construction)

###5. Build and Run
Download the Qt framework for your respective platform from https://www.qt.io/download-open-source/.

####Compiling on Windows
(under construction)
1. Install Qt (MinGW).
2. Open and compile all task plugins (open respective *.pro files in subdirectory plugins).
3. Compile UltraStar Manager.
4. Add DLL dependencies to /bin/wip diretory (Qt + MinGW runtime libraries, bass, taglib, mediainfo).

####Compiling on Linux/BSD using make
(under construction)

####Compiling on OS X
1. Install Qt (clang).
2. 2. Open and compile all task plugins (open respective *.pro files in subdirectory plugins).
3. Compile UltraStar Manager.

Feel free to fork this project, modify it to your hearts content and maybe also do pull requests to this repository for additional features, improvements or clean-ups.
