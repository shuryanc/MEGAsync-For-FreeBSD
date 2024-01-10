import QtQuick 2.15
import QtQuick.Layouts 1.15

import common 1.0

import onboard.syncs_types 1.0

import BackupsModel 1.0

SyncsPage {
    id: root

    footerButtons.rightPrimary {
        text: BackupsStrings.backUp
        icons.source: Images.database
        enabled: backupsModelAccess.checkAllState !== Qt.Unchecked
    }

    ColumnLayout {
        id: selectFolderLayout

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        spacing: 24

        Header {
            id: headerItem

            title: BackupsStrings.selectBackupFoldersTitle
            description: BackupsStrings.selectBackupFoldersDescription
        }

        InfoAccount {
            id: infoAccount

            Layout.preferredWidth: parent.width
        }
    }

    SelectTable {
        id: backupsTable

        anchors {
            top: selectFolderLayout.bottom
            left: parent.left
            right: parent.right
            bottom: footerButtons.top
            topMargin: 8 // value by design
            bottomMargin: selectFolderLayout.spacing
        }
    }
}
