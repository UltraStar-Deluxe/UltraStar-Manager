#ifndef QKFILEWATCHER_H
#define QKFILEWATCHER_H

//#include <qkGlobalHeader.h>

// Qt includes
#include <QObject>
#include <QDateTime>

class qkFileWatcherPrivate;

class qkFileWatcher;

class QCORE_EXPORT qkFileWatch
{
public: // Enums
	enum FileType
	{
		invalidType = 0,
		fileType,
		folderType
	};

public: // Ctor
	qkFileWatch(qkFileWatcher * watcher);
	qkFileWatch(const QString & path, qkFileWatcher * watcher);
	qkFileWatch(FileType type, const QString & path, qkFileWatcher * watcher);
	virtual ~qkFileWatch() {}

public: // Interface
	virtual void checkChange();
	void refreshDate();

protected: // Functions
	void request_fileModified(const QString parentPath, const QString fileName);
	void request_filesCreated(const QString parentPath, const QStringList fileNames);
	void request_filesDeleted(const QString parentPath, const QStringList fileNames);

	void request_folderModified(const QString parentPath, const QString folderName);
	void request_foldersCreated(const QString parentPath, const QStringList folderNames);
	void request_foldersDeleted(const QString parentPath, const QStringList folderNames);

protected: // Variables
	QString mPath;
	QString mAbsolutePath;
	QString mName;

	FileType mType;

	QDateTime mLastModified;

	qint64 mSize;

	qkFileWatcher * mWatcher;

public: // Properties
	QString path() const;
	QString absolutePath() const;
	QString name() const;

	bool isValid() const;

	bool isFile() const;
	bool isDirectory() const;
	bool exists() const;

	FileType type() const;
};

class QCORE_EXPORT qkFolderWatch : public qkFileWatch
{
public: // Ctor
	qkFolderWatch(const QString & path, qkFileWatcher * watcher);

public: // qkFileWatch interface reimplementation
	bool contains(const QString & path);
	virtual void checkChange();

private: // Functions
	void recurseDirectories();
	void checkDeleted();

	int getFolderIndex_from_path(const QString & path);
	int getFileIndex_from_path(const QString & path);

private: // Variables
	QList<qkFileWatch> mFileWatchs;
	QList<qkFolderWatch> mFolderWatchs;
};

class QCORE_EXPORT qkFileWatcher : public QObject, public qkPrivatable
{
	Q_OBJECT
	friend class qkFileWatch;
	friend class qkFolderWatch;
private:
	Q_DECLARE_PRIVATE(qkFileWatcher);

public:
	qkFileWatcher(QObject * parent = 0);
	virtual ~qkFileWatcher();

public: // Interface
	void addPath(const QString & path);
	void removePath(const QString & path);

	bool contains(const QString & path);

private slots:
	void onCheckForChange();

private: // Functions
	void request_folderModified(const QString parentPath, const QString folderName);
	void request_foldersCreated(const QString parentPath, const QStringList folderNames);
	void request_foldersDeleted(const QString parentPath, const QStringList folderNames);

	void request_fileModified(const QString parentPath, const QString fileName);
	void request_filesCreated(const QString parentPath, const QStringList fileNames);
	void request_filesDeleted(const QString parentPath, const QStringList fileNames);

signals:
	// Folder
	void folderModified(const QString parentPath, const QString folderName);
	void foldersCreated(const QString parentPath, const QStringList folderNames);
	void foldersDeleted(const QString parentPath, const QStringList folderNames);

	// File
	void fileModified(const QString parentPath, const QString fileName);
	void filesCreated(const QString parentPath, const QStringList fileNames);
	void filesDeleted(const QString parentPath, const QStringList fileNames);
};

#endif // QKFILEWATCHER_H
