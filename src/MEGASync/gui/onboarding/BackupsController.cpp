#include "BackupsController.h"

BackupsController::BackupsController(QObject *parent)
    : QObject(parent)
    , mBackupController(new SyncController())
{
    connect(mBackupController, &SyncController::syncAddStatus, this, &BackupsController::onBackupAddRequestStatus);
}

QSet<QString> BackupsController::getRemoteFolders() const
{
    return SyncInfo::getRemoteBackupFolderNames();
}

void BackupsController::addBackups(const BackupInfoList& backupsInfoList)
{
    if(backupsInfoList.size() <= 0)
    {
        return;
    }

    mBackupsProcessedWithError = 0;
    mBackupsToDoSize = backupsInfoList.size();
    mBackupsToDoList = backupsInfoList;
    mBackupController->addBackup(mBackupsToDoList.first().first, mBackupsToDoList.first().second);
}

void BackupsController::onBackupAddRequestStatus(int errorCode,
                                          const QString& errorMsg,
                                          const QString& name)
{
    Q_UNUSED(errorMsg)

    bool found = false;
    auto it = mBackupsToDoList.constBegin();
    while(!found && it != mBackupsToDoList.constEnd())
    {
        found = (it->first == name);
        it++;
    }

    if(!found)
    {
        return;
    }

    if(errorCode == mega::MegaError::API_OK)
    {
        emit backupFinished(mBackupsToDoList.first().first, true);
    }
    else
    {
        emit backupFinished(mBackupsToDoList.first().first, false);
        mBackupsProcessedWithError++;
    }

    mBackupsToDoList.removeFirst();
    if(mBackupsToDoList.size() > 0)
    {
        mBackupController->addBackup(mBackupsToDoList.first().first, mBackupsToDoList.first().second);
    }

    if(mBackupsToDoList.size() == 0)
    {
        QString message(QString::fromUtf8(""));
        if(errorCode != mega::MegaError::API_OK)
        {
            if(mBackupsProcessedWithError == 1)
            {
                message = tr("Folder wasn't backed up. Try again.");
            }
            else
            {
                message = tr("These folders weren't backed up. Try again.");
            }
        }

        emit backupsCreationFinished(mBackupsProcessedWithError == 0, message);
    }
}
