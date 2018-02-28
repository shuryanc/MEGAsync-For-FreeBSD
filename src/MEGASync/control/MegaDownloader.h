#ifndef MEGADOWNLOADER_H
#define MEGADOWNLOADER_H

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QQueue>
#include <QMap>
#include "megaapi.h"

class MegaDownloader : public QObject
{
    Q_OBJECT

public:
    // If you want to manage public transfers in a different MegaApi object,
    // provide megaApiGuest
    MegaDownloader(mega::MegaApi *megaApi, mega::MegaApi *megaApiGuest = NULL);
    virtual ~MegaDownloader();
    void processDownloadQueue(QQueue<mega::MegaNode *> *downloadQueue, QString path, unsigned long long appDataId = 0);
    void download(mega::MegaNode *parent, QString path, unsigned long long appDataId);

protected:
    void download(mega::MegaNode *parent, QFileInfo info, unsigned long long appDataId);

    mega::MegaApi *megaApi;
    mega::MegaApi *megaApiGuest;
    QMap<mega::MegaHandle, QString> pathMap;

signals:
    void finishedTransfers(unsigned long long appDataId);
};

#endif // MEGADOWNLOADER_H
