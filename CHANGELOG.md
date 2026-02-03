# UltraStar Manager Changelog

## Changelog (2.3.0 to 2.3.1)
### Bug fixes
- Fix formatting of report dialog

## Changelog (2.2.1 to 2.3.0)
### Features
- Full support for `#VERSION`:
  - Add a column in the song tree that displays the version of the text file
  - Add a preparatory task that allows users to batch convert text files between versions
- Full support for `#AUDIO`:
  - Respect `#AUDIO` in addition to `#MP3` for all program features (audio file identification/selection in song tree, song details tab, etc.)
  - Add a preparatory task that allows users to batch convert their text files between using `#MP3` and `#AUDIO`
  - Add an optional/non-default column in the song tree which indicates whether a text uses `#MP3` or `#AUDIO`, for the purpose of allow users to enforce a standard for their library
- Make settings for reports persistent, and add a new button to reset the settings to their default values

### Bug fixes
- Make MacOS portable build executable
- Remove unused DLL file on Windows

## Changelog (2.2.0 to 2.2.1)
### Bug fixes
- Fix ReplayGain scanner on macOS
- In song tree, use text color according to the active system theme
- Fix dependency management issue on macOS
- Improve Linux desktop integration

## Changelog (2.1.0 to 2.2.0)
### Features
- Improved support for text file line endings:
  - Automatically detect if a text file has CRLF or LF line endings. When writing changes to a file, preserve the file's existing line ending.
  - Added a column in the song tree which indicates if the text file has CRLF or LF line endings
  - Added a task which allows the user to batch convert text files to CRLF or LF line endings
- Modernized first time path setup dialog by removing unecessary settings.

### Bug fixes
- Fixed display of rap notes in media player

## Changelog (2.0.0 to 2.1.0)
### Features
- ReplayGain support:
  - A new column is added in the song tree which indicates with a tick or cross whether a song's audio file has ReplayGain information. If the song has ReplayGain information, the tooltip will display the gain and peak values
  - Scanning feature: select one or more songs from the song tree, then right click and select "Calculate Song ReplayGain" from the context menu. The Manager will scan the audio files for loudness, write ReplayGain tags to the files, while the progress dialog displays the scan progress.
- Support for UltraStar song format versions up to 1.2.0
  - Tags required by the format version are read/written to the song file, but converting between different format versions is not yet supported
- Support Opus and FLAC audio files by default

### Bug fixes
- Fixed media player
- Fixed color of directory files in the song tree
- Fixed images that had an incorrect sRGB profile.
- Improve determining image dimensions, remove determining video dimensions
- Fixed various issues with the Windows installed version:
  - Fixed incorrect application style
  - Fixed media plugins
  - Fixed check for updates feature
  - Fixed installer showing incorrect program version
- Reduce size of macOS portable package by not including the DMG file (can be downloaded separately)

## Changelog (1.8.4 to 2.0.0)

- UTF8 (enforced output)
- cld2 integration for language detection
- hitparade.ch integration for release year, artist/title spell check
- song list PDF creation customization
- mediainfo integration for video codec / resolution information
- resource quality indicators (resolution/dimensions)
- now with support for Linux and ARM Macs
- ...many more minor features and fixes are part of this release, too numerous to list.


## Changelog (1.8.2 to 1.8.4)

### NEW FEATURES

#### MEDIA
- MediaInfo library integrated for multimedia (audio/video) file analysis

### CHANGES

#### QT
- updated to Qt 5.12.1

### BUG FIXES

### KNOWN BUGS

## Changelog (1.8.2 to 1.8.3)

### NEW FEATURES
SONG DETAILS
- Added news tags: #ALBUM, #COVER, #P1, #P2, #ARTIST-ON-SORTING, #TITLE-ON-SORTING

SONG TREE
- Video files may now be set for both audio and video file (also simultaneously).


### CHANGES

#### QT
- updated to Qt 5.10.0

#### BASS
- dependency to BASS removed

#### TAGLIB
- updated to TagLib 1.11.1


### BUG FIXES

#### RENAME TASK
- Fixed renamed cover/background when a single file was assigned to both cover and background

### KNOWN BUGS

#### INSTALLER
- Uninstaller does not delete "non-standard" tasks

#### TASKS
- convert syllable placeholders does not ignore "wanted" dashes ("A-B-C")
- "remove empty syllables" breaks timestamps in relative songs
- capitalization task makes "TV" to "Tv" - should ignore words that are entirely upper-case

#### TASK LIST
- tooltip of slot buttons is empty before the first selection of the slot

#### SONG FRIENDS (general workaround: switch friend to primary song using the context menu)
- cannot add song friends to playlist
- cannot change #MP3 for karaoke song friend
- cannot change #EDITION for song friend
- cannot add song friends in reports
- cannot play song friends in media player
- some tasks may not affect song friends (e.g. "Remove unsupported tags")

#### MEDIA PLAYER
- correct songs get ansychronous after a while (seek works correctly)
- uman crashes if you play a song, change the song directory and skip

#### AUTO-SAVE
- tasks that change a song's path perform kind of auto-save because the path change cannot be deferred

#### REPORTS
- you cannot sort by a custom tag column in the report
- Unicode symbols (e.g. Chinese/Japanese characters) break fixed column width in text report

#### SONG TREE
- cannot drop external files into song tree anymore :-/

#### COVER DOWNLOAD
- AlbumArtExchange blocked by Blockscript (http://www.blockscript.com/)
- Amazon yields invalid request

#### MENU
- hide/show menu stopped working

#### PLAYLISTS
- there are several playlist-related issues not working correctly


## Changelog (1.8.1 to 1.8.2)

### NEW FEATURES

#### TASKS
- new task to fix line break timings
- new task to set medley tags (experimental?)
- new task to fix overlapping notes

#### REPORTS
- new columns "Medley exists?" and "Golden Notes exist?" for report generation
- new CSV report

#### SONGTREE
- new columns "Medley exists?" and "Golden Notes exist?"
- new action "Lookup on swisscharts" to check artist/title spelling and release year
  (swisscharts.com seems to have a large and complete database and is quite exact with spelling)

### CHANGES

#### QT
- based on Qt 4.8.2 now

#### BASS/BASS FX
- updated to BASS 2.4.9
- updated to BASS FX 2.4.8

#### RIBBON MENU
- replaced last two checkboxes by toolbuttons in View
- simplified language menu (removed translated language string)
- reorganized actions

#### SONGTREE
- tooltip for #START and #END tag now shows start and end time in min:sec

#### REPORTS
- text report now has CR/LF line endings
- HTML and text report are UTF-8 encoded and file names include the current date (just like PDF report)

### BUG FIXES

#### TASKS
- increase low BPM values now also adjusts medley tags, if (both) present
- set first timestamp to zero now also adjusts medley tags, if (both) present
- fixed modifier (negation) handling for song/ID3 tag tasks

#### RIBBON MENU
- fixed automatic setting of medium quality for MP3/cover/background when highest setting was chosen for high quality

#### LOG
- log message about defaultOutputEncoding not being sufficient is now correct

#### SONGTREE
- sorting by score file working
- sorting by relative tag working

#### SONGDETAILS
- suffix vanishes also for values entered using decimal dot instead of decimal comma

### KNOWN BUGS

#### INSTALLER
- Uninstaller does not delete "non-standard" tasks

#### TASKS
- convert syllable placeholders does not ignore "wanted" dashes ("A-B-C")
- "remove empty syllables" breaks timestamps in relative songs
- capitalization task makes "TV" to "Tv" - should ignore words that are entirely upper-case

#### TASK LIST
- tooltip of slot buttons is empty before the first selection of the slot

#### SONG FRIENDS (general workaround: switch friend to primary song using the context menu)
- cannot add song friends to playlist
- cannot change #MP3 for karaoke song friend
- cannot change #EDITION for song friend
- cannot add song friends in reports
- cannot play song friends in media player
- some tasks may not affect song friends (e.g. "Remove unsupported tags")

#### MEDIA PLAYER
- correct songs get ansychronous after a while (seek works correctly)
- uman crashes if you play a song, change the song directory and skip

#### AUTO-SAVE
- tasks that change a song's path perform kind of auto-save because the path change cannot be deferred

#### REPORTS
- you cannot sort by a custom tag column in the report
- Unicode symbols (e.g. Chinese/Japanese characters) break fixed column width in text report

#### SONG TREE
- cannot drop external files into song tree anymore :-/

#### COVER DOWNLOAD
- AlbumArtExchange blocked by Blockscript (http://www.blockscript.com/)
- Amazon yields invalid request

#### MENU
- hide/show menu stopped working

#### PLAYLISTS
- there are several playlist-related issues not working correctly

## Changelog (1.8.0 to 1.8.1)

### NEW FEATURES

#### SONG TREE
- indicate mp3/cover/background quality in song tree, quality settings adjustable in Options
- indicate cover/background resolution problems (covers expected to be 1:1, backgrounds expected to be 16:9)
- do not warn about no background if linked video found and vice versa (either one is enough)
- option to open files internally/externally in context menu, default action (double-click) is bold
- refresh selection of songs via F5

#### SUPPORTED FILE TYPES
- added support for *.ts and *.m4v video files
- removed support for *.gif and *.bmp (not supported by UltraStar)

#### SUPPORTED ENCODINGS
- CP1250, CP1252 (default) and UTF-8 are supported (with or without BOM)
- ENCODING tag is only written if CP1250, CP1252 is default
- default input and output encodings may be changed in Options

#### SONG DETAILS
- if user sets CALCMEDLEY to "OFF", MEDLEYSTARTBEAT and MEDLEYENDBEAT are deleted

#### TRANSLATIONS
- new French translation (thanks to Fred)
- new Spanish translation (thanks to Pantero)
- new Portuguese translation (thanks to zup3r_vock)
- updated Polish translation (thanks to michalek^)

#### TAGS
- new tags supported: ENCODING, PREVIEWSTART, MEDLEYSTARTBEAT, MEDLEYENDBEAT, CALCMEDLEY

#### TASKS:
- new task: write background tag even if file does not exist now works for songs with title ending with period (e.g. Born In The U.S.A.)
- new task: increase (double, quadruple...) BPM and all timings for low BPM values
- new task: normalize note pitches (all notes are shifted octave-wise towards zero mean)
- new task: capitalize first word of each line
- new: allow inversion of conditions with "NOT" for scriptable tasks
- new: task to change song files to "Artist - Title.*" and use "txd" as extension for duets (recommended)
- Rock Band song directories are labeled with "[RB]" in rename tasks
- change: pattern change for task assigning files automatically

#### MONTY:
- Monty knows about the new functions as well ;-)

#### MENU:
- set default input and output encoding for song files in Options
- check for update feature (run once to allow automatic silent check on startup)

#### REPORTS:
- PDF report

### CHANGES

#### QT
- based on Qt 4.7.4 now

#### BASS
- based on BASS 2.4.8

#### DUETS
- duets are recognized by file extension (txd) or by starting with 'P'

#### SONG TREE
- highlight *all- linked files of friends with yellow/orange background
- links to unsupported files are handled like files that cannot be found

#### SONG DETAILS
- default genre list for #GENRE tag is now in alphabetical order
- additional languages in #LANGUAGE tag dropdown field
- new default editions (SingStar, Rock Band titles, ...)

#### MENU
- language menu is now in a separate tab

#### TASKS:
- "Set first timestamp to zero" adjusts medley tags as well
- change of file extensions "jpeg" to "jpg" and "mpeg" to "mpg" on corresponding rename tasks (silently)

#### SONG FRIENDS:
- changes made in one song file are no longer reflected in friends (friends might or might not have resources/tags in common)
- do not blindly take first text file as primary song file, but try to find the simplest non-duet file

### BUG FIXES

#### TASKS
- fixed undesired behaviour for renaming files/folders to names
  that contain slashes ('/', e.g. 'AC/DC')
  - a slash will be replaced by a hyphen
  - other unsupported characters will be removed
- fixed undesired behaviour for changing duet files (P2 appearing in the wrong place at the beginning of the last line of P1)

#### MAIN WINDOW
- fixed saving and restoring window state and size
- fixed column-width for icon-columns (Qt-Bug 19915)

#### MONTY
- Monty now handles 0 songs and 1 song in songDB correctly =)

#### MENU
- fixed bug where song paths were added twice through paths dialog (forward/backward slash problem)

#### SONG FILE PREVIEW
- fixed preview of song file contents (ignored some spaces)

#### LOG FILE
- fixed line endings to CR/LF

#### TRANSLATIONS
- added Qt translations (shortcuts, standard GUI elements, etc.)

### KNOWN BUGS

#### INSTALLER
- Uninstaller does not delete "non-standard" tasks

TASKS
- convert syllable placeholders does not ignore "wanted" dashes ("A-B-C")
- "remove empty syllables" breaks timestamps in relative songs
- capitalization task makes "TV" to "Tv" - should ignore words that are entirely upper-case

TASK LIST
- tooltip of slot buttons is empty before the first selection of the slot

SONG FRIENDS (general workaround: switch friend to primary song with double-click on song text file)
- cannot add song friends to playlist
- cannot change #MP3 for karaoke song friend
- cannot change #EDITION for song friend
- cannot add song friends in reports
- cannot play song friends in media player
- some tasks may not affect song friends (e.g. "Remove unsupported tags")

#### MEDIA PLAYER
- correct songs get ansychronous after a while (seek works right)
- uman crashes if you play a song, change the song directory and skip

AUTO-SAVE
- tasks that change a song's path perform kind of auto-save because the path change cannot be deferred

REPORTS
- you cannot sort by a custom tag column in the report

SONG TREE
- cannot drop external files into song tree anymore :-/

COVER DOWNLOAD
- AlbumArtExchange blocked by blockscript
- Amazon yields invalid request

MENU
- hide/show menu stopped working

PLAYLISTS
- there are several playlist-related issues not working correctly

## Changelog (1.7.1 to 1.8.0)

### NEW FEATURES

MENU
- Office2007-like ribbon bar added
  - reduces mouse click count
  - hideable

TASKS
- convert syllable placeholders (~ > -, - > ~)
- convert absolute to relative timestamps and vice versa
- fix capitalization for #TITLE tag

TASK LIST
- you may save 4 different selections now (flags above the list)
  - should be useful because there are so many tasks right now

#### MEDIA PLAYER
- play a song and watch the lyrics
- support for duet songs present =)
- jump directly to edit mode of the current line via context menu

#### SONGS
- cache any []-tags in the filename at song load
  - e.g. "Wizo - Hund [kar] [foo].txt" caches "kar" and "foo"
  - used for recognition of karaoke songs
  - used for recognition of duet songs (at least one possibility)
  - used for song friends (do avoid naming conflicts)

#### SONG/ID3 TAG TASKS
- new conditions: "isDuet", "isKaraoke", "isSingStar"

#### RENAME TASKS
- new conditions: "isDuet", "isKaraoke", "isSingStar"
- new source: "titlecompact" -> #TITLE without []-tags

#### SONG FRIENDS
- if more than one song is in a folder, the first one will be the primary
  song and the rest will be "songs friends"
  - select primary song via double-click or "open" on the proper song text file
- share primary song's tags except #GAP, #EDITION
  - karaoke songs do not share #MP3
  - song friends append all cached []-tags to #TITLE
- limited access to song friends possible (see KNOWN BUGS)

#### KARAOKE SONGS
- meet one of the following requirements
  - #EDITION contains "karaoke" or "kar"
  - #TITLE contains a []-tag with "karaoke" or "kar"
  - cached []-tags from filename contain "kar" or "karaoke"

#### DUET SONGS
- meet one of the following requirements
  - song filename suffix is "txd"
  - cached []-tags from filename contain one that is not "kar" or "karaoke"
- duet format "*.txd" with "P 1", "P 2", "P 3" in front of each line will
  be parsed correctly

#### SONG TREE
- column for multiple songs displays now the type of each song text file
  - types of song friends are also shown here
- two columns for song type "karaoke" or "duet"
  - also added to context menu of song tree header (-> "Types")

#### SUPPORTED FILE TYPES
- "*.txd" for duet songs added

#### FIND BAD COVERS/BACKGROUND
- picture flow preview for covers and background images to detect bad ones
  and unlink them (-> remove tag)

#### EDIT LYRICS
- modify each syllable of the song
  - fix spelling errors manually
  - fix wrong whitespaces manually
- highlight lines that contain keywords (search function)

#### SMART SETTINGS
- some tasks can be expanded and offer settings to modify
  - e.g. a list of unsupported tags in "Remove unsupported tags"
- easy integration into plugin development

#### COMMAND LINE
- first parameter enables you to specify a song folder to open

#### SONG TREE
- new columns for #RELATIVE, #GAP, #BPM
- context menu added when not clicking a specific item
  - useful to show all songs in the tree again

#### COVER DOWNLOADS
- available as plugins now
- "www.albumartexchange.com" now accessible
  - needs to enter a captcha sometimes (use your internet browser)
- "freecovers.net" now accessible
  - but only compressed covers due to gold membership needed
  - may be down at the moment (8th August 2010)
- BUY-Button removed

### CHANGES

#### QT
- based on Qt 4.5.3 now
- keep on licensing under GPL

#### PLUGINS
- tasks are now plugins (dynamically loaded libraries)
- task description: "Fix spaces at end of line" -> "Fix spaces"

#### SUPPORTED FILE TYPES
- icon for "*.kar" files changed -> these are only MIDI files with lyrics

#### REPORTS
- Shortcut changed from F2 to F8 (F2 is default edit key)

#### FILTER SONGS
- renamed to "Find Songs"
- ui refreshed (more common like the search field in Thunderbird)

#### SONG PATH SELECTION
- new dialog enables the user to specify UltraStar folders and multiple
  song folders (Options -> Paths...)
- you may choose between those preconfigured paths now

#### SONG DETAILS
- read-only fields are skipped when pressing tab key

#### AMAZON COVER DOWNLOAD
- new "Amazon Product Advertising API" needs signed requests
  - signatures become mandatory at 15th August 2009
- "Secret Access Key" of AWS Accounts needed to sign each request
  - NOT shareable -> user needs his own account

#### KEYBOARD SHORTCUTS
- CTRL + [1..6] show/hide dock widgets (details, tasks, media player ...)
- CTRL + F11 toggles full-screen mode
- CTRL + Return opens Explorer
- CTRL + E edit the lyrics of the current song

#### PATHS
- new dialog for managing paths
- multiple song paths can be configured and selected via menu

#### PROGRESS DIALOG
- not so exact remaining time displayed now
  - "about 5 seconds" or "1 minute, 20 seconds"

#### TASKS
- added file tags as targets for "Song/ID3-tag tasks"
- added some song tags as sources for "Song/ID3-tag tasks"

#### PICTURE PREVIEW
- dialog width will be saved and restored for the next picture

### BUG FIXES

#### LIBTAG.DLL
- recompiled version 1.5.0 to suppress false-positives from some
  anti-virus tools

#### TASKS
- "Fix spaces at end of line" crashed if song had empty lines
- "Fix spaces" now trims the whole song line correctly
- I have no clue, why path changing did work before ... =)
- "Remove unused files" works as expected now
- fixed undesired behaviour for renaming files/folders to names
  that contain slashes ('/', e.g. 'AC/DC')
  - slash will be removed
- "Set first timestamp to zero" does not mess with invisible times and
   pause times in relative songs anymore

#### MAIN WINDOW
- "Options -> Always On Top" now saved and restored during app restart

#### SONG TREE
- sorting works now for #START, #END, #VIDEOGAP
- fixed problem were audio and video files did not open correctly with
  the local audio and video player

#### PLAYLISTS
- playlists are now always sync with song database
  - no app crash if a deleted song is referenced in a playlist

#### SONG WATCHER
- recognition of external song file changes removed

#### DUPLICATE DETECTION
- bugfix from escaped added (http://github.com/escaped/uman)

### KNOWN BUGS

#### INSTALLER
- Uninstaller does not delete
  - "non-standard" tasks

#### TRANSLATION
- "Leerzeichen an Zeilenende verschieben" -> "Leerzeichen am Zeilenende korrigieren" or "Leerzeichen vor die Silben verschieben"

#### TASKS
- convert syllable placeholders does not ignore "wanted" dashes ("A-B-C")
- "remove empty syllables" breaks timestamps in relative songs
- capitalization task makes "TV" to "Tv" - should ignore words that are entirely upper-case

#### TASK LIST
- tooltip of slot buttons is empty before the first selection of the slot

#### SONG FRIENDS (general workaround: switch friend to primary song with double-click on song text file)
- cannot add song friends to playlist
- cannot change #MP3 for karaoke song friend
- cannot change #EDITION for song friend
- cannot add song friends in reports
- cannot play song friends in media player
- some tasks may not affect song friends (e.g. "Remove unsupported tags")

#### MEDIA PLAYER
- correct songs get ansychronous after a while (seek works right)
- uman crashes if you play a song, change the song directory and skip

#### AUTO-SAVE
- tasks that change a song's path perform kind of auto-save because
  the path change cannot be deferred

#### REPORTS
- you cannot sort after a custom tag column in the report

#### DUET SONGS
- detection is not perfect with []-tags
  - "Wheatus - A little respect [LIVE]"

#### SONG TREE
- cannot drop external files into song tree anymore :-/

## Changelog (1.7.0 to 1.7.1)

### NEW FEATURES

- new language: Polish

#### TASKS
- set timestamps to 0
- fix spaces in lyrics
- round #GAP to nearest integer
- remove empty syllables

#### SONG TREE
- show more from current artist
- open current folder in file explorer
- "song speed", new column

### CHANGES

#### TASKS

#### SONG TREE

#### SONG DETAILS

#### REPORTS
- save last settings in the report dialog

#### PLAYLIST SUPPORT
- you can add a song to the playlist only once now

#### FILTER SUPPORT

#### PICTURE PREVIEW
- dimensions and file size added to info label

#### AMAZON COVER DOWNLOAD
- added checkbox to delete the current cover before copy & set the new one

#### MONTY
- you can ask Monty something now
  - better help feature, I think

#### INTERNAL STUFF
- support for "*.mkv" video files added
- app crashes will now be detected
  - you can change your song folder on next app start

### BUG FIXES

#### TASKS
- target for "id3-050-useAlbum.xml" was empty - is now "album"
- no APP FREEZE anymore if you run a renaming task and the song title
  contains a '%'

#### SONG TREE

#### SONG DETAILS

#### REPORTS

#### PLAYLIST SUPPORT
- active list will now be updated if you remove a playlist

#### FILTER SUPPORT

#### PICTURE PREVIEW

#### AMAZON COVER DOWNLOAD

INTERNAL STUFF
- song folder scan ignores symlinks now
  - no indefinite loop possible anymore

### KNOWN BUGS

#### NOT SORTED
- you cannot sort after a custom tag column in the report
- Uninstaller does not delete
  - "non-standard" tasks
- short application hang under vista while getting covers from amazon
  the first time after application start
- cannot save "Always On Top" state to config because it's not possible to
  reset that state on application startup (-> Qt 4.3.5 bug?)
- cannot sort after "videogap" column correctly
- broken gif-support for amazon cover files
- update playlists if you delete a song -> crash on exit


## changes from 1.6.0 to 1.7.0
---------------------------
### NOTES
- Still using Qt 4.3.5 because Qt 4.4.1 is terribly slow with QTreeWidget.
- Next release will be 1.7.1 -> not so extensive.

### NEW FEATURES ("in a nutshell")
- custom song tags possible
- configurable, scriptable (and more) tasks
- manage playlists
- get covers from amazon
- new file information dock
  - song database statistics
  - current file information (e.g. some ID3v2 tags of audio file)
  - see all supported file types
- extended configuration possible through registry
  - supported file types
  - custom tags (also with uman)
- more report features added, e.g.:
  - lyrics
  - song length

### CHANGES (more detailed)
- "rename tasks" and "song/ID3 tag tasks" are now scriptable
  - see task-def folder
  - create easily new custom tasks
- context menu for scriptable tasks added
  - edit these tasks by double-click or through the context menu
- first pre-defined value for EDITION in song details: "[SC]-Songs"
- added a cancel button and a elapsed time indicator to progress bar
  - most long operations can be cancelled
- new target for rename tasks: "path"
  - change the complete path of a song
- re-organize folder structure with rename tasks
  - Songs/Artist - Title (default)
  - Songs/Artist/Artist - Title
  - Songs/Edition/Artist - Title
  - Songs/Genre/Title - Artist
- new database info window/dock added
  - displays database statistics, e.g. song count
  - shows ID3 tag information of current audio file
  - ID3 tag status bar information removed
  - shows some information about picture files
  - shows recognized file types (you'll need to expand that node with a
	  double-click)
- new column in song tree which indicates whether there are multiple
  song text files in one folder
- manage song playlists
  - load, save, edit
  - connect playlist entries to songs in the song tree, quickly find all
    playlist changes
  - custom order of songs in playlist (drag & drop)
- splash screen at application startup
  - for more responsiveness
- a cross with a small warning-symbol in the file-related columns
  - indicates a tag-value with points to a non-existent file
  - red cross only for empty file-related tags
- create custom song tags, e.g. #COMMENT
  - use them as sources for rename tasks (the blue entries in the sources combo box)
  - use them as targets for song/id3 tag tasks
  - added as a new filter target
  - contents displayed in the last columns of the song tree
  - can be used for reports
- show song paths in the report
  - for each song in the table
  - at first the path for the current songs directory
- tooltips for (blue) ticks/crosses in the song tree added
  - gives the user more infos about the state of a song
- name of folder is now compared without illegal characters (e.g. '?', ':')
  against "Artist - Title" in the appropriate song tree column
- tooltips for "unused files" and "multiple songs" column added
  - displays the files affected
- Monty talks now in a white speech bubble.
- toolbar splitted into more toolbars
- context menu for song tree header added (right-click)
  - show/hide various columns
  - some presets
  - state of columns saved
- two new columns in the song tree (artist, title)
  - hidden by default
- new shortcut: hit F12 to change the song directory
- "open" added to context menu for files in song tree
  - same function as double-click on a file (or "Enter" pressed)
- classes "odd" and "even" added to HTML report
- first style sheet for HTML report added
- "*.mp4" files now recognized as valid video files
- "*.divx" files now recognized as valid video files
- event logs are now saved on application exit or whenever you want
- "hide" added to context menu for selected songs
  - hide selected
  - hide only selected (discards a previous filter)
  - hide all but selected
- 4 new columns in the song tree for time comparisons
  - lyrics time
  - audio file time
  - total time
  - warning indicator
- you can display the (readable) lyrics of a song now
  - Ctrl+L
  - context menu in song tree
  - "Songs > Show Lyrics..." menu
- you can get your song covers from amazon now
- new icon for "unused files found"
- alternative song tree view added (see View menu)
  - other icons for spell- / file-check columns
- progressbar for dropping song files in song tree
- filter duplicate songs
- new task: setting #end to song length
- new task: removing end tag
- show lengths of selected songs in preview tree
- two new tasks based on custom tags (will be installed by default)
  - use the ID3 tag of the audio file for #ALBUM
  - change song path to "artist/album/artist - title"
  - you need the custom tag #ALBUM for that!
- you can delete whole song directories now
  - useful for eleminate duplicate songs
- your can merge multiple songs now
  - copy all files into one folder
- invalid songs get a light red background now
  - no artist
  - no title
  - no song length -> no lyrics
- the setup creates two custom tags: #COMMENT, #ALBUM
- new entries in context menu for a folder
  - delete a whole song
  - merge two songs (copy the files to one point)
- extended reports
  - use all, visible or playlist songs
  - append song lyrics
  - link songs to lyrics in HTML report
- selected lengths are shown in "file information" dock
- external changes in the song file will be noticed
  - internal information are updated to new data
  - cannot update if you still have unsaved changes
- new supported file types
  - midi files (*.mid *.midi)
  - karaoke files (*.kar)
  - license files (license.txt license.htm license.html)
- new about dialog
  - take a look at (Help -> About)
- "Buy" button added in amazon cover search :)
- "Always On Top" added to options menu
- supported file extensions are available through the registry now

### BUG FIXES
- minor change in object renaming
  - "oldName" now renamed to "_oldName" then to "newName"
  - try to switch CO and BG, then rename the files with tasks, you'll see... :)
- you can now sort after the columns with ticks and crosses
  - e.g. a cross is "smaller" than a tick
- fixed sorting for "unused files" column
- fixed an application crash where Qt found a file with a trailing dot by ignoring
  that dot and where TagLib did not found that file which produced a null pointer
  exception
- inactive song text files in a folder (*.txt) are greyed out only now
  - double-click on them to make them active (as before)
- tags which are not upper-case (e.g. "#artist") are now recognized correctly
  - will be saved upper-case
- artist/title detection works now correctly for more than one hyphen
  - "Farin Urlaub - Unter - Wasser" :) (regard the blanks around the hyphen!)
  - the first occurence of " - " splits the artist from the title now
- lyrics do not mess up anymore, if song text file contains lines with preceding whitespaces
- freshly dropped songs into the song tree can now send (save) messages to event log
  - you needed to rebuilt the whole tree up to now

### KNOWN BUGS
- you cannot sort after a custom tag column in the report
- Uninstaller does not delete
  - "non-standard" tasks
- short application hang under vista while getting covers from amazon
  the first time after application start
- cannot save "Always On Top" state to config because it's not possible to
  reset that state on application startup (-> Qt 4.3.5 bug?)
- cannot sort after "videogap" column correctly
- broken gif-support for amazon cover files
- target for "id3-050-useAlbum.xml" is empty - should be "album"
- APP FREEZE if you run a renaming task and the song title contains a '%'
- song folder scan does not ignore links to other folder
  - indefinite loop possible
- active list will not be updated if you remove a playlist

## changes from 1.5.1 to 1.6.0
### NEW FEATURES
- new editable tags: VIDEOGAP, START, END
- drag&drop a file from die explorer into the song tree to
  1. copy that file to the song folder
  2. use that file according to its type (discard previous tags if used)
  - if you try to drop more than 5 files you will be asked to confirm.
- new (inofficial) tag: COMMENT
- new progress dialogs for long operations
- it is now possible to refresh only the selected songs instead of the whole song tree
  - could be necessary if you e.g. modified the song dir outside of the application
- context menu in song tree added
  - refresh selected songs
  - delete current file (shortcut: "del")
- song text files can be dropped into the song tree (not on items) to create a new folder and
  copy the file into that folder
- new column: "unused files found"
- it is now possible to disable "save-on-change"
  - you will have to save the song files yourself then
- toolbar added
- filter for the song tree added
- extended selection in "edit tag save order" used
  - now you can drag&drop multiple tags at once
- double-click on an unused txt-file uses that file for the song in the folder
  - useful if you keep several valid song text files in one folder (NOT RECOMMENDED!)
- html & plain text reports added
  - F2 or "Songs > New Report"
- "n/a" replaced with "-"
- new messagebox style
- application language "german" added
  - Options > Language > German
  - Monty talks german! (Monty spricht deutsch! :)
- append an 'E' after end of lyrics to the song file if there is none
- keep all other stuff after the song end mark 'E'
  - all empty lines will be removed from the song text file
- new task: "delete unused files" (clean-up task)
  - without undo!
- new task: "clear invalid file-related tags" from song (clean-up task)
  - e.g. clear the song tag "#COVER:blubb.jpg" if the cover file "blubb.jpg" is NOT present
  - without undo!
- simple picture preview added
  - double-click on a picture file

### BUG FIXES
- details do not show "n/a seconds" anymore, but only "n/a"
- selected songs will not be unselected after applying a task anymore
- list now all file-types in a subfolder to determine unnecessary files
  - gray + striked-out = useless :)
- progress dialog also shown while renaming lots of details ("mass-tagging")
- song lyrics not corrupted anymore if started with a pause/break ("-")

## changes from 1.5.0 to 1.5.1
### BUG FIXES
- does not corrupt song files anymore if first note is not ":" (e.g. *, F)

## changes from 1.4 to 1.5.0
- UI improved a little bit
  - e.g. column header text removed from tick/cross columns

### NEW FEATURES
- show current song folder in header of first column in song tree
- detail editor with auto-completion and default values added
  - e.g. list all audio files for the mp3 tag
- show progress bar whenever tasks are applied to more than 10 songs concurrently
- some event log messages revised (more details)
- default values for "genre" added
  - http://www.id3.org/id3v2-00
- new tags introduced: END, CREATOR
- default tag save order changed due to compatibility
- tag order in details changed to default tag save order :)
- new task: use the ID3 tag to retrieve the year
- song details: default value for artist, title, year are from ID3 tag if present
- most spoken languages added as default values for LANGUAGE tag
- more columns in song tree to enable simple tag sorting (language, edition, ...)
- you can edit some details for multiple selected songs now
- tasks grouped
- dockable widgets for details, tasks and event log added
- application window state saved on exit
- possibility added to reset the tag order to its default value in the options menu
- a double-click on a txt-file opens its contents (read-only)
- task modified: if you rename your song folder with [SC] and [VIDEO] tags, other present tags will be preserved and appended to the end of the folder name
  - present (unknown) tags will be sorted

### BUG FIXES
- case-sensitive renaming works now under Windows ("abba" -> "ABBA")
- cannot select logically exclusive tasks anymore
- tasks work for the current song now although no song item is "selected"
- switch for case-sensitive auto completion in details to avoid renaming issues ("abba" -> "ABBA")
- modified task: if no #VIDEOGAP present the video file is renamed like #VIDEOGAP:0

## changes from 1.3 to 1.4
- increased performance for the song tree
### NEW FEATURES
- more song details listed (but not all editable for security reasons)
- new task: rename a directory to "Artist - Title [VIDEO] [SC]" if it has a video or it is "song-checked"
  - display tick and cross correctly for that folder name scheme
- new task: rename video file to something like "Artist - Title [VD#5].flv" where the VIDEOGAP tag is considered
- more tooltips added
- more file types recognized for
  - AUDIO: ogg, mp3
  - VIDEO: mpg, mpeg, avi, flv
  - PICTURE: jpg, png, bmp
- more tags editable: EDITION, GENRE, LANGUAGE
- refresh the song tree (re-inspect the song-folder for song files) with F5 or "Songs->Refresh"
- nice icons for event log messages added to recognize warnings easier
- new tasks for ID3 tag added: use ID3 tag for artist, title or genre now (artist and title splitted)
- all songs in every subdirectory of the song folder is now recognized correctly
  - see "Options" Menu to switch the relative path
- UltraStar song directory can now be changed in the "Options" menu
  - you will not be asked everytime you start this application
- introduced blue folder icons for song files that have tags not used by ultrastar
  - tooltips of blue folder icons display all unsupported tags that where found
- artist, title, genre is read from ID3 tag and shown in the statusbar when an audio file is selected
- YEAR tag now editable due to support in UltraStar DX 1.1 (Alpha)
- new task: look for the right files in the song directory automatically and use these files in the song file if needed

### BUG FIXES
- file endings/suffixes preserved while renameing them
- tags that are not recognized will not be dropped anymore when saveing the song text file
- ID3 tag is not used anymore whenever the wanted information is not present

## changes from 1.2 to 1.3
### NEW FEATURES
- progressbar added when song files are read
- reads ID3 tags from mp3 files and uses artist/title
- Monty the Mammoth added
- visible switches for monty, tasks and event log added
- you can modify the order of the tags in the song file now
### BUG FIXES
- link was not set through double click if the file extension was correct but not lowercase (MPG > mpg)
