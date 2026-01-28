  TEMPLATE = subdirs
 
  SUBDIRS = \
            src/plugins/audiotag \
            src/plugins/cleanup \
            src/plugins/lyric \
            src/plugins/preparatory \
            src/plugins/rename \
            src

  src.file = src/UltraStar-Manager.pro

win32 {
  nsis.CONFIG += phony
  nsis.CONFIG += recursive
  nsis.recurse = src
  nsis.recurse_target = nsis
  nsis.depends = all
  QMAKE_EXTRA_TARGETS += nsis
}
macx {
  dmg.CONFIG += phony
  dmg.CONFIG += recursive
  dmg.recurse = src
  dmg.recurse_target = dmg
  dmg.depends = all
  QMAKE_EXTRA_TARGETS += dmg
}
