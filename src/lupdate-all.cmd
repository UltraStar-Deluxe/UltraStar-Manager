lupdate uman.pro

cd plugins/audiotag
python getTaskText.py
cd ../rename
python getTaskText.py
cd ..

lupdate audiotag/audiotag.pro
lupdate cleanup/cleanup.pro
lupdate lyric/lyric.pro
lupdate preparatory/preparatory.pro
lupdate rename/rename.pro

lupdate freecovers/freecovers.pro
lupdate albumartex/albumartex.pro
lupdate amazon/amazon.pro

cd ..
