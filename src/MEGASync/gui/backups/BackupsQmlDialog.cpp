#include "BackupsQmlDialog.h"

#include "SettingsDialog.h"
#include "DialogOpener.h"

#include <QEvent>

bool BackupsQmlDialog::event(QEvent* event)
{
    if(event->type() == QEvent::Close)
    {
        if(auto dialog = DialogOpener::findDialog<SettingsDialog>())
        {
            dialog->getDialog()->setSyncAddButtonEnabled(true, SettingsDialog::Tabs::BACKUP_TAB);
        }
    }
    else if (event->type() == QEvent::Show)
    {
        if(auto dialog = DialogOpener::findDialog<SettingsDialog>())
        {
            dialog->getDialog()->setSyncAddButtonEnabled(false, SettingsDialog::Tabs::BACKUP_TAB);
        }
    }

    return QmlDialog::event(event);
}
