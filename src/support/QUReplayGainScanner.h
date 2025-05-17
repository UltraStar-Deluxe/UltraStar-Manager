#ifndef QUREPLAYGAINSCANNER_H_
#define QUREPLAYGAINSCANNER_H_

#include <ebur128.h>
#include <QThread>
#include <QList>
#include <QAudioDecoder>
#include <QEventLoop>

class QAudioFormat;
class QUReplayGainFile: public QObject {
    Q_OBJECT
public:
    QUReplayGainFile(const QString &path, QObject *parent = nullptr);
    ~QUReplayGainFile();
    bool scan();
    bool tag();

public slots:
    void readBuffer();
    void decodingError(QAudioDecoder::Error error);
    void decodingFinished() { _loop.quit(); }
    void cancel();

private:
    QString _path;
    QAudioDecoder _decoder;
    ebur128_state *_ebur128_state;
    QEventLoop _loop;
    double _loudness;
    double _peak;
    bool _error;
    bool _cancelled;

    bool initializeEbur128(const QAudioFormat &format);
    void setDecodingError();
};

class QUSongItem;
class QUSongFile;
class QUReplayGainScanner: public QThread {
    Q_OBJECT
public:
    QUReplayGainScanner(const QList<QUSongItem*> &selectedItems, QObject *parent = nullptr);
    ~QUReplayGainScanner() override = default;

public slots:
    void cancel();

signals:
    void userCancelled();
    void updateProgress(const QString &itemText, bool forceUpdate = false);
    void updateItem(QUSongFile *item);
    void scanFinished(int r);

protected:
    void run() override;

private:
    QList<QUSongItem*> _selectedItems;
    QAudioDecoder _decoder;
    bool _cancelled;
};

#endif
