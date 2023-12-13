import QtQuick 2.15

QtObject {
    id: root

    enum Types {
        None = 0,
        Sync = 1,
        SelectiveSync = 2,
        FullSync = 3,
        Backup = 4
    }

    property int type: SyncsType.Sync

}
