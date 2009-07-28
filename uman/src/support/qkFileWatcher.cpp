#include "qkFileWatcher.h"

// Qt includes
#include <qtimer>

// qk includes
#include <qkFileController.h>

//=============================================================================
// qkFileWatch
//=============================================================================

qkFileWatch::qkFileWatch(FileType type, const QString & path, qkFileWatcher * watcher)
{
	QFileInfo info(path);

	mWatcher = watcher;

	mPath = path;
	mAbsolutePath = info.absolutePath();
	mName = info.fileName();
	mType = type;
	mSize = -1;

	refreshDate();
}

qkFileWatch::qkFileWatch(const QString & path, qkFileWatcher * watcher)
{
	QFileInfo info(path);

	mWatcher = watcher;

	mPath = path;
	mAbsolutePath = info.absolutePath();
	mName = info.fileName();
	mType = fileType;
	mSize = -1;

	refreshDate();
}

qkFileWatch::qkFileWatch(qkFileWatcher * watcher)
{
	mWatcher = watcher;

	mType = invalidType;
	mSize = -1;

	refreshDate();
}

//=============================================================================

void qkFileWatch::request_fileModified(const QString parentPath, const QString fileName)
{
	mWatcher->request_fileModified(parentPath, fileName);
}

void qkFileWatch::request_filesCreated(const QString parentPath, const QStringList fileNames)
{
	mWatcher->request_filesCreated(parentPath, fileNames);
}

void qkFileWatch::request_filesDeleted(const QString parentPath, const QStringList fileNames)
{
	mWatcher->request_filesDeleted(parentPath, fileNames);
}

//=============================================================================

void qkFileWatch::request_folderModified(const QString parentPath, const QString folderName)
{
	mWatcher->request_folderModified(parentPath, folderName);
}

void qkFileWatch::request_foldersCreated(const QString parentPath, const QStringList folderNames)
{
	mWatcher->request_foldersCreated(parentPath, folderNames);
}

void qkFileWatch::request_foldersDeleted(const QString parentPath, const QStringList folderNames)
{
	mWatcher->request_foldersDeleted(parentPath, folderNames);
}

//=============================================================================

void qkFileWatch::refreshDate()
{
	QFileInfo info(mPath);
	if (info.exists() == false)
	{
//		qkDebug("Deleted %s !", mPath.C_STR);

		mSize = -1;
		mLastModified = QDateTime();

		return;
	}

	mSize = info.size();
	mLastModified = info.lastModified();
}

/* virtual */ void qkFileWatch::checkChange()
{
	QDateTime oldModified = mLastModified;
	qint64 oldSize = mSize;

	refreshDate();

	// We check both modified date and size for greater precision
	if (oldModified < mLastModified || oldSize != mSize)
	{
		if (mType == qkFileWatch::folderType)
		{
			request_folderModified(mAbsolutePath, mName);
		}
		else if (mType == qkFileWatch::fileType)
		{
			request_fileModified(mAbsolutePath, mName);
		}
	}
}

//=============================================================================

QString qkFileWatch::path() const { return mPath; }

QString qkFileWatch::absolutePath() const { return mAbsolutePath; }

QString qkFileWatch::name() const { return mName; }

bool qkFileWatch::isValid() const
{
	if (mType == invalidType) return false;
	else                      return true;
}

bool qkFileWatch::isFile() const
{
	if (mType == fileType) return true;
	else                   return false;
}

bool qkFileWatch::isDirectory() const
{
	if (mType == folderType) return true;
	else                     return false;
}

bool qkFileWatch::exists() const
{
	if (mSize == -1) return false;
	else             return true;
}

qkFileWatch::FileType qkFileWatch::type() const
{
	return mType;
}

//=============================================================================
// qkFolderWatch
//=============================================================================

qkFolderWatch::qkFolderWatch(const QString & path, qkFileWatcher * watcher)
	: qkFileWatch(folderType, path, watcher)
{
	recurseDirectories();
}

//=============================================================================

bool qkFolderWatch::contains(const QString & path)
{
	for (int i = 0; i < mFolderWatchs.size(); i++)
	{
		if (mFolderWatchs[i].path() == path) return true;
	}

	for (int i = 0; i < mFileWatchs.size(); i++)
	{
		if (mFileWatchs[i].path() == path) return true;
	}

	return false;
}

void qkFolderWatch::checkChange()
{
	QDateTime oldModified = mLastModified;

	qkFileWatch::checkChange();

	// Checking all sub-directories
	for (int i = 0; i < mFolderWatchs.size(); i++)
	{
		mFolderWatchs[i].checkChange();
	}

	// Note: on windows we have to check files even if the directory has not changed
	for (int i = 0; i < mFileWatchs.size(); i++)
	{
		mFileWatchs[i].checkChange();
	}

	// Checking files only if the directory has been modified
	if (oldModified != mLastModified)
	{
		// Checking for deleted files
		checkDeleted();

		// Recurse for new files
		recurseDirectories();
	}
}

//=============================================================================

void qkFolderWatch::recurseDirectories()
{
	QDir dir(mPath);
	QFileInfoList list = dir.entryInfoList();
	QStringList newFolders;
	QStringList newFiles;

	foreach (QFileInfo info, list)
	{
		if (contains(info.filePath())) continue;
		if (info.fileName() == ".." || info.fileName() == ".") continue;
		if (info.isHidden()) continue;

		if (info.isDir())
		{
			qkFolderWatch folder(info.filePath(), mWatcher);
			mFolderWatchs.push_back(folder);

			newFolders += info.fileName();
		}
		else if (info.isFile())
		{
			qkFileWatch file(info.filePath(), mWatcher);
			mFileWatchs.push_back(file);

			newFiles += info.fileName();
		}
	}

	if (newFolders.size())
	{
		request_foldersCreated(mPath, newFolders);
	}

	if (newFiles.size())
	{
		request_filesCreated(mPath, newFiles);
	}
}

void qkFolderWatch::checkDeleted()
{
	QStringList deletedFolders;
	QStringList deletedFiles;

	for (int i = 0; i < mFolderWatchs.size(); i++)
	{
		if (mFolderWatchs[i].exists() == false)
		{
			deletedFolders += mFolderWatchs[i].name();

			mFolderWatchs.removeAt(i);
			i = -1;
		}
	}

	for (int i = 0; i < mFileWatchs.size(); i++)
	{
		if (mFileWatchs[i].exists() == false)
		{
			deletedFiles += mFileWatchs[i].name();

			mFileWatchs.removeAt(i);
			i = -1;
		}
	}

	if (deletedFolders.size())
	{
		request_foldersDeleted(mPath, deletedFolders);
	}

	if (deletedFiles.size())
	{
		request_filesDeleted(mPath, deletedFiles);
	}
}

//=============================================================================

int qkFolderWatch::getFolderIndex_from_path(const QString & path)
{
	for (int i = 0; i < mFolderWatchs.size(); i++)
	{
		if (mFolderWatchs[i].path() == path) return i;
	}
	return -1;
}

int qkFolderWatch::getFileIndex_from_path(const QString & path)
{
	for (int i = 0; i < mFileWatchs.size(); i++)
	{
		if (mFileWatchs[i].path() == path) return i;
	}
	return -1;
}

//=============================================================================
// Private
//=============================================================================

#include <qkGlobalHeader_p.h>

class qkFileWatcherPrivate : public qkPrivate
{
protected:
	QK_DECLARE_PUBLIC(qkFileWatcher);

public:
	qkFileWatcherPrivate(qkFileWatcher * p);
	void init();

private: // Functions
	void addPath(const QString & path);
	void removePath(const QString & path);

	int getFolderIndex_from_path(const QString & path);
	int getFileIndex_from_path(const QString & path);

private: // Variables
	QTimer timer;

	QList<qkFileWatch> fileWatchs;
	QList<qkFolderWatch> folderWatchs;
};

qkFileWatcherPrivate::qkFileWatcherPrivate(qkFileWatcher * p)
	: qkPrivate(p)
{
}

void qkFileWatcherPrivate::init()
{
	Q_Q(qkFileWatcher);

	QObject::connect(&timer, SIGNAL(timeout()), q, SLOT(onCheckForChange()));

	timer.start(100);
}

//=============================================================================

void qkFileWatcherPrivate::addPath(const QString & path)
{
	Q_Q(qkFileWatcher);

	QFileInfo info(path);

	if (info.isDir())
	{
		qkFolderWatch folder(path, q);
		folderWatchs.push_back(folder);
	}
	else
	{
		qkFileWatch file(path, q);
		fileWatchs.push_back(file);
	}
}

void qkFileWatcherPrivate::removePath(const QString & path)
{
	int index = getFolderIndex_from_path(path);
	if (index != -1) folderWatchs.removeAt(index);

	index = getFileIndex_from_path(path);
	if (index != -1) fileWatchs.removeAt(index);
}

//=============================================================================

int qkFileWatcherPrivate::getFolderIndex_from_path(const QString & path)
{
	for (int i = 0; i < folderWatchs.size(); i++)
	{
		if (folderWatchs[i].path() == path) return i;
	}
	return -1;
}

int qkFileWatcherPrivate::getFileIndex_from_path(const QString & path)
{
	for (int i = 0; i < fileWatchs.size(); i++)
	{
		if (fileWatchs[i].path() == path) return i;
	}
	return -1;
}

//=============================================================================
// Ctor / dtor
//=============================================================================

qkFileWatcher::qkFileWatcher(QObject * parent)
	: QObject(parent), qkPrivatable(new qkFileWatcherPrivate(this))
{
	Q_D(qkFileWatcher);
	d->init();
}

qkFileWatcher::~qkFileWatcher()
{
}

//=============================================================================
// Interface
//=============================================================================

void qkFileWatcher::addPath(const QString & path)
{
	Q_D(qkFileWatcher);

	if (contains(path)) return;

	d->addPath(path);
}

void qkFileWatcher::removePath(const QString & path)
{
	Q_D(qkFileWatcher);

	if (contains(path) == false) return;

	d->removePath(path);
}

bool qkFileWatcher::contains(const QString & path)
{
	Q_D(qkFileWatcher);

	for (int i = 0; i < d->folderWatchs.size(); i++)
	{
		if (d->folderWatchs[i].path() == path) return true;
	}

	for (int i = 0; i < d->fileWatchs.size(); i++)
	{
		if (d->fileWatchs[i].path() == path) return true;
	}

	return false;
}

//=============================================================================
// Private slots
//=============================================================================

void qkFileWatcher::onCheckForChange()
{
	Q_D(qkFileWatcher);

	QFileInfo info;

	for (int i = 0; i < d->fileWatchs.size(); i++)
	{
		d->fileWatchs[i].checkChange();

		if (d->fileWatchs[i].exists() == false)
		{
			request_filesDeleted(d->fileWatchs[i].absolutePath(),
								 QStringList() << d->fileWatchs[i].name());

			d->fileWatchs.removeAt(i);
			i = -1;
		}
	}

	for (int i = 0; i < d->folderWatchs.size(); i++)
	{
		d->folderWatchs[i].checkChange();

		if (d->folderWatchs[i].exists() == false)
		{
			request_foldersDeleted(d->folderWatchs[i].absolutePath(),
								   QStringList() << d->folderWatchs[i].name());

			d->folderWatchs.removeAt(i);
			i = -1;
		}

	}
}

//=============================================================================
// Private functions
//=============================================================================

void qkFileWatcher::request_folderModified(const QString parentPath, const QString folderName)
{
//	qkDebug("Folder modified %s %s", parentPath.C_STR, folderName.C_STR);

	emit folderModified(parentPath, folderName);
}

void qkFileWatcher::request_foldersCreated(const QString parentPath, const QStringList folderNames)
{
//	foreach (QString name, folderNames)
//	{
//		qkDebug("Folder created %s %s", parentPath.C_STR, name.C_STR);
//	}

	emit foldersCreated(parentPath, folderNames);
}

void qkFileWatcher::request_foldersDeleted(const QString parentPath, const QStringList folderNames)
{
//	foreach (QString name, folderNames)
//	{
//		qkDebug("Folder deleted %s %s", parentPath.C_STR, name.C_STR);
//	}

	emit foldersDeleted(parentPath, folderNames);
}

//=============================================================================

void qkFileWatcher::request_fileModified(const QString parentPath, const QString fileName)
{
//	qkDebug("File modified %s %s", parentPath.C_STR, fileName.C_STR);

	emit fileModified(parentPath, fileName);
}

void qkFileWatcher::request_filesCreated(const QString parentPath, const QStringList fileNames)
{
//	foreach (QString name, fileNames)
//	{
//		qkDebug("File created %s %s", parentPath.C_STR, name.C_STR);
//	}

	emit filesCreated(parentPath, fileNames);
}

void qkFileWatcher::request_filesDeleted(const QString parentPath, const QStringList fileNames)
{
//	foreach (QString name, fileNames)
//	{
//		qkDebug("File deleted %s %s", parentPath.C_STR, name.C_STR);
//	}

	emit filesDeleted(parentPath, fileNames);
}
