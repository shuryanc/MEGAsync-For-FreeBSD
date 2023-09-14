#ifndef DOLPHIN_FILEMANAGER_H
#define DOLPHIN_FILEMANAGER_H

#include <string>

#include "ISystemApplicationManager.h"
#include "SystemApplicationManagerFactory.h"

class DolphinFileManager : public ISystemApplicationManager
{
public:
    QString getShowInFolderParams() const override;

private:
    static bool registered;
};

#endif
